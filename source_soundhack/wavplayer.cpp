#include "wavplayer.h"
#include "nsmb.h"
#include <cstdlib>

extern "C" void AS_StereoDesinterleave(void *input, void *outputL, void *outputR, u32 samples);

const int bpm = 123; //Beats per min
const int spm = 32000 * 60; //Samples per min
const int spb = (spm + bpm / 2) / bpm; //Samples per beat


static void error(const char* c)
{
//	fprintf(stderr, "%s\n", c);
//	exit(1);
	nocashPrint(c);
	while(1);
}

void AS_SetTimer(int freq)
{
    if(freq) {
        TIMER2_DATA = 0x10000 - (0x1000000 / freq) * 2;
        TIMER2_CR = TIMER_ENABLE | TIMER_DIV_1;
        TIMER3_DATA = 0;
        TIMER3_CR = TIMER_ENABLE | TIMER_CASCADE | TIMER_DIV_1;
    } else {
        TIMER2_DATA = 0;
        TIMER2_CR = 0;
        TIMER3_DATA = 0;
        TIMER3_CR = 0;
    }
}

void wavPlayer::seek(int pos)
{
	if(pos < 0) error("Negative seek!");

	FS_SeekFile(fileLeft, pos*bitdepth, 0);
	FS_SeekFile(fileRight, pos*bitdepth, 0);

	fileCursor = pos;
}

void wavPlayer::update()
{
	if(fileCursor >= loopEnd)
		seek(loopBegin);
	
	if(buffercursor >= audiobuffer_size)
		buffercursor = 0;

	//Find out how many new samples should we push to the buffer.
	int curtimer = TIMER3_DATA;	
	int newdata = curtimer - prevtimer;
	if(newdata < 0) newdata += 65536; //Timer wraps around at 2^16
	
	if(newdata > audiobuffer_size) newdata = audiobuffer_size;
	if(newdata > loopEnd - fileCursor) newdata = loopEnd - fileCursor;
	if(newdata > audiobuffer_size - buffercursor) newdata = audiobuffer_size - buffercursor;
	
	newdata &= alignMask;
//	if(newdata < minNewData)
//		return;

	if(!newdata) return;
	
	prevtimer = (prevtimer+newdata)%65536;

	int len = newdata*bitdepth;
	int pos = buffercursor*bitdepth;

	nocashPrint2("Read %r0% %r1%", len, pos);
	FS_ReadFileAsync(fileLeft, audiobufferLeft+pos, len);
//	FS_ReadFileAsync(fileRight, audiobufferRight+pos, len);
	
	fileCursor += newdata;
	buffercursor += newdata;
}

void wavPlayer::setLoopPart(int p, int mode)
{
//	iprintf("Set %d %d\n", p, mode);
	
	loopNewBegin = (spm*p*4*8 + bpm / 2) / bpm;
	loopNewCursor = loopNewBegin;
	loopNewEnd = (spm*(p+1)*4*8 + bpm / 2) / bpm;
	loopNewMode = mode;
	
	switch(mode)
	{
		case 0: //Instant to beginning
			seek(loopNewBegin);
			loopBegin = loopNewBegin;
			loopEnd = loopNewEnd;
			break;
		case 1: //Instant to corresponding part
			seek(fileCursor+loopNewBegin-loopBegin);
			loopBegin = loopNewBegin;
			loopEnd = loopNewEnd;
			break;
		case 2: //When current loop ends.
			newLoop = true;
			break;
		case 3: //When current beat ends, to beginning.
			loopEnd = fileCursor + spb - (fileCursor % spb);
			newLoop = true;
			break;
		case 4: //When current beat ends, to corresponding part
			loopEnd = fileCursor + spb - (fileCursor % spb);
			loopNewCursor = loopEnd + loopNewBegin - loopBegin;
			newLoop = true;
			break;
		case 5: //When current bar ends, to beginning.
			loopEnd = fileCursor + (spb*4) - (fileCursor % (spb*4));
			newLoop = true;
			break;
		case 6: //When current bar ends, to corresponding part
			loopEnd = fileCursor + (spb*4) - (fileCursor % (spb*4));
			loopNewCursor = loopEnd + loopNewBegin - loopBegin;
			newLoop = true;
			break;
	}
}


void wavPlayer::playFile(int fileid)
{
	int err;
	stopped = false;
	newLoop = false;
	
	FS_InitFile(fileLeft);
	FS_InitFile(fileRight);
	if(!FS_OpenFileFast(fileLeft, 0x02096114, fileid))
		error("Cant open file left");
	if(!FS_OpenFileFast(fileRight, 0x02096114, fileid+1))
		error("Cant open file right");

	nocashPrint("Files opened!");

	loopBegin = 0;
	loopEnd = spb*4*32*5;
	loopEnd &= alignMask;

	memset(audiobufferLeft, 0, sizeof(audiobufferLeft));
	memset(audiobufferRight, 0, sizeof(audiobufferRight));
	
	SND_SetupChannelPcm(0, bitdepth==2?1:0, audiobufferLeft, 1, 0, audiobuffer_size*bitdepth/4, 127, 0, 16777216 / rate , 0);
	SND_SetupChannelPcm(2, bitdepth==2?1:0, audiobufferRight, 1, 0, audiobuffer_size*bitdepth/4, 127, 0, 16777216 / rate, 127);
	SND_StartTimer(5, 0, 0, 0);

//	leftSoundID = soundPlaySample(audiobufferLeft, SoundFormat_16Bit, audiobuffer_size*bitdepth, rate, 127, 0, true, 0);
//	rightSoundID = soundPlaySample(audiobufferRight, SoundFormat_16Bit, audiobuffer_size*bitdepth, rate, 127, 127, true, 0);

	AS_SetTimer(rate);
	
	buffercursor = audiobuffer_size/2;
	prevtimer = 0;
	fileCursor = 0;
	
	destroyed = false;
}
/*
wavPlayer::~wavPlayer()
{
	destroyed = true;
	//TODO Close file
	AS_SetTimer(0);
}*/


asm (
"@--------------------------------------------------------------------------------------\n"
"    .text                                                                              \n"
"    .arm                                                                               \n"
"                                                                                       \n"
"@ desinterleave an mp3 stereo source                                                   \n"
"@ r0 = interleaved data, r1 = left, r2 = right, r3 = len                               \n"
"                                                                                       \n"
"    .global AS_StereoDesinterleave                                                     \n"
"                                                                                       \n"
"AS_StereoDesinterleave:                                                                \n"
"    stmfd sp!, {r4-r11}                                                                \n"
"                                                                                       \n"
"_loop:                                                                                 \n"
"                                                                                       \n"
"    ldmia r0!, {r4-r12}                                                                \n"
"                                                                                       \n"
"    strh r4, [r1], #2                                                                  \n"
"    mov r4, r4, lsr #16                                                                \n"
"    strh  r4, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r5, [r1], #2                                                                  \n"
"    mov r5, r5, lsr #16                                                                \n"
"    strh  r5, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r6, [r1], #2                                                                  \n"
"    mov r6, r6, lsr #16                                                                \n"
"    strh  r6, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r7, [r1], #2                                                                  \n"
"    mov r7, r7, lsr #16                                                                \n"
"    strh  r7, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r8, [r1], #2                                                                  \n"
"    mov r8, r8, lsr #16                                                                \n"
"    strh  r8, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r9, [r1], #2                                                                  \n"
"    mov r9, r9, lsr #16                                                                \n"
"    strh  r9, [r2], #2                                                                 \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r10, [r1], #2                                                                 \n"
"    mov r10, r10, lsr #16                                                              \n"
"    strh  r10, [r2], #2                                                                \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r11, [r1], #2                                                                 \n"
"    mov r11, r11, lsr #16                                                              \n"
"    strh  r11, [r2], #2                                                                \n"
"    subs r3, #1                                                                        \n"
"    beq _done                                                                          \n"
"                                                                                       \n"
"    strh r12, [r1], #2                                                                 \n"
"    mov r12, r12, lsr #16                                                              \n"
"    strh  r12, [r2], #2                                                                \n"
"    subs r3, #1                                                                        \n"
"    bne _loop                                                                          \n"
"_done:                                                                                 \n"
"                                                                                       \n"
"    ldmia sp!, {r4-r11}                                                                \n"
"    bx lr                                                                              \n"
"@--------------------------------------------------------------------------------------\n"
);

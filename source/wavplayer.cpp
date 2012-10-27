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
	nocashPrint1("SEEK %r0%", pos);
	if(pos < 0) error("Negative seek!");

	FS_SeekFile(&fileLeft, sizeof(loop) + pos*bitdepth, 0);
	FS_SeekFile(&fileRight, sizeof(loop) + (pos+loop.end)*bitdepth, 0);

	fileCursor = pos;
}

bool wavPlayer::update()
{
	if(!playing || paused)
		return false;

	if(fileCursor >= loop.end)
		seek(loop.begin);
	
	if(buffercursor >= audiobuffer_size)
		buffercursor = 0;

	//Find out how many new samples should we push to the buffer.
	int curtimer = TIMER3_DATA;	
	int newData = curtimer - prevtimer;
	if(newData < 0) newData += 65536;

	if(newData > audiobuffer_size - buffercursor)
		newData = audiobuffer_size - buffercursor;

	int readBegin = fileCursor;
	int readEnd = fileCursor + newData;
	
	if(readEnd >= loop.end) readEnd = loop.end;
	
	newData = (readEnd-readBegin);
	prevtimer = (prevtimer+newData)%65536;

	int pos = buffercursor*bitdepth;
	int len = newData*bitdepth;
	FS_ReadFile(&fileLeft, audiobufferLeft+pos, len);
	FS_ReadFile(&fileRight, audiobufferRight+pos, len);
	
	fileCursor += newData;
	buffercursor += newData;

	return true;
}

void wavPlayer::playFile(int fileid)
{
	if(playing) return;
	FS_InitFile(&fileLeft);
	FS_InitFile(&fileRight);
	if(!FS_OpenFileFast(&fileLeft, 0x02096114, fileid))
		error("Cant open file left");
	if(!FS_OpenFileFast(&fileRight, 0x02096114, fileid))
		error("Cant open file right");
	
	FS_ReadFile(&fileLeft, &loop, sizeof(loop));
	nocashPrint2("Loop %r0% %r1%", loop.begin, loop.end);
	
	seek(0);
	paused = true;
	playing = true;
	unpause();
}

void wavPlayer::unpause()
{
	if(!playing || !paused) return;

	AS_SetTimer(rate);
	buffercursor = audiobuffer_size/2;
	prevtimer = TIMER3_DATA;

	memset(audiobufferLeft, 0, sizeof(audiobufferLeft));
	memset(audiobufferRight, 0, sizeof(audiobufferRight));
	
	SND_SetupChannelPcm(0, bitdepth==2?1:0, audiobufferLeft, 1, 0, audiobuffer_size*bitdepth/4, 127, 0, 16777216 / rate , 0);
	SND_SetupChannelPcm(1, bitdepth==2?1:0, audiobufferRight, 1, 0, audiobuffer_size*bitdepth/4, 127, 0, 16777216 / rate, 127);
	SND_StartTimer(3, 0, 0, 0);

	paused = false;
}

void wavPlayer::pause()
{
	if(!playing || paused) return;
	SND_StopTimer(3, 0, 0, 0);
	paused = true;
}

void wavPlayer::stop()
{
	if(!playing) return;
	SND_StopTimer(3, 0, 0, 0);
	playing = false;
	FS_CloseFile(&fileLeft);
	FS_CloseFile(&fileRight);
}

bool wavPlayer::isPlaying()
{
	return playing;
}

void wavPlayer::init()
{
	playing = false;
	paused = false;
}


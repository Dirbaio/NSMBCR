#include<nds.h>
#include "nsmb.h"
#include "wavplayer.h"

#define BUFSIZE 0x200
wavPlayer player;

#define STREAM_THREAD_STACKSIZE (0x1000)
u8 streamThreadStack[STREAM_THREAD_STACKSIZE];
OSThread streamThread;


void streamThreadMain(void* arg)
{
	while(1)
		player.update();
}

void repl_02004EB4()
{
	OS_CreateThread(&streamThread, streamThreadMain, NULL, streamThreadStack+STREAM_THREAD_STACKSIZE, STREAM_THREAD_STACKSIZE, 17);
	OS_WakeupThreadDirect(&streamThread);
	
	//Lock channel 0
	SND_LockChannel(0x0003, 0);
	player.playFile(177);
	
	GX_DispOn();
}

//Disable sound capture.
void nsub_0204F1DC()
{

}

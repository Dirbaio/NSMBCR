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
	{
		player.update();
		waitForVblank();
	}
}

void repl_02004EB4()
{
	player.init();
	
	OS_CreateThread(&streamThread, streamThreadMain, NULL, streamThreadStack+STREAM_THREAD_STACKSIZE, STREAM_THREAD_STACKSIZE, 30);
	OS_WakeupThreadDirect(&streamThread);
	
	//Lock channels 0 and 1
	SND_LockChannel(0x0003, 0);

	GX_DispOn();
}

void everyFrame()
{
	if(myKeysDown() & KEY_L)
		player.unpause();
	
	if(myKeysDown() & KEY_R)
		player.pause();
}

//startMusic
void nsub_0204E5AC(int musicId)
{
	if(player.isPlaying())
		player.stop();
	player.playFile(177);
}

//stopMusic
void nsub_0204E420(int musicId)
{
	player.stop();
}

//pauseMusic
void nsub_0204E47C(bool pause)
{
	if(pause)
		player.pause();
	else
		player.unpause();
}

//Disable sound capture.
void nsub_0204F1DC() {}

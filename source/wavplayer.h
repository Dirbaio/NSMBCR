#ifndef _WAVPLAYER_H
#define _WAVPLAYER_H

#include <nds.h>
#include "nsmb.h"

//Variables that tune playback.
#define bitdepth 1
#define audiobuffer_size (16384)
#define channels 2
#define rate 32768

struct LoopInfo
{
	int begin;
	int end;
};
class wavPlayer
{
	private:
	
	FSFile fileLeft;
	FSFile fileRight;
	
	u8 audiobufferLeft[audiobuffer_size*bitdepth];
	u8 audiobufferRight[audiobuffer_size*bitdepth];
	
	int buffercursor;
	int prevtimer;
	
	bool playing;
	bool paused;

	void seek(int pos);
	
	LoopInfo loop;

	int fileCursor;

	public:
	void init();
	
	void playFile(int fileid);
	void stop();
	
	void pause();
	void unpause();
	
	bool update();
	bool isPlaying();
};

#endif

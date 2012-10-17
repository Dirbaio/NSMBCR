#ifndef _WAVPLAYER_H
#define _WAVPLAYER_H

#include <nds.h>
#include "nsmb.h"

//Variables that tune playback.
#define bitdepth 1
#define audiobuffer_size (16384)
#define channels 2
#define rate 32768

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
	
	int loopBegin;
	int loopEnd;

	bool newLoop;
	int loopNewBegin;
	int loopNewEnd;
	int loopNewCursor;
	int loopNewMode;
		
	int fileCursor;

	public:
	void init();
	
	void setLoopPart(int p, int mode);
	void playFile(int fileid);
	void stop();
	
	void pause();
	void unpause();
	
	bool update();
};

#endif

#ifndef _WAVPLAYER_H
#define _WAVPLAYER_H

#include<nds.h>

//Variables that tune playback.
#define bitdepth 1
#define audiobuffer_size (16384)
#define channels 2
#define rate 24576
#define rate 32768

class wavPlayer
{
	private:
	
	u8 fileLeft[512];
	u8 fileRight[512];
	
	u8 audiobufferLeft[audiobuffer_size*bitdepth];
	u8 audiobufferRight[audiobuffer_size*bitdepth];
	
	int buffercursor;
	int prevtimer;
	
	bool destroyed;
	bool stopped;

	void seek(int pos);
	
	int loopBegin;
	int loopEnd;
	bool loopEnabled;
	
	bool newLoop;
	int loopNewBegin;
	int loopNewEnd;
	int loopNewCursor;
	int loopNewMode;
		
	int fileCursor;

	public:
	void setLoopPart(int p, int mode);
	void playFile(int fileid);
	void stop();
	
	bool update();
};

#endif

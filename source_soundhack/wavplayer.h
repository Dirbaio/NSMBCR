#ifndef _WAVPLAYER_H
#define _WAVPLAYER_H

#include<nds.h>

//Variables that tune playback.
#define bitdepth 1
#define minNewData 4096
#define audiobuffer_size 16384
#define mainbuffer_size (audiobuffer_size*2)
#define channels 2
#define rate 24576
#define alignMask (~2047)

class wavPlayer
{
	private:
	
	u8 fileLeft[512];
	u8 fileRight[512];
	
//	u8 mainbuffer [mainbuffer_size*bitdepth];
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
	
	void update();
};

#endif

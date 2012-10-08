#include<nds.h>
#include "nsmb.h"
#include "wavplayer.h"

#define BUFSIZE 0x200

wavPlayer player;

void respl_02004EB4()
{
	//Lock channel 0
//	SND_LockChannel(0x0005, 0);
	SND_LockChannel(0xFFFF, 0);
	player.playFile(178);
	GX_DispOn();
}

void hosok_020123DC()
{
	player.update();
}

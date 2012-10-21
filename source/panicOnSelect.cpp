#include<nds.h>
#include "nsmb.h"

#define bottomScreenIcon (*((u32*)0x02085A9C))
#define scene35Flag (*((u32*)0x02085A20))

void myLoadLevel(int world, int level)
{
	saveData.world = world;
	bottomScreenIcon = 1;
	loadLevel(0xD, false, world, level, 0, 0, 1, 0, 0, 1, 0xFF, 1);
	scene35Flag = 2;
}

//Replace entrnaces.
//TODO: Make it goto any level based on entrance settings.
void nsub_0201E744()
{
	myLoadLevel(1, 2);
}

void everyFrame();
void hook_020123DC()
{
//	void loadLevel(int sceneId, bool mvsl, int world, int level, int area, int playerNum, int playerMask, int playerChar1, int playerChar2, int unk1, int unk2, int unk3);
	myScanKeys();
	
	if(myKeysDown() & KEY_SELECT)
		myLoadLevel(0, 0);
	everyFrame();
	
//		OS_Panic(); <<- lol.
}

int exitType;
//Replaces exitLevel()
//void nsub_020A189C()
int currLevel = 0;
void repl_020A19E0()
{
	if(exitType == 1)
		currLevel++;
	myLoadLevel(0, currLevel);
}

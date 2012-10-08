#include<nds.h>
#include "nsmb.h"
#include "animation_bin.h"

//Frame is 0 or 1
void copyTiles(int frame)
{
	//We are using pointers of type u16. This is 16-bit numbers, which is 2 bytes.
	//We do this because we can't copy things to VRAM using u8 (1 byte).	
	
	//This is a pointer to the 4 8x8 tiles in VRAM.
	u16* vramPointer = (u16*) 0x06001200;
	//This is a pointer to the 4 8x8 tiles in animation.bin
	u16* animationPointer = (u16*) animation_bin;

	//We have to copy 256 bytes. But because we are copying in 16 bits (2 bytes) we need to divide it by 2.
	for(int i = 0; i < 128; i++)
	{
		vramPointer[i] = animationPointer[i + frame*128];
	}	
}

int counter = 0;

//This function gets called 60 times per second when in game.
void hook_0211f840(MarioActor *athis)
{
	//We increment a "counter" variable.
	counter = counter+1;
	
	//We divide the counter by 20 so it's slower
	int dividedcounter = counter/20;
	
	//We do dividedcounter % 2 so it changes between 0 and 1.
	copyTiles(dividedcounter % 2);
}


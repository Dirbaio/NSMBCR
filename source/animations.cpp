#include "nsmb.h"

#define SHIT       (*(vu32*)0x02000c00)


void doBehavior(MarioActor *athis, u32 i)
{
	if(i == 0x000100FF)
	{
		athis->yAccel=17000;
		athis->yPos += 300;
		athis->xAccel=0;
	}
	
	if(i == 0x020000FE)
	{
		athis->yAccel-=170;
	}
}

int runningTime = 0;
int tripleJumpOverride = 0;
bool lastjumped = false;
bool nowjumped = false;

bool lastrun = false;
bool nowrun = false;
bool touchedground = true;
int flyTime = 0;
int flyPressTime = 0;
int invisibleHack = 0;

int abss(int a)
{
	return a<0?-a:a;
}

bool runKeyNowPressed()
{
	if(controlOptionsA)
		return (pressedKeys & X) != 0 || (pressedKeys & Y) != 0;
	else
		return (pressedKeys & B) != 0 || (pressedKeys & Y) != 0;
}
bool jumpKeyNowPressed()
{
	if(controlOptionsA)
		return (pressedKeys & A) != 0 || (pressedKeys & B) != 0;
	else
		return (pressedKeys & A) != 0 || (pressedKeys & X) != 0;
}

void myReadKeys()
{
	lastjumped = nowjumped;
	lastrun = nowrun;
	nowjumped = jumpKeyNowPressed();
	nowrun = runKeyNowPressed();
}

bool jumpKeyPressed()
{	
	return nowjumped && !lastjumped;
}
bool runKeyPressed()
{
	return nowrun && !lastrun;
}
void doTripleJump(MarioActor* athis)
{
	athis->tripleJumpRelated778 = 0;
	athis->tripleJumpRelated7BF = 0;
	athis->consecutiveJumps = 2;
	tripleJumpOverride = 2;
}

void hook_0211f840(MarioActor *athis)
{
	myReadKeys();
	
	u32 x = athis->xPos>>12;
	u32 y = (-athis->yPos)>>12;
	doBehavior(athis, getTileBehaviorAtPos2(x+6, y+3));
	doBehavior(athis, getTileBehaviorAtPos2(x-6, y+3));
	SHIT = (u32) athis;
	
	if(athis->marioPowerupState != 2) //TODO Change it to replace something that's not fireflower
	{
		runningTime = 0;
		flyTime = 0;
		flyPressTime = 0;
		touchedground = true;
		invisibleHack = 0;
	}
	else
	{
	
		if(abss(athis->xAccel) == 0x3000 && flyTime == 0 && touchedground)
		{
//			nocashPrint1("RUN %r0%", runningTime);
			runningTime++;
		}
		else
			runningTime = 0;
			
		if(runningTime > 0x30 && athis->isOnGround)
		{
			invisibleHack = (runningTime / 3) % 2;
			if(jumpKeyPressed())
			{
				flyTime = 5*60;
				doTripleJump(athis);
				touchedground = false;
			}
		}
		else invisibleHack = 0;
		
		if(jumpKeyPressed())
		{
			flyPressTime = 10;
		}

		if(flyTime > 0)
		{
//			nocashPrint1("FLY %r0%", flyTime);
			
			flyTime--;
		}
		
		if(flyPressTime > 0)
		{
			if(flyTime > 0)
			{
				if(athis->yAccel < 0x2000)
					athis->yAccel = 0x2000;
			}
			else
				if(athis->yAccel < -0x1000)
					athis->yAccel = -0X1000;
					
			flyPressTime --;
		}
		
		if(athis->isOnGround != 0 && flyTime < 5*60-10)
			flyTime = 0;
		if(athis->isOnGround != 0)
			touchedground = true;
	}
}

int frame = 1;
u16* charPtr = NULL;
u16* animationFile = NULL;

void copy8Tile(int dn, int sn)
{
	u16* destPtr = charPtr + dn * (8 * 8 / 2);
	u16* srcPtr = animationFile + sn * (8 * 8 / 2);
	
	for(int i = 0; i < 8*8/2; i++)
		destPtr[i] = srcPtr[i];
}

void copyTile(int dx, int dy, int sx, int sy)
{
	sy += sx / 4;
	sx %= 4;
	copy8Tile(dx*2 + dy*64, sx*2+sy*16);
	copy8Tile(dx*2+1 + dy*64, sx*2+1+sy*16);
	copy8Tile(dx*2 + dy*64+32, sx*2+sy*16+8);
	copy8Tile(dx*2+1 + dy*64+32, sx*2+1+sy*16+8);
}

void hook_020b8370()
{
	frame++;
	if(frame == 4*6) frame = 0;
	
	charPtr = G2_GetBG2CharPtr();
	copyTile(4, 1, frame%4, 0);
	copyTile(6, 1, frame%4, 1);
}

void hook_020AE318()
{
	animationFile = (u16*) allocFromCacheTable(4096);
	loadFileByExtId_LZ_Dest(48, animationFile); //wrecking_crew.narc
//	nocashPrint1("Animations loaded! %r0%", (u32)animationFile);
}

#include<nds.h>

#ifndef _NSMB_H
#define _NSMB_H

#ifdef __cplusplus
extern "C"
{
#endif
	extern void* ptrToLevelActor;
	extern u16 setTileTable[280];
	
	void changeTile(void* scenePtr, int x, int y, int tile);
	
	void nocashPrint(const char* txt);
	void nocashPrint1(const char* txt, u32 r0);
	void nocashPrint2(const char* txt, u32 r0, u32 r1);
	void nocashPrint3(const char* txt, u32 r0, u32 r1, u32 r2);
	
	void CARDi_ReadRom(u32 dma,
    	const void* src, void* dst, u32 len, 
	    u32 callback, void *arg, bool is_async);
	void OSi_ReadCardRom32Hax(void* src, void* dest, u32 len);
	
	u32 loadCompressedFile(u32 fileID, void* dest);
	void* findActorByType(short classId, int from);
	u32 getTileBehaviorAtPos2(u32 x, u32 y);
	
    void ChangeSceneIfNeeded();
    
	void FS_Init( u32 unk );
	u16* G2_GetBG2CharPtr();
	void GX_DispOn();

	void SND_LockChannel( u32 chBitMask, u32 flags );
	void SND_SetupChannelPsg(int chNo, int duty, int volume, int shift, int timer, int pan );

	void SND_SetupChannelPcm(int chNo, int format, const void* dataAddr, int loop,
		int loopStart, int dataLen, int volume, int shift, int timer, int pan );
	void SND_StartTimer(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags );


	void FS_InitFile( void *p_file );
	bool FS_OpenFileFast( void *p_file, u32 archivePtr, int file_id );
	bool FS_SeekFile( void *p_file, s32 offset, int origin );
	s32 FS_ReadFile( void *p_file, void *dst, s32 len );
	s32 FS_ReadFileAsync( void *p_file, void *dst, s32 len );

	void* allocFromGameHeap(u32 len);
	void freeToGameHeap(void* ptr);

	void OS_Panic();
	void loadLevel(int sceneId, bool mvsl, int world, int level, int area, int playerNum, int playerMask, int playerChar1, int playerChar2, int unk1, int unk2, int unk3);

#ifdef __cplusplus
}
#endif

struct MarioActor
{
	void* vtable;
	u32 unk004; //Settings
	u32 unk008; //Zeros
	u16 unk00C; //
	u16 unk00E; //
	u32 unk010; //SOme Ptr
	u32 unk014; //settings
	u32 unk018;
	u32 unk01C; //POINTER to table or struct? Used in pipes??
	u32 unk020;
	u32 unk024;
	u32 unk028;
	u32 unk02C;
	u32 unk030;
	u32 unk034;
	u32 unk038;
	u32 unk03C;
	u32 unk040;
	u32 unk044;
	u32 unk048;
	u32 unk04C;
	u32 unk050;
	u32 unk054;
	u32 unk058;
	u32 unk05C;
	u32 xPos;
	u32 yPos;
	u32 unk068;
	u32 unk06C;
	u32 xPos0;
	u32 yPos0;
	u32 unk078;
	u32 unk07C;
	u32 xAccel0;
	u32 yAccel0;
	u32 unk088;
	u16 unk08C;
	u16 unk08E; //PIPE and behavior RELATED
	u32 unk090;
	u32 unk094;
	u32 unk098;
	u32 unk09C;
	u16 unk0A0; //Collision related
	u16 unk0A2;
	u32 unk0A4;
	u32 unk0A8; //PIPE RELATED
	u32 unk0AC;
	u32 unk0B0;
	u8  unk0B4; // collision related hmmm
	u8  unk0B5;
	u8  unk0B6;
	u8  unk0B7;
	u32 unk0B8;
	u32 unk0BC;
	u32 unk0C0;
	u32 unk0C4;
	u32 unk0C8;
	u32 unk0CC;
	u32 xAccel;
	u32 yAccel;
	u32 unk0D8;
	u32 unk0DC;
	u32 unk0E0;
	u32 unk0E4;
	u32 unk0E8;
	u32 unk0EC;
	u32 unk0F0;
	u32 unk0F4;
	u32 unk0F8;
	u32 unk0FC;
};

struct SaveData 
{
	u32 header;
	u32 field_04;
	u32 field_08;
	u32 lives;
	u32 field_10;
	u32 field_14;
	u32 field_18;
	u32 field_1C;
	u32 world;
	u32 field_24;
	u32 worldMapNode;
	u32 field_2C;
	u32 field_30;
	u32 field_34;
	u32 field_38;
	u32 field_3C;
	u32 field_40;
	u32 field_44;
	u32 field_48;
	u32 field_4C;
	u32 field_50;
	u32 field_54;
	u32 field_58;
	u8 field_5C;
	u8 field_5D;
	u8 field_5E;
	u8 field_5F;
	u16 worlds[8];
	u8 starCoins[200];
	u8 levels[240];
	u8 unk[32];
};

extern SaveData saveData;

#define KEYS_CR			(*(vu32*)0x04000130)
#define START_REG       (*(vu32*)0x023FFC20)
#define PRINT_REG       (*(vu32*)0x04FFF010)

inline void print(const char* s)
{
	PRINT_REG = (u32) s;
}

#endif

//Custom key reading funcs.
void myScanKeys();
int myKeysHeld();
int myKeysDown();

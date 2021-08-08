#ifndef OBJ_LEVELSELECT_H
#define OBJ_LEVELSELECT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    int bgScrollPos;                //= 240;
    int bgScrollPositions[4];       //= { 240, 3, 3, 3 };
    int cheat_RickyMode[9];         //= { 1, 9, 7, 9, 0, 8, 1, 1, 255 };
    int cheat_AllEmeralds[5];       //= { 4, 1, 2, 6, 255 };
    int cheat_MaxContinues[9];      //= { 1, 9, 9, 2, 1, 1, 2, 4, 255 };
    int cheat_SwapGameMode[9];      //= { 2, 0, 1, 8, 0, 6, 2, 3, 255 };
    int cheat_UnlockAllMedals[9];   //= { 1, 9, 8, 9, 0, 5, 0, 1, 255 };
    int cheat_SuperDash[9];         //= { 2, 0, 1, 7, 0, 8, 1, 5, 255 };
    int cheat_MaxControl[5];        //= { 9, 0, 0, 1, 255 };
    int cheat_DisableSuperMusic[5]; //= { 6, 2, 1, 4, 255 };
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int bgScrollPosID;
    int startMusicID;
    int soundTestMax;
    ushort sfxFail;
    ushort sfxRing;
    ushort sfxEmerald;
    ushort sfxContinue;
    ushort sfxMedalGot;
    int *cheatCodePtrs[8];
    int cheatUnknown[8];
    void (*checkCheatActivated[8])(void);
#else
    int bgScrollPos;          //= 240;
    int bgScrollPosID;
    int bgScrollPositions[4]; //= { 240, 3, 3, 3 };
    int startMusicID;
    int soundTestMax;
    ushort sfxFail;
#endif
} ObjectLevelSelect;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
    int labelID;
    int soundTestID;
    int playerID;
    int player2ID;
    Entity *labelPtrs2[32];
    Entity *labelPtrs[32];
    Entity *soundTestLabel;
    Entity *zoneIcon;
    Entity *player1Icon;
    Entity *player2Icon;
#if RETRO_USE_PLUS
    Entity *storedTextEntity;
#endif
    int labelCount;
#if RETRO_USE_PLUS
    int field_18C;
    int field_190;
    int field_194;
    int field_198;
    int field_19C;
    int field_1A0;
#endif
} EntityLevelSelect;

// Object Struct
extern ObjectLevelSelect *LevelSelect;

// Standard Entity Events
void LevelSelect_Update(void);
void LevelSelect_LateUpdate(void);
void LevelSelect_StaticUpdate(void);
void LevelSelect_Draw(void);
void LevelSelect_Create(void* data);
void LevelSelect_StageLoad(void);
void LevelSelect_EditorDraw(void);
void LevelSelect_EditorLoad(void);
void LevelSelect_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void LevelSelect_CheatActivated_AllEmeralds(void);
void LevelSelect_CheatActivated_ChangeSuperMusicFlag(void);
void LevelSelect_CheatActivated_MaxContinues(void);
void LevelSelect_CheatActivated_MaxControl(void);
void LevelSelect_CheatActivated_RickyMode(void);
void LevelSelect_CheatActivated_SuperDash(void);
void LevelSelect_CheatActivated_SwapGameMode(void);
void LevelSelect_CheatActivated_UnlockAllMedals(void);
#endif

void LevelSelect_State_SetupEntities(void);
void LevelSelect_ManagePlayerIcon(void);

void LevelSelect_StateDraw_Fade(void);

void LevelSelect_Unknown1(void);
void LevelSelect_Unknown2(void);
void LevelSelect_Unknown3(void);
void LevelSelect_SetLabelHighlighted(bool32 highlight);
void LevelSelect_Unknown6(void);
void LevelSelect_Unknown7(void);

#endif //!OBJ_LEVELSELECT_H

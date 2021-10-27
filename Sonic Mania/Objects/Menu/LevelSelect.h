#ifndef OBJ_LEVELSELECT_H
#define OBJ_LEVELSELECT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    STATIC(int32 bgScrollPos, 240);
    TABLE(int32 bgScrollPositions[4], { 240, 3, 3, 3 });
    TABLE(int32 cheat_RickyMode[9], { 1, 9, 7, 9, 0, 8, 1, 1, 255 });
    TABLE(int32 cheat_AllEmeralds[5], { 4, 1, 2, 6, 255 });
    TABLE(int32 cheat_MaxContinues[9], { 1, 9, 9, 2, 1, 1, 2, 4, 255 });
    TABLE(int32 cheat_SwapGameMode[9], { 2, 0, 1, 8, 0, 6, 2, 3, 255 });
    TABLE(int32 cheat_UnlockAllMedals[9], { 1, 9, 8, 9, 0, 5, 0, 1, 255 });
    TABLE(int32 cheat_SuperDash[9], { 2, 0, 1, 7, 0, 8, 1, 5, 255 });
    TABLE(int32 cheat_MaxControl[5], { 9, 0, 0, 1, 255 });
    TABLE(int32 cheat_DisableSuperMusic[5], { 6, 2, 1, 4, 255 });
    int32 field_108;
    int32 field_10C;
    int32 field_110;
    int32 field_114;
    int32 bgScrollPosID;
    int32 startMusicID;
    int32 soundTestMax;
    uint16 sfxFail;
    uint16 sfxRing;
    uint16 sfxEmerald;
    uint16 sfxContinue;
    uint16 sfxMedalGot;
    int32 *cheatCodePtrs[8];
    int32 cheatUnknown[8];
    void (*checkCheatActivated[8])(void);
#else
    STATIC(int32 bgScrollPos, 240);
    int32 bgScrollPosID;
    TABLE(int32 bgScrollPositions[4], { 240, 3, 3, 3 });
    int32 startMusicID;
    int32 soundTestMax;
    uint16 sfxFail;
#endif
} ObjectLevelSelect;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 labelID;
    int32 soundTestID;
    int32 playerID;
    int32 player2ID;
    Entity *labelPtrs2[32];
    Entity *labelPtrs[32];
    Entity *soundTestLabel;
    Entity *zoneIcon;
    Entity *player1Icon;
    Entity *player2Icon;
#if RETRO_USE_PLUS
    Entity *storedTextEntity;
#endif
    int32 labelCount;
#if RETRO_USE_PLUS
    int32 field_18C;
    int32 field_190;
    int32 field_194;
    int32 field_198;
    int32 field_19C;
    int32 field_1A0;
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
#if RETRO_INCLUDE_EDITOR
void LevelSelect_EditorDraw(void);
void LevelSelect_EditorLoad(void);
#endif
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

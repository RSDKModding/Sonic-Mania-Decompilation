#ifndef OBJ_CUTSCENEHBH_H
#define OBJ_CUTSCENEHBH_H

#include "SonicMania.h"

typedef enum  {
    HBH_GUNNER,
    HBH_SHINOBI,
    HBH_MYSTIC,
    HBH_RIDER,
    HBH_KING,
    HBH_ROUGESA,
    HBH_ROUGESB,
    HBH_ROUGESC,
    HBH_KINGDAMAGED,
    HBH_PILE,
    HBH_KINGTMZ2,
} HBHTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    colour paletteColours[128];
} ObjectCutsceneHBH;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 characterID;
    int oscillate;
    int hiddenAtStart;
    int field_68;
    char field_6C;
    char field_6D;
    char field_6E;
    char field_6F;
    int dword70;
    int dword74;
    Vector2 originPos;
    int timer;
    int dword84;
    int colourSet;
    colour colours[128];
    bool32 paletteFlag;
    bool32 timerFlag;
    uint16 spriteIndex;
    uint16 field_296;
    Animator animator;
    Animator animator2;
} EntityCutsceneHBH;

// Object Struct
extern ObjectCutsceneHBH *CutsceneHBH;

// Standard Entity Events
void CutsceneHBH_Update(void);
void CutsceneHBH_LateUpdate(void);
void CutsceneHBH_StaticUpdate(void);
void CutsceneHBH_Draw(void);
void CutsceneHBH_Create(void* data);
void CutsceneHBH_StageLoad(void);
void CutsceneHBH_EditorDraw(void);
void CutsceneHBH_EditorLoad(void);
void CutsceneHBH_Serialize(void);

// Extra Entity Functions
void CutsceneHBH_SetupColours(void);
void CutsceneHBH_SetupPalettes(void);
void CutsceneHBH_StorePalette(void);
void CutsceneHBH_RestorePalette(void);
void CutsceneHBH_LoadSprites(void);
EntityCutsceneHBH *CutsceneHBH_GetEntity(char charID);

void CutsceneHBH_Unknown7(void);
void CutsceneHBH_Unknown8(void);
void CutsceneHBH_Unknown9(void);
void CutsceneHBH_Unknown10(void);
void CutsceneHBH_Unknown11(void);
void CutsceneHBH_Unknown12(void);
void CutsceneHBH_Unknown13(void);
void CutsceneHBH_Unknown14(void);
void CutsceneHBH_Unknown15(void);
void CutsceneHBH_Unknown16(void);
void CutsceneHBH_Unknown17(void);
void CutsceneHBH_Unknown18(void);
void CutsceneHBH_Unknown19(void);
void CutsceneHBH_Unknown20(void);
void CutsceneHBH_Unknown21(void);

#endif //!OBJ_CUTSCENEHBH_H

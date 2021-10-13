#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUITABanner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    EntityUIControl *parent;
    TextInfo text;
    int32 timer;
    uint8 characterID;
    uint8 zoneID;
    uint8 actID;
    int32 isEncore;
    int32 startDir;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator labelAnimator;
} EntityUITABanner;

// Object Struct
extern ObjectUITABanner *UITABanner;

// Standard Entity Events
void UITABanner_Update(void);
void UITABanner_LateUpdate(void);
void UITABanner_StaticUpdate(void);
void UITABanner_Draw(void);
void UITABanner_Create(void* data);
void UITABanner_StageLoad(void);
void UITABanner_EditorDraw(void);
void UITABanner_EditorLoad(void);
void UITABanner_Serialize(void);

// Extra Entity Functions
void UITABanner_Unknown1(uint8 characterID, EntityUITABanner *banner, uint8 zoneID, uint8 actID, bool32 isEncore);
void UITABanner_Unknown2(int32 drawX, int32 drawY, bool32 isEncore);
void UITABanner_Unknown3(uint8 actID, uint8 characterID, bool32 isEncore, int32 drawX, int32 drawY);
void UITABanner_Unknown4(int32 drawX, int32 drawY, int32 zoneID);
void UITABanner_Unknown5(uint8 actID, uint8 zoneID, uint8 characterID, bool32 isEncore, int32 drawX, int32 drawY);

#endif

#endif //!OBJ_UITABANNER_H

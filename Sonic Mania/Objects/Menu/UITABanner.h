#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUITABanner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    EntityUIControl *parent;
    TextInfo text;
    int timer;
    byte characterID;
    byte zoneID;
    byte actID;
    int field_78;
    int startDir;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
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
void UITABanner_Unknown1(byte characterID, EntityUITABanner *banner, byte zoneID, byte actID, int a5);
void UITABanner_Unknown2(int drawX, int drawY, int a3);
void UITABanner_Unknown3(byte actID, byte characterID, int a3, int drawX, int drawY);
void UITABanner_Unknown4(int drawX, int drawY, int zoneID);
void UITABanner_Unknown5(byte actID, byte zoneID, byte characterID, int a4, int drawX, int drawY);

#endif

#endif //!OBJ_UITABANNER_H

#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    int field_8;
    bool32 field_C;
    bool32 flag;
} ObjectUITAZoneModule;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    byte zoneID;
    TextInfo text1;
    TextInfo text2;
#if !RETRO_USE_PLUS
    bool32 debugExpand;
#endif
    bool32 wasDisabled;
    TextInfo text3;
    TextInfo text4;
    int field_12C;
    Vector2 drawPos;
    byte field_138;
    byte characterID;
    byte actID;
    int id;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
} EntityUITAZoneModule;

// Object Struct
extern ObjectUITAZoneModule *UITAZoneModule;

// Standard Entity Events
void UITAZoneModule_Update(void);
void UITAZoneModule_LateUpdate(void);
void UITAZoneModule_StaticUpdate(void);
void UITAZoneModule_Draw(void);
void UITAZoneModule_Create(void* data);
void UITAZoneModule_StageLoad(void);
void UITAZoneModule_EditorDraw(void);
void UITAZoneModule_EditorLoad(void);
void UITAZoneModule_Serialize(void);

// Extra Entity Functions
void UITAZoneModule_Setup(void);
void UITAZoneModule_SetupText(void);
void UITAZoneModule_Unknown3(void);
void UITAZoneModule_Unknown4(void);
void UITAZoneModule_Unknown5(void);
void UITAZoneModule_Unknown6(void);
void UITAZoneModule_FailCB(void);
void UITAZoneModule_Options3CB(void);
bool32 UITAZoneModule_Options7CB(void);
bool32 UITAZoneModule_Options8CB(void);
void UITAZoneModule_Options5CB(void);
void UITAZoneModule_Options6CB(void);
void UITAZoneModule_TransitionCB(void);
void UITAZoneModule_Unknown14(void);
void UITAZoneModule_Unknown15(void);
void UITAZoneModule_Unknown16(void);
void UITAZoneModule_Unknown17(void);
void UITAZoneModule_Unknown18(void);

#endif //!OBJ_UITAZONEMODULE_H

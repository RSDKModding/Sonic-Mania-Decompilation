#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Entity *entityPtr;
#if RETRO_USE_PLUS
    bool32 isEncoreMode;
#endif
    bool32 flag;
} ObjectUITAZoneModule;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
#if !RETRO_USE_PLUS
    int32 value;
#endif
    uint8 zoneID;
    TextInfo text1;
    TextInfo text2;
#if !RETRO_USE_PLUS
    bool32 debugExpand;
#endif
    bool32 wasDisabled;
    TextInfo text3;
    TextInfo text4;
    int32 field_12C;
    Vector2 drawPos;
#if !RETRO_USE_PLUS
    int32 field_138_2;
    int32 id;
    int32 field_140;
    int32 announceTimer;
#endif
    uint8 field_138;
    uint8 characterID;
    uint8 actID;
#if RETRO_USE_PLUS
    bool32 isEncore;
#else
    uint8 rank;
#endif
    int32 field_14C;
    int32 field_150;
#if RETRO_USE_PLUS
    int32 field_154;
    int32 field_158;
#endif
#if !RETRO_USE_PLUS
    Animator animator7;
#endif
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
#if RETRO_INCLUDE_EDITOR
void UITAZoneModule_EditorDraw(void);
void UITAZoneModule_EditorLoad(void);
#endif
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

#if !RETRO_USE_PLUS
void UITAZoneModule_DrawTime(Vector2 *drawPos, int32 minutes, int32 seconds, int32 milliseconds);
void UITAZoneModule_Unknown8(void);
void UITAZoneModule_Unknown7(void);
void UITAZoneModule_Unknown19(void);
void UITAZoneModule_Unknown21(void);
void UITAZoneModule_Unknown22(void);
void UITAZoneModule_Unknown23(void);
void UITAZoneModule_Unknown24(void);
void UITAZoneModule_Unknown25(int32 player, int32 zone, int32 act, int32 a4, void (*callback)(void));
void UITAZoneModule_Unknown26(EntityUIControl *control, uint8 characterID, uint32 zoneID, uint8 actID, int32 score);
void UITAZoneModule_Unknown27(void);
void UITAZoneModule_Unknown28(void);
void UITAZoneModule_Unknown29(void);
void UITAZoneModule_Unknown30(void);
void UITAZoneModule_Unknown31(void);
#endif

#endif //!OBJ_UITAZONEMODULE_H

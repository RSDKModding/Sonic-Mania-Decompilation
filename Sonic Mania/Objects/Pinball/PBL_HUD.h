#ifndef OBJ_PBL_HUD_H
#define OBJ_PBL_HUD_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort field_6;
} ObjectPBL_HUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
    Vector2 offset;
    TextInfo text;
    Animator data1;
    Animator animator2;
    Animator data3;
} EntityPBL_HUD;

// Object Struct
extern ObjectPBL_HUD *PBL_HUD;

// Standard Entity Events
void PBL_HUD_Update(void);
void PBL_HUD_LateUpdate(void);
void PBL_HUD_StaticUpdate(void);
void PBL_HUD_Draw(void);
void PBL_HUD_Create(void* data);
void PBL_HUD_StageLoad(void);
void PBL_HUD_EditorDraw(void);
void PBL_HUD_EditorLoad(void);
void PBL_HUD_Serialize(void);

// Extra Entity Functions
void PBL_HUD_DisplayMessage(EntityPBL_HUD *entity, const char* message, int type);
void PBL_HUD_State_Message0(void);
void PBL_HUD_State_Message1(void);
void PBL_HUD_State_Message2(void);
void PBL_HUD_State_Message3(void);
void PBL_HUD_State_Message4(void);
void PBL_HUD_State_Message6(void);
void PBL_HUD_State_Message7(void);
void PBL_HUD_StateDraw_Unknown1(void);
void PBL_HUD_StateDraw_Unknown2(void);
void PBL_HUD_StateDraw_Unknown3(void);
void PBL_HUD_Unknown12(void);
void PBL_HUD_Unknown13(void);
#endif

#endif //!OBJ_PBL_HUD_H

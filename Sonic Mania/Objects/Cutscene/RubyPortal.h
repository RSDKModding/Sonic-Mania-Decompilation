#ifndef OBJ_RUBYPORTAL_H
#define OBJ_RUBYPORTAL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
#if RETRO_USE_PLUS
    bool32 openPortal;
#endif
} ObjectRubyPortal;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Animator animator;
    Hitbox hitbox;
} EntityRubyPortal;

// Object Struct
extern ObjectRubyPortal *RubyPortal;

// Standard Entity Events
void RubyPortal_Update(void);
void RubyPortal_LateUpdate(void);
void RubyPortal_StaticUpdate(void);
void RubyPortal_Draw(void);
void RubyPortal_Create(void* data);
void RubyPortal_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void RubyPortal_EditorDraw(void);
void RubyPortal_EditorLoad(void);
#endif
void RubyPortal_Serialize(void);

// Extra Entity Functions
void RubyPortal_Unknown1(void);
void RubyPortal_Unknown2(void);
void RubyPortal_Unknown3(void);
void RubyPortal_Unknown4(void);
void RubyPortal_Unknown5(void);
void RubyPortal_Unknown6(void);
void RubyPortal_Unknown7(void);
void RubyPortal_Unknown8(void);
void RubyPortal_Unknown9(void);

#endif //!OBJ_RUBYPORTAL_H

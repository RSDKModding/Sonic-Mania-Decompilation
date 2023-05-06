#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "Game.h"

// Object Class
struct ObjectSyringe {
    RSDK_OBJECT
    Hitbox hitboxBody;
    Hitbox hitboxHandle;
    uint16 aniFrames;
    uint16 sfxBloop;
};

// Entity Class
struct EntitySyringe {
    RSDK_ENTITY
    StateMachine(state); // unused
    int32 type;
    int32 offsetY;
    bool32 activated;
    color color;
    uint8 tag;
    Animator containerAnimator;
    Animator handleAnimator;
};

// Object Struct
extern ObjectSyringe *Syringe;

// Standard Entity Events
void Syringe_Update(void);
void Syringe_LateUpdate(void);
void Syringe_StaticUpdate(void);
void Syringe_Draw(void);
void Syringe_Create(void *data);
void Syringe_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Syringe_EditorDraw(void);
void Syringe_EditorLoad(void);
#endif
void Syringe_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_SYRINGE_H

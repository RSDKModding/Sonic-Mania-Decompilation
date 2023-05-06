#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

#include "Game.h"

// Object Class
struct ObjectFlipper {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxFlipper;
};

// Entity Class
struct EntityFlipper {
    RSDK_ENTITY
    StateMachine(state);
    uint8 activePlayers;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectFlipper *Flipper;

// Standard Entity Events
void Flipper_Update(void);
void Flipper_LateUpdate(void);
void Flipper_StaticUpdate(void);
void Flipper_Draw(void);
void Flipper_Create(void *data);
void Flipper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Flipper_EditorDraw(void);
void Flipper_EditorLoad(void);
#endif
void Flipper_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FLIPPER_H

#ifndef OBJ_OOZFLAMES_H
#define OBJ_OOZFLAMES_H

#include "Game.h"

// Object Class
struct ObjectOOZFlames {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityOOZFlames {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 flamePos;
    Animator animator;
};

// Object Struct
extern ObjectOOZFlames *OOZFlames;

// Standard Entity Events
void OOZFlames_Update(void);
void OOZFlames_LateUpdate(void);
void OOZFlames_StaticUpdate(void);
void OOZFlames_Draw(void);
void OOZFlames_Create(void *data);
void OOZFlames_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OOZFlames_EditorDraw(void);
void OOZFlames_EditorLoad(void);
#endif
void OOZFlames_Serialize(void);

// Extra Entity Functions
void OOZFlames_State_Appear(void);
void OOZFlames_State_Rise(void);

#endif //! OBJ_OOZFLAMES_H

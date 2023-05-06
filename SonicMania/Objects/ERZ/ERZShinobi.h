#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "Game.h"

// Object Class
struct ObjectERZShinobi {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityERZShinobi {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 finRadius;
    int32 rotStore;
    int32 rotSpeed; // Not used... yet! (See ERZ/PhantomShinobi)
    int32 numBounces;
    bool32 prevOnGround;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator bodyAnimator;
    Animator armAnimator;
    Animator finAnimator;
};

// Object Struct
extern ObjectERZShinobi *ERZShinobi;

// Standard Entity Events
void ERZShinobi_Update(void);
void ERZShinobi_LateUpdate(void);
void ERZShinobi_StaticUpdate(void);
void ERZShinobi_Draw(void);
void ERZShinobi_Create(void *data);
void ERZShinobi_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZShinobi_EditorDraw(void);
void ERZShinobi_EditorLoad(void);
#endif
void ERZShinobi_Serialize(void);

// Extra Entity Functions
void ERZShinobi_CheckPlayerCollisions(void);
void ERZShinobi_Hit(void);
void ERZShinobi_HandleTileCollisions(void);
void ERZShinobi_State_Moving(void);

#endif //! OBJ_ERZSHINOBI_H

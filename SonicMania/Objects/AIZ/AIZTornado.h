#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

#include "Game.h"

// Object Class
struct ObjectAIZTornado {
    RSDK_OBJECT
    TABLE(int32 flameOffsets[5], { -0x20000, -0x10000, 0x00000, 0x10000, 0x20000 });
    uint16 aniFrames;
};

// Entity Class
struct EntityAIZTornado {
    RSDK_ENTITY
    StateMachine(state);
    int32 collideTimer; // unused here (used in MSZ/Tornado)
    int32 turnAngle;
    bool32 isStood;
    bool32 showFlame;
    int32 storeY;       // unused here (used in MSZ/Tornado)
    int32 gravityForce; // unused here (used in MSZ/Tornado)
    Vector2 prevPos;
    Vector2 movePos;
    Vector2 newPos;
    Vector2 moveVelocity;
    bool32 disableInteractions;
    int32 offsetX; // unused here (used in MSZ/Tornado)
    Animator animatorTornado;
    Animator animatorPropeller;
    Animator animatorFlame;
    Animator animatorPilot;
};

// Object Struct
extern ObjectAIZTornado *AIZTornado;

// Standard Entity Events
void AIZTornado_Update(void);
void AIZTornado_LateUpdate(void);
void AIZTornado_StaticUpdate(void);
void AIZTornado_Draw(void);
void AIZTornado_Create(void *data);
void AIZTornado_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZTornado_EditorDraw(void);
void AIZTornado_EditorLoad(void);
#endif
void AIZTornado_Serialize(void);

// Extra Entity Functions
void AIZTornado_HandleMovement(void);
void AIZTornado_HandlePlayerCollisions(void);
void AIZTornado_State_Move(void);

#endif //! OBJ_AIZTORNADO_H

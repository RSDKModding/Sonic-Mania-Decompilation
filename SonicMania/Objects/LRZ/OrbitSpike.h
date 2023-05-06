#ifndef OBJ_ORBITSPIKE_H
#define OBJ_ORBITSPIKE_H

#include "Game.h"

// Object Class
struct ObjectOrbitSpike {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityOrbitSpike {
    RSDK_ENTITY
    Animator animator;
    Vector2 amplitude;
    uint8 offset;
    Vector2 center;
};

// Object Struct
extern ObjectOrbitSpike *OrbitSpike;

// Standard Entity Events
void OrbitSpike_Update(void);
void OrbitSpike_LateUpdate(void);
void OrbitSpike_StaticUpdate(void);
void OrbitSpike_Draw(void);
void OrbitSpike_Create(void *data);
void OrbitSpike_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OrbitSpike_EditorDraw(void);
void OrbitSpike_EditorLoad(void);
#endif
void OrbitSpike_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_ORBITSPIKE_H

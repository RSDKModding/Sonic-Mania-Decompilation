#ifndef OBJ_ORBITSPIKE_H
#define OBJ_ORBITSPIKE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectOrbitSpike;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    Vector2 amplitude;
    byte offset;
    Vector2 center;
} EntityOrbitSpike;

// Object Struct
extern ObjectOrbitSpike *OrbitSpike;

// Standard Entity Events
void OrbitSpike_Update(void);
void OrbitSpike_LateUpdate(void);
void OrbitSpike_StaticUpdate(void);
void OrbitSpike_Draw(void);
void OrbitSpike_Create(void* data);
void OrbitSpike_StageLoad(void);
void OrbitSpike_EditorDraw(void);
void OrbitSpike_EditorLoad(void);
void OrbitSpike_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ORBITSPIKE_H

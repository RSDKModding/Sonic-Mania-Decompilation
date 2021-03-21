#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectDebris;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int timer;
    int gravity;
    int rotSpeed;
    Vector2 scaleInc;
    int cooldown;
    AnimationData data;
} EntityDebris;

// Object Struct
extern ObjectDebris *Debris;

// Standard Entity Events
void Debris_Update();
void Debris_LateUpdate();
void Debris_StaticUpdate();
void Debris_Draw();
void Debris_Create(void* data);
void Debris_StageLoad();
void Debris_EditorDraw();
void Debris_EditorLoad();
void Debris_Serialize();

// Extra Entity Functions
void Debris_State_Fall();
void Debris_State_LightningSpark();
void Debris_State_Unknkown();

void Debris_Unknkown1(int spriteIndex, int *a2, int animationID);
void Debris_Unknkown2(int spriteIndex, int *a2);

#endif //!OBJ_DEBRIS_H

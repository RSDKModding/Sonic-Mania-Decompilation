#ifndef OBJ_SPRING_H
#define OBJ_SPRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_Spring;
} ObjectSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int type;
    int flipFlag;
    byte planeFilter;
    char gap65;
    char field_66;
    char field_67;
    int timer;
    EntityAnimationData data;
    Hitbox hitbox;
} EntitySpring;

// Object Struct
extern ObjectSpring *Spring;

// Standard Entity Events
void Spring_Update();
void Spring_LateUpdate();
void Spring_StaticUpdate();
void Spring_Draw();
void Spring_Create(void* data);
void Spring_StageLoad();
void Spring_EditorDraw();
void Spring_EditorLoad();
void Spring_Serialize();

// Extra Entity Functions
void Spring_State_Vertical();
void Spring_State_Horizontal();
void Spring_State_Diagonal();

#endif //!OBJ_SPRING_H

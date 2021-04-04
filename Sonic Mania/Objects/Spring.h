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
    void (*state)(void);
    int type;
    int flipFlag;
    byte planeFilter;
    char gap65;
    char field_66;
    char field_67;
    int timer;
    AnimationData data;
    Hitbox hitbox;
} EntitySpring;

// Object Struct
extern ObjectSpring *Spring;

// Standard Entity Events
void Spring_Update(void);
void Spring_LateUpdate(void);
void Spring_StaticUpdate(void);
void Spring_Draw(void);
void Spring_Create(void* data);
void Spring_StageLoad(void);
void Spring_EditorDraw(void);
void Spring_EditorLoad(void);
void Spring_Serialize(void);

// Extra Entity Functions
void Spring_State_Vertical(void);
void Spring_State_Horizontal(void);
void Spring_State_Diagonal(void);

#endif //!OBJ_SPRING_H

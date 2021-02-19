#ifndef OBJ_SPRING_H
#define OBJ_SPRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpring : Object {
    ushort spriteIndex;
    ushort sfx_Spring;
};

// Entity Class
struct EntitySpring : Entity {
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
};

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
void Spring_Unknown1();
void Spring_Unknown2();
void Spring_Unknown3();

#endif //!OBJ_SPRING_H

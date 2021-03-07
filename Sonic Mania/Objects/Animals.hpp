#ifndef OBJ_ANIMALS_H
#define OBJ_ANIMALS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAnimals : Object {
    int hitboxes[12];
    int gravityStrength[12];
    int yVelocity[12];
    int xVelocity[12];
    ushort spriteIndex;
    int animalType1;
    int animalType2;
    bool32 hasPlatform;
    bool32 hasBridge;
};

// Entity Class
struct EntityAnimals : Entity {
    void(*state)();
    int delay;
    int type;
    int behaviour;
    EntityAnimationData data;
    Hitbox hitbox;
};

// Object Struct
extern ObjectAnimals *Animals;

// Standard Entity Events
void Animals_Update();
void Animals_LateUpdate();
void Animals_StaticUpdate();
void Animals_Draw();
void Animals_Create(void* data);
void Animals_StageLoad();
void Animals_EditorDraw();
void Animals_EditorLoad();
void Animals_Serialize();

// Extra Entity Functions


#endif //!OBJ_ANIMALS_H

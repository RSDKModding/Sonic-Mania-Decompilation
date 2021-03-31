#ifndef OBJ_ANIMALS_H
#define OBJ_ANIMALS_H

#include "../SonicMania.h" 

typedef enum {
    ANIMAL_FLICKY,
    ANIMAL_RICKY,
    ANIMAL_POCKY,
    ANIMAL_PECKY,
    ANIMAL_PICKY,
    ANIMAL_CUCKY,
    ANIMAL_ROCKY,
    ANIMAL_BECKY,
    ANIMAL_LOCKY,
    ANIMAL_TOCKY,
    ANIMAL_WOCKY,
    ANIMAL_MICKY,
} ANIMAL_TYPES;

// Object Class
typedef struct {
    RSDK_OBJECT
    int hitboxes[12];
    int gravityStrength[12];
    int yVelocity[12];
    int xVelocity[12];
    ushort spriteIndex;
    int animalTypes[2];
    bool32 hasPlatform;
    bool32 hasBridge;
} ObjectAnimals;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int delay;
    int type;
    int behaviour;
    AnimationData data;
    Hitbox hitbox;
} EntityAnimals;

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
void Animals_CheckPlayerPos();
bool32 Animals_CheckPlatformCollision(void *plat);
bool32 Animals_CheckGroundCollision();

void Animals_State_Freed();
void Animals_State_FollowPlayer_Normal();
void Animals_State_FollowPlayer_Bird();
void Animals_State_BounceAround();
#endif //!OBJ_ANIMALS_H

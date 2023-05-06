#ifndef OBJ_ANIMALS_H
#define OBJ_ANIMALS_H

#include "Game.h"

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
} AnimalTypes;

typedef enum {
    ANIMAL_BEHAVE_FREE,
    ANIMAL_BEHAVE_FOLLOW,
    ANIMAL_BEHAVE_FIXED,
} AnimalBehaviours;

// Object Class
struct ObjectAnimals {
    RSDK_OBJECT
    TABLE(int32 hitboxes[12], { 0x70000, 0x70000, 0xC0000, 0xA0000, 0x80000, 0x80000, 0x80000, 0x80000, 0x70000, 0x50000, 0x70000, 0x60000 });
    TABLE(int32 gravityStrength[12], { 0x1800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x3800, 0x1800, 0x3800, 0x3800, 0x3800 });
    TABLE(int32 yVelocity[12],
          { -0x40000, -0x38000, -0x40000, -0x30000, -0x30000, -0x30000, -0x18000, -0x30000, -0x30000, -0x20000, -0x30000, -0x38000 });
    TABLE(int32 xVelocity[12],
          { -0x30000, -0x28000, -0x20000, -0x18000, -0x1C000, -0x20000, -0x14000, -0x20000, -0x28000, -0x14000, -0x2C000, -0x20000 });
    uint16 aniFrames;
    int32 animalTypes[2];
    bool32 hasPlatform;
    bool32 hasBridge;
};

// Entity Class
struct EntityAnimals {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    AnimalTypes type;
    AnimalBehaviours behaviour;
    Animator animator;
    Hitbox hitboxAnimal;
};

// Object Struct
extern ObjectAnimals *Animals;

// Standard Entity Events
void Animals_Update(void);
void Animals_LateUpdate(void);
void Animals_StaticUpdate(void);
void Animals_Draw(void);
void Animals_Create(void *data);
void Animals_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Animals_EditorDraw(void);
void Animals_EditorLoad(void);
#endif
void Animals_Serialize(void);

// Extra Entity Functions
void Animals_CheckDirection(void);
bool32 Animals_CheckPlatformCollision(void *platform);
bool32 Animals_CheckGroundCollision(void);

void Animals_State_Fall(void);
void Animals_State_Bounce(void);
void Animals_State_Fly(void);
void Animals_State_Placed(void);
#endif //! OBJ_ANIMALS_H

#ifndef OBJ_ANIMALS_H
#define OBJ_ANIMALS_H

#include "SonicMania.h" 

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
    StateMachine(state);
    int delay;
    int type;
    int behaviour;
    Animator animator;
    Hitbox hitbox;
} EntityAnimals;

// Object Struct
extern ObjectAnimals *Animals;

// Standard Entity Events
void Animals_Update(void);
void Animals_LateUpdate(void);
void Animals_StaticUpdate(void);
void Animals_Draw(void);
void Animals_Create(void* data);
void Animals_StageLoad(void);
void Animals_EditorDraw(void);
void Animals_EditorLoad(void);
void Animals_Serialize(void);

// Extra Entity Functions
void Animals_CheckPlayerPos(void);
bool32 Animals_CheckPlatformCollision(void *plat);
bool32 Animals_CheckGroundCollision(void);

void Animals_State_Freed(void);
void Animals_State_FollowPlayer_Normal(void);
void Animals_State_FollowPlayer_Bird(void);
void Animals_State_BounceAround(void);
#endif //!OBJ_ANIMALS_H

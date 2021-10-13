#ifndef OBJ_EGGPRISON_H
#define OBJ_EGGPRISON_H

#include "SonicMania.h"

typedef enum {
    EGGPRISON_NORMAL,
    EGGPRISON_FLYING,
    EGGPRISON_DUD,
    EGGPRISON_RINGS,
    EGGPRISON_TRAP,
    EGGPRISON_ANIMALS,
}EggPrisonTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    uint16 sfxDestroy;
    uint16 sfxSpring;
} ObjectEggPrison;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    EggPrisonTypes type;
    int timer;
    int originY;
    int buttonPos;
    int activated;
    int field_70;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityEggPrison;

// Object Struct
extern ObjectEggPrison *EggPrison;

// Standard Entity Events
void EggPrison_Update(void);
void EggPrison_LateUpdate(void);
void EggPrison_StaticUpdate(void);
void EggPrison_Draw(void);
void EggPrison_Create(void* data);
void EggPrison_StageLoad(void);
void EggPrison_EditorDraw(void);
void EggPrison_EditorLoad(void);
void EggPrison_Serialize(void);

// Extra Entity Functions
void EggPrison_HandleMovement(void);
void EggPrison_Activated(void);
void EggPrison_Unknown1(void);
void EggPrison_Unknown2(void);
void EggPrison_Unknown3(void);
void EggPrison_Unknown4(void);
void EggPrison_Unknown5(void);

#endif //!OBJ_EGGPRISON_H

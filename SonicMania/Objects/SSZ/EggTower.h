#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

#include "Game.h"

// Object Class
struct ObjectEggTower {
    RSDK_OBJECT
    uint16 modelIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityEggTower {
    RSDK_ENTITY
    StateMachine(state); // unused
    int32 unused2;
    int32 rotationX;
    Matrix unusedMatrix1;
    Matrix matWorld;
    Matrix matTemp;
    Matrix unusedMatrix2;
};

// Object Struct
extern ObjectEggTower *EggTower;

// Standard Entity Events
void EggTower_Update(void);
void EggTower_LateUpdate(void);
void EggTower_StaticUpdate(void);
void EggTower_Draw(void);
void EggTower_Create(void *data);
void EggTower_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggTower_EditorDraw(void);
void EggTower_EditorLoad(void);
#endif
void EggTower_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_EGGTOWER_H

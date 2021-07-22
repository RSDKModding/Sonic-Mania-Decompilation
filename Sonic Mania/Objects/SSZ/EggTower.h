#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort modelIndex;
    ushort sceneIndex;
} ObjectEggTower;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int field_5C;
    int rotationX;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
} EntityEggTower;

// Object Struct
extern ObjectEggTower *EggTower;

// Standard Entity Events
void EggTower_Update(void);
void EggTower_LateUpdate(void);
void EggTower_StaticUpdate(void);
void EggTower_Draw(void);
void EggTower_Create(void* data);
void EggTower_StageLoad(void);
void EggTower_EditorDraw(void);
void EggTower_EditorLoad(void);
void EggTower_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGTOWER_H

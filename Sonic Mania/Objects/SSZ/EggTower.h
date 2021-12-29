#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

#include "SonicMania.h"

// Object Class
struct ObjectEggTower {
    RSDK_OBJECT
    uint16 modelIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityEggTower {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 rotationX;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
};

// Object Struct
extern ObjectEggTower *EggTower;

// Standard Entity Events
void EggTower_Update(void);
void EggTower_LateUpdate(void);
void EggTower_StaticUpdate(void);
void EggTower_Draw(void);
void EggTower_Create(void* data);
void EggTower_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void EggTower_EditorDraw(void);
void EggTower_EditorLoad(void);
#endif
void EggTower_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_EGGTOWER_H

#ifndef OBJ_BALLCANNON_H
#define OBJ_BALLCANNON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBallCannon : Object {
    colour value1;
    colour value2;
    colour value3;
    ushort value4;
    int value5[32]; //= { -786432, -786432, -262144, -786432, 262144, -786432, 786432, -786432, -786432, -262144, -262144, -262144, 262144, -262144, 786432, -262144, -786432, 262144, -262144, 262144, 262144, 262144, 786432, 262144, -786432, 786432, -262144, 786432, 262144, 786432, 786432, 786432 };
    int value6[32]; //= { -262144, -262144, -131072, -262144, 131072, -262144, 262144, -262144, -245760, -131072, -114688, -131072, 114688, -131072, 245760, -131072, -229376, 131072, -98304, 131072, 98304, 131072, 229376, 131072, -212992, 262144, -81920, 262144, 81920, 262144, 212992, 262144 };
    ushort value7;
    ushort value8;
};

// Entity Class
struct EntityBallCannon : Entity {

};

// Object Struct
extern ObjectBallCannon *BallCannon;

// Standard Entity Events
void BallCannon_Update();
void BallCannon_LateUpdate();
void BallCannon_StaticUpdate();
void BallCannon_Draw();
void BallCannon_Create(void* data);
void BallCannon_StageLoad();
void BallCannon_EditorDraw();
void BallCannon_EditorLoad();
void BallCannon_Serialize();

// Extra Entity Functions


#endif //!OBJ_BALLCANNON_H

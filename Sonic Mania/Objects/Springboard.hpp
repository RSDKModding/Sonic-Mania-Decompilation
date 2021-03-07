#ifndef OBJ_SPRINGBOARD_H
#define OBJ_SPRINGBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpringboard : Object {
    int value1[3]; //= { -262144, -655360, -524288 };
    int value2[40]; //= { 0, 0, 0, 0, 0, 0, 0, 0, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -65536, -131072, -131072, -131072, -131072, -131072, -131072, -131072, -131072, -196608, -196608, -196608, -196608, -196608, -196608, -262144, -262144 };
    int value3[28]; //= { 8, 9, 10, 11, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 20, 21, 21, 22, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24 };
    int value4[28]; //= { 8, 9, 10, 11, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 14, 14, 15, 15, 16, 16, 16, 16, 15, 15, 14, 14, 13, 13 };
    ushort value5;
    ushort value6;
};

// Entity Class
struct EntitySpringboard : Entity {

};

// Object Struct
extern ObjectSpringboard *Springboard;

// Standard Entity Events
void Springboard_Update();
void Springboard_LateUpdate();
void Springboard_StaticUpdate();
void Springboard_Draw();
void Springboard_Create(void* data);
void Springboard_StageLoad();
void Springboard_EditorDraw();
void Springboard_EditorLoad();
void Springboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPRINGBOARD_H

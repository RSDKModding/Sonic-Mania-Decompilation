#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBigSqueeze : Object {
    int value1[10]; //= { 3, 12, -131072, -65536, 13, 0, -65536, 14, 131072, -65536 };
    int value2[13]; //= { 4, 8, -131072, -65536, 9, -65536, -65536, 10, 65536, -65536, 11, 131072, -65536 };
    int value3[25]; //= { 8, 0, -131072, -65536, 1, -65536, -65536, 2, 65536, -65536, 3, 131072, -65536, 4, -131072, -32768, 5, -65536, -32768, 6, 65536, -32768, 7, 131072, -32768 };
    int value4;
    byte value5;
    int value6;
    int value7;
    ushort value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
};

// Entity Class
struct EntityBigSqueeze : Entity {

};

// Object Struct
extern ObjectBigSqueeze *BigSqueeze;

// Standard Entity Events
void BigSqueeze_Update();
void BigSqueeze_LateUpdate();
void BigSqueeze_StaticUpdate();
void BigSqueeze_Draw();
void BigSqueeze_Create(void* data);
void BigSqueeze_StageLoad();
void BigSqueeze_EditorDraw();
void BigSqueeze_EditorLoad();
void BigSqueeze_Serialize();

// Extra Entity Functions


#endif //!OBJ_BIGSQUEEZE_H

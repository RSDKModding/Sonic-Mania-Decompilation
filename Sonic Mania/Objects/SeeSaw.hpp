#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSeeSaw : Object {
    int value1[48]; //= { -36, -36, -38, -40, -42, -44, -42, -40, -38, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -2, -2, -2, -2 };
    TextInfo value2;
    int value3;
    colour value4;
    colour value5;
    ushort value6;
    ushort value7;
};

// Entity Class
struct EntitySeeSaw : Entity {

};

// Object Struct
extern ObjectSeeSaw *SeeSaw;

// Standard Entity Events
void SeeSaw_Update();
void SeeSaw_LateUpdate();
void SeeSaw_StaticUpdate();
void SeeSaw_Draw();
void SeeSaw_Create(void* data);
void SeeSaw_StageLoad();
void SeeSaw_EditorDraw();
void SeeSaw_EditorLoad();
void SeeSaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_SEESAW_H

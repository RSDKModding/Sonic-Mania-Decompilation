#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoBean : Object {
    int value1[5]; //= { 16, 12, 8, 4, 2 };
    colour value2;
    int value3;
    byte value4;
    bool32 value5;
    int value6;
    TextInfo value7;
    byte value8;
    int value9;
    EntityAnimationData value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
    ushort value15;
    ushort value16;
};

// Entity Class
struct EntityPuyoBean : Entity {

};

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update();
void PuyoBean_LateUpdate();
void PuyoBean_StaticUpdate();
void PuyoBean_Draw();
void PuyoBean_Create(void* data);
void PuyoBean_StageLoad();
void PuyoBean_EditorDraw();
void PuyoBean_EditorLoad();
void PuyoBean_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOBEAN_H

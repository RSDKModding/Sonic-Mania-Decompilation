#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWater : Object {
    int value1;
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
    int value7;
    int value8[18]; //= { 2, 4, 2, 2, 2, 2, 4, 2, 4, 2, 2, 4, 2, 4, 2, 2, 4, 2 };
    ushort value9;
    ushort value10;
    ushort value11;
    colour value12;
    colour value13;
    ushort value14;
    ushort value15;
    ushort value16;
    ushort value17;
    ushort value18;
    ushort value19;
    ushort value20;
    ushort value21;
    ushort value22;
    ushort value23;
    int value24;
    int value25;
    int value26;
    byte value27;
    byte value28;
    int value29;
    byte value30;
    byte value31;
    int value32;
    byte value33;
    EntityAnimationData value34;
    byte value35;
    byte value36;
};

// Entity Class
struct EntityWater : Entity {

};

// Object Struct
extern ObjectWater *Water;

// Standard Entity Events
void Water_Update();
void Water_LateUpdate();
void Water_StaticUpdate();
void Water_Draw();
void Water_Create(void* data);
void Water_StageLoad();
void Water_EditorDraw();
void Water_EditorLoad();
void Water_Serialize();

// Extra Entity Functions


#endif //!OBJ_WATER_H

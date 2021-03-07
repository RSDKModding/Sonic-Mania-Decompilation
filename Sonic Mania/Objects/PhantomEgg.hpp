#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomEgg : Object {
    int value1[29]; //= { 4, 0, 0, -131072, -163840, 1, 0, -131072, -163840, 2, 0, 131072, -131072, 3, 0, 163840, -131072, 4, 0, -65536, -65536, 5, 0, 65536, -65536, 6, 0, -32768, -65536 };
    int value2[32]; //= { 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 4, 0, 0, 1, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value3[32]; //= { 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value4[32]; //= { 0, 0, 2, 0, 1, 3, 3, 2, 0, 3, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 };
    int value5;
    int value6;
    int value7;
    int value8;
    int value9;
    int value10;
    int value11;
    int value12;
    int value13;
    int value14;
    int value15;
    byte value16;
    byte value17;
    ushort value18;
    ushort value19;
    ushort value20;
    ushort value21;
    ushort value22;
    ushort value23;
    ushort value24;
    ushort value25;
    ushort value26;
    ushort value27;
    ushort value28;
    ushort value29;
    byte value30;
};

// Entity Class
struct EntityPhantomEgg : Entity {

};

// Object Struct
extern ObjectPhantomEgg *PhantomEgg;

// Standard Entity Events
void PhantomEgg_Update();
void PhantomEgg_LateUpdate();
void PhantomEgg_StaticUpdate();
void PhantomEgg_Draw();
void PhantomEgg_Create(void* data);
void PhantomEgg_StageLoad();
void PhantomEgg_EditorDraw();
void PhantomEgg_EditorLoad();
void PhantomEgg_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMEGG_H

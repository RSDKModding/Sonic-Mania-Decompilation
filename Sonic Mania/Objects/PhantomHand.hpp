#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomHand : Object {
    int value1[41]; //= { 10, 0, 0, -131072, -65536, 1, 0, -65536, -65536, 2, 0, 65536, -65536, 3, 0, 131072, -65536, 4, 0, -131072, -32768, 5, 0, -65536, -32768, 6, 0, 65536, -32768, 7, 0, 131072, 32768, 8, 0, 131072, -32768, 9, 0, 131072, -16384 };
    byte value2;
    colour value3;
    ushort value4;
};

// Entity Class
struct EntityPhantomHand : Entity {

};

// Object Struct
extern ObjectPhantomHand *PhantomHand;

// Standard Entity Events
void PhantomHand_Update();
void PhantomHand_LateUpdate();
void PhantomHand_StaticUpdate();
void PhantomHand_Draw();
void PhantomHand_Create(void* data);
void PhantomHand_StageLoad();
void PhantomHand_EditorDraw();
void PhantomHand_EditorLoad();
void PhantomHand_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMHAND_H

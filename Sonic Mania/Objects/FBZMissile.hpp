#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFBZMissile : Object {
    colour value1;
    colour value2;
    colour value3;
    int value4[8]; //= { 65536, 57344, 73728, 81920, 65536, 57344, 49152, 57344 };
    int value5;
    ushort value6;
    ushort value7;
    ushort value8;
    ushort value9;
};

// Entity Class
struct EntityFBZMissile : Entity {

};

// Object Struct
extern ObjectFBZMissile *FBZMissile;

// Standard Entity Events
void FBZMissile_Update();
void FBZMissile_LateUpdate();
void FBZMissile_StaticUpdate();
void FBZMissile_Draw();
void FBZMissile_Create(void* data);
void FBZMissile_StageLoad();
void FBZMissile_EditorDraw();
void FBZMissile_EditorLoad();
void FBZMissile_Serialize();

// Extra Entity Functions


#endif //!OBJ_FBZMISSILE_H

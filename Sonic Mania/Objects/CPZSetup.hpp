#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCPZSetup : Object {
    ushort value1;
    bool32 value2;
    int value3[64]; //= { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3, 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 };
    int value4;
    int value5;
    int value6;
    int value7;
    int value8;
    int value9;
    bool32 value10;
};

// Entity Class
struct EntityCPZSetup : Entity {

};

// Object Struct
extern ObjectCPZSetup *CPZSetup;

// Standard Entity Events
void CPZSetup_Update();
void CPZSetup_LateUpdate();
void CPZSetup_StaticUpdate();
void CPZSetup_Draw();
void CPZSetup_Create(void* data);
void CPZSetup_StageLoad();
void CPZSetup_EditorDraw();
void CPZSetup_EditorLoad();
void CPZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_CPZSETUP_H

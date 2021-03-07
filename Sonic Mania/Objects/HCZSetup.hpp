#ifndef OBJ_HCZSETUP_H
#define OBJ_HCZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHCZSetup : Object {
    int value1[14]; //= { 4, 3, 2, 2, 2, 3, 3, 4, 3, 2, 2, 2, 3, 3 };
    int value2;
    int value3;
    int value4;
    int value5;
    ushort value6;
    ushort value7;
    ushort value8;
    bool32 value9;
    int value10;
    byte value11;
    int value12;
    ushort value13;
    ushort value14;
};

// Entity Class
struct EntityHCZSetup : Entity {

};

// Object Struct
extern ObjectHCZSetup *HCZSetup;

// Standard Entity Events
void HCZSetup_Update();
void HCZSetup_LateUpdate();
void HCZSetup_StaticUpdate();
void HCZSetup_Draw();
void HCZSetup_Create(void* data);
void HCZSetup_StageLoad();
void HCZSetup_EditorDraw();
void HCZSetup_EditorLoad();
void HCZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_HCZSETUP_H

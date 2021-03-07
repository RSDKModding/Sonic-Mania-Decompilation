#ifndef OBJ_OOZSETUP_H
#define OBJ_OOZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOOZSetup : Object {
    int value1;
    ushort value2;
    bool32 value3;
    int value4[64]; //= { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3, 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 };
    int value5;
    int value6;
    int value7; //= 60;
    int value8[9]; //= { 60, 60, 3, 3, 3, 3, 3, 3, 4 };
    int value9;
    int value10;
    byte value11;
    byte value12;
    bool32 value13;
    TextInfo value14;
    ushort value15;
    byte value16;
    unknown value17;
    unknown value18;
    ushort value19;
    ushort value20;
    byte value21;
    bool32 value22;
};

// Entity Class
struct EntityOOZSetup : Entity {

};

// Object Struct
extern ObjectOOZSetup *OOZSetup;

// Standard Entity Events
void OOZSetup_Update();
void OOZSetup_LateUpdate();
void OOZSetup_StaticUpdate();
void OOZSetup_Draw();
void OOZSetup_Create(void* data);
void OOZSetup_StageLoad();
void OOZSetup_EditorDraw();
void OOZSetup_EditorLoad();
void OOZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_OOZSETUP_H

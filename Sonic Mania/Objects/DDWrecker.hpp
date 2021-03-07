#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDDWrecker : Object {
    int value1;
    int value2;
    int value3;
    int value4;
    int value5;
    int value6;
    int value7;
    int value8;
    int value9[6]; //= { -64, -32, -16, 16, 32, 64 };
    int value10[6]; //= { 32, 48, 64, 80, 96, 128 };
    ushort value11;
    ushort value12;
    ushort value13;
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
    ushort value24;
};

// Entity Class
struct EntityDDWrecker : Entity {

};

// Object Struct
extern ObjectDDWrecker *DDWrecker;

// Standard Entity Events
void DDWrecker_Update();
void DDWrecker_LateUpdate();
void DDWrecker_StaticUpdate();
void DDWrecker_Draw();
void DDWrecker_Create(void* data);
void DDWrecker_StageLoad();
void DDWrecker_EditorDraw();
void DDWrecker_EditorLoad();
void DDWrecker_Serialize();

// Extra Entity Functions


#endif //!OBJ_DDWRECKER_H

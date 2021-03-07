#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMMZSetup : Object {
    int value1;
    int value2; //= 60;
    int value3[12]; //= { 9, 4, 4, 2, 2, 2, 9, 4, 4, 2, 2, 2 };
    byte value4;
    byte value5;
    byte value6;
    byte value7;
    int value8;
    int value9;
    int value10;
    ushort value11;
};

// Entity Class
struct EntityMMZSetup : Entity {

};

// Object Struct
extern ObjectMMZSetup *MMZSetup;

// Standard Entity Events
void MMZSetup_Update();
void MMZSetup_LateUpdate();
void MMZSetup_StaticUpdate();
void MMZSetup_Draw();
void MMZSetup_Create(void* data);
void MMZSetup_StageLoad();
void MMZSetup_EditorDraw();
void MMZSetup_EditorLoad();
void MMZSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_MMZSETUP_H

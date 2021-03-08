#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAmoebaDroid : Object {
    int value1[17]; //= { 4, 0, 0, -131072, -163840, 1, 0, 131072, -163840, 2, 0, -163840, -131072, 3, 0, 163840, -131072 };
    int value2[13]; //= { 3, 4, 0, 0, -131072, 5, 0, -163840, -131072, 5, 1, 163840, -131072 };
    int value3;
    int value4;
    int value5;
    int value6;
    int value7;
    ushort value8;
    ushort value9;
    ushort value10;
    ushort value11;
    ushort value12;
    ushort value13;
    ushort value14;
};

// Entity Class
struct EntityAmoebaDroid : Entity {

};

// Object Struct
extern ObjectAmoebaDroid *AmoebaDroid;

// Standard Entity Events
void AmoebaDroid_Update();
void AmoebaDroid_LateUpdate();
void AmoebaDroid_StaticUpdate();
void AmoebaDroid_Draw();
void AmoebaDroid_Create(void* data);
void AmoebaDroid_StageLoad();
void AmoebaDroid_EditorDraw();
void AmoebaDroid_EditorLoad();
void AmoebaDroid_Serialize();

// Extra Entity Functions


#endif //!OBJ_AMOEBADROID_H
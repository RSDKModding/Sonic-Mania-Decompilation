#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
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
} ObjectAmoebaDroid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAmoebaDroid;

// Object Struct
extern ObjectAmoebaDroid *AmoebaDroid;

// Standard Entity Events
void AmoebaDroid_Update(void);
void AmoebaDroid_LateUpdate(void);
void AmoebaDroid_StaticUpdate(void);
void AmoebaDroid_Draw(void);
void AmoebaDroid_Create(void* data);
void AmoebaDroid_StageLoad(void);
void AmoebaDroid_EditorDraw(void);
void AmoebaDroid_EditorLoad(void);
void AmoebaDroid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AMOEBADROID_H

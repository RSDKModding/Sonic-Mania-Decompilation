#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[56]; //= { 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 53, 52, 52, 52, 52, 51, 51, 51, 50, 50, 50, 50, 49, 49, 48, 48 };
    byte value2;
    byte value3;
    byte value4;
    byte value5;
    byte value6;
    byte value7;
    byte value8;
    byte value9;
    byte value10;
    byte value11;
    byte value12;
    byte value13;
    byte value14;
    short value15;
    byte value16;
    byte value17;
    byte value18;
    byte value19;
    byte value20;
    byte value21;
    byte value22;
    byte value23;
    int value24;
    int value25;
    byte value26;
    byte value27;
    bool32 value28;
    ushort value29;
    ushort value30;
    colour value31;
    colour value32;
    colour value33;
    colour value34;
    colour value35;
    colour value36;
    ushort value37;
    ushort value38;
    ushort value39;
    ushort value40;
    ushort value41;
    ushort value42;
} ObjectGachapandora;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGachapandora;

// Object Struct
extern ObjectGachapandora *Gachapandora;

// Standard Entity Events
void Gachapandora_Update(void);
void Gachapandora_LateUpdate(void);
void Gachapandora_StaticUpdate(void);
void Gachapandora_Draw(void);
void Gachapandora_Create(void* data);
void Gachapandora_StageLoad(void);
void Gachapandora_EditorDraw(void);
void Gachapandora_EditorLoad(void);
void Gachapandora_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GACHAPANDORA_H

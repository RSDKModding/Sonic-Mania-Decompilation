#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[56], { 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53,
                            54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 53, 52, 52, 52, 52, 51, 51, 51, 50, 50, 50, 50, 49, 49, 48, 48 });
    uint8 value2;
    uint8 value3;
    uint8 value4;
    uint8 value5;
    uint8 value6;
    uint8 value7;
    uint8 value8;
    uint8 value9;
    uint8 value10;
    uint8 value11;
    uint8 value12;
    uint8 value13;
    uint8 value14;
    int16 value15;
    uint8 value16;
    uint8 value17;
    uint8 value18;
    uint8 value19;
    uint8 value20;
    uint8 value21;
    uint8 value22;
    uint8 value23;
    int32 value24;
    int32 value25;
    uint8 value26;
    uint8 value27;
    bool32 value28;
    uint16 value29;
    uint16 value30;
    Hitbox value31;
    Hitbox value32;
    Hitbox value33;
    Hitbox value34;
    Hitbox value35;
    Hitbox value36;
    uint16 value37;
    uint16 value38;
    uint16 value39;
    uint16 value40;
    uint16 value41;
    uint16 value42;
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

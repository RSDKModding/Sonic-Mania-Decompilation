#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    colour coloursManiaA[8];  //= { 32, 32, 4247680, 4247680, 4247680, 4247680, 32, 32 };
    colour coloursManiaB[8];  //= { 32, 32, 15225048, 15225048, 15225048, 15225048, 32, 32 };
    colour coloursEncoreA[8]; //= { 32, 32, 13637704, 13637704, 13637704, 13637704, 32, 32 };
    colour coloursEncoreB[8]; //= { 32, 32, 13676568, 13676568, 13676568, 13676568, 32, 32 };
    ushort spriteIndex;
} ObjectSSZSpotlight;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte color;
    byte size;
    byte drawFlag;
    byte speed;
    byte offset;
    byte flashSpeed;
    Vector2 offsetPos;
    Vector2 vertStore[8];
    Vector2 vertPos[8];
    colour *vertClrPtrs;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
} EntitySSZSpotlight;

// Object Struct
extern ObjectSSZSpotlight *SSZSpotlight;

// Standard Entity Events
void SSZSpotlight_Update(void);
void SSZSpotlight_LateUpdate(void);
void SSZSpotlight_StaticUpdate(void);
void SSZSpotlight_Draw(void);
void SSZSpotlight_Create(void* data);
void SSZSpotlight_StageLoad(void);
void SSZSpotlight_EditorDraw(void);
void SSZSpotlight_EditorLoad(void);
void SSZSpotlight_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZSPOTLIGHT_H

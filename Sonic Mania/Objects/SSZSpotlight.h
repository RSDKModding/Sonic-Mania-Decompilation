#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[8]; //= { 32, 32, 4247680, 4247680, 4247680, 4247680, 32, 32 };
    int value2[8]; //= { 32, 32, 15225048, 15225048, 15225048, 15225048, 32, 32 };
    int value3[8]; //= { 32, 32, 13637704, 13637704, 13637704, 13637704, 32, 32 };
    int value4[8]; //= { 32, 32, 13676568, 13676568, 13676568, 13676568, 32, 32 };
    ushort value5;
} ObjectSSZSpotlight;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZSpotlight;

// Object Struct
extern ObjectSSZSpotlight *SSZSpotlight;

// Standard Entity Events
void SSZSpotlight_Update();
void SSZSpotlight_LateUpdate();
void SSZSpotlight_StaticUpdate();
void SSZSpotlight_Draw();
void SSZSpotlight_Create(void* data);
void SSZSpotlight_StageLoad();
void SSZSpotlight_EditorDraw();
void SSZSpotlight_EditorLoad();
void SSZSpotlight_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZSPOTLIGHT_H

#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(colour coloursManiaA[8], { 0x000020, 0x000020, 0x40D080, 0x40D080, 0x40D080, 0x40D080, 0x000020, 0x000020 });
    TABLE(colour coloursManiaB[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
#if RETRO_USE_PLUS
    TABLE(colour coloursEncoreA[8], { 0x000020, 0x000020, 0xD01848, 0xD01848, 0xD01848, 0xD01848, 0x000020, 0x000020 });
    TABLE(colour coloursEncoreB[8], { 0x000020, 0x000020, 0xD0B018, 0xD0B018, 0xD0B018, 0xD0B018, 0x000020, 0x000020 });
#endif
    uint16 spriteIndex;
} ObjectSSZSpotlight;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 color;
    uint8 size;
    uint8 drawFlag;
    uint8 speed;
    uint8 offset;
    uint8 flashSpeed;
    Vector2 offsetPos;
    Vector2 vertStore[8];
    Vector2 vertPos[8];
    colour *vertClrPtrs;
    int32 field_EC;
    int32 field_F0;
    int32 field_F4;
    int32 field_F8;
    int32 field_FC;
    int32 field_100;
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
#if RETRO_INCLUDE_EDITOR
void SSZSpotlight_EditorDraw(void);
void SSZSpotlight_EditorLoad(void);
#endif
void SSZSpotlight_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZSPOTLIGHT_H

#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "Game.h"

typedef enum {
    SSZSPOTLIGHT_CLR_CYAN,
    SSZSPOTLIGHT_CLR_PINK,
} SSZSpotlightColors;

typedef enum {
    SSZSPOTLIGHT_SIZE_SMALL,
    SSZSPOTLIGHT_SIZE_MED,
    SSZSPOTLIGHT_SIZE_LARGE,
} SSZSpotlightSizes;

typedef enum {
    SSZSPOTLIGHT_DRAW_LOW,
    SSZSPOTLIGHT_DRAW_HIGH,
    SSZSPOTLIGHT_DRAW_LOWER,
} SSZSpotlightDrawFlags;

// Object Class
struct ObjectSSZSpotlight {
    RSDK_OBJECT
    TABLE(color cyanSpotlightColors[8], { 0x000020, 0x000020, 0x40D080, 0x40D080, 0x40D080, 0x40D080, 0x000020, 0x000020 });
    TABLE(color pinkSpotlightColors[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
#if MANIA_USE_PLUS
    TABLE(color redSpotlightColors[8], { 0x000020, 0x000020, 0xD01848, 0xD01848, 0xD01848, 0xD01848, 0x000020, 0x000020 });
    TABLE(color yellowSpotlightColors[8], { 0x000020, 0x000020, 0xD0B018, 0xD0B018, 0xD0B018, 0xD0B018, 0x000020, 0x000020 });
#endif
    uint16 aniFrames;
};

// Entity Class
struct EntitySSZSpotlight {
    RSDK_ENTITY
    uint8 color;
    uint8 size;
    uint8 drawFlag;
    uint8 speed;
    uint8 offset;
    uint8 flashSpeed;
    Vector2 originPos;
    Vector2 spotlightVertices[8];
    Vector2 drawVertices[8];
    color *colorTable;
    Animator animator;
};

// Object Struct
extern ObjectSSZSpotlight *SSZSpotlight;

// Standard Entity Events
void SSZSpotlight_Update(void);
void SSZSpotlight_LateUpdate(void);
void SSZSpotlight_StaticUpdate(void);
void SSZSpotlight_Draw(void);
void SSZSpotlight_Create(void *data);
void SSZSpotlight_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZSpotlight_EditorDraw(void);
void SSZSpotlight_EditorLoad(void);
#endif
void SSZSpotlight_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_SSZSPOTLIGHT_H

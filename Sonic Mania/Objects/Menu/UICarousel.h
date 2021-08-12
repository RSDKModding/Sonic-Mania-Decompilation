#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
} ObjectUICarousel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 shift;
    Vector2 clipSize;
    Vector2 clipOffset;
    int field_70;
    int field_74;
    int field_78;
    Entity *field_7C;
    char field_80;
    char field_81;
    char field_82;
    char field_83;
    int scrollOffset;
    int virtualIndex;
    int field_8C;
    Vector2 startPos;
    int field_98;
    int minOffset;
    int maxOffset;
    Vector2 offset;
    Entity *parent;
} EntityUICarousel;

// Object Struct
extern ObjectUICarousel *UICarousel;

// Standard Entity Events
void UICarousel_Update(void);
void UICarousel_LateUpdate(void);
void UICarousel_StaticUpdate(void);
void UICarousel_Draw(void);
void UICarousel_Create(void* data);
void UICarousel_StageLoad(void);
void UICarousel_EditorDraw(void);
void UICarousel_EditorLoad(void);
void UICarousel_Serialize(void);

// Extra Entity Functions
void UICarousel_Unknown1(void);
void UICarousel_Unknown2(void);

#endif

#endif //!OBJ_UICAROUSEL_H

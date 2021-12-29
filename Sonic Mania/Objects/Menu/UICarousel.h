#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
struct ObjectUICarousel {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
};

// Entity Class
struct EntityUICarousel {
    RSDK_ENTITY
    Vector2 shift;
    Vector2 clipSize;
    Vector2 clipOffset;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Entity *field_7C;
    char field_80;
    char field_81;
    char field_82;
    char field_83;
    int32 scrollOffset;
    int32 virtualIndex;
    int32 field_8C;
    Vector2 startPos;
    int32 field_98;
    int32 minOffset;
    int32 maxOffset;
    Vector2 offset;
    Entity *parent;
};

// Object Struct
extern ObjectUICarousel *UICarousel;

// Standard Entity Events
void UICarousel_Update(void);
void UICarousel_LateUpdate(void);
void UICarousel_StaticUpdate(void);
void UICarousel_Draw(void);
void UICarousel_Create(void* data);
void UICarousel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UICarousel_EditorDraw(void);
void UICarousel_EditorLoad(void);
#endif
void UICarousel_Serialize(void);

// Extra Entity Functions
void UICarousel_Unknown1(void);
void UICarousel_Unknown2(void);

#endif

#endif //!OBJ_UICAROUSEL_H

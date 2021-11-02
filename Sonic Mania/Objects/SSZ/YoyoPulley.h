#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
} ObjectYoyoPulley;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 activePlayers;
    Animator animator1;
    Animator animator3;
    Animator animator2;
    Animator animator4;
    uint8 pullDir;
    int32 length;
    int32 speed;
    int32 field_C8;
    int32 pulleyLength;
    int32 playerTimers[4];
    Vector2 handlePos;
} EntityYoyoPulley;

// Object Struct
extern ObjectYoyoPulley *YoyoPulley;

// Standard Entity Events
void YoyoPulley_Update(void);
void YoyoPulley_LateUpdate(void);
void YoyoPulley_StaticUpdate(void);
void YoyoPulley_Draw(void);
void YoyoPulley_Create(void* data);
void YoyoPulley_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void YoyoPulley_EditorDraw(void);
void YoyoPulley_EditorLoad(void);
#endif
void YoyoPulley_Serialize(void);

// Extra Entity Functions
void YoyoPulley_UpdateHandlePos(void);
void YoyoPulley_DrawSprites(void);
int32 YoyoPulley_GetLength(void);

#endif //!OBJ_YOYOPULLEY_H

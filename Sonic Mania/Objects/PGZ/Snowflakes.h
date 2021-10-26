#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 count;
    int32 field_4;
    uint16 aniFrames;
} ObjectSnowflakes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 field_60;
    Animator animator;
    uint8 animIDs[0x40];
    uint16 frameIDs[0x40];
    Vector2 positions[0x40];
    uint8 flipFlags[0x40];
    uint16 angles[0x40];
} EntitySnowflakes;

// Object Struct
extern ObjectSnowflakes *Snowflakes;

// Standard Entity Events
void Snowflakes_Update(void);
void Snowflakes_LateUpdate(void);
void Snowflakes_StaticUpdate(void);
void Snowflakes_Draw(void);
void Snowflakes_Create(void *data);
void Snowflakes_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Snowflakes_EditorDraw(void);
void Snowflakes_EditorLoad(void);
#endif
void Snowflakes_Serialize(void);

// Extra Entity Functions
Vector2 Snowflakes_Unknown1(int32 id);

#endif //! OBJ_SNOWFLAKES_H

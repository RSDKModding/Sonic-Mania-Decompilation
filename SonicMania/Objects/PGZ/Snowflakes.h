#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "Game.h"

// Object Class
struct ObjectSnowflakes {
    RSDK_OBJECT
    uint8 count;
    int32 unused;
    uint16 aniFrames;
};

// Entity Class
struct EntitySnowflakes {
    RSDK_ENTITY
    int32 unused1;
    int32 unused2;
    int32 unused3;
    Animator animator;
    uint8 animIDs[0x40];
    uint16 frameIDs[0x40];
    Vector2 positions[0x40];
    uint8 priority[0x40];
    uint16 angles[0x40];
};

// Object Struct
extern ObjectSnowflakes *Snowflakes;

// Standard Entity Events
void Snowflakes_Update(void);
void Snowflakes_LateUpdate(void);
void Snowflakes_StaticUpdate(void);
void Snowflakes_Draw(void);
void Snowflakes_Create(void *data);
void Snowflakes_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Snowflakes_EditorDraw(void);
void Snowflakes_EditorLoad(void);
#endif
void Snowflakes_Serialize(void);

// Extra Entity Functions
Vector2 Snowflakes_HandleWrap(int32 id);

#endif //! OBJ_SNOWFLAKES_H

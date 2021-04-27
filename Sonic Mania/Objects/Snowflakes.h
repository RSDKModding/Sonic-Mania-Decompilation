#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    byte count;
    int field_4;
    ushort aniFrames;
} ObjectSnowflakes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int field_5C;
    int field_60;
    Animator animator;
    byte animIDs[0x40];
    ushort frameIDs[0x40];
    Vector2 positions[0x40];
    byte flipFlags[0x40];
    ushort angles[0x40];
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
void Snowflakes_EditorDraw(void);
void Snowflakes_EditorLoad(void);
void Snowflakes_Serialize(void);

// Extra Entity Functions
Vector2 Snowflakes_Unknown1(int id);

#endif //! OBJ_SNOWFLAKES_H

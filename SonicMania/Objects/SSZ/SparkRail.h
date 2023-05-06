#ifndef OBJ_SPARKRAIL_H
#define OBJ_SPARKRAIL_H

#include "Game.h"

// Object Class
struct ObjectSparkRail {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPon;
};

// Entity Class
struct EntitySparkRail {
    RSDK_ENTITY
    Vector2 size;
    Hitbox hitbox;
};

// Object Struct
extern ObjectSparkRail *SparkRail;

// Standard Entity Events
void SparkRail_Update(void);
void SparkRail_LateUpdate(void);
void SparkRail_StaticUpdate(void);
void SparkRail_Draw(void);
void SparkRail_Create(void *data);
void SparkRail_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SparkRail_EditorDraw(void);
void SparkRail_EditorLoad(void);
#endif
void SparkRail_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_SPARKRAIL_H

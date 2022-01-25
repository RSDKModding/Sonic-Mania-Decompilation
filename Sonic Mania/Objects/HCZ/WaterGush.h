#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

#include "SonicMania.h"

// Object Class
struct ObjectWaterGush {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGush;
};

// Entity Class
struct EntityWaterGush {
    RSDK_ENTITY
    StateMachine(state);
    uint8 orientation;
    int32 length;
    int32 speed;
    uint8 activePlayers;
    Vector2 posUnknown1;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    bool32 flag;
    int32 field_84;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectWaterGush *WaterGush;

// Standard Entity Events
void WaterGush_Update(void);
void WaterGush_LateUpdate(void);
void WaterGush_StaticUpdate(void);
void WaterGush_Draw(void);
void WaterGush_Create(void* data);
void WaterGush_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void WaterGush_EditorDraw(void);
void WaterGush_EditorLoad(void);
#endif
void WaterGush_Serialize(void);

// Extra Entity Functions
void WaterGush_SetupHitboxes(void);
void WaterGush_DrawSprites(void);
void WaterGush_Unknown3(void);

#endif //!OBJ_WATERGUSH_H

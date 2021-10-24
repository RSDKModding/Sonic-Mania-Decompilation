#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[5], { 16, 12, 8, 4, 2 });
    Hitbox hitbox;
    int field_20[2];
    int field_28[2];
    Entity *gameState[256];
    int field_430;
    Vector2 field_434[256];
    bool32 field_C34[84];
    int shinkDelay;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxLand;
    uint16 sfxRotate;
    uint16 chainFrames[6];
    uint16 sfxJunk;
    uint16 sfxFall;
} ObjectPuyoBean;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoBean;

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update(void);
void PuyoBean_LateUpdate(void);
void PuyoBean_StaticUpdate(void);
void PuyoBean_Draw(void);
void PuyoBean_Create(void* data);
void PuyoBean_StageLoad(void);
void PuyoBean_EditorDraw(void);
void PuyoBean_EditorLoad(void);
void PuyoBean_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOBEAN_H

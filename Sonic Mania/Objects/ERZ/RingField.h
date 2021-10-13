#ifndef OBJ_RINGFIELD_H
#define OBJ_RINGFIELD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectRingField;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    Vector2 size;
    int32 frequency;
    int32 fluctuation;
    bool32 running;
    int32 field_70;
    Vector2 startPos;
    int32 timer;
    Hitbox hitbox;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
} EntityRingField;

// Object Struct
extern ObjectRingField *RingField;

// Standard Entity Events
void RingField_Update(void);
void RingField_LateUpdate(void);
void RingField_StaticUpdate(void);
void RingField_Draw(void);
void RingField_Create(void* data);
void RingField_StageLoad(void);
void RingField_EditorDraw(void);
void RingField_EditorLoad(void);
void RingField_Serialize(void);

// Extra Entity Functions
void RingField_GetPos(Vector2 *pos);


#endif //!OBJ_RINGFIELD_H

#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox bodyHitbox;
    Hitbox handleHitbox;
    uint16 aniFrames;
    uint16 sfxBloop;
} ObjectSyringe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 type;
    int32 offsetY;
    bool32 activated;
    colour colour;
    uint8 tag;
    Animator animator1;
    Animator animator2;
} EntitySyringe;

// Object Struct
extern ObjectSyringe *Syringe;

// Standard Entity Events
void Syringe_Update(void);
void Syringe_LateUpdate(void);
void Syringe_StaticUpdate(void);
void Syringe_Draw(void);
void Syringe_Create(void* data);
void Syringe_StageLoad(void);
void Syringe_EditorDraw(void);
void Syringe_EditorLoad(void);
void Syringe_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SYRINGE_H

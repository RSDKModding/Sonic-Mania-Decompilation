#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox bodyHitbox;
    Hitbox handleHitbox;
    ushort aniFrames;
    ushort sfxBloop;
} ObjectSyringe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int type;
    int offsetY;
    bool32 activated;
    colour colour;
    byte tag;
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

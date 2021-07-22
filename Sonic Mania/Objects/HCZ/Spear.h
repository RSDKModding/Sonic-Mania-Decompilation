#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxSpear;
} ObjectSpear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    byte orientation;
    int interval;
    int intervalOffset;
    int duration;
    int field_6C;
    Vector2 startPos;
    int field_78;
    int field_7C;
    Hitbox hitbox;
    Animator animator;
    Animator animator2;
} EntitySpear;

// Object Struct
extern ObjectSpear *Spear;

// Standard Entity Events
void Spear_Update(void);
void Spear_LateUpdate(void);
void Spear_StaticUpdate(void);
void Spear_Draw(void);
void Spear_Create(void* data);
void Spear_StageLoad(void);
void Spear_EditorDraw(void);
void Spear_EditorLoad(void);
void Spear_Serialize(void);

// Extra Entity Functions
void Spear_Unknown1(void);


#endif //!OBJ_SPEAR_H

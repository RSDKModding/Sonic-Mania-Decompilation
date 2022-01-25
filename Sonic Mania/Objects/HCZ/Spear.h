#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "SonicMania.h"

// Object Class
struct ObjectSpear {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSpear;
};

// Entity Class
struct EntitySpear {
    RSDK_ENTITY
    int32 field_58;
    uint8 orientation;
    int32 interval;
    int32 intervalOffset;
    int32 duration;
    int32 field_6C;
    Vector2 startPos;
    int32 field_78;
    int32 field_7C;
    Hitbox hitbox;
    Animator animator;
    Animator animator2;
};

// Object Struct
extern ObjectSpear *Spear;

// Standard Entity Events
void Spear_Update(void);
void Spear_LateUpdate(void);
void Spear_StaticUpdate(void);
void Spear_Draw(void);
void Spear_Create(void* data);
void Spear_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Spear_EditorDraw(void);
void Spear_EditorLoad(void);
#endif
void Spear_Serialize(void);

// Extra Entity Functions
void Spear_Unknown1(void);


#endif //!OBJ_SPEAR_H

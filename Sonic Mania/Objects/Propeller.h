#ifndef OBJ_PROPELLER_H
#define OBJ_PROPELLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxFan;
} ObjectPropeller;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Hitbox playerHitbox;
    Hitbox hitbox2;
    Hitbox hitbox;
    bool32 fanEnabled;
    byte activePlayers;
    Animator animator;
} EntityPropeller;

// Object Struct
extern ObjectPropeller *Propeller;

// Standard Entity Events
void Propeller_Update(void);
void Propeller_LateUpdate(void);
void Propeller_StaticUpdate(void);
void Propeller_Draw(void);
void Propeller_Create(void* data);
void Propeller_StageLoad(void);
void Propeller_EditorDraw(void);
void Propeller_EditorLoad(void);
void Propeller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PROPELLER_H

#ifndef OBJ_MAGSPIKEBALL_H
#define OBJ_MAGSPIKEBALL_H

#include "Game.h"

// Object Class
struct ObjectMagSpikeBall {
    RSDK_OBJECT
    Hitbox hitboxSpikeBall;
    uint16 aniFrames;
};

// Entity Class
struct EntityMagSpikeBall {
    RSDK_ENTITY
    Animator animator;
};

// Object Struct
extern ObjectMagSpikeBall *MagSpikeBall;

// Standard Entity Events
void MagSpikeBall_Update(void);
void MagSpikeBall_LateUpdate(void);
void MagSpikeBall_StaticUpdate(void);
void MagSpikeBall_Draw(void);
void MagSpikeBall_Create(void *data);
void MagSpikeBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MagSpikeBall_EditorDraw(void);
void MagSpikeBall_EditorLoad(void);
#endif
void MagSpikeBall_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_MAGSPIKEBALL_H

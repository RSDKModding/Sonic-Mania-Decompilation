#ifndef OBJ_MAGSPIKEBALL_H
#define OBJ_MAGSPIKEBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectMagSpikeBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
} EntityMagSpikeBall;

// Object Struct
extern ObjectMagSpikeBall *MagSpikeBall;

// Standard Entity Events
void MagSpikeBall_Update(void);
void MagSpikeBall_LateUpdate(void);
void MagSpikeBall_StaticUpdate(void);
void MagSpikeBall_Draw(void);
void MagSpikeBall_Create(void* data);
void MagSpikeBall_StageLoad(void);
void MagSpikeBall_EditorDraw(void);
void MagSpikeBall_EditorLoad(void);
void MagSpikeBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MAGSPIKEBALL_H

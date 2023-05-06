#ifndef OBJ_RTELEPORTER_H
#define OBJ_RTELEPORTER_H

#include "Game.h"

// Object Class
struct ObjectRTeleporter {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxTeleporter;
    uint16 unused1;
    uint16 unused2;
    uint16 unused3;
    uint16 sfxExplosion;
    uint16 sfxGoodFuture;
};

// Entity Class
struct EntityRTeleporter {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 originY;
    int32 timer;
    Animator mainAnimator;
    Animator electricAnimator;
};

// Object Struct
extern ObjectRTeleporter *RTeleporter;

// Standard Entity Events
void RTeleporter_Update(void);
void RTeleporter_LateUpdate(void);
void RTeleporter_StaticUpdate(void);
void RTeleporter_Draw(void);
void RTeleporter_Create(void *data);
void RTeleporter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RTeleporter_EditorDraw(void);
void RTeleporter_EditorLoad(void);
#endif
void RTeleporter_Serialize(void);

// Extra Entity Functions
void RTeleporter_State_CheckPlayerCollisions(void);
void RTeleporter_State_Destroyed(void);

void RTeleporter_Draw_Active(void);
void RTeleporter_Draw_Exploding(void);
void RTeleporter_Draw_Destroyed(void);

#endif //! OBJ_RTELEPORTER_H

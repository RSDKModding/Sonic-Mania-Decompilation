#ifndef OBJ_SKYTELEPORTER_H
#define OBJ_SKYTELEPORTER_H

#include "Game.h"

// Object Class
struct ObjectSkyTeleporter {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntitySkyTeleporter {
    RSDK_ENTITY
    StateMachine(state); // unused
    int32 unused1;
    Vector2 startPos;
    int32 unused2;
    int32 timer;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectSkyTeleporter *SkyTeleporter;

// Standard Entity Events
void SkyTeleporter_Update(void);
void SkyTeleporter_LateUpdate(void);
void SkyTeleporter_StaticUpdate(void);
void SkyTeleporter_Draw(void);
void SkyTeleporter_Create(void *data);
void SkyTeleporter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SkyTeleporter_EditorDraw(void);
void SkyTeleporter_EditorLoad(void);
#endif
void SkyTeleporter_Serialize(void);

// Extra Entity Functions
void SkyTeleporter_DrawTeleportBeam(void);

#endif //! OBJ_SKYTELEPORTER_H

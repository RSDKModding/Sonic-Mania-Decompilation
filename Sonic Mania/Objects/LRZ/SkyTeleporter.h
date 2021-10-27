#ifndef OBJ_SKYTELEPORTER_H
#define OBJ_SKYTELEPORTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectSkyTeleporter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    Vector2 startPos;
    int32 field_68;
    int32 timer;
    Hitbox hitbox;
    Animator animator;
} EntitySkyTeleporter;

// Object Struct
extern ObjectSkyTeleporter *SkyTeleporter;

// Standard Entity Events
void SkyTeleporter_Update(void);
void SkyTeleporter_LateUpdate(void);
void SkyTeleporter_StaticUpdate(void);
void SkyTeleporter_Draw(void);
void SkyTeleporter_Create(void* data);
void SkyTeleporter_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SkyTeleporter_EditorDraw(void);
void SkyTeleporter_EditorLoad(void);
#endif
void SkyTeleporter_Serialize(void);

// Extra Entity Functions
void SkyTeleporter_Unknown1(void);

#endif //!OBJ_SKYTELEPORTER_H

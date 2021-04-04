#ifndef OBJ_SKYTELEPORTER_H
#define OBJ_SKYTELEPORTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSkyTeleporter;

// Entity Class
typedef struct {
	RSDK_ENTITY
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
void SkyTeleporter_EditorDraw(void);
void SkyTeleporter_EditorLoad(void);
void SkyTeleporter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SKYTELEPORTER_H

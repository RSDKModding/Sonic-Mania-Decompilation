#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSilverSonic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySilverSonic;

// Object Struct
extern ObjectSilverSonic *SilverSonic;

// Standard Entity Events
void SilverSonic_Update(void);
void SilverSonic_LateUpdate(void);
void SilverSonic_StaticUpdate(void);
void SilverSonic_Draw(void);
void SilverSonic_Create(void* data);
void SilverSonic_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void);
void SilverSonic_EditorLoad(void);
#endif
void SilverSonic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SILVERSONIC_H

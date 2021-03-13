#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

#include "../SonicMania.h"

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
void SilverSonic_Update();
void SilverSonic_LateUpdate();
void SilverSonic_StaticUpdate();
void SilverSonic_Draw();
void SilverSonic_Create(void* data);
void SilverSonic_StageLoad();
void SilverSonic_EditorDraw();
void SilverSonic_EditorLoad();
void SilverSonic_Serialize();

// Extra Entity Functions


#endif //!OBJ_SILVERSONIC_H

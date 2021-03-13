#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSZSpotlight;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSZSpotlight;

// Object Struct
extern ObjectMSZSpotlight *MSZSpotlight;

// Standard Entity Events
void MSZSpotlight_Update();
void MSZSpotlight_LateUpdate();
void MSZSpotlight_StaticUpdate();
void MSZSpotlight_Draw();
void MSZSpotlight_Create(void* data);
void MSZSpotlight_StageLoad();
void MSZSpotlight_EditorDraw();
void MSZSpotlight_EditorLoad();
void MSZSpotlight_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZSPOTLIGHT_H

#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectScrewMobile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityScrewMobile;

// Object Struct
extern ObjectScrewMobile *ScrewMobile;

// Standard Entity Events
void ScrewMobile_Update();
void ScrewMobile_LateUpdate();
void ScrewMobile_StaticUpdate();
void ScrewMobile_Draw();
void ScrewMobile_Create(void* data);
void ScrewMobile_StageLoad();
void ScrewMobile_EditorDraw();
void ScrewMobile_EditorLoad();
void ScrewMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_SCREWMOBILE_H

#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

#include "SonicMania.h"

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
void ScrewMobile_Update(void);
void ScrewMobile_LateUpdate(void);
void ScrewMobile_StaticUpdate(void);
void ScrewMobile_Draw(void);
void ScrewMobile_Create(void* data);
void ScrewMobile_StageLoad(void);
void ScrewMobile_EditorDraw(void);
void ScrewMobile_EditorLoad(void);
void ScrewMobile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SCREWMOBILE_H

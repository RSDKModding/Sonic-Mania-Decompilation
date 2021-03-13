#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTwistingSlide;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTwistingSlide;

// Object Struct
extern ObjectTwistingSlide *TwistingSlide;

// Standard Entity Events
void TwistingSlide_Update();
void TwistingSlide_LateUpdate();
void TwistingSlide_StaticUpdate();
void TwistingSlide_Draw();
void TwistingSlide_Create(void* data);
void TwistingSlide_StageLoad();
void TwistingSlide_EditorDraw();
void TwistingSlide_EditorLoad();
void TwistingSlide_Serialize();

// Extra Entity Functions


#endif //!OBJ_TWISTINGSLIDE_H

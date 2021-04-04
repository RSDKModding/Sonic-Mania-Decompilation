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
void TwistingSlide_Update(void);
void TwistingSlide_LateUpdate(void);
void TwistingSlide_StaticUpdate(void);
void TwistingSlide_Draw(void);
void TwistingSlide_Create(void* data);
void TwistingSlide_StageLoad(void);
void TwistingSlide_EditorDraw(void);
void TwistingSlide_EditorLoad(void);
void TwistingSlide_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TWISTINGSLIDE_H

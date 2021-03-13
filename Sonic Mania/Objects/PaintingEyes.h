#ifndef OBJ_PAINTINGEYES_H
#define OBJ_PAINTINGEYES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPaintingEyes;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPaintingEyes;

// Object Struct
extern ObjectPaintingEyes *PaintingEyes;

// Standard Entity Events
void PaintingEyes_Update();
void PaintingEyes_LateUpdate();
void PaintingEyes_StaticUpdate();
void PaintingEyes_Draw();
void PaintingEyes_Create(void* data);
void PaintingEyes_StageLoad();
void PaintingEyes_EditorDraw();
void PaintingEyes_EditorLoad();
void PaintingEyes_Serialize();

// Extra Entity Functions


#endif //!OBJ_PAINTINGEYES_H

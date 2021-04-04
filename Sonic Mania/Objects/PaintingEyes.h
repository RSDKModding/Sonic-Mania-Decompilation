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
void PaintingEyes_Update(void);
void PaintingEyes_LateUpdate(void);
void PaintingEyes_StaticUpdate(void);
void PaintingEyes_Draw(void);
void PaintingEyes_Create(void* data);
void PaintingEyes_StageLoad(void);
void PaintingEyes_EditorDraw(void);
void PaintingEyes_EditorLoad(void);
void PaintingEyes_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PAINTINGEYES_H

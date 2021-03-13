#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMatryoshkaBom;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMatryoshkaBom;

// Object Struct
extern ObjectMatryoshkaBom *MatryoshkaBom;

// Standard Entity Events
void MatryoshkaBom_Update();
void MatryoshkaBom_LateUpdate();
void MatryoshkaBom_StaticUpdate();
void MatryoshkaBom_Draw();
void MatryoshkaBom_Create(void* data);
void MatryoshkaBom_StageLoad();
void MatryoshkaBom_EditorDraw();
void MatryoshkaBom_EditorLoad();
void MatryoshkaBom_Serialize();

// Extra Entity Functions


#endif //!OBJ_MATRYOSHKABOM_H

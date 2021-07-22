#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "SonicMania.h"

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
void MatryoshkaBom_Update(void);
void MatryoshkaBom_LateUpdate(void);
void MatryoshkaBom_StaticUpdate(void);
void MatryoshkaBom_Draw(void);
void MatryoshkaBom_Create(void* data);
void MatryoshkaBom_StageLoad(void);
void MatryoshkaBom_EditorDraw(void);
void MatryoshkaBom_EditorLoad(void);
void MatryoshkaBom_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MATRYOSHKABOM_H

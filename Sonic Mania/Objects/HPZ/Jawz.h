#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJawz;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJawz;

// Object Struct
extern ObjectJawz *Jawz;

// Standard Entity Events
void Jawz_Update(void);
void Jawz_LateUpdate(void);
void Jawz_StaticUpdate(void);
void Jawz_Draw(void);
void Jawz_Create(void* data);
void Jawz_StageLoad(void);
void Jawz_EditorDraw(void);
void Jawz_EditorLoad(void);
void Jawz_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JAWZ_H

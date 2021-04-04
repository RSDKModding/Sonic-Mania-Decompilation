#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSP500MkII;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySP500MkII;

// Object Struct
extern ObjectSP500MkII *SP500MkII;

// Standard Entity Events
void SP500MkII_Update(void);
void SP500MkII_LateUpdate(void);
void SP500MkII_StaticUpdate(void);
void SP500MkII_Draw(void);
void SP500MkII_Create(void* data);
void SP500MkII_StageLoad(void);
void SP500MkII_EditorDraw(void);
void SP500MkII_EditorLoad(void);
void SP500MkII_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SP500MKII_H

#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHeavyShinobi;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyShinobi;

// Object Struct
extern ObjectHeavyShinobi *HeavyShinobi;

// Standard Entity Events
void HeavyShinobi_Update(void);
void HeavyShinobi_LateUpdate(void);
void HeavyShinobi_StaticUpdate(void);
void HeavyShinobi_Draw(void);
void HeavyShinobi_Create(void* data);
void HeavyShinobi_StageLoad(void);
void HeavyShinobi_EditorDraw(void);
void HeavyShinobi_EditorLoad(void);
void HeavyShinobi_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HEAVYSHINOBI_H

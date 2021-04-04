#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomShinobi;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomShinobi;

// Object Struct
extern ObjectPhantomShinobi *PhantomShinobi;

// Standard Entity Events
void PhantomShinobi_Update(void);
void PhantomShinobi_LateUpdate(void);
void PhantomShinobi_StaticUpdate(void);
void PhantomShinobi_Draw(void);
void PhantomShinobi_Create(void* data);
void PhantomShinobi_StageLoad(void);
void PhantomShinobi_EditorDraw(void);
void PhantomShinobi_EditorLoad(void);
void PhantomShinobi_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMSHINOBI_H

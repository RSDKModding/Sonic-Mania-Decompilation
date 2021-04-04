#ifndef OBJ_PROPELLERSHAFT_H
#define OBJ_PROPELLERSHAFT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPropellerShaft;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPropellerShaft;

// Object Struct
extern ObjectPropellerShaft *PropellerShaft;

// Standard Entity Events
void PropellerShaft_Update(void);
void PropellerShaft_LateUpdate(void);
void PropellerShaft_StaticUpdate(void);
void PropellerShaft_Draw(void);
void PropellerShaft_Create(void* data);
void PropellerShaft_StageLoad(void);
void PropellerShaft_EditorDraw(void);
void PropellerShaft_EditorLoad(void);
void PropellerShaft_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PROPELLERSHAFT_H

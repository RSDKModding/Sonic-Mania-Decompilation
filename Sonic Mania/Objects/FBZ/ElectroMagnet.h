#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectElectroMagnet;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityElectroMagnet;

// Object Struct
extern ObjectElectroMagnet *ElectroMagnet;

// Standard Entity Events
void ElectroMagnet_Update(void);
void ElectroMagnet_LateUpdate(void);
void ElectroMagnet_StaticUpdate(void);
void ElectroMagnet_Draw(void);
void ElectroMagnet_Create(void* data);
void ElectroMagnet_StageLoad(void);
void ElectroMagnet_EditorDraw(void);
void ElectroMagnet_EditorLoad(void);
void ElectroMagnet_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ELECTROMAGNET_H

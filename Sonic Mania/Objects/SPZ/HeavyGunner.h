#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHeavyGunner;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyGunner;

// Object Struct
extern ObjectHeavyGunner *HeavyGunner;

// Standard Entity Events
void HeavyGunner_Update(void);
void HeavyGunner_LateUpdate(void);
void HeavyGunner_StaticUpdate(void);
void HeavyGunner_Draw(void);
void HeavyGunner_Create(void* data);
void HeavyGunner_StageLoad(void);
void HeavyGunner_EditorDraw(void);
void HeavyGunner_EditorLoad(void);
void HeavyGunner_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HEAVYGUNNER_H

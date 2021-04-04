#ifndef OBJ_ROCKEMSOCKEM_H
#define OBJ_ROCKEMSOCKEM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRockemSockem;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRockemSockem;

// Object Struct
extern ObjectRockemSockem *RockemSockem;

// Standard Entity Events
void RockemSockem_Update(void);
void RockemSockem_LateUpdate(void);
void RockemSockem_StaticUpdate(void);
void RockemSockem_Draw(void);
void RockemSockem_Create(void* data);
void RockemSockem_StageLoad(void);
void RockemSockem_EditorDraw(void);
void RockemSockem_EditorLoad(void);
void RockemSockem_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ROCKEMSOCKEM_H

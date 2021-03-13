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
void RockemSockem_Update();
void RockemSockem_LateUpdate();
void RockemSockem_StaticUpdate();
void RockemSockem_Draw();
void RockemSockem_Create(void* data);
void RockemSockem_StageLoad();
void RockemSockem_EditorDraw();
void RockemSockem_EditorLoad();
void RockemSockem_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROCKEMSOCKEM_H

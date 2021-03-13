#ifndef OBJ_UFO_ITEMBOX_H
#define OBJ_UFO_ITEMBOX_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_ItemBox;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_ItemBox;

// Object Struct
extern ObjectUFO_ItemBox *UFO_ItemBox;

// Standard Entity Events
void UFO_ItemBox_Update();
void UFO_ItemBox_LateUpdate();
void UFO_ItemBox_StaticUpdate();
void UFO_ItemBox_Draw();
void UFO_ItemBox_Create(void* data);
void UFO_ItemBox_StageLoad();
void UFO_ItemBox_EditorDraw();
void UFO_ItemBox_EditorLoad();
void UFO_ItemBox_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_ITEMBOX_H

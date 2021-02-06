#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectItemBox : Object {

};

// Entity Class
struct EntityItemBox : Entity {

};

// Object Entity
extern ObjectItemBox ItemBox;

// Standard Entity Events
void ItemBox_Update();
void ItemBox_LateUpdate();
void ItemBox_StaticUpdate();
void ItemBox_Draw();
void ItemBox_Create(void* data);
void ItemBox_StageLoad();
void ItemBox_EditorDraw();
void ItemBox_EditorLoad();
void ItemBox_Serialize();

// Extra Entity Functions


#endif //!OBJ_ITEMBOX_H

#ifndef OBJ_TITLECARD_H
#define OBJ_TITLECARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTitleCard : Object {

};

// Entity Class
struct EntityTitleCard : Entity {

};

// Object Entity
extern ObjectTitleCard TitleCard;

// Standard Entity Events
void TitleCard_Update();
void TitleCard_LateUpdate();
void TitleCard_StaticUpdate();
void TitleCard_Draw();
void TitleCard_Create(void* data);
void TitleCard_StageLoad();
void TitleCard_EditorDraw();
void TitleCard_EditorLoad();
void TitleCard_Serialize();

// Extra Entity Functions


#endif //!OBJ_TITLECARD_H

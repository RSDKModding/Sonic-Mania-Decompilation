#ifndef OBJ_TITLEEGGMAN_H
#define OBJ_TITLEEGGMAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTitleEggman : Object {

};

// Entity Class
struct EntityTitleEggman : Entity {

};

// Object Entity
extern ObjectTitleEggman TitleEggman;

// Standard Entity Events
void TitleEggman_Update();
void TitleEggman_LateUpdate();
void TitleEggman_StaticUpdate();
void TitleEggman_Draw();
void TitleEggman_Create(void* data);
void TitleEggman_StageLoad();
void TitleEggman_EditorDraw();
void TitleEggman_EditorLoad();
void TitleEggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_TITLEEGGMAN_H

#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTitleSonic : Object {

};

// Entity Class
struct EntityTitleSonic : Entity {

};

// Object Entity
extern ObjectTitleSonic TitleSonic;

// Standard Entity Events
void TitleSonic_Update();
void TitleSonic_LateUpdate();
void TitleSonic_StaticUpdate();
void TitleSonic_Draw();
void TitleSonic_Create(void* data);
void TitleSonic_StageLoad();
void TitleSonic_EditorDraw();
void TitleSonic_EditorLoad();
void TitleSonic_Serialize();

// Extra Entity Functions


#endif //!OBJ_TITLESONIC_H

#ifndef OBJ_UIVIDEO_H
#define OBJ_UIVIDEO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIVideo : Object {

};

// Entity Class
struct EntityUIVideo : Entity {

};

// Object Struct
extern ObjectUIVideo *UIVideo;

// Standard Entity Events
void UIVideo_Update();
void UIVideo_LateUpdate();
void UIVideo_StaticUpdate();
void UIVideo_Draw();
void UIVideo_Create(void* data);
void UIVideo_StageLoad();
void UIVideo_EditorDraw();
void UIVideo_EditorLoad();
void UIVideo_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVIDEO_H

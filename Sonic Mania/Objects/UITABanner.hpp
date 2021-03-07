#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUITABanner : Object {

};

// Entity Class
struct EntityUITABanner : Entity {

};

// Object Struct
extern ObjectUITABanner *UITABanner;

// Standard Entity Events
void UITABanner_Update();
void UITABanner_LateUpdate();
void UITABanner_StaticUpdate();
void UITABanner_Draw();
void UITABanner_Create(void* data);
void UITABanner_StageLoad();
void UITABanner_EditorDraw();
void UITABanner_EditorLoad();
void UITABanner_Serialize();

// Extra Entity Functions


#endif //!OBJ_UITABANNER_H

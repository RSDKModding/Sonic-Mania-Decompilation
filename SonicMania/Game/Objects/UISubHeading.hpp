#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUISubHeading : Object {

};

// Entity Class
struct EntityUISubHeading : Entity {

};

// Object Entity
extern ObjectUISubHeading UISubHeading;

// Standard Entity Events
void UISubHeading_Update();
void UISubHeading_LateUpdate();
void UISubHeading_StaticUpdate();
void UISubHeading_Draw();
void UISubHeading_Create(void* data);
void UISubHeading_StageLoad();
void UISubHeading_EditorDraw();
void UISubHeading_EditorLoad();
void UISubHeading_Serialize();

// Extra Entity Functions


#endif //!OBJ_UISUBHEADING_H

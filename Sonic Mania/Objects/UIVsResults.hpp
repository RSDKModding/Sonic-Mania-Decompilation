#ifndef OBJ_UIVSRESULTS_H
#define OBJ_UIVSRESULTS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIVsResults : Object {

};

// Entity Class
struct EntityUIVsResults : Entity {

};

// Object Entity
extern ObjectUIVsResults UIVsResults;

// Standard Entity Events
void UIVsResults_Update();
void UIVsResults_LateUpdate();
void UIVsResults_StaticUpdate();
void UIVsResults_Draw();
void UIVsResults_Create(void* data);
void UIVsResults_StageLoad();
void UIVsResults_EditorDraw();
void UIVsResults_EditorLoad();
void UIVsResults_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVSRESULTS_H

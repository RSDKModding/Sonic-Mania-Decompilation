#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIVsScoreboard : Object {

};

// Entity Class
struct EntityUIVsScoreboard : Entity {

};

// Object Struct
extern ObjectUIVsScoreboard *UIVsScoreboard;

// Standard Entity Events
void UIVsScoreboard_Update();
void UIVsScoreboard_LateUpdate();
void UIVsScoreboard_StaticUpdate();
void UIVsScoreboard_Draw();
void UIVsScoreboard_Create(void* data);
void UIVsScoreboard_StageLoad();
void UIVsScoreboard_EditorDraw();
void UIVsScoreboard_EditorLoad();
void UIVsScoreboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVSSCOREBOARD_H

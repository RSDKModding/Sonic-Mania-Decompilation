#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

//Object Class
class UILeaderboard : Object {

};

//Entity Class
class EntityUILeaderboard : Entity {

};

//Entity Functions
void UILeaderboard_Update();
void UILeaderboard_EarlyUpdate();
void UILeaderboard_LateUpdate();
void UILeaderboard_Draw();
void UILeaderboard_Setup(void* subtype);
void UILeaderboard_StageLoad();
void UILeaderboard_GetAttributes();

#endif //!OBJ_UILEADERBOARD_H

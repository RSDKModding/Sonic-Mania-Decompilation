#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectUILeaderboard : public Object {
public:

};

//Entity Class
class EntityUILeaderboard : public Entity {
public:

};

//Object Entity
ObjectUILeaderboard UILeaderboard;

//Entity Functions
void UILeaderboard_Update();
void UILeaderboard_EarlyUpdate();
void UILeaderboard_LateUpdate();
void UILeaderboard_Draw();
void UILeaderboard_Setup(void* subtype);
void UILeaderboard_StageLoad();
void UILeaderboard_GetAttributes();

#endif //!OBJ_UILEADERBOARD_H

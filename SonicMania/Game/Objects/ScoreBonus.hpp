#ifndef OBJ_SCOREBONUS_H
#define OBJ_SCOREBONUS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectScoreBonus : Object {

};

// Entity Class
struct EntityScoreBonus : Entity {

};

// Object Entity
extern ObjectScoreBonus ScoreBonus;

// Standard Entity Events
void ScoreBonus_Update();
void ScoreBonus_LateUpdate();
void ScoreBonus_StaticUpdate();
void ScoreBonus_Draw();
void ScoreBonus_Create(void* data);
void ScoreBonus_StageLoad();
void ScoreBonus_EditorDraw();
void ScoreBonus_EditorLoad();
void ScoreBonus_Serialize();

// Extra Entity Functions


#endif //!OBJ_SCOREBONUS_H

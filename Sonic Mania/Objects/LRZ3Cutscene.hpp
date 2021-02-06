#ifndef OBJ_LRZ3CUTSCENE_H
#define OBJ_LRZ3CUTSCENE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ3Cutscene : Object {

};

// Entity Class
struct EntityLRZ3Cutscene : Entity {

};

// Object Entity
extern ObjectLRZ3Cutscene LRZ3Cutscene;

// Standard Entity Events
void LRZ3Cutscene_Update();
void LRZ3Cutscene_LateUpdate();
void LRZ3Cutscene_StaticUpdate();
void LRZ3Cutscene_Draw();
void LRZ3Cutscene_Create(void* data);
void LRZ3Cutscene_StageLoad();
void LRZ3Cutscene_EditorDraw();
void LRZ3Cutscene_EditorLoad();
void LRZ3Cutscene_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ3CUTSCENE_H

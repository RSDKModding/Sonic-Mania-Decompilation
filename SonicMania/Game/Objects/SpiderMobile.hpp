#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpiderMobile : Object {

};

// Entity Class
struct EntitySpiderMobile : Entity {

};

// Object Entity
extern ObjectSpiderMobile SpiderMobile;

// Standard Entity Events
void SpiderMobile_Update();
void SpiderMobile_LateUpdate();
void SpiderMobile_StaticUpdate();
void SpiderMobile_Draw();
void SpiderMobile_Create(void* data);
void SpiderMobile_StageLoad();
void SpiderMobile_EditorDraw();
void SpiderMobile_EditorLoad();
void SpiderMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIDERMOBILE_H

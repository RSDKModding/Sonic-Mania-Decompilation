#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHeavyKing : Object{

};

// Entity Class
struct EntityHeavyKing : Entity {

};

// Object Struct
extern ObjectHeavyKing *HeavyKing;

// Standard Entity Events
void HeavyKing_Update();
void HeavyKing_LateUpdate();
void HeavyKing_StaticUpdate();
void HeavyKing_Draw();
void HeavyKing_Create(void* data);
void HeavyKing_StageLoad();
void HeavyKing_EditorDraw();
void HeavyKing_EditorLoad();
void HeavyKing_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYKING_H

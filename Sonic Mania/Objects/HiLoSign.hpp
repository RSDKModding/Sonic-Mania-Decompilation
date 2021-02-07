#ifndef OBJ_HILOSIGN_H
#define OBJ_HILOSIGN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHiLoSign : Object{

};

// Entity Class
struct EntityHiLoSign : Entity {

};

// Object Struct
extern ObjectHiLoSign *HiLoSign;

// Standard Entity Events
void HiLoSign_Update();
void HiLoSign_LateUpdate();
void HiLoSign_StaticUpdate();
void HiLoSign_Draw();
void HiLoSign_Create(void* data);
void HiLoSign_StageLoad();
void HiLoSign_EditorDraw();
void HiLoSign_EditorLoad();
void HiLoSign_Serialize();

// Extra Entity Functions


#endif //!OBJ_HILOSIGN_H

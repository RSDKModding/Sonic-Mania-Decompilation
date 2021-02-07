#ifndef OBJ_LOTTOMACHINE_H
#define OBJ_LOTTOMACHINE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLottoMachine : Object{

};

// Entity Class
struct EntityLottoMachine : Entity {

};

// Object Struct
extern ObjectLottoMachine *LottoMachine;

// Standard Entity Events
void LottoMachine_Update();
void LottoMachine_LateUpdate();
void LottoMachine_StaticUpdate();
void LottoMachine_Draw();
void LottoMachine_Create(void* data);
void LottoMachine_StageLoad();
void LottoMachine_EditorDraw();
void LottoMachine_EditorLoad();
void LottoMachine_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOTTOMACHINE_H

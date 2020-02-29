#ifndef OBJ_LOTTOMACHINE_H
#define OBJ_LOTTOMACHINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectLottoMachine : public Object {
public:

};

//Entity Class
class EntityLottoMachine : public Entity {
public:

};

//Object Entity
ObjectLottoMachine LottoMachine;

//Entity Functions
void LottoMachine_Update();
void LottoMachine_EarlyUpdate();
void LottoMachine_LateUpdate();
void LottoMachine_Draw();
void LottoMachine_Setup(void* subtype);
void LottoMachine_StageLoad();
void LottoMachine_GetAttributes();

#endif //!OBJ_LOTTOMACHINE_H

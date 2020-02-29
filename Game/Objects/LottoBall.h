#ifndef OBJ_LOTTOBALL_H
#define OBJ_LOTTOBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLottoBall : public Object {
public:

};

//Entity Class
class EntityLottoBall : public Entity {
public:

};

//Object Entity
ObjectLottoBall LottoBall;

//Entity Functions
void LottoBall_Update();
void LottoBall_EarlyUpdate();
void LottoBall_LateUpdate();
void LottoBall_Draw();
void LottoBall_Setup(void* subtype);
void LottoBall_StageLoad();
void LottoBall_GetAttributes();

#endif //!OBJ_LOTTOBALL_H

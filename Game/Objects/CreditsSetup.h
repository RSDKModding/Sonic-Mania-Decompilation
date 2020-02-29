#ifndef OBJ_CREDITSSETUP_H
#define OBJ_CREDITSSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectCreditsSetup : public Object {
public:

};

//Entity Class
class EntityCreditsSetup : public Entity {
public:

};

//Object Entity
ObjectCreditsSetup CreditsSetup;

//Entity Functions
void CreditsSetup_Update();
void CreditsSetup_EarlyUpdate();
void CreditsSetup_LateUpdate();
void CreditsSetup_Draw();
void CreditsSetup_Setup(void* subtype);
void CreditsSetup_StageLoad();
void CreditsSetup_GetAttributes();

#endif //!OBJ_CREDITSSETUP_H

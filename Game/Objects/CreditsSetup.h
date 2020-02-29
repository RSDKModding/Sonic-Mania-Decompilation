#ifndef OBJ_CREDITSSETUP_H
#define OBJ_CREDITSSETUP_H

//Object Class
class CreditsSetup : Object {

};

//Entity Class
class EntityCreditsSetup : Entity {

};

//Entity Functions
void CreditsSetup_Update();
void CreditsSetup_EarlyUpdate();
void CreditsSetup_LateUpdate();
void CreditsSetup_Draw();
void CreditsSetup_Setup(void* subtype);
void CreditsSetup_StageLoad();
void CreditsSetup_GetAttributes();

#endif //!OBJ_CREDITSSETUP_H

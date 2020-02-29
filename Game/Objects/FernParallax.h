#ifndef OBJ_FERNPARALLAX_H
#define OBJ_FERNPARALLAX_H

//Object Class
class FernParallax : Object {

};

//Entity Class
class EntityFernParallax : Entity {

};

//Entity Functions
void FernParallax_Update();
void FernParallax_EarlyUpdate();
void FernParallax_LateUpdate();
void FernParallax_Draw();
void FernParallax_Setup(void* subtype);
void FernParallax_StageLoad();
void FernParallax_GetAttributes();

#endif //!OBJ_FERNPARALLAX_H

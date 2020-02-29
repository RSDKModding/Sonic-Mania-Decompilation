#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUberCaterkiller : public Object {
public:

};

//Entity Class
class EntityUberCaterkiller : public Entity {
public:

};

//Object Entity
ObjectUberCaterkiller UberCaterkiller;

//Entity Functions
void UberCaterkiller_Update();
void UberCaterkiller_EarlyUpdate();
void UberCaterkiller_LateUpdate();
void UberCaterkiller_Draw();
void UberCaterkiller_Setup(void* subtype);
void UberCaterkiller_StageLoad();
void UberCaterkiller_GetAttributes();

#endif //!OBJ_UBERCATERKILLER_H

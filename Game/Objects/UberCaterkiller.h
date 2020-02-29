#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

//Object Class
class UberCaterkiller : Object {

};

//Entity Class
class EntityUberCaterkiller : Entity {

};

//Entity Functions
void UberCaterkiller_Update();
void UberCaterkiller_EarlyUpdate();
void UberCaterkiller_LateUpdate();
void UberCaterkiller_Draw();
void UberCaterkiller_Setup(void* subtype);
void UberCaterkiller_StageLoad();
void UberCaterkiller_GetAttributes();

#endif //!OBJ_UBERCATERKILLER_H

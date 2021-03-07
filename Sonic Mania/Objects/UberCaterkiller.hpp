#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUberCaterkiller : Object {

};

// Entity Class
struct EntityUberCaterkiller : Entity {

};

// Object Struct
extern ObjectUberCaterkiller *UberCaterkiller;

// Standard Entity Events
void UberCaterkiller_Update();
void UberCaterkiller_LateUpdate();
void UberCaterkiller_StaticUpdate();
void UberCaterkiller_Draw();
void UberCaterkiller_Create(void* data);
void UberCaterkiller_StageLoad();
void UberCaterkiller_EditorDraw();
void UberCaterkiller_EditorLoad();
void UberCaterkiller_Serialize();

// Extra Entity Functions


#endif //!OBJ_UBERCATERKILLER_H

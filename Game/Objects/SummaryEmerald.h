#ifndef OBJ_SUMMARYEMERALD_H
#define OBJ_SUMMARYEMERALD_H

#include "../../SonicMania.h"

//Object Class
class ObjectSummaryEmerald : public Object {
public:

};

//Entity Class
class EntitySummaryEmerald : public Entity {
public:

};

//Object Entity
ObjectSummaryEmerald SummaryEmerald;

//Entity Functions
void SummaryEmerald_Update();
void SummaryEmerald_EarlyUpdate();
void SummaryEmerald_LateUpdate();
void SummaryEmerald_Draw();
void SummaryEmerald_Setup(void* subtype);
void SummaryEmerald_StageLoad();
void SummaryEmerald_GetAttributes();

#endif //!OBJ_SUMMARYEMERALD_H

#ifndef OBJ_SUMMARYEMERALD_H
#define OBJ_SUMMARYEMERALD_H

//Object Class
class SummaryEmerald : Object {

};

//Entity Class
class EntitySummaryEmerald : Entity {

};

//Entity Functions
void SummaryEmerald_Update();
void SummaryEmerald_EarlyUpdate();
void SummaryEmerald_LateUpdate();
void SummaryEmerald_Draw();
void SummaryEmerald_Setup(void* subtype);
void SummaryEmerald_StageLoad();
void SummaryEmerald_GetAttributes();

#endif //!OBJ_SUMMARYEMERALD_H

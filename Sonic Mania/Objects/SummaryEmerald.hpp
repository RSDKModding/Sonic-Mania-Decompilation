#ifndef OBJ_SUMMARYEMERALD_H
#define OBJ_SUMMARYEMERALD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSummaryEmerald : Object {

};

// Entity Class
struct EntitySummaryEmerald : Entity {

};

// Object Struct
extern ObjectSummaryEmerald *SummaryEmerald;

// Standard Entity Events
void SummaryEmerald_Update();
void SummaryEmerald_LateUpdate();
void SummaryEmerald_StaticUpdate();
void SummaryEmerald_Draw();
void SummaryEmerald_Create(void* data);
void SummaryEmerald_StageLoad();
void SummaryEmerald_EditorDraw();
void SummaryEmerald_EditorLoad();
void SummaryEmerald_Serialize();

// Extra Entity Functions


#endif //!OBJ_SUMMARYEMERALD_H

#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSentryBug : Object{

};

// Entity Class
struct EntitySentryBug : Entity {

};

// Object Struct
extern ObjectSentryBug *SentryBug;

// Standard Entity Events
void SentryBug_Update();
void SentryBug_LateUpdate();
void SentryBug_StaticUpdate();
void SentryBug_Draw();
void SentryBug_Create(void* data);
void SentryBug_StageLoad();
void SentryBug_EditorDraw();
void SentryBug_EditorLoad();
void SentryBug_Serialize();

// Extra Entity Functions


#endif //!OBJ_SENTRYBUG_H

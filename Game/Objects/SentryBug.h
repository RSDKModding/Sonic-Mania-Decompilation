#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "../../SonicMania.h"

//Object Class
class ObjectSentryBug : public Object {
public:

};

//Entity Class
class EntitySentryBug : public Entity {
public:

};

//Object Entity
ObjectSentryBug SentryBug;

//Entity Functions
void SentryBug_Update();
void SentryBug_EarlyUpdate();
void SentryBug_LateUpdate();
void SentryBug_Draw();
void SentryBug_Setup(void* subtype);
void SentryBug_StageLoad();
void SentryBug_GetAttributes();

#endif //!OBJ_SENTRYBUG_H

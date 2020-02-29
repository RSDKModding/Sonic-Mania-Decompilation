#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

//Object Class
class SentryBug : Object {

};

//Entity Class
class EntitySentryBug : Entity {

};

//Entity Functions
void SentryBug_Update();
void SentryBug_EarlyUpdate();
void SentryBug_LateUpdate();
void SentryBug_Draw();
void SentryBug_Setup(void* subtype);
void SentryBug_StageLoad();
void SentryBug_GetAttributes();

#endif //!OBJ_SENTRYBUG_H

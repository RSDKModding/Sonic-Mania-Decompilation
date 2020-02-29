#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

//Object Class
class TTCutscene : Object {

};

//Entity Class
class EntityTTCutscene : Entity {

};

//Entity Functions
void TTCutscene_Update();
void TTCutscene_EarlyUpdate();
void TTCutscene_LateUpdate();
void TTCutscene_Draw();
void TTCutscene_Setup(void* subtype);
void TTCutscene_StageLoad();
void TTCutscene_GetAttributes();

#endif //!OBJ_TTCUTSCENE_H

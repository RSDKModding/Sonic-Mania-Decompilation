#ifndef OBJ_MONARCHBG_H
#define OBJ_MONARCHBG_H

//Object Class
class MonarchBG : Object {

};

//Entity Class
class EntityMonarchBG : Entity {

};

//Entity Functions
void MonarchBG_Update();
void MonarchBG_EarlyUpdate();
void MonarchBG_LateUpdate();
void MonarchBG_Draw();
void MonarchBG_Setup(void* subtype);
void MonarchBG_StageLoad();
void MonarchBG_GetAttributes();

#endif //!OBJ_MONARCHBG_H

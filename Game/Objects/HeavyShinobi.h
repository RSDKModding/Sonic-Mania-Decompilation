#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

//Object Class
class HeavyShinobi : Object {

};

//Entity Class
class EntityHeavyShinobi : Entity {

};

//Entity Functions
void HeavyShinobi_Update();
void HeavyShinobi_EarlyUpdate();
void HeavyShinobi_LateUpdate();
void HeavyShinobi_Draw();
void HeavyShinobi_Setup(void* subtype);
void HeavyShinobi_StageLoad();
void HeavyShinobi_GetAttributes();

#endif //!OBJ_HEAVYSHINOBI_H

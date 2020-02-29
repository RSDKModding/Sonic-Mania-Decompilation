#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

//Object Class
class AIZTornado : Object {

};

//Entity Class
class EntityAIZTornado : Entity {

};

//Entity Functions
void AIZTornado_Update();
void AIZTornado_EarlyUpdate();
void AIZTornado_LateUpdate();
void AIZTornado_Draw();
void AIZTornado_Setup(void* subtype);
void AIZTornado_StageLoad();
void AIZTornado_GetAttributes();

#endif //!OBJ_AIZTORNADO_H

#ifndef OBJ_MEGAOCTUS_H
#define OBJ_MEGAOCTUS_H

//Object Class
class MegaOctus : Object {

};

//Entity Class
class EntityMegaOctus : Entity {

};

//Entity Functions
void MegaOctus_Update();
void MegaOctus_EarlyUpdate();
void MegaOctus_LateUpdate();
void MegaOctus_Draw();
void MegaOctus_Setup(void* subtype);
void MegaOctus_StageLoad();
void MegaOctus_GetAttributes();

#endif //!OBJ_MEGAOCTUS_H

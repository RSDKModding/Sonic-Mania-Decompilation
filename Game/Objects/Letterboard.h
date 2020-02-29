#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

//Object Class
class Letterboard : Object {

};

//Entity Class
class EntityLetterboard : Entity {

};

//Entity Functions
void Letterboard_Update();
void Letterboard_EarlyUpdate();
void Letterboard_LateUpdate();
void Letterboard_Draw();
void Letterboard_Setup(void* subtype);
void Letterboard_StageLoad();
void Letterboard_GetAttributes();

#endif //!OBJ_LETTERBOARD_H

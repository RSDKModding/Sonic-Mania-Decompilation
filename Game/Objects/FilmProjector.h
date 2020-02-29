#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

//Object Class
class FilmProjector : Object {

};

//Entity Class
class EntityFilmProjector : Entity {

};

//Entity Functions
void FilmProjector_Update();
void FilmProjector_EarlyUpdate();
void FilmProjector_LateUpdate();
void FilmProjector_Draw();
void FilmProjector_Setup(void* subtype);
void FilmProjector_StageLoad();
void FilmProjector_GetAttributes();

#endif //!OBJ_FILMPROJECTOR_H

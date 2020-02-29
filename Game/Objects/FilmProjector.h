#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectFilmProjector : public Object {
public:

};

//Entity Class
class EntityFilmProjector : public Entity {
public:

};

//Object Entity
ObjectFilmProjector FilmProjector;

//Entity Functions
void FilmProjector_Update();
void FilmProjector_EarlyUpdate();
void FilmProjector_LateUpdate();
void FilmProjector_Draw();
void FilmProjector_Setup(void* subtype);
void FilmProjector_StageLoad();
void FilmProjector_GetAttributes();

#endif //!OBJ_FILMPROJECTOR_H

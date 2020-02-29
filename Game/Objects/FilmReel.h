#ifndef OBJ_FILMREEL_H
#define OBJ_FILMREEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectFilmReel : public Object {
public:

};

//Entity Class
class EntityFilmReel : public Entity {
public:

};

//Object Entity
ObjectFilmReel FilmReel;

//Entity Functions
void FilmReel_Update();
void FilmReel_EarlyUpdate();
void FilmReel_LateUpdate();
void FilmReel_Draw();
void FilmReel_Setup(void* subtype);
void FilmReel_StageLoad();
void FilmReel_GetAttributes();

#endif //!OBJ_FILMREEL_H

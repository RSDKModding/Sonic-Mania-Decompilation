#ifndef OBJ_FILMREEL_H
#define OBJ_FILMREEL_H

//Object Class
class FilmReel : Object {

};

//Entity Class
class EntityFilmReel : Entity {

};

//Entity Functions
void FilmReel_Update();
void FilmReel_EarlyUpdate();
void FilmReel_LateUpdate();
void FilmReel_Draw();
void FilmReel_Setup(void* subtype);
void FilmReel_StageLoad();
void FilmReel_GetAttributes();

#endif //!OBJ_FILMREEL_H

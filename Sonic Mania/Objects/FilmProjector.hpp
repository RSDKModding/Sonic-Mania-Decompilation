#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFilmProjector : Object{

};

// Entity Class
struct EntityFilmProjector : Entity {

};

// Object Struct
extern ObjectFilmProjector *FilmProjector;

// Standard Entity Events
void FilmProjector_Update();
void FilmProjector_LateUpdate();
void FilmProjector_StaticUpdate();
void FilmProjector_Draw();
void FilmProjector_Create(void* data);
void FilmProjector_StageLoad();
void FilmProjector_EditorDraw();
void FilmProjector_EditorLoad();
void FilmProjector_Serialize();

// Extra Entity Functions


#endif //!OBJ_FILMPROJECTOR_H

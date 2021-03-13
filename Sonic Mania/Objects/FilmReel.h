#ifndef OBJ_FILMREEL_H
#define OBJ_FILMREEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFilmReel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFilmReel;

// Object Struct
extern ObjectFilmReel *FilmReel;

// Standard Entity Events
void FilmReel_Update();
void FilmReel_LateUpdate();
void FilmReel_StaticUpdate();
void FilmReel_Draw();
void FilmReel_Create(void* data);
void FilmReel_StageLoad();
void FilmReel_EditorDraw();
void FilmReel_EditorLoad();
void FilmReel_Serialize();

// Extra Entity Functions


#endif //!OBJ_FILMREEL_H

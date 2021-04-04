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
void FilmReel_Update(void);
void FilmReel_LateUpdate(void);
void FilmReel_StaticUpdate(void);
void FilmReel_Draw(void);
void FilmReel_Create(void* data);
void FilmReel_StageLoad(void);
void FilmReel_EditorDraw(void);
void FilmReel_EditorLoad(void);
void FilmReel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FILMREEL_H

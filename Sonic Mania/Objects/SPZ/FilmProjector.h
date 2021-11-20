#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectFilmProjector;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 pathMovement;
    Vector2 posUnknown;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
} EntityFilmProjector;

// Object Struct
extern ObjectFilmProjector *FilmProjector;

// Standard Entity Events
void FilmProjector_Update(void);
void FilmProjector_LateUpdate(void);
void FilmProjector_StaticUpdate(void);
void FilmProjector_Draw(void);
void FilmProjector_Create(void* data);
void FilmProjector_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FilmProjector_EditorDraw(void);
void FilmProjector_EditorLoad(void);
#endif
void FilmProjector_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FILMPROJECTOR_H

#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

#include "Game.h"

// Object Class
struct ObjectFilmProjector {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityFilmProjector {
    RSDK_ENTITY
    uint8 pathMovement;
    Vector2 filmPos;
    Animator projectorAnimator;
    Animator screenAnimator;
    Animator filmReelAnimator;
    Animator reelEdgeAnimator;
    Animator celluoidAnimator;
    Animator eggmanAnimator;
};

// Object Struct
extern ObjectFilmProjector *FilmProjector;

// Standard Entity Events
void FilmProjector_Update(void);
void FilmProjector_LateUpdate(void);
void FilmProjector_StaticUpdate(void);
void FilmProjector_Draw(void);
void FilmProjector_Create(void *data);
void FilmProjector_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FilmProjector_EditorDraw(void);
void FilmProjector_EditorLoad(void);
#endif
void FilmProjector_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_FILMPROJECTOR_H

#ifndef OBJ_FILMREEL_H
#define OBJ_FILMREEL_H

#include "Game.h"

// Object Class
struct ObjectFilmReel {
    RSDK_OBJECT
    Hitbox hitboxWheel;
    Vector2 offsetPos;
    uint16 aniFrames;
    uint16 sfxUnravel;
    uint16 sfxLanding;
};

// Entity Class
struct EntityFilmReel {
    RSDK_ENTITY
    Vector2 pathSize;
    uint8 spinDirection;
    Vector2 drawPos;
    Vector2 endPos;
    Vector2 lastPos;
    Vector2 moveOffset;
    int32 spinAngle;
    int32 unused;
    int32 spinSpeed;
    int32 pathFlags;
    int32 pathDir;
    int32 lineDir;
    bool32 moveCamera;
    Animator reelAnimator;
    Animator celluoidAnimator;
    Animator edgeAnimator;
    Animator pinAnimator;
};

// Object Struct
extern ObjectFilmReel *FilmReel;

// Standard Entity Events
void FilmReel_Update(void);
void FilmReel_LateUpdate(void);
void FilmReel_StaticUpdate(void);
void FilmReel_Draw(void);
void FilmReel_Create(void *data);
void FilmReel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FilmReel_EditorDraw(void);
void FilmReel_EditorLoad(void);
#endif
void FilmReel_Serialize(void);

// Extra Entity Functions
void FilmReel_SpinLeft(void);
void FilmReel_SpinRight(void);

#endif //! OBJ_FILMREEL_H

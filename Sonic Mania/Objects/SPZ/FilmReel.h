#ifndef OBJ_FILMREEL_H
#define OBJ_FILMREEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Vector2 offsetPos;
    uint16 aniFrames;
    uint16 sfxUnravel;
    uint16 sfxLanding;
} ObjectFilmReel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 pathSize;
    uint8 spinDirection;
    Vector2 drawPos;
    Vector2 endPos;
    Vector2 field_74;
    Vector2 field_7C;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    bool32 flag;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
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
#if RETRO_INCLUDE_EDITOR
void FilmReel_EditorDraw(void);
void FilmReel_EditorLoad(void);
#endif
void FilmReel_Serialize(void);

// Extra Entity Functions
void FilmReel_SpinLeft(void);
void FilmReel_SpinRight(void);

#endif //!OBJ_FILMREEL_H

#ifndef OBJ_FILMPROJECTOR_H
#define OBJ_FILMPROJECTOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectFilmProjector;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte pathMovement;
    Vector2 posUnknown;
    AnimationData data1;
    AnimationData data2;
    AnimationData data3;
    AnimationData data4;
    AnimationData data5;
    AnimationData data6;
} EntityFilmProjector;

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

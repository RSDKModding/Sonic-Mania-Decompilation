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
void FilmProjector_Update(void);
void FilmProjector_LateUpdate(void);
void FilmProjector_StaticUpdate(void);
void FilmProjector_Draw(void);
void FilmProjector_Create(void* data);
void FilmProjector_StageLoad(void);
void FilmProjector_EditorDraw(void);
void FilmProjector_EditorLoad(void);
void FilmProjector_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FILMPROJECTOR_H

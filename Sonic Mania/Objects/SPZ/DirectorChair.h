#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

#include "SonicMania.h"

typedef enum {
    DIRECTORCHAIR_CLOSED,
    DIRECTORCHAIR_STRETCHED,
}DirectorChairTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 field_3;
    uint8 field_4;
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 sfxUnravel;
    uint16 sfxExtend;
    uint16 sfxRetract;
} ObjectDirectorChair;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 speed;
    bool32 hasTension;
    int8 frameID;
    uint8 collision;
    Vector2 tileOrigin;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 size;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;

    Animator animator2;
    int32 field_E4;
    int32 field_E8;
    int32 field_EC;
} EntityDirectorChair;

// Object Struct
extern ObjectDirectorChair *DirectorChair;

// Standard Entity Events
void DirectorChair_Update(void);
void DirectorChair_LateUpdate(void);
void DirectorChair_StaticUpdate(void);
void DirectorChair_Draw(void);
void DirectorChair_Create(void* data);
void DirectorChair_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DirectorChair_EditorDraw(void);
void DirectorChair_EditorLoad(void);
#endif
void DirectorChair_Serialize(void);

// Extra Entity Functions
void DirectorChair_StateCollide_Chair(void);

void DirectorChair_Unknown2(void);
void DirectorChair_Unknown3(void);
void DirectorChair_Unknown4(void);
void DirectorChair_Unknown5(void);
void DirectorChair_Unknown6(void);

#endif //!OBJ_DIRECTORCHAIR_H

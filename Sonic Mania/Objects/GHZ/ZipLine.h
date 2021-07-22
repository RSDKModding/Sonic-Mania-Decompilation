#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox;
} ObjectZipLine;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte activePlayers;
    Animator animator;
    int length;
    Vector2 startPos;
    Vector2 endPos;
    Vector2 handlePos;
    byte grabDelay[PLAYER_MAX];
    Vector2 field_98;
    int field_A0;
} EntityZipLine;

// Object Struct
extern ObjectZipLine *ZipLine;

// Standard Entity Events
void ZipLine_Update(void);
void ZipLine_LateUpdate(void);
void ZipLine_StaticUpdate(void);
void ZipLine_Draw(void);
void ZipLine_Create(void* data);
void ZipLine_StageLoad(void);
void ZipLine_EditorDraw(void);
void ZipLine_EditorLoad(void);
void ZipLine_Serialize(void);

// Extra Entity Functions
void ZipLine_Unknown1(void);
void ZipLine_Unknown2(void);
Vector2 ZipLine_Unknown3(void);
void ZipLine_Unknown4(void);

#endif //!OBJ_ZIPLINE_H

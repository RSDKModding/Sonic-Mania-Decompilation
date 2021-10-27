#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPaper;
    uint32 colours[64];
} ObjectPaperRoller;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 length;
    Vector2 startPos;
    int32 field_64;
    int32 angleVal;
    int32 playerTimer[4];
    int32 playerUnknown[4];
    Vector2 field_8C;
    Vector2 field_94;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
} EntityPaperRoller;

// Object Struct
extern ObjectPaperRoller *PaperRoller;

// Standard Entity Events
void PaperRoller_Update(void);
void PaperRoller_LateUpdate(void);
void PaperRoller_StaticUpdate(void);
void PaperRoller_Draw(void);
void PaperRoller_Create(void* data);
void PaperRoller_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PaperRoller_EditorDraw(void);
void PaperRoller_EditorLoad(void);
#endif
void PaperRoller_Serialize(void);

// Extra Entity Functions
void PaperRoller_DrawLineDeform(uint32 colour, int32 len, int32 posX1, int32 posY1, int32 posX2, int32 posY2, int32 posX3, int32 posY3, int32 posX4, int32 posY4);
void PaperRoller_DrawPaperLines(void);
void PaperRoller_DrawRollers(void);
void PaperRoller_Unknown4(void);
void PaperRoller_Unknown5(void);

#endif //!OBJ_PAPERROLLER_H

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
    int length;
    Vector2 startPos;
    int field_64;
    int angleVal;
    int playerTimer[4];
    int playerUnknown[4];
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
void PaperRoller_EditorDraw(void);
void PaperRoller_EditorLoad(void);
void PaperRoller_Serialize(void);

// Extra Entity Functions
void PaperRoller_DrawLineDeform(uint32 colour, int len, int posX1, int posY1, int posX2, int posY2, int posX3, int posY3, int posX4, int posY4);
void PaperRoller_DrawPaperLines(void);
void PaperRoller_DrawRollers(void);
void PaperRoller_Unknown4(void);
void PaperRoller_Unknown5(void);

#endif //!OBJ_PAPERROLLER_H

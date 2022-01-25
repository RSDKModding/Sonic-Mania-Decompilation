#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "SonicMania.h"

// Object Class
struct ObjectPaperRoller {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPaper;
    uint32 colours[64];
};

// Entity Class
struct EntityPaperRoller {
    RSDK_ENTITY
    int32 length;
    Vector2 startPos;
    int32 unused;
    int32 divotAngle;
    int32 playerTimer[4];
    int32 lastJumpTimer[4]; //not used, is set and updated, idk what it was for
    Vector2 deformPosTop;
    Vector2 deformPosBottom;
    Hitbox hitbox;
    Animator rollerAnimator;
    Animator divotAnimator;
};

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
void PaperRoller_DrawDeformedLine(uint32 colour, int32 len, int32 posX1, int32 posY1, int32 posX2, int32 posY2, int32 posX3, int32 posY3, int32 posX4, int32 posY4);
void PaperRoller_DrawPaperLines(void);
void PaperRoller_DrawRollers(void);
void PaperRoller_HandleRollerCollisions(void);
void PaperRoller_HandlePrintCollisions(void);

#endif //!OBJ_PAPERROLLER_H

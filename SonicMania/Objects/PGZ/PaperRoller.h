#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "Game.h"

// Object Class
struct ObjectPaperRoller {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPaper;
    uint32 colors[64];
};

// Entity Class
struct EntityPaperRoller {
    RSDK_ENTITY
    int32 length;
    Vector2 startPos;
    int32 unused;
    int32 divotAngle;
    int32 playerTimer[PLAYER_COUNT];
    int32 lastJumpTimer[PLAYER_COUNT]; // not used, is set and updated, idk what it was for
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
void PaperRoller_Create(void *data);
void PaperRoller_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PaperRoller_EditorDraw(void);
void PaperRoller_EditorLoad(void);
#endif
void PaperRoller_Serialize(void);

// Extra Entity Functions
void PaperRoller_DrawDeformedLine(int32 startX, int32 startY, int32 endX, int32 endY, int32 offsetX, int32 offsetY, int32 deformX, int32 deformY,
                                  int32 len, uint32 *color);
void PaperRoller_DrawPaperLines(void);
void PaperRoller_DrawRollers(void);
void PaperRoller_HandleRollerCollisions(void);
void PaperRoller_HandlePrintCollisions(void);

#endif //! OBJ_PAPERROLLER_H

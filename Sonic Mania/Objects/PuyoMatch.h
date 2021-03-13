#ifndef OBJ_PUYOMATCH_H
#define OBJ_PUYOMATCH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    int value2[24]; //= { 4, 20, 24, 32, 48, 96, 160, 240, 320, 480, 600, 700, 800, 900, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 };
    int value3[24]; //= { 0, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672 };
    int value4[8]; //= { 0, 2, 3, 4, 5, 6, 7, 10 };
    int value5[6]; //= { 0, 0, 3, 6, 12, 24 };
    int value6[24]; //= { 0, 2, 5, 1, 3, 4, 5, 3, 4, 0, 1, 2, 3, 1, 0, 4, 2, 5, 0, 2, 1, 3, 5, 4 };
    ushort value7;
} ObjectPuyoMatch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoMatch;

// Object Struct
extern ObjectPuyoMatch *PuyoMatch;

// Standard Entity Events
void PuyoMatch_Update();
void PuyoMatch_LateUpdate();
void PuyoMatch_StaticUpdate();
void PuyoMatch_Draw();
void PuyoMatch_Create(void* data);
void PuyoMatch_StageLoad();
void PuyoMatch_EditorDraw();
void PuyoMatch_EditorLoad();
void PuyoMatch_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOMATCH_H

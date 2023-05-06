// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PrintBlock Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPrintBlock *PrintBlock;

void PrintBlock_Update(void)
{
    RSDK_THIS(PrintBlock);

    if (self->state == Platform_State_Fixed) {
        if (self->interval != (uint16)-1) {
            int32 timer = (Zone->timer + self->intervalOffset) % self->interval;

            if (timer >= self->duration) {
                if (timer >= self->duration + 12) {
                    self->animator.frameID = 1;
                    self->animator.timer   = 0;
                }
                else {
                    self->active           = ACTIVE_NORMAL;
                    self->state            = PrintBlock_State_Disappear;
                    self->timer            = timer - self->duration;
                    self->animator.frameID = 4 - self->timer / 3;
                    self->animator.timer   = self->timer % 3;
                }
            }
            else {
                self->active = ACTIVE_NORMAL;
                self->timer  = timer;
                if (self->timer > 12) {
                    self->animator.frameID = 4;
                    self->animator.timer   = 0;
                }
                else {
                    self->animator.timer   = self->timer % 3;
                    self->animator.frameID = self->timer / 3 + 1;
                }

                Vector2 pos;
                pos.x = 0x200000;
                pos.y = 0x200000;
                if (RSDK.CheckOnScreen(self, &pos)) {
                    int32 channel = RSDK.PlaySfx(PrintBlock->sfxLetter, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                }

                self->state = PrintBlock_State_Appear;
            }
        }
    }

    if (self->animator.frameID <= 2) {
        self->stateCollide = Platform_Collision_None;
        self->collision    = PLATFORM_C_NONE;
    }
    else {
        self->stateCollide = Platform_Collision_Solid;
        self->collision    = PLATFORM_C_SOLID;
    }

    Platform_Update();
}

void PrintBlock_LateUpdate(void) {}

void PrintBlock_StaticUpdate(void) {}

void PrintBlock_Draw(void)
{
    RSDK_THIS(PrintBlock);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->inkEffect              = INK_SUB;
    PrintBlock->animator.frameID = self->animator.frameID;
    RSDK.DrawSprite(&PrintBlock->animator, &self->drawPos, false);

    self->inkEffect = INK_NONE;
}

void PrintBlock_Create(void *data)
{
    RSDK_THIS(PrintBlock);

    self->collision = PLATFORM_C_SOLID;
    Platform_Create(NULL);

    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, self->letter, &self->animator, true, 0);

    self->alpha     = 128;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->state     = Platform_State_Fixed;
}

void PrintBlock_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        PrintBlock->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PrintBlock.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, 12, &PrintBlock->animator, true, 0);

    PrintBlock->sfxLetter = RSDK.GetSfx("PSZ/Letter.wav");
}

void PrintBlock_State_Appear(void)
{
    RSDK_THIS(PrintBlock);

    ++self->timer;
    if (++self->animator.timer == 3) {
        self->animator.timer = 0;
        if (self->animator.frameID < 4)
            self->animator.frameID++;
    }

    if (self->timer == self->duration)
        self->state = PrintBlock_State_Disappear;
}

void PrintBlock_State_Disappear(void)
{
    RSDK_THIS(PrintBlock);

    if (++self->animator.timer == 3) {
        self->animator.timer = 0;

        if (self->animator.frameID <= 1) {
            self->active = ACTIVE_BOUNDS;
            self->state  = Platform_State_Fixed;
        }
        else {
            self->animator.frameID--;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PrintBlock_EditorDraw(void)
{
    RSDK_THIS(PrintBlock);

    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, self->letter, &self->animator, true, self->interval == (uint16)-1 ? 0 : 4);
    self->alpha   = 0x80;
    self->drawPos = self->position;

    PrintBlock_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void PrintBlock_EditorLoad(void)
{
    PrintBlock->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PrintBlock.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(-1, 12, &PrintBlock->animator, true, 0);

    RSDK_ACTIVE_VAR(PrintBlock, letter);
    RSDK_ENUM_VAR("E", PRINTBLOCK_LETTER_E);
    RSDK_ENUM_VAR("T", PRINTBLOCK_LETTER_T);
    RSDK_ENUM_VAR("A", PRINTBLOCK_LETTER_A);
    RSDK_ENUM_VAR("O", PRINTBLOCK_LETTER_O);
    RSDK_ENUM_VAR("I", PRINTBLOCK_LETTER_I);
    RSDK_ENUM_VAR("N", PRINTBLOCK_LETTER_N);
    RSDK_ENUM_VAR("S", PRINTBLOCK_LETTER_S);
    RSDK_ENUM_VAR("H", PRINTBLOCK_LETTER_H);
    RSDK_ENUM_VAR("R", PRINTBLOCK_LETTER_R);
    RSDK_ENUM_VAR("D", PRINTBLOCK_LETTER_D);
    RSDK_ENUM_VAR("L", PRINTBLOCK_LETTER_L);
    RSDK_ENUM_VAR("U", PRINTBLOCK_LETTER_U);
}
#endif

void PrintBlock_Serialize(void)
{
    RSDK_EDITABLE_VAR(PrintBlock, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT8, letter);
}

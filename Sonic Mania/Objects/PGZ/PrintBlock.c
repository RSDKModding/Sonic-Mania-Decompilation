#include "SonicMania.h"

ObjectPrintBlock *PrintBlock;

void PrintBlock_Update(void)
{
    RSDK_THIS(PrintBlock);
    if (self->state == Platform_State_Fixed) {
        if (self->interval != 0xFFFF) {
            int32 timer = (Zone->timer + self->intervalOffset) % self->interval;
            if (timer >= self->duration) {
                if (timer >= self->duration + 12) {
                    self->animator.frameID        = 1;
                    self->animator.animationTimer = 0;
                }
                else {
                    self->active                  = ACTIVE_NORMAL;
                    self->state                   = PrintBlock_Unknown2;
                    self->timer           = timer - self->duration;
                    self->animator.frameID        = 4 - self->timer / 3;
                    self->animator.animationTimer = self->timer % 3;
                }
            }
            else {
                self->active        = ACTIVE_NORMAL;
                self->timer = timer;
                if (self->timer > 12) {
                    self->animator.frameID        = 4;
                    self->animator.animationTimer = 0;
                }
                else {
                    self->animator.animationTimer = self->timer % 3;
                    self->animator.frameID        = self->timer / 3 + 1;
                }

                Vector2 pos;
                pos.x = 0x200000;
                pos.y = 0x200000;
                if (RSDK.CheckOnScreen(self, &pos)) {
                    int32 channel = RSDK.PlaySfx(PrintBlock->sfxLetter, 0, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                }
                self->state = PrintBlock_Unknown1;
            }
        }
    }

    if (self->animator.frameID <= 2) {
        self->stateCollide = Platform_CollisionState_None;
        self->collision    = 4;
    }
    else {
        self->stateCollide = Platform_CollisionState_AllSolid;
        self->collision    = 1;
    }
    Platform_Update();
}

void PrintBlock_LateUpdate(void) {}

void PrintBlock_StaticUpdate(void) {}

void PrintBlock_Draw(void)
{
    RSDK_THIS(PrintBlock);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
    self->inkEffect            = INK_SUB;
    PrintBlock->animator.frameID = self->animator.frameID;
    RSDK.DrawSprite(&PrintBlock->animator, &self->drawPos, false);
    self->inkEffect = INK_NONE;
}

void PrintBlock_Create(void *data)
{
    RSDK_THIS(PrintBlock);
    self->collision = PLATFORM_C_SOLID_ALL;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, self->letter, &self->animator, true, 0);
    self->alpha     = 128;
    self->drawOrder = Zone->drawOrderLow;
    self->state     = Platform_State_Fixed;
}

void PrintBlock_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        PrintBlock->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PrintBlock.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, 12, &PrintBlock->animator, true, 0);
    PrintBlock->sfxLetter = RSDK.GetSfx("PSZ/Letter.wav");
}

void PrintBlock_Unknown1(void)
{
    RSDK_THIS(PrintBlock);
    ++self->timer;
    if (++self->animator.animationTimer == 3) {
        self->animator.animationTimer = 0;
        if (self->animator.frameID < 4)
            self->animator.frameID++;
    }

    if (self->timer == self->duration)
        self->state = PrintBlock_Unknown2;
}

void PrintBlock_Unknown2(void)
{
    RSDK_THIS(PrintBlock);
    if (++self->animator.animationTimer == 3) {
        self->animator.animationTimer = 0;
        if (self->animator.frameID <= 1) {
            self->active = ACTIVE_BOUNDS;
            self->state  = Platform_State_Fixed;
        }
        else {
            self->animator.frameID--;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PrintBlock_EditorDraw(void) {}

void PrintBlock_EditorLoad(void) {}
#endif

void PrintBlock_Serialize(void)
{
    RSDK_EDITABLE_VAR(PrintBlock, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT8, letter);
}

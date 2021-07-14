#include "../SonicMania.h"

ObjectPrintBlock *PrintBlock;

void PrintBlock_Update(void)
{
    RSDK_THIS(PrintBlock);
    if (entity->state == Platform_State_Normal) {
        if (entity->interval != 0xFFFF) {
            int timer = (Zone->timer + entity->intervalOffset) % entity->interval;
            if (timer >= entity->duration) {
                if (timer >= entity->duration + 12) {
                    entity->animator.frameID        = 1;
                    entity->animator.animationTimer = 0;
                }
                else {
                    entity->active                  = ACTIVE_NORMAL;
                    entity->state                   = PrintBlock_Unknown2;
                    entity->collapseDelay           = timer - entity->duration;
                    entity->animator.frameID        = 4 - entity->collapseDelay / 3;
                    entity->animator.animationTimer = entity->collapseDelay % 3;
                }
            }
            else {
                entity->active        = ACTIVE_NORMAL;
                entity->collapseDelay = timer;
                if (entity->collapseDelay > 12) {
                    entity->animator.frameID        = 4;
                    entity->animator.animationTimer = 0;
                }
                else {
                    entity->animator.animationTimer = entity->collapseDelay % 3;
                    entity->animator.frameID        = entity->collapseDelay / 3 + 1;
                }

                Vector2 pos;
                pos.x = 0x200000;
                pos.y = 0x200000;
                if (RSDK.CheckOnScreen(entity, &pos)) {
                    int channel = RSDK.PlaySFX(PrintBlock->sfxLetter, 0, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                }
                entity->state = PrintBlock_Unknown1;
            }
        }
    }

    if (entity->animator.frameID <= 2) {
        entity->stateCollide = Platform_CollisionState_Null;
        entity->collision    = 4;
    }
    else {
        entity->stateCollide = Platform_CollisionState_AllSolid;
        entity->collision    = 1;
    }
    Platform_Update();
}

void PrintBlock_LateUpdate(void) {}

void PrintBlock_StaticUpdate(void) {}

void PrintBlock_Draw(void)
{
    RSDK_THIS(PrintBlock);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
    entity->inkEffect            = INK_SUB;
    PrintBlock->animator.frameID = entity->animator.frameID;
    RSDK.DrawSprite(&PrintBlock->animator, &entity->drawPos, false);
    entity->inkEffect = INK_NONE;
}

void PrintBlock_Create(void *data)
{
    RSDK_THIS(PrintBlock);
    entity->collision = 1;
    Platform_Create(0);
    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, entity->letter, &entity->animator, true, 0);
    entity->alpha     = 128;
    entity->drawOrder = Zone->drawOrderLow;
    entity->state     = Platform_State_Normal;
}

void PrintBlock_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        PrintBlock->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PrintBlock.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PrintBlock->aniFrames, 12, &PrintBlock->animator, true, 0);
    PrintBlock->sfxLetter = RSDK.GetSFX("PSZ/Letter.wav");
}

void PrintBlock_Unknown1(void)
{
    RSDK_THIS(PrintBlock);
    ++entity->collapseDelay;
    if (++entity->animator.animationTimer == 3) {
        entity->animator.animationTimer = 0;
        if (entity->animator.frameID < 4)
            entity->animator.frameID++;
    }

    if (entity->collapseDelay == entity->duration)
        entity->state = PrintBlock_Unknown2;
}

void PrintBlock_Unknown2(void)
{
    RSDK_THIS(PrintBlock);
    if (++entity->animator.animationTimer == 3) {
        entity->animator.animationTimer = 0;
        if (entity->animator.frameID <= 1) {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = Platform_State_Normal;
        }
        else {
            entity->animator.frameID--;
        }
    }
}

void PrintBlock_EditorDraw(void) {}

void PrintBlock_EditorLoad(void) {}

void PrintBlock_Serialize(void)
{
    RSDK_EDITABLE_VAR(PrintBlock, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(PrintBlock, VAR_UINT8, letter);
}

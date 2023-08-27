// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CorkscrewPath Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCorkscrewPath *CorkscrewPath;

void CorkscrewPath_Update(void)
{
    RSDK_THIS(CorkscrewPath);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (abs(self->position.x - player->position.x) >> 16 > self->xSize) {
            self->activePlayers &= ~playerID;
            if (player->animator.animationID != ANI_SPRING_CS)
                player->direction &= ~FLIP_Y;
        }
        else {
            int32 corkscrewPos = self->xSize + ((player->position.x - self->position.x) >> 16);
            int32 frame        = 24 * corkscrewPos / self->period;
            int32 yOffset      = self->amplitude * RSDK.Cos1024((corkscrewPos << 10) / self->period);

            if (!(playerID & self->activePlayers)) {
                if (abs(yOffset + self->position.y - player->position.y) >= 0x100000) {
                    self->activePlayers &= ~playerID;
                }
                else if (abs(player->groundVel) > 0x40000 && player->velocity.y > -0x40000 && player->groundedStore) {
                    player->position.y = self->position.y + yOffset;
                    player->velocity.y = 0;
                    player->onGround   = true;

                    if (player->animator.animationID != ANI_JUMP) {
                        if (player->groundVel < 0) {
                            player->direction |= FLIP_Y;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, CorkscrewPath->frameTable[frame]);
                        }
                        else {
                            player->direction &= ~FLIP_Y;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, frame);
                        }
                    }
                }
                else {
                    self->activePlayers &= ~playerID;
                }
            }
            else if (abs(player->groundVel) > 0x40000 && player->groundedStore && abs(yOffset + self->position.y - player->position.y) < 0x100000) {
                self->activePlayers |= playerID;
                player->position.y = yOffset + self->position.y;
                player->velocity.y = 0;
                player->onGround   = true;

                if (player->animator.animationID != ANI_JUMP) {
                    if (player->groundVel < 0) {
                        player->direction |= FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, CorkscrewPath->frameTable[frame]);
                    }
                    else {
                        player->direction &= ~FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, frame);
                    }
                }
            }
        }
    }
}

void CorkscrewPath_LateUpdate(void) {}

void CorkscrewPath_StaticUpdate(void) {}

void CorkscrewPath_Draw(void) {}

void CorkscrewPath_Create(void *data)
{
    RSDK_THIS(CorkscrewPath);

    if (!SceneInfo->inEditor) {
        self->amplitude <<= 6;
        self->period        = abs(self->period);
        self->xSize         = self->period >> 1;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = abs(self->period) << 15;
        self->updateRange.y = self->amplitude << 3;
    }
}

void CorkscrewPath_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void CorkscrewPath_EditorDraw(void)
{
    RSDK_THIS(CorkscrewPath);
    RSDK.SetSpriteAnimation(CorkscrewPath->aniFrames, 0, &self->animator, true, 4);
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Bounds

    Vector2 size;
    size.x = abs(self->period) << 15;
    size.y = (self->amplitude << 6) * RSDK.Cos1024(0);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, size.x << 1, size.y << 1, 0xFFFF00);
}

void CorkscrewPath_EditorLoad(void) { CorkscrewPath->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void CorkscrewPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, period);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, amplitude);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, angle);
}

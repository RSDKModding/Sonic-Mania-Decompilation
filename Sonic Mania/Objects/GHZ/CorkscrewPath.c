#include "SonicMania.h"

ObjectCorkscrewPath *CorkscrewPath;

void CorkscrewPath_Update(void)
{
    RSDK_THIS(CorkscrewPath);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (abs(self->position.x - player->position.x) >> 16 > self->periodShifted) {
            self->activePlayers &= ~playerID;
            if (player->animator.animationID != ANI_SPRINGCS)
                player->direction &= ~FLIP_Y;
        }
        else {
            int32 x     = self->periodShifted + ((player->position.x - self->position.x) >> 16);
            int32 frame = 24 * x / self->period;
            int32 arc   = self->amplitude * RSDK.Cos1024((x << 10) / self->period);
            if (!(playerID & self->activePlayers)) {
                if (abs(arc + self->position.y - player->position.y) >= 0x100000) {
                    self->activePlayers &= ~playerID;
                }
                else {
                    if (abs(player->groundVel) > 0x40000 && player->velocity.y > -0x40000 && player->groundedStore) {
                        player->position.y = arc + self->position.y;
                        player->velocity.y = 0;
                        player->onGround   = true;
                        if (player->animator.animationID != ANI_JUMP) {
                            if (player->groundVel < 0) {
                                player->direction |= FLIP_Y;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, CorkscrewPath->frameTable[frame]);
                            }
                            else {
                                player->direction &= ~FLIP_Y;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);
                            }
                        }
                    }
                    else {
                        self->activePlayers &= ~playerID;
                    }
                }
            }
            else if (abs(player->groundVel) > 0x40000 && player->groundedStore && abs(arc + player->position.y - player->position.y) < 0x100000) {
                self->activePlayers |= playerID;
                player->position.y = arc + self->position.y;
                player->velocity.y = 0;
                player->onGround   = true;
                if (player->animator.animationID != ANI_JUMP) {
                    if (player->groundVel < 0) {
                        player->direction |= FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, CorkscrewPath->frameTable[frame]);
                    }
                    else {
                        player->direction &= ~FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);
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
        self->periodShifted = abs(self->period) >> 1;
        self->period        = abs(self->period);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = abs(self->period) << 15;
        self->updateRange.y = 4 * self->amplitude;
    }
}

void CorkscrewPath_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void CorkscrewPath_EditorDraw(void)
{
    RSDK_THIS(CorkscrewPath);
    RSDK.SetSpriteAnimation(CorkscrewPath->aniFrames, 0, &self->animator, true, 4);
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Bounds
    Vector2 drawPos;

    Vector2 size;
    size.x = abs(self->period) << 15;
    size.y = (self->amplitude << 6) * RSDK.Cos1024(0);

    RSDK.DrawLine(self->position.x - size.x, self->position.y - size.y, self->position.x + size.x, self->position.y - size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(self->position.x - size.x, self->position.y + size.y, self->position.x + size.x, self->position.y + size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(self->position.x - size.x, self->position.y - size.y, self->position.x - size.x, self->position.y + size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(self->position.x + size.x, self->position.y - size.y, self->position.x + size.x, self->position.y + size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
}

void CorkscrewPath_EditorLoad(void) { CorkscrewPath->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void CorkscrewPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, period);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, amplitude);
    RSDK_EDITABLE_VAR(CorkscrewPath, VAR_ENUM, angle);
}

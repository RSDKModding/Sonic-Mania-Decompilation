#include "SonicMania.h"

ObjectCorkscrewPath *CorkscrewPath;

void CorkscrewPath_Update(void)
{
    RSDK_THIS(CorkscrewPath);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (abs(entity->position.x - player->position.x) >> 16 > entity->periodShifted) {
            entity->activePlayers &= ~playerID;
            if (player->playerAnimator.animationID != ANI_SPRINGCS)
                player->direction &= ~FLIP_Y;
        }
        else {
            int32 x     = entity->periodShifted + ((player->position.x - entity->position.x) >> 16);
            int32 frame = 24 * x / entity->period;
            int32 arc   = entity->amplitude * RSDK.Cos1024((x << 10) / entity->period);
            if (!(playerID & entity->activePlayers)) {
                if (abs(arc + entity->position.y - player->position.y) >= 0x100000) {
                    entity->activePlayers &= ~playerID;
                }
                else {
                    if (abs(player->groundVel) > 0x40000 && player->velocity.y > -0x40000 && player->groundedStore) {
                        player->position.y = arc + entity->position.y;
                        player->velocity.y = 0;
                        player->onGround   = true;
                        if (player->playerAnimator.animationID != ANI_JUMP) {
                            if (player->groundVel < 0) {
                                player->direction |= FLIP_Y;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, true, CorkscrewPath->frameTable[frame]);
                            }
                            else {
                                player->direction &= ~FLIP_Y;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, true, frame);
                            }
                        }
                    }
                    else {
                        entity->activePlayers &= ~playerID;
                    }
                }
            }
            else if (abs(player->groundVel) > 0x40000 && player->groundedStore && abs(arc + player->position.y - player->position.y) < 0x100000) {
                entity->activePlayers |= playerID;
                player->position.y = arc + entity->position.y;
                player->velocity.y = 0;
                player->onGround   = true;
                if (player->playerAnimator.animationID != ANI_JUMP) {
                    if (player->groundVel < 0) {
                        player->direction |= FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, true, CorkscrewPath->frameTable[frame]);
                    }
                    else {
                        player->direction &= ~FLIP_Y;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, true, frame);
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
        entity->amplitude <<= 6;
        entity->periodShifted = abs(entity->period) >> 1;
        entity->period        = abs(entity->period);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = abs(entity->period) << 15;
        entity->updateRange.y = 4 * entity->amplitude;
    }
}

void CorkscrewPath_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void CorkscrewPath_EditorDraw(void)
{
    RSDK_THIS(CorkscrewPath);
    RSDK.SetSpriteAnimation(CorkscrewPath->aniFrames, 0, &entity->animator, true, 4);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    // Bounds
    Vector2 drawPos;

    Vector2 size;
    size.x = abs(entity->period) << 15;
    size.y = (entity->amplitude << 6) * RSDK.Cos1024(0);

    RSDK.DrawLine(entity->position.x - size.x, entity->position.y - size.y, entity->position.x + size.x, entity->position.y - size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(entity->position.x - size.x, entity->position.y + size.y, entity->position.x + size.x, entity->position.y + size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(entity->position.x - size.x, entity->position.y - size.y, entity->position.x - size.x, entity->position.y + size.y, 0xFFFF00, 0xFF,
                  INK_NONE, false);
    RSDK.DrawLine(entity->position.x + size.x, entity->position.y - size.y, entity->position.x + size.x, entity->position.y + size.y, 0xFFFF00, 0xFF,
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

#include "SonicMania.h"

ObjectDashLift *DashLift;

void DashLift_Update(void) { Platform_Update(); }

void DashLift_LateUpdate(void) {}

void DashLift_StaticUpdate(void) {}

void DashLift_Draw(void)
{
    RSDK_THIS(DashLift);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void DashLift_Create(void *data)
{
    RSDK_THIS(DashLift);

    entity->collision = PLATFORM_C_1;
    Platform_Create(NULL);

    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder = Zone->playerDrawHigh;
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &entity->animator, true, 4);

        entity->drawPos.y += entity->startOff << 15;
        entity->updateRange.y = entity->length << 15;
        entity->amplitude.x   = entity->position.y - (entity->length << 15);
        entity->amplitude.y   = entity->position.y + (entity->length << 15);

        entity->updateRange.x = 0x800000;
        entity->state         = DashLift_State_Setup;
    }
}

void DashLift_StageLoad(void) { DashLift->sfxPulley = RSDK.GetSFX("Stage/Pulley.wav"); }

void DashLift_CheckOnScreen(void)
{
    RSDK_THIS(DashLift);

    if (!RSDK.CheckPosOnScreen(&entity->drawPos, &entity->updateRange) && !RSDK.CheckPosOnScreen(&entity->centerPos, &entity->updateRange)) {
        entity->drawPos.x     = entity->centerPos.x;
        entity->drawPos.y     = entity->centerPos.y;
        entity->position.x    = entity->centerPos.x;
        entity->position.y    = entity->centerPos.y;
        entity->activePlayers = 0;
        entity->active        = ACTIVE_BOUNDS;
        DashLift_Create(NULL);
    }
}

void DashLift_State_Setup(void)
{
    RSDK_THIS(DashLift);
    entity->active = ACTIVE_NORMAL;
    entity->state  = DashLift_State_HandleDash;
}

void DashLift_State_HandleDash(void)
{
    RSDK_THIS(DashLift);

    int totalSpeed = 0;
    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & entity->stoodPlayers)) {
            if (((1 << playerID) & entity->activePlayers)) {
                player->position.x = entity->position.x;
                entity->activePlayers &= ~(1 << playerID);
                if (player->playerAnimator.animationID == ANI_JUMP)
                    player->velocity.x = 0;
            }
        }
        else {
            int anim = player->playerAnimator.animationID;
            if (anim == ANI_SPINDASH || (anim == ANI_JUMP && ((1 << playerID) & entity->activePlayers))) {
                entity->activePlayers |= (1 << playerID);

                foreach_active(Dust, dust)
                {
                    if (dust->parent == (Entity *)player)
                        destroyEntity(dust);
                }

                player->velocity.x -= player->velocity.x >> 5;
                player->position.x = entity->position.x;
                if (player->direction == FLIP_X) {
                    if (player->sidekick && totalSpeed < 0 || entity->drawPos.y >= entity->amplitude.y)
                        continue;

                    int speed = 0;
                    if (player->state == Player_State_Spindash) {
                        if (player->superState == SUPERSTATE_SUPER)
                            speed = ((player->abilityTimer >> 1) & 0x7FFF8000) + 0xB0000;
                        else
                            speed = ((player->abilityTimer >> 1) & 0x7FFF8000) + 0x80000;
                    }
                    else {
                        speed = -player->groundVel;
                    }

                    speed -= 0x40000;
                    if (speed < 0) {
                        speed              = 0;
                        player->groundVel  = 0;
                        player->velocity.x = 0;
                    }

                    entity->drawPos.y += speed >> 2;
                    totalSpeed += speed >> 2;
                    entity->animator.animationTimer += abs((speed >> 2) >> 16);
                    if (entity->animator.animationTimer >= 4) {
                        entity->animator.animationTimer = 0;
                        if (++entity->animator.frameID > 4)
                            entity->animator.frameID = 0;
                    }
                    if (!(Zone->timer & 0xF))
                        RSDK.PlaySfx(DashLift->sfxPulley, false, 255);
                }
                else {
                    if (player->sidekick && totalSpeed > 0 || entity->drawPos.y <= entity->amplitude.x)
                        continue;

                    int speed = 0;
                    if (player->state == Player_State_Spindash) {
                        if (player->superState == SUPERSTATE_SUPER)
                            speed = -((player->abilityTimer >> 1) & 0x7FFF8000) - 0xB0000;
                        else
                            speed = -((player->abilityTimer >> 1) & 0x7FFF8000) - 0x80000;
                    }
                    else {
                        speed = -player->groundVel;
                    }

                    speed += 0x40000;
                    if (speed > 0) {
                        speed              = 0;
                        player->groundVel  = 0;
                        player->velocity.x = 0;
                    }

                    entity->drawPos.y += speed >> 2;
                    totalSpeed += speed >> 2;
                    entity->animator.animationTimer -= abs((speed >> 2) >> 16);
                    if (entity->animator.animationTimer <= 0) {
                        entity->animator.animationTimer = 3;
                        if (--entity->animator.frameID < 0)
                            entity->animator.frameID = 4;
                    }
                    if (!(Zone->timer & 0xF))
                        RSDK.PlaySfx(DashLift->sfxPulley, false, 255);
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
    }

    DashLift_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void DashLift_EditorDraw(void)
{
    RSDK_THIS(DashLift);

    entity->drawPos = entity->position;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &entity->animator, true, 4);

    DashLift_Draw();
}

void DashLift_EditorLoad(void) {}
#endif

void DashLift_Serialize(void)
{
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, startOff);
}

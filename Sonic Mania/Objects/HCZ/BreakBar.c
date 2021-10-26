#include "SonicMania.h"

ObjectBreakBar *BreakBar;

void BreakBar_Update(void)
{
    RSDK_THIS(BreakBar);
    StateMachine_Run(entity->state);
}

void BreakBar_LateUpdate(void) { BreakBar_CheckPlayerCollisions(); }

void BreakBar_StaticUpdate(void) {}

void BreakBar_Draw(void) { BreakBar_DrawSprites(); }

void BreakBar_Create(void *data)
{
    RSDK_THIS(BreakBar);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    entity->hitbox.left   = 24;
    entity->hitbox.top    = 24;
    entity->hitbox.right  = 32;
    entity->hitbox.bottom = 32;
    if (!entity->orientation) {
        entity->hitbox.top    = -8 - ((8 * entity->length) >> 1);
        entity->hitbox.bottom = ((8 * entity->length) >> 1) + 8;
    }
    else {
        entity->hitbox.left  = -8 - ((8 * entity->length) >> 1);
        entity->hitbox.right = ((8 * entity->length) >> 1) + 8;
    }
    entity->state = BreakBar_State_Setup;
}

void BreakBar_StageLoad(void)
{
    BreakBar->aniFrames = RSDK.LoadSpriteAnimation("HCZ/BreakBar.bin", SCOPE_STAGE);
    BreakBar->sfxBreak  = RSDK.GetSFX("Stage/LedgeBreak.wav");
}

void BreakBar_DrawSprites(void)
{
    RSDK_THIS(BreakBar);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->orientation) {
        drawPos.x += -0x40000 - (((8 * entity->length) >> 1) << 16);
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &entity->animator, true, 0);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        drawPos.x += 0x80000;
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &entity->animator, true, 1);
        for (int32 i = 0; i < entity->length; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.x += 0x80000;
        }

        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &entity->animator, true, 2);
    }
    else {
        drawPos.y += -0x40000 - (((8 * entity->length) >> 1) << 16);
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &entity->animator, true, 0);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &entity->animator, true, 1);
        for (int32 i = 0; i < entity->length; ++i) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.y += 0x80000;
        }

        RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &entity->animator, true, 2);
    }
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void BreakBar_CheckPlayerCollisions(void)
{
    RSDK_THIS(BreakBar);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (((1 << playerID) & entity->activePlayersGrabbed) && !((1 << playerID) & entity->activePlayersReleased)) {
            if (!Player_CheckValidState(player)) {
                entity->activePlayersGrabbed &= ~(1 << playerID);
            }
            else {
                player->direction = FLIP_NONE;
                if (entity->orientation) {
                    player->position.y = entity->startPos.y;
                    if (player->velocity.y <= 0) {
                        player->position.y -= 0x140000;
                        player->rotation = 384;
                    }
                    else {
                        player->position.y += 0x140000;
                        player->rotation = 128;
                    }
                }
                else {
                    player->position.x = entity->startPos.x;
                    if (player->velocity.x <= 0) {
                        player->position.x -= 0x140000;
                        player->rotation = 256;
                    }
                    else {
                        player->position.x += 0x140000;
                        player->rotation = 0;
                    }
                }
            }
        }
    }
}

void BreakBar_HandlePlayerInteractions(void *p)
{
    RSDK_THIS(BreakBar);
    EntityPlayer *player = (EntityPlayer *)p;

    if (!entity->destroyFlag) {
        int32 spawnX = entity->position.x;
        int32 spawnY = entity->position.y;

        if (entity->orientation)
            spawnX += -0x40000 - ((8 * entity->length) >> 1 << 16);
        else
            spawnY += -0x40000 - ((8 * entity->length) >> 1 << 16);

        if (entity->length != 0xFFFE) {
            int32 len = (entity->length + 2) << 19;
            for (int32 i = 0; i < entity->length + 2; ++i) {
                int32 frame = 0;
                if (i == entity->length + 1)
                    frame = 2;
                else
                    frame = i != 0;
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, spawnX, spawnY);
                debris->drawOrder    = Zone->drawOrderLow;
                if (player->underwater)
                    debris->gravity = 0x2000;
                else
                    debris->gravity = 0x3800;
                debris->velocity.x = player->velocity.x >> 3;
                debris->velocity.y = player->velocity.y >> 3;

                if (entity->orientation) {
                    RSDK.SetSpriteAnimation(BreakBar->aniFrames, 1, &debris->animator, true, frame);
                    debris->velocity.x = abs(debris->position.x - player->position.x) >> 6;
                    if (debris->position.x < player->position.x)
                        debris->velocity.x = -(abs(debris->position.x - player->position.x) >> 6);
                    if (len > abs(debris->position.x - player->position.x)) {
                        debris->velocity.y += (2 * (player->velocity.y > 0) - 1) * (maxVal(len - abs(debris->position.x - player->position.x), 0) >> 5);
                    }
                    debris->velocity.y = (3 * ((RSDK.Rand(-12, 12) << 10) + debris->velocity.y)) >> 3;
                    spawnX += 0x80000;
                }
                else {
                    RSDK.SetSpriteAnimation(BreakBar->aniFrames, 0, &debris->animator, true, frame);
                    debris->velocity.y = abs(debris->position.y - player->position.y) >> 6;
                    if (debris->position.y < player->position.y)
                        debris->velocity.y = -debris->velocity.y;

                    if (len > abs(debris->position.y - player->position.y)) {
                        debris->velocity.x += (2 * (player->velocity.x > 0) - 1) * (maxVal(len - abs(debris->position.y - player->position.y), 0) >> 5);
                    }

                    debris->velocity.x = (3 * ((RSDK.Rand(-12, 12) << 10) + debris->velocity.x)) >> 3;
                    spawnY += 0x80000;
                }
            }
        }
        RSDK.PlaySfx(BreakBar->sfxBreak, false, 255);
        entity->destroyFlag = true;
    }
}

void BreakBar_State_Setup(void)
{
    RSDK_THIS(BreakBar);
    entity->activePlayersGrabbed  = 0;
    entity->activePlayersReleased = 0;
    entity->releaseTimer          = 0;
    entity->state                 = BreakBar_State_Main;
}

void BreakBar_State_Main(void)
{
    RSDK_THIS(BreakBar);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((player->sidekick && Player->jumpInDelay >= 239) || !Player_CheckValidState(player)) {
            entity->activePlayersReleased &= ~(1 << playerID);
            entity->activePlayersGrabbed &= ~(1 << playerID);
            entity->playerTimers[playerID] = 8;
        }
        else {
            if (entity->playerTimers[playerID]) {
                entity->playerTimers[playerID]--;
            }
            else if (!((1 << playerID) & entity->activePlayersGrabbed) && !((1 << playerID) & entity->activePlayersReleased)) {
                entity->direction = FLIP_NONE;
                if (player->velocity.x < 0)
                    entity->direction = FLIP_X;
                if (player->velocity.y < 0)
                    entity->direction |= FLIP_Y;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    entity->activePlayersGrabbed |= 1 << playerID;
                    RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                }
            }

            if (((1 << playerID) & entity->activePlayersGrabbed)) {
                if (!((1 << playerID) & entity->activePlayersReleased)) {
                    if (player->jumpPress || entity->releaseTimer >= 240 || entity->destroyFlag) {
                        entity->activePlayersReleased |= (1 << playerID);
                    }
                    else {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_CLING, &player->playerAnimator, false, 0);
                        player->onGround        = false;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->state           = Player_State_None;
                    }
                    if (!player->sidekick && globals->gameMode != MODE_COMPETITION)
                        ++entity->releaseTimer;
                }
            }

            if (((1 << playerID) & entity->activePlayersReleased)) {
                if (globals->gameMode == MODE_COMPETITION) {
                    entity->activePlayersReleased &= ~(1 << playerID);
                    entity->activePlayersGrabbed &= ~(1 << playerID);
                    entity->playerTimers[playerID] = 8;
                }
                else if (!player->sidekick && !entity->destroyFlag) {
                    BreakBar_HandlePlayerInteractions(player);
                }
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                player->state = Player_State_None;
            }
        }
    }

    if (entity->destroyFlag)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void BreakBar_EditorDraw(void) {}

void BreakBar_EditorLoad(void) {}
#endif

void BreakBar_Serialize(void)
{
    RSDK_EDITABLE_VAR(BreakBar, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(BreakBar, VAR_UINT16, length);
}

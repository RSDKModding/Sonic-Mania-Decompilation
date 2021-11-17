#include "SonicMania.h"

ObjectTVPole *TVPole;

void TVPole_Update(void)
{
    RSDK_THIS(TVPole);
    StateMachine_Run(entity->state);
}

void TVPole_LateUpdate(void) {}

void TVPole_StaticUpdate(void) {}

void TVPole_Draw(void)
{
    RSDK_THIS(TVPole);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += -0x40000 - ((8 * entity->length) >> 1 << 16);
    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x += 0x80000;
    entity->animator.frameID = 1;
    for (int i = 0; i < entity->length; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void TVPole_Create(void *data)
{
    RSDK_THIS(TVPole);

    entity->active    = ACTIVE_BOUNDS;
    entity->drawOrder = Zone->drawOrderLow;
    entity->visible   = true;
    entity->drawFX    = FX_FLIP;
    if (!SceneInfo->inEditor) {
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;

        int len               = (8 * entity->length) >> 1;
        entity->hitbox.left   = -8 - len;
        entity->hitbox.right  = len + 8;
        entity->hitbox.top    = -32;
        entity->hitbox.bottom = -24;
        RSDK.SetSpriteAnimation(TVPole->aniFrames, 1, &entity->animator, true, 2);
    }
}

void TVPole_StageLoad(void) { TVPole->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVPole.bin", SCOPE_STAGE); }

void TVPole_State_CheckGrab(void)
{
    RSDK_THIS(TVPole);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerTimers[playerID] > 0)
            entity->playerTimers[playerID]--;

        if (((1 << playerID) & entity->activePlayers) == 0 && !entity->playerTimers[playerID]) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->velocity.y <= 0) {
                entity->activePlayers |= 1 << playerID;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_CLING, &player->playerAnimator, true, 0);
                player->direction ^= FLIP_X;
                if (player->direction == FLIP_NONE)
                    player->rotation = 0x180;
                else
                    player->rotation = 0x80;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->state           = Player_State_None;
            }
        }
        if ((1 << playerID) & entity->activePlayers) {
            player->position.y = entity->position.y - 0x140000;
            if (player->left) {
                player->position.x -= 0x10000;
                int x = entity->position.x + ((entity->hitbox.left + 16) << 16);
                if (player->position.x < x)
                    player->position.x = x;
            }
            else if (player->right) {
                player->position.x += 0x10000;
                int x = entity->position.x + ((entity->hitbox.right - 16) << 16);
                if (player->position.x > x)
                    player->position.x = x;
            }

            if (player->jumpPress) {
                entity->activePlayers &= ~(1 << playerID);
                entity->playerTimers[playerID] = 60;
                player->onGround               = false;
                player->velocity.y             = -0x30000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, true, 0);
                player->state = Player_State_Air;
            }
        }
    }
}

void TVPole_State_ForceRelease(void)
{
    RSDK_THIS(TVPole);

    foreach_active(Player, player)
    {
        int playerID                   = RSDK.GetEntityID(player);
        entity->playerTimers[playerID] = 0;
        if ((1 << playerID) & entity->activePlayers) {
            player->onGround   = false;
            player->velocity.y = -0x30000;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, true, 0);
            player->state = Player_State_Air;
        }
    }

    entity->state         = StateMachine_None;
    entity->activePlayers = 0;
}

#if RETRO_INCLUDE_EDITOR
void TVPole_EditorDraw(void)
{
    RSDK_THIS(TVPole);
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    RSDK.SetSpriteAnimation(TVPole->aniFrames, 1, &entity->animator, true, 2);

    TVPole_Draw();
}

void TVPole_EditorLoad(void) { TVPole->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVPole.bin", SCOPE_STAGE); }
#endif

void TVPole_Serialize(void) { RSDK_EDITABLE_VAR(TVPole, VAR_UINT16, length); }

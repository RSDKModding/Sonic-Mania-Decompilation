#include "SonicMania.h"

ObjectBungee *Bungee;

void Bungee_Update(void)
{
    RSDK_THIS(Bungee);
    if (entity->hasAttatchedPlayer) {
        EntityPlayer *player = (EntityPlayer *)entity->attatchedPlayer;
        entity->timer        = 2;
        if (player && !Player_CheckValidState(player)) {
            entity->attatchedPlayer    = 0;
            entity->hasAttatchedPlayer = false;
        }
        if (entity->attatchedPlayer) {
            RSDK.GetHitbox(&player->playerAnimator, 0);
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;
            player->angle      = 0;
            player->rotation   = 0;
            player->drawFX     = FX_ROTATE | FX_FLIP;
            player->position   = entity->bungeePos;
            player->position.y += 0x1A0000;
        }
        if (entity->velocity.y <= 0) {
            entity->field_80 -= 2048;
            if (entity->bungeePos.y <= entity->startPos.y) {
                entity->bungeePos.x        = entity->startPos.x;
                entity->bungeePos.y        = entity->startPos.y;
                entity->hasAttatchedPlayer = false;
                if (entity->attatchedPlayer) {
                    player->tileCollisions = true;
                    player->velocity       = entity->velocity;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                    player->rotation                      = 0;
                    player->playerAnimator.animationSpeed = 48;
                    player->onGround                      = false;
                    player->state                         = Player_State_Air;
                }
            }
        }
        else {
            entity->field_80 = -0x2C00;
        }
        entity->velocity.y += entity->field_80;
        entity->bungeePos.x += entity->velocity.x;
        entity->bungeePos.y += entity->velocity.y;
    }
    else {

        foreach_active(Player, player)
        {
            if (!player->sidekick) {
                if (abs(player->position.y - entity->position.y) < 0x180000) {
                    if (abs(player->position.x - entity->position.x) < 0x180000 && entity->playerY[player->playerID] <= entity->position.y
                        && player->position.y >= entity->position.y) {
                        if (abs(player->groundVel) > 0x20000 || player->velocity.y > 0x20000) {
                            if (player->state != Player_State_None && entity->hasAttatchedPlayer == false && !entity->timer) {
                                entity->field_80           = -0x2C00;
                                entity->timer              = 2;
                                entity->attatchedPlayer    = (Entity *)player;
                                entity->bungeePos.x        = entity->startPos.x;
                                entity->bungeePos.y        = entity->startPos.y;
                                entity->hasAttatchedPlayer = true;
                                entity->velocity.x         = 0;

                                int velocity = 0;
                                if (player->onGround)
                                    velocity = abs(player->groundVel);
                                else
                                    velocity = player->velocity.y;

                                entity->velocity.y = velocity >> 1;
                                player->velocity.x = 0;
                                player->velocity.y = 0;
                                player->groundVel  = 0;
                                player->angle      = 0;
                                player->rotation   = 0;
                                player->position   = entity->bungeePos;
                                player->position.y += 0x1A0000;
                                player->onGround        = false;
                                player->nextGroundState = 0;
                                player->nextAirState    = 0;
                                player->tileCollisions  = false;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BUNGEE, &player->playerAnimator, true, 0);
                                player->state = Player_State_None;
                                RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                                entity->playerDeathBoundary[player->playerID] = Zone->deathBoundary[player->playerID];
                                Zone->deathBoundary[player->playerID]         = 0x7FFFFFFF;
                            }
                        }
                    }
                }
            }
            entity->playerY[player->playerID] = player->position.y;
        }
    }

    if (entity->timer) {
        entity->timer--;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)entity->attatchedPlayer;
        if (player) {
            Zone->deathBoundary[player->playerID]         = entity->playerDeathBoundary[player->playerID];
            entity->playerDeathBoundary[player->playerID] = 0;
            entity->attatchedPlayer                       = NULL;
        }
    }
}

void Bungee_LateUpdate(void) {}

void Bungee_StaticUpdate(void) {}

void Bungee_Draw(void)
{
    RSDK_THIS(Bungee);
    if (entity->hasAttatchedPlayer) {
        EntityPlayer *player = (EntityPlayer *)entity->attatchedPlayer;
        Vector2 drawPos      = entity->startPos;

        int playerY = player->position.y;
        for (int i = 0; i < 9; ++i) {
            int offset = i * ((playerY - 0x1A0000 - entity->startPos.y) >> 3);
            drawPos.x  = entity->startPos.x & 0xFFFF0000;
            drawPos.y  = (entity->startPos.y + offset) & 0xFFFF0000;
            RSDK.DrawSprite(&Bungee->animator, &drawPos, false);
        }
    }
    else {
        RSDK.DrawSprite(&Bungee->animator, NULL, false);
    }
}

void Bungee_Create(void *data)
{
    RSDK_THIS(Bungee);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x2000000;
}

void Bungee_StageLoad(void)
{
    Bungee->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Bungee.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(Bungee->aniFrames, 0, &Bungee->animator, true, 0);
}

void Bungee_EditorDraw(void)
{
    RSDK_THIS(Bungee);
    RSDK.DrawSprite(&Bungee->animator, NULL, false);
}

void Bungee_EditorLoad(void) { Bungee_StageLoad(); }

void Bungee_Serialize(void) {}

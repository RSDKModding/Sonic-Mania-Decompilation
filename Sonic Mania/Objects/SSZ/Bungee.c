#include "SonicMania.h"

ObjectBungee *Bungee;

void Bungee_Update(void)
{
    RSDK_THIS(Bungee);
    if (self->hasAttatchedPlayer) {
        EntityPlayer *player = (EntityPlayer *)self->attatchedPlayer;
        self->timer        = 2;
        if (player && !Player_CheckValidState(player)) {
            self->attatchedPlayer    = 0;
            self->hasAttatchedPlayer = false;
        }
        if (self->attatchedPlayer) {
            RSDK.GetHitbox(&player->animator, 0);
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;
            player->angle      = 0;
            player->rotation   = 0;
            player->drawFX     = FX_ROTATE | FX_FLIP;
            player->position   = self->bungeePos;
            player->position.y += 0x1A0000;
        }
        if (self->velocity.y <= 0) {
            self->field_80 -= 2048;
            if (self->bungeePos.y <= self->startPos.y) {
                self->bungeePos.x        = self->startPos.x;
                self->bungeePos.y        = self->startPos.y;
                self->hasAttatchedPlayer = false;
                if (self->attatchedPlayer) {
                    player->tileCollisions = true;
                    player->velocity       = self->velocity;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
                    player->rotation                      = 0;
                    player->animator.speed = 48;
                    player->onGround                      = false;
                    player->state                         = Player_State_Air;
                }
            }
        }
        else {
            self->field_80 = -0x2C00;
        }
        self->velocity.y += self->field_80;
        self->bungeePos.x += self->velocity.x;
        self->bungeePos.y += self->velocity.y;
    }
    else {

        foreach_active(Player, player)
        {
            if (!player->sidekick) {
                if (abs(player->position.y - self->position.y) < 0x180000) {
                    if (abs(player->position.x - self->position.x) < 0x180000 && self->playerY[player->playerID] <= self->position.y
                        && player->position.y >= self->position.y) {
                        if (abs(player->groundVel) > 0x20000 || player->velocity.y > 0x20000) {
                            if (player->state != Player_State_None && self->hasAttatchedPlayer == false && !self->timer) {
                                self->field_80           = -0x2C00;
                                self->timer              = 2;
                                self->attatchedPlayer    = (Entity *)player;
                                self->bungeePos.x        = self->startPos.x;
                                self->bungeePos.y        = self->startPos.y;
                                self->hasAttatchedPlayer = true;
                                self->velocity.x         = 0;

                                int velocity = 0;
                                if (player->onGround)
                                    velocity = abs(player->groundVel);
                                else
                                    velocity = player->velocity.y;

                                self->velocity.y = velocity >> 1;
                                player->velocity.x = 0;
                                player->velocity.y = 0;
                                player->groundVel  = 0;
                                player->angle      = 0;
                                player->rotation   = 0;
                                player->position   = self->bungeePos;
                                player->position.y += 0x1A0000;
                                player->onGround        = false;
                                player->nextGroundState = 0;
                                player->nextAirState    = 0;
                                player->tileCollisions  = false;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_BUNGEE, &player->animator, true, 0);
                                player->state = Player_State_None;
                                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                                self->playerDeathBoundary[player->playerID] = Zone->deathBoundary[player->playerID];
                                Zone->deathBoundary[player->playerID]         = 0x7FFFFFFF;
                            }
                        }
                    }
                }
            }
            self->playerY[player->playerID] = player->position.y;
        }
    }

    if (self->timer) {
        self->timer--;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)self->attatchedPlayer;
        if (player) {
            Zone->deathBoundary[player->playerID]         = self->playerDeathBoundary[player->playerID];
            self->playerDeathBoundary[player->playerID] = 0;
            self->attatchedPlayer                       = NULL;
        }
    }
}

void Bungee_LateUpdate(void) {}

void Bungee_StaticUpdate(void) {}

void Bungee_Draw(void)
{
    RSDK_THIS(Bungee);
    if (self->hasAttatchedPlayer) {
        EntityPlayer *player = (EntityPlayer *)self->attatchedPlayer;
        Vector2 drawPos      = self->startPos;

        int playerY = player->position.y;
        for (int i = 0; i < 9; ++i) {
            int offset = i * ((playerY - 0x1A0000 - self->startPos.y) >> 3);
            drawPos.x  = self->startPos.x & 0xFFFF0000;
            drawPos.y  = (self->startPos.y + offset) & 0xFFFF0000;
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
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->drawOrder     = Zone->drawOrderHigh;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x2000000;
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

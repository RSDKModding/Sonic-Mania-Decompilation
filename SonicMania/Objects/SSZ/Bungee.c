// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bungee Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBungee *Bungee;

void Bungee_Update(void)
{
    RSDK_THIS(Bungee);

    if (self->hasAttachedPlayer) {
        EntityPlayer *player = self->attachedPlayer;

        self->timer = 2;
#if GAME_VERSION != VER_100
        if (player && !Player_CheckValidState(player)) {
            self->attachedPlayer    = NULL;
            self->hasAttachedPlayer = false;
        }
        if (self->attachedPlayer) {
#endif
            RSDK.GetHitbox(&player->animator, 0); // yeah

            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;
            player->angle      = 0;
            player->rotation   = 0;
            player->drawFX     = FX_ROTATE | FX_FLIP;
            player->position   = self->bungeePos;
            player->position.y += 0x1A0000;
#if GAME_VERSION != VER_100
        }
#endif

        if (self->velocity.y <= 0) {
            self->stretchForce -= 0x800;

            if (self->bungeePos.y <= self->startPos.y) {
                self->bungeePos.x        = self->startPos.x;
                self->bungeePos.y        = self->startPos.y;
                self->hasAttachedPlayer = false;

                if (self->attachedPlayer) {
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->velocity       = self->velocity;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);

                    player->rotation       = 0;
                    player->animator.speed = 48;
                    player->onGround       = false;
                    player->state          = Player_State_Air;
                }
            }
        }
        else {
            self->stretchForce = -0x2C00;
        }

        self->velocity.y += self->stretchForce;
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
                            if (player->state != Player_State_Static && !self->hasAttachedPlayer && !self->timer) {
                                self->stretchForce       = -0x2C00;
                                self->timer              = 2;
                                self->attachedPlayer    = player;
                                self->bungeePos.x        = self->startPos.x;
                                self->bungeePos.y        = self->startPos.y;
                                self->hasAttachedPlayer = true;
                                self->velocity.x         = 0;

                                int32 velocity = player->onGround ? abs(player->groundVel) : player->velocity.y;

                                self->velocity.y   = velocity >> 1;
                                player->velocity.x = 0;
                                player->velocity.y = 0;
                                player->groundVel  = 0;
                                player->angle      = 0;
                                player->rotation   = 0;
                                player->position   = self->bungeePos;
                                player->position.y += 0x1A0000;
                                player->onGround        = false;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                player->tileCollisions  = TILECOLLISION_NONE;

                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_BUNGEE, &player->animator, true, 0);
                                player->state = Player_State_Static;
                                RSDK.PlaySfx(Player->sfxGrab, false, 255);

                                self->deathBoundary[player->playerID] = Zone->deathBoundary[player->playerID];
                                Zone->deathBoundary[player->playerID] = 0x7FFFFFFF;
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
        EntityPlayer *player = self->attachedPlayer;
        if (player) {
            Zone->deathBoundary[player->playerID] = self->deathBoundary[player->playerID];
            self->deathBoundary[player->playerID] = 0;
            self->attachedPlayer                 = NULL;
        }
    }
}

void Bungee_LateUpdate(void) {}

void Bungee_StaticUpdate(void) {}

void Bungee_Draw(void)
{
    RSDK_THIS(Bungee);

    if (self->hasAttachedPlayer) {
        EntityPlayer *player = self->attachedPlayer;
        Vector2 drawPos      = self->startPos;

        int32 playerY = player->position.y;
        for (int32 i = 0; i < 9; ++i) {
            int32 offset = i * ((playerY - 0x1A0000 - self->startPos.y) >> 3);

            drawPos.x = self->startPos.x & 0xFFFF0000;
            drawPos.y = (self->startPos.y + offset) & 0xFFFF0000;
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
    self->drawGroup     = Zone->objectDrawGroup[1];
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

#if GAME_INCLUDE_EDITOR
void Bungee_EditorDraw(void) { RSDK.DrawSprite(&Bungee->animator, NULL, false); }

void Bungee_EditorLoad(void) { Bungee_StageLoad(); }
#endif

void Bungee_Serialize(void) {}

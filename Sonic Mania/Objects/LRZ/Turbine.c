#include "SonicMania.h"

ObjectTurbine *Turbine;

void Turbine_Update(void)
{
    RSDK_THIS(Turbine);

    self->angle = (6 * Zone->timer) & 0x1FF;
    RSDK.SetSpriteAnimation(Turbine->aniFrames, self->type, &self->animator, true, (self->angle >> 3) & 7);
    StateMachine_Run(self->state);
}

void Turbine_LateUpdate(void) {}

void Turbine_StaticUpdate(void)
{
    bool32 flag = false;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {

        foreach_active(Turbine, turbine)
        {
            if (!Turbine->playingTurbineSfx) {
                RSDK.PlaySfx(Turbine->sfxTurbine, 34161, 255);
                Turbine->playingTurbineSfx = true;
            }
            flag = true;
            foreach_break;
        }
    }

    if (!flag) {
        if (Turbine->playingTurbineSfx) {
            RSDK.StopSFX(Turbine->sfxTurbine);
            Turbine->playingTurbineSfx = false;
        }
    }
}

void Turbine_Draw(void)
{
    RSDK_THIS(Turbine);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Turbine_Create(void *data)
{
    RSDK_THIS(Turbine);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Turbine->aniFrames, self->type, &self->animator, true, 0);
        if (self->type) {
            if (self->type == 1)
                self->state = Turbine_CheckPlayerCollisions_Hurt;
            else
                self->state = StateMachine_None;
            self->drawOrder = Zone->drawOrderHigh;
        }
        else {
            self->state     = Turbine_CheckPlayerCollisions;
            self->drawOrder = Zone->drawOrderHigh;
        }
    }
}

void Turbine_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ2"))
        Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE);
    Turbine->hitbox1.left      = -16;
    Turbine->hitbox1.right     = 16;
    Turbine->hitbox1.top       = -48;
    Turbine->hitbox1.bottom    = 48;
    Turbine->hitboxHurt.left   = -6;
    Turbine->hitboxHurt.right  = 6;
    Turbine->hitboxHurt.top    = -44;
    Turbine->hitboxHurt.bottom = 44;
    Turbine->sfxTurbine        = RSDK.GetSFX("LRZ/Turbine.wav");
}

void Turbine_CheckPlayerCollisions(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (self->playerTimers[playerID] > 0)
            self->playerTimers[playerID]--;
        if (!((1 << playerID) & self->activePlayers) && !self->playerTimers[playerID] && player->animator.animationID != ANI_HURT
            && player->state != Player_State_None) {
            if (player->animator.animationID != ANI_HURT && player->state != Player_State_None) {
                if (Player_CheckCollisionTouch(player, self, &Turbine->hitbox1)) {
                    self->activePlayers |= 1 << playerID;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->onGround        = false;
                    player->state           = Player_State_None;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, true, 0);
                    player->animator.animationSpeed = 0;

                    if (player->position.y >= self->position.y)
                        self->playerAngles[playerID] = 0x80;
                    else
                        self->playerAngles[playerID] = 0x180;
                    self->playerAngles[playerID] += self->angle & 0x3F;
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                }
            }
        }
        else if ((1 << playerID) & self->activePlayers) {
            player->velocity.x             = 0;
            player->velocity.y             = 0;
            player->groundVel              = 0;
            self->playerAngles[playerID] = (self->playerAngles[playerID] + 6) % 512;
            player->position.x             = self->position.x;
            player->position.y             = self->position.y;

            player->position.y += 0x1700 * RSDK.Sin512(self->playerAngles[playerID]) + 0x20000;
            if ((uint32)(self->playerAngles[playerID] - 0x81) > 0xFF)
                player->drawOrder = Zone->playerDrawLow;
            else
                player->drawOrder = Zone->playerDrawHigh;

            player->animator.frameID = (((self->playerAngles[playerID] >> 1) / 21) + 9) % 12;

            if (player->jumpPress) {
                player->velocity.y = 0x600 * RSDK.Cos512(self->playerAngles[playerID]);
                if (player->velocity.y < 0)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_WALK, &player->animator, true, 0);

                self->activePlayers &= ~(1 << playerID);
                player->state                  = Player_State_Air;
                self->playerTimers[playerID] = 30;
            }
            else if (player->animator.animationID != ANI_POLESWINGH || player->state != Player_State_None) {
                self->activePlayers &= ~(1 << playerID);
                player->state                  = Player_State_Air;
                self->playerTimers[playerID] = 30;
            }
        }
    }
}

void Turbine_CheckPlayerCollisions_Hurt(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Turbine->hitboxHurt)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Turbine_EditorDraw(void)
{
    RSDK_THIS(Turbine);
    RSDK.SetSpriteAnimation(Turbine->aniFrames, self->type, &self->animator, true, 0);
    Turbine_Draw();
}

void Turbine_EditorLoad(void) { Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE); }
#endif

void Turbine_Serialize(void) { RSDK_EDITABLE_VAR(Turbine, VAR_UINT8, type); }

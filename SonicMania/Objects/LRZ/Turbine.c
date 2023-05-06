// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Turbine Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    bool32 playingSfx = false;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {

        foreach_active(Turbine, turbine)
        {
            if (!Turbine->playingTurbineSfx) {
                RSDK.PlaySfx(Turbine->sfxTurbine, 34161, 0xFF);
                Turbine->playingTurbineSfx = true;
            }
            playingSfx = true;
            foreach_break;
        }
    }

    if (!playingSfx) {
        if (Turbine->playingTurbineSfx) {
            RSDK.StopSfx(Turbine->sfxTurbine);
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

        switch (self->type) {
            case TURBINE_HANDLES: self->state = Turbine_State_Handles; break;

            case TURBINE_SPIKES: self->state = Turbine_State_Spikes; break;

            default:
            case TURBINE_WALLDECOR: self->state = StateMachine_None; break;
        }

        self->drawGroup = Zone->objectDrawGroup[1];
    }
}

void Turbine_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ2"))
        Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE);

    Turbine->hitboxHandle.left   = -16;
    Turbine->hitboxHandle.right  = 16;
    Turbine->hitboxHandle.top    = -48;
    Turbine->hitboxHandle.bottom = 48;

    Turbine->hitboxSpikes.left   = -6;
    Turbine->hitboxSpikes.right  = 6;
    Turbine->hitboxSpikes.top    = -44;
    Turbine->hitboxSpikes.bottom = 44;

    Turbine->sfxTurbine = RSDK.GetSfx("LRZ/Turbine.wav");
}

void Turbine_State_Handles(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimers[playerID] > 0)
            self->playerTimers[playerID]--;

        if (!((1 << playerID) & self->activePlayers) && !self->playerTimers[playerID] && player->animator.animationID != ANI_HURT
            && player->state != Player_State_Static) {
            if (player->animator.animationID != ANI_HURT && player->state != Player_State_Static) {
                if (Player_CheckCollisionTouch(player, self, &Turbine->hitboxHandle)) {
                    self->activePlayers |= 1 << playerID;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->onGround        = false;
                    player->state           = Player_State_Static;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLE_SWING_H, &player->animator, true, 0);
                    player->animator.speed = 0;

                    self->playerAngles[playerID] = player->position.y >= self->position.y ? 0x80 : 0x180;
                    self->playerAngles[playerID] += self->angle & 0x3F;

                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                }
            }
        }
        else if ((1 << playerID) & self->activePlayers) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;

            self->playerAngles[playerID] = (self->playerAngles[playerID] + 6) % 512;

            player->position.x = self->position.x;
            player->position.y = self->position.y + (0x1700 * RSDK.Sin512(self->playerAngles[playerID]) + 0x20000);

            if ((uint32)(self->playerAngles[playerID] - 0x81) > 0xFF)
                player->drawGroup = Zone->playerDrawGroup[0];
            else
                player->drawGroup = Zone->playerDrawGroup[1];

            player->animator.frameID = (((self->playerAngles[playerID] >> 1) / 21) + 9) % 12;

            if (player->jumpPress) {
                player->velocity.y = 0x600 * RSDK.Cos512(self->playerAngles[playerID]);
                if (player->velocity.y < 0)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_WALK, &player->animator, true, 0);

                self->activePlayers &= ~(1 << playerID);
                player->state                = Player_State_Air;
                self->playerTimers[playerID] = 30;
            }
            else if (player->animator.animationID != ANI_POLE_SWING_H || player->state != Player_State_Static) {
                self->activePlayers &= ~(1 << playerID);
                player->state                = Player_State_Air;
                self->playerTimers[playerID] = 30;
            }
        }
    }
}

void Turbine_State_Spikes(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Turbine->hitboxSpikes)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Turbine_EditorDraw(void)
{
    RSDK_THIS(Turbine);

    RSDK.SetSpriteAnimation(Turbine->aniFrames, self->type, &self->animator, true, 0);

    Turbine_Draw();
}

void Turbine_EditorLoad(void)
{
    Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Turbine, type);
    RSDK_ENUM_VAR("Handles", TURBINE_HANDLES);
    RSDK_ENUM_VAR("Spikes", TURBINE_SPIKES);
    RSDK_ENUM_VAR("Wall Decor", TURBINE_WALLDECOR);
}
#endif

void Turbine_Serialize(void) { RSDK_EDITABLE_VAR(Turbine, VAR_UINT8, type); }

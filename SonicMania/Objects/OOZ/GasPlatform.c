// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GasPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGasPlatform *GasPlatform;

void GasPlatform_Update(void)
{
    RSDK_THIS(GasPlatform);

    if (self->state == Platform_State_Fixed) {
        switch (self->type) {
            case GASPLATFORM_INTERVAL:
                if (!((Zone->timer + self->intervalOffset) % self->interval))
                    GasPlatform_PopPlatform();
                break;

            case GASPLATFORM_CONTACT: {
                int32 playerID = 0;
                foreach_active(Player, player)
                {
                    if ((1 << playerID) & self->stoodPlayers && !player->sidekick) {
                        if (abs(player->position.x - self->position.x) < 0x40000) {
                            player->position.x = self->position.x;
                            player->state      = Player_State_Static;
                            player->velocity.x = 0;
                            player->groundVel  = 0;

                            GasPlatform_PopPlatform();
                        }
                    }

                    ++playerID;
                }
                break;
            }
        }
    }

    Platform_Update();
}

void GasPlatform_LateUpdate(void) {}

void GasPlatform_StaticUpdate(void) {}

void GasPlatform_Draw(void)
{
    RSDK_THIS(GasPlatform);

    if (self->centerPos.y - self->drawPos.y > 0x180000)
        RSDK.DrawSprite(&self->gasAnimator, NULL, false);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void GasPlatform_Create(void *data)
{
    RSDK_THIS(GasPlatform);

    if (!self->interval)
        self->interval = 1;

    self->frameID   = 2;
    self->collision = PLATFORM_C_PLATFORM;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->gasAnimator, true, 0);

    self->stateCollide = Platform_Collision_Solid;
    self->state        = Platform_State_Fixed;
}

void GasPlatform_StageLoad(void)
{
    GasPlatform->hitboxGas.top    = -16;
    GasPlatform->hitboxGas.left   = -16;
    GasPlatform->hitboxGas.right  = 16;
    GasPlatform->hitboxGas.bottom = 0;

    GasPlatform->range.x = 0x800000;
    GasPlatform->range.y = 0x180000;

    GasPlatform->sfxGasPop = RSDK.GetSfx("OOZ/GasPop.wav");
    GasPlatform->sfxSpring = RSDK.GetSfx("Global/Spring.wav");
}

void GasPlatform_PopPlatform(void)
{
    RSDK_THIS(GasPlatform);

    if (RSDK.CheckOnScreen(self, &GasPlatform->range))
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);

    self->active     = ACTIVE_NORMAL;
    self->velocity.y = -0x96800;
    self->state      = GasPlatform_State_Popped;
}

void GasPlatform_State_Popped(void)
{
    RSDK_THIS(GasPlatform);

    self->drawPos.y += self->velocity.y;

    if (self->drawPos.y >= self->centerPos.y) {
        self->drawPos.y = self->centerPos.y;
        if (self->velocity.y <= 0x10000) {
            self->active = ACTIVE_BOUNDS;
            self->state  = Platform_State_Fixed;
            return;
        }

        self->velocity.y = -(self->velocity.y >> 2);
    }
    else {
        self->velocity.y += 0x3800;
    }

    if (self->centerPos.y - self->drawPos.y > 0x180000) {
        RSDK.ProcessAnimation(&self->gasAnimator);
        int32 storeX     = self->position.x;
        int32 storeY     = self->position.y;
        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &GasPlatform->hitboxGas))
                Player_ElementHurt(player, self, SHIELD_FIRE);
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }

    if (self->type == GASPLATFORM_CONTACT) {
        int32 playerID = 0;
        foreach_active(Player, player)
        {
            int32 launchY = self->centerPos.y - 0x780000;

            if (self->position.y <= launchY) {
                self->position.y = launchY;
                if ((1 << playerID) & self->stoodPlayers) {
                    player->velocity.y = -0x100000;
                    player->state      = Player_State_Air;
                    player->onGround   = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);
                    RSDK.PlaySfx(GasPlatform->sfxSpring, false, 255);
                    self->timer = 240;
                    self->state = GasPlatform_State_SpringCooldown;
                }
            }
            else if ((1 << playerID) & self->stoodPlayers) {
                if (abs(player->position.x - self->position.x) < 0x40000) {
                    player->position.x = self->position.x;
                    player->state      = Player_State_Static;
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                }
            }

            ++playerID;
        }
    }
}

void GasPlatform_State_SpringCooldown(void)
{
    RSDK_THIS(GasPlatform);

    RSDK.ProcessAnimation(&self->gasAnimator);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->centerPos.x;
    self->position.y = self->centerPos.y;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &GasPlatform->hitboxGas))
            Player_ElementHurt(player, self, SHIELD_FIRE);
    }

    self->position.x = storeX;
    self->position.y = storeY;

    if (!--self->timer) {
        self->velocity.y = 0;
        self->state      = GasPlatform_State_Popped;
    }
}

void GasPlatform_State_Shaking(void)
{
    RSDK_THIS(GasPlatform);

    self->drawPos.x = (RSDK.Rand(-1, 1) << 16) + self->centerPos.x;
    self->drawPos.y = (RSDK.Rand(-2, 2) << 16) + self->centerPos.y;

    if (self->timer <= 0) {
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);
        self->active     = ACTIVE_NORMAL;
        self->velocity.y = -0x8C000;
        self->state      = GasPlatform_State_Popped;
    }
    else {
        self->timer--;
    }
}

#if GAME_INCLUDE_EDITOR
void GasPlatform_EditorDraw(void)
{
    RSDK_THIS(GasPlatform);

    self->drawPos = self->position;

    GasPlatform_Draw();
}

void GasPlatform_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GasPlatform, type);
    RSDK_ENUM_VAR("Pop On Interval", GASPLATFORM_INTERVAL);
    RSDK_ENUM_VAR("Pop On Contact", GASPLATFORM_CONTACT); // never used in mania afaik, it's used in S2 though
}
#endif

void GasPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, intervalOffset);
}

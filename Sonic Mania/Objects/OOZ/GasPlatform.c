// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GasPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGasPlatform *GasPlatform;

void GasPlatform_Update(void)
{
    RSDK_THIS(GasPlatform);
    if (self->state == Platform_State_Fixed) {
        switch (self->type) {
            case 0:
                if (!((Zone->timer + self->intervalOffset) % self->interval))
                    GasPlatform_Unknown1();
                break;
            case 1: {
                int32 id = 0;
                foreach_active(Player, player)
                {
                    if ((1 << id) & self->stoodPlayers && !player->sidekick) {
                        if (abs(player->position.x - self->position.x) < 0x40000) {
                            player->position.x = self->position.x;
                            player->state      = Player_State_None;
                            player->velocity.x = 0;
                            player->groundVel  = 0;
                            GasPlatform_Unknown1();
                        }
                    }
                    ++id;
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
        RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void GasPlatform_Create(void *data)
{
    RSDK_THIS(GasPlatform);
    if (!self->interval)
        self->interval = 1;
    self->frameID   = 2;
    self->collision = PLATFORM_C_SOLID_TOP;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator2, true, 0);
    self->stateCollide = Platform_CollisionState_AllSolid;
    self->state        = Platform_State_Fixed;
}

void GasPlatform_StageLoad(void)
{
    GasPlatform->hitbox.top    = -16;
    GasPlatform->hitbox.left   = -16;
    GasPlatform->hitbox.right  = 16;
    GasPlatform->hitbox.bottom = 0;
    GasPlatform->range.x       = 0x800000;
    GasPlatform->range.y       = 0x180000;
    GasPlatform->sfxGasPop     = RSDK.GetSfx("OOZ/GasPop.wav");
    GasPlatform->sfxSpring     = RSDK.GetSfx("Global/Spring.wav");
}

void GasPlatform_Unknown1(void)
{
    RSDK_THIS(GasPlatform);

    if (RSDK.CheckOnScreen(self, &GasPlatform->range))
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);
    self->active     = ACTIVE_NORMAL;
    self->velocity.y = -0x96800;
    self->state      = GasPlatform_Unknown2;
}

void GasPlatform_Unknown2(void)
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

    int32 dist = self->centerPos.y - self->drawPos.y;
    if (dist > 0x180000) {
        RSDK.ProcessAnimation(&self->animator2);
        int32 storeX         = self->position.x;
        int32 storeY         = self->position.y;
        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &GasPlatform->hitbox)) {
                Player_CheckElementalHit(player, self, SHIELD_FIRE);
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }

    if (self->type == 1) {
        int32 id = 0;
        foreach_active(Player, player)
        {
            int32 posY = self->centerPos.y - 0x780000;
            if (self->position.y <= posY) {
                self->position.y = posY;
                if ((1 << id) & self->stoodPlayers) {
                    player->velocity.y = -0x100000;
                    player->state      = Player_State_Air;
                    player->onGround   = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
                    RSDK.PlaySfx(GasPlatform->sfxSpring, false, 255);
                    self->timer = 240;
                    self->state         = GasPlatform_Unknown3;
                }
            }
            else if ((1 << id) & self->stoodPlayers) {
                if (abs(player->position.x - self->position.x) < 0x40000) {
                    player->position.x = self->position.x;
                    player->state      = Player_State_None;
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                }
            }
            ++id;
        }
    }
}

void GasPlatform_Unknown3(void)
{
    RSDK_THIS(GasPlatform);
    RSDK.ProcessAnimation(&self->animator2);
    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->centerPos.x;
    self->position.y = self->centerPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &GasPlatform->hitbox)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    if (!--self->timer) {
        self->velocity.y = 0;
        self->state      = GasPlatform_Unknown2;
    }
}

void GasPlatform_Unknown4(void)
{
    RSDK_THIS(GasPlatform);
    self->drawPos.x = (RSDK.Rand(-1, 1) << 16) + self->centerPos.x;
    self->drawPos.y = (RSDK.Rand(-2, 2) << 16) + self->centerPos.y;
    if (self->timer <= 0) {
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);
        self->active     = ACTIVE_NORMAL;
        self->velocity.y = -0x8C000;
        self->state      = GasPlatform_Unknown2;
    }
    else {
        self->timer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void GasPlatform_EditorDraw(void)
{
    RSDK_THIS(GasPlatform);
    self->drawPos = self->position;

    GasPlatform_Draw();
}

void GasPlatform_EditorLoad(void) {}
#endif

void GasPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, intervalOffset);
}

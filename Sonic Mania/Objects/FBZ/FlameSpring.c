#include "SonicMania.h"

ObjectFlameSpring *FlameSpring;

void FlameSpring_Update(void)
{
    RSDK_THIS(FlameSpring);
    StateMachine_Run(self->state);
}

void FlameSpring_LateUpdate(void) {}

void FlameSpring_StaticUpdate(void) {}

void FlameSpring_Draw(void)
{
    RSDK_THIS(FlameSpring);
    StateMachine_Run(self->stateDraw);
}

void FlameSpring_Create(void *data)
{
    RSDK_THIS(FlameSpring);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow + 1;
        if (data) {
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 2, &self->animator1, true, 0);
            self->state     = FlameSpring_State_Unknown2;
            self->stateDraw = FlameSpring_StateDraw_Unknown2;
        }
        else {
            if (!(self->force & 1))
                self->velocity.y = -0xA0000;
            else
                self->velocity.y = -0x100000;
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 1, &self->animator2, true, 0);
            self->field_6C.x = self->position.x - 768 * RSDK.Sin1024(256);
            self->field_6C.y = self->position.y - 0x40000;
            self->field_74.x = 768 * RSDK.Sin1024(256) + self->position.x;
            self->field_74.y = self->position.y - 0x40000;
            if (self->type > 2)
                self->animator1.frameID = 2;
            self->state     = FlameSpring_State_Unknown1;
            self->type      = self->type % 3;
            self->stateDraw = FlameSpring_StateDraw_Unknown1;
        }
    }
}

void FlameSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        FlameSpring->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FlameSpring.bin", SCOPE_STAGE);
    FlameSpring->hitbox1.left   = -16;
    FlameSpring->hitbox1.top    = -8;
    FlameSpring->hitbox1.right  = 16;
    FlameSpring->hitbox1.bottom = 8;
    FlameSpring->hitbox2.left   = -4;
    FlameSpring->hitbox2.top    = -4;
    FlameSpring->hitbox2.right  = 4;
    FlameSpring->hitbox2.bottom = 4;
    FlameSpring->sfxSpring      = RSDK.GetSfx("Global/Spring.wav");
    FlameSpring->sfxFlame       = RSDK.GetSfx("Stage/Flame2.wav");
}

void FlameSpring_State_Unknown1(void)
{
    RSDK_THIS(FlameSpring);

    self->flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &FlameSpring->hitbox1) == 1 && player->velocity.y >= 0) {
            if (!self->animator1.frameID) {
                self->timer             = 0;
                self->animator1.frameID = 1;
            }
            if (self->animator1.frameID <= 1)
                self->flag = true;
        }
    }

    if (self->animator1.frameID == 1) {
        if (self->flag) {
            if (++self->timer >= 60) {
                foreach_active(Player, player)
                {
                    if (Player_CheckCollisionBox(player, self, &FlameSpring->hitbox1) == 1) {
                        int32 anim = player->animator.animationID;
                        if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                            player->storedAnim = player->animator.animationID;
                        else
                            player->storedAnim = ANI_WALK;
                        if (self->playerAni)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->animator, true, 0);
                        player->state      = Player_State_Air;
                        player->onGround   = false;
                        player->velocity.y = self->velocity.y;
                        if (player->underwater)
                            player->velocity.y >>= 1;
                        RSDK.PlaySfx(FlameSpring->sfxSpring, false, 255);
                    }
                }
            }
        }
        else {
            self->animator1.frameID = 0;
        }
    }
    else {
        if (!(Zone->timer & 7)) {
            RSDK.PlaySfx(FlameSpring->sfxFlame, false, 255);
        }

        int32 timer = Zone->timer + self->offset;
        if (!self->type) {
            self->field_6C.x = self->position.x - 768 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256);
            self->field_74.x = 768 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256) + self->position.x;
        }

        if (!(Zone->timer & 3)) {
            switch (self->type) {
                case 0: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), self->field_6C.x, self->field_6C.y);
                    spring->drawOrder         = Zone->drawOrderLow;
                    spring->velocity.x        = -256 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256);

                    spring             = CREATE_ENTITY(FlameSpring, intToVoid(1), self->field_74.x, self->field_74.y);
                    spring->velocity.x = (RSDK.Sin1024(((4 * timer) & 0x1FF) + 256) << 8);
                    break;
                }
                case 1: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), self->field_74.x, self->field_74.y);
                    spring->drawOrder         = Zone->drawOrderLow;
                    if (192 * RSDK.Sin1024((timer & 0x1FF) + 256) >= 0)
                        spring->velocity.x = (0x44000 - 192 * RSDK.Sin1024((timer & 0x1FF) + 256));
                    else
                        spring->velocity.x = (0x44000 - -192 * RSDK.Sin1024((timer & 0x1FF) + 256));
                    break;
                }
                case 2: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), self->field_6C.x, self->field_6C.y);
                    spring->drawOrder         = Zone->drawOrderLow;
                    if (-192 * RSDK.Sin1024((timer & 0x1FF) + 256) >= 0)
                        spring->velocity.x = (-192 * RSDK.Sin1024((timer & 0x1FF) + 256) - 0x44000);
                    else
                        spring->velocity.x = (192 * RSDK.Sin1024((timer & 0x1FF) + 256) - 0x44000);
                    break;
                }
            }
        }
    }
}

void FlameSpring_State_Unknown2(void)
{
    RSDK_THIS(FlameSpring);

    self->position.x += self->velocity.x;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &FlameSpring->hitbox2)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}

void FlameSpring_StateDraw_Unknown1(void)
{
    RSDK_THIS(FlameSpring);
    if (self->animator1.frameID != 1 && self->type != 1) {
        self->animator2.frameID = self->field_6C.x > self->position.x;
        RSDK.DrawSprite(&self->animator2, &self->field_6C, false);
    }

    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (self->animator1.frameID != 1 && self->type != 2) {
        self->animator2.frameID = self->field_74.x > self->position.x;
        RSDK.DrawSprite(&self->animator2, &self->field_74, false);
    }
}

void FlameSpring_StateDraw_Unknown2(void)
{
    RSDK_THIS(FlameSpring);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void FlameSpring_EditorDraw(void) {}

void FlameSpring_EditorLoad(void) {}
#endif

void FlameSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(FlameSpring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_ENUM, force);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_UINT8, playerAni);
    RSDK_EDITABLE_VAR(FlameSpring, VAR_UINT8, offset);
}

#include "SonicMania.h"

ObjectFlowerPod *FlowerPod;

void FlowerPod_Update(void)
{
    RSDK_THIS(FlowerPod);
    StateMachine_Run(self->state);
}

void FlowerPod_LateUpdate(void) {}

void FlowerPod_StaticUpdate(void) {}

void FlowerPod_Draw(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void FlowerPod_Create(void *data)
{
    RSDK_THIS(FlowerPod);
    if (SceneInfo->inEditor != true) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (data) {
            self->drawFX = FX_ROTATE | FX_FLIP;
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 3, &self->animator1, true, 0);
            self->active = ACTIVE_NORMAL;
            self->state  = FlowerPod_State_SeedFall;
        }
        else {
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 1, &self->animator2, true, 0);
            self->state = FlowerPod_State_Pod;
        }
    }
}

void FlowerPod_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        FlowerPod->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/FlowerPod.bin", SCOPE_STAGE);
    FlowerPod->hitbox.top    = -36;
    FlowerPod->hitbox.left   = -32;
    FlowerPod->hitbox.right  = 32;
    FlowerPod->hitbox.bottom = 0;
    FlowerPod->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    FlowerPod->sfxTwinkle    = RSDK.GetSfx("Global/Twinkle.wav");
}

void FlowerPod_SpawnSeeds(void)
{
    RSDK_THIS(FlowerPod);

    for (int32 v = -0x10000; v < 0x18000; v += 0x8000) {
        EntityFlowerPod *seed = CREATE_ENTITY(FlowerPod, intToVoid(true), self->position.x, self->position.y);
        seed->isPermanent     = true;
        seed->velocity.x      = v;
        seed->drawOrder       = Zone->drawOrderLow;
        seed->velocity.y      = -0x30000;
    }
}

void FlowerPod_State_Pod(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.ProcessAnimation(&self->animator2);
    foreach_active(Player, player)
    {
        if (Player_CheckAttacking(player, self) && Player_CheckBadnikTouch(player, self, &FlowerPod->hitbox))
            self->state = FlowerPod_State_Exploding;
    }
}

void FlowerPod_State_Exploding(void)
{
    RSDK_THIS(FlowerPod);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(FlowerPod->sfxExplosion, false, 255);

        if ((Zone->timer & 4)) {
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), (RSDK.Rand(-32, 32) << 16) + self->position.x,
                              (RSDK.Rand(-32, 0) << 16) + self->position.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 30) {
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 2, &self->animator2, true, 0);
        self->state = 0;
        RSDK.PlaySfx(FlowerPod->sfxTwinkle, false, 255);
        FlowerPod_SpawnSeeds();
        self->timer = 48;
        self->state = FlowerPod_State_Destroyed;
    }
}

void FlowerPod_State_Destroyed(void)
{
    RSDK_THIS(FlowerPod);

    if (self->timer > 0) {
        self->visible = (self->timer & 4) != 0;
        self->timer--;
    }
    else {
        destroyEntity(self);
    }
}

void FlowerPod_State_SeedFall(void)
{
    RSDK_THIS(FlowerPod);
    self->velocity.y += 0x1C00;
    self->rotation = 2 * RSDK.ATan2(self->velocity.x, self->velocity.y);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true)) {
        self->rotation = 0;
        self->timer    = 30;
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 4, &self->animator1, true, 0);
        self->state = FlowerPod_State_SpawnBeanstalk;
    }
    RSDK.ProcessAnimation(&self->animator1);
}

void FlowerPod_State_SpawnBeanstalk(void)
{
    RSDK_THIS(FlowerPod);

    if (self->timer <= 15) {
        Hitbox hitbox;
        hitbox.left   = -4;
        hitbox.top    = -4;
        hitbox.right  = 4;
        hitbox.bottom = 4;
        foreach_active(Beanstalk, beanstalk)
        {
            if (!beanstalk->type && RSDK.CheckObjectCollisionTouchBox(beanstalk, &Beanstalk->hitboxSeed, self, &hitbox)) {
                beanstalk->startGrowth = true;
            }
        }
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 5, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 6, &self->animator1, true, 0);
        self->startPos = self->position;
        self->state    = FlowerPod_State_FlowerGrow;
        RSDK.ProcessAnimation(&self->animator1);
    }
    else {
        self->timer--;
        RSDK.ProcessAnimation(&self->animator1);
    }
}

void FlowerPod_State_FlowerGrow(void)
{
    RSDK_THIS(FlowerPod);

    if (self->position.y <= self->startPos.y - 0x200000) {
        self->active = ACTIVE_BOUNDS;
        self->state  = FlowerPod_State_Flower;
    }
    else {
        self->position.y -= 0x40000;
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

void FlowerPod_State_Flower(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

#if RETRO_INCLUDE_EDITOR
void FlowerPod_EditorDraw(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 1, &self->animator2, false, 0);

    FlowerPod_Draw();
}

void FlowerPod_EditorLoad(void) { FlowerPod->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/FlowerPod.bin", SCOPE_STAGE); }
#endif

void FlowerPod_Serialize(void) {}

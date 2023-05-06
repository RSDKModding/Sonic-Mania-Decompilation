// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSHologram Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSHologram *MSHologram;

void MSHologram_Update(void)
{
    RSDK_THIS(MSHologram);

    self->angle = (self->angle + 4) & 0xFF;

    RSDK.ProcessAnimation(&self->lightsAnimator);
    RSDK.ProcessAnimation(&self->metalSonicAnimator);
    RSDK.ProcessAnimation(&self->rabbitAnimator);
    RSDK.ProcessAnimation(&self->canaryAnimator);

    StateMachine_Run(self->state);
}

void MSHologram_LateUpdate(void) {}

void MSHologram_StaticUpdate(void) {}

void MSHologram_Draw(void)
{
    RSDK_THIS(MSHologram);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->projectorAnimator, NULL, false);

    if (!(Zone->timer & 2)) {
        RSDK.DrawSprite(&self->lightsAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->rabbitAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->metalSonicAnimator, NULL, false);

        if (self->angle < 0x80)
            self->direction = FLIP_X;

        drawPos.x += (RSDK.Cos256(self->angle) - 0x140) << 13;
        drawPos.y += (RSDK.Sin256(self->angle) - 0x1C0) << 12;
        RSDK.DrawSprite(&self->canaryAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
    }
}

void MSHologram_Create(void *data)
{
    RSDK_THIS(MSHologram);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->visible       = true;
            self->drawFX        = FX_FLIP;
            self->drawGroup     = Zone->objectDrawGroup[0];
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 0, &self->projectorAnimator, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 1, &self->lightsAnimator, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 2, &self->metalSonicAnimator, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 3, &self->rabbitAnimator, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 4, &self->canaryAnimator, true, 0);

            self->state = MSHologram_State_CheckPlayerCollisions;
        }
    }
}

void MSHologram_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/MSHologram.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSHologram.bin", SCOPE_STAGE);

    MSHologram->hitbox.left   = -12;
    MSHologram->hitbox.top    = -10;
    MSHologram->hitbox.right  = 12;
    MSHologram->hitbox.bottom = 10;

    MSHologram->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
}

void MSHologram_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(MSHologram);

    foreach_active(Player, player)
    {
        if (player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_DROPDASH) {
            if (Player_CheckBadnikTouch(player, self, &MSHologram->hitbox))
                self->state = MSHologram_State_Explode;
        }
        else {
            Player_CheckCollisionBox(player, self, &MSHologram->hitbox);
        }
    }
}

void MSHologram_State_Explode(void)
{
    RSDK_THIS(MSHologram);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MSHologram->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = RSDK.Rand(-8, 8) << 16;
            int32 y                    = RSDK.Rand(-8, 8) << 16;
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), x + self->position.x, y + self->position.y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }

    if (++self->timer == 80) {
        self->timer     = 0;
        self->destroyed = true;
        self->visible   = false;
        self->state     = MSHologram_State_Destroyed;

        for (int32 i = 0; i < 16; ++i) {
            int32 x               = self->position.x + RSDK.Rand(0x800000, 0xE00000);
            int32 y               = self->position.y - RSDK.Rand(0x200000, 0x800000);
            EntityAnimals *animal = CREATE_ENTITY(Animals, INT_TO_VOID(RSDK.Rand(1, 12)), x, y);
            animal->updateRange.x = 0x1000000;
            animal->updateRange.y = 0x1000000;
            animal->behaviour     = ANIMAL_BEHAVE_FOLLOW;
        }
    }
}

void MSHologram_State_Destroyed(void)
{
    RSDK_THIS(MSHologram);

    if (++self->timer == 384) {
        foreach_active(Animals, animal)
        {
            if (animal->behaviour == ANIMAL_BEHAVE_FOLLOW)
                animal->behaviour = ANIMAL_BEHAVE_FREE;
        }

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void MSHologram_EditorDraw(void)
{
    RSDK_THIS(MSHologram);

    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 0, &self->projectorAnimator, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 1, &self->lightsAnimator, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 2, &self->metalSonicAnimator, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 3, &self->rabbitAnimator, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 4, &self->canaryAnimator, false, 0);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->projectorAnimator, NULL, false);

    RSDK.DrawSprite(&self->lightsAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->rabbitAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->metalSonicAnimator, NULL, false);

    if (self->angle < 128)
        self->direction = FLIP_X;
    drawPos.x += (RSDK.Cos256(self->angle) - 0x140) << 13;
    drawPos.y += (RSDK.Sin256(self->angle) - 0x1C0) << 12;
    RSDK.DrawSprite(&self->canaryAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
}

void MSHologram_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/MSHologram.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSHologram.bin", SCOPE_STAGE);
}
#endif

void MSHologram_Serialize(void) {}

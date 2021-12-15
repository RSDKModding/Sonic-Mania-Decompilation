// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSHologram Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMSHologram *MSHologram;

void MSHologram_Update(void)
{
    RSDK_THIS(MSHologram);
    self->angle = (self->angle + 4) & 0xFF;
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator5);
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
    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (!(Zone->timer & 2)) {
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator3, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        if (self->angle < 128)
            self->direction = FLIP_X;
        drawPos.x += (RSDK.Cos256(self->angle) - 320) << 13;
        drawPos.y += (RSDK.Sin256(self->angle) - 448) << 12;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);

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
            self->drawOrder     = Zone->drawOrderLow;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 1, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 2, &self->animator4, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 3, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 4, &self->animator5, true, 0);
            self->state = MSHologram_State_CheckPlayerCollisions;
        }
    }
}

void MSHologram_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/MSHologram.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSHologram.bin", SCOPE_STAGE);

    MSHologram->hitbox.left   = -12;
    MSHologram->hitbox.top    = -10;
    MSHologram->hitbox.right  = 12;
    MSHologram->hitbox.bottom = 10;
    MSHologram->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
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
            int x                      = RSDK.Rand(-8, 8) << 16;
            int y                      = RSDK.Rand(-8, 8) << 16;
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x + self->position.x, y + self->position.y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 80) {
        self->timer     = 0;
        self->destroyed = true;
        self->visible   = false;
        self->state     = MSHologram_State_Destroyed;
        for (int i = 0; i < 16; ++i) {
            int x                 = self->position.x + RSDK.Rand(0x800000, 0xE00000);
            int y                 = self->position.y - RSDK.Rand(0x200000, 0x800000);
            EntityAnimals *animal = CREATE_ENTITY(Animals, intToVoid(RSDK.Rand(1, 12)), x, y);
            animal->updateRange.x = 0x1000000;
            animal->updateRange.y = 0x1000000;
            animal->behaviour     = 1;
        }
    }
}

void MSHologram_State_Destroyed(void)
{
    RSDK_THIS(MSHologram);

    if (++self->timer == 384) {
        foreach_active(Animals, animal)
        {
            if (animal->behaviour == 1)
                animal->behaviour = 0;
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void MSHologram_EditorDraw(void)
{
    RSDK_THIS(MSHologram);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 1, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 2, &self->animator4, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 3, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(MSHologram->aniFrames, 4, &self->animator5, false, 0);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    if (self->angle < 128)
        self->direction = FLIP_X;
    drawPos.x += (RSDK.Cos256(self->angle) - 320) << 13;
    drawPos.y += (RSDK.Sin256(self->angle) - 448) << 12;
    RSDK.DrawSprite(&self->animator5, &drawPos, false);

    self->direction = FLIP_NONE;
}

void MSHologram_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/MSHologram.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        MSHologram->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSHologram.bin", SCOPE_STAGE);
}
#endif

void MSHologram_Serialize(void) {}

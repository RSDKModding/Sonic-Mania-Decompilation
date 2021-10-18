#include "SonicMania.h"

ObjectMSHologram *MSHologram;

void MSHologram_Update(void)
{
    RSDK_THIS(MSHologram);
    entity->angle = (entity->angle + 4) & 0xFF;
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator5);
    StateMachine_Run(entity->state);
}

void MSHologram_LateUpdate(void) {}

void MSHologram_StaticUpdate(void) {}

void MSHologram_Draw(void)
{
    RSDK_THIS(MSHologram);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (!(Zone->timer & 2)) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        if (entity->angle < 128)
            entity->direction = FLIP_X;
        drawPos.x += (RSDK.Cos256(entity->angle) - 320) << 13;
        drawPos.y += (RSDK.Sin256(entity->angle) - 448) << 12;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        entity->direction = FLIP_NONE;
    }
}

void MSHologram_Create(void *data)
{
    RSDK_THIS(MSHologram);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->visible       = true;
            entity->drawFX        = FX_FLIP;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 1, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 2, &entity->animator4, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 3, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(MSHologram->aniFrames, 4, &entity->animator5, true, 0);
            entity->state = MSHologram_State_CheckPlayerCollisions;
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
    MSHologram->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
}

void MSHologram_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(MSHologram);

    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID == ANI_JUMP || player->playerAnimator.animationID == ANI_DROPDASH) {
            if (Player_CheckBadnikTouch(player, entity, &MSHologram->hitbox))
                entity->state = MSHologram_State_Explode;
        }
        else {
            Player_CheckCollisionBox(player, entity, &MSHologram->hitbox);
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
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 80) {
        entity->timer     = 0;
        entity->destroyed = true;
        entity->visible   = false;
        entity->state     = MSHologram_State_Destroyed;
        for (int i = 0; i < 16; ++i) {
            int x                 = entity->position.x + RSDK.Rand(0x800000, 0xE00000);
            int y                 = entity->position.y - RSDK.Rand(0x200000, 0x800000);
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

    if (++entity->timer == 384) {
        foreach_active(Animals, animal)
        {
            if (animal->behaviour == 1)
                animal->behaviour = 0;
        }
        destroyEntity(entity);
    }
}

void MSHologram_EditorDraw(void) {}

void MSHologram_EditorLoad(void) {}

void MSHologram_Serialize(void) {}

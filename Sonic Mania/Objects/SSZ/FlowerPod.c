#include "SonicMania.h"

ObjectFlowerPod *FlowerPod;

void FlowerPod_Update(void)
{
    RSDK_THIS(FlowerPod);
    StateMachine_Run(entity->state);
}

void FlowerPod_LateUpdate(void) {}

void FlowerPod_StaticUpdate(void) {}

void FlowerPod_Draw(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void FlowerPod_Create(void *data)
{
    RSDK_THIS(FlowerPod);
    if (RSDK_sceneInfo->inEditor != true) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = 1;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (data) {
            entity->drawFX = FX_ROTATE | FX_FLIP;
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 3, &entity->animator1, true, 0);
            entity->active = ACTIVE_NORMAL;
            entity->state  = FlowerPod_State_SeedFall;
        }
        else {
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 1, &entity->animator2, true, 0);
            entity->state = FlowerPod_State_Pod;
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
    FlowerPod->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    FlowerPod->sfxTwinkle    = RSDK.GetSFX("Global/Twinkle.wav");
}

void FlowerPod_SpawnSeeds(void)
{
    RSDK_THIS(FlowerPod);

    for (int v = -0x10000; v < 0x18000; v += 0x8000) {
        EntityFlowerPod *seed = CREATE_ENTITY(FlowerPod, intToVoid(true), entity->position.x, entity->position.y);
        seed->isPermanent     = true;
        seed->velocity.x      = v;
        seed->drawOrder       = Zone->drawOrderLow;
        seed->velocity.y      = -0x30000;
    }
}

void FlowerPod_State_Pod(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.ProcessAnimation(&entity->animator2);
    foreach_active(Player, player)
    {
        int anim    = player->playerAnimator.animationID;
        bool32 flag = anim == ANI_JUMP || anim == ANI_SPINDASH;
        switch (player->characterID) {
            case ID_SONIC:
#if RETRO_USE_PLUS
            case ID_MIGHTY: flag |= anim == ANI_DROPDASH; break;
#endif
            case ID_TAILS:
                if (!flag) {
                    flag = anim == ANI_FLY || anim == ANI_FLYTIRED || anim == ANI_FLYLIFT;
                    if (player->position.y <= entity->position.y)
                        flag = true;
                }
                break;
            case ID_KNUCKLES: flag |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED; break;
            default: break;
        }

        if (flag && Player_CheckBadnikHit(player, entity, &FlowerPod->hitbox))
            entity->state = FlowerPod_State_Exploding;
    }
}

void FlowerPod_State_Exploding(void)
{
    RSDK_THIS(FlowerPod);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySFX(FlowerPod->sfxExplosion, false, 255);

        if ((Zone->timer & 4)) {
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), (RSDK.Rand(-32, 32) << 16) + entity->position.x,
                              (RSDK.Rand(-32, 0) << 16) + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 30) {
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = 0;
        RSDK.PlaySFX(FlowerPod->sfxTwinkle, false, 255);
        FlowerPod_SpawnSeeds();
        entity->timer = 48;
        entity->state = FlowerPod_State_Destroyed;
    }
}

void FlowerPod_State_Destroyed(void)
{
    RSDK_THIS(FlowerPod);

    if (entity->timer > 0) {
        entity->visible = (entity->timer & 4) != 0;
        entity->timer--;
    }
    else {
        destroyEntity(entity);
    }
}

void FlowerPod_State_SeedFall(void)
{
    RSDK_THIS(FlowerPod);
    entity->velocity.y += 0x1C00;
    entity->rotation = 2 * RSDK.ATan2(entity->velocity.x, entity->velocity.y);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true)) {
        entity->rotation = 0;
        entity->timer    = 30;
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 4, &entity->animator1, true, 0);
        entity->state = FlowerPod_State_SpawnBeanstalk;
    }
    RSDK.ProcessAnimation(&entity->animator1);
}

void FlowerPod_State_SpawnBeanstalk(void)
{
    RSDK_THIS(FlowerPod);

    if (entity->timer <= 15) {
        Hitbox hitbox;
        hitbox.left   = -4;
        hitbox.top    = -4;
        hitbox.right  = 4;
        hitbox.bottom = 4;
        foreach_active(Beanstalk, beanStalk)
        {
            // if (!beanStalk->field_5C && RSDK.CheckObjectCollisionTouchBox(beanStalk, &Beanstalk->hitboxSeed, entity, &hitbox)) {
            //    beanStalk->startGrowth = true;
            //}
        }
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 5, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(FlowerPod->aniFrames, 6, &entity->animator1, true, 0);
        entity->startPos = entity->position;
        entity->state    = FlowerPod_State_FlowerGrow;
        RSDK.ProcessAnimation(&entity->animator1);
    }
    else {
        entity->timer--;
        RSDK.ProcessAnimation(&entity->animator1);
    }
}

void FlowerPod_State_FlowerGrow(void)
{
    RSDK_THIS(FlowerPod);

    if (entity->position.y <= entity->startPos.y - 0x200000) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = FlowerPod_State_Flower;
    }
    else {
        entity->position.y -= 0x40000;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void FlowerPod_State_Flower(void)
{
    RSDK_THIS(FlowerPod);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void FlowerPod_EditorDraw(void) {}

void FlowerPod_EditorLoad(void) {}

void FlowerPod_Serialize(void) {}

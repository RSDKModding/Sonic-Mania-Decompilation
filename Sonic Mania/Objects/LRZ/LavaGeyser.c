#include "SonicMania.h"

ObjectLavaGeyser *LavaGeyser;

void LavaGeyser_Update(void)
{
    RSDK_THIS(LavaGeyser);
    StateMachine_Run(entity->state);
}

void LavaGeyser_LateUpdate(void) {}

void LavaGeyser_StaticUpdate(void) {}

void LavaGeyser_Draw(void)
{
    RSDK_THIS(LavaGeyser);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->height > 0) {
        SpriteFrame *frame = RSDK.GetFrame(LavaGeyser->aniFrames, 0, entity->animator2.frameID);
        frame->height      = 48;
        drawPos.y -= entity->height;

        for (int i = (entity->height >> 16) / 48; i > 0; --i) {
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            drawPos.y += 0x300000;
        }

        if ((entity->height >> 16) % 48) {
            frame->height = (entity->height >> 16) % 48;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        }

        drawPos.y = entity->position.y - entity->height;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }
}

void LavaGeyser_Create(void *data)
{
    RSDK_THIS(LavaGeyser);
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->force <<= 12;
        entity->state = LavaGeyser_State_Unknown1;
    }
}

void LavaGeyser_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LavaGeyser.bin", SCOPE_STAGE);

    LavaGeyser->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    LavaGeyser->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    LavaGeyser->sfxDrill      = RSDK.GetSFX("LRZ/Drill.wav");
    LavaGeyser->sfxLavaGeyser = RSDK.GetSFX("LRZ/LavaGeyser.wav");
}

void LavaGeyser_CheckPlayerCollisions(void)
{
    RSDK_THIS(LavaGeyser);

    Hitbox hitbox;
    hitbox.left   = -24;
    hitbox.top    = -(entity->height >> 16);
    hitbox.right  = 24;
    hitbox.bottom = 0;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }
}

void LavaGeyser_HandleSetup(void)
{
    RSDK_THIS(LavaGeyser);
    entity->velocity.y = entity->force;
    entity->visible    = true;
    entity->active     = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &entity->animator3, true, 16);
    entity->state = LavaGeyser_State_Unknown2;
}

void LavaGeyser_HandleIntervals(void)
{
    RSDK_THIS(LavaGeyser);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval))
        LavaGeyser_HandleSetup();
}

void LavaGeyser_State_Unknown1(void) { LavaGeyser_HandleSetup(); }

void LavaGeyser_State_Unknown2(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == 16) {
        --entity->animator1.animationTimer;
        RSDK.PlaySfx(LavaGeyser->sfxLavaGeyser, false, 255);
        entity->state = LavaGeyser_State_Unknown3;
        LavaGeyser_State_Unknown3();
    }
}

void LavaGeyser_State_Unknown3(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->height += entity->velocity.y;
    entity->velocity.y -= 0x1800;
    LavaGeyser_CheckPlayerCollisions();

    if (entity->velocity.y < 0) {
        if (entity->duration) {
            entity->field_A8 = entity->height;
            entity->angle    = 0;
            entity->timer    = entity->duration;
            entity->state    = LavaGeyser_State_Unknown4;
        }
        else {
            entity->state = LavaGeyser_State_Unknown5;
        }
    }
}

void LavaGeyser_State_Unknown4(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->angle += 2;
    entity->height = 0x600 * RSDK.Cos256(entity->angle) + entity->field_A8;
    LavaGeyser_CheckPlayerCollisions();
    if (!--entity->timer)
        entity->state = LavaGeyser_State_Unknown5;
}

void LavaGeyser_State_Unknown5(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->height += entity->velocity.y;
    entity->velocity.y -= 0x1800;

    LavaGeyser_CheckPlayerCollisions();
    if (entity->height < 0) {
        entity->height     = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 2, &entity->animator1, true, 0);
        entity->state = LavaGeyser_State_Unknown6;
    }
}

void LavaGeyser_State_Unknown6(void)
{
    RSDK_THIS(LavaGeyser);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        if (entity->interval) {
            entity->visible = false;
            if (!entity->type)
                entity->state = LavaGeyser_HandleIntervals;
            else
                entity->state = LavaGeyser_State_Unknown1;
            entity->active = ACTIVE_BOUNDS;
        }
        else {
            destroyEntity(entity);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LavaGeyser_EditorDraw(void)
{
    RSDK_THIS(LavaGeyser);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &entity->animator3, true, 16);

    entity->velocity.y = entity->force << 12;
    entity->height     = 0;

    while (entity->velocity.y > 0) {
        entity->height += entity->velocity.y;
        entity->velocity.y -= 0x1800;
    }

    LavaGeyser_Draw();
}

void LavaGeyser_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaGeyser.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2"))
        LavaGeyser->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LavaGeyser.bin", SCOPE_STAGE);
}
#endif

void LavaGeyser_Serialize(void)
{
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(LavaGeyser, VAR_ENUM, force);
}

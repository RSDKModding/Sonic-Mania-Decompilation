#include "SonicMania.h"

ObjectRTeleporter *RTeleporter;

void RTeleporter_Update(void)
{
    RSDK_THIS(RTeleporter);
    StateMachine_Run(entity->state);
}

void RTeleporter_LateUpdate(void) {}

void RTeleporter_StaticUpdate(void) {}

void RTeleporter_Draw(void)
{
    RSDK_THIS(RTeleporter);
    StateMachine_Run(entity->stateDraw);
}

void RTeleporter_Create(void *data)
{
    RSDK_THIS(RTeleporter);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 1, &entity->animator2, true, 0);
        entity->startY    = entity->position.y;
        entity->state     = RTeleporter_State_CheckPlayerCollisions;
        entity->stateDraw = RTeleporter_StateDraw_Active;
    }
}

void RTeleporter_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("SSZ/RTeleporter.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MMZ1") || RSDK.CheckStageFolder("MMZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RTeleporter.bin", SCOPE_STAGE);

    RTeleporter->hitbox.top    = -32;
    RTeleporter->hitbox.left   = -34;
    RTeleporter->hitbox.right  = 34;
    RTeleporter->hitbox.bottom = 32;
    RTeleporter->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    RTeleporter->sfxGoodFuture = RSDK.GetSFX("Stage/GoodFuture.wav");
}

void RTeleporter_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.y = (RSDK.Sin256(entity->angle) << 9) + entity->startY;
    entity->angle      = (entity->angle + 4) & 0xFF;

    foreach_active(Player, player)
    {
        if (!player->onGround) {
            if (Player_CheckCollisionTouch(player, entity, &RTeleporter->hitbox)) {
                Player_CheckHit2(player, entity, false);
                entity->state     = RTeleporter_State_Destroyed;
                entity->stateDraw = RTeleporter_StateDraw_Exploding;
            }
        }
    }
}

void RTeleporter_State_Destroyed(void)
{
    RSDK_THIS(RTeleporter);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(RTeleporter->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = RSDK.Rand(-32, 32) << 16;
            int y = RSDK.Rand(-32, 32) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid(((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS)), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 80) {
        RSDK.PlaySfx(RTeleporter->sfxGoodFuture, false, 255);
        RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 2, &entity->animator1, true, 0);
        entity->position.y += 0x180000;
        RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8);
        entity->state     = StateMachine_None;
        entity->stateDraw = RTeleporter_StateDraw_Destroyed;
    }
}

void RTeleporter_StateDraw_Active(void)
{
    RSDK_THIS(RTeleporter);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    drawPos.x -= 0x100000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x += 0x200000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void RTeleporter_StateDraw_Exploding(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->direction = FLIP_Y;

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->direction = FLIP_NONE;
}

void RTeleporter_StateDraw_Destroyed(void)
{
    RSDK_THIS(RTeleporter);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void RTeleporter_EditorDraw(void)
{
    RSDK_THIS(RTeleporter);

    RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(RTeleporter->aniFrames, 1, &entity->animator2, true, 0);

    RTeleporter_StateDraw_Active();
}

void RTeleporter_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("SSZ/RTeleporter.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MMZ1") || RSDK.CheckStageFolder("MMZ2"))
        RTeleporter->aniFrames = RSDK.LoadSpriteAnimation("MMZ/RTeleporter.bin", SCOPE_STAGE);
}
#endif

void RTeleporter_Serialize(void) {}

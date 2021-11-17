#include "SonicMania.h"

ObjectFlameSpring *FlameSpring;

void FlameSpring_Update(void)
{
    RSDK_THIS(FlameSpring);
    StateMachine_Run(entity->state);
}

void FlameSpring_LateUpdate(void) {}

void FlameSpring_StaticUpdate(void) {}

void FlameSpring_Draw(void)
{
    RSDK_THIS(FlameSpring);
    StateMachine_Run(entity->stateDraw);
}

void FlameSpring_Create(void *data)
{
    RSDK_THIS(FlameSpring);
    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow + 1;
        if (data) {
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 2, &entity->animator1, true, 0);
            entity->state     = FlameSpring_State_Unknown2;
            entity->stateDraw = FlameSpring_StateDraw_Unknown2;
        }
        else {
            if (!(entity->force & 1))
                entity->velocity.y = -0xA0000;
            else
                entity->velocity.y = -0x100000;
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(FlameSpring->aniFrames, 1, &entity->animator2, true, 0);
            entity->field_6C.x = entity->position.x - 768 * RSDK.Sin1024(256);
            entity->field_6C.y = entity->position.y - 0x40000;
            entity->field_74.x = 768 * RSDK.Sin1024(256) + entity->position.x;
            entity->field_74.y = entity->position.y - 0x40000;
            if (entity->type > 2)
                entity->animator1.frameID = 2;
            entity->state     = FlameSpring_State_Unknown1;
            entity->type      = entity->type % 3;
            entity->stateDraw = FlameSpring_StateDraw_Unknown1;
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
    FlameSpring->sfxSpring      = RSDK.GetSFX("Global/Spring.wav");
    FlameSpring->sfxFlame       = RSDK.GetSFX("Stage/Flame2.wav");
}

void FlameSpring_State_Unknown1(void)
{
    RSDK_THIS(FlameSpring);

    entity->flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &FlameSpring->hitbox1) == 1 && player->velocity.y >= 0) {
            if (!entity->animator1.frameID) {
                entity->timer             = 0;
                entity->animator1.frameID = 1;
            }
            if (entity->animator1.frameID <= 1)
                entity->flag = true;
        }
    }

    if (entity->animator1.frameID == 1) {
        if (entity->flag) {
            if (++entity->timer >= 60) {
                foreach_active(Player, player)
                {
                    if (Player_CheckCollisionBox(player, entity, &FlameSpring->hitbox1) == 1) {
                        int32 anim = player->playerAnimator.animationID;
                        if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                            player->storedAnim = player->playerAnimator.animationID;
                        else
                            player->storedAnim = ANI_WALK;
                        if (entity->playerAni)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                        player->state      = Player_State_Air;
                        player->onGround   = false;
                        player->velocity.y = entity->velocity.y;
                        if (player->underwater)
                            player->velocity.y >>= 1;
                        RSDK.PlaySfx(FlameSpring->sfxSpring, false, 255);
                    }
                }
            }
        }
        else {
            entity->animator1.frameID = 0;
        }
    }
    else {
        if (!(Zone->timer & 7)) {
            RSDK.PlaySfx(FlameSpring->sfxFlame, false, 255);
        }

        int32 timer = Zone->timer + entity->offset;
        if (!entity->type) {
            entity->field_6C.x = entity->position.x - 768 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256);
            entity->field_74.x = 768 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256) + entity->position.x;
        }

        if (!(Zone->timer & 3)) {
            switch (entity->type) {
                case 0: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), entity->field_6C.x, entity->field_6C.y);
                    spring->drawOrder         = Zone->drawOrderLow;
                    spring->velocity.x        = -256 * RSDK.Sin1024(((4 * timer) & 0x1FF) + 256);

                    spring             = CREATE_ENTITY(FlameSpring, intToVoid(1), entity->field_74.x, entity->field_74.y);
                    spring->velocity.x = (RSDK.Sin1024(((4 * timer) & 0x1FF) + 256) << 8);
                    break;
                }
                case 1: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), entity->field_74.x, entity->field_74.y);
                    spring->drawOrder         = Zone->drawOrderLow;
                    if (192 * RSDK.Sin1024((timer & 0x1FF) + 256) >= 0)
                        spring->velocity.x = (0x44000 - 192 * RSDK.Sin1024((timer & 0x1FF) + 256));
                    else
                        spring->velocity.x = (0x44000 - -192 * RSDK.Sin1024((timer & 0x1FF) + 256));
                    break;
                }
                case 2: {
                    EntityFlameSpring *spring = CREATE_ENTITY(FlameSpring, intToVoid(1), entity->field_6C.x, entity->field_6C.y);
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

    entity->position.x += entity->velocity.x;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &FlameSpring->hitbox2)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void FlameSpring_StateDraw_Unknown1(void)
{
    RSDK_THIS(FlameSpring);
    if (entity->animator1.frameID != 1 && entity->type != 1) {
        entity->animator2.frameID = entity->field_6C.x > entity->position.x;
        RSDK.DrawSprite(&entity->animator2, &entity->field_6C, false);
    }

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (entity->animator1.frameID != 1 && entity->type != 2) {
        entity->animator2.frameID = entity->field_74.x > entity->position.x;
        RSDK.DrawSprite(&entity->animator2, &entity->field_74, false);
    }
}

void FlameSpring_StateDraw_Unknown2(void)
{
    RSDK_THIS(FlameSpring);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
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

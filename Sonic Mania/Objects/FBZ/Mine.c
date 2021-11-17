#include "SonicMania.h"

ObjectMine *Mine;

void Mine_Update(void)
{
    RSDK_THIS(Mine);

    if (entity->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Mine->hitbox)) {
                entity->active = ACTIVE_NORMAL;
                entity->timer  = 30;
            }
        }
#if RETRO_USE_PLUS 
        Mine_CheckMightyHit();
#endif
    }
    else {
        entity->animator.frameID ^= 1;
        entity->timer--;
        if (entity->timer) {
#if RETRO_USE_PLUS 
            Mine_CheckMightyHit();
#endif
        }
        else {
            foreach_active(FBZMissile, missile)
            {
                if (missile->type == 2
                    && RSDK.CheckObjectCollisionTouchBox(missile, &FBZMissile->hitbox3, entity, &FBZMissile->hitbox1)) {
                    if (--missile->timer <= 0) {
                        destroyEntity(missile);
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }
                    CREATE_ENTITY(Explosion, intToVoid(3), entity->position.x, entity->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                    destroyEntity(entity);
                    foreach_return;
                }
            }

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &Mine->hitbox)) {
#if RETRO_USE_PLUS
                    if (Player_CheckMightyUnspin(1024, player, true, &player->uncurlTimer))
                        player->onGround = false;
                    else
#endif
                        Player_CheckHit(player, entity);
                }
            }

            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y - 0x30000)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Mine->sfxExplosion, false, 255);
#if RETRO_USE_PLUS
            Mine_CheckMightyHit();
#endif
            destroyEntity(entity);
        }
    }
}

void Mine_LateUpdate(void) {}

void Mine_StaticUpdate(void) {}

void Mine_Draw(void)
{
    RSDK_THIS(Mine);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Mine_Create(void *data)
{
    RSDK_THIS(Mine);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Mine->aniFrames, 0, &entity->animator, true, 0);
    }
}

void Mine_StageLoad(void)
{
    Mine->aniFrames     = RSDK.LoadSpriteAnimation("FBZ/Mine.bin", SCOPE_STAGE);
    Mine->hitbox.top    = -6;
    Mine->hitbox.left   = -12;
    Mine->hitbox.right  = 12;
    Mine->hitbox.bottom = 0;
    Mine->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
}

#if RETRO_USE_PLUS
void Mine_CheckMightyHit(void)
{
    RSDK_THIS(Mine);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Mine->hitbox) && (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1)) {
            entity->timer = 1;
        }
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void Mine_EditorDraw(void) {}

void Mine_EditorLoad(void) {}
#endif

void Mine_Serialize(void) {}

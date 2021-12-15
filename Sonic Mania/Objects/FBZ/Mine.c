// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Mine Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMine *Mine;

void Mine_Update(void)
{
    RSDK_THIS(Mine);

    if (self->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Mine->hitbox)) {
                self->active = ACTIVE_NORMAL;
                self->timer  = 30;
            }
        }
#if RETRO_USE_PLUS 
        Mine_CheckMightyHit();
#endif
    }
    else {
        self->animator.frameID ^= 1;
        if (--self->timer) {
#if RETRO_USE_PLUS 
            Mine_CheckMightyHit();
#endif
        }
        else {
            foreach_active(FBZMissile, missile)
            {
                if (missile->type == 2
                    && RSDK.CheckObjectCollisionTouchBox(missile, &FBZMissile->hitbox3, self, &FBZMissile->hitbox1)) {
                    if (--missile->timer <= 0) {
                        destroyEntity(missile);
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }
                    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                    destroyEntity(self);
                    foreach_return;
                }
            }

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &Mine->hitbox)) {
#if RETRO_USE_PLUS
                    if (Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
                        player->onGround = false;
                    else
#endif
                        Player_CheckHit(player, self);
                }
            }

            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y - 0x30000)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Mine->sfxExplosion, false, 255);
#if RETRO_USE_PLUS
            Mine_CheckMightyHit();
#endif
            destroyEntity(self);
        }
    }
}

void Mine_LateUpdate(void) {}

void Mine_StaticUpdate(void) {}

void Mine_Draw(void)
{
    RSDK_THIS(Mine);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Mine_Create(void *data)
{
    RSDK_THIS(Mine);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Mine->aniFrames, 0, &self->animator, true, 0);
    }
}

void Mine_StageLoad(void)
{
    Mine->aniFrames     = RSDK.LoadSpriteAnimation("FBZ/Mine.bin", SCOPE_STAGE);
    Mine->hitbox.top    = -6;
    Mine->hitbox.left   = -12;
    Mine->hitbox.right  = 12;
    Mine->hitbox.bottom = 0;
    Mine->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
}

#if RETRO_USE_PLUS
void Mine_CheckMightyHit(void)
{
    RSDK_THIS(Mine);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Mine->hitbox) && (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1)) {
            self->timer = 1;
        }
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void Mine_EditorDraw(void)
{
    RSDK_THIS(Mine);
    self->drawOrder = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Mine->aniFrames, 0, &self->animator, true, 0);

    Mine_Draw();
}

void Mine_EditorLoad(void) { Mine->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Mine.bin", SCOPE_STAGE); }
#endif

void Mine_Serialize(void) {}

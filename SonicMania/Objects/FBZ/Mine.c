// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Mine Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMine *Mine;

void Mine_Update(void)
{
    RSDK_THIS(Mine);

    if (self->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Mine->hitboxMine)) {
                self->active = ACTIVE_NORMAL;
                self->timer  = 30;
            }
        }

#if MANIA_USE_PLUS
        Mine_CheckMightyHit();
#endif
    }
    else {
        self->animator.frameID ^= 1;
        if (--self->timer) {
#if MANIA_USE_PLUS
            Mine_CheckMightyHit();
#endif
        }
        else {
            foreach_active(FBZMissile, missile)
            {
                if (missile->type == FBZMISSILE_HULL
                    && RSDK.CheckObjectCollisionTouchBox(missile, &FBZMissile->hitboxHull, self, &FBZMissile->hitboxMissileV)) {
                    if (--missile->timer <= 0) {
                        destroyEntity(missile);
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }

                    int32 x                                                                    = self->position.x;
                    int32 y                                                                    = self->position.y + 0x30000;
                    CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y)->drawGroup = Zone->objectDrawGroup[1];
                    RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                    destroyEntity(self);
                    foreach_return;
                }
            }

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &Mine->hitboxMine)) {
#if MANIA_USE_PLUS
                    if (Player_CheckMightyUnspin(player, 0x400, true, &player->uncurlTimer))
                        player->onGround = false;
                    else
#endif
                        Player_Hurt(player, self);
                }
            }

            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y - 0x30000)->drawGroup =
                Zone->objectDrawGroup[1];
            RSDK.PlaySfx(Mine->sfxExplosion, false, 255);
#if MANIA_USE_PLUS
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
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;

        RSDK.SetSpriteAnimation(Mine->aniFrames, 0, &self->animator, true, 0);
    }
}

void Mine_StageLoad(void)
{
    Mine->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Mine.bin", SCOPE_STAGE);

    Mine->hitboxMine.top    = -6;
    Mine->hitboxMine.left   = -12;
    Mine->hitboxMine.right  = 12;
    Mine->hitboxMine.bottom = 0;

    Mine->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
}

#if MANIA_USE_PLUS
void Mine_CheckMightyHit(void)
{
    RSDK_THIS(Mine);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Mine->hitboxMine) && (player->characterID == ID_MIGHTY && player->jumpAbilityState > 1)) {
            self->timer = 1;
        }
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void Mine_EditorDraw(void)
{
    RSDK_THIS(Mine);
    self->drawGroup = Zone->objectDrawGroup[0];
    RSDK.SetSpriteAnimation(Mine->aniFrames, 0, &self->animator, true, 0);

    Mine_Draw();
}

void Mine_EditorLoad(void) { Mine->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Mine.bin", SCOPE_STAGE); }
#endif

void Mine_Serialize(void) {}

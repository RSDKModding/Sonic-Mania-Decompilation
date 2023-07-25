// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DoorTrigger Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDoorTrigger *DoorTrigger;

void DoorTrigger_Update(void)
{
    RSDK_THIS(DoorTrigger);

    RSDK.ProcessAnimation(&self->baseAnimator);

    if (self->bulbAnimator.frameID) {
        if (!--self->id) {
            self->id = RSDK.Rand(15, 121);

            int32 anim = 0;
            switch (self->orientation) {
                case DOORTRIGGER_ORIENATION_L:
                case DOORTRIGGER_ORIENATION_R: anim = 3; break;

                case DOORTRIGGER_ORIENATION_U:
                case DOORTRIGGER_ORIENATION_D: anim = 4; break;

                default: break;
            }

            int32 x             = self->position.x + (RSDK.Rand(-4, 5) << 16);
            int32 y             = self->position.y + (RSDK.Rand(-4, 5) << 16);
            EntityDebris *shard = CREATE_ENTITY(Debris, NULL, x, y);

            shard->state     = Debris_State_Move;
            shard->drawFX    = FX_FLIP;
            shard->direction = self->direction;
            shard->drawGroup = Zone->objectDrawGroup[1] - 1;
            RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, anim, &shard->animator, true, 0);
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckAttackingNoInvTimer(player, self)) {
                if (Player_CheckCollisionTouch(player, self, &DoorTrigger->hitboxBulb[self->baseAnimator.frameID])) {
                    self->bulbAnimator.frameID = 1;
                    if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_GLIDE) {
                        player->velocity.x = -player->velocity.x >> 1;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, false, 0);
                        player->state = Player_State_KnuxGlideDrop;
                    }
                    else {
                        int32 x = 0, y = 0;
                        if (self->baseAnimator.frameID) {
                            x = player->position.x - self->position.x;

                            if (self->direction)
                                y = (player->position.y - self->position.y) - 0xE0000;
                            else
                                y = (player->position.y - self->position.y) + 0xE0000;
                        }
                        else {
                            if (self->direction)
                                x = (player->position.x - self->position.x) - 0xE0000;
                            else
                                x = (player->position.x - self->position.x) + 0xE0000;

                            y = player->position.y - self->position.y;
                        }

                        int32 angle = RSDK.ATan2(x, y);
#if MANIA_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP) {
                            player->velocity.y -= 0x10000;
                        }
                        else {
#endif
                            player->velocity.x = 0x300 * RSDK.Cos256(angle);
                            player->groundVel  = 0x300 * RSDK.Cos256(angle);
                            player->velocity.y = 0x300 * RSDK.Sin256(angle);
#if MANIA_USE_PLUS
                        }
#endif

                        player->applyJumpCap = false;
                        player->onGround     = false;
                    }

                    RSDK.PlaySfx(DoorTrigger->sfxShatter, false, 255);

                    int32 spawnX = self->position.x;
                    int32 spawnY = self->position.y;
                    switch (self->orientation) {
                        case DOORTRIGGER_ORIENATION_L: spawnX -= 0x100000; break;
                        case DOORTRIGGER_ORIENATION_R: spawnX += 0x100000; break;
                        case DOORTRIGGER_ORIENATION_U: spawnY -= 0x100000; break;
                        case DOORTRIGGER_ORIENATION_D: spawnY += 0x100000; break;
                        default: break;
                    }

                    for (int32 i = 0; i < 8; ++i) {
                        EntityDebris *shard =
                            CREATE_ENTITY(Debris, NULL, spawnX + RSDK.Rand(-0xA0000, 0xA0000), spawnY + RSDK.Rand(-0xA0000, 0xA0000));
                        shard->state           = Debris_State_Fall;
                        shard->gravityStrength = 0x4000;
                        shard->velocity.x      = RSDK.Rand(0, 0x20000);
                        if (shard->position.x < self->position.x)
                            shard->velocity.x = -shard->velocity.x;
                        shard->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                        shard->drawFX     = FX_FLIP;
                        shard->direction  = i & 3;
                        shard->drawGroup  = Zone->objectDrawGroup[1];
                        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &shard->animator, true, RSDK.Rand(0, 4));
                    }

                    CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), spawnX, spawnY)->drawGroup = Zone->objectDrawGroup[1];
                }
            }
            else {
                Player_CheckCollisionBox(player, self, &DoorTrigger->hitboxBulb[self->baseAnimator.frameID]);
            }
        }
    }
}

void DoorTrigger_LateUpdate(void) {}

void DoorTrigger_StaticUpdate(void) {}

void DoorTrigger_Draw(void)
{
    RSDK_THIS(DoorTrigger);

    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
    RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

    if (!self->bulbAnimator.frameID && !(Zone->timer & 1)) {
        self->inkEffect            = INK_ADD;
        self->bulbAnimator.frameID = 2;
        self->alpha                = 0xB8 + ((56 * RSDK.Sin256(Zone->timer)) >> 8);
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

        self->bulbAnimator.frameID = 3;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

        self->bulbAnimator.frameID = 0;
        self->inkEffect            = INK_NONE;
    }
}

void DoorTrigger_Create(void *data)
{
    RSDK_THIS(DoorTrigger);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.y = 0x800000;
        self->updateRange.x = 0x800000;
        RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 0, &self->baseAnimator, true, 0);

        self->direction = self->orientation & 1;
        if (self->orientation < DOORTRIGGER_ORIENATION_U) {
            RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 1, &self->bulbAnimator, true, 0);
        }
        else {
            self->baseAnimator.frameID = 1;
            self->direction *= FLIP_Y;
            RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 2, &self->bulbAnimator, true, 0);
        }
    }
}

void DoorTrigger_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        DoorTrigger->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/DoorTrigger.bin", SCOPE_STAGE);

    // Active
    DoorTrigger->hitboxBulb[0].top    = -10;
    DoorTrigger->hitboxBulb[0].left   = -26;
    DoorTrigger->hitboxBulb[0].bottom = 10;
    DoorTrigger->hitboxBulb[0].right  = 0;

    // Busted
    DoorTrigger->hitboxBulb[1].top    = -26;
    DoorTrigger->hitboxBulb[1].left   = -10;
    DoorTrigger->hitboxBulb[1].bottom = 0;
    DoorTrigger->hitboxBulb[1].right  = 10;

    DoorTrigger->sfxShatter = RSDK.GetSfx("Stage/WindowShatter.wav");
}

#if GAME_INCLUDE_EDITOR
void DoorTrigger_EditorDraw(void)
{
    RSDK_THIS(DoorTrigger);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[1];
    self->updateRange.y = 0x800000;
    self->updateRange.x = 0x800000;
    RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 0, &self->baseAnimator, true, 0);

    self->direction = self->orientation & 1;
    if (self->orientation < DOORTRIGGER_ORIENATION_U) {
        RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 1, &self->bulbAnimator, true, 0);
    }
    else {
        self->baseAnimator.frameID = 1;
        self->direction *= FLIP_Y;
        RSDK.SetSpriteAnimation(DoorTrigger->aniFrames, 2, &self->bulbAnimator, true, 0);
    }

    DoorTrigger_Draw();
}

void DoorTrigger_EditorLoad(void)
{
    DoorTrigger->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/DoorTrigger.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DoorTrigger, orientation);
    RSDK_ENUM_VAR("Left", DOORTRIGGER_ORIENATION_L);
    RSDK_ENUM_VAR("Right", DOORTRIGGER_ORIENATION_R);
    RSDK_ENUM_VAR("Up", DOORTRIGGER_ORIENATION_U);
    RSDK_ENUM_VAR("Down", DOORTRIGGER_ORIENATION_D);
}
#endif

void DoorTrigger_Serialize(void)
{
    RSDK_EDITABLE_VAR(DoorTrigger, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(DoorTrigger, VAR_UINT8, id);
}

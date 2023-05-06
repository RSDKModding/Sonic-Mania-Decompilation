// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZRockPile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectAIZRockPile *AIZRockPile;

void AIZRockPile_Update(void)
{
    RSDK_THIS(AIZRockPile);

    Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 1);

    foreach_active(Player, player)
    {
        if (!self->onlyMighty || player->characterID == ID_MIGHTY) {
            if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                int32 cMode     = player->collisionMode;
                int32 playerX   = player->position.x;
                int32 playerY   = player->position.y;
                int32 xVelocity = player->velocity.x;
                int32 yVelocity = player->velocity.y;
                int32 jumping   = player->animator.animationID == ANI_JUMP;
                int32 groundVel = player->groundVel;

                if (self->smashSides || self->smashTop) {
                    int32 side = Player_CheckCollisionBox(player, self, hitbox);
                    if (self->smashSides && (side == C_LEFT || side == C_RIGHT)) {
                        if (side == C_LEFT || side == C_RIGHT) {
                            bool32 canBreak = jumping && player->onGround && abs(groundVel) >= 0x48000;
                            if (player->shield == SHIELD_FIRE) {
                                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                                canBreak |= shield->shieldAnimator.animationID == SHIELDANI_FIREATTACK;
                            }

                            canBreak |= player->characterID == ID_SONIC && player->superState == SUPERSTATE_SUPER;
                            if (!player->sidekick && (canBreak || (player->characterID == ID_KNUCKLES))) {
                                player->position.x = playerX;
                                player->position.y = playerY;
                                player->velocity.x = xVelocity;
                                player->velocity.y = yVelocity;
                                player->groundVel  = groundVel;
                                if (xVelocity <= 0)
                                    AIZRockPile_SpawnRocks(self->rockSpeedsL);
                                else
                                    AIZRockPile_SpawnRocks(self->rockSpeedsR);

                                foreach_return;
                            }
                        }
                    }

                    if (self->smashTop && side == C_TOP) {
                        bool32 canBreak = jumping;
                        canBreak |= player->characterID == ID_SONIC && player->animator.animationID == ANI_DROPDASH;
                        canBreak |= player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop;
                        if (player->groundedStore && cMode != CMODE_FLOOR && cMode != CMODE_ROOF)
                            canBreak = false;

                        if (canBreak && !player->sidekick) {
                            player->onGround = false;
                            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                player->velocity.y = yVelocity - 0x10000;
                            else
                                player->velocity.y = -0x30000;
                            AIZRockPile_SpawnRocks(self->rockSpeedsT);
                            foreach_return;
                        }
                    }
                }
            }
        }

        Player_CheckCollisionBox(player, self, hitbox);
    }
}

void AIZRockPile_LateUpdate(void) {}

void AIZRockPile_StaticUpdate(void) {}

void AIZRockPile_Draw(void)
{
    RSDK_THIS(AIZRockPile);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void AIZRockPile_Create(void *data)
{
    RSDK_THIS(AIZRockPile);

    RSDK.SetSpriteAnimation(AIZRockPile->aniFrames, 0, &self->animator, true, self->size + 3);

    if (!SceneInfo->inEditor) {
        switch (self->size) {
            case AIZROCKPILE_SMALL:
                self->size          = 4;
                self->rockPositions = AIZRockPile->rockPositions_small;
                self->rockSpeedsT   = AIZRockPile->rockSpeedsT_small;
                self->rockSpeedsL   = AIZRockPile->rockSpeedsL_small;
                self->rockSpeedsR   = AIZRockPile->rockSpeedsR_small;
                break;

            case AIZROCKPILE_MED:
                self->size          = 5;
                self->rockPositions = AIZRockPile->rockPositions_med;
                self->rockSpeedsT   = AIZRockPile->rockSpeedsT_med;
                self->rockSpeedsL   = AIZRockPile->rockSpeedsL_med;
                self->rockSpeedsR   = AIZRockPile->rockSpeedsR_med;
                break;

            case AIZROCKPILE_BIG:
                self->size          = 8;
                self->rockPositions = AIZRockPile->rockPositions_large;
                self->rockSpeedsT   = AIZRockPile->rockSpeedsT_large;
                self->rockSpeedsL   = AIZRockPile->rockSpeedsL_large;
                self->rockSpeedsR   = AIZRockPile->rockSpeedsR_large;
                break;
        }

        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
    }
}

void AIZRockPile_StageLoad(void)
{
    AIZRockPile->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);

    AIZRockPile->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak3.wav");
}

void AIZRockPile_SpawnRocks(int32 *speeds)
{
    RSDK_THIS(AIZRockPile);

    for (int32 i = 0; i < self->size; ++i) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + self->rockPositions[2 * i],
                                             self->position.y + self->rockPositions[(2 * i) + 1]);
        RSDK.SetSpriteAnimation(AIZRockPile->aniFrames, 1, &debris->animator, true, 0);
        debris->velocity.x      = speeds[2 * i];
        debris->velocity.y      = speeds[(2 * i) + 1];
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x800000;
        debris->updateRange.y   = 0x800000;
        debris->gravityStrength = 0x1800;
    }

    RSDK.PlaySfx(AIZRockPile->sfxBreak, false, 255);
    destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void AIZRockPile_EditorDraw(void)
{
    RSDK_THIS(AIZRockPile);
    RSDK.SetSpriteAnimation(AIZRockPile->aniFrames, 0, &self->animator, true, self->size + 3);

    AIZRockPile_Draw();
}

void AIZRockPile_EditorLoad(void)
{
    AIZRockPile->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(AIZRockPile, size);
    RSDK_ENUM_VAR("Small", AIZROCKPILE_SMALL);
    RSDK_ENUM_VAR("Medium", AIZROCKPILE_MED);
    RSDK_ENUM_VAR("Large", AIZROCKPILE_BIG);
}
#endif

void AIZRockPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, smashTop);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, smashSides);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(AIZRockPile, VAR_BOOL, onlyMighty);
}
#endif

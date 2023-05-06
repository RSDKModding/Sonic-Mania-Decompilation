// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Springboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpringboard *Springboard;

void Springboard_Update(void)
{
    RSDK_THIS(Springboard);

    RSDK.ProcessAnimation(&self->animator);

    // bounceDelay is unused, but if it was used, it'd prolly be "if (!--self->bounceDelay) {" around this foreach loop.
    // source: it was exactly like that in S2 '13
    foreach_active(Player, playerPtr)
    {
        if (playerPtr->velocity.y >= 0 && ((1 << RSDK.GetEntitySlot(playerPtr)) & self->activePlayers)) {
            int32 pos = CLAMP((playerPtr->position.x - self->position.x + 0x1C0000) >> 17, 0, 28);

            if ((self->direction & FLIP_X))
                pos = 28 - pos;

            if (pos >= 8) {
                if (self->animator.frameID == 3)
                    RSDK.SetSpriteAnimation(Springboard->aniFrames, 0, &self->animator, true, 0);

                if (self->animator.frameID == 2) {
                    int32 anim = playerPtr->animator.animationID;
                    if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                        playerPtr->animationReserve = playerPtr->animator.animationID;
                    else
                        playerPtr->animationReserve = ANI_WALK;

                    playerPtr->state          = Player_State_Air;
                    playerPtr->onGround       = false;
                    playerPtr->tileCollisions = TILECOLLISION_DOWN;
                    RSDK.SetSpriteAnimation(playerPtr->aniFrames, ANI_SPRING_CS, &playerPtr->animator, true, 1);
                    playerPtr->groundVel    = playerPtr->velocity.x;
                    playerPtr->velocity.y   = Springboard->springPower[MIN(2 * pos - 16, 39)] - playerPtr->gravityStrength - self->force;
                    playerPtr->applyJumpCap = false;

                    RSDK.PlaySfx(Springboard->sfxSpring, false, 0xFF);
                }
            }
        }
    }

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        int32 playerGndVel = player->groundVel;
        int32 playerVelX   = player->velocity.x;
        int32 springPos    = CLAMP((player->position.x - self->position.x + 0x1C0000) >> 17, 0, 27);
        bool32 bounced     = false;

        if (!self->direction) {
            int32 hitboxTop = 0;

            if (self->animator.frameID <= 2)
                hitboxTop = Springboard->heightsFlat[springPos];
            else if (self->animator.frameID == 3)
                hitboxTop = Springboard->heightsReady[springPos];

            Hitbox hitbox;
            hitbox.left   = -28;
            hitbox.top    = -hitboxTop;
            hitbox.right  = 28;
            hitbox.bottom = 8;

            uint8 collision = 0;
            if (!((1 << playerID) & self->activePlayers))
                bounced = collision = Player_CheckCollisionBox(player, self, &hitbox);
            else
                bounced = collision = Player_CheckCollisionPlatform(player, self, &hitbox);

            bounced = collision == C_TOP;
            switch (collision) {
                case C_LEFT:
                    player->groundVel  = playerGndVel;
                    player->velocity.x = playerVelX;
                    // [Fallthrough]
                case C_NONE:
                case C_RIGHT:
                case C_BOTTOM:
                    if (player->velocity.y >= 0 && ((1 << playerID) & self->activePlayers)) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        player->position.y   = self->position.y - (playerHitbox->bottom << 16) - (hitboxTop << 16);

                        if (!bounced)
                            bounced = player->position.x > self->position.x;
                    }
                    break;

                case C_TOP:
#if MANIA_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop)
                        player->state = Player_State_Air;
#endif
                    break;

                default: break;
            }

            player->flailing = false;
        }
        else if (self->direction == FLIP_X) {
            int32 pos       = abs(springPos - 27);
            int32 hitboxTop = 0;

            if (self->animator.frameID <= 2)
                hitboxTop = Springboard->heightsFlat[pos];
            else if (self->animator.frameID == 3)
                hitboxTop = Springboard->heightsReady[pos];

            Hitbox hitbox;
            hitbox.left   = -28;
            hitbox.top    = -hitboxTop;
            hitbox.right  = 28;
            hitbox.bottom = 8;

            uint8 collision = 0;
            if (!((1 << playerID) & self->activePlayers))
                collision = Player_CheckCollisionBox(player, self, &hitbox);
            else
                collision = Player_CheckCollisionPlatform(player, self, &hitbox);

            bounced = collision == C_TOP;
            switch (collision) {
                case C_NONE:
                case C_LEFT:
                case C_BOTTOM: break;

                case C_TOP:
#if MANIA_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        player->state = Player_State_Air;
                    }
#endif
                    break;

                case C_RIGHT:
                    player->groundVel  = playerGndVel;
                    player->velocity.x = playerVelX;
                    break;

                default: break;
            }

            if (player->velocity.y >= 0 && ((1 << playerID) & self->activePlayers)) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                player->position.y   = self->position.y - (playerHitbox->bottom << 16) - (hitboxTop << 16);
                if (!bounced)
                    bounced = player->position.x < self->position.x;
            }

            player->flailing = false;
        }

        if (bounced) {
            self->activePlayers |= (1 << playerID);
            if (springPos >= 8 && !self->bounceDelay)
                self->bounceDelay = 6;
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }
}

void Springboard_LateUpdate(void) {}

void Springboard_StaticUpdate(void) {}

void Springboard_Draw(void)
{
    RSDK_THIS(Springboard);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Springboard_Create(void *data)
{
    RSDK_THIS(Springboard);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    if (!SceneInfo->inEditor) {
        self->force = (self->force + 8) << 15;
        RSDK.SetSpriteAnimation(Springboard->aniFrames, 0, &self->animator, true, 3);
    }
}

void Springboard_StageLoad(void)
{
    Springboard->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Springboard.bin", SCOPE_STAGE);

    Springboard->sfxSpring = RSDK.GetSfx("Global/Spring.wav");

    DEBUGMODE_ADD_OBJ(Springboard);
}

void Springboard_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Springboard, NULL, self->position.x, self->position.y);
}
void Springboard_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Springboard->aniFrames, 1, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void Springboard_EditorDraw(void)
{
    RSDK_THIS(Springboard);
    RSDK.SetSpriteAnimation(Springboard->aniFrames, 0, &self->animator, false, 3);

    Springboard_Draw();
}

void Springboard_EditorLoad(void)
{
    Springboard->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Springboard.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Springboard, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Springboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(Springboard, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Springboard, VAR_ENUM, force);
}

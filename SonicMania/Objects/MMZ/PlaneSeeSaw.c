// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlaneSeeSaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlaneSeeSaw *PlaneSeeSaw;

void PlaneSeeSaw_Update(void)
{
    RSDK_THIS(PlaneSeeSaw);

    StateMachine_Run(self->state);
}

void PlaneSeeSaw_LateUpdate(void) {}

void PlaneSeeSaw_StaticUpdate(void) {}

void PlaneSeeSaw_Draw(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Vector2 drawPos;
    drawPos.x       = self->position.x;
    drawPos.y       = self->position.y + self->scale.y * (self->seeSawPos >> 9);
    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->swingAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->swingAnimator, NULL, false);
    RSDK.DrawSprite(&self->weightAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->platformAnimator, NULL, false);
}

void PlaneSeeSaw_Create(void *data)
{
    RSDK_THIS(PlaneSeeSaw);
    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->state         = PlaneSeeSaw_State_WaitForPlayer;
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 0, &self->swingAnimator, true, 0);
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 1, &self->platformAnimator, true, 0);
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 2, &self->weightAnimator, true, 0);
    }
}

void PlaneSeeSaw_StageLoad(void)
{
    PlaneSeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SeeSaw.bin", SCOPE_STAGE);

    PlaneSeeSaw->sfxSpring  = RSDK.GetSfx("Global/Spring.wav");
    PlaneSeeSaw->sfxFlipper = RSDK.GetSfx("Stage/Flipper.wav");
}

void PlaneSeeSaw_State_WaitForPlayer(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Hitbox *hitbox = RSDK.GetHitbox(&self->platformAnimator, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitbox)) {
#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif
            self->state = PlaneSeeSaw_State_PlayerPushDown;
        }
    }
}

void PlaneSeeSaw_State_PlayerPushDown(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Hitbox *hitboxOld = RSDK.GetHitbox(&self->platformAnimator, 0);
    if (self->platformAnimator.frameID >= self->platformAnimator.frameCount - 1) {
        self->velocity.y += 0x3800;
        self->seeSawPos += self->velocity.y;
    }
    else {
        ++self->swingAnimator.frameID;
        ++self->platformAnimator.frameID;

        if (self->platformAnimator.frameID == self->platformAnimator.frameCount - 1) {
            self->velocity.y = -0x60000;
            RSDK.PlaySfx(PlaneSeeSaw->sfxFlipper, false, 255);
        }

        self->seeSawPos = RSDK.GetHitbox(&self->swingAnimator, 0)->top << 16;
    }

    Hitbox *hitboxNew = RSDK.GetHitbox(&self->platformAnimator, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitboxOld))
            player->position.y += (hitboxNew->bottom - hitboxOld->bottom) << 16;
    }

    if (self->seeSawPos > -0x230000 && self->velocity.y > 0) {
        self->velocity.y = 0;
        self->seeSawPos  = -0x230000;
        self->state      = PlaneSeeSaw_State_Launch;
        RSDK.PlaySfx(PlaneSeeSaw->sfxSpring, false, 255);
    }
}

void PlaneSeeSaw_State_Launch(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Hitbox *hitboxOld = RSDK.GetHitbox(&self->platformAnimator, 0);
    if (self->platformAnimator.frameID <= 0) {
        self->state = PlaneSeeSaw_State_WaitForPlayer;
    }
    else {
        --self->swingAnimator.frameID;
        --self->platformAnimator.frameID;

        self->seeSawPos = RSDK.GetHitbox(&self->swingAnimator, 0)->top << 16;
    }

    Hitbox *hitboxNew = RSDK.GetHitbox(&self->platformAnimator, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitboxOld)) {
            player->position.y += (hitboxNew->bottom - hitboxOld->bottom) << 16;

            if (self->platformAnimator.frameID < 4) {
                if (self->scale.x == 0x100) {
                    player->position.x -= FarPlane->originPos.x;
                    player->position.y -= FarPlane->originPos.y;
                    player->position.x += FarPlane->position.x;
                    player->position.y += FarPlane->position.y;

                    if (player->camera) {
                        player->camera->targetMoveVel.x = 0;
                        player->camera->targetMoveVel.y = 0;
                    }

                    player->groundVel  = 0;
                    player->velocity.x = 0;
                    player->drawGroup  = Zone->playerDrawGroup[0];
                    player->state      = PlaneSeeSaw_PlayerState_ToFG;
                    player->velocity.y = -0x60000;
                }
                else {
                    player->groundVel  = 0;
                    player->velocity.x = 0;
                    player->scale.x    = 0x200;
                    player->scale.y    = 0x200;
                    player->state      = PlaneSeeSaw_PlayerState_ToBG;
                    player->velocity.y = -0xA0000;
                }

                player->abilityValues[0] = self->position.x;
                player->nextAirState     = StateMachine_None;
                player->nextGroundState  = StateMachine_None;
                player->interaction      = false;
                player->tileCollisions   = TILECOLLISION_NONE;
                player->jumpAbilityState = 0;
                player->drawFX |= FX_SCALE;
                player->applyJumpCap = false;
                player->onGround     = false;

                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                if (shield && shield->classID)
                    shield->drawFX |= FX_SCALE;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->animator.speed = 0x80;
            }
        }
    }
}

void PlaneSeeSaw_PlayerState_ToBG(void)
{
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    self->left  = false;
    self->right = false;

    if ((self->abilityValues[0] ^ self->position.x) & 0xFFFF0000) {
        self->position.x += self->position.x <= self->abilityValues[0] ? 0x8000 : -0x8000;
    }

    ControllerInfo[self->controllerID].keyY.press = false;
    Player_State_Air();

    self->tileCollisions = TILECOLLISION_NONE;
    if (self->scale.x <= 0x100) {
        self->interaction    = true;
        self->tileCollisions = TILECOLLISION_DOWN;
        self->drawGroup      = 2;

        self->position.x -= FarPlane->position.x;
        self->position.y -= FarPlane->position.y;
        self->position.x += FarPlane->originPos.x;
        self->position.y += FarPlane->originPos.y;
        Zone->deathBoundary[0] += 0x8000000;

        self->state = Player_State_Air;
        if (self->camera) {
            self->camera->targetMoveVel.x = FarPlane->position.x - FarPlane->originPos.x;
            self->camera->targetMoveVel.y = FarPlane->position.y - FarPlane->originPos.y;
        }

        self->scale.y = self->scale.x;
    }
    else {
        self->scale.x -= 4;
        self->scale.y = self->scale.x;
    }
}

void PlaneSeeSaw_PlayerState_ToFG(void)
{
    // ???
    EntityPlayer *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, Player);

    self->left  = false;
    self->right = false;
    if (((self->abilityValues[0] ^ self->position.x) & 0xFFFF0000)) {
        self->position.x += self->position.x <= self->abilityValues[0] ? 0x8000 : -0x8000;
    }

    ControllerInfo[self->controllerID].keyY.press = false;
    Player_State_Air();

    self->tileCollisions = TILECOLLISION_NONE;
    if (self->scale.x >= 0x200) {
        self->interaction    = true;
        self->tileCollisions = TILECOLLISION_DOWN;
        Zone->deathBoundary[0] -= 0x8000000;

        self->drawFX &= ~FX_SCALE;
        self->state = Player_State_Air;

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(self), Shield);
        if (shield && shield->classID)
            shield->drawFX &= ~FX_SCALE;

        self->scale.y = self->scale.x;
    }
    else {
        self->scale.x += 4;
        self->scale.y = self->scale.x;
    }
}

#if GAME_INCLUDE_EDITOR
void PlaneSeeSaw_EditorDraw(void)
{
    RSDK_THIS(PlaneSeeSaw);

    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 0, &self->swingAnimator, false, 0);
    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 1, &self->platformAnimator, false, 0);
    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 2, &self->weightAnimator, false, 0);

    PlaneSeeSaw_Draw();
}

void PlaneSeeSaw_EditorLoad(void) { PlaneSeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SeeSaw.bin", SCOPE_STAGE); }
#endif

void PlaneSeeSaw_Serialize(void) {}

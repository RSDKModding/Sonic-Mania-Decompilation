// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Crate Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCrate *Crate;

void Crate_Update(void)
{
    RSDK_THIS(Crate);

    if (self->collision != PLATFORM_C_SOLID || !Crate_Collide()) {
        if (self->animator.animationID) {
            StateMachine_Run(self->state);
        }
        else {
            Platform_Update();
        }
    }
}

void Crate_LateUpdate(void) {}

void Crate_StaticUpdate(void) {}

void Crate_Draw(void)
{
    RSDK_THIS(Crate);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void Crate_Create(void *data)
{
    RSDK_THIS(Crate);

    int32 frameID = self->frameID;
    if (frameID == 3)
        self->collision = PLATFORM_C_NONE;
    else
        self->collision = PLATFORM_C_SOLID;

    Platform_Create(NULL);
    self->frameID = frameID;

    if (!SceneInfo->inEditor) {
        self->hitbox.left   = -24;
        self->hitbox.top    = -24;
        self->hitbox.right  = 24;
        self->hitbox.bottom = 24;

        self->active    = ACTIVE_XBOUNDS;
        self->drawFX    = FX_SCALE | FX_FLIP;
        self->scale.x   = 0x200;
        self->scale.y   = 0x200;
        self->drawGroup = frameID == 3 ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];
        RSDK.SetSpriteAnimation(Crate->aniFrames, 0, &self->animator, true, frameID);
        self->state = Crate_State_None;
    }
}

void Crate_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Crate->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Crate.bin", SCOPE_STAGE);

    Crate->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
}

void Crate_Break(EntityCrate *create)
{
    for (int32 s = 0; s < 64; ++s) {
        // ice is used to create a shattering effect
        int32 x        = create->position.x + (RSDK.Rand(-24, 25) << 16);
        int32 y        = create->position.y + (RSDK.Rand(-24, 25) << 16);
        EntityIce *ice = CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_SHARD), x, y);

        ice->velocity.x          = RSDK.Rand(-6, 8) << 15;
        ice->velocity.y          = RSDK.Rand(-10, 2) << 15;
        ice->direction           = RSDK.Rand(0, 4);
        ice->blockAnimator.speed = RSDK.Rand(1, 4);
        ice->drawGroup           = Zone->objectDrawGroup[0] + 1;

        switch (create->animator.frameID) {
            case 0:
            case 3: RSDK.SetSpriteAnimation(Crate->aniFrames, 1, &ice->blockAnimator, true, 0); break;

            case 1:
            case 2:
                if (RSDK.Rand(0, 6) >= 2)
                    RSDK.SetSpriteAnimation(Crate->aniFrames, 2, &ice->blockAnimator, true, 0);
                else
                    RSDK.SetSpriteAnimation(Crate->aniFrames, 3, &ice->blockAnimator, true, 0);
                break;

            default: break;
        }
    }

    RSDK.PlaySfx(Crate->sfxExplosion2, false, 255);
    create->position.y -= 0x10000;

    foreach_active(Crate, crate)
    {
        if (crate != create && crate->state == Crate_State_None
            && RSDK.CheckObjectCollisionTouchBox(create, &create->hitbox, crate, &crate->hitbox)) {
            crate->state = Crate_State_ApplyGravity;
        }
    }

    destroyEntity(create);
}
void Crate_MoveY(EntityCrate *self, int32 offset)
{
    self->drawPos.y += offset;
    self->centerPos.x = self->drawPos.x;
    self->centerPos.y = self->drawPos.y;
    self->position.x  = self->drawPos.x;
    self->position.y  = self->drawPos.y;

    int32 start = (self->drawPos.y - 0x300000) & 0xFFFF0000;
    foreach_active(Crate, crate)
    {
        if (crate != self && RSDK.CheckObjectCollisionBox(self, &self->hitbox, crate, &crate->hitbox, true) == C_TOP)
            Crate_MoveY(crate, start - crate->drawPos.y);
    }
}
bool32 Crate_Collide(void)
{
    RSDK_THIS(Crate);

    foreach_active(Player, player)
    {
        int32 storeX         = player->position.x;
        int32 storeY         = player->position.y;
        int32 storeXVel      = player->velocity.x;
        int32 storeYVel      = player->velocity.y;
        int32 storeVel       = player->groundVel;
        int32 storeGrounded  = player->onGround;
        Hitbox *playerHitbox = Player_GetHitbox(player);

        int32 shieldAnim = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield)->shieldAnimator.animationID;

        switch (MathHelpers_CheckBoxCollision(self, &self->hitbox, player, playerHitbox)) {
            default:
            case C_NONE:
                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, self, &self->hitbox);
                break;

            case C_TOP:
                if (self->frameID != 1) {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    Player_CheckCollisionBox(player, self, &self->hitbox);
                    break;
                }
#if MANIA_USE_PLUS
                else if (player->state == Player_State_MightyHammerDrop) {
                    player->velocity.y -= 0x10000;
                    Crate_Break(self);
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    player->groundVel  = storeVel;
                    player->onGround   = storeGrounded;
                    foreach_return true;
                }
#endif
                else if (player->shield == SHIELD_BUBBLE && player->invincibleTimer <= 0) {
                    if (shieldAnim == 8 && player->velocity.y >= 0x80000) {
                        Crate_Break(self);
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        player->groundVel  = storeVel;
                        player->onGround   = storeGrounded;
                        foreach_return true;
                    }
                }

                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, self, &self->hitbox);
                break;

            case C_LEFT:
            case C_RIGHT:
                if (self->frameID != 1 || player->shield != SHIELD_FIRE || player->invincibleTimer > 0 || shieldAnim != 2) {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    Player_CheckCollisionBox(player, self, &self->hitbox);
                }
                else if (player->position.x < self->position.x) {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;

                    if (player->velocity.x >= 0x78000) {
                        Crate_Break(self);
                        player->groundVel = storeVel;
                        player->onGround  = storeGrounded;
                        foreach_return true;
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &self->hitbox);
                    }
                }
                else {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;

                    if (player->velocity.x <= -0x78000) {
                        Crate_Break(self);

                        player->groundVel = storeVel;
                        player->onGround  = storeGrounded;
                        foreach_return true;
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &self->hitbox);
                    }
                }
                break;

            case C_BOTTOM:
                if (self->collisionOffset.y >= 0)
                    player->collisionFlagV |= 2;

                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, self, &self->hitbox);
                break;
        }
    }

    return false;
}
void Crate_State_None(void)
{
    // hehe
}
void Crate_State_ApplyGravity(void)
{
    RSDK_THIS(Crate);

    self->position.y -= 0x10000;

    foreach_active(Crate, crate)
    {
        if (crate != self && crate->state == Crate_State_None && RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, crate, &crate->hitbox)) {
            crate->state = Crate_State_ApplyGravity;
        }
    }

    self->position.y += 0x10000;
    self->timer = 15;
    self->state = Crate_State_WaitToFall;
}
void Crate_State_WaitToFall(void)
{
    RSDK_THIS(Crate);

    if (--self->timer <= 0)
        self->state = Crate_State_Fall;
}
void Crate_State_Fall(void)
{
    RSDK_THIS(Crate);

    self->drawPos.y += self->velocity.y;

    int32 x = self->position.x;
    int32 y = self->position.y;

    self->velocity.y += 0x3800;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, 0, CMODE_FLOOR, 0, 0x180000, true)) {
        self->velocity.y = 0;
        self->state      = Crate_State_None;
    }
    else {
        foreach_active(Crate, crate)
        {
            if (crate != self && !crate->velocity.y && RSDK.CheckObjectCollisionBox(crate, &crate->hitbox, self, &self->hitbox, true) == C_TOP) {
                self->velocity.y = 0;
                self->state      = Crate_State_None;
            }
        }
    }

    self->drawPos.x   = self->position.x;
    self->drawPos.y   = self->position.y;
    self->centerPos.x = self->position.x;
    self->centerPos.y = self->position.y;

    self->position.x = x;
    self->position.y = y;
}

#if GAME_INCLUDE_EDITOR
void Crate_EditorDraw(void)
{
    RSDK_THIS(Crate);

    RSDK.SetSpriteAnimation(Crate->aniFrames, 0, &self->animator, true, self->frameID);
    self->drawPos = self->position;

    Crate_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Crate_EditorLoad(void)
{
    Crate->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Crate.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Crate, frameID);
    RSDK_ENUM_VAR("Blue", CRATE_FRAME_BLUE);
    RSDK_ENUM_VAR("Orange (Broken)", CRATE_FRAME_ORANGE_BROKEN);
    RSDK_ENUM_VAR("Orange", CRATE_FRAME_ORANGE);
    RSDK_ENUM_VAR("Blue (BG)", CRATE_FRAME_BLUE_BG);
}
#endif

void Crate_Serialize(void)
{
    RSDK_EDITABLE_VAR(Crate, VAR_UINT8, frameID);
    RSDK_EDITABLE_VAR(Crate, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(Crate, VAR_BOOL, ignoreItemBox);
}

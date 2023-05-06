// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Blastoid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBlastoid *Blastoid;

void Blastoid_Update(void)
{
    RSDK_THIS(Blastoid);

    StateMachine_Run(self->state);
}

void Blastoid_LateUpdate(void) {}

void Blastoid_StaticUpdate(void) {}

void Blastoid_Draw(void)
{
    RSDK_THIS(Blastoid);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Blastoid_Create(void *data)
{
    RSDK_THIS(Blastoid);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;

    if (!SceneInfo->inEditor) {
        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 1, &self->animator, true, 0);
            self->state = Blastoid_State_Projectile;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &self->animator, true, 1);
            self->state = Blastoid_State_Init;
        }
    }
}

void Blastoid_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        Blastoid->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Blastoid.bin", SCOPE_STAGE);

    Blastoid->hitboxBody.left   = -12;
    Blastoid->hitboxBody.top    = -9;
    Blastoid->hitboxBody.right  = 12;
    Blastoid->hitboxBody.bottom = 12;

    Blastoid->hitboxProjectile.left   = -3;
    Blastoid->hitboxProjectile.top    = -3;
    Blastoid->hitboxProjectile.right  = 3;
    Blastoid->hitboxProjectile.bottom = 3;

    Blastoid->sfxShot = RSDK.GetSfx("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Blastoid);
}

void Blastoid_DebugSpawn(void)
{
    RSDK_THIS(Blastoid);

    CREATE_ENTITY(Blastoid, NULL, self->position.x, self->position.y);
}

void Blastoid_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Blastoid_CheckPlayerCollisions(void)
{
    RSDK_THIS(Blastoid);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Blastoid->hitboxBody) && Player_CheckBadnikBreak(player, self, false)) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
            if (platform->classID == CollapsingPlatform->classID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 30;
                platform->stoodPos.x    = self->position.x;
            }
            destroyEntity(self);
        }
    }
}

void Blastoid_State_Init(void)
{
    RSDK_THIS(Blastoid);

    self->active = ACTIVE_NORMAL;

    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
    if (platform->classID == CollapsingPlatform->classID)
        platform->active = ACTIVE_NEVER;

    self->state = Blastoid_State_Body;
}

void Blastoid_State_Body(void)
{
    RSDK_THIS(Blastoid);

    RSDK.ProcessAnimation(&self->animator);

    switch (++self->timer) {
        case 1:
        case 16:
        case 31: {
            EntityBlastoid *projectile = CREATE_ENTITY(Blastoid, INT_TO_VOID(true), self->position.x, self->position.y);

            if (self->direction & FLIP_X) {
                projectile->position.x += 0x100000;
                projectile->velocity.x = 0x20000;
            }
            else {
                projectile->position.x -= 0x100000;
                projectile->velocity.x = -0x20000;
            }

            if (self->direction & FLIP_Y) {
                projectile->position.y += 0x60000;
                projectile->velocity.y = 0x10000;
            }
            else {
                projectile->position.y -= 0x60000;
                projectile->velocity.y = -0x10000;
            }

            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &self->animator, true, 0);
            RSDK.PlaySfx(Blastoid->sfxShot, false, 0xFF);
            break;
        }

        case 121: self->timer = 0; break;

        default: break;
    }

    Blastoid_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->timer  = 0;
        self->active = ACTIVE_BOUNDS;
    }
}

void Blastoid_State_Projectile(void)
{
    RSDK_THIS(Blastoid);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
    else {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        RSDK.ProcessAnimation(&self->animator);

        // Bug(?): setPos value is 4??????
        // This implies these used to be "ObjectTileGrip" at some point, and were then changed to "ObjectTileCollision" without changing the last
        // parameter Not that it *really* matters, since 4 will evaluate to true since it's non-zero

        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, Blastoid->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, Blastoid->hitboxProjectile.left << 13, 0, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, Blastoid->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, Blastoid->hitboxProjectile.right << 13, 0, 4)) {
            destroyEntity(self);
        }
        else {
            foreach_active(Player, player)
            {
                int32 shield = player->shield;
                // HCZBubble overrides active shields for collision
                if (Water_GetPlayerBubble(player))
                    player->shield = SHIELD_BUBBLE;

                if (Player_CheckCollisionTouch(player, self, &Blastoid->hitboxProjectile))
                    Player_ProjectileHurt(player, self);
                player->shield = shield;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Blastoid_EditorDraw(void)
{
    RSDK_THIS(Blastoid);

    RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &self->animator, false, 0);

    Blastoid_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
        if (CollapsingPlatform && platform->classID == CollapsingPlatform->classID)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Blastoid_EditorLoad(void)
{
    Blastoid->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Blastoid.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Blastoid, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
    RSDK_ENUM_VAR("Left (Flipped)", FLIP_Y);
    RSDK_ENUM_VAR("Right (Flipped)", FLIP_XY);
}
#endif

void Blastoid_Serialize(void) { RSDK_EDITABLE_VAR(Blastoid, VAR_UINT8, direction); }

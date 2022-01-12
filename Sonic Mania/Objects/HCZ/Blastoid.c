// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Blastoid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    self->drawOrder = Zone->drawOrderLow;
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
            self->state = Blastoid_State_Setup;
        }
    }
}

void Blastoid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        Blastoid->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Blastoid.bin", SCOPE_STAGE);
    Blastoid->hitboxBody.left         = -12;
    Blastoid->hitboxBody.top          = -9;
    Blastoid->hitboxBody.right        = 12;
    Blastoid->hitboxBody.bottom       = 12;
    Blastoid->hitboxProjectile.left   = -3;
    Blastoid->hitboxProjectile.top    = -3;
    Blastoid->hitboxProjectile.right  = 3;
    Blastoid->hitboxProjectile.bottom = 3;
    Blastoid->sfxShot                 = RSDK.GetSfx("Stage/Shot.wav");
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
        if (Player_CheckBadnikTouch(player, self, &Blastoid->hitboxBody) && Player_CheckBadnikBreak(self, player, false)) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
            if (platform->objectID == CollapsingPlatform->objectID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 30;
                platform->stoodPos.x    = self->position.x;
            }
            destroyEntity(self);
        }
    }
}

void Blastoid_State_Setup(void)
{
    RSDK_THIS(Blastoid);
    self->active     = ACTIVE_NORMAL;
    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, CollapsingPlatform);
    if (platform->objectID == CollapsingPlatform->objectID)
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
            EntityBlastoid *projectile = CREATE_ENTITY(Blastoid, intToVoid(true), self->position.x, self->position.y);
            if ((self->direction & 1)) {
                projectile->position.x += 0x100000;
                projectile->velocity.x = 0x20000;
            }
            else {
                projectile->position.x -= 0x100000;
                projectile->velocity.x = -0x20000;
            }
            if ((self->direction & 2) != 0) {
                projectile->position.y += 0x60000;
                projectile->velocity.y = 0x10000;
            }
            else {
                projectile->position.y -= 0x60000;
                projectile->velocity.y = -0x10000;
            }
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &self->animator, true, 0);
            RSDK.PlaySfx(Blastoid->sfxShot, false, 255);
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
    if (RSDK.CheckOnScreen(self, NULL)) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        RSDK.ProcessAnimation(&self->animator);

        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, Blastoid->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, Blastoid->hitboxProjectile.left << 13, 0, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, Blastoid->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, Blastoid->hitboxProjectile.right << 13, 0, 4)) {
            destroyEntity(self);
            return;
        }

        foreach_active(Player, player)
        {
            int32 shield = player->shield;
            if (Water_GetPlayerBubble(player))
                player->shield = SHIELD_BUBBLE;
            if (Player_CheckCollisionTouch(player, self, &Blastoid->hitboxProjectile))
                Player_CheckProjectileHit(player, self);
            player->shield = shield;
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Blastoid_EditorDraw(void)
{
    RSDK_THIS(Blastoid);
    RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &self->animator, false, 0);
    Blastoid_Draw();
}

void Blastoid_EditorLoad(void) { Blastoid->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Blastoid.bin", SCOPE_STAGE); }
#endif

void Blastoid_Serialize(void) { RSDK_EDITABLE_VAR(Blastoid, VAR_UINT8, direction); }

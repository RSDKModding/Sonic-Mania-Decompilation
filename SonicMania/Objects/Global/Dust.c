// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Dust Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDust *Dust;

void Dust_Update(void) {}

void Dust_LateUpdate(void)
{
    RSDK_THIS(Dust);

    StateMachine_Run(self->state);
}

void Dust_StaticUpdate(void) {}

void Dust_Draw(void)
{
    RSDK_THIS(Dust);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Dust_Create(void *data)
{
    RSDK_THIS(Dust);

    RSDK.SetSpriteAnimation(Dust->aniFrames, 0, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawFX    = FX_FLIP | FX_ROTATE;
        self->drawGroup = Zone->objectDrawGroup[1];
        self->parent    = (Entity *)data;
    }
}

void Dust_StageLoad(void) { Dust->aniFrames = RSDK.LoadSpriteAnimation("Global/Dust.bin", SCOPE_STAGE); }

void Dust_State_SpinDash(void)
{
    RSDK_THIS(Dust);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (!player) {
        destroyEntity(self);
    }
    else {
        Hitbox *playerHitbox = Player_GetHitbox(player);

        self->position.x = player->position.x;
        self->position.y = player->position.y;
        int32 bottom     = playerHitbox->bottom << 16;
        if (player->invertGravity)
            self->position.y -= bottom;
        else
            self->position.y += bottom;
        self->direction = player->direction;
        self->drawGroup = player->drawGroup;
        self->rotation  = player->rotation;

        RSDK.ProcessAnimation(&self->animator);

        if (player->state != Player_State_Spindash)
            destroyEntity(self);
    }
}
void Dust_State_DustTrail(void)
{
    RSDK_THIS(Dust);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (!player) {
        destroyEntity(self);
    }
    else {
        self->visible = false;
        if (!self->timer && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = CREATE_ENTITY(Dust, self, player->position.x, player->position.y);
            dust->state          = Dust_State_DustPuff;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawGroup = player->drawGroup;
        }

        self->timer = (self->timer + 1) & 7;
        if (player->animator.animationID != ANI_SKID)
            destroyEntity(self);
    }
}
void Dust_State_GlideTrail(void)
{
    RSDK_THIS(Dust);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (!player) {
        destroyEntity(self);
    }
    else {
        self->visible = false;
        if (!self->timer && player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            EntityDust *dust     = CREATE_ENTITY(Dust, self, player->position.x, player->position.y - TO_FIXED(4));
            dust->state          = Dust_State_DustPuff;
            dust->position.y += playerHitbox->bottom << 16;
            dust->drawGroup = player->drawGroup;
        }

        self->timer = (self->timer + 1) & 7;
        if (player->animator.animationID != ANI_GLIDE_SLIDE || !player->groundVel)
            destroyEntity(self);
    }
}
void Dust_State_DustPuff(void)
{
    RSDK_THIS(Dust);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}
#if MANIA_USE_PLUS
void Dust_State_DustPuff_Collide(void)
{
    RSDK_THIS(Dust);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ObjectTileGrip(self, self->collisionLayers, self->collisionMode, self->collisionPlane, 0, 0, 8);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}
#endif
void Dust_State_DustPuff_Friction(void)
{
    RSDK_THIS(Dust);

    RSDK.ProcessAnimation(&self->animator);

    self->velocity.x -= 0x2000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Dust_EditorDraw(void) {}

void Dust_EditorLoad(void) {}
#endif

void Dust_Serialize(void) {}

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Animals Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAnimals *Animals;

void Animals_Update(void)
{
    RSDK_THIS(Animals);

    StateMachine_Run(self->state);

    if (!self->behaviour && !RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Animals_LateUpdate(void) {}

void Animals_StaticUpdate(void) {}

void Animals_Draw(void)
{
    RSDK_THIS(Animals);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Animals_Create(void *data)
{
    RSDK_THIS(Animals);

    if (self->behaviour == ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS)
        self->active = ACTIVE_BOUNDS;
    else
        self->active = ACTIVE_NORMAL;

    self->drawFX |= FX_FLIP;
    self->visible       = true;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->drawOrder     = Zone->objectDrawLow;

    int32 type = ANIMAL_POCKY;
#if MANIA_USE_PLUS
    if (!(globals->secrets & GET_MEDAL_MOD(SECRET_RICKYMODE)))
#endif
        type = voidToInt(data);

    if (!self->type && ZONE_RAND(0, 256) == 21) {
        type                      = ANIMAL_POCKY;
        self->velocity.y          = -0x40000;
        self->type                = type - 1;
        self->state               = Animals_State_Freed;
        self->hitboxAnimal.top    = -Animals->hitboxes[self->type] >> 16;
        self->hitboxAnimal.left   = -4;
        self->hitboxAnimal.right  = 4;
        self->hitboxAnimal.bottom = Animals->hitboxes[self->type] >> 16;
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0);
    }
    else if (type) {
        self->velocity.y = -0x40000;
        self->type       = type - 1;
        self->state      = Animals_State_Freed;

        self->hitboxAnimal.top    = -Animals->hitboxes[self->type] >> 16;
        self->hitboxAnimal.left   = -4;
        self->hitboxAnimal.right  = 4;
        self->hitboxAnimal.bottom = Animals->hitboxes[self->type] >> 16;

        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0);
    }
    else if (self->behaviour == ANIMAL_BEHAVE_FOLLOW) {
        self->active = ACTIVE_BOUNDS;
        switch (self->type) {
            case ANIMAL_FLICKY:
            case ANIMAL_CUCKY:
            case ANIMAL_LOCKY: self->state = Animals_State_FollowPlayer_Bird; break;

            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: self->state = Animals_State_FollowPlayer_Normal; break;

            default: break;
        }

        Animals_CheckPlayerPos();
        self->velocity.y = Animals->yVelocity[self->type];
        if (self->direction == FLIP_NONE)
            self->velocity.x = -Animals->xVelocity[self->type];
        else
            self->velocity.x = Animals->xVelocity[self->type];
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type + 1, &self->animator, true, 0);
    }
    else {
        self->state = Animals_State_BounceAround;
    }
}

void Animals_StageLoad(void)
{
    Animals->aniFrames = RSDK.LoadSpriteAnimation("Global/Animals.bin", SCOPE_STAGE);

    if (RSDK.FindObject("Platform"))
        Animals->hasPlatform = true;

    if (RSDK.FindObject("Bridge"))
        Animals->hasBridge = true;
}

void Animals_CheckPlayerPos(void)
{
    RSDK_THIS(Animals);

    switch (self->behaviour) {
        default:
        case ANIMAL_BEHAVE_BOUNCEAROUND: self->direction = FLIP_X; break;

        case ANIMAL_BEHAVE_FOLLOW: {
            EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (!player) {
                self->direction = FLIP_NONE;
            }
            else if (self->position.x < player->position.x) {
                self->direction = FLIP_NONE;
            }
            else {
                self->direction = FLIP_X;
            }
            break;
        }

        case ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS: self->direction = ZONE_RAND(0, 2); break;
    }

    if (!self->direction)
        self->velocity.x = -Animals->xVelocity[self->type];
    else
        self->velocity.x = Animals->xVelocity[self->type];
}

bool32 Animals_CheckPlatformCollision(void *p)
{
    RSDK_THIS(Animals);
    EntityPlatform *platform = (EntityPlatform *)p;

    bool32 collided = false;
    if (platform->state != Platform_State_Collapse_Falling && platform->state != Platform_State_Collapse_CheckReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

        if (platform->collision <= PLATFORM_C_SOLID_ALL) {
            collided = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), self, &self->hitboxAnimal, true);
        }
        else if (platform->collision == PLATFORM_C_USE_TILES
                 && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &self->hitboxAnimal)) {
            if (self->collisionLayers & Zone->moveMask) {
                TileLayer *move  = RSDK.GetTileLayer(Zone->moveLayer);
                move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
            }

            if (self->velocity.y >= 0x3800)
                collided = true;
        }
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
    }

    return collided;
}

bool32 Animals_CheckGroundCollision(void)
{
    RSDK_THIS(Animals);

    if (self->velocity.y <= 0)
        return false;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, Animals->hitboxes[self->type], false))
        return true;

    if (Animals->hasPlatform) {
        foreach_active(Platform, platform)
        {
            if (Animals_CheckPlatformCollision(platform))
                return true;
        }
    }

    if (Animals->hasBridge) {
        foreach_active(Bridge, bridge)
        {
            bool32 collided = Bridge_HandleCollisions(self, bridge, &self->hitboxAnimal, false, false);
            if (collided) {
                foreach_return true;
            }
        }
    }
    return false;
}

void Animals_State_Freed(void)
{
    RSDK_THIS(Animals);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    RSDK.ProcessAnimation(&self->animator);

    if (Animals_CheckGroundCollision()) {
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type + 1, &self->animator, true, 0);
        switch (self->type) {
            case ANIMAL_FLICKY:
            case ANIMAL_CUCKY:
            case ANIMAL_LOCKY: self->state = Animals_State_FollowPlayer_Bird; break;

            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: self->state = Animals_State_FollowPlayer_Normal; break;

            default: break;
        }

        Animals_CheckPlayerPos();
        self->velocity.y = Animals->yVelocity[self->type];
    }
}

void Animals_State_FollowPlayer_Normal(void)
{
    RSDK_THIS(Animals);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += Animals->gravityStrength[self->type];

    if (Animals_CheckGroundCollision()) {
        Animals_CheckPlayerPos();
        self->velocity.y = Animals->yVelocity[self->type];
    }

    self->animator.frameID = self->velocity.y < 0;
}

void Animals_State_FollowPlayer_Bird(void)
{
    RSDK_THIS(Animals);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += Animals->gravityStrength[self->type];

    if (Animals_CheckGroundCollision()) {
        Animals_CheckPlayerPos();
        self->velocity.y = Animals->yVelocity[self->type];
    }

    RSDK.ProcessAnimation(&self->animator);
}

void Animals_State_BounceAround(void)
{
    RSDK_THIS(Animals);

    if (self->timer <= 0)
        self->state = Animals_State_Freed;
    else
        self->timer--;
}

#if RETRO_INCLUDE_EDITOR
void Animals_EditorDraw(void)
{
    RSDK_THIS(Animals);

    switch (self->behaviour) {
        default: break;
        case ANIMAL_BEHAVE_BOUNCEAROUND: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0); break;
        case ANIMAL_BEHAVE_FOLLOW:
        case ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type + 1, &self->animator, true, 0); break;
    }

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Animals_EditorLoad(void)
{
    Animals->aniFrames = RSDK.LoadSpriteAnimation("Global/Animals.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Animals, type);
    RSDK_ENUM_VAR("Flicky", ANIMAL_FLICKY);
    RSDK_ENUM_VAR("Ricky", ANIMAL_RICKY);
    RSDK_ENUM_VAR("Pocky", ANIMAL_POCKY);
    RSDK_ENUM_VAR("Pecky", ANIMAL_PECKY);
    RSDK_ENUM_VAR("Picky", ANIMAL_PICKY);
    RSDK_ENUM_VAR("Cucky", ANIMAL_CUCKY);
    RSDK_ENUM_VAR("Rocky", ANIMAL_ROCKY);
    RSDK_ENUM_VAR("Becky", ANIMAL_BECKY);
    RSDK_ENUM_VAR("Locky", ANIMAL_LOCKY);
    RSDK_ENUM_VAR("Tocky", ANIMAL_TOCKY);
    RSDK_ENUM_VAR("Wocky", ANIMAL_WOCKY);
    RSDK_ENUM_VAR("Mickey", ANIMAL_MICKY);

    RSDK_ACTIVE_VAR(Animals, behaviour);
    RSDK_ENUM_VAR("Bounce Around (No Bounds)", ANIMAL_BEHAVE_BOUNCEAROUND);
    RSDK_ENUM_VAR("Follow Player", ANIMAL_BEHAVE_FOLLOW);
    RSDK_ENUM_VAR("Bounce Around (Use Bounds)", ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS);
}
#endif

void Animals_Serialize(void)
{
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, behaviour);
}

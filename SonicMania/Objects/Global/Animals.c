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

    if (self->behaviour == ANIMAL_BEHAVE_FIXED)
        self->active = ACTIVE_BOUNDS;
    else
        self->active = ACTIVE_NORMAL;

    self->drawFX |= FX_FLIP;
    self->visible       = true;
    self->updateRange.x = TO_FIXED(64);
    self->updateRange.y = TO_FIXED(64);
    self->drawGroup     = Zone->objectDrawGroup[0];

    int32 type = ANIMAL_POCKY;
#if MANIA_USE_PLUS
    if (!(globals->secrets & SECRET_RICKYMODE))
#endif
        type = VOID_TO_INT(data);

    if (!self->type && ZONE_RAND(0, 256) == 21) {
        type                      = ANIMAL_POCKY;
        self->velocity.y          = -TO_FIXED(4);
        self->type                = type - 1;
        self->state               = Animals_State_Fall;
        self->hitboxAnimal.top    = -Animals->hitboxes[self->type] >> 16;
        self->hitboxAnimal.left   = -4;
        self->hitboxAnimal.right  = 4;
        self->hitboxAnimal.bottom = Animals->hitboxes[self->type] >> 16;
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0);
    }
    else if (type) {
        self->velocity.y = -TO_FIXED(4);
        self->type       = type - 1;
        self->state      = Animals_State_Fall;

        self->hitboxAnimal.top    = -FROM_FIXED(Animals->hitboxes[self->type]);
        self->hitboxAnimal.left   = -4;
        self->hitboxAnimal.right  = 4;
        self->hitboxAnimal.bottom = FROM_FIXED(Animals->hitboxes[self->type]);

        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0);
    }
    else if (self->behaviour == ANIMAL_BEHAVE_FOLLOW) {
        self->active = ACTIVE_BOUNDS;
        switch (self->type) {
            case ANIMAL_FLICKY:
            case ANIMAL_CUCKY:
            case ANIMAL_LOCKY: self->state = Animals_State_Fly; break;

            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: self->state = Animals_State_Bounce; break;

            default: break;
        }

        Animals_CheckDirection();
        self->velocity.y = Animals->yVelocity[self->type];
        if (self->direction == FLIP_NONE)
            self->velocity.x = -Animals->xVelocity[self->type];
        else
            self->velocity.x = Animals->xVelocity[self->type];
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type + 1, &self->animator, true, 0);
    }
    else {
        self->state = Animals_State_Placed;
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

void Animals_CheckDirection(void)
{
    RSDK_THIS(Animals);

    switch (self->behaviour) {
        default:
        case ANIMAL_BEHAVE_FREE: self->direction = FLIP_X; break;

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

        case ANIMAL_BEHAVE_FIXED: self->direction = ZONE_RAND(0, 2); break;
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
    if (platform->state != Platform_State_Falling2 && platform->state != Platform_State_Hold) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

        if (platform->collision <= PLATFORM_C_SOLID) {
            collided = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), self, &self->hitboxAnimal, true);
        }
        else if (platform->collision == PLATFORM_C_TILED
                 && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &self->hitboxAnimal)) {
            if (self->collisionLayers & Zone->moveLayerMask) {
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

void Animals_State_Fall(void)
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
            case ANIMAL_LOCKY: self->state = Animals_State_Fly; break;

            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: self->state = Animals_State_Bounce; break;

            default: break;
        }

        Animals_CheckDirection();
        self->velocity.y = Animals->yVelocity[self->type];
    }
}

void Animals_State_Bounce(void)
{
    RSDK_THIS(Animals);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += Animals->gravityStrength[self->type];

    if (Animals_CheckGroundCollision()) {
        Animals_CheckDirection();
        self->velocity.y = Animals->yVelocity[self->type];
    }

    self->animator.frameID = self->velocity.y < 0;
}

void Animals_State_Fly(void)
{
    RSDK_THIS(Animals);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += Animals->gravityStrength[self->type];

    if (Animals_CheckGroundCollision()) {
        Animals_CheckDirection();
        self->velocity.y = Animals->yVelocity[self->type];
    }

    RSDK.ProcessAnimation(&self->animator);
}

void Animals_State_Placed(void)
{
    RSDK_THIS(Animals);

    if (self->timer <= 0)
        self->state = Animals_State_Fall;
    else
        self->timer--;
}

#if GAME_INCLUDE_EDITOR
void Animals_EditorDraw(void)
{
    RSDK_THIS(Animals);

    switch (self->behaviour) {
        default: break;
        case ANIMAL_BEHAVE_FREE: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type, &self->animator, true, 0); break;
        case ANIMAL_BEHAVE_FOLLOW:
        case ANIMAL_BEHAVE_FIXED: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * self->type + 1, &self->animator, true, 0); break;
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
    RSDK_ENUM_VAR("Free", ANIMAL_BEHAVE_FREE);
    RSDK_ENUM_VAR("Follow", ANIMAL_BEHAVE_FOLLOW);
    RSDK_ENUM_VAR("Fixed", ANIMAL_BEHAVE_FIXED);
}
#endif

void Animals_Serialize(void)
{
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, behaviour);
}

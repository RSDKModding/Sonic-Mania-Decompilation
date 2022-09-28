// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Flamethrower Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFlamethrower *Flamethrower;

void Flamethrower_Update(void)
{
    RSDK_THIS(Flamethrower);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void Flamethrower_LateUpdate(void) {}

void Flamethrower_StaticUpdate(void)
{
    int32 activeFlamethrowerCount = 0;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        foreach_active(Flamethrower, flamethrower)
        {
            if (flamethrower->state == Flamethrower_State_EmittingFlames)
                activeFlamethrowerCount++;
        }

        if (activeFlamethrowerCount) {
            if (!Flamethrower->playingFlameSfx) {
                RSDK.PlaySfx(Flamethrower->sfxFlame, true, 255);
                Flamethrower->playingFlameSfx = true;
            }
        }
    }

    if (!activeFlamethrowerCount && Flamethrower->playingFlameSfx) {
        RSDK.StopSfx(Flamethrower->sfxFlame);
        Flamethrower->playingFlameSfx = false;
    }
}

void Flamethrower_Draw(void)
{
    RSDK_THIS(Flamethrower);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Flamethrower_Create(void *data)
{
    RSDK_THIS(Flamethrower);

    self->active = ACTIVE_BOUNDS;

    if (!RSDK.CheckSceneFolder("LRZ3")) {
        if (Flamethrower->hitboxMouthH.left)
            self->drawGroup = Zone->playerDrawGroup[0];
        else
            self->drawGroup = Zone->objectDrawGroup[0];
    }
    else {
        self->drawGroup = Zone->playerDrawGroup[0];
    }

    self->origin        = self->position;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (SceneInfo->inEditor) {
        if (!self->maxDist)
            self->maxDist = 96;

        if (!self->spread)
            self->spread = 42;

        if (!self->duration)
            self->duration = 120;

        if (!self->interval)
            self->interval = 60;
    }

    self->state = Flamethrower_State_Init;
}

void Flamethrower_StageLoad(void)
{
    Flamethrower->hitboxMouthH.left   = -4;
    Flamethrower->hitboxMouthH.top    = -17;
    Flamethrower->hitboxMouthH.right  = 12;
    Flamethrower->hitboxMouthH.bottom = 17;

    Flamethrower->hitboxMouthV.left   = -16;
    Flamethrower->hitboxMouthV.top    = 0;
    Flamethrower->hitboxMouthV.right  = 16;
    Flamethrower->hitboxMouthV.bottom = 12;

    Flamethrower->hitboxSmallFireball.left   = -4;
    Flamethrower->hitboxSmallFireball.top    = -4;
    Flamethrower->hitboxSmallFireball.right  = 4;
    Flamethrower->hitboxSmallFireball.bottom = 4;

    Flamethrower->hitboxMediumFireball.left   = -6;
    Flamethrower->hitboxMediumFireball.top    = -6;
    Flamethrower->hitboxMediumFireball.right  = 6;
    Flamethrower->hitboxMediumFireball.bottom = 6;

    Flamethrower->hitboxLargeFireball.left   = -9;
    Flamethrower->hitboxLargeFireball.top    = -9;
    Flamethrower->hitboxLargeFireball.right  = 9;
    Flamethrower->hitboxLargeFireball.bottom = 9;

    Flamethrower->sfxFlame = RSDK.GetSfx("Stage/Flame2.wav");

    if (RSDK.CheckSceneFolder("LRZ2")) {
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Flamethrower.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ3")) {
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Flamethrower.bin", SCOPE_STAGE);

        Flamethrower->hitboxMouthH.left = 0;
    }

    Flamethrower->active = ACTIVE_ALWAYS;
}

void Flamethrower_SetupOrientation(uint8 orientation)
{
    RSDK_THIS(Flamethrower);

    if (orientation == FLAMETHROWER_ORIENTATION_DOWN || orientation == FLAMETHROWER_ORIENTATION_UP)
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, 1, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, 0, &self->animator, true, 0);
}

Hitbox Flamethrower_GetHitbox(void)
{
    RSDK_THIS(Flamethrower);

    switch (self->animator.animationID) {
        case 2:
        case 5: return Flamethrower->hitboxSmallFireball;

        case 3:
        case 6: return Flamethrower->hitboxMediumFireball;

        default:
        case 4:
        case 7: return Flamethrower->hitboxLargeFireball;
    }
}

void Flamethrower_CheckOffScreen(void)
{
    RSDK_THIS(Flamethrower);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Flamethrower_State_AwaitInterval;
    }
}

void Flamethrower_HandleAnimations(void)
{
    RSDK_THIS(Flamethrower);

    int32 dist = 0;
    if (self->mode == FLAMETHROWER_MODE_SPRINKLER) {
        dist = self->currentDist;
    }
    else {
        if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
            dist = abs(self->position.y - self->origin.y) >> 16;
        else
            dist = abs(self->position.x - self->origin.x) >> 16;
    }

    int32 anim = dist > 0x20 ? (dist > 0x40 ? 2 : 1) : 0;

    if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, anim + 5, &self->animator, false, 0);
    else
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, anim + 2, &self->animator, false, 0);
}

void Flamethrower_HandleAngles(void)
{
    RSDK_THIS(Flamethrower);

    if (self->mode != FLAMETHROWER_MODE_EMITTER) {
        if (self->lastPos.x || self->lastPos.y) {
            int32 angle = RSDK.ATan2(self->position.x - self->lastPos.x, self->position.y - self->lastPos.y);
            int32 rot   = 2 * angle;

            switch (self->orientation) {
                default:
                case FLAMETHROWER_ORIENTATION_RIGHT: self->rotation = rot; break;
                case FLAMETHROWER_ORIENTATION_DOWN: self->rotation = rot + 0x180; break;
                case FLAMETHROWER_ORIENTATION_LEFT: self->rotation = rot + 0x100; break;
                case FLAMETHROWER_ORIENTATION_UP: self->rotation = rot + 0x180; break;
            }
            self->rotation &= 0x1FF;
        }
        else {
            self->rotation = 0;
        }
    }
    else {
        self->rotation = self->orientation == FLAMETHROWER_ORIENTATION_UP ? 0x100 : 0x00;
    }
}

void Flamethrower_HandleTileCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (self->timer >= 15) {
        Hitbox hitbox = Flamethrower_GetHitbox();

        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, hitbox.bottom << 13, 4)
            || RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, hitbox.right << 13, 0, 4)
            || RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, hitbox.top << 13, 4)
            || RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, hitbox.left << 13, 0, 4)) {
            destroyEntity(self);
        }
    }
}

void Flamethrower_CheckOutOfBounds(uint8 orientation)
{
    RSDK_THIS(Flamethrower);

    int32 dist = 0;
    if (self->mode == FLAMETHROWER_MODE_SPRINKLER) {
        dist = self->currentDist;
    }
    else {
        if (orientation == FLAMETHROWER_ORIENTATION_DOWN || orientation == FLAMETHROWER_ORIENTATION_UP)
            dist = abs(self->position.y - self->origin.y) >> 16;
        else
            dist = abs(self->position.x - self->origin.x) >> 16;
    }

    if (dist >= self->maxDist)
        destroyEntity(self);
}

void Flamethrower_CheckFlameCollisions(void)
{
    RSDK_THIS(Flamethrower);

    Hitbox hitbox = Flamethrower_GetHitbox();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &hitbox)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }
}

void Flamethrower_CheckMouthCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (Flamethrower->hitboxMouthH.left) {
        Hitbox *hitbox;
        if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
            hitbox = &Flamethrower->hitboxMouthV;
        else
            hitbox = &Flamethrower->hitboxMouthH;

        foreach_active(Player, player) { Player_CheckCollisionBox(player, self, hitbox); }
    }
}

void Flamethrower_State_Init(void)
{
    RSDK_THIS(Flamethrower);

    self->timer  = 0;
    self->active = ACTIVE_BOUNDS;

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = MANIA_USE_PLUS ? FLIP_NONE : FLIP_Y; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = MANIA_USE_PLUS ? FLIP_Y : FLIP_NONE; break;
    }

    Flamethrower_SetupOrientation(self->orientation);
    if (!RSDK.CheckSceneFolder("LRZ3")) {
        if (Flamethrower->hitboxMouthH.left)
            self->drawGroup = Zone->playerDrawGroup[0];
        else
            self->drawGroup = Zone->objectDrawGroup[0];
    }
    else {
        self->drawGroup = Zone->playerDrawGroup[0];
    }

    self->state = Flamethrower_State_AwaitInterval;
    Flamethrower_State_AwaitInterval();
}

void Flamethrower_State_AwaitInterval(void)
{
    RSDK_THIS(Flamethrower);

    self->active = ACTIVE_BOUNDS;
    if (!self->interval || !((Zone->timer + self->intervalOffset) % (self->interval + self->duration))) {
        self->active = ACTIVE_NORMAL;
        self->timer  = 0;
        self->state  = Flamethrower_State_EmittingFlames;
    }

    Flamethrower_CheckMouthCollisions();
}

void Flamethrower_State_EmittingFlames(void)
{
    RSDK_THIS(Flamethrower);

    int32 angle = 0;
    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: angle = 0x00; break;
        case FLAMETHROWER_ORIENTATION_DOWN: angle = 0x80; break;
        case FLAMETHROWER_ORIENTATION_LEFT: angle = 0x100; break;
        case FLAMETHROWER_ORIENTATION_UP: angle = 0x180; break;
    }

    self->angle = (angle + (((self->spread >> 1) * RSDK.Sin512(8 * self->flameAngle)) >> 9)) & 0x1FF;
    if (!(self->timer & 3)) {
        EntityFlamethrower *flame = CREATE_ENTITY(Flamethrower, self, self->position.x, self->position.y);
        flame->active             = ACTIVE_NORMAL;
        flame->visible            = true;
        if (RSDK.CheckSceneFolder("LRZ3"))
            flame->drawGroup = Zone->objectDrawGroup[0] - 1;
        else
            flame->drawGroup = Zone->objectDrawGroup[0];
        flame->maxDist     = self->maxDist;
        flame->state       = Flamethrower_State_SetupFireball;
        flame->angle       = self->angle;
        flame->orientation = self->orientation;
        flame->mode        = self->mode;
        flame->direction   = self->direction;
        flame->parent      = self;
        flame->velocity.x  = RSDK.Cos512(self->angle) << 17 >> 8;
        flame->velocity.y  = RSDK.Sin512(self->angle) << 17 >> 8;
    }

    if (self->interval && self->timer >= self->duration) {
        self->active = ACTIVE_BOUNDS;
        self->timer  = 0;
        self->state  = Flamethrower_State_AwaitInterval;
    }

    Flamethrower_CheckOffScreen();

    ++self->timer;
    ++self->flameAngle;

    Flamethrower_CheckMouthCollisions();
}

void Flamethrower_State_SetupFireball(void)
{
    RSDK_THIS(Flamethrower);

    self->timer  = 0;
    self->active = ACTIVE_NORMAL;
    self->drawFX = FX_ROTATE | FX_FLIP;

    Flamethrower_HandleAnimations();

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = MANIA_USE_PLUS ? FLIP_NONE : FLIP_Y; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = MANIA_USE_PLUS ? FLIP_Y : FLIP_NONE; break;
    }

    if (RSDK.CheckSceneFolder("LRZ3"))
        self->drawGroup = Zone->objectDrawGroup[0] - 1;
    else
        self->drawGroup = Zone->objectDrawGroup[0];

    self->state         = Flamethrower_State_Fireball;
    self->lastPos.x     = 0;
    self->lastPos.y     = 0;
    self->lastYVelocity = 0;

    Flamethrower_State_Fireball();
}

void Flamethrower_State_Fireball(void)
{
    RSDK_THIS(Flamethrower);

    if (self->mode == FLAMETHROWER_MODE_SPRINKLER)
        self->velocity.y += 0x3800;

    if (self->velocity.y < self->lastYVelocity)
        self->lastYVelocity = self->velocity.y;

    // Check for flipping downwards after going upwards
    if (self->currentDist > 12 && self->lastYVelocity < 0 && self->velocity.y > 0 && self->drawGroup == Zone->objectDrawGroup[0])
        self->drawGroup = Zone->objectDrawGroup[1];

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (HeavyRider && self->velocity.y > 0)
        self->drawGroup = Zone->objectDrawGroup[1];

    if (self->lastPos.x && self->lastPos.y) {
        int32 rx = (self->position.x - self->lastPos.x) >> 8;
        int32 ry = (self->position.y - self->lastPos.y) >> 8;
        self->currentDist += MathHelpers_SquareRoot((rx * rx + ry * ry) >> 16);
    }

#if MANIA_USE_PLUS
    if (self->orientation == FLAMETHROWER_ORIENTATION_UP && self->velocity.y > 0) {
        self->orientation = FLAMETHROWER_ORIENTATION_DOWN;
        self->direction   = FLIP_NONE;
    }
#endif

    Flamethrower_HandleAnimations();
    Flamethrower_HandleTileCollisions();
    Flamethrower_HandleAngles();

    if (self->mode == FLAMETHROWER_MODE_EMITTER)
        Flamethrower_CheckOutOfBounds(self->orientation);

    Flamethrower_CheckFlameCollisions();

    ++self->timer;
    self->lastPos = self->position;
}

#if RETRO_INCLUDE_EDITOR
void Flamethrower_EditorDraw(void)
{
    RSDK_THIS(Flamethrower);

    Flamethrower_Create(NULL);
    Flamethrower_SetupOrientation(self->orientation);

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = MANIA_USE_PLUS ? FLIP_NONE : FLIP_Y; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = MANIA_USE_PLUS ? FLIP_Y : FLIP_NONE; break;
    }

    Flamethrower_Draw();
}

void Flamethrower_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ2"))
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Flamethrower.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ3"))
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Flamethrower.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Flamethrower, orientation);
    RSDK_ENUM_VAR("Right", FLAMETHROWER_ORIENTATION_RIGHT);
    RSDK_ENUM_VAR("Down", FLAMETHROWER_ORIENTATION_DOWN);
    RSDK_ENUM_VAR("Left", FLAMETHROWER_ORIENTATION_LEFT);
    RSDK_ENUM_VAR("Up", FLAMETHROWER_ORIENTATION_UP);

    RSDK_ACTIVE_VAR(Flamethrower, mode);
    RSDK_ENUM_VAR("Emitter", FLAMETHROWER_MODE_EMITTER);
    RSDK_ENUM_VAR("Sprinkler", FLAMETHROWER_MODE_SPRINKLER);
}
#endif

void Flamethrower_Serialize(void)
{
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT32, maxDist);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT16, spread);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Flamethrower, VAR_UINT16, duration);
}

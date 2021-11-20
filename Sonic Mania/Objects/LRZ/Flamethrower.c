#include "SonicMania.h"

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
    int count = 0;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_active(Flamethrower, flamethrower)
        {
            if (flamethrower->state == Flamethrower_State_Unknown3)
                count++;
        }
        if (count) {
            if (!Flamethrower->playingFlameSfx) {
                RSDK.PlaySfx(Flamethrower->sfxFlame, true, 255);
                Flamethrower->playingFlameSfx = true;
            }
        }
    }

    if (!count && Flamethrower->playingFlameSfx) {
        RSDK.StopSFX(Flamethrower->sfxFlame);
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

    if (!RSDK.CheckStageFolder("LRZ3")) {
        if (Flamethrower->hitbox1.left) {
            self->drawOrder = Zone->playerDrawLow;
        }
        else
            self->drawOrder = Zone->drawOrderLow;
    }
    else {
        self->drawOrder = Zone->playerDrawLow;
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
    self->state = Flamethrower_State_Unknown1;
}

void Flamethrower_StageLoad(void)
{
    Flamethrower->hitbox1.left   = -4;
    Flamethrower->hitbox1.top    = -17;
    Flamethrower->hitbox1.right  = 12;
    Flamethrower->hitbox1.bottom = 17;

    Flamethrower->hitbox2.left   = -16;
    Flamethrower->hitbox2.top    = 0;
    Flamethrower->hitbox2.right  = 16;
    Flamethrower->hitbox2.bottom = 12;

    Flamethrower->hitbox3.left   = -4;
    Flamethrower->hitbox3.top    = -4;
    Flamethrower->hitbox3.right  = 4;
    Flamethrower->hitbox3.bottom = 4;

    Flamethrower->hitbox4.left   = -6;
    Flamethrower->hitbox4.top    = -6;
    Flamethrower->hitbox4.right  = 6;
    Flamethrower->hitbox4.bottom = 6;

    Flamethrower->hitbox5.left   = -9;
    Flamethrower->hitbox5.top    = -9;
    Flamethrower->hitbox5.right  = 9;
    Flamethrower->hitbox5.bottom = 9;

    Flamethrower->sfxFlame = RSDK.GetSFX("Stage/Flame2.wav");
    if (RSDK.CheckStageFolder("LRZ2")) {
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Flamethrower.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ3")) {
        Flamethrower->aniFrames    = RSDK.LoadSpriteAnimation("LRZ3/Flamethrower.bin", SCOPE_STAGE);
        Flamethrower->hitbox1.left = 0;
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
        case 5: return Flamethrower->hitbox3;
        case 3:
        case 6: return Flamethrower->hitbox4;
        default: return Flamethrower->hitbox5;
    }
}

void Flamethrower_CheckOnScreen(void)
{
    RSDK_THIS(Flamethrower);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Flamethrower_State_Unknown2;
    }
}

void Flamethrower_Unknown3(void)
{
    RSDK_THIS(Flamethrower);

    int dist = 0;
    if (self->mode == 1) {
        dist = self->field_74;
    }
    else {
        if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
            dist = abs(self->position.y - self->origin.y) >> 16;
        else
            dist = abs(self->position.x - self->origin.x) >> 16;
    }

    int anim = dist > 0x20;
    if (dist > 0x40)
        anim = 2;

    if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, (anim + 5), &self->animator, false, 0);
    else
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, (anim + 2), &self->animator, false, 0);
}

void Flamethrower_HandleAngles(void)
{
    RSDK_THIS(Flamethrower);

    if (self->mode) {
        if (self->field_84.x || self->field_84.y) {
            int angle = RSDK.ATan2(self->position.x - self->field_84.x, self->position.y - self->field_84.y);
            int rot   = 2 * angle;

            switch (self->orientation) {
                default:
                case FLAMETHROWER_ORIENTATION_RIGHT: self->rotation = (rot & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_DOWN: self->rotation = ((rot + 0x180) & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_LEFT: self->rotation = ((rot + 0x100) & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_UP: self->rotation = ((rot + 0x180) & 0x1FF); break;
            }
        }
        else {
            self->rotation = 0;
        }
    }
    else {
        if (self->orientation == FLAMETHROWER_ORIENTATION_UP)
            self->rotation = 0x100;
        else
            self->rotation = 0;
    }
}

void Flamethrower_HandleTileCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (self->timer >= 15) {
        Hitbox hitbox = Flamethrower_GetHitbox();

        if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox.bottom << 13, 4)
            || RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, hitbox.right << 13, 0, 4)
            || RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_ROOF, 0, 0, hitbox.top << 13, 4)
            || RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, hitbox.left << 13, 0, 4)) {
            destroyEntity(self);
        }
    }
}

void Flamethrower_Unknown7(uint8 orientation)
{
    RSDK_THIS(Flamethrower);

    int dist = 0;
    if (self->mode == 1) {
        dist = self->field_74;
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
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }
}

void Flamethrower_CheckBaseCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (Flamethrower->hitbox1.left) {
        Hitbox hitbox;
        if (self->orientation == FLAMETHROWER_ORIENTATION_DOWN || self->orientation == FLAMETHROWER_ORIENTATION_UP)
            hitbox = Flamethrower->hitbox2;
        else
            hitbox = Flamethrower->hitbox1;

        foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &hitbox); }
    }
}

void Flamethrower_State_Unknown1(void)
{
    RSDK_THIS(Flamethrower);

    self->timer  = 0;
    self->active = ACTIVE_BOUNDS;

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = FLIP_Y; break;
    }

    Flamethrower_SetupOrientation(self->orientation);
    if (!RSDK.CheckStageFolder("LRZ3")) {
        if (Flamethrower->hitbox1.left) {
            self->drawOrder = Zone->playerDrawLow;
        }
        else
            self->drawOrder = Zone->drawOrderLow;
    }
    else {
        self->drawOrder = Zone->playerDrawLow;
    }

    self->state = Flamethrower_State_Unknown2;
    Flamethrower_State_Unknown2();
}

void Flamethrower_State_Unknown2(void)
{
    RSDK_THIS(Flamethrower);

    self->active = ACTIVE_BOUNDS;
    if (!self->interval || !((Zone->timer + self->intervalOffset) % (self->interval + self->duration))) {
        self->active = ACTIVE_NORMAL;
        self->timer  = 0;
        self->state  = Flamethrower_State_Unknown3;
    }
    Flamethrower_CheckBaseCollisions();
}

void Flamethrower_State_Unknown3(void)
{
    RSDK_THIS(Flamethrower);

    int angle = 0;
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
        flame->visible            = 1;
        if (RSDK.CheckStageFolder("LRZ3"))
            flame->drawOrder = Zone->drawOrderLow - 1;
        else
            flame->drawOrder = Zone->drawOrderLow;
        flame->maxDist     = self->maxDist;
        flame->state       = Flamethrower_State1_Unknown1;
        flame->angle       = self->angle;
        flame->orientation = self->orientation;
        flame->mode        = self->mode;
        flame->direction   = self->direction;
        flame->parent      = (Entity *)self;
        flame->velocity.x  = RSDK.Cos512(self->angle) << 17 >> 8;
        flame->velocity.y  = RSDK.Sin512(self->angle) << 17 >> 8;
    }
    if (self->interval && self->timer >= self->duration) {
        self->active = ACTIVE_BOUNDS;
        self->timer  = 0;
        self->state  = Flamethrower_State_Unknown2;
    }
    Flamethrower_CheckOnScreen();
    ++self->timer;
    ++self->flameAngle;
    Flamethrower_CheckBaseCollisions();
}

void Flamethrower_State1_Unknown1(void)
{
    RSDK_THIS(Flamethrower);

    self->timer  = 0;
    self->active = ACTIVE_NORMAL;
    self->drawFX = FX_ROTATE | FX_FLIP;
    Flamethrower_Unknown3();

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = FLIP_Y; break;
    }

    if (RSDK.CheckStageFolder("LRZ3"))
        self->drawOrder = Zone->drawOrderLow - 1;
    else
        self->drawOrder = Zone->drawOrderLow;
    self->state      = Flamethrower_State1_Unknown2;
    self->field_84.x = 0;
    self->field_84.y = 0;
    self->field_70   = 0;
    Flamethrower_State1_Unknown2();
}

void Flamethrower_State1_Unknown2(void)
{
    RSDK_THIS(Flamethrower);

    if (self->mode == 1)
        self->velocity.y += 0x3800;

    if (self->velocity.y < self->field_70)
        self->field_70 = self->velocity.y;
    if (self->field_74 > 12 && self->field_70 < 0 && self->velocity.y > 0 && self->drawOrder == Zone->drawOrderLow)
        self->drawOrder = Zone->drawOrderHigh;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (HeavyRider && self->velocity.y > 0)
        self->drawOrder = Zone->drawOrderHigh;

    if (self->field_84.x && self->field_84.y) {
        int rx = (self->position.x - self->field_84.x) >> 8;
        int ry = (self->position.y - self->field_84.y) >> 8;
        self->field_74 += MathHelpers_SquareRoot((rx * rx + ry * ry) >> 16);
    }
    if (self->orientation == FLAMETHROWER_ORIENTATION_UP && self->velocity.y > 0) {
        self->orientation = FLAMETHROWER_ORIENTATION_DOWN;
        self->direction   = FLIP_NONE;
    }

    Flamethrower_Unknown3();
    Flamethrower_HandleTileCollisions();
    Flamethrower_HandleAngles();
    if (!self->mode)
        Flamethrower_Unknown7(self->orientation);
    Flamethrower_CheckFlameCollisions();

    ++self->timer;
    self->field_84.x = self->position.x;
    self->field_84.y = self->position.y;
}

#if RETRO_INCLUDE_EDITOR
void Flamethrower_EditorDraw(void)
{
    RSDK_THIS(Flamethrower);
    Flamethrower_SetupOrientation(self->orientation);

    switch (self->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: self->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: self->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: self->direction = FLIP_Y; break;
    }

    Flamethrower_Draw();
}

void Flamethrower_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ2"))
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Flamethrower.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ3"))
        Flamethrower->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Flamethrower.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Flamethrower, orientation);
    RSDK_ENUM_VAR("Right", FLAMETHROWER_ORIENTATION_RIGHT);
    RSDK_ENUM_VAR("Down", FLAMETHROWER_ORIENTATION_DOWN);
    RSDK_ENUM_VAR("Left", FLAMETHROWER_ORIENTATION_LEFT);
    RSDK_ENUM_VAR("Up", FLAMETHROWER_ORIENTATION_UP);
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

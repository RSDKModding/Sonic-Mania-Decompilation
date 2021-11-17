#include "SonicMania.h"

ObjectFlamethrower *Flamethrower;

void Flamethrower_Update(void)
{
    RSDK_THIS(Flamethrower);

    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
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
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Flamethrower_Create(void *data)
{
    RSDK_THIS(Flamethrower);

    entity->active = ACTIVE_BOUNDS;

    if (!RSDK.CheckStageFolder("LRZ3")) {
        if (Flamethrower->hitbox1.left) {
            entity->drawOrder = Zone->playerDrawLow;
        }
        else
            entity->drawOrder = Zone->drawOrderLow;
    }
    else {
        entity->drawOrder = Zone->playerDrawLow;
    }

    entity->origin        = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (SceneInfo->inEditor) {
        if (!entity->maxDist)
            entity->maxDist = 96;
        if (!entity->spread)
            entity->spread = 42;
        if (!entity->duration)
            entity->duration = 120;
        if (!entity->interval)
            entity->interval = 60;
    }
    entity->state = Flamethrower_State_Unknown1;
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
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, 1, &entity->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, 0, &entity->animator, true, 0);
}

Hitbox Flamethrower_GetHitbox(void)
{
    RSDK_THIS(Flamethrower);
    switch (entity->animator.animationID) {
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

    if (!RSDK.CheckOnScreen(SceneInfo->entity, NULL)) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Flamethrower_State_Unknown2;
    }
}

void Flamethrower_Unknown3(void)
{
    RSDK_THIS(Flamethrower);

    int dist = 0;
    if (entity->mode == 1) {
        dist = entity->field_74;
    }
    else {
        if (entity->orientation == FLAMETHROWER_ORIENTATION_DOWN || entity->orientation == FLAMETHROWER_ORIENTATION_UP)
            dist = abs(entity->position.y - entity->origin.y) >> 16;
        else
            dist = abs(entity->position.x - entity->origin.x) >> 16;
    }

    int anim = dist > 0x20;
    if (dist > 0x40)
        anim = 2;

    if (entity->orientation == FLAMETHROWER_ORIENTATION_DOWN || entity->orientation == FLAMETHROWER_ORIENTATION_UP)
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, (anim + 5), &entity->animator, false, 0);
    else
        RSDK.SetSpriteAnimation(Flamethrower->aniFrames, (anim + 2), &entity->animator, false, 0);
}

void Flamethrower_HandleAngles(void)
{
    RSDK_THIS(Flamethrower);

    if (entity->mode) {
        if (entity->field_84.x || entity->field_84.y) {
            int angle = RSDK.ATan2(entity->position.x - entity->field_84.x, entity->position.y - entity->field_84.y);
            int rot   = 2 * angle;

            switch (entity->orientation) {
                default:
                case FLAMETHROWER_ORIENTATION_RIGHT: entity->rotation = (rot & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_DOWN: entity->rotation = ((rot + 0x180) & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_LEFT: entity->rotation = ((rot + 0x100) & 0x1FF); break;
                case FLAMETHROWER_ORIENTATION_UP: entity->rotation = ((rot + 0x180) & 0x1FF); break;
            }
        }
        else {
            entity->rotation = 0;
        }
    }
    else {
        if (entity->orientation == FLAMETHROWER_ORIENTATION_UP)
            entity->rotation = 0x100;
        else
            entity->rotation = 0;
    }
}

void Flamethrower_HandleTileCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (entity->timer >= 15) {
        Hitbox hitbox = Flamethrower_GetHitbox();

        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, hitbox.bottom << 13, 4)
            || RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, hitbox.right << 13, 0, 4)
            || RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, hitbox.top << 13, 4)
            || RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, hitbox.left << 13, 0, 4)) {
            destroyEntity(entity);
        }
    }
}

void Flamethrower_Unknown7(uint8 orientation)
{
    RSDK_THIS(Flamethrower);

    int dist = 0;
    if (entity->mode == 1) {
        dist = entity->field_74;
    }
    else {
        if (orientation == FLAMETHROWER_ORIENTATION_DOWN || orientation == FLAMETHROWER_ORIENTATION_UP)
            dist = abs(entity->position.y - entity->origin.y) >> 16;
        else
            dist = abs(entity->position.x - entity->origin.x) >> 16;
    }

    if (dist >= entity->maxDist)
        destroyEntity(entity);
}

void Flamethrower_CheckFlameCollisions(void)
{
    RSDK_THIS(Flamethrower);

    Hitbox hitbox = Flamethrower_GetHitbox();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }
}

void Flamethrower_CheckBaseCollisions(void)
{
    RSDK_THIS(Flamethrower);

    if (Flamethrower->hitbox1.left) {
        Hitbox hitbox;
        if (entity->orientation == FLAMETHROWER_ORIENTATION_DOWN || entity->orientation == FLAMETHROWER_ORIENTATION_UP)
            hitbox = Flamethrower->hitbox2;
        else
            hitbox = Flamethrower->hitbox1;

        foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &hitbox); }
    }
}

void Flamethrower_State_Unknown1(void)
{
    RSDK_THIS(Flamethrower);

    entity->timer  = 0;
    entity->active = ACTIVE_BOUNDS;

    switch (entity->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: entity->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: entity->direction = FLIP_Y; break;
    }

    Flamethrower_SetupOrientation(entity->orientation);
    if (!RSDK.CheckStageFolder("LRZ3")) {
        if (Flamethrower->hitbox1.left) {
            entity->drawOrder = Zone->playerDrawLow;
        }
        else
            entity->drawOrder = Zone->drawOrderLow;
    }
    else {
        entity->drawOrder = Zone->playerDrawLow;
    }

    entity->state = Flamethrower_State_Unknown2;
    Flamethrower_State_Unknown2();
}

void Flamethrower_State_Unknown2(void)
{
    RSDK_THIS(Flamethrower);

    entity->active = ACTIVE_BOUNDS;
    if (!entity->interval || !((Zone->timer + entity->intervalOffset) % (entity->interval + entity->duration))) {
        entity->active = ACTIVE_NORMAL;
        entity->timer  = 0;
        entity->state  = Flamethrower_State_Unknown3;
    }
    Flamethrower_CheckBaseCollisions();
}

void Flamethrower_State_Unknown3(void)
{
    RSDK_THIS(Flamethrower);

    int angle = 0;
    switch (entity->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: angle = 0x00; break;
        case FLAMETHROWER_ORIENTATION_DOWN: angle = 0x80; break;
        case FLAMETHROWER_ORIENTATION_LEFT: angle = 0x100; break;
        case FLAMETHROWER_ORIENTATION_UP: angle = 0x180; break;
    }

    entity->angle = (angle + (((entity->spread >> 1) * RSDK.Sin512(8 * entity->flameAngle)) >> 9)) & 0x1FF;
    if (!(entity->timer & 3)) {
        EntityFlamethrower *flame = CREATE_ENTITY(Flamethrower, entity, entity->position.x, entity->position.y);
        flame->active             = ACTIVE_NORMAL;
        flame->visible            = 1;
        if (RSDK.CheckStageFolder("LRZ3"))
            flame->drawOrder = Zone->drawOrderLow - 1;
        else
            flame->drawOrder = Zone->drawOrderLow;
        flame->maxDist     = entity->maxDist;
        flame->state       = Flamethrower_State1_Unknown1;
        flame->angle       = entity->angle;
        flame->orientation = entity->orientation;
        flame->mode        = entity->mode;
        flame->direction   = entity->direction;
        flame->parent      = (Entity *)entity;
        flame->velocity.x  = RSDK.Cos512(entity->angle) << 17 >> 8;
        flame->velocity.y  = RSDK.Sin512(entity->angle) << 17 >> 8;
    }
    if (entity->interval && entity->timer >= entity->duration) {
        entity->active = ACTIVE_BOUNDS;
        entity->timer  = 0;
        entity->state  = Flamethrower_State_Unknown2;
    }
    Flamethrower_CheckOnScreen();
    ++entity->timer;
    ++entity->flameAngle;
    Flamethrower_CheckBaseCollisions();
}

void Flamethrower_State1_Unknown1(void)
{
    RSDK_THIS(Flamethrower);

    entity->timer  = 0;
    entity->active = ACTIVE_NORMAL;
    entity->drawFX = FX_ROTATE | FX_FLIP;
    Flamethrower_Unknown3();

    switch (entity->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: entity->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: entity->direction = FLIP_Y; break;
    }

    if (RSDK.CheckStageFolder("LRZ3"))
        entity->drawOrder = Zone->drawOrderLow - 1;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->state      = Flamethrower_State1_Unknown2;
    entity->field_84.x = 0;
    entity->field_84.y = 0;
    entity->field_70   = 0;
    Flamethrower_State1_Unknown2();
}

void Flamethrower_State1_Unknown2(void)
{
    RSDK_THIS(Flamethrower);

    if (entity->mode == 1)
        entity->velocity.y += 0x3800;

    if (entity->velocity.y < entity->field_70)
        entity->field_70 = entity->velocity.y;
    if (entity->field_74 > 12 && entity->field_70 < 0 && entity->velocity.y > 0 && entity->drawOrder == Zone->drawOrderLow)
        entity->drawOrder = Zone->drawOrderHigh;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (HeavyRider && entity->velocity.y > 0)
        entity->drawOrder = Zone->drawOrderHigh;

    if (entity->field_84.x && entity->field_84.y) {
        int rx = (entity->position.x - entity->field_84.x) >> 8;
        int ry = (entity->position.y - entity->field_84.y) >> 8;
        entity->field_74 += MathHelpers_SquareRoot((rx * rx + ry * ry) >> 16);
    }
    if (entity->orientation == FLAMETHROWER_ORIENTATION_UP && entity->velocity.y > 0) {
        entity->orientation = FLAMETHROWER_ORIENTATION_DOWN;
        entity->direction   = FLIP_NONE;
    }

    Flamethrower_Unknown3();
    Flamethrower_HandleTileCollisions();
    Flamethrower_HandleAngles();
    if (!entity->mode)
        Flamethrower_Unknown7(entity->orientation);
    Flamethrower_CheckFlameCollisions();

    ++entity->timer;
    entity->field_84.x = entity->position.x;
    entity->field_84.y = entity->position.y;
}

#if RETRO_INCLUDE_EDITOR
void Flamethrower_EditorDraw(void)
{
    RSDK_THIS(Flamethrower);
    Flamethrower_SetupOrientation(entity->orientation);

    switch (entity->orientation) {
        default:
        case FLAMETHROWER_ORIENTATION_RIGHT: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_DOWN: entity->direction = FLIP_NONE; break;
        case FLAMETHROWER_ORIENTATION_LEFT: entity->direction = FLIP_X; break;
        case FLAMETHROWER_ORIENTATION_UP: entity->direction = FLIP_Y; break;
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

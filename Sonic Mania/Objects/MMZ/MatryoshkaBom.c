#include "SonicMania.h"

ObjectMatryoshkaBom *MatryoshkaBom;

void MatryoshkaBom_Update(void)
{
    RSDK_THIS(MatryoshkaBom);
    StateMachine_Run(entity->state);
}

void MatryoshkaBom_LateUpdate(void) {}

void MatryoshkaBom_StaticUpdate(void) {}

void MatryoshkaBom_Draw(void)
{
    RSDK_THIS(MatryoshkaBom);
    if (entity->state == MatryoshkaBom_Unknown8) {
        Vector2 drawPos;
        drawPos.x = entity->position.x;
        drawPos.y = entity->field_84 + entity->position.y;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void MatryoshkaBom_Create(void *data)
{
    RSDK_THIS(MatryoshkaBom);

    entity->visible = true;
    if (entity->planeFilter > 0 && ((uint8)(entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh + 2;
    else
        entity->drawOrder = Zone->drawOrderLow + 2;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    int32 size = voidToInt(data);
    if (size < MATRYOSHKA_SIZE_SHRAPNEL) {
        entity->active = ACTIVE_BOUNDS;
        if (size)
            entity->size = size;
        entity->startDir = entity->direction;
        entity->startPos = entity->position;

        if (!size)
            entity->velocity.x = -0xC000;
        else
            entity->velocity.x = 0xC000;
        entity->drawFX |= FX_FLIP;
        entity->timer = 0x600;

        switch (entity->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &entity->animator3, true, 0);
                entity->offsetY    = 0x1B0000;
                entity->offsetX    = 0x140000;
                entity->canExplode = true;
                break;
            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &entity->animator3, true, 0);
                entity->offsetY    = 0x140000;
                entity->offsetX    = 0xD0000;
                entity->canExplode = true;
                break;
            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &entity->animator3, true, 0);
                entity->canExplode = false;
                entity->offsetY    = 0xE0000;
                entity->offsetX    = 0x90000;
                break;
        }
        entity->state = MatryoshkaBom_State_Setup;
    }
    else {
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 16, &entity->animator1, true, 0);
        entity->state = MatryoshkaBom_Unknown11;
    }
}

void MatryoshkaBom_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        MatryoshkaBom->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MatryoshkaBom.bin", SCOPE_STAGE);
    MatryoshkaBom->hitboxHurt.left   = -12;
    MatryoshkaBom->hitboxHurt.top    = -18;
    MatryoshkaBom->hitboxHurt.right  = 12;
    MatryoshkaBom->hitboxHurt.bottom = 18;

    MatryoshkaBom->hitboxExplode.left   = -96;
    MatryoshkaBom->hitboxExplode.top    = -96;
    MatryoshkaBom->hitboxExplode.right  = 96;
    MatryoshkaBom->hitboxExplode.bottom = 96;

    MatryoshkaBom->hitboxShrapnel.left   = -6;
    MatryoshkaBom->hitboxShrapnel.top    = -6;
    MatryoshkaBom->hitboxShrapnel.right  = 6;
    MatryoshkaBom->hitboxShrapnel.bottom = 6;

    DEBUGMODE_ADD_OBJ(MatryoshkaBom);

    MatryoshkaBom->sfxExplosion = RSDK.GetSFX("Stage/Explosion.wav");
    MatryoshkaBom->sfxButton    = RSDK.GetSFX("Stage/Button2.wav");
    MatryoshkaBom->sfxPon       = RSDK.GetSFX("Stage/Pon.wav");
}

void MatryoshkaBom_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(MatryoshkaBom, NULL, entity->position.x, entity->position.y);
}

void MatryoshkaBom_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MatryoshkaBom_CheckPlayerCollisions(void)
{
    RSDK_THIS(MatryoshkaBom);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
            if (entity->state != MatryoshkaBom_Unknown10) {
                if (entity->canExplode) {
                    if (entity->state != MatryoshkaBom_State_Exploding) {
                        if (Player_CheckCollisionTouch(player, entity, &MatryoshkaBom->hitboxExplode)) {
                            RSDK.PlaySfx(MatryoshkaBom->sfxButton, false, 255);

                            switch (entity->size) {
                                default: break;
                                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 1, &entity->animator1, true, 0); break;
                                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 5, &entity->animator1, true, 0); break;
                                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &entity->animator1, true, 0); break;
                            }

                            entity->timer = 21;
                            entity->state = MatryoshkaBom_State_Exploding;
                        }
                    }
                }
                else if (entity->state != MatryoshkaBom_Unknown8) {
                    if (Player_CheckCollisionTouch(player, entity, &MatryoshkaBom->hitboxExplode)) {
                        entity->timer = 144;
                        entity->state = MatryoshkaBom_Unknown8;
                    }
                }
            }

            if (Player_CheckCollisionTouch(player, entity, &MatryoshkaBom->hitboxHurt)
#if RETRO_USE_PLUS
                && !Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer)
#endif
            ) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void MatryoshkaBom_CheckOnScreen(void)
{
    RSDK_THIS(MatryoshkaBom);
    if (!RSDK.CheckOnScreen(SceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        if (entity->destroyOffscreen) {
            destroyEntity(entity);
        }
        else {
            entity->position  = entity->startPos;
            entity->direction = entity->startDir;
            MatryoshkaBom_Create(NULL);
        }
    }
}

void MatryoshkaBom_State_Setup(void)
{
    RSDK_THIS(MatryoshkaBom);
    entity->active = ACTIVE_NORMAL;
    entity->state  = MatryoshkaBom_Unknown5;
    MatryoshkaBom_Unknown5();
}

void MatryoshkaBom_Unknown5(void)
{
    RSDK_THIS(MatryoshkaBom);

    entity->position.x += entity->velocity.x;
    if (!--entity->timer) {
        entity->timer = 60;
        entity->state = MatryoshkaBom_Unknown6;
    }
    else {
        bool32 collided = false;
        if (entity->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -entity->offsetX, 0, true);
        else
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, entity->offsetX, 0, true);

        if (!collided) {
            if (entity->direction & FLIP_Y)
                collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -entity->offsetY, 8);
            else
                collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, entity->offsetY, 8);
        }

        if (!collided) {
            entity->timer = 60;
            entity->state = MatryoshkaBom_Unknown6;
            switch (entity->size) {
                default: break;
                case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &entity->animator1, true, 0); break;
                case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &entity->animator1, true, 0); break;
                case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &entity->animator1, true, 0); break;
            }
        }
    }

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator3);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown6(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (!--entity->timer) {
        switch (entity->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 3, &entity->animator1, true, 0); break;
            case MATRYOSHKA_SIZE_MED: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 7, &entity->animator1, true, 0); break;
            case MATRYOSHKA_SIZE_SMALL: RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 11, &entity->animator1, true, 0); break;
        }
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        entity->state = MatryoshkaBom_Unknown7;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown7(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
        entity->timer      = 0x600;
        entity->state      = MatryoshkaBom_Unknown5;
        switch (entity->size) {
            default: break;
            case MATRYOSHKA_SIZE_BIG:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &entity->animator3, true, 0);
                break;
            case MATRYOSHKA_SIZE_MED:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &entity->animator3, true, 0);
                break;
            case MATRYOSHKA_SIZE_SMALL:
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &entity->animator3, true, 0);
                break;
        }
    }
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown8(void)
{
    RSDK_THIS(MatryoshkaBom);

    if (entity->direction & FLIP_Y)
        entity->field_84 -= 0x1000;
    else
        entity->field_84 += 0x1000;

    if (--entity->timer > 0) {
        RSDK.ProcessAnimation(&entity->animator1);
        RSDK.ProcessAnimation(&entity->animator2);
        MatryoshkaBom_CheckPlayerCollisions();
        MatryoshkaBom_CheckOnScreen();
    }
    else {
        RSDK.PlaySfx(MatryoshkaBom->sfxExplosion, false, 255);

        EntityMatryoshkaBom *shrapnel = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), entity->position.x, entity->position.y);
        shrapnel->velocity.x          = -0x20000;
        shrapnel->velocity.y          = -0x30000;
        shrapnel->planeFilter         = entity->planeFilter;
        shrapnel->drawOrder           = entity->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), entity->position.x, entity->position.y);
        shrapnel->velocity.x  = -0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = entity->planeFilter;
        shrapnel->drawOrder   = entity->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), entity->position.x, entity->position.y);
        shrapnel->velocity.x  = 0x20000;
        shrapnel->velocity.y  = -0x30000;
        shrapnel->planeFilter = entity->planeFilter;
        shrapnel->drawOrder   = entity->drawOrder;

        shrapnel              = CREATE_ENTITY(MatryoshkaBom, intToVoid(3), entity->position.x, entity->position.y);
        shrapnel->velocity.x  = 0x10000;
        shrapnel->velocity.y  = -0x20000;
        shrapnel->planeFilter = entity->planeFilter;
        shrapnel->drawOrder   = entity->drawOrder;

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y);
        explosion->drawOrder       = entity->drawOrder + 1;
        explosion->planeFilter     = entity->planeFilter;

        destroyEntity(entity);
    }
}

void MatryoshkaBom_State_Exploding(void)
{
    RSDK_THIS(MatryoshkaBom);

    RSDK.ProcessAnimation(&entity->animator1);
    if (!--entity->timer) {
        RSDK.PlaySfx(MatryoshkaBom->sfxPon, false, 255);

        EntityMatryoshkaBom *child = CREATE_ENTITY(MatryoshkaBom, intToVoid(entity->size + 1), entity->position.x, entity->position.y);

        if (entity->direction == FLIP_NONE)
            child->velocity.x = -0x18000;
        else
            child->velocity.x = 0x18000;
        child->velocity.y = -0x40000;

        child->planeFilter      = entity->planeFilter;
        child->drawOrder        = entity->drawOrder - 1;
        child->active           = ACTIVE_NORMAL;
        child->direction        = entity->direction;
        child->destroyOffscreen = true;
        child->state            = MatryoshkaBom_Unknown10;
        entity->canExplode      = false;
    }

    if (entity->animator1.frameID >= entity->animator1.frameDelay - 1)
        entity->state = MatryoshkaBom_Unknown5;
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown10(void)
{
    RSDK_THIS(MatryoshkaBom);

    entity->position.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x3800;
    if (entity->velocity.x) {
        bool32 collided = false;
        if (entity->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, -entity->offsetX, 0, true);
        else
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, entity->offsetX, 0, true);

        if (collided)
            entity->velocity.x = 0;
    }

    bool32 collided = false;
    if (entity->direction & FLIP_Y)
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -entity->offsetY, true);
    else
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, entity->offsetY, true);
    if (collided) {
        entity->timer = 0x600;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0xC000;
        else
            entity->velocity.x = 0xC000;
        entity->state = MatryoshkaBom_Unknown5;
    }

    RSDK.ProcessAnimation(&entity->animator1);
    MatryoshkaBom_CheckPlayerCollisions();
    MatryoshkaBom_CheckOnScreen();
}

void MatryoshkaBom_Unknown11(void)
{
    RSDK_THIS(MatryoshkaBom);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        RSDK.ProcessAnimation(&entity->animator1);

        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, entity, &MatryoshkaBom->hitboxShrapnel)) {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void MatryoshkaBom_EditorDraw(void)
{
    RSDK_THIS(MatryoshkaBom);
    entity->drawFX |= FX_FLIP;

    switch (entity->size) {
        default: break;
        case MATRYOSHKA_SIZE_BIG:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 0, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 13, &entity->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 2, &entity->animator3, false, 0);
            break;
        case MATRYOSHKA_SIZE_MED:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 4, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 14, &entity->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 6, &entity->animator3, false, 0);
            break;
        case MATRYOSHKA_SIZE_SMALL:
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 9, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 15, &entity->animator2, false, 0);
            RSDK.SetSpriteAnimation(MatryoshkaBom->aniFrames, 10, &entity->animator3, false, 0);
            break;
    }

    MatryoshkaBom_Draw();
}

void MatryoshkaBom_EditorLoad(void)
{
    MatryoshkaBom->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MatryoshkaBom.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MatryoshkaBom, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);

    RSDK_ACTIVE_VAR(MatryoshkaBom, size);
    RSDK_ENUM_VAR("Big", MATRYOSHKA_SIZE_BIG);
    RSDK_ENUM_VAR("Medium", MATRYOSHKA_SIZE_MED);
    RSDK_ENUM_VAR("Small", MATRYOSHKA_SIZE_SMALL);
}

void MatryoshkaBom_Serialize(void)
{
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(MatryoshkaBom, VAR_ENUM, size);
}

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectMSBomb *MSBomb;

void MSBomb_Update(void)
{
    RSDK_THIS(MSBomb);
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void MSBomb_LateUpdate(void) {}

void MSBomb_StaticUpdate(void) {}

void MSBomb_Draw(void)
{
    RSDK_THIS(MSBomb);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MSBomb_Create(void *data)
{
    RSDK_THIS(MSBomb);

    entity->active    = ACTIVE_NORMAL;
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (data) {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 1, &entity->animator, true, 0);
        entity->hitbox.left   = 4;
        entity->hitbox.top    = 4;
        entity->hitbox.right  = -4;
        entity->hitbox.bottom = -4;
        entity->state         = MSBomb_Unknown4;
    }
    else {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 0, &entity->animator, true, 0);
        entity->hitbox.left   = 8;
        entity->hitbox.top    = 8;
        entity->hitbox.right  = -8;
        entity->hitbox.bottom = -8;
        entity->drawFX        = FX_SCALE;
        entity->scale.x       = 0xC0;
        entity->scale.y       = 0xC0;
        entity->state         = MSBomb_Unknown1;
    }
}

void MSBomb_StageLoad(void)
{
    MSBomb->aniFrames    = RSDK.LoadSpriteAnimation("SSZ2/MSBomb.bin", SCOPE_STAGE);
    MSBomb->sfxExplosion = RSDK.GetSFX("Stage/Explosion4.wav");
}

void MSBomb_Unknown1(void)
{
    RSDK_THIS(MSBomb);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->scale.x >= 0x200) {
        entity->drawFX = FX_NONE;
        entity->state  = MSBomb_Unknown3;
    }
    else {
        entity->scale.x += 0x40;
        entity->scale.y = entity->scale.x;
    }
}

void MSBomb_Unknown2(void)
{
    RSDK_THIS(MSBomb);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->scale.x >= 0x200) {
        int yVel = 0x18000;
        int xVel = 0x30000;

        foreach_active(MetalSonic, metal)
        {
            xVel = maxVal(abs(entity->position.x - metal->position.x) >> 6, 0x20000);
            yVel = abs(entity->position.y - metal->position.y) >> 6;
        }

        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, intToVoid(true), entity->position.x, entity->position.y);
        yVel               = -yVel;
        bomb->velocity.x   = -xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), entity->position.x, entity->position.y);
        bomb->velocity.x   = xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), entity->position.x, entity->position.y);
        yVel               = -(yVel >> 1);
        bomb->velocity.x   = -xVel;
        bomb->velocity.y   = yVel;

        bomb = CREATE_ENTITY(MSBomb, intToVoid(true), entity->position.x, entity->position.y);
        bomb->velocity.x   = xVel;
        bomb->velocity.y   = yVel;

        RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
        RSDK.ResetEntityPtr(entity, Explosion->objectID, intToVoid(EXPLOSION_BOSS));
        entity->position.x = bomb->position.x;
        entity->position.y = bomb->position.y;
    }
    else {
        entity->scale.x += 0x40;
        entity->scale.y = entity->scale.x;
    }
}

void MSBomb_Unknown3(void)
{
    RSDK_THIS(MSBomb);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xA0000, true)) {
        entity->velocity.y = -0x40000;
        if (entity->timer > 0) {
            entity->timer--;
            if (!entity->timer == 1) {
                int storeX = entity->position.x;
                int storeY = entity->position.y;
                RSDK.ResetEntityPtr(entity, Explosion->objectID, intToVoid(EXPLOSION_BOSS));
                entity->position.x = storeX;
                entity->position.y = storeY;
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
            }
        }
    }
    else if (!entity->timer && !RSDK.CheckOnScreen(entity, NULL)) {
        destroyEntity(entity);
    }

    if (entity->objectID == MSBomb->objectID) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                if (Player_CheckHit(player, entity))
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y);
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
                destroyEntity(entity);
            }
        }
    }
}

void MSBomb_Unknown4(void)
{
    RSDK_THIS(MSBomb);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            Player_CheckHit(player, entity);
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->hitboxPtr, entity, &entity->hitbox)) {
            MetalSonic->field_8 = 16;
            if (--metal->field_70 <= 0) {
                metal->timer = 0;
                // metal->state = MetalSonic_Unknown29;
            }
            RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);
            destroyEntity(entity);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void MSBomb_EditorDraw(void) {}

void MSBomb_EditorLoad(void) {}
#endif

void MSBomb_Serialize(void) {}
#endif

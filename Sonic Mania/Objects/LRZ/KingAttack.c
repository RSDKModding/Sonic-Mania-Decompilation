#include "SonicMania.h"

ObjectKingAttack *KingAttack;

void KingAttack_Update(void)
{
    RSDK_THIS(KingAttack);
    StateMachine_Run(entity->state);
}

void KingAttack_LateUpdate(void) {}

void KingAttack_StaticUpdate(void) {}

void KingAttack_Draw(void)
{
    RSDK_THIS(KingAttack);

    int x = RSDK_screens->position.x << 16;
    int y = RSDK_screens->position.y << 16;

    switch (entity->type) {
        case 0:
            for (int i = 0; i < 8; i += 4) {
                Vector2 vertices[4];
                colour colours[4];

                vertices[0].x = entity->laserVertPostions[i].x - x;
                vertices[0].y = entity->laserVertPostions[i].y - y;
                colours[0]    = entity->laserColours[i];

                vertices[1].x = entity->laserVertPostions[i + 1].x - x;
                vertices[1].y = entity->laserVertPostions[i + 1].y - y;
                colours[1]    = entity->laserColours[i + 1];

                vertices[2].x = entity->laserVertPostions[i + 3].x - x;
                vertices[2].y = entity->laserVertPostions[i + 3].y - y;
                colours[2]    = entity->laserColours[i + 3];

                vertices[3].x = entity->laserVertPostions[i + 2].x - x;
                vertices[3].y = entity->laserVertPostions[i + 2].y - y;
                colours[3]    = entity->laserColours[i + 2];

                RSDK.DrawBlendedQuad(vertices, colours, 4, entity->alpha, INK_ADD);
            }
            //[Fallthrough]
        default: RSDK.DrawSprite(&entity->animator, NULL, false); break;
        case 5:
            RSDK.DrawLine(entity->position.x, entity->position.y, entity->lineEndPos.x, entity->lineEndPos.y, 33008u, entity->alpha, INK_ADD, false);
            break;
    }
}

void KingAttack_Create(void *data)
{
    RSDK_THIS(KingAttack);

    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->type          = voidToInt(data);

        switch (entity->type) {
            case 0:
                entity->inkEffect    = INK_ADD;
                entity->visible      = true;
                entity->laserColours = KingAttack->laserColours;
                entity->state        = KingAttack_State0_Unknown;
                KingAttack->flag     = false;
                RSDK.PlaySfx(KingAttack->sfxLaserSweep, false, 255);
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 21, &entity->animator, true, 0);
                break;
            case 1:
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 23, &entity->animator, true, 0);
                entity->state         = KingAttack_State1_Unknown1;
                entity->drawFX        = FX_FLIP;
                entity->hitbox.left   = -16;
                entity->hitbox.top    = -64;
                entity->hitbox.right  = 16;
                entity->hitbox.bottom = 0;
                break;
            case 2:
                entity->drawFX  = FX_SCALE;
                entity->visible = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &entity->animator, true, RSDK.Rand(0, 6));
                entity->state         = KingAttack_State2_Unknown1;
                entity->hitbox.left   = -10;
                entity->hitbox.top    = -10;
                entity->hitbox.right  = 10;
                entity->hitbox.bottom = 10;
                break;
            case 3:
                entity->drawOrder = Zone->drawOrderLow;
                entity->inkEffect = INK_ADD;
                entity->visible   = true;
                entity->alpha     = 0xC0;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 24, &entity->animator, true, 0);
                entity->state = KingAttack_State3_Unknown1;
                break;
            case 4:
                entity->drawFX  = FX_SCALE;
                entity->visible = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &entity->animator, true, RSDK.Rand(0, 6));
                entity->scale.x = 0x100;
                entity->scale.y = 0x100;
                entity->state   = KingAttack_State4_Unknown1;
                break;
            case 5:
                entity->drawOrder = Zone->drawOrderLow;
                entity->visible   = true;
                entity->state     = KingAttack_State5_Unknown;
                entity->angle     = RSDK.Rand(0, 256);
                entity->timer     = 0x4000;
                entity->alpha     = 0x100;
                break;
            case 6:
                entity->drawOrder = Zone->drawOrderLow;
                entity->drawFX    = FX_ROTATE;
                entity->visible   = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 21, &entity->animator, true, 0);
                entity->state         = KingAttack_State6_Unknown1;
                entity->hitbox.left   = -4;
                entity->hitbox.top    = -4;
                entity->hitbox.right  = 4;
                entity->hitbox.bottom = 4;
                break;
            default: break;
        }
    }
}

void KingAttack_StageLoad(void)
{
    KingAttack->aniFrames     = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE);
    KingAttack->sfxLaserSweep = RSDK.GetSFX("LRZ/LaserSweep.wav");
    KingAttack->sfxLaserErupt = RSDK.GetSFX("LRZ/LaserErupt.wav");
    KingAttack->flag          = false;
    KingAttack->sfxElecPulse  = RSDK.GetSFX("Stage/ElecPulse.wav");
    KingAttack->sfxTwinShot   = RSDK.GetSFX("LRZ/TwinShot.wav");
    int id                    = Soundboard_LoadSFX("LRZ/ElecIdle.wav", 1u, KingAttack_ElecIdleCheckCB, 0);
    if (id >= 0)
        Soundboard->sfxUnknown8[id] = 30;
}

bool32 KingAttack_ElecIdleCheckCB(void)
{
    int count = 0;
    foreach_active(KingAttack, attack)
    {
        if (attack->type == 2)
            count++;
    }

    return count > 0;
}

void KingAttack_CheckPlayerCollisions(void)
{
    RSDK_THIS(KingAttack);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            Player_CheckHit(player, entity);
        }
    }
}

void KingAttack_HandleLaserPositions(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)entity->parent;

    int x = 0;
    if (king->direction)
        x = king->position.x - 0x240000;
    else
        x = king->position.x + 0x240000;
    int y = king->position.y - 0x300000;

    entity->laserVertPostions[0].x = x - 0x40000;
    entity->laserVertPostions[1].x = entity->position.x - 0x40000;
    entity->laserVertPostions[2].x = x - 0x20000;
    entity->laserVertPostions[3].x = entity->position.x - 0x20000;
    entity->laserVertPostions[4].x = x + 0x20000;
    entity->laserVertPostions[5].x = entity->position.x + 0x20000;
    entity->laserVertPostions[6].x = x + 0x40000;
    entity->laserVertPostions[7].x = entity->position.x + 0x40000;

    // entity->laserVertPostions[0].y = y;
    // entity->laserVertPostions[1].y = HeavyKing->field_6C;
    // entity->laserVertPostions[2].y = y;
    // entity->laserVertPostions[3].y = HeavyKing->field_6C;
    // entity->laserVertPostions[4].y = y;
    // entity->laserVertPostions[5].y = HeavyKing->field_6C;
    // entity->laserVertPostions[6].y = y;
    // entity->laserVertPostions[7].y = HeavyKing->field_6C;
}

void KingAttack_State0_Unknown(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    ++entity->timer;
    KingAttack_HandleLaserPositions();

    if (entity->timer < 16) {
        if (entity->alpha < 0xC0)
            entity->alpha += 0x10;
    }
    else if (entity->timer > 48) {
        if (entity->alpha > 0)
            entity->alpha -= 0x10;
    }

    if (!(entity->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(1), entity->position.x, entity->position.y);
        attack->direction        = (entity->timer >> 3) & 1;
    }
    if (entity->timer == 64)
        destroyEntity(entity);
}

void KingAttack_State1_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    if (++entity->timer == 30) {
        entity->timer   = 0;
        entity->visible = true;
        if (KingAttack->flag) {
            KingAttack->flag = false;
        }
        else {
            Camera_ShakeScreen(0, 0, 5);
            RSDK.PlaySfx(KingAttack->sfxLaserErupt, false, 255);
            KingAttack->flag = true;
        }
        entity->state = KingAttack_State1_Unknown2;
    }
}

void KingAttack_State1_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID > 8 && entity->animator.frameID < 16)
        KingAttack_CheckPlayerCollisions();

    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

void KingAttack_State2_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator);

    entity->angle = (entity->angle + 12) & 0x3FF;
    if (entity->scale.x < 512) {
        entity->scale.x += 0x20;
        entity->scale.y = entity->scale.x;
    }

    entity->velocity.x = entity->position.x;
    entity->velocity.y = entity->position.y;

    int x = 0;
    if (king->direction)
        x = RSDK.Cos1024(entity->angle) - 0x240;
    else
        x = RSDK.Cos1024(entity->angle) + 0x240;
    entity->position.x = (x << 12) + king->position.x;
    entity->position.y = (((RSDK.Sin1024(entity->angle) - 0x300) << 12) + king->position.y);
    entity->velocity.x = entity->position.x - entity->velocity.x;
    entity->velocity.y = entity->position.y - entity->velocity.y;
    KingAttack_CheckPlayerCollisions();
    if (++entity->timer == 120) {
        entity->timer        = 0;
        entity->lineEndPos.x = king->position.x;
        entity->lineEndPos.y = king->position.y;
        entity->state        = KingAttack_State2_Unknown2;
    }
}

void KingAttack_State2_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator);

    entity->position.x += king->position.x - entity->lineEndPos.x;
    entity->position.y += king->position.y - entity->lineEndPos.y;
    if (entity->position.x <= king->position.x) {
        if (entity->position.x < king->position.x)
            entity->velocity.x += 0x4000;
    }
    else {
        entity->velocity.x -= 0x4000;
    }

    if (entity->position.y <= king->position.y) {
        if (entity->position.y < king->position.y)
            entity->velocity.y += 0x3800;
    }
    else {
        entity->velocity.y -= 0x3800;
    }

    if (entity->velocity.x >= -0x50000) {
        if (entity->velocity.x > 0x50000)
            entity->velocity.x = 0x50000;
    }
    else {
        entity->velocity.x = -0x50000;
    }

    if (entity->velocity.y >= -0x50000) {
        if (entity->velocity.y > 0x50000)
            entity->velocity.y = 0x50000;
    }
    else {
        entity->velocity.y = -0x50000;
    }

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->lineEndPos.x = king->position.x;
    entity->lineEndPos.y = king->position.y;

    entity->angle = (entity->angle + 12) & 0x3FF;
    entity->scale.x += (0x180 - entity->scale.x - (RSDK.Sin1024(entity->angle) >> 3)) >> 3;
    entity->scale.y = entity->scale.x;
    KingAttack_CheckPlayerCollisions();

    if (entity->angle < 512)
        entity->drawOrder = Zone->drawOrderLow - 1;
    else
        entity->drawOrder = Zone->drawOrderLow;
}

void KingAttack_State_Unknown(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    ++entity->timer;
    if (!(entity->timer & 3))
        CREATE_ENTITY(KingAttack, intToVoid(3), entity->position.x, entity->position.y);

    if (entity->scale.x < 512) {
        entity->scale.x += 0x20;
        entity->scale.y = entity->scale.x;
    }

    entity->velocity.x += ((entity->field_70.x - entity->velocity.x) >> 3);
    entity->velocity.y += ((entity->field_70.y - entity->velocity.y) >> 3);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    KingAttack_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void KingAttack_State3_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    if (entity->timer++ >= 8) {
        entity->alpha -= 8;
        if (entity->alpha <= 0)
            destroyEntity(entity);
    }
}

void KingAttack_State4_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->scale.x < 0x400)
        entity->scale.x += 4;
    entity->scale.y = entity->scale.x;

    ++entity->timer;
    entity->position.x = king->position.x;
    entity->position.y = king->position.y;
    if (king->direction)
        entity->position.x -= 0x240000;
    else
        entity->position.x += 0x240000;
    entity->position.y -= 0x300000;

    if (!(entity->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(5), entity->position.x, entity->position.y);
        attack->parent           = (Entity *)entity;
    }
    if (entity->timer == 120) {
        entity->timer = 0;
        entity->state = KingAttack_State4_Unknown2;
    }
}

void KingAttack_State5_Unknown(void)
{
    RSDK_THIS(KingAttack);

    EntityKingAttack *parent = (EntityKingAttack *)entity->parent;

    entity->position.x = entity->timer * RSDK.Cos256(entity->angle);
    entity->position.y = entity->timer * RSDK.Sin256(entity->angle);

    entity->lineEndPos.x = entity->position.x - (entity->position.x >> 3);
    entity->lineEndPos.y = entity->position.y - (entity->position.y >> 3);
    entity->position.x   = entity->position.y + parent->position.x;
    entity->position.y   = entity->position.y + parent->position.y;
    entity->lineEndPos.x += parent->position.x;
    entity->lineEndPos.y += parent->position.y;

    entity->alpha -= 16;
    entity->timer -= 0x200;
    if (entity->timer < 0x400)
        destroyEntity(entity);
}

void KingAttack_State4_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator);

    entity->position.x = king->position.x;
    entity->position.y = king->position.y;

    if (king->direction)
        entity->position.x -= 0x240000;
    else
        entity->position.x += 0x240000;
    entity->position.y -= 0x300000;
    ++entity->timer;

    if (!(entity->timer & 6))
        RSDK.PlaySfx(KingAttack->sfxTwinShot, false, 255);

    if (!(entity->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(6), entity->position.x, entity->position.y);
        attack->velocity.x       = RSDK.Sin1024(entity->angle) << 9;
        attack->velocity.y       = RSDK.Cos1024(entity->angle) << 9;
        attack->rotation         = entity->angle >> 1;

        attack             = CREATE_ENTITY(KingAttack, intToVoid(6), entity->position.x, entity->position.y);
        attack->velocity.x = RSDK.Sin1024(entity->angle + 0x200) << 9;
        attack->velocity.y = RSDK.Cos1024(entity->angle + 0x200) << 9;
        attack->rotation   = entity->angle >> 1;
    }

    entity->angle += 4;
    if (entity->timer > 60) {
        entity->scale.x -= 4;
        entity->scale.y = entity->scale.x;
        if (!entity->scale.x)
            destroyEntity(entity);
    }
}

void KingAttack_State6_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    entity->rotation = (entity->rotation + 16) & 0x1FF;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    KingAttack_CheckPlayerCollisions();

    foreach_active(HPZEmerald, emerald)
    {
        if (emerald->type && RSDK.CheckObjectCollisionPlatform(emerald, emerald->hitbox, entity, &entity->hitbox, true)) {
            RSDK.SetSpriteAnimation(KingAttack->aniFrames, 22, &entity->animator, false, 0);
            entity->position.y += 0x40000;
            entity->state = KingAttack_State6_Unknown2;
            foreach_break;
        }
    }

    if (entity->state == KingAttack_State6_Unknown1) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
            RSDK.SetSpriteAnimation(KingAttack->aniFrames, 22, &entity->animator, false, 0);
            entity->state = KingAttack_State6_Unknown2;
            entity->position.y += 0x40000;
        }

        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
}

void KingAttack_State6_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void KingAttack_EditorDraw(void)
{
    RSDK_THIS(KingAttack);
    RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void KingAttack_EditorLoad(void) { KingAttack->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE); }
#endif

void KingAttack_Serialize(void) {}

#include "SonicMania.h"

ObjectBeanstalk *Beanstalk;

void Beanstalk_Update(void)
{
    RSDK_THIS(Beanstalk);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->type > 1)
        RSDK.ProcessAnimation(&entity->animator2);
    StateMachine_Run(entity->state);
}

void Beanstalk_LateUpdate(void) {}

void Beanstalk_StaticUpdate(void) {}

void Beanstalk_Draw(void)
{
    RSDK_THIS(Beanstalk);

    if (entity->shown) {
        Beanstalk_Unknown4();
        Beanstalk_Unknown5();

        entity->scale.x = entity->field_78;
        entity->scale.y = entity->field_78;
        entity->drawFX  = FX_SCALE | FX_FLIP;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        if (entity->type > 1)
            RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->drawFX  = FX_FLIP;
        entity->scale.x = 0x200;
        entity->scale.y = 0x200;
    }
}

void Beanstalk_Create(void *data)
{
    RSDK_THIS(Beanstalk);

    entity->active       = ACTIVE_BOUNDS;
    entity->visible      = true;
    entity->drawFX       = FX_FLIP;
    entity->drawOrder    = Zone->drawOrderLow;
    entity->bezCtrlAngle = entity->bezCtrlAngle & 0xFF;

    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    entity->field_74      = -1;
    if (entity->type == 0) {
        entity->field_78 = 512;
        entity->active   = ACTIVE_NORMAL;
    }
    else {
        entity->field_78 = 0;
    }
    entity->field_8C = entity->position;
    entity->field_94 = entity->position;
    entity->field_9C = 0;
    entity->timer    = 0;
    entity->field_A0 = 0;
    entity->state    = Beanstalk_State_Setup;
}

void Beanstalk_StageLoad(void)
{
    Beanstalk->aniFrames             = RSDK.LoadSpriteAnimation("SSZ1/Beanstalk.bin", SCOPE_STAGE);
    Beanstalk->sfxImpact             = RSDK.GetSFX("Stage/Impact6.wav");
    Beanstalk->sfxBeanNode           = RSDK.GetSFX("SSZ1/BeanNode.wav");
    Beanstalk->sfxBeanChomp          = RSDK.GetSFX("SSZ1/BeanChomp.wav");
    Beanstalk->hitboxSeed.left       = -1;
    Beanstalk->hitboxSeed.top        = -64;
    Beanstalk->hitboxSeed.right      = 1;
    Beanstalk->hitboxSeed.bottom     = 1;
    Beanstalk->hitboxPlatform.left   = 0;
    Beanstalk->hitboxPlatform.top    = -3;
    Beanstalk->hitboxPlatform.right  = 46;
    Beanstalk->hitboxPlatform.bottom = 13;
    Beanstalk->hitboxPlant.left      = 8;
    Beanstalk->hitboxPlant.top       = -8;
    Beanstalk->hitboxPlant.right     = 44;
    Beanstalk->hitboxPlant.bottom    = 8;
}

int32 Beanstalk_Unknown1(void)
{
    RSDK_THIS(Beanstalk);
    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) + 1, Beanstalk);

    if (entity->forceEnd || next->objectID != Beanstalk->objectID)
        return 0;

    int thisX = ((entity->bezCtrlLength * RSDK.Cos256(entity->bezCtrlAngle)) << 9) + entity->position.x;
    int thisY = ((entity->bezCtrlLength * RSDK.Sin256(entity->bezCtrlAngle)) << 9) + entity->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return MathHelpers_Unknown7(entity->position.x, entity->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y) / 0xA0000;
}

int32 Beanstalk_Unknown2(void)
{
    RSDK_THIS(Beanstalk);
    int len = 0;

    while (entity->type) {
        EntityBeanstalk *prev = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) - 1, Beanstalk);
        if (prev->objectID != Beanstalk->objectID)
            break;

        int32 prevX = ((prev->bezCtrlLength * RSDK.Cos256(prev->bezCtrlAngle)) << 9) + prev->position.x;
        int32 prevY = ((prev->bezCtrlLength * RSDK.Sin256(prev->bezCtrlAngle)) << 9) + prev->position.y;

        uint8 angle = (entity->bezCtrlAngle + 0x80);
        int32 thisX = ((entity->bezCtrlLength * RSDK.Cos256(angle)) << 9) + entity->position.x;
        int32 thisY = ((entity->bezCtrlLength * RSDK.Sin256(angle)) << 9) + entity->position.y;
        len += MathHelpers_Unknown7(prev->position.x, prev->position.y, prevX, prevY, thisX, thisY, entity->position.x, entity->position.y);
        entity = prev;
    }
    return len;
}

int32 Beanstalk_Unknown3(void)
{
    RSDK_THIS(Beanstalk);
    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) + 1, Beanstalk);

    if (entity->forceEnd || next->objectID != Beanstalk->objectID)
        return 0;

    int32 thisX = ((entity->bezCtrlLength * RSDK.Cos256(entity->bezCtrlAngle)) << 9) + entity->position.x;
    int32 thisY = ((entity->bezCtrlLength * RSDK.Sin256(entity->bezCtrlAngle)) << 9) + entity->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int32 nextX = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int32 nextY = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return 0x10000
           / (MathHelpers_Unknown7(entity->position.x, entity->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y) / 0x32000);
}

void Beanstalk_Unknown4(void)
{
    RSDK_THIS(Beanstalk);

    if (!entity->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) + 1, Beanstalk);
        if (next->objectID == Beanstalk->objectID) {
            if (RSDK_sceneInfo->inEditor) {
                entity->field_74 = Beanstalk_Unknown1();
                entity->field_80 = 0x10000;
            }
            if (entity->field_74 == 0xFFFF)
                entity->field_74 = Beanstalk_Unknown1();

            if (entity->field_74) {
                if (entity->field_88 == 0 || RSDK_sceneInfo->inEditor) {
                    int val          = Beanstalk_Unknown2();
                    entity->field_88 = 1;
                    entity->field_85 = (val / 0xA0000 / 3) % 9;
                    entity->field_84 = 3 * ((val / 0xA0000 / 3) + 1) - val / 0xA0000;
                }

                int thisX   = ((entity->bezCtrlLength * RSDK.Cos256(entity->bezCtrlAngle)) << 9) + entity->position.x;
                int thisY   = ((entity->bezCtrlLength * RSDK.Sin256(entity->bezCtrlAngle)) << 9) + entity->position.y;
                uint8 angle = (next->bezCtrlAngle + 0x80);
                int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
                int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;

                int inc = 0x10000 / entity->field_74;
                int pos = (0x10000 / entity->field_74) >> 1;

                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &entity->animator3, true, 0);
                for (int i = pos; i <= 0x10000; i += inc) {
                    if (i >= entity->field_80)
                        break;
                    Vector2 drawPos = MathHelpers_Unknown5(i, entity->position.x, entity->position.y, thisX, thisY, nextX, nextY, next->position.x,
                                                           next->position.y);
                    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                }

                int val = entity->field_84;
                for (int i = entity->field_85; pos <= 0x10000; ++val) {
                    if (pos >= entity->field_80)
                        break;
                    Vector2 drawPos = MathHelpers_Unknown5(pos, entity->position.x, entity->position.y, thisX, thisY, nextX, nextY, next->position.x,
                                                           next->position.y);
                    if (!(val % 3)) {
                        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, i + 5, &entity->animator3, true, 0);
                        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                        i = ((i + 1) % 9);
                    }
                    pos += inc;
                }
            }
        }
    }
}

void Beanstalk_Unknown5(void)
{
    RSDK_THIS(Beanstalk);

    if (!entity->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) + 1, Beanstalk);
        if (next->objectID == Beanstalk->objectID) {
            if (entity->field_74) {
                uint8 angle = next->bezCtrlAngle + 0x80;
                int thisX   = ((entity->bezCtrlLength * RSDK.Cos256(entity->bezCtrlAngle)) << 9) + entity->position.x;
                int thisY   = ((entity->bezCtrlLength * RSDK.Sin256(entity->bezCtrlAngle)) << 9) + entity->position.y;
                int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
                int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;

                Vector2 drawPos = MathHelpers_Unknown5(entity->field_80, entity->position.x, entity->position.y, thisX, thisY, nextX, nextY,
                                                       next->position.x, next->position.y);
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &entity->animator3, true, 0);
                RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            }
        }
    }
}

void Beanstalk_Unknown6(void)
{
    RSDK_THIS(Beanstalk);

    if (!entity->flag2) {
        EntityBeanstalk *node = NULL;
        if (!entity->forceEnd) {
            node = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) + 1, Beanstalk);
            if (node->objectID != Beanstalk->objectID)
                node = NULL;
        }

        if (entity->forceEnd || !node)
            entity->field_80 = 0x10000;

        if (entity->field_80 < 0x10000) {
            if (!entity->field_7C)
                entity->field_7C = Beanstalk_Unknown3();
            entity->field_80 += entity->field_7C;
        }
        if (entity->field_80 > 0x10000)
            entity->field_80 = 0x10000;

        if (entity->field_80 == 0x10000) {
            entity->flag2  = true;
            entity->active = ACTIVE_BOUNDS;
            if (node) {
                node->shown  = true;
                node->active = ACTIVE_NORMAL;
            }
        }
    }
}

void Beanstalk_Unknown7(void)
{
    RSDK_THIS(Beanstalk);

    entity->field_78 = (entity->timer << 9) / 20;
    if (entity->timer < 20) {
        if (!entity->timer) {
            if (RSDK.CheckOnScreen(entity, NULL))
                RSDK.PlaySfx(Beanstalk->sfxBeanNode, false, 255);
        }
        ++entity->timer;
    }
}

void Beanstalk_CheckPlayerCollisions_Leaf(void)
{
    RSDK_THIS(Beanstalk);

    int storeX       = entity->position.x;
    int storeY       = entity->position.y;
    entity->position = entity->field_94;
    entity->position.y &= 0xFFFF0000;
    entity->position.x = ((RSDK.Sin512(2 * entity->angle2) << 9) + entity->position.x) & 0xFFFF0000;

    if (entity->activePlayers == 0) {
        if (entity->field_9C > 0)
            entity->field_9C -= 4;
    }
    else {
        if (entity->field_9C < 64)
            entity->field_9C += 4;
    }

    entity->activePlayers = 0;
    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (Player_CheckCollisionPlatform(player, entity, &Beanstalk->hitboxPlatform)) {
            player->position.x += entity->position.x - storeX;

            if (entity->position.y - storeY > 0)
                player->position.y += entity->position.y - storeY;
            player->position.y += 0x10000;
            entity->activePlayers |= 1 << playerID;
            player->position.y &= 0xFFFF0000;
        }
    }

    entity->field_94.x = entity->field_8C.x;
    entity->field_94.y = entity->field_8C.y;
    entity->field_94.y += RSDK.Sin256(entity->field_9C) << 10;
}

void Beanstalk_CheckPlayerCollisions_Plant(void)
{
    RSDK_THIS(Beanstalk);

    int mult = 0;
    if (entity->field_A0 >= 15) {
        if (entity->field_A0 >= 60) {
            entity->field_A0 = 0;
        }
        else {
            mult = 0x15540 - 0x5B0 * entity->field_A0;
            entity->field_A0++;
        }
    }
    else {
        mult = 0x1111 * entity->field_A0;
        entity->field_A0++;
    }

    entity->position = entity->field_8C;
    if (entity->direction == FLIP_X)
        entity->position.x += mult * -16;
    else
        entity->position.x += mult * 16;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Beanstalk->hitboxPlant)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                entity->state = Beanstalk_State1_Unknown;
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &entity->animator2, true, 0);
            }
            else if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer)) {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}

void Beanstalk_State_Setup(void)
{
    RSDK_THIS(Beanstalk);

    switch (entity->type) {
        case 0:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &entity->animator1, true, 9);
            entity->state = Beanstalk_State_Unknown1;
            break;
        case 1:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &entity->animator1, true, 0);
            entity->state = Beanstalk_State1_Unknown;
            break;
        case 2:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 2, &entity->animator2, true, 0);
            entity->state = Beanstalk_State2_Unknown;
            break;
        case 3:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 3, &entity->animator2, true, 0);
            entity->state = Beanstalk_State3_Unknown;
            break;
        default: break;
    }
}

void Beanstalk_State_Unknown1(void)
{
    RSDK_THIS(Beanstalk);
    if (entity->startGrowth) {
        entity->state = Beanstalk_State_Unknown2;
        entity->timer = 15;
    }
}

void Beanstalk_State_Unknown2(void)
{
    RSDK_THIS(Beanstalk);

    if (entity->timer <= 0) {
        RSDK.PlaySfx(Beanstalk->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 5);
        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &entity->animator1, true, 0);
        entity->shown  = true;
        entity->active = ACTIVE_NORMAL;
        entity->state  = Beanstalk_State_Unknown3;
    }
    else {
        entity->timer--;
    }
}

void Beanstalk_State_Unknown3(void) { Beanstalk_Unknown6(); }

void Beanstalk_State1_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (entity->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        entity->position   = entity->field_8C;
        entity->position.x = ((RSDK.Sin512(2 * entity->angle2) << 9) + entity->position.x) & 0xFFFF0000;
        entity->position.y &= 0xFFFF0000;
        ++entity->angle2;
    }
}

void Beanstalk_State2_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (entity->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        Beanstalk_CheckPlayerCollisions_Leaf();
        ++entity->angle2;
    }
}

void Beanstalk_State3_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (entity->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        Beanstalk_CheckPlayerCollisions_Plant();
        if (RSDK.CheckOnScreen(entity, NULL)) {
            if (entity->animator2.frameID == 5 && entity->animator2.animationTimer == 1)
                RSDK.PlaySfx(Beanstalk->sfxBeanChomp, false, 255);
        }
    }
}

void Beanstalk_EditorDraw(void) {}

void Beanstalk_EditorLoad(void) {}

void Beanstalk_Serialize(void)
{
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlAngle);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlLength);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_BOOL, forceEnd);
}

#include "SonicMania.h"

ObjectTVVan *TVVan;

void TVVan_Update(void)
{
    RSDK_THIS(TVVan);
    StateMachine_Run(entity->state);
}

void TVVan_LateUpdate(void)
{

}

void TVVan_StaticUpdate(void)
{

}

void TVVan_Draw(void)
{
    RSDK_THIS(TVVan);
    StateMachine_Run(entity->stateDraw[RSDK_sceneInfo->currentDrawGroup]);
}

void TVVan_Create(void* data)
{
    RSDK_THIS(TVVan);

    if (!RSDK_sceneInfo->inEditor) {
        if (data)
            entity->type = voidToInt(data);

        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawFX        = FX_FLIP;
        switch (entity->type) {
            case TVVAN_SATELLITE_NE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw2_High;
                entity->field_C4[0]                        = 0xE0;
                entity->field_C4[1]                        = 0xE0;
                entity->field_CC[0]                        = 0x60;
                entity->field_CC[1]                        = 0x60;
                break;
            case TVVAN_SATELLITE_SE_NW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw3_High;
                entity->field_C4[0]                        = 0x20;
                entity->field_C4[1]                        = 0x20;
                entity->field_CC[0]                        = 0xA0;
                entity->field_CC[1]                        = 0xA0;
                break;
            case TVVAN_SATELLITE_NW_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw4_High;
                entity->field_C4[0]                        = 0x20;
                entity->field_C4[1]                        = 0x20;
                entity->field_CC[0]                        = 0xE0;
                entity->field_CC[1]                        = 0xE0;
                break;
            case TVVAN_SATELLITE_NE_SE:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw5_High;
                entity->field_C4[0]                        = 0x60;
                entity->field_C4[1]                        = 0x60;
                entity->field_CC[0]                        = 0xA0;
                entity->field_CC[1]                        = 0xA0;
                break;
            case TVVAN_SATELLITE_NE_NW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw6_High;
                entity->field_C4[0]                        = 0x20;
                entity->field_C4[1]                        = 0x20;
                entity->field_CC[0]                        = 0x60;
                entity->field_CC[1]                        = 0x60;
                break;
            case TVVAN_SATELLITE_SE_SW:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw7_High;
                entity->field_C4[0]                        = 0xE0;
                entity->field_C4[1]                        = 0xE0;
                entity->field_CC[0]                        = 0xA0;
                entity->field_CC[1]                        = 0xA0;
                break;
            case TVVAN_SATELLITE_SW_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw8_High;
                entity->field_C4[0]                        = 0xA0;
                entity->field_C4[1]                        = 0xA0;
                entity->field_CC[0]                        = 0x00;
                entity->field_CC[1]                        = 0x00;
                break;
            case TVVAN_SATELLITE_NW_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw9_High;
                entity->field_C4[0]                        = 0x20;
                entity->field_C4[1]                        = 0x20;
                entity->field_CC[0]                        = 0xC0;
                entity->field_CC[1]                        = 0xC0;
                break;
            case TVVAN_SATELLITE_SE_N:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw10_High;
                entity->field_C4[0]                        = 0xA0;
                entity->field_C4[1]                        = 0xA0;
                entity->field_CC[0]                        = 0x40;
                entity->field_CC[1]                        = 0x40;
                break;
            case TVVAN_SATELLITE_NE_S:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw11_High;
                entity->field_C4[0]                        = 0xC0;
                entity->field_C4[1]                        = 0xC0;
                entity->field_CC[0]                        = 0x40;
                entity->field_CC[1]                        = 0x40;
                break;
            case TVVAN_EXITTV:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &entity->animator7, true, 9);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &entity->animator6, true, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &entity->animator9, true, 0);
                entity->drawOrder                    = Zone->drawOrderLow;
                entity->stateDraw[Zone->drawOrderLow] = TVVan_StateDraw14_High;
                entity->updateRange.x                = 0x1000000;
                entity->updateRange.y                = 0x1000000;
                entity->state                             = TVVan_State14_Unknown1;
                break;
            case TVVAN_15:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &entity->animator5, true, 0);
                entity->active                        = ACTIVE_NORMAL;
                entity->visible                       = 0;
                entity->drawOrder                     = Zone->drawOrderHigh;
                entity->state                              = TVVan_State15_Unknown1;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw15_High;
                break;
            default:
                entity->drawOrder = Zone->drawOrderLow;
                entity->state          = TVVan_State_Unknown1;
                if (entity->type == TVVAN_VAN_RIGHT)
                    entity->direction = FLIP_X;
                entity->drawPos                            = entity->position;
                entity->drawPos2                           = entity->position;
                entity->stateDraw[Zone->drawOrderLow]  = TVVan_StateDraw_Low;
                entity->stateDraw[Zone->drawOrderHigh] = TVVan_StateDraw_High;
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &entity->animator1, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &entity->animator2, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &entity->animator3, false, 0);
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &entity->animator4, false, 0);
                break;
        }
    }
}

void TVVan_StageLoad(void)
{
    TVVan->aniFrames          = RSDK.LoadSpriteAnimation("SPZ1/TVVan.bin", SCOPE_STAGE);
    TVVan->hitbox3.left   = -40;
    TVVan->hitbox3.right  = 28;
    TVVan->hitbox3.top    = -28;
    TVVan->hitbox3.bottom = -20;
    TVVan->hitbox1.left   = -56;
    TVVan->hitbox1.right  = 24;
    TVVan->hitbox1.top    = 22;
    TVVan->hitbox1.bottom = 32;
    TVVan->hitbox2.left   = 20;
    TVVan->hitbox2.right  = 28;
    TVVan->hitbox2.top    = -20;
    TVVan->hitbox2.bottom = 32;
    TVVan->hitbox4.left   = 12;
    TVVan->hitbox4.right  = 20;
    TVVan->hitbox4.top    = -16;
    TVVan->hitbox4.bottom = 24;
    TVVan->hitbox5.left   = -24;
    TVVan->hitbox5.right  = 24;
    TVVan->hitbox5.top    = -24;
    TVVan->hitbox5.bottom = 24;
    TVVan->sfxDoor        = RSDK.GetSFX("Stage/Door.wav");
    TVVan->sfxSatellite1  = RSDK.GetSFX("Stage/Satellite1.wav");
    TVVan->sfxSatellite2  = RSDK.GetSFX("Stage/Satellite2.wav");
    TVVan->sfxExplosion   = RSDK.GetSFX("Stage/Explosion3.wav");
}

void TVVan_Unknown1(void)
{
    RSDK_THIS(TVVan);

    if (entity->field_B8 != 1) {
        if (entity->field_B4 > 0)
            entity->field_B4 -= 4;

        if (entity->angle <= 0) {
            if (entity->angle < 0)
                entity->angle++;
        }
        else {
            entity->angle--;
        }
    }
    else if (entity->field_B4 < 64)
        entity->field_B4 += 4;

    entity->angle = clampVal(entity->angle + entity->field_BC, -8, 8);

    entity->position.x = entity->drawPos2.x;
    entity->position.y = entity->drawPos2.y;
    entity->drawPos2.x      = entity->drawPos.x;
    entity->drawPos2.y      = entity->drawPos.y;
    entity->drawPos2.y      = (RSDK.Sin256(entity->field_B4) + entity->drawPos2.y) & 0xFFFF0000;
    entity->field_B0        = entity->drawPos2.y - entity->position.y;
}

void TVVan_StateDraw_Low(void)
{
    RSDK_THIS(TVVan);

    entity->drawFX       = FX_ROTATE | FX_FLIP;
    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, 0, false);

    entity->drawFX       = FX_FLIP;
    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    entity->drawFX |= FX_ROTATE;
    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, 0, false);

    entity->animator1.frameID = 8;
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&entity->animator5, 0, false);

    entity->drawFX       = FX_FLIP;
    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    entity->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&entity->animator2, 0, false);
    RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK_sceneInfo->entitySlot);
}

void TVVan_StateDraw_High(void)
{
    RSDK_THIS(TVVan);

    entity->drawFX       = FX_FLIP;
    entity->animator1.frameID = 5;
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    entity->animator1.frameID = 6;
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    entity->animator1.frameID = 7;
    RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);

    entity->drawFX |= FX_ROTATE;
    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
}

void TVVan_StateDraw2_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw3_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw4_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw5_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw6_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw7_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw8_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 1;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_XY;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw9_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 1;
    entity->direction    = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw10_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 1;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw11_High(void)
{
    RSDK_THIS(TVVan);

    entity->animator1.frameID = 1;
    entity->direction    = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 0;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->direction    = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void TVVan_StateDraw15_High(void)
{
    RSDK_THIS(TVVan);

    RSDK.DrawSprite(&entity->animator5, NULL, false);
}

void TVVan_StateDraw14_High(void)
{
    RSDK_THIS(TVVan);
    Vector2 drawPos;

    RSDK.DrawSprite(&entity->animator7, NULL, false);
    RSDK.DrawSprite(&entity->animator6, NULL, false);
}

void TVVan_StateDraw14_Low(void)
{
    RSDK_THIS(TVVan);
    Vector2 drawPos;

    RSDK.DrawSprite(&entity->animator7, NULL, false);
    RSDK.DrawSprite(&entity->animator6, NULL, false);

    entity->drawFX = FX_SCALE;
    drawPos.x           = entity->position.x;
    drawPos.y           = entity->position.y - 0x40000;
    RSDK.DrawSprite(&entity->animator8, &drawPos, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator9, NULL, false);
}

void TVVan_State_Unknown1(void)
{
    RSDK_THIS(TVVan);

    TVVan_Unknown1();
    entity->field_B8 = 0;
    entity->field_BC = 0;
    bool32 flag      = false;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &TVVan->hitbox3) == C_TOP) {
            player->position.y += entity->field_B0;
            player->position.y &= 0xFFFF0000;
            entity->field_B8           = 1;
            
            int dist = player->position.x - entity->position.x;
            if (entity->direction == FLIP_NONE)
                dist += 0x100000;
            entity->field_BC += dist >> 20;
        }
        Player_CheckCollisionBox(player, entity, &TVVan->hitbox2);

        int dist = 0;
        if (entity->direction)
            dist = player->position.x - entity->position.x;
        else
            dist = entity->position.x - player->position.x;
        TVVan->hitbox1.top = dist >> 17;

        if (TVVan->hitbox1.top >= 22) {
            if (TVVan->hitbox1.top > 32)
                TVVan->hitbox1.top = 32;
        }
        else {
            TVVan->hitbox1.top = 22;
        }

        if (Player_CheckCollisionPlatform(player, entity, &TVVan->hitbox1) == C_TOP) {
            player->position.y += entity->field_B0;
            player->position.y &= 0xFFFF0000;
            if (TVVan->hitbox1.top == 22)
                entity->field_B8 = 1;
        }
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &TVVan->hitbox4)) {
                player->state           = Player_State_None;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->tileCollisions  = false;
                player->interaction     = false;
                player->visible         = false;
                player->blinkTimer      = 0;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 120;
                flag                                    = true;
                entity->player                        = player;
                foreach_break;
            }
        }
    }

    entity->rotation = entity->angle >> 2;
    entity->position.y = entity->drawPos2.y - abs(entity->rotation << 15);
    if (flag) {
        entity->state = TVVan_State_Unknown2;
        RSDK.PlaySfx(TVVan->sfxSatellite1, false, 255);
        entity->animator2.animationSpeed = 1;
        entity->animator3.animationSpeed = 1;
        entity->animator4.animationSpeed = 1;
    }
    RSDK.ProcessAnimation(&entity->animator5);
}

void TVVan_CheckPlayerCollisions(void)
{
    RSDK_THIS(TVVan);

    foreach_active(Player, player) {
        Player_CheckCollisionBox(player, entity, &TVVan->hitbox3);
        Player_CheckCollisionBox(player, entity, &TVVan->hitbox2);
        entity->direction ^= FLIP_X;
        Player_CheckCollisionBox(player, entity, &TVVan->hitbox2);
        entity->direction ^= FLIP_X;
        Player_CheckCollisionPlatform(player, entity, &TVVan->hitbox1);
    }
}

void TVVan_State_Unknown2(void)
{
    RSDK_THIS(TVVan);

    TVVan_Unknown1();
    TVVan_CheckPlayerCollisions();
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);

    if (entity->animator4.frameID >= entity->animator4.frameCount - 1) {
        entity->state = TVVan_State_Unknown3;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 3, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 4, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 7, &entity->animator5, true, 0);

        if (entity->direction) {
            EntityTVVan *van = CREATE_ENTITY(TVVan, intToVoid(TVVAN_15), entity->position.x + 0x80000, entity->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = entity->player;
            van->angle       = 0xA0;
            van->nextNode    = (Entity *)entity;
        }
        else {
            EntityTVVan *van = CREATE_ENTITY(TVVan, intToVoid(TVVAN_15), entity->position.x - 0x80000, entity->position.y - 0x420000);
            van->isPermanent = true;
            van->player      = entity->player;
            van->angle       = 0xE0;
            van->nextNode    = (Entity *)entity;
        }
        entity->player   = NULL;
    }
}

void TVVan_State_Unknown3(void)
{
    RSDK_THIS(TVVan);

    TVVan_Unknown1();
    TVVan_CheckPlayerCollisions();
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator5);

    if (entity->animator2.frameID >= entity->animator2.frameCount - 1) {
        entity->state = TVVan_State_Unknown1;
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &entity->animator3, true, 0);
    }
}

void TVVan_State15_Unknown1(void)
{
    RSDK_THIS(TVVan);

    if (++entity->timer == 20) {
        entity->timer = 0;
        entity->state = TVVan_State15_Unknown2;
    }
}

void TVVan_State15_Unknown2(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = entity->player;

    entity->position.x += 0xC00 * RSDK.Cos256(entity->angle);
    entity->position.y += 0xC00 * RSDK.Sin256(entity->angle);
    
    EntityDebris *debris        = CREATE_ENTITY(Debris, Debris_State_Move, entity->position.x, entity->position.y);
    debris->timer = 16;
    debris->drawOrder = entity->drawOrder;
    debris->drawFX    = FX_ROTATE;
    debris->inkEffect = INK_ALPHA;
    debris->alpha     = (RSDK.Sin256(32 * Zone->timer) >> 3) + 192;
    debris->rotation  = 2 * entity->angle;
    RSDK.SetSpriteAnimation(TVVan->aniFrames, 13, &debris->animator, true, entity->timer >> 1);

    entity->timer = (entity->timer + 1) % 22;
    player->position.x += (entity->position.x - player->position.x) >> 2;
    player->position.y += (entity->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        foreach_active(TVVan, van)
        {
            if (entity != van && entity->nextNode != (Entity *)van) {
                if (van->type && van->type < TVVAN_15) {
                    int rx = (van->position.x - entity->position.x) >> 16;
                    int ry = (van->position.y - entity->position.y) >> 16;
                    if (rx * rx + ry * ry < 0x800) {
                        entity->position.x = van->position.x;
                        entity->position.y = van->position.y;
                        entity->nextNode   = (Entity *)van;
                        entity->visible    = true;
                        if (van->type == TVVAN_EXITTV) {
                            RSDK.SetSpriteAnimation(TVVan->aniFrames, 14, &entity->animator5, true, 0);
                            RSDK.SetSpriteAnimation(TVVan->aniFrames, 16, &van->animator6, true, 0);
                            switch (player->characterID) {
                                case ID_TAILS: RSDK.SetSpriteAnimation(TVVan->aniFrames, 20, &van->animator8, true, 0); break;
                                case ID_KNUCKLES: RSDK.SetSpriteAnimation(TVVan->aniFrames, 21, &van->animator8, true, 0); break;
#if RETRO_USE_PLUS
                                case ID_MIGHTY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 22, &van->animator8, true, 0); break;
                                case ID_RAY: RSDK.SetSpriteAnimation(TVVan->aniFrames, 23, &van->animator8, true, 0); break;
#endif
                                default: RSDK.SetSpriteAnimation(TVVan->aniFrames, 19, &van->animator8, true, 0); break;
                            }
                            van->scale.x                       = 0x000;
                            van->scale.y                       = 0x000;
                            van->stateDraw[Zone->drawOrderLow] = TVVan_StateDraw14_Low;
                            van->state                         = TVVan_State14_Unknown2;
                            entity->state                      = TVVan_State15_Unknown5;
                            entity->timer                      = 0;
                            entity->direction                  = FLIP_NONE;
                        }
                        else {
                            switch (entity->angle >> 5) {
                                case 1:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &entity->animator5, true, 0);
                                    entity->direction = FLIP_X;
                                    break;
                                case 2:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &entity->animator5, true, 0);
                                    entity->direction = FLIP_NONE;
                                    break;
                                case 3:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &entity->animator5, true, 0);
                                    entity->direction = FLIP_NONE;
                                    break;
                                case 5:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &entity->animator5, true, 0);
                                    entity->direction = FLIP_Y;
                                    break;
                                case 6:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 11, &entity->animator5, true, 0);
                                    entity->direction = FLIP_Y;
                                    break;
                                case 7:
                                    RSDK.SetSpriteAnimation(TVVan->aniFrames, 9, &entity->animator5, true, 0);
                                    entity->direction = FLIP_XY;
                                    break;
                                default: break;
                            }
                            entity->field_C4[0] = van->field_C4[0];
                            entity->field_C4[1] = van->field_C4[1];
                            entity->field_CC[0] = van->field_CC[0];
                            entity->field_CC[1] = van->field_CC[1];
                            RSDK.PlaySfx(TVVan->sfxSatellite2, false, 255);
                            entity->state = TVVan_State15_Unknown3;
                        }
                        foreach_break;
                    }
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void TVVan_State15_Unknown3(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = entity->player;

    RSDK.ProcessAnimation(&entity->animator5);
    player = entity->player;
    player->position.x += (entity->position.x - player->position.x) >> 2;
    player->position.y += (entity->position.y - player->position.y) >> 2;

    if (Player_CheckValidState(player)) {
        if (entity->angle == entity->field_C4[0] || entity->angle == entity->field_C4[1])
            entity->angle = entity->field_CC[Zone->timer & 1];
        else
            entity->angle = entity->field_C4[Zone->timer & 1];
        switch (entity->angle >> 5) {
            case 1:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &entity->animator5, true, 0);
                entity->direction = FLIP_X;
                break;
            case 2:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &entity->animator5, true, 0);
                entity->direction = FLIP_NONE;
                break;
            case 3:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &entity->animator5, true, 0);
                entity->direction = FLIP_NONE;
                break;
            case 5:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &entity->animator5, true, 0);
                entity->direction = FLIP_Y;
                break;
            case 6:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &entity->animator5, true, 0);
                entity->direction = FLIP_Y;
                break;
            case 7:
                RSDK.SetSpriteAnimation(TVVan->aniFrames, 8, &entity->animator5, true, 0);
                entity->direction = FLIP_XY;
                break;
            default: break;
        }
        entity->angle = (entity->angle + 0x80) & 0xFF;
        RSDK.PlaySfx(TVVan->sfxSatellite2, false, 255);
        entity->state = TVVan_State15_Unknown4;
    }
    else {
        destroyEntity(entity);
    }
}

void TVVan_State15_Unknown4(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = entity->player;

    RSDK.ProcessAnimation(&entity->animator5);
    player->position.x += (entity->position.x - player->position.x) >> 2;
    player->position.y += (entity->position.y - player->position.y) >> 2;
    if (Player_CheckValidState(player)) {
        if (entity->animator5.frameID >= entity->animator5.frameCount - 1) {
            int vel = 0;
            if (entity->animator5.animationID == 8)
                vel = 0x240000;
            else
                vel = 0;

            if (entity->direction & FLIP_X)
                vel = -vel;
            entity->position.x += vel;

            if (entity->direction & FLIP_Y)
                entity->position.y += 0x280000;
            else
                entity->position.y += -0x280000;
            entity->visible = false;
            entity->timer   = 0;
            entity->state = TVVan_State15_Unknown2;
        }
    }
    else {
        destroyEntity(entity);
    }
}

void TVVan_State14_Unknown1(void)
{
    RSDK_THIS(TVVan);
    RSDK.ProcessAnimation(&entity->animator6);
}

void TVVan_State14_Unknown2(void)
{
    RSDK_THIS(TVVan);

    RSDK.ProcessAnimation(&entity->animator6);
    RSDK.ProcessAnimation(&entity->animator8);
    RSDK.ProcessAnimation(&entity->animator9);

    if (entity->scale.x >= 0x200) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator7, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 17, &entity->animator6, true, 0);
        entity->state             = TVVan_State14_Unknown3;
        entity->stateDraw[Zone->drawOrderLow] = TVVan_StateDraw14_High;
        entity->active       = ACTIVE_NORMAL;
    }
    else {
        entity->scale.x += 16;
        entity->scale.y = entity->scale.x;
    }
}

void TVVan_State14_Unknown3(void)
{
    RSDK_THIS(TVVan);

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &entity->animator7, true, 9);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &entity->animator6, true, 0);
        RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &entity->animator9, true, 0);
        entity->state       = TVVan_State14_Unknown1;
        entity->active = ACTIVE_BOUNDS;
    }
}

void TVVan_State15_Unknown5(void)
{
    RSDK_THIS(TVVan);
    EntityPlayer *player = entity->player;

    player->position.x += (entity->position.x - player->position.x) >> 2;
    player->position.y += (entity->position.y - player->position.y) >> 2;
    if (Player_CheckValidState(player)) {
        RSDK.ProcessAnimation(&entity->animator5);
        if (++entity->timer != 32)
            return;
        player->state = Player_State_Air;
        RSDK.SetSpriteAnimation(player->aniFrames, 10, &player->playerAnimator, false, 0);
        player->velocity.x                    = 0;
        player->playerAnimator.animationSpeed = 120;
        player->velocity.y                    = -0x80000;
        player->onGround                      = false;
        player->tileCollisions                = true;
        player->interaction                   = true;
        player->visible                       = true;
        player->jumpAbility                   = 0;
    }
    RSDK.PlaySfx(TVVan->sfxExplosion, false, 255);
    
    EntityExplosion *explosion          = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), entity->position.x, entity->position.y - 0x100000);
    explosion->drawFX  = FX_SCALE;
    explosion->scale.x = 0x300;
    explosion->scale.y = 0x300;

    for (int i = 0; i < 12; ++i) {
        int x              = entity->position.x + RSDK.Rand(-0xC0000, 0xC0000);
        int y              = entity->position.y + RSDK.Rand(-0xC0000, 0xC0000);
        EntityDebris *debris             = CREATE_ENTITY(Debris, NULL, x, y);
        debris->state      = Debris_State_Fall;
        debris->gravity    = 0x4000;
        debris->velocity.x = RSDK.Rand(0, 0x20000);
        if (debris->position.x < entity->position.x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawFX     = FX_FLIP;
        debris->direction  = i & 3;
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
    
    }

    foreach_active(ShopWindow, window) {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &TVVan->hitbox5, window, &window->hitbox)) {
            window->stateDraw = ShopWindow_StateDraw_Shattered;
            window->state     = ShopWindow_State_Shattered;
        }
    }

    destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void TVVan_EditorDraw(void)
{
    RSDK_THIS(TVVan);

    entity->drawFX        = FX_FLIP;
    switch (entity->type) {
        case TVVAN_SATELLITE_NE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0xE0;
            entity->field_C4[1]                    = 0xE0;
            entity->field_CC[0]                    = 0x60;
            entity->field_CC[1]                    = 0x60;
            TVVan_StateDraw2_High();
            break;
        case TVVAN_SATELLITE_SE_NW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0x20;
            entity->field_C4[1]                    = 0x20;
            entity->field_CC[0]                    = 0xA0;
            entity->field_CC[1]                    = 0xA0;
            TVVan_StateDraw3_High();
            break;
        case TVVAN_SATELLITE_NW_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0x20;
            entity->field_C4[1]                    = 0x20;
            entity->field_CC[0]                    = 0xE0;
            entity->field_CC[1]                    = 0xE0;
            TVVan_StateDraw4_High();
            break;
        case TVVAN_SATELLITE_NE_SE:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0x60;
            entity->field_C4[1]                    = 0x60;
            entity->field_CC[0]                    = 0xA0;
            entity->field_CC[1]                    = 0xA0;
            TVVan_StateDraw5_High();
            break;
        case TVVAN_SATELLITE_NE_NW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0x20;
            entity->field_C4[1]                    = 0x20;
            entity->field_CC[0]                    = 0x60;
            entity->field_CC[1]                    = 0x60;
            TVVan_StateDraw6_High();
            break;
        case TVVAN_SATELLITE_SE_SW:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0xE0;
            entity->field_C4[1]                    = 0xE0;
            entity->field_CC[0]                    = 0xA0;
            entity->field_CC[1]                    = 0xA0;
            TVVan_StateDraw7_High();
            break;
        case TVVAN_SATELLITE_SW_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0xA0;
            entity->field_C4[1]                    = 0xA0;
            entity->field_CC[0]                    = 0x00;
            entity->field_CC[1]                    = 0x00;
            TVVan_StateDraw8_High();
            break;
        case TVVAN_SATELLITE_NW_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0x20;
            entity->field_C4[1]                    = 0x20;
            entity->field_CC[0]                    = 0xC0;
            entity->field_CC[1]                    = 0xC0;
            TVVan_StateDraw9_High();
            break;
        case TVVAN_SATELLITE_SE_N:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0xA0;
            entity->field_C4[1]                    = 0xA0;
            entity->field_CC[0]                    = 0x40;
            entity->field_CC[1]                    = 0x40;
            TVVan_StateDraw10_High();
            break;
        case TVVAN_SATELLITE_NE_S:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 6, &entity->animator1, true, 0);
            entity->drawOrder                      = Zone->drawOrderHigh;
            entity->field_C4[0]                    = 0xC0;
            entity->field_C4[1]                    = 0xC0;
            entity->field_CC[0]                    = 0x40;
            entity->field_CC[1]                    = 0x40;
            TVVan_StateDraw11_High();
            break;
        case TVVAN_EXITTV:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &entity->animator7, true, 9);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 15, &entity->animator6, true, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 18, &entity->animator9, true, 0);
            entity->drawOrder                     = Zone->drawOrderLow;
            TVVan_StateDraw14_High();
            break;
        case TVVAN_15:
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 10, &entity->animator5, true, 0);
            entity->active                         = ACTIVE_NORMAL;
            entity->drawOrder                      = Zone->drawOrderHigh;
            TVVan_StateDraw15_High();
            break;
        default:
            entity->drawOrder = Zone->drawOrderLow;
            entity->state     = TVVan_State_Unknown1;
            if (entity->type == TVVAN_VAN_RIGHT)
                entity->direction = FLIP_X;
            entity->drawPos                        = entity->position;
            entity->drawPos2                       = entity->position;
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 0, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 1, &entity->animator2, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 2, &entity->animator3, false, 0);
            RSDK.SetSpriteAnimation(TVVan->aniFrames, 5, &entity->animator4, false, 0);

            TVVan_StateDraw_Low();
            TVVan_StateDraw_High();
            break;
    }
}

void TVVan_EditorLoad(void)
{
    TVVan->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/TVVan.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TVVan, type);
    RSDK_ENUM_VAR("Van (Left)", TVVAN_VAN_LEFT);
    RSDK_ENUM_VAR("Van (Right)", TVVAN_VAN_RIGHT);
    RSDK_ENUM_VAR("Satellite (NE, SW)", TVVAN_SATELLITE_NE_SW);
    RSDK_ENUM_VAR("Satellite (SE, NW)", TVVAN_SATELLITE_SE_NW);
    RSDK_ENUM_VAR("Satellite (NW, SW)", TVVAN_SATELLITE_NW_SW);
    RSDK_ENUM_VAR("Satellite (NE, SE)", TVVAN_SATELLITE_NE_SE);
    RSDK_ENUM_VAR("Satellite (NE, NE)", TVVAN_SATELLITE_NE_NW);
    RSDK_ENUM_VAR("Satellite (SE, SW)", TVVAN_SATELLITE_SE_SW);
    RSDK_ENUM_VAR("Satellite (SW, N)", TVVAN_SATELLITE_SW_N);
    RSDK_ENUM_VAR("Satellite (NW, S)", TVVAN_SATELLITE_NW_S);
    RSDK_ENUM_VAR("Satellite (SE, S)", TVVAN_SATELLITE_SE_N);
    RSDK_ENUM_VAR("Satellite (NE, S)", TVVAN_SATELLITE_NE_S);
    RSDK_ENUM_VAR("Unused 1", TVVAN_UNUSED1);
    RSDK_ENUM_VAR("Unused 2", TVVAN_UNUSED2);
    RSDK_ENUM_VAR("Exit TV", TVVAN_EXITTV);
}
#endif

void TVVan_Serialize(void) { RSDK_EDITABLE_VAR(TVVan, VAR_UINT8, type); }


#include "SonicMania.h"

ObjectDERobot *DERobot;

void DERobot_Update(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(entity->state);
}

void DERobot_LateUpdate(void) {}

void DERobot_StaticUpdate(void) {}

void DERobot_Draw(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(entity->stateDraw);
}

void DERobot_Create(void *data)
{
    RSDK_THIS(DERobot);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->drawOrder     = Zone->drawOrderLow;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            int slotID            = RSDK.GetEntityID(entity);
            if (data)
                entity->aniID = voidToInt(data);
            switch (entity->aniID) {
                case 0:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->parts[3]  = (Entity *)RSDK.GetEntityByID(slotID - 7);
                    entity->parts[4]  = (Entity *)RSDK.GetEntityByID(slotID - 6);
                    entity->parts[5]  = (Entity *)RSDK.GetEntityByID(slotID - 5);
                    entity->parts[9]  = (Entity *)RSDK.GetEntityByID(slotID - 4);
                    entity->parts[10] = (Entity *)RSDK.GetEntityByID(slotID - 3);
                    entity->parts[2]  = (Entity *)RSDK.GetEntityByID(slotID - 2);
                    entity->parts[0]  = (Entity *)RSDK.GetEntityByID(slotID - 1);
                    entity->parts[6]  = (Entity *)RSDK.GetEntityByID(slotID + 1);
                    entity->parts[7]  = (Entity *)RSDK.GetEntityByID(slotID + 2);
                    entity->parts[8]  = (Entity *)RSDK.GetEntityByID(slotID + 3);
                    entity->parts[1]  = (Entity *)RSDK.GetEntityByID(slotID + 4);
                    entity->parts[11] = (Entity *)RSDK.GetEntityByID(slotID + 5);
                    entity->parts[12] = (Entity *)RSDK.GetEntityByID(slotID + 6);
                    entity->field_C8  = 8;
                    entity->state     = DERobot_Unknown31;
                    entity->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 1:
                    entity->dword70   = RSDK.GetEntityByID(slotID + 1);
                    entity->stateDraw = DERobot_StateDraw_Unknown1;
                    entity->drawFX    = FX_ROTATE;
                    entity->offset.x  = -0x160000;
                    entity->offset.y  = -0x240000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 2:
                    if (entity->frameID == 2) {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 5, &entity->animator2, true, 0);
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 4, &entity->animator3, true, 0);
                        entity->stateDraw = DERobot_StateDraw_Unknown2;
                        entity->drawFX    = FX_ROTATE;
                        entity->state     = DERobot_Unknown18;
                    }
                    else if (entity->frameID) {
                        entity->stateDraw = DERobot_StateDraw_Unknown3;
                    }
                    else {
                        entity->dword70   = RSDK.GetEntityByID(slotID - 4);
                        entity->stateDraw = DERobot_StateDraw_Unknown1;
                        entity->offset.x  = -0xC0000;
                        entity->offset.y  = -0x100000;
                    }
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 3:
                    if (entity->frameID) {
                        entity->stateDraw = DERobot_StateDraw_Unknown3;
                    }
                    else {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator2, true, 1);
                        entity->stateDraw = DERobot_StateDraw_Unknown4;
                    }
                    if (entity->frameID > 1)
                        entity->drawFX = FX_ROTATE;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 6:
                    entity->active     = ACTIVE_NORMAL;
                    entity->visible    = true;
                    entity->drawFX     = FX_FLIP;
                    entity->drawOrder  = Zone->drawOrderHigh;
                    entity->velocity.x = 0x20000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, 7, &entity->animator2, true, 0);
                    entity->state     = DERobot_Unknown33;
                    entity->stateDraw = DERobot_StateDraw_Unknown5;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 9:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->state     = DERobot_Unknown28;
                    entity->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                case 10:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = true;
                    entity->stateDraw = DERobot_StateDraw_Unknown3;
                    entity->parts[1]  = RSDK.GetEntityByID(slotID + 4);
                    entity->parts[9]  = RSDK.GetEntityByID(slotID + 1);
                    entity->parts[10] = RSDK.GetEntityByID(slotID + 2);
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
                default:
                    entity->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, entity->aniID, &entity->animator1, true, entity->frameID);
                    break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void DERobot_StageLoad(void)
{
    DERobot->aniFrames      = RSDK.LoadSpriteAnimation("GHZ/DERobot.bin", SCOPE_STAGE);
    DERobot->hitbox.left    = -32;
    DERobot->hitbox.top     = -40;
    DERobot->hitbox.right   = 32;
    DERobot->hitbox.bottom  = 32;
    DERobot->hitbox2.left   = -9;
    DERobot->hitbox2.top    = -9;
    DERobot->hitbox2.right  = 9;
    DERobot->hitbox2.bottom = 9;
    DERobot->sfxHit         = RSDK.GetSFX("Stage/BossHit.wav");
    DERobot->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
    DERobot->sfxImpact      = RSDK.GetSFX("Stage/Impact2.wav");
    DERobot->sfxTargeting   = RSDK.GetSFX("Stage/Targeting1.wav");
    DERobot->sfxLedgeBreak  = RSDK.GetSFX("Stage/LedgeBreak.wav");
    DERobot->sfxBuzzsaw     = RSDK.GetSFX("Stage/Buzzsaw.wav");
    DERobot->sfxDrop        = RSDK.GetSFX("Stage/Drop.wav");
    DERobot->sfxButton2     = RSDK.GetSFX("Stage/Button2.wav");
    DERobot->sfxHullClose   = RSDK.GetSFX("Stage/HullClose.wav");
    RSDK.SetPaletteEntry(0, 236, 0x282028);
    RSDK.SetPaletteEntry(0, 237, 0x383040);
    RSDK.SetPaletteEntry(0, 238, 0x484868);
    RSDK.SetPaletteEntry(0, 239, 0x587090);
    RSDK.SetPaletteEntry(0, 244, 0x000000);
}

void DERobot_Unknown1(void)
{
    RSDK_THIS(DERobot);
    int x = (entity->position.x >> 16) - RSDK_screens->centerX + 128;
    if (x > Zone->screenBoundsL1[0]) {
        Zone->screenBoundsL1[0]                        = x;
        Zone->screenBoundsR1[0]                        = RSDK_screens->width + 96 + x;
        Zone->screenBoundsL2[0]                        = Zone->screenBoundsL1[0] << 16;
        Zone->screenBoundsR2[0]                        = Zone->screenBoundsR1[0] << 16;
        Zone->playerBoundActiveB[0]                    = 0;
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsL = Zone->screenBoundsL1[0];
    }
}

void DERobot_Unknown2(int a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)entity->parts[a1 + 3];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[a1 + 4];
    EntityDERobot *part3 = (EntityDERobot *)entity->parts[a1 + 5];

    part1->position.x = 0x440 * RSDK.Sin1024(part1->angle) + entity->position.x;
    part1->position.y = 0x440 * RSDK.Cos1024(part1->angle) + entity->position.y + 0x190000;
    part2->position.x = part1->position.x;
    part2->position.y = part1->position.y;
    part2->angle      = maxVal(part1->angle >> 2, 0);
    part2->rotation   = -(part2->angle >> 1);
    part3->position.x = 0xA00 * RSDK.Sin1024(part2->angle) + part2->position.x;
    part3->position.y = 0xA00 * RSDK.Cos1024(part2->angle) + part2->position.y;
    part3->onGround   = false;
    int storeX        = part3->position.x;
    int storeY        = part3->position.y;
    if (RSDK.ObjectTileGrip(part3, Zone->fgLayers, CMODE_FLOOR, 0, -0x100000, 0x100000, 32)) {
        part3->position.y = storeY;
        if (RSDK.ObjectTileGrip(part3, Zone->fgLayers, CMODE_FLOOR, 0, 0x100000, 0x100000, 32))
            part3->rotation = 2 * RSDK.ATan2(32, (part3->position.y >> 16) - (part3->position.y >> 16));
    }
    part3->position.x = storeX;
    part3->position.y = storeY;
    if (RSDK.ObjectTileCollision(part3, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x110000, true))
        part3->onGround = true;
}

void DERobot_Unknown3(int a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)entity->parts[a1 + 3];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[a1 + 4];
    EntityDERobot *part3 = (EntityDERobot *)entity->parts[a1 + 2];
    part1->position.x    = part2->position.x - 0xA00 * RSDK.Sin1024(part1->angle);
    part1->position.y    = part2->position.y - 0xA00 * RSDK.Cos1024(part1->angle);
    part1->angle -= part1->angle >> 3;
    part1->rotation -= part1->rotation >> 3;
    part3->position.x  = part1->position.x;
    part3->position.y  = part1->position.y;
    entity->position.x = part3->position.x - 0x440 * RSDK.Sin1024(part3->angle);
    entity->position.y = part3->position.y - (0x440 * RSDK.Cos1024(part3->angle)) - 0x190000;
}

void DERobot_Unknown4(int a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)entity->parts[a1 + 9];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[a1 + 10];
    part1->position.x    = 0x600 * (RSDK.Sin1024(part1->angle) - 0x200) + entity->position.x;
    part1->position.y    = 0x600 * (RSDK.Cos1024(part1->angle) - 0x300) + entity->position.y;
    part2->position.x    = part1->position.x + 0x600 * RSDK.Cos1024(minVal(part1->angle, 0));
    part2->position.y    = part1->position.y - 0x600 * RSDK.Sin1024(minVal(part1->angle, 0));
}

void DERobot_Unknown5(void)
{
    RSDK_THIS(DERobot);
    int tx = (entity->position.x + 0xC0000) >> 20;
    if (tx > entity->field_D0) {
        entity->field_D0 = tx;
        int ty           = (entity->position.y >> 20) - 16;
        int spawnY       = (ty << 20) + 0x80000;

        bool32 playSFX = false;
        for (int i = 0; i < 32; ++i) {
            ushort tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                EntityBreakableWall *wall =
                    (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, intToVoid(1), (tx << 20) + 0x80000, spawnY);
                wall->drawOrder  = Zone->drawOrderHigh;
                wall->visible    = true;
                wall->tileInfo   = tile;
                wall->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y = RSDK.Rand(-0x20000, 0x20000);
                playSFX          = true;
                wall->drawFX     = FX_ROTATE | FX_FLIP;
            }
            ++ty;
            spawnY += 0x100000;
        }

        if (playSFX)
            RSDK.PlaySFX(DERobot->sfxLedgeBreak, 0, 255);
    }
}

void DERobot_Unknown6(void)
{
    RSDK_THIS(DERobot);

    int tx     = (entity->position.x >> 20) - 4;
    int ty     = (entity->position.y >> 20) - 16;
    int spawnX = (tx << 20) + 0x80000;
    for (int y = 0; y < 8; ++y) {
        int spawnY = (ty << 20) + 0x80000;
        for (int x = 0; x < 32; ++x) {
            ushort tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                EntityBreakableWall *wall = (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, intToVoid(1), spawnX, spawnY);
                wall->drawOrder           = Zone->drawOrderHigh;
                wall->visible             = true;
                wall->tileInfo            = tile;
                wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                wall->drawFX              = FX_ROTATE | FX_FLIP;
            }
            ++ty;
            spawnY += 0x100000;
        }
        spawnX += 0x100000;
        ty -= 32;
        ++tx;
    }

    tx     = (entity->position.x >> 20) - 4;
    ty     = (entity->position.y >> 20) - 16;
    spawnX = (tx << 20) + 0x80000;
    for (int y = 0; y < 32; ++y) {
        int spawnY = (ty << 20) + 0x80000;
        for (int x = 0; x < 32; ++x) {
            ushort tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                EntityBreakableWall *wall = (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, intToVoid(1), spawnX, spawnY);
                wall->drawOrder           = Zone->drawOrderHigh;
                wall->visible             = true;
                wall->tileInfo            = tile;
                wall->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                wall->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                wall->drawFX              = FX_ROTATE | FX_FLIP;
            }
            ++ty;
            spawnY += 0x100000;
        }
        spawnX += 0x100000;
        ty -= 32;
        ++tx;
    }
}

void DERobot_Unknown7(void)
{
    RSDK_THIS(DERobot);
    if (--entity->field_C8 <= 0) {
        entity->timer               = 0;
        entity->state               = DERobot_Unknown38;
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        ((EntityDERobot *)entity->parts[10])->state = DERobot_Unknown22;
        ((EntityDERobot *)entity->parts[12])->state = DERobot_Unknown22;
    }
    else {
        entity->field_C4 = 48;
        RSDK.PlaySFX(DERobot->sfxHit, 0, 255);
        if (entity->field_C8 == 2) {
            entity->field_D4 = 16;
            entity->field_D8 = 0x20000;
            entity->field_DC = -0x4000;
            ((EntityDERobot *)entity->parts[3])->field_B0 *= 2;
            ((EntityDERobot *)entity->parts[3])->field_B4 *= 4;
            ((EntityDERobot *)entity->parts[6])->field_B0 *= 2;
            ((EntityDERobot *)entity->parts[6])->field_B4 *= 4;
        }
    }
}

void DERobot_Unknown8(void)
{
    RSDK_THIS(DERobot);

    if (entity->field_C4 > 0) {
        entity->field_C4--;
        if ((entity->field_C4 & 2)) {
            RSDK.SetPaletteEntry(0, 236, 0xC0C0C0);
            RSDK.SetPaletteEntry(0, 237, 0xD0D0D0);
            RSDK.SetPaletteEntry(0, 238, 0xE0E0E0);
            RSDK.SetPaletteEntry(0, 239, 0xF0F0F0);
            RSDK.SetPaletteEntry(0, 244, 0xA0A0A0);
        }
        else {
            RSDK.SetPaletteEntry(0, 236, 0x282028);
            RSDK.SetPaletteEntry(0, 237, 0x383040);
            RSDK.SetPaletteEntry(0, 238, 0x484868);
            RSDK.SetPaletteEntry(0, 239, 0x587090);
            RSDK.SetPaletteEntry(0, 244, 0x000000);
        }
    }

    foreach_active(Player, player)
    {
        if (!entity->field_C4 && Player_CheckBadnikHit(player, entity, &DERobot->hitbox) && Player_CheckBossHit(player, entity)) {
            DERobot_Unknown7();
            RSDK.PlaySFX(DERobot->sfxHit, 0, 255);
            player->velocity.x = 0x60000;
        }
    }
}

void DERobot_Unknown9(void)
{
    RSDK_THIS(DERobot);
    int val = (entity->field_CC + 22) << 16;
    val >>= 8;
    int angle            = -(entity->angle >> 3);
    int val1             = (-0xE00 * RSDK.Sin256(angle)) + (0x1600 * RSDK.Cos256(angle));
    int val2             = (-0xE00 * RSDK.Cos256(angle)) - (0x1600 * RSDK.Sin256(angle));
    int val3             = (-0xE00 * RSDK.Sin256(angle)) + val * RSDK.Cos256(angle);
    int val4             = (((-0xE00 * RSDK.Cos256(angle)) - (val * RSDK.Sin256(angle))) >> 16) - (val2 >> 16);
    entity->hitbox.left  = (val1 >> 16);
    entity->hitbox.right = (val3 >> 16);
    foreach_active(Player, player)
    {
        int dist = (player->position.x - entity->position.x) >> 16;
        if (dist >= val1 >> 16) {
            if (dist > (val3 >> 16))
                dist = (val3 >> 16);
        }
        else {
            dist = val1 >> 16;
        }
        entity->hitbox.top    = (val2 >> 16) + val4 * (dist - (val1 >> 16)) / ((val3 >> 16) - (val1 >> 16));
        entity->hitbox.bottom = entity->hitbox.top + 16;
        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
            player->position.y += 0x40000;
    }
}

void DERobot_Unknown10(void)
{
    RSDK_THIS(DERobot);
    int storeX = entity->position.x;
    int storeY = entity->position.y;
    Vector2 pos;
    pos.x = entity->position.x;
    pos.y = entity->position.y;
    entity->position.x += (entity->field_CC + 35) << 16;
    Zone_Unknown3(&pos, &entity->position, -(entity->angle >> 3));

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &DERobot->hitbox2)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(1024, player, false, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }
    }

    DERobot_Unknown5();
    entity->position.x = storeX;
    entity->position.y = storeY;
}

bool32 DERobot_Unknown11(void)
{
    RSDK_THIS(DERobot);
    bool32 flag = false;
    int storeX  = entity->position.x;
    int storeY  = entity->position.y;
    Vector2 pos;
    pos.x = entity->position.x;
    pos.y = entity->position.y;
    entity->position.x += (entity->field_CC + 48) << 16;
    Zone_Unknown3(&pos, &entity->position, -(entity->angle >> 3));

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(entity->position.x - ruby->position.x) < 0x80000) {
            if (abs(entity->position.y - ruby->position.y) < 0x80000) {
                flag             = true;
                ruby->position.x = entity->position.x;
                ruby->position.y = entity->position.y;
                ruby->drawOrder  = Zone->drawOrderLow;
                ruby->state      = PhantomRuby_Unknown10;
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    return flag;
}

void DERobot_Unknown12(void)
{
    RSDK_THIS(DERobot);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &DERobot->hitbox2)) {
            Player_CheckHit(player, entity);
        }
    }
}

void DERobot_StateDraw_Unknown1(void)
{
    RSDK_THIS(DERobot);
    entity->position.x = entity->dword70->position.x;
    entity->position.y = entity->dword70->position.y;
    entity->position.x += entity->offset.x;
    entity->position.y += entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, 0, false);
}

void DERobot_StateDraw_Unknown2(void)
{
    RSDK_THIS(DERobot);
    entity->rotation = entity->angle >> 2;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (entity->field_CC > 0) {
        SpriteFrame *frame = RSDK.GetFrame(DERobot->aniFrames, entity->animator2.animationID, entity->animator2.frameID);
        frame->width       = entity->field_CC;
        frame->sprX        = 4 * (Zone->timer & 3);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }

    RSDK.GetFrame(DERobot->aniFrames, entity->animator3.animationID, entity->animator3.frameID)->pivotX = entity->field_CC + 22;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void DERobot_StateDraw_Unknown3(void)
{
    RSDK_THIS(DERobot);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void DERobot_StateDraw_Unknown4(void)
{
    RSDK_THIS(DERobot);

    entity->drawFX   = FX_NONE;
    entity->rotation = -(RSDK_sceneInfo->entity->angle >> 1);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void DERobot_StateDraw_Unknown5(void)
{
    RSDK_THIS(DERobot);
    Vector2 drawPos;

    entity->animator1.frameID = 0;
    entity->inkEffect         = INK_ALPHA;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y;
    drawPos.x -= entity->offset.x;
    drawPos.y -= entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;
    drawPos.x += entity->offset.x;
    drawPos.y -= entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 1;
    entity->direction         = FLIP_NONE;
    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y;
    drawPos.x -= entity->offset.x;
    drawPos.y += entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_X;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;
    drawPos.x += entity->offset.x;
    drawPos.y += entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->inkEffect = INK_NONE;
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void DERobot_Unknown18(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->animator3.animationSpeed > 0x40)
        entity->animator3.animationSpeed -= 4;
    DERobot_Unknown10();
}

void DERobot_Unknown19(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->animator3.animationSpeed == 240)
        RSDK.PlaySFX(DERobot->sfxBuzzsaw, 0, 255);

    if (entity->animator3.animationSpeed >= 0x100u)
        entity->state = DERobot_Unknown20;
    else
        entity->animator3.animationSpeed += 4;
    DERobot_Unknown10();
}

void DERobot_Unknown20(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->field_CC < 192)
        entity->field_CC += 8;
    DERobot_Unknown9();
    DERobot_Unknown10();
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = DERobot_Unknown21;
    }
}

void DERobot_Unknown21(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->field_CC <= 0) {
        entity->state = DERobot_Unknown18;
    }
    else {
        DERobot_Unknown9();
        entity->field_CC -= 8;
    }
    DERobot_Unknown10();
}

void DERobot_Unknown22(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->field_CC > 0) {
        entity->field_CC -= 8;
    }
}

void DERobot_Unknown23(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->angle > -96)
        entity->angle -= 4;

    if (entity->animator3.animationSpeed < 0x80) {
        entity->animator3.animationSpeed += 4;
    }

    if (entity->timer++ == -1)
        RSDK.PlaySFX(DERobot->sfxButton2, 0, 255);

    if (entity->timer == 30) {
        entity->timer = 0;
        entity->state = DERobot_Unknown24;
        RSDK.PlaySFX(DERobot->sfxBuzzsaw, 0, 255);
    }
}

void DERobot_Unknown24(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (DERobot_Unknown11() || entity->field_CC >= 192) {
        entity->state = DERobot_Unknown25;
    }
    else {
        entity->field_CC += 8;
    }
}

void DERobot_Unknown25(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->field_CC <= 0) {
        RSDK.StopSFX(DERobot->sfxBuzzsaw);
        entity->state = DERobot_Unknown26;
    }
    else {
        entity->field_CC -= 8;
        DERobot_Unknown11();
    }
}

void DERobot_Unknown26(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->animator3.animationSpeed)
        entity->animator3.animationSpeed--;
}

void DERobot_Unknown27(void)
{
    RSDK_THIS(DERobot);

    entity->timer++;
    if (entity->timer == 30) {
        RSDK.PlaySFX(DERobot->sfxButton2, 0, 255);
    }
    if (entity->timer >= 30) {
        if (entity->rotation >= 0) {
            entity->timer  = 0;
            entity->state  = 0;
            entity->drawFX = FX_NONE;
            RSDK.PlaySFX(DERobot->sfxHullClose, 0, 255);
        }
        else {
            entity->rotation += 8;
        }
    }
}

void DERobot_Unknown28(void)
{
    RSDK_THIS(DERobot);
    entity->angle += 7;
    if (entity->angle > 480) {
        entity->velocity.y += 0x4000;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }
    else {
        entity->velocity.x = entity->position.x;
        entity->position.x = entity->offset.x - entity->scale.x * RSDK.Cos1024(entity->angle);
        entity->velocity.x = entity->position.x - entity->velocity.x;
        entity->velocity.y = entity->position.y;
        entity->position.y = entity->offset.y - entity->scale.y * RSDK.Sin1024(entity->angle);
        entity->velocity.y = entity->position.y - entity->velocity.y;
    }
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xE0000, true))
        entity->state = DERobot_Unknown29;
    DERobot_Unknown12();
}

void DERobot_Unknown29(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.animationSpeed >= 0x80) {
        entity->visible = false;
        entity->state   = DERobot_Unknown30;
        EntityExplosion *explosion =
            (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, intToVoid(3), entity->position.x, entity->position.y - 0x80000);
        explosion->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySFX(DERobot->sfxExplosion, 0, 255);
    }
    else {
        entity->animator1.animationSpeed++;
    }
    DERobot_Unknown12();
}

void DERobot_Unknown30(void)
{
    RSDK_THIS(DERobot);
    ++entity->timer;
    entity->position.y -= 0x20000;
    if (entity->timer < 16)
        DERobot_Unknown12();

    if (!(entity->timer & 7)) {
        EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, intToVoid(3), entity->position.x, entity->position.y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }

    if (entity->timer == 32)
        destroyEntity(entity);
}

void DERobot_Unknown31(void)
{
    RSDK_THIS(DERobot);
    if (++entity->timer >= 8) {
        entity->timer         = 0;
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.y <= entity->position.y + 0x200000 && player1->state != Player_State_ForceRoll_Ground) {
            for (int i = 0; i < Player->playerCount; ++i) {
                Zone->screenBoundsL1[i]     = (entity->position.x >> 16) - RSDK_screens->centerX + 128;
                Zone->screenBoundsR1[i]     = RSDK_screens->centerX + 128 + (entity->position.x >> 16);
                Zone->screenBoundsB1[i]     = entity->position.y >> 16;
                Zone->screenBoundsB2[i]     = Zone->screenBoundsB1[i] << 16;
                Zone->playerBoundActiveL[i] = true;
                Zone->playerBoundActiveR[i] = true;
                Zone->playerBoundActiveB[i] = false;
            }
            entity->position.y -= 0x1800000;
            entity->active = ACTIVE_NORMAL;
            entity->state  = DERobot_Unknown32;
        }
    }
}

void DERobot_Unknown32(void)
{
    RSDK_THIS(DERobot);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 60) {
            RSDK.CreateEntity(DERobot->objectID, intToVoid(6), entity->position.x, 0x3080000);
            RSDK.PlaySFX(DERobot->sfxTargeting, 0, 255);
            Music_TransitionTrack(TRACK_EGGMAN1, 0.125);
        }

        if (entity->timer == 160) {
            entity->timer   = 0;
            entity->visible = true;
            entity->state   = DERobot_Unknown34;
            if (player1->characterID == ID_TAILS)
                player1->movesetState = Player_TailsJumpAbility;
            EntityDERobot *part = (EntityDERobot *)entity->parts[3];
            part->active        = ACTIVE_NORMAL;
            part->visible       = true;

            part          = (EntityDERobot *)entity->parts[4];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[5];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[6];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[7];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[8];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part           = (EntityDERobot *)entity->parts[0];
            part->active   = ACTIVE_NORMAL;
            part->visible  = true;
            part->rotation = -128;

            part          = (EntityDERobot *)entity->parts[1];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[9];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[10];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[11];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)entity->parts[12];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            EntityEggman *eggman = (EntityEggman *)entity->parts[2];
            RSDK.ResetEntityPtr(eggman, Eggman->objectID, entity);
            eggman->state    = Eggman_Unknown1;
            eggman->animID   = 0;
            eggman->offset.x = -0x40000;
            eggman->offset.y = -0x200000;
        }
    }
    else if (player1->position.x > 0x800000 + entity->position.x && player1->onGround) {
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsOffset.x = 1;
        ++entity->timer;
        if (player1->characterID == ID_TAILS)
            player1->movesetState = NULL;
    }

    foreach_active(Player, player)
    {
        if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air)
            player->state = Player_State_Air;
    }
}

void DERobot_Unknown33(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->dword70) {
        entity->position.x = entity->dword70->position.x;
        entity->position.y = entity->dword70->position.y;
    }

    entity->alpha += 32;
    entity->offset.x -= entity->velocity.x;
    if (entity->offset.x <= 0xC0000) {
        entity->alpha    = 0;
        entity->offset.x = 0x2C0000;
    }

    entity->offset.y = entity->offset.x;
    if (++entity->timer == 60)
        RSDK.SetSpriteAnimation(DERobot->aniFrames, 8, &entity->animator3, true, 0);
    if (entity->timer == 96) {
        entity->dword70 = 0;
        foreach_active(DERobot, robot)
        {
            if (!robot->aniID) {
                robot->offset.x = entity->position.x;
                robot->offset.y = entity->position.y;
            }
        }
    }
    if (entity->timer == 128)
        destroyEntity(entity);
}

void DERobot_Unknown34(void)
{
    RSDK_THIS(DERobot);
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    DERobot_Unknown2(0);
    DERobot_Unknown2(3);
    if (entity->parts[5]->onGround) {
        entity->field_B0     = -0x10000;
        entity->state        = DERobot_Unknown35;
        EntityEggman *eggman = (EntityEggman *)entity->parts[2];
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, 2, &eggman->animator, true, 0);
        eggman->state = Eggman_Unknown2;
        Camera_ShakeScreen(0, 0, 8);
        RSDK.PlaySFX(DERobot->sfxLedgeBreak, 0, 255);
    }
    DERobot_Unknown4(0);
    DERobot_Unknown4(2);
}

void DERobot_Unknown35(void)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)entity->parts[3];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[6];
    part1->angle += entity->field_B0 >> 12;
    if (part1->angle <= 0) {
        entity->field_B0 += 2048;
    }
    else {
        part1->angle         = 0;
        entity->field_B0     = 0;
        entity->field_A8     = 0;
        part1->field_B0      = 0x10000;
        part1->field_B4      = 0;
        part2->field_B0      = 0x10000;
        part2->field_B4      = 0;
        EntityEggman *eggman = (EntityEggman *)entity->parts[2];
        entity->field_D4     = 8;
        entity->field_D8     = 0x10000;
        entity->field_DC     = -0x1000;
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, 3, &eggman->animator, true, 0);
        eggman->state                              = Eggman_Unknown2;
        ((EntityDERobot *)entity->parts[0])->state = DERobot_Unknown27;
        entity->state                              = DERobot_Unknown36;
    }
    part2->angle = part1->angle;
    DERobot_Unknown3(1);
    DERobot_Unknown3(4);
    DERobot_Unknown4(0);
    DERobot_Unknown4(2);
    DERobot_Unknown8();
}

void DERobot_Unknown36(void)
{
    RSDK_THIS(DERobot);
    int v0                = entity->field_A8;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    DERobot_Unknown1();

    ++entity->timer;
    if (entity->timer == 240) {
        EntityDERobot *robotPart = (EntityDERobot *)RSDK.CreateEntity(DERobot->objectID, intToVoid(6), entity->position.x, 0x3080000);
        robotPart->dword70       = (Entity *)player1;
        RSDK.PlaySFX(DERobot->sfxTargeting, 0, 255);
    }
    else if (entity->timer == 340) {
        entity->timer = 0;
        if (entity->offset.x - entity->position.x <= 0xD00000) {
            ((EntityDERobot *)entity->parts[12])->state = DERobot_Unknown19;
            entity->state                               = DERobot_Unknown37;
        }
        else {
            EntityDERobot *robotPart =
                (EntityDERobot *)RSDK.CreateEntity(DERobot->objectID, intToVoid(9), entity->position.x - 0x360000, entity->position.y - 0x60000);
            robotPart->offset.x = (entity->offset.x >> 1) + ((entity->position.x - 0x360000) >> 1);
            robotPart->offset.y = robotPart->position.y;
            robotPart->scale.y  = 0x2000;
            robotPart->scale.x  = (entity->offset.x - entity->position.x + 0x360000) >> 11;
            RSDK.PlaySFX(DERobot->sfxDrop, 0, 255);
        }
    }

    if (entity->field_A8 == 3) {
        entity->parts[9]->angle += (-112 - entity->parts[9]->angle) >> 4;
        entity->parts[10]->angle += (64 - entity->parts[10]->angle) >> 4;
        if (entity->timer >= 240 && player1->position.x - entity->position.x <= 0xD00000) {
            int angle = 4
                        * RSDK.ATan2((player1->position.x - entity->parts[12]->position.x) >> 16,
                                     (player1->position.y - entity->parts[12]->position.y) >> 16);

            int val = angle - 1024;
            if (angle <= 512)
                val = angle;

            val      = clampVal(val, -96, 128);
            int val2 = entity->parts[11]->angle + (-(entity->parts[11]->angle + val) >> 3);
            if (val2 > 128)
                val2 = 128;

            entity->parts[11]->angle = val2;
            entity->parts[12]->angle += (val - entity->parts[12]->angle) >> 3;
        }
        else {
            entity->parts[11]->angle += (112 - entity->parts[11]->angle) >> 4;
            entity->parts[12]->angle += (-32 - entity->parts[12]->angle) >> 4;
        }
    }
    else {
        entity->parts[9]->angle += (112 - entity->parts[9]->angle) >> 4;
        entity->parts[10]->angle += (-32 - entity->parts[10]->angle) >> 4;
        if (entity->timer >= 240 && player1->position.x - entity->position.x <= 0xD00000) {
            int angle = 4
                        * RSDK.ATan2((player1->position.x - entity->parts[12]->position.x) >> 16,
                                     (player1->position.y - entity->parts[12]->position.y) >> 16);
            int val = angle - 1024;
            if (angle <= 512)
                val = angle;

            if (val > 128)
                val = 128;
            int val2 = entity->parts[11]->angle + (-(entity->parts[11]->angle + val) >> 3);
            if (val2 > 128)
                val2 = 128;
            entity->parts[11]->angle = val2;
            entity->parts[12]->angle += (val - entity->parts[12]->angle) >> 3;
        }
        else {
            entity->parts[11]->angle += (-112 - entity->parts[11]->angle) >> 4;
            entity->parts[12]->angle += (64 - entity->parts[12]->angle) >> 4;
        }
    }

    DERobot_Unknown4(0);
    DERobot_Unknown4(2);
    EntityDERobot *part1 = (EntityDERobot *)entity->parts[v0 + 3];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[((v0 + 3) % 6) + 3];
    EntityDERobot *part3 = (EntityDERobot *)entity->parts[((v0 + 3) % 6) + 5];
    part1->angle -= entity->field_D4;
    DERobot_Unknown3(v0 + 1);
    part2->angle += part2->field_B0 >> 12;
    part2->field_B0 += part2->field_B4;
    if (part2->angle > 176)
        part2->field_B4 = entity->field_DC;
    DERobot_Unknown2((v0 + 3) % 6);
    if (part3->onGround && (-part1->angle >> 6) > 0) {
        entity->field_A8 = (entity->field_A8 + 3) % 6;
        part2->field_B0  = entity->field_D8;
        part2->field_B4  = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySFX(DERobot->sfxImpact, 0, 255);
        if (entity->position.x > 0x4D800000) {
            entity->field_C8 = 1;
            DERobot_Unknown7();
            foreach_active(CollapsingPlatform, platform) { platform->playerPos.x = platform->position.x; }
        }
    }
    DERobot_Unknown8();
}

void DERobot_Unknown37(void)
{
    RSDK_THIS(DERobot);
    DERobot_Unknown1();
    EntityDERobot *part9 = (EntityDERobot *)entity->parts[9];
    if (entity->field_A8 == 3) {
        part9->angle += (-112 - part9->angle) >> 4;
        entity->parts[10]->angle += (64 - entity->parts[10]->angle) >> 4;
    }
    else {
        part9->angle += (112 - part9->angle) >> 4;
        entity->parts[10]->angle += (-32 - entity->parts[10]->angle) >> 4;
    }

    if (((EntityDERobot *)entity->parts[12])->state == DERobot_Unknown21)
        entity->state = DERobot_Unknown36;

    DERobot_Unknown4(0);
    DERobot_Unknown4(2);

    EntityDERobot *part3 = (EntityDERobot *)entity->parts[entity->field_A8 + 3];
    part3->angle -= entity->field_D4;
    DERobot_Unknown3(entity->field_A8 + 1);

    EntityDERobot *part1 = (EntityDERobot *)entity->parts[((entity->field_A8 + 3) % 6) + 3];
    EntityDERobot *part2 = (EntityDERobot *)entity->parts[((entity->field_A8 + 3) % 6) + 5];

    part1->angle += part1->field_B0 >> 12;
    part1->field_B0 += part1->field_B4;
    if (part1->angle > 176)
        part1->field_B4 = entity->field_DC;

    DERobot_Unknown2((entity->field_A8 + 3) % 6);
    if (part2->onGround && (-part3->angle >> 6) > 0) {
        entity->field_A8 = (entity->field_A8 + 3) % 6;
        part1->field_B0  = entity->field_D8;
        part1->field_B4  = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySFX(DERobot->sfxImpact, 0, 255);
        if (entity->position.x > 0x4E000000) {
            entity->field_C8 = 1;
            DERobot_Unknown7();
        }
    }
    DERobot_Unknown8();
}

void DERobot_Unknown38(void)
{
    RSDK_THIS(DERobot);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySFX(DERobot->sfxExplosion, 0, 255);
        if ((Zone->timer & 4)) {
            EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, intToVoid((RSDK.Rand(0, 256) > 192) + 2),
                                                                              (RSDK.Rand(-48, 48) << 16) + entity->position.x,
                                                                              (RSDK.Rand(-48, 48) << 16) + entity->position.y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 90) {
        foreach_active(DERobot, robot)
        {
            switch (robot->aniID) {
                case 0: break;
                case 1:
                    robot->velocity.x = -0x10000;
                    robot->velocity.y = -0x40000;
                    robot->field_B0   = -8;
                    robot->drawFX     = FX_ROTATE;
                    robot->state      = DERobot_Unknown41;
                    robot->stateDraw  = DERobot_StateDraw_Unknown3;
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    robot->velocity.x = RSDK.Rand(-0x40000, 0x40000);
                    robot->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                    robot->field_B0   = RSDK.Rand(-16, 16);
                    robot->drawFX     = FX_ROTATE;
                    robot->state      = DERobot_Unknown41;
                    if (robot->stateDraw == DERobot_StateDraw_Unknown1)
                        robot->stateDraw = DERobot_StateDraw_Unknown3;
                    break;
            }
        }

        EntityEggman *eggman = (EntityEggman *)entity->parts[2];
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, 4, &eggman->animator, true, 0);
        entity->timer      = 0;
        entity->offset.x   = 0;
        entity->velocity.y = -0x40000;
        entity->state      = DERobot_Unknown39;
        RSDK.PlaySFX(DERobot->sfxLedgeBreak, 0, 255);
    }
}

void DERobot_Unknown39(void)
{
    RSDK_THIS(DERobot);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySFX(DERobot->sfxExplosion, 0, 255);
        if ((Zone->timer & 4)) {
            EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, intToVoid((RSDK.Rand(0, 256) > 192) + 2),
                                                                              (RSDK.Rand(-48, 48) << 16) + entity->position.x,
                                                                              (RSDK.Rand(-48, 48) << 16) + entity->position.y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        DERobot_Unknown6();
        RSDK.PlaySFX(DERobot->sfxLedgeBreak, 0, 255);
        entity->state      = DERobot_Unknown40;
        entity->velocity.y = -0x10000 - (entity->velocity.y >> 1);
        Camera_ShakeScreen(0, 0, 8);
    }

    if (entity->offset.x < 0x480000)
        entity->offset.x += 0x40000;

    foreach_active(Player, player)
    {
        if (player->position.x < entity->offset.x + entity->position.x) {
            player->position.x = entity->offset.x + entity->position.x;
            player->velocity.x = 0x40000;
            player->groundVel  = 0x40000;
        }
    }
}

void DERobot_Unknown40(void)
{
    RSDK_THIS(DERobot);
    entity->velocity.y += 0x2000;
    entity->position.y += entity->velocity.y;
    if (++entity->timer == 96)
        RSDK.PlaySFX(DERobot->sfxDrop, 0, 255);

    if (entity->timer == 180) {
        if (Music_CheckMusicStack()) {
            Music->nextTrack = 0;
        }
        else {
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
        }

        EntityEggPrison *prison = (EntityEggPrison *)entity->parts[2];
        RSDK.ResetEntityPtr(prison, EggPrison->objectID, intToVoid(1));
        prison->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        prison->field_70   = 1;
        prison->position.y = (RSDK_screens->position.y - 48) << 16;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        entity->state = DERobot_Unknown42;
    }

    if (!(Zone->timer & 7)) {
        EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(
            Explosion->objectID, intToVoid(3), (RSDK.Rand(-48, 48) << 16) + entity->position.x, (RSDK.Rand(-48, 48) << 16) + entity->position.y);
        explosion->drawOrder = Zone->drawOrderHigh;
    }

    foreach_active(Player, player)
    {
        if (player->position.x < entity->position.x + 0x480000) {
            player->position.x = entity->position.x + 0x480000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
    }
}

void DERobot_Unknown41(void)
{
    RSDK_THIS(DERobot);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->rotation += entity->field_B0;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void DERobot_Unknown42(void)
{
    RSDK_THIS(DERobot);

    foreach_active(Player, player)
    {
        if (player->position.x < entity->position.x + 0x480000) {
            player->position.x = entity->position.x + 0x480000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
        if (player->position.x > 0x4E800000) {
            player->position.x = 0x4E800000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
    }
}

void DERobot_Unknown43(void)
{
    RSDK_THIS(DERobot);
    if (!(Zone->timer & 0x3F)) {
        EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(
            Explosion->objectID, intToVoid(3), entity->position.x + (RSDK.Rand(-32, 32) << 16), entity->position.y + (RSDK.Rand(-64, 0) << 16));
        explosion->drawOrder = Zone->drawOrderHigh;
    }
}

void DERobot_EditorDraw(void) {}

void DERobot_EditorLoad(void) {}

void DERobot_Serialize(void)
{
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, aniID);
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, frameID);
}

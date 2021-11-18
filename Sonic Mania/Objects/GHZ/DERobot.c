#include "SonicMania.h"

ObjectDERobot *DERobot;

void DERobot_Update(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(self->state);
}

void DERobot_LateUpdate(void) {}

void DERobot_StaticUpdate(void) {}

void DERobot_Draw(void)
{
    RSDK_THIS(DERobot);
    StateMachine_Run(self->stateDraw);
}

void DERobot_Create(void *data)
{
    RSDK_THIS(DERobot);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->drawOrder     = Zone->drawOrderLow;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            int32 slotID            = RSDK.GetEntityID(self);
            if (data)
                self->aniID = voidToInt(data);
            switch (self->aniID) {
                case DEROBOT_BODY:
                    self->active    = ACTIVE_BOUNDS;
                    self->parts[3]  = RSDK_GET_ENTITY(slotID - 7, );
                    self->parts[4]  = RSDK_GET_ENTITY(slotID - 6, );
                    self->parts[5]  = RSDK_GET_ENTITY(slotID - 5, );
                    self->parts[9]  = RSDK_GET_ENTITY(slotID - 4, );
                    self->parts[10] = RSDK_GET_ENTITY(slotID - 3, );
                    self->parts[2]  = RSDK_GET_ENTITY(slotID - 2, );
                    self->parts[0]  = RSDK_GET_ENTITY(slotID - 1, );
                    self->parts[6]  = RSDK_GET_ENTITY(slotID + 1, );
                    self->parts[7]  = RSDK_GET_ENTITY(slotID + 2, );
                    self->parts[8]  = RSDK_GET_ENTITY(slotID + 3, );
                    self->parts[1]  = RSDK_GET_ENTITY(slotID + 4, );
                    self->parts[11] = RSDK_GET_ENTITY(slotID + 5, );
                    self->parts[12] = RSDK_GET_ENTITY(slotID + 6, );
                    self->field_C8  = 8;
                    self->state     = DERobot_Unknown31;
                    self->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_HEAD:
                    self->dword70   = RSDK.GetEntityByID(slotID + 1);
                    self->stateDraw = DERobot_StateDraw_Unknown1;
                    self->drawFX    = FX_ROTATE;
                    self->offset.x  = -0x160000;
                    self->offset.y  = -0x240000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_ARM:
                    if (self->frameID == 2) {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 5, &self->animator2, true, 0);
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, 4, &self->animator3, true, 0);
                        self->stateDraw = DERobot_StateDraw_Unknown2;
                        self->drawFX    = FX_ROTATE;
                        self->state     = DERobot_Unknown18;
                    }
                    else if (self->frameID) {
                        self->stateDraw = DERobot_StateDraw_Unknown3;
                    }
                    else {
                        self->dword70   = RSDK.GetEntityByID(slotID - 4);
                        self->stateDraw = DERobot_StateDraw_Unknown1;
                        self->offset.x  = -0xC0000;
                        self->offset.y  = -0x100000;
                    }
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_LEG:
                    if (self->frameID) {
                        self->stateDraw = DERobot_StateDraw_Unknown3;
                    }
                    else {
                        RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator2, true, 1);
                        self->stateDraw = DERobot_StateDraw_Unknown4;
                    }
                    if (self->frameID > 1)
                        self->drawFX = FX_ROTATE;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_TARGET_EDGE:
                    self->active     = ACTIVE_NORMAL;
                    self->visible    = true;
                    self->drawFX     = FX_FLIP;
                    self->drawOrder  = Zone->drawOrderHigh;
                    self->velocity.x = 0x20000;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, 7, &self->animator2, true, 0);
                    self->state     = DERobot_Unknown33;
                    self->stateDraw = DERobot_StateDraw_Unknown5;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_BOMB:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->state     = DERobot_Unknown28;
                    self->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                case DEROBOT_BODY_CUTSCENE:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->stateDraw = DERobot_StateDraw_Unknown3;
                    self->parts[1]  = RSDK.GetEntityByID(slotID + 4);
                    self->parts[9]  = RSDK.GetEntityByID(slotID + 1);
                    self->parts[10] = RSDK.GetEntityByID(slotID + 2);
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
                default:
                    self->stateDraw = DERobot_StateDraw_Unknown3;
                    RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
                    break;
            }
        }
        else {
            destroyEntity(self);
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
    int32 x = (self->position.x >> 16) - ScreenInfo->centerX + 128;
    if (x > Zone->screenBoundsL1[0]) {
        Zone->screenBoundsL1[0]                        = x;
        Zone->screenBoundsR1[0]                        = ScreenInfo->width + 96 + x;
        Zone->screenBoundsL2[0]                        = Zone->screenBoundsL1[0] << 16;
        Zone->screenBoundsR2[0]                        = Zone->screenBoundsR1[0] << 16;
        Zone->playerBoundActiveB[0]                    = 0;
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsL = Zone->screenBoundsL1[0];
    }
}

void DERobot_Unknown2(int32 a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)self->parts[a1 + 3];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[a1 + 4];
    EntityDERobot *part3 = (EntityDERobot *)self->parts[a1 + 5];

    part1->position.x = 0x440 * RSDK.Sin1024(part1->angle) + self->position.x;
    part1->position.y = 0x440 * RSDK.Cos1024(part1->angle) + self->position.y + 0x190000;
    part2->position.x = part1->position.x;
    part2->position.y = part1->position.y;
    part2->angle      = maxVal(part1->angle >> 2, 0);
    part2->rotation   = -(part2->angle >> 1);
    part3->position.x = 0xA00 * RSDK.Sin1024(part2->angle) + part2->position.x;
    part3->position.y = 0xA00 * RSDK.Cos1024(part2->angle) + part2->position.y;
    part3->onGround   = false;
    int32 storeX        = part3->position.x;
    int32 storeY        = part3->position.y;
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

void DERobot_Unknown3(int32 a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)self->parts[a1 + 3];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[a1 + 4];
    EntityDERobot *part3 = (EntityDERobot *)self->parts[a1 + 2];
    part1->position.x    = part2->position.x - 0xA00 * RSDK.Sin1024(part1->angle);
    part1->position.y    = part2->position.y - 0xA00 * RSDK.Cos1024(part1->angle);
    part1->angle -= part1->angle >> 3;
    part1->rotation -= part1->rotation >> 3;
    part3->position.x  = part1->position.x;
    part3->position.y  = part1->position.y;
    self->position.x = part3->position.x - 0x440 * RSDK.Sin1024(part3->angle);
    self->position.y = part3->position.y - (0x440 * RSDK.Cos1024(part3->angle)) - 0x190000;
}

void DERobot_Unknown4(int32 a1)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)self->parts[a1 + 9];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[a1 + 10];
    part1->position.x    = 0x600 * (RSDK.Sin1024(part1->angle) - 0x200) + self->position.x;
    part1->position.y    = 0x600 * (RSDK.Cos1024(part1->angle) - 0x300) + self->position.y;
    part2->position.x    = part1->position.x + 0x600 * RSDK.Cos1024(minVal(part1->angle, 0));
    part2->position.y    = part1->position.y - 0x600 * RSDK.Sin1024(minVal(part1->angle, 0));
}

void DERobot_Unknown5(void)
{
    RSDK_THIS(DERobot);
    int32 tx = (self->position.x + 0xC0000) >> 20;
    if (tx > self->field_D0) {
        self->field_D0 = tx;
        int32 ty           = (self->position.y >> 20) - 16;
        int32 spawnY       = (ty << 20) + 0x80000;

        bool32 playSFX = false;
        for (int32 i = 0; i < 32; ++i) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), (tx << 20) + 0x80000, spawnY);
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
            RSDK.PlaySfx(DERobot->sfxLedgeBreak, 0, 255);
    }
}

void DERobot_Unknown6(void)
{
    RSDK_THIS(DERobot);

    int32 tx     = (self->position.x >> 20) - 4;
    int32 ty     = (self->position.y >> 20) - 16;
    int32 spawnX = (tx << 20) + 0x80000;
    for (int32 y = 0; y < 8; ++y) {
        int32 spawnY = (ty << 20) + 0x80000;
        for (int32 x = 0; x < 32; ++x) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), spawnX, spawnY);
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

    tx     = (self->position.x >> 20) - 4;
    ty     = (self->position.y >> 20) - 16;
    spawnX = (tx << 20) + 0x80000;
    for (int32 y = 0; y < 32; ++y) {
        int32 spawnY = (ty << 20) + 0x80000;
        for (int32 x = 0; x < 32; ++x) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
            if (tile != 0xFFFF) {
                RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                EntityBreakableWall *wall = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), spawnX, spawnY);
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
    if (--self->field_C8 <= 0) {
        self->timer               = 0;
        self->state               = DERobot_Unknown38;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        ((EntityDERobot *)self->parts[10])->state = DERobot_Unknown22;
        ((EntityDERobot *)self->parts[12])->state = DERobot_Unknown22;
    }
    else {
        self->field_C4 = 48;
        RSDK.PlaySfx(DERobot->sfxHit, 0, 255);
        if (self->field_C8 == 2) {
            self->field_D4 = 16;
            self->field_D8 = 0x20000;
            self->field_DC = -0x4000;
            ((EntityDERobot *)self->parts[3])->field_B0 *= 2;
            ((EntityDERobot *)self->parts[3])->field_B4 *= 4;
            ((EntityDERobot *)self->parts[6])->field_B0 *= 2;
            ((EntityDERobot *)self->parts[6])->field_B4 *= 4;
        }
    }
}

void DERobot_Unknown8(void)
{
    RSDK_THIS(DERobot);

    if (self->field_C4 > 0) {
        self->field_C4--;
        if ((self->field_C4 & 2)) {
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
        if (!self->field_C4 && Player_CheckBadnikTouch(player, self, &DERobot->hitbox) && Player_CheckBossHit(player, self)) {
            DERobot_Unknown7();
            RSDK.PlaySfx(DERobot->sfxHit, 0, 255);
            player->velocity.x = 0x60000;
        }
    }
}

void DERobot_Unknown9(void)
{
    RSDK_THIS(DERobot);
    int32 val = (self->field_CC + 22) << 16;
    val >>= 8;
    int32 angle            = -(self->angle >> 3);
    int32 val1             = (-0xE00 * RSDK.Sin256(angle)) + (0x1600 * RSDK.Cos256(angle));
    int32 val2             = (-0xE00 * RSDK.Cos256(angle)) - (0x1600 * RSDK.Sin256(angle));
    int32 val3             = (-0xE00 * RSDK.Sin256(angle)) + val * RSDK.Cos256(angle);
    int32 val4             = (((-0xE00 * RSDK.Cos256(angle)) - (val * RSDK.Sin256(angle))) >> 16) - (val2 >> 16);
    self->hitbox.left  = (val1 >> 16);
    self->hitbox.right = (val3 >> 16);
    foreach_active(Player, player)
    {
        int32 dist = (player->position.x - self->position.x) >> 16;
        if (dist >= val1 >> 16) {
            if (dist > (val3 >> 16))
                dist = (val3 >> 16);
        }
        else {
            dist = val1 >> 16;
        }
        self->hitbox.top    = (val2 >> 16) + val4 * (dist - (val1 >> 16)) / ((val3 >> 16) - (val1 >> 16));
        self->hitbox.bottom = self->hitbox.top + 16;
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
            player->position.y += 0x40000;
    }
}

void DERobot_Unknown10(void)
{
    RSDK_THIS(DERobot);
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    Vector2 pos;
    pos.x = self->position.x;
    pos.y = self->position.y;
    self->position.x += (self->field_CC + 35) << 16;
    Zone_RotateOnPivot(&self->position, &pos, -(self->angle >> 3));

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &DERobot->hitbox2)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer))
#endif
            Player_CheckHit(player, self);
        }
    }

    DERobot_Unknown5();
    self->position.x = storeX;
    self->position.y = storeY;
}

bool32 DERobot_Unknown11(void)
{
    RSDK_THIS(DERobot);
    bool32 flag = false;
    int32 storeX  = self->position.x;
    int32 storeY  = self->position.y;
    Vector2 pos;
    pos.x = self->position.x;
    pos.y = self->position.y;
    self->position.x += (self->field_CC + 48) << 16;
    Zone_RotateOnPivot(&self->position, &pos, -(self->angle >> 3));

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(self->position.x - ruby->position.x) < 0x80000) {
            if (abs(self->position.y - ruby->position.y) < 0x80000) {
                flag             = true;
                ruby->position.x = self->position.x;
                ruby->position.y = self->position.y;
                ruby->drawOrder  = Zone->drawOrderLow;
                ruby->state      = PhantomRuby_Unknown10;
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    return flag;
}

void DERobot_Unknown12(void)
{
    RSDK_THIS(DERobot);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &DERobot->hitbox2)) {
            Player_CheckHit(player, self);
        }
    }
}

void DERobot_StateDraw_Unknown1(void)
{
    RSDK_THIS(DERobot);
    self->position.x = self->dword70->position.x;
    self->position.y = self->dword70->position.y;
    self->position.x += self->offset.x;
    self->position.y += self->offset.y;
    RSDK.DrawSprite(&self->animator1, 0, false);
}

void DERobot_StateDraw_Unknown2(void)
{
    RSDK_THIS(DERobot);
    self->rotation = self->angle >> 2;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (self->field_CC > 0) {
        SpriteFrame *frame = RSDK.GetFrame(DERobot->aniFrames, self->animator2.animationID, self->animator2.frameID);
        frame->width       = self->field_CC;
        frame->sprX        = 4 * (Zone->timer & 3);
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }

    RSDK.GetFrame(DERobot->aniFrames, self->animator3.animationID, self->animator3.frameID)->pivotX = self->field_CC + 22;
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void DERobot_StateDraw_Unknown3(void)
{
    RSDK_THIS(DERobot);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void DERobot_StateDraw_Unknown4(void)
{
    RSDK_THIS(DERobot);

    self->drawFX   = FX_NONE;
    self->rotation = -(self->angle >> 1);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void DERobot_StateDraw_Unknown5(void)
{
    RSDK_THIS(DERobot);
    Vector2 drawPos;

    self->animator1.frameID = 0;
    self->inkEffect         = INK_ALPHA;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x;
    drawPos.y                 = self->position.y;
    drawPos.x -= self->offset.x;
    drawPos.y -= self->offset.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;
    drawPos.x += self->offset.x;
    drawPos.y -= self->offset.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 1;
    self->direction         = FLIP_NONE;
    drawPos.x                 = self->position.x;
    drawPos.y                 = self->position.y;
    drawPos.x -= self->offset.x;
    drawPos.y += self->offset.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_X;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;
    drawPos.x += self->offset.x;
    drawPos.y += self->offset.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void DERobot_Unknown18(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator3);
    if (self->animator3.animationSpeed > 0x40)
        self->animator3.animationSpeed -= 4;
    DERobot_Unknown10();
}

void DERobot_Unknown19(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator3);
    if (self->animator3.animationSpeed == 240)
        RSDK.PlaySfx(DERobot->sfxBuzzsaw, 0, 255);

    if (self->animator3.animationSpeed >= 0x100u)
        self->state = DERobot_Unknown20;
    else
        self->animator3.animationSpeed += 4;
    DERobot_Unknown10();
}

void DERobot_Unknown20(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    if (self->field_CC < 192)
        self->field_CC += 8;
    DERobot_Unknown9();
    DERobot_Unknown10();
    if (++self->timer == 120) {
        self->timer = 0;
        self->state = DERobot_Unknown21;
    }
}

void DERobot_Unknown21(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    if (self->field_CC <= 0) {
        self->state = DERobot_Unknown18;
    }
    else {
        DERobot_Unknown9();
        self->field_CC -= 8;
    }
    DERobot_Unknown10();
}

void DERobot_Unknown22(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->field_CC > 0) {
        self->field_CC -= 8;
    }
}

void DERobot_Unknown23(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->angle > -96)
        self->angle -= 4;

    if (self->animator3.animationSpeed < 0x80) {
        self->animator3.animationSpeed += 4;
    }

    if (self->timer++ == -1)
        RSDK.PlaySfx(DERobot->sfxButton2, 0, 255);

    if (self->timer == 30) {
        self->timer = 0;
        self->state = DERobot_Unknown24;
        RSDK.PlaySfx(DERobot->sfxBuzzsaw, 0, 255);
    }
}

void DERobot_Unknown24(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (DERobot_Unknown11() || self->field_CC >= 192) {
        self->state = DERobot_Unknown25;
    }
    else {
        self->field_CC += 8;
    }
}

void DERobot_Unknown25(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->field_CC <= 0) {
        RSDK.StopSFX(DERobot->sfxBuzzsaw);
        self->state = DERobot_Unknown26;
    }
    else {
        self->field_CC -= 8;
        DERobot_Unknown11();
    }
}

void DERobot_Unknown26(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->animator3.animationSpeed)
        self->animator3.animationSpeed--;
}

void DERobot_Unknown27(void)
{
    RSDK_THIS(DERobot);

    self->timer++;
    if (self->timer == 30) {
        RSDK.PlaySfx(DERobot->sfxButton2, 0, 255);
    }
    if (self->timer >= 30) {
        if (self->rotation >= 0) {
            self->timer  = 0;
            self->state  = 0;
            self->drawFX = FX_NONE;
            RSDK.PlaySfx(DERobot->sfxHullClose, 0, 255);
        }
        else {
            self->rotation += 8;
        }
    }
}

void DERobot_Unknown28(void)
{
    RSDK_THIS(DERobot);
    self->angle += 7;
    if (self->angle > 480) {
        self->velocity.y += 0x4000;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }
    else {
        self->velocity.x = self->position.x;
        self->position.x = self->offset.x - self->scale.x * RSDK.Cos1024(self->angle);
        self->velocity.x = self->position.x - self->velocity.x;
        self->velocity.y = self->position.y;
        self->position.y = self->offset.y - self->scale.y * RSDK.Sin1024(self->angle);
        self->velocity.y = self->position.y - self->velocity.y;
    }
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xE0000, true))
        self->state = DERobot_Unknown29;
    DERobot_Unknown12();
}

void DERobot_Unknown29(void)
{
    RSDK_THIS(DERobot);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.animationSpeed >= 0x80) {
        self->visible = false;
        self->state   = DERobot_Unknown30;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y - 0x80000);
        explosion->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(DERobot->sfxExplosion, false, 255);
    }
    else {
        self->animator1.animationSpeed++;
    }
    DERobot_Unknown12();
}

void DERobot_Unknown30(void)
{
    RSDK_THIS(DERobot);
    ++self->timer;
    self->position.y -= 0x20000;
    if (self->timer < 16)
        DERobot_Unknown12();

    if (!(self->timer & 7)) {
        EntityExplosion *explosion = (EntityExplosion *)RSDK.CreateEntity(Explosion->objectID, intToVoid(3), self->position.x, self->position.y);
        explosion->drawOrder       = Zone->drawOrderHigh;
    }

    if (self->timer == 32)
        destroyEntity(self);
}

void DERobot_Unknown31(void)
{
    RSDK_THIS(DERobot);
    if (++self->timer >= 8) {
        self->timer         = 0;
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.y <= self->position.y + 0x200000 && player1->state != Player_State_ForceRoll_Ground) {
            for (int32 i = 0; i < Player->playerCount; ++i) {
                Zone->screenBoundsL1[i]     = (self->position.x >> 16) - ScreenInfo->centerX + 128;
                Zone->screenBoundsR1[i]     = ScreenInfo->centerX + 128 + (self->position.x >> 16);
                Zone->screenBoundsB1[i]     = self->position.y >> 16;
                Zone->screenBoundsB2[i]     = Zone->screenBoundsB1[i] << 16;
                Zone->playerBoundActiveL[i] = true;
                Zone->playerBoundActiveR[i] = true;
                Zone->playerBoundActiveB[i] = false;
            }
            self->position.y -= 0x1800000;
            self->active = ACTIVE_NORMAL;
            self->state  = DERobot_Unknown32;
        }
    }
}

void DERobot_Unknown32(void)
{
    RSDK_THIS(DERobot);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->timer) {
        self->timer++;
        if (self->timer == 60) {
            CREATE_ENTITY(DERobot, intToVoid(6), self->position.x, 0x3080000);
            RSDK.PlaySfx(DERobot->sfxTargeting, 0, 255);
            Music_TransitionTrack(TRACK_EGGMAN1, 0.125);
        }

        if (self->timer == 160) {
            self->timer   = 0;
            self->visible = true;
            self->state   = DERobot_Unknown34;
            if (player1->characterID == ID_TAILS)
                player1->movesetState = Player_TailsJumpAbility;
            EntityDERobot *part = (EntityDERobot *)self->parts[3];
            part->active        = ACTIVE_NORMAL;
            part->visible       = true;

            part          = (EntityDERobot *)self->parts[4];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[5];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[6];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[7];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[8];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part           = (EntityDERobot *)self->parts[0];
            part->active   = ACTIVE_NORMAL;
            part->visible  = true;
            part->rotation = -128;

            part          = (EntityDERobot *)self->parts[1];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[9];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[10];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[11];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            part          = (EntityDERobot *)self->parts[12];
            part->active  = ACTIVE_NORMAL;
            part->visible = true;

            EntityEggman *eggman = (EntityEggman *)self->parts[2];
            RSDK.ResetEntityPtr(eggman, Eggman->objectID, self);
            eggman->state    = Eggman_Unknown1;
            eggman->animID   = 0;
            eggman->offset.x = -0x40000;
            eggman->offset.y = -0x200000;
        }
    }
    else if (player1->position.x > 0x800000 + self->position.x && player1->onGround) {
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->boundsOffset.x = 1;
        ++self->timer;
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
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->dword70) {
        self->position.x = self->dword70->position.x;
        self->position.y = self->dword70->position.y;
    }

    self->alpha += 32;
    self->offset.x -= self->velocity.x;
    if (self->offset.x <= 0xC0000) {
        self->alpha    = 0;
        self->offset.x = 0x2C0000;
    }

    self->offset.y = self->offset.x;
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(DERobot->aniFrames, 8, &self->animator3, true, 0);
    if (self->timer == 96) {
        self->dword70 = 0;
        foreach_active(DERobot, robot)
        {
            if (!robot->aniID) {
                robot->offset.x = self->position.x;
                robot->offset.y = self->position.y;
            }
        }
    }
    if (self->timer == 128)
        destroyEntity(self);
}

void DERobot_Unknown34(void)
{
    RSDK_THIS(DERobot);
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    DERobot_Unknown2(0);
    DERobot_Unknown2(3);
    if (self->parts[5]->onGround) {
        self->field_B0     = -0x10000;
        self->state        = DERobot_Unknown35;
        EntityEggman *eggman = (EntityEggman *)self->parts[2];
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 2, &eggman->animator, true, 0);
        eggman->state = Eggman_Unknown2;
        Camera_ShakeScreen(0, 0, 8);
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, 0, 255);
    }
    DERobot_Unknown4(0);
    DERobot_Unknown4(2);
}

void DERobot_Unknown35(void)
{
    RSDK_THIS(DERobot);
    EntityDERobot *part1 = (EntityDERobot *)self->parts[3];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[6];
    part1->angle += self->field_B0 >> 12;
    if (part1->angle <= 0) {
        self->field_B0 += 2048;
    }
    else {
        part1->angle         = 0;
        self->field_B0     = 0;
        self->field_A8     = 0;
        part1->field_B0      = 0x10000;
        part1->field_B4      = 0;
        part2->field_B0      = 0x10000;
        part2->field_B4      = 0;
        EntityEggman *eggman = (EntityEggman *)self->parts[2];
        self->field_D4     = 8;
        self->field_D8     = 0x10000;
        self->field_DC     = -0x1000;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 3, &eggman->animator, true, 0);
        eggman->state                              = Eggman_Unknown2;
        ((EntityDERobot *)self->parts[0])->state = DERobot_Unknown27;
        self->state                              = DERobot_Unknown36;
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
    int32 id                = self->field_A8;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    DERobot_Unknown1();

    ++self->timer;
    if (self->timer == 240) {
        EntityDERobot *robotPart = CREATE_ENTITY(DERobot, intToVoid(6), self->position.x, 0x3080000);
        robotPart->dword70       = (Entity *)player1;
        RSDK.PlaySfx(DERobot->sfxTargeting, 0, 255);
    }
    else if (self->timer == 340) {
        self->timer = 0;
        if (self->offset.x - self->position.x <= 0xD00000) {
            ((EntityDERobot *)self->parts[12])->state = DERobot_Unknown19;
            self->state                               = DERobot_Unknown37;
        }
        else {
            EntityDERobot *robotPart = CREATE_ENTITY(DERobot, intToVoid(9), self->position.x - 0x360000, self->position.y - 0x60000);
            robotPart->offset.x = (self->offset.x >> 1) + ((self->position.x - 0x360000) >> 1);
            robotPart->offset.y = robotPart->position.y;
            robotPart->scale.y  = 0x2000;
            robotPart->scale.x  = (self->offset.x - self->position.x + 0x360000) >> 11;
            RSDK.PlaySfx(DERobot->sfxDrop, 0, 255);
        }
    }

    if (self->field_A8 == 3) {
        self->parts[9]->angle += (-112 - self->parts[9]->angle) >> 4;
        self->parts[10]->angle += (64 - self->parts[10]->angle) >> 4;
        if (self->timer >= 240 && player1->position.x - self->position.x <= 0xD00000) {
            int32 angle = 4
                        * RSDK.ATan2((player1->position.x - self->parts[12]->position.x) >> 16,
                                     (player1->position.y - self->parts[12]->position.y) >> 16);

            int32 val = angle - 1024;
            if (angle <= 512)
                val = angle;

            val      = clampVal(val, -96, 128);
            int32 val2 = self->parts[11]->angle + (-(self->parts[11]->angle + val) >> 3);
            if (val2 > 128)
                val2 = 128;

            self->parts[11]->angle = val2;
            self->parts[12]->angle += (val - self->parts[12]->angle) >> 3;
        }
        else {
            self->parts[11]->angle += (112 - self->parts[11]->angle) >> 4;
            self->parts[12]->angle += (-32 - self->parts[12]->angle) >> 4;
        }
    }
    else {
        self->parts[9]->angle += (112 - self->parts[9]->angle) >> 4;
        self->parts[10]->angle += (-32 - self->parts[10]->angle) >> 4;
        if (self->timer >= 240 && player1->position.x - self->position.x <= 0xD00000) {
            int32 angle = 4
                        * RSDK.ATan2((player1->position.x - self->parts[12]->position.x) >> 16,
                                     (player1->position.y - self->parts[12]->position.y) >> 16);
            int32 val = angle - 1024;
            if (angle <= 512)
                val = angle;

            if (val > 128)
                val = 128;
            int32 val2 = self->parts[11]->angle + (-(self->parts[11]->angle + val) >> 3);
            if (val2 > 128)
                val2 = 128;
            self->parts[11]->angle = val2;
            self->parts[12]->angle += (val - self->parts[12]->angle) >> 3;
        }
        else {
            self->parts[11]->angle += (-112 - self->parts[11]->angle) >> 4;
            self->parts[12]->angle += (64 - self->parts[12]->angle) >> 4;
        }
    }

    DERobot_Unknown4(0);
    DERobot_Unknown4(2);
    EntityDERobot *part1 = (EntityDERobot *)self->parts[id + 3];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[((id + 3) % 6) + 3];
    EntityDERobot *part3 = (EntityDERobot *)self->parts[((id + 3) % 6) + 5];
    part1->angle -= self->field_D4;
    DERobot_Unknown3(id + 1);
    part2->angle += part2->field_B0 >> 12;
    part2->field_B0 += part2->field_B4;
    if (part2->angle > 176)
        part2->field_B4 = self->field_DC;
    DERobot_Unknown2((id + 3) % 6);
    if (part3->onGround && (-part1->angle >> 6) > 0) {
        self->field_A8 = (self->field_A8 + 3) % 6;
        part2->field_B0  = self->field_D8;
        part2->field_B4  = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(DERobot->sfxImpact, 0, 255);
        if (self->position.x > 0x4D800000) {
            self->field_C8 = 1;
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
    EntityDERobot *part9 = (EntityDERobot *)self->parts[9];
    if (self->field_A8 == 3) {
        part9->angle += (-112 - part9->angle) >> 4;
        self->parts[10]->angle += (64 - self->parts[10]->angle) >> 4;
    }
    else {
        part9->angle += (112 - part9->angle) >> 4;
        self->parts[10]->angle += (-32 - self->parts[10]->angle) >> 4;
    }

    if (((EntityDERobot *)self->parts[12])->state == DERobot_Unknown21)
        self->state = DERobot_Unknown36;

    DERobot_Unknown4(0);
    DERobot_Unknown4(2);

    EntityDERobot *part3 = (EntityDERobot *)self->parts[self->field_A8 + 3];
    part3->angle -= self->field_D4;
    DERobot_Unknown3(self->field_A8 + 1);

    EntityDERobot *part1 = (EntityDERobot *)self->parts[((self->field_A8 + 3) % 6) + 3];
    EntityDERobot *part2 = (EntityDERobot *)self->parts[((self->field_A8 + 3) % 6) + 5];

    part1->angle += part1->field_B0 >> 12;
    part1->field_B0 += part1->field_B4;
    if (part1->angle > 176)
        part1->field_B4 = self->field_DC;

    DERobot_Unknown2((self->field_A8 + 3) % 6);
    if (part2->onGround && (-part3->angle >> 6) > 0) {
        self->field_A8 = (self->field_A8 + 3) % 6;
        part1->field_B0  = self->field_D8;
        part1->field_B4  = 0;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(DERobot->sfxImpact, 0, 255);
        if (self->position.x > 0x4E000000) {
            self->field_C8 = 1;
            DERobot_Unknown7();
        }
    }
    DERobot_Unknown8();
}

void DERobot_Unknown38(void)
{
    RSDK_THIS(DERobot);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(DERobot->sfxExplosion, false, 255);
        if ((Zone->timer & 4)) {
            int x = self->position.x + (RSDK.Rand(-48, 48) << 16);
            int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 90) {
        foreach_active(DERobot, robot)
        {
            switch (robot->aniID) {
                case DEROBOT_BODY: break;
                case DEROBOT_HEAD:
                    robot->velocity.x = -0x10000;
                    robot->velocity.y = -0x40000;
                    robot->field_B0   = -8;
                    robot->drawFX     = FX_ROTATE;
                    robot->state      = DERobot_Unknown41;
                    robot->stateDraw  = DERobot_StateDraw_Unknown3;
                    break;
                case DEROBOT_ARM:
                case DEROBOT_LEG:
                case DEROBOT_SPIKES:
                case DEROBOT_ARM_THREAD:
                case DEROBOT_TARGET_EDGE:
                case DEROBOT_TARGET_CENTER:
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

        EntityEggman *eggman = (EntityEggman *)self->parts[2];
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 4, &eggman->animator, true, 0);
        self->timer      = 0;
        self->offset.x   = 0;
        self->velocity.y = -0x40000;
        self->state      = DERobot_Unknown39;
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, 0, 255);
    }
}

void DERobot_Unknown39(void)
{
    RSDK_THIS(DERobot);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(DERobot->sfxExplosion, 0, 255);
        if ((Zone->timer & 4)) {
            int x                      = self->position.x + (RSDK.Rand(-48, 48) << 16);
            int y                      = self->position.y + (RSDK.Rand(-48, 48) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        DERobot_Unknown6();
        RSDK.PlaySfx(DERobot->sfxLedgeBreak, 0, 255);
        self->state      = DERobot_Unknown40;
        self->velocity.y = -0x10000 - (self->velocity.y >> 1);
        Camera_ShakeScreen(0, 0, 8);
    }

    if (self->offset.x < 0x480000)
        self->offset.x += 0x40000;

    foreach_active(Player, player)
    {
        if (player->position.x < self->offset.x + self->position.x) {
            player->position.x = self->offset.x + self->position.x;
            player->velocity.x = 0x40000;
            player->groundVel  = 0x40000;
        }
    }
}

void DERobot_Unknown40(void)
{
    RSDK_THIS(DERobot);
    self->velocity.y += 0x2000;
    self->position.y += self->velocity.y;
    if (++self->timer == 96)
        RSDK.PlaySfx(DERobot->sfxDrop, 0, 255);

    if (self->timer == 180) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        EntityEggPrison *prison = (EntityEggPrison *)self->parts[2];
        RSDK.ResetEntityPtr(prison, EggPrison->objectID, intToVoid(1));
        prison->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        prison->checkTileCollisions = true;
        prison->position.y = (ScreenInfo->position.y - 48) << 16;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        self->state = DERobot_Unknown42;
    }

    if (!(Zone->timer & 7)) {
        int x                      = (RSDK.Rand(-48, 48) << 16) + self->position.x;
        int y                      = (RSDK.Rand(-48, 48) << 16) + self->position.y;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(3), x, y);
        explosion->drawOrder = Zone->drawOrderHigh;
    }

    foreach_active(Player, player)
    {
        if (player->position.x < self->position.x + 0x480000) {
            player->position.x = self->position.x + 0x480000;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->pushing    = false;
        }
    }
}

void DERobot_Unknown41(void)
{
    RSDK_THIS(DERobot);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += self->field_B0;
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void DERobot_Unknown42(void)
{
    RSDK_THIS(DERobot);

    foreach_active(Player, player)
    {
        if (player->position.x < self->position.x + 0x480000) {
            player->position.x = self->position.x + 0x480000;
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
        int x                      = (RSDK.Rand(-32, 32) << 16) + self->position.x;
        int y                      = (RSDK.Rand(-32, 32) << 16) + self->position.y;
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawOrder = Zone->drawOrderHigh;
    }
}

#if RETRO_INCLUDE_EDITOR
void DERobot_EditorDraw(void)
{
    RSDK_THIS(DERobot);

    self->drawFX  = FX_NONE;
    self->dword70 = (Entity *)self;
    switch (self->aniID) {
        case DEROBOT_BODY:
            self->stateDraw = DERobot_StateDraw_Unknown3;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);

            if (showGizmos()) {
                DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 8, -212 + 128, -SCREEN_YSIZE, 212 + 128, 0);
            }
            break;
        case DEROBOT_HEAD:
            self->stateDraw = DERobot_StateDraw_Unknown1;
            self->drawFX    = FX_ROTATE;
            self->offset.x  = -0x160000;
            self->offset.y  = -0x240000;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
            break;
        case DEROBOT_ARM:
            if (self->frameID == 2) {
                RSDK.SetSpriteAnimation(DERobot->aniFrames, 5, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(DERobot->aniFrames, 4, &self->animator3, true, 0);
                self->stateDraw = DERobot_StateDraw_Unknown2;
                self->drawFX    = FX_ROTATE;
                self->state     = DERobot_Unknown18;
            }
            else if (self->frameID) {
                self->stateDraw = DERobot_StateDraw_Unknown3;
            }
            else {
                self->stateDraw = DERobot_StateDraw_Unknown1;
                self->offset.x  = -0xC0000;
                self->offset.y  = -0x100000;
            }
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
            break;
        case DEROBOT_LEG:
            if (self->frameID) {
                self->stateDraw = DERobot_StateDraw_Unknown3;
            }
            else {
                RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator2, true, 1);
                self->stateDraw = DERobot_StateDraw_Unknown4;
            }
            if (self->frameID > 1)
                self->drawFX = FX_ROTATE;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
            break;
        case DEROBOT_TARGET_EDGE:
            self->drawFX     = FX_FLIP;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, 7, &self->animator2, true, 0);
            self->stateDraw = DERobot_StateDraw_Unknown5;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
            break;
        default:
            self->stateDraw = DERobot_StateDraw_Unknown3;
            RSDK.SetSpriteAnimation(DERobot->aniFrames, self->aniID, &self->animator1, true, self->frameID);
            break;
    }

    StateMachine_Run(self->stateDraw);
}

void DERobot_EditorLoad(void)
{
    DERobot->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DERobot.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DERobot, aniID);
    RSDK_ENUM_VAR("Body", DEROBOT_BODY);
    RSDK_ENUM_VAR("Head", DEROBOT_HEAD);
    RSDK_ENUM_VAR("Arm", DEROBOT_ARM);
    RSDK_ENUM_VAR("Leg", DEROBOT_LEG);
    RSDK_ENUM_VAR("Spikes", DEROBOT_SPIKES);
    RSDK_ENUM_VAR("Arm Thread", DEROBOT_ARM_THREAD);
    RSDK_ENUM_VAR("Target Edge", DEROBOT_TARGET_EDGE);
    RSDK_ENUM_VAR("Target Center", DEROBOT_TARGET_CENTER);
    RSDK_ENUM_VAR("Target Lock", DEROBOT_TARGET_LOCK);
    RSDK_ENUM_VAR("Bomb", DEROBOT_BOMB);
    RSDK_ENUM_VAR("Body (Cutscene)", DEROBOT_BODY_CUTSCENE);
}
#endif

void DERobot_Serialize(void)
{
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, aniID);
    RSDK_EDITABLE_VAR(DERobot, VAR_ENUM, frameID);
}

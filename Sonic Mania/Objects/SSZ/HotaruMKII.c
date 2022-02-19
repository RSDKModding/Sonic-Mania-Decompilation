// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HotaruMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHotaruMKII *HotaruMKII;

void HotaruMKII_Update(void)
{
    RSDK_THIS(HotaruMKII);
    StateMachine_Run(self->state);
}

void HotaruMKII_LateUpdate(void) {}

void HotaruMKII_StaticUpdate(void) {}

void HotaruMKII_Draw(void)
{
    RSDK_THIS(HotaruMKII);
    if (self->state != HotaruMKII_State_Unknown1) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        if (self->state != HotaruMKII_State_Unknown2 && ((Zone->timer & 1) || self->state == HotaruMKII_State_Unknown5)) {
            int ink           = self->inkEffect;
            self->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(&self->animator2, NULL, false);
            self->inkEffect = ink;
        }
    }
}

void HotaruMKII_Create(void *data)
{
    RSDK_THIS(HotaruMKII);
    if (!SceneInfo->inEditor) {
        self->drawOrder     = Zone->drawOrderHigh;
        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->drawFX        = FX_FLIP;
        if (!self->triggerSize.x) {
            self->triggerSize.x = 0x1000000;
            self->triggerSize.y = 0x1000000;
        }
        self->hitboxTrigger.right  = self->triggerSize.x >> 17;
        self->hitboxTrigger.left   = -self->hitboxTrigger.right;
        self->hitboxTrigger.bottom = self->triggerSize.y >> 17;
        self->hitboxTrigger.top    = -self->hitboxTrigger.bottom;
        self->type                 = voidToInt(data);

        switch (self->type) {
            default: break;
            case 0:
                self->active    = ACTIVE_BOUNDS;
                self->inkEffect = INK_NONE;
                self->visible   = true;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->animator2, true, 0);
                self->state = HotaruMKII_State_Setup;
                break;
            case 1:
                self->active    = ACTIVE_NORMAL;
                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->animator1, true, 0);
                self->state = HotaruMKII_State1_Unknown;
                break;
            case 2:
                --self->drawOrder;
                self->active    = ACTIVE_NORMAL;
                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 0x80;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 2, &self->animator1, true, 0);
                self->state = HotaruMKII_State2_Unknown1;
                break;
        }
    }
}

void HotaruMKII_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);

    HotaruMKII->hitbox1.top    = -6;
    HotaruMKII->hitbox1.left   = -6;
    HotaruMKII->hitbox1.right  = 6;
    HotaruMKII->hitbox1.bottom = 6;
    HotaruMKII->hitbox2.top    = -8;
    HotaruMKII->hitbox2.left   = -8;
    HotaruMKII->hitbox2.right  = 8;
    HotaruMKII->hitbox2.bottom = 8;

    HotaruMKII->sfxLaser  = RSDK.GetSfx("SSZ1/HotaruLaser.wav");
    HotaruMKII->sfxAppear = RSDK.GetSfx("SSZ1/HotaruAppear.wav");
    HotaruMKII->sfxFly    = RSDK.GetSfx("SSZ1/HotaruFly.wav");
    HotaruMKII->sfxCharge = RSDK.GetSfx("SSZ1/HotaruCharge.wav");
    DEBUGMODE_ADD_OBJ(HotaruMKII);
}

void HotaruMKII_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityHotaruMKII *hotaruMKII = CREATE_ENTITY(HotaruMKII, NULL, self->position.x, self->position.y);
    hotaruMKII->origin           = RSDK.Rand(0, 0x100);
    hotaruMKII->offset1.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0x100);
    hotaruMKII->offset1.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset2.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.x        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
    hotaruMKII->offset3.y        = -0x10000 * RSDK.Rand(0, 2) * RSDK.Rand(0x20, 0xC0);
}

void HotaruMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void HotaruMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(HotaruMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &HotaruMKII->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void HotaruMKII_CheckOffScreen(void)
{
    RSDK_THIS(HotaruMKII);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        self->offsetID   = 0;
        HotaruMKII_Create(NULL);
    }
}

void HotaruMKII_HandleDistances(void *p)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)p;

    if (Player_CheckValidState(player)) {
        int distX = self->distance.x;
        switch (self->offsetID) {
            case 0:
                self->distance.x = self->offset1.x;
                self->distance.y = self->offset1.y;
                break;
            case 1:
                self->distance.x = self->offset2.x;
                self->distance.y = self->offset2.y;
                break;
            case 2:
                self->distance.x = self->offset3.x;
                self->distance.y = self->offset3.y;
                break;
            case 3:
                self->distance.x = 0;
                self->distance.y = 0;
                break;
            default: break;
        }

        self->distance.x &= 0xFFFF0000;
        self->distance.y &= 0xFFFF0000;
        if (!self->distance.x && !self->distance.y) {
            self->distance.x = distX;
            self->distance.y = 0xB0010000;
        }
        int angle =
            RSDK.ATan2(player->position.x + self->distance.x - self->position.x, self->distance.y + player->position.y - self->position.y);

        self->field_6C = 0x300 * RSDK.Cos256(angle);
        self->field_70 = 0x300 * RSDK.Sin256(angle);
        self->velocity.x += self->field_6C;
        self->velocity.y += self->field_70;
        ++self->offsetID;
    }
    else {
        self->playerPtr  = NULL;
        self->offsetID   = 4;
        self->velocity.x = 0;
        self->velocity.y = -0x30000;
        self->state      = HotaruMKII_State_Unknown6;
    }
}

void HotaruMKII_State_Setup(void)
{
    RSDK_THIS(HotaruMKII);

    self->active     = ACTIVE_NORMAL;
    self->childCount = 0;
    self->state      = HotaruMKII_State_Unknown1;
    HotaruMKII_State_Unknown1();
}

void HotaruMKII_State_Unknown1(void)
{
    RSDK_THIS(HotaruMKII);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)) {
            self->playerPtr = (Entity *)player;
            if (!player->sidekick) {
                flag         = true;
                int screenID = 0;
                if (player->camera)
                    screenID = player->camera->screenID;

                self->position.x = RSDK.Cos256(self->origin) << 17;
                self->position.y = RSDK.Sin256(self->origin) << 17;
                RSDKScreenInfo *screen = &ScreenInfo[screenID];

                if (self->position.x > (screen->width & 0xFFFFFFFE) << 15)
                    self->position.x = (screen->width & 0xFFFFFFFE) << 15;

                if (self->position.x < -0x8000 * (screen->width & 0xFFFFFFFE))
                    self->position.x = -0x8000 * (screen->width & 0xFFFFFFFE);

                if (self->position.y > (screen->height & 0xFFFFFFFE) << 15)
                    self->position.y = (screen->height & 0xFFFFFFFE) << 15;

                if (self->position.y < -0x8000 * (screen->height & 0xFFFFFFFE))
                    self->position.y = -0x8000 * (screen->height & 0xFFFFFFFE);

                if (self->position.x >= 0)
                    self->position.x += 0x100000;
                else
                    self->position.x -= 0x100000;

                if (self->position.y >= 0)
                    self->position.y += 0x100000;
                else
                    self->position.y -= 0x100000;

                self->position.x += ((screen->width & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
                self->position.y += ((screen->height & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
                HotaruMKII_HandleDistances(player);
                self->inkEffect = INK_ALPHA;
                self->visible   = true;
                self->alpha     = 128;
                RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->animator1, true, 0);
                RSDK.PlaySfx(HotaruMKII->sfxFly, false, 255);
                self->state = HotaruMKII_State_Unknown2;
                foreach_break;
            }
        }
    }

    if (!flag)
        HotaruMKII_CheckOffScreen();
}

void HotaruMKII_State_Unknown6(void)
{
    RSDK_THIS(HotaruMKII);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    HotaruMKII_CheckOffScreen();
}

void HotaruMKII_State_Unknown2(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (player && !Player_CheckValidState(player)) {
        HotaruMKII_HandleDistances(player);
    }
    else {
        if ((Zone->timer & 7) == 0) {
            EntityHotaruMKII *child = CREATE_ENTITY(HotaruMKII, intToVoid(1), self->position.x, self->position.y);
            child->playerPtr        = self->playerPtr;
            child->distance.x       = self->position.x - player->position.x;
            child->distance.y       = self->position.y - player->position.y;
        }

        int flags = 0;
        if (self->field_6C < 0) {
            int x = player->position.x + self->distance.x;
            if (self->position.x <= x) {
                flags              = 1;
                self->position.x = x;
            }
        }
        if (self->field_6C > 0) {
            int x = player->position.x + self->distance.x;
            if (self->position.x >= x) {
                flags              = 1;
                self->position.x = x;
            }
        }

        if (self->field_70 < 0) {
            int y = player->position.y + self->distance.y;
            if (self->position.y <= y) {
                flags++;
                self->position.y = y;
            }
        }
        if (self->field_70 > 0) {
            int y = player->position.y + self->distance.y;
            if (self->position.y >= y) {
                flags++;
                self->position.x = y;
            }
        }

        if (flags == 2) {
            self->inkEffect = INK_NONE;
            self->alpha     = 0;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &self->animator1, true, 0);
            self->timer = 60;
            RSDK.PlaySfx(HotaruMKII->sfxAppear, false, 255);
            self->state = HotaruMKII_State_Unknown3;
        }
        else {
            RSDK.ProcessAnimation(&self->animator1);
            int angle          = RSDK.ATan2(player->position.x + self->distance.x - self->position.x,
                                   player->position.y + self->distance.y - self->position.y);
            self->field_6C   = 0x300 * RSDK.Cos256(angle);
            self->velocity.x = 0x300 * RSDK.Cos256(angle) + player->velocity.x;
            self->field_70   = 0x300 * RSDK.Sin256(angle);
            self->velocity.y = 0x300 * RSDK.Sin256(angle) + player->velocity.y;
        }
        HotaruMKII_CheckOffScreen();
    }
}

void HotaruMKII_State_Unknown3(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    RSDK.ProcessAnimation(&self->animator3);
    if (Player_CheckValidState(player)) {
        self->position.x = player->position.x + self->distance.x;
        self->position.y = player->position.y + self->distance.y;
        if (--self->timer <= 0) {
            RSDK.PlaySfx(HotaruMKII->sfxCharge, false, 255);
            self->state = HotaruMKII_State_Unknown4;
        }
        HotaruMKII_CheckOffScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Unknown4(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    RSDK.ProcessAnimation(&self->animator2);
    if (Player_CheckValidState(player)) {
        self->position.x = player->position.x + self->distance.x;
        self->position.y = player->position.y + self->distance.y;
        self->alpha += 2;
        if (self->alpha == 0x100) {
            self->timer     = 90;
            self->alpha     = 256;
            self->inkEffect = INK_ALPHA;
            self->state     = HotaruMKII_State_Unknown5;
        }
        HotaruMKII_CheckPlayerCollisions();
        HotaruMKII_CheckOffScreen();
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State_Unknown5(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    RSDK.ProcessAnimation(&self->animator1);
    if (Player_CheckValidState(player)) {
        if (--self->timer > 0) {
            if (self->timer <= 60) {
                if (self->timer == 60) {
                    RSDK.PlaySfx(HotaruMKII->sfxLaser, false, 255);
                    RSDK.StopSfx(HotaruMKII->sfxCharge);
                }
                self->alpha -= 3;
                if (!(self->timer & 3)) {
                    EntityHotaruMKII *child = CREATE_ENTITY(HotaruMKII, intToVoid(2), self->position.x, self->position.y + 0xE0000);
                    if (!self->childCount) {
                        self->childCount       = 1;
                        child->animator1.frameID = 1;
                    }
                    if (self->childCount == 1)
                        child->childCount = 1;
                    if (++self->childCount == 3)
                        self->childCount = 1;
                }
            }
            RSDK.ProcessAnimation(&self->animator2);
            HotaruMKII_CheckPlayerCollisions();
            HotaruMKII_CheckOffScreen();
        }
        else {
            HotaruMKII_HandleDistances(player);
            self->childCount = 0;
            self->alpha      = 128;
            self->inkEffect  = INK_ALPHA;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 1, &self->animator1, true, 0);
            self->state = HotaruMKII_State_Unknown2;
        }
    }
    else {
        HotaruMKII_HandleDistances(player);
    }
}

void HotaruMKII_State1_Unknown(void)
{
    RSDK_THIS(HotaruMKII);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->position.x = player->position.x + self->distance.x;
    self->position.y = player->position.y + self->distance.y;
    RSDK.ProcessAnimation(&self->animator1);
    self->alpha -= 4;
    if (self->alpha <= 0)
        destroyEntity(self);
}

void HotaruMKII_State2_Unknown1(void)
{
    RSDK_THIS(HotaruMKII);

    self->position.y += 0x40000;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &HotaruMKII->hitbox2)) {
            Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (self->childCount == 1) {
            ++self->drawOrder;
            self->position.y += 0x80000;
            self->inkEffect = INK_NONE;
            RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 3, &self->animator1, true, 0);
            self->state = HotaruMKII_State2_Unknown2;
        }
        else {
            destroyEntity(self);
        }
    }
}

void HotaruMKII_State2_Unknown2(void)
{
    RSDK_THIS(HotaruMKII);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID >= self->animator1.frameCount - 1)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void HotaruMKII_EditorDraw(void)
{
    RSDK_THIS(HotaruMKII);
    self->drawOrder     = Zone->drawOrderHigh;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->inkEffect     = INK_NONE;
    self->visible       = true;
    RSDK.SetSpriteAnimation(HotaruMKII->aniFrames, 0, &self->animator1, false, 0);

    RSDK.DrawSprite(&self->animator1, NULL, false);

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->triggerSize.x >> 1;
    drawPos.y -= self->triggerSize.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + self->triggerSize.x, drawPos.y, 0x00FF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, self->triggerSize.y + drawPos.y, drawPos.x + self->triggerSize.x, self->triggerSize.y + drawPos.y, 0x00FF00, 0,
                  INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + self->triggerSize.y, 0x00FF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->triggerSize.x, drawPos.y, drawPos.x + self->triggerSize.x, drawPos.y + self->triggerSize.y, 0x00FF00, 0,
                  INK_NONE, false);
}

void HotaruMKII_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        HotaruMKII->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruMKII_Serialize(void)
{
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_UINT8, origin);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset1);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset2);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, offset3);
    RSDK_EDITABLE_VAR(HotaruMKII, VAR_VECTOR2, triggerSize);
}

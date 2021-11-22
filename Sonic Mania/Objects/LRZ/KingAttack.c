#include "SonicMania.h"

ObjectKingAttack *KingAttack;

void KingAttack_Update(void)
{
    RSDK_THIS(KingAttack);
    StateMachine_Run(self->state);
}

void KingAttack_LateUpdate(void) {}

void KingAttack_StaticUpdate(void) {}

void KingAttack_Draw(void)
{
    RSDK_THIS(KingAttack);

    int x = ScreenInfo->position.x << 16;
    int y = ScreenInfo->position.y << 16;

    switch (self->type) {
        case 0:
            for (int i = 0; i < 6; i += 2) {
                Vector2 vertices[4];
                colour colours[4];

                vertices[0].x = self->laserVertPostions[i].x - x;
                vertices[0].y = self->laserVertPostions[i].y - y;
                colours[0]    = self->laserColours[i];

                vertices[1].x = self->laserVertPostions[i + 1].x - x;
                vertices[1].y = self->laserVertPostions[i + 1].y - y;
                colours[1]    = self->laserColours[i + 1];

                vertices[2].x = self->laserVertPostions[i + 3].x - x;
                vertices[2].y = self->laserVertPostions[i + 3].y - y;
                colours[2]    = self->laserColours[i + 3];

                vertices[3].x = self->laserVertPostions[i + 2].x - x;
                vertices[3].y = self->laserVertPostions[i + 2].y - y;
                colours[3]    = self->laserColours[i + 2];

                RSDK.DrawBlendedQuad(vertices, colours, 4, self->alpha, INK_ADD);
            }
            //[Fallthrough]
        default: RSDK.DrawSprite(&self->animator, NULL, false); break;
        case 5:
            RSDK.DrawLine(self->position.x, self->position.y, self->lineEndPos.x, self->lineEndPos.y, 0x0080F0, self->alpha, INK_ADD,
                          false);
            break;
    }
}

void KingAttack_Create(void *data)
{
    RSDK_THIS(KingAttack);

    if (!SceneInfo->inEditor) {
        self->drawOrder     = Zone->drawOrderHigh;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->type          = voidToInt(data);

        switch (self->type) {
            case 0:
                self->inkEffect    = INK_ADD;
                self->visible      = true;
                self->laserColours = KingAttack->laserColours;
                self->state        = KingAttack_State0_Unknown;
                KingAttack->flag     = false;
                RSDK.PlaySfx(KingAttack->sfxLaserSweep, false, 255);
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 21, &self->animator, true, 0);
                break;
            case 1:
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 23, &self->animator, true, 0);
                self->state         = KingAttack_State1_Unknown1;
                self->drawFX        = FX_FLIP;
                self->hitbox.left   = -16;
                self->hitbox.top    = -64;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 0;
                break;
            case 2:
                self->drawFX  = FX_SCALE;
                self->visible = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &self->animator, true, RSDK.Rand(0, 6));
                self->state         = KingAttack_State2_Unknown1;
                self->hitbox.left   = -10;
                self->hitbox.top    = -10;
                self->hitbox.right  = 10;
                self->hitbox.bottom = 10;
                break;
            case 3:
                self->drawOrder = Zone->drawOrderLow;
                self->inkEffect = INK_ADD;
                self->visible   = true;
                self->alpha     = 0xC0;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 24, &self->animator, true, 0);
                self->state = KingAttack_State3_Unknown1;
                break;
            case 4:
                self->drawFX  = FX_SCALE;
                self->visible = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &self->animator, true, RSDK.Rand(0, 6));
                self->scale.x = 0x100;
                self->scale.y = 0x100;
                self->state   = KingAttack_State4_Unknown1;
                break;
            case 5:
                self->drawOrder = Zone->drawOrderLow;
                self->visible   = true;
                self->state     = KingAttack_State5_Unknown;
                self->angle     = RSDK.Rand(0, 256);
                self->timer     = 0x4000;
                self->alpha     = 0x100;
                break;
            case 6:
                self->drawOrder = Zone->drawOrderLow;
                self->drawFX    = FX_ROTATE;
                self->visible   = true;
                RSDK.SetSpriteAnimation(KingAttack->aniFrames, 21, &self->animator, true, 0);
                self->state         = KingAttack_State6_Unknown1;
                self->hitbox.left   = -4;
                self->hitbox.top    = -4;
                self->hitbox.right  = 4;
                self->hitbox.bottom = 4;
                break;
            default: break;
        }
    }
}

void KingAttack_StageLoad(void)
{
    KingAttack->aniFrames     = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE);
    KingAttack->sfxLaserSweep = RSDK.GetSfx("LRZ/LaserSweep.wav");
    KingAttack->sfxLaserErupt = RSDK.GetSfx("LRZ/LaserErupt.wav");
    KingAttack->flag          = false;
    KingAttack->sfxElecPulse  = RSDK.GetSfx("Stage/ElecPulse.wav");
    KingAttack->sfxTwinShot   = RSDK.GetSfx("LRZ/TwinShot.wav");
    int id                    = Soundboard_LoadSFX("LRZ/ElecIdle.wav", true, KingAttack_ElecIdleCheckCB, NULL);
    if (id >= 0)
        Soundboard->sfxFadeOutDuration[id] = 30;
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
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Player_CheckHit(player, self);
        }
    }
}

void KingAttack_HandleLaserPositions(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)self->parent;

    int x = 0;
    if (king->direction)
        x = king->position.x - 0x240000;
    else
        x = king->position.x + 0x240000;
    int y = king->position.y - 0x300000;

    self->laserVertPostions[0].x = x - 0x40000;
    self->laserVertPostions[1].x = self->position.x - 0x40000;
    self->laserVertPostions[2].x = x - 0x20000;
    self->laserVertPostions[3].x = self->position.x - 0x20000;
    self->laserVertPostions[4].x = x + 0x20000;
    self->laserVertPostions[5].x = self->position.x + 0x20000;
    self->laserVertPostions[6].x = x + 0x40000;
    self->laserVertPostions[7].x = self->position.x + 0x40000;

    self->laserVertPostions[0].y = y;
    self->laserVertPostions[1].y = HeavyKing->value9;
    self->laserVertPostions[2].y = y;
    self->laserVertPostions[3].y = HeavyKing->value9;
    self->laserVertPostions[4].y = y;
    self->laserVertPostions[5].y = HeavyKing->value9;
    self->laserVertPostions[6].y = y;
    self->laserVertPostions[7].y = HeavyKing->value9;
}

void KingAttack_State0_Unknown(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    ++self->timer;
    KingAttack_HandleLaserPositions();

    if (self->timer < 16) {
        if (self->alpha < 0xC0)
            self->alpha += 0x10;
    }
    else if (self->timer > 48) {
        if (self->alpha > 0)
            self->alpha -= 0x10;
    }

    if (!(self->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(1), self->position.x, self->position.y);
        attack->direction        = (self->timer >> 3) & 1;
    }
    if (self->timer == 64)
        destroyEntity(self);
}

void KingAttack_State1_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    if (++self->timer == 30) {
        self->timer   = 0;
        self->visible = true;
        if (KingAttack->flag) {
            KingAttack->flag = false;
        }
        else {
            Camera_ShakeScreen(0, 0, 5);
            RSDK.PlaySfx(KingAttack->sfxLaserErupt, false, 255);
            KingAttack->flag = true;
        }
        self->state = KingAttack_State1_Unknown2;
    }
}

void KingAttack_State1_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID > 8 && self->animator.frameID < 16)
        KingAttack_CheckPlayerCollisions();

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void KingAttack_State2_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)self->parent;

    RSDK.ProcessAnimation(&self->animator);

    self->angle = (self->angle + 12) & 0x3FF;
    if (self->scale.x < 512) {
        self->scale.x += 0x20;
        self->scale.y = self->scale.x;
    }

    self->velocity.x = self->position.x;
    self->velocity.y = self->position.y;

    int x = 0;
    if (king->direction)
        x = RSDK.Cos1024(self->angle) - 0x240;
    else
        x = RSDK.Cos1024(self->angle) + 0x240;
    self->position.x = (x << 12) + king->position.x;
    self->position.y = (((RSDK.Sin1024(self->angle) - 0x300) << 12) + king->position.y);
    self->velocity.x = self->position.x - self->velocity.x;
    self->velocity.y = self->position.y - self->velocity.y;
    KingAttack_CheckPlayerCollisions();
    if (++self->timer == 120) {
        self->timer        = 0;
        self->lineEndPos.x = king->position.x;
        self->lineEndPos.y = king->position.y;
        self->state        = KingAttack_State2_Unknown2;
    }
}

void KingAttack_State2_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)self->parent;

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += king->position.x - self->lineEndPos.x;
    self->position.y += king->position.y - self->lineEndPos.y;
    if (self->position.x <= king->position.x) {
        if (self->position.x < king->position.x)
            self->velocity.x += 0x4000;
    }
    else {
        self->velocity.x -= 0x4000;
    }

    if (self->position.y <= king->position.y) {
        if (self->position.y < king->position.y)
            self->velocity.y += 0x3800;
    }
    else {
        self->velocity.y -= 0x3800;
    }

    if (self->velocity.x >= -0x50000) {
        if (self->velocity.x > 0x50000)
            self->velocity.x = 0x50000;
    }
    else {
        self->velocity.x = -0x50000;
    }

    if (self->velocity.y >= -0x50000) {
        if (self->velocity.y > 0x50000)
            self->velocity.y = 0x50000;
    }
    else {
        self->velocity.y = -0x50000;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->lineEndPos.x = king->position.x;
    self->lineEndPos.y = king->position.y;

    self->angle = (self->angle + 12) & 0x3FF;
    self->scale.x += (0x180 - self->scale.x - (RSDK.Sin1024(self->angle) >> 3)) >> 3;
    self->scale.y = self->scale.x;
    KingAttack_CheckPlayerCollisions();

    if (self->angle < 512)
        self->drawOrder = Zone->drawOrderLow - 1;
    else
        self->drawOrder = Zone->drawOrderLow;
}

void KingAttack_State_Unknown(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&self->animator);
    ++self->timer;
    if (!(self->timer & 3))
        CREATE_ENTITY(KingAttack, intToVoid(3), self->position.x, self->position.y);

    if (self->scale.x < 512) {
        self->scale.x += 0x20;
        self->scale.y = self->scale.x;
    }

    self->velocity.x += ((self->field_70.x - self->velocity.x) >> 3);
    self->velocity.y += ((self->field_70.y - self->velocity.y) >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    KingAttack_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void KingAttack_State3_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    if (self->timer++ >= 8) {
        self->alpha -= 8;
        if (self->alpha <= 0)
            destroyEntity(self);
    }
}

void KingAttack_State4_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)self->parent;

    RSDK.ProcessAnimation(&self->animator);

    if (self->scale.x < 0x400)
        self->scale.x += 4;
    self->scale.y = self->scale.x;

    ++self->timer;
    self->position.x = king->position.x;
    self->position.y = king->position.y;
    if (king->direction)
        self->position.x -= 0x240000;
    else
        self->position.x += 0x240000;
    self->position.y -= 0x300000;

    if (!(self->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(5), self->position.x, self->position.y);
        attack->parent           = (Entity *)self;
    }
    if (self->timer == 120) {
        self->timer = 0;
        self->state = KingAttack_State4_Unknown2;
    }
}

void KingAttack_State5_Unknown(void)
{
    RSDK_THIS(KingAttack);

    EntityKingAttack *parent = (EntityKingAttack *)self->parent;

    self->position.x = self->timer * RSDK.Cos256(self->angle);
    self->position.y = self->timer * RSDK.Sin256(self->angle);
    self->lineEndPos.x = self->position.x - (self->position.x >> 3);
    self->lineEndPos.y = self->position.y - (self->position.y >> 3);

    self->position.x += parent->position.x;
    self->position.y += parent->position.y;
    self->lineEndPos.x += parent->position.x;
    self->lineEndPos.y += parent->position.y;

    self->alpha -= 16;
    self->timer -= 0x200;
    if (self->timer < 0x400)
        destroyEntity(self);
}

void KingAttack_State4_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    EntityHeavyKing *king = (EntityHeavyKing *)self->parent;

    RSDK.ProcessAnimation(&self->animator);

    self->position.x = king->position.x;
    self->position.y = king->position.y;

    if (king->direction)
        self->position.x -= 0x240000;
    else
        self->position.x += 0x240000;
    self->position.y -= 0x300000;
    ++self->timer;

    if (!(self->timer & 6))
        RSDK.PlaySfx(KingAttack->sfxTwinShot, false, 255);

    if (!(self->timer & 3)) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(6), self->position.x, self->position.y);
        attack->velocity.x       = RSDK.Sin1024(self->angle) << 9;
        attack->velocity.y       = RSDK.Cos1024(self->angle) << 9;
        attack->rotation         = self->angle >> 1;

        attack             = CREATE_ENTITY(KingAttack, intToVoid(6), self->position.x, self->position.y);
        attack->velocity.x = RSDK.Sin1024(self->angle + 0x200) << 9;
        attack->velocity.y = RSDK.Cos1024(self->angle + 0x200) << 9;
        attack->rotation   = self->angle >> 1;
    }

    self->angle += 4;
    if (self->timer > 60) {
        self->scale.x -= 4;
        self->scale.y = self->scale.x;
        if (!self->scale.x)
            destroyEntity(self);
    }
}

void KingAttack_State6_Unknown1(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&self->animator);
    self->rotation = (self->rotation + 16) & 0x1FF;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    KingAttack_CheckPlayerCollisions();

    foreach_active(HPZEmerald, emerald)
    {
        if (emerald->type && RSDK.CheckObjectCollisionPlatform(emerald, emerald->hitbox, self, &self->hitbox, true)) {
            RSDK.SetSpriteAnimation(KingAttack->aniFrames, 22, &self->animator, false, 0);
            self->position.y += 0x40000;
            self->state = KingAttack_State6_Unknown2;
            foreach_break;
        }
    }

    if (self->state == KingAttack_State6_Unknown1) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
            RSDK.SetSpriteAnimation(KingAttack->aniFrames, 22, &self->animator, false, 0);
            self->state = KingAttack_State6_Unknown2;
            self->position.y += 0x40000;
        }

        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
}

void KingAttack_State6_Unknown2(void)
{
    RSDK_THIS(KingAttack);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void KingAttack_EditorDraw(void)
{
    RSDK_THIS(KingAttack);
    RSDK.SetSpriteAnimation(KingAttack->aniFrames, 20, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void KingAttack_EditorLoad(void) { KingAttack->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE); }
#endif

void KingAttack_Serialize(void) {}

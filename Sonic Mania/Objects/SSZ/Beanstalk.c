#include "SonicMania.h"

ObjectBeanstalk *Beanstalk;

void Beanstalk_Update(void)
{
    RSDK_THIS(Beanstalk);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->type > 1)
        RSDK.ProcessAnimation(&self->animator2);
    StateMachine_Run(self->state);
}

void Beanstalk_LateUpdate(void) {}

void Beanstalk_StaticUpdate(void) {}

void Beanstalk_Draw(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_Unknown4();
        Beanstalk_Unknown5();

        self->scale.x = self->field_78;
        self->scale.y = self->field_78;
        self->drawFX  = FX_SCALE | FX_FLIP;
        RSDK.DrawSprite(&self->animator1, NULL, false);
        if (self->type > 1)
            RSDK.DrawSprite(&self->animator2, NULL, false);
        self->drawFX  = FX_FLIP;
        self->scale.x = 0x200;
        self->scale.y = 0x200;
    }
}

void Beanstalk_Create(void *data)
{
    RSDK_THIS(Beanstalk);

    self->active       = ACTIVE_BOUNDS;
    self->visible      = true;
    self->drawFX       = FX_FLIP;
    self->drawOrder    = Zone->drawOrderLow;
    self->bezCtrlAngle = self->bezCtrlAngle & 0xFF;

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->field_74      = -1;
    if (self->type == 0) {
        self->field_78 = 512;
        self->active   = ACTIVE_NORMAL;
    }
    else {
        self->field_78 = 0;
    }
    self->field_8C = self->position;
    self->field_94 = self->position;
    self->field_9C = 0;
    self->timer    = 0;
    self->field_A0 = 0;
    self->state    = Beanstalk_State_Setup;
}

void Beanstalk_StageLoad(void)
{
    Beanstalk->aniFrames             = RSDK.LoadSpriteAnimation("SSZ1/Beanstalk.bin", SCOPE_STAGE);
    Beanstalk->sfxImpact             = RSDK.GetSfx("Stage/Impact6.wav");
    Beanstalk->sfxBeanNode           = RSDK.GetSfx("SSZ1/BeanNode.wav");
    Beanstalk->sfxBeanChomp          = RSDK.GetSfx("SSZ1/BeanChomp.wav");
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
    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(self) + 1, Beanstalk);

    if (self->forceEnd || next->objectID != Beanstalk->objectID)
        return 0;

    int thisX = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
    int thisY = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return MathHelpers_Unknown7(self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y) / 0xA0000;
}

int32 Beanstalk_Unknown2(void)
{
    RSDK_THIS(Beanstalk);
    int len = 0;

    while (self->type) {
        EntityBeanstalk *prev = RSDK_GET_ENTITY(RSDK.GetEntityID(self) - 1, Beanstalk);
        if (prev->objectID != Beanstalk->objectID)
            break;

        int32 prevX = ((prev->bezCtrlLength * RSDK.Cos256(prev->bezCtrlAngle)) << 9) + prev->position.x;
        int32 prevY = ((prev->bezCtrlLength * RSDK.Sin256(prev->bezCtrlAngle)) << 9) + prev->position.y;

        uint8 angle = (self->bezCtrlAngle + 0x80);
        int32 thisX = ((self->bezCtrlLength * RSDK.Cos256(angle)) << 9) + self->position.x;
        int32 thisY = ((self->bezCtrlLength * RSDK.Sin256(angle)) << 9) + self->position.y;
        len += MathHelpers_Unknown7(prev->position.x, prev->position.y, prevX, prevY, thisX, thisY, self->position.x, self->position.y);
        self = prev;
    }
    return len;
}

int32 Beanstalk_Unknown3(void)
{
    RSDK_THIS(Beanstalk);
    EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(self) + 1, Beanstalk);

    if (self->forceEnd || next->objectID != Beanstalk->objectID)
        return 0;

    int32 thisX = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
    int32 thisY = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;

    uint8 angle = (next->bezCtrlAngle + 0x80);
    int32 nextX = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
    int32 nextY = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;
    return 0x10000
           / (MathHelpers_Unknown7(self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x, next->position.y) / 0x32000);
}

void Beanstalk_Unknown4(void)
{
    RSDK_THIS(Beanstalk);

    if (!self->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(self) + 1, Beanstalk);
        if (next->objectID == Beanstalk->objectID) {
            if (SceneInfo->inEditor) {
                self->field_74 = Beanstalk_Unknown1();
                self->field_80 = 0x10000;
            }
            if (self->field_74 == 0xFFFF)
                self->field_74 = Beanstalk_Unknown1();

            if (self->field_74) {
                if (self->field_88 == 0 || SceneInfo->inEditor) {
                    int val          = Beanstalk_Unknown2();
                    self->field_88 = 1;
                    self->field_85 = (val / 0xA0000 / 3) % 9;
                    self->field_84 = 3 * ((val / 0xA0000 / 3) + 1) - val / 0xA0000;
                }

                int thisX   = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
                int thisY   = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;
                uint8 angle = (next->bezCtrlAngle + 0x80);
                int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
                int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;

                int inc = 0x10000 / self->field_74;
                int pos = (0x10000 / self->field_74) >> 1;

                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &self->animator3, true, 0);
                for (int i = pos; i <= 0x10000; i += inc) {
                    if (i >= self->field_80)
                        break;
                    Vector2 drawPos = MathHelpers_Unknown5(i, self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x,
                                                           next->position.y);
                    RSDK.DrawSprite(&self->animator3, &drawPos, false);
                }

                int val = self->field_84;
                for (int i = self->field_85; pos <= 0x10000; ++val) {
                    if (pos >= self->field_80)
                        break;
                    Vector2 drawPos = MathHelpers_Unknown5(pos, self->position.x, self->position.y, thisX, thisY, nextX, nextY, next->position.x,
                                                           next->position.y);
                    if (!(val % 3)) {
                        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, i + 5, &self->animator3, true, 0);
                        RSDK.DrawSprite(&self->animator3, &drawPos, false);
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

    if (!self->forceEnd) {
        EntityBeanstalk *next = RSDK_GET_ENTITY(RSDK.GetEntityID(self) + 1, Beanstalk);
        if (next->objectID == Beanstalk->objectID) {
            if (self->field_74) {
                uint8 angle = next->bezCtrlAngle + 0x80;
                int thisX   = ((self->bezCtrlLength * RSDK.Cos256(self->bezCtrlAngle)) << 9) + self->position.x;
                int thisY   = ((self->bezCtrlLength * RSDK.Sin256(self->bezCtrlAngle)) << 9) + self->position.y;
                int nextX   = ((next->bezCtrlLength * RSDK.Cos256(angle)) << 9) + next->position.x;
                int nextY   = ((next->bezCtrlLength * RSDK.Sin256(angle)) << 9) + next->position.y;

                Vector2 drawPos = MathHelpers_Unknown5(self->field_80, self->position.x, self->position.y, thisX, thisY, nextX, nextY,
                                                       next->position.x, next->position.y);
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 1, &self->animator3, true, 0);
                RSDK.DrawSprite(&self->animator3, &drawPos, false);
            }
        }
    }
}

void Beanstalk_Unknown6(void)
{
    RSDK_THIS(Beanstalk);

    if (!self->flag2) {
        EntityBeanstalk *node = NULL;
        if (!self->forceEnd) {
            node = RSDK_GET_ENTITY(RSDK.GetEntityID(self) + 1, Beanstalk);
            if (node->objectID != Beanstalk->objectID)
                node = NULL;
        }

        if (self->forceEnd || !node)
            self->field_80 = 0x10000;

        if (self->field_80 < 0x10000) {
            if (!self->field_7C)
                self->field_7C = Beanstalk_Unknown3();
            self->field_80 += self->field_7C;
        }
        if (self->field_80 > 0x10000)
            self->field_80 = 0x10000;

        if (self->field_80 == 0x10000) {
            self->flag2  = true;
            self->active = ACTIVE_BOUNDS;
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

    self->field_78 = (self->timer << 9) / 20;
    if (self->timer < 20) {
        if (!self->timer) {
            if (RSDK.CheckOnScreen(self, NULL))
                RSDK.PlaySfx(Beanstalk->sfxBeanNode, false, 255);
        }
        ++self->timer;
    }
}

void Beanstalk_CheckPlayerCollisions_Leaf(void)
{
    RSDK_THIS(Beanstalk);

    int storeX       = self->position.x;
    int storeY       = self->position.y;
    self->position = self->field_94;
    self->position.y &= 0xFFFF0000;
    self->position.x = ((RSDK.Sin512(2 * self->angle2) << 9) + self->position.x) & 0xFFFF0000;

    if (self->activePlayers == 0) {
        if (self->field_9C > 0)
            self->field_9C -= 4;
    }
    else {
        if (self->field_9C < 64)
            self->field_9C += 4;
    }

    self->activePlayers = 0;
    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (Player_CheckCollisionPlatform(player, self, &Beanstalk->hitboxPlatform)) {
            player->position.x += self->position.x - storeX;

            if (self->position.y - storeY > 0)
                player->position.y += self->position.y - storeY;
            player->position.y += 0x10000;
            self->activePlayers |= 1 << playerID;
            player->position.y &= 0xFFFF0000;
        }
    }

    self->field_94.x = self->field_8C.x;
    self->field_94.y = self->field_8C.y;
    self->field_94.y += RSDK.Sin256(self->field_9C) << 10;
}

void Beanstalk_CheckPlayerCollisions_Plant(void)
{
    RSDK_THIS(Beanstalk);

    int mult = 0;
    if (self->field_A0 >= 15) {
        if (self->field_A0 >= 60) {
            self->field_A0 = 0;
        }
        else {
            mult = 0x15540 - 0x5B0 * self->field_A0;
            self->field_A0++;
        }
    }
    else {
        mult = 0x1111 * self->field_A0;
        self->field_A0++;
    }

    self->position = self->field_8C;
    if (self->direction == FLIP_X)
        self->position.x += mult * -16;
    else
        self->position.x += mult * 16;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Beanstalk->hitboxPlant)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                self->state = Beanstalk_State1_Unknown;
                RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator2, true, 0);
            }
            else if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer)) {
#endif
                Player_CheckHit(player, self);
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}

void Beanstalk_State_Setup(void)
{
    RSDK_THIS(Beanstalk);

    switch (self->type) {
        case 0:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->animator1, true, 9);
            self->state = Beanstalk_State_Unknown1;
            break;
        case 1:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            self->state = Beanstalk_State1_Unknown;
            break;
        case 2:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 2, &self->animator2, true, 0);
            self->state = Beanstalk_State2_Unknown;
            break;
        case 3:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 3, &self->animator2, true, 0);
            self->state = Beanstalk_State3_Unknown;
            break;
        default: break;
    }
}

void Beanstalk_State_Unknown1(void)
{
    RSDK_THIS(Beanstalk);
    if (self->startGrowth) {
        self->state = Beanstalk_State_Unknown2;
        self->timer = 15;
    }
}

void Beanstalk_State_Unknown2(void)
{
    RSDK_THIS(Beanstalk);

    if (self->timer <= 0) {
        RSDK.PlaySfx(Beanstalk->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 5);
        RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->animator1, true, 0);
        self->shown  = true;
        self->active = ACTIVE_NORMAL;
        self->state  = Beanstalk_State_Unknown3;
    }
    else {
        self->timer--;
    }
}

void Beanstalk_State_Unknown3(void) { Beanstalk_Unknown6(); }

void Beanstalk_State1_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        self->position   = self->field_8C;
        self->position.x = ((RSDK.Sin512(2 * self->angle2) << 9) + self->position.x) & 0xFFFF0000;
        self->position.y &= 0xFFFF0000;
        ++self->angle2;
    }
}

void Beanstalk_State2_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        Beanstalk_CheckPlayerCollisions_Leaf();
        ++self->angle2;
    }
}

void Beanstalk_State3_Unknown(void)
{
    RSDK_THIS(Beanstalk);

    if (self->shown) {
        Beanstalk_Unknown6();
        Beanstalk_Unknown7();
        Beanstalk_CheckPlayerCollisions_Plant();
        if (RSDK.CheckOnScreen(self, NULL)) {
            if (self->animator2.frameID == 5 && self->animator2.timer == 1)
                RSDK.PlaySfx(Beanstalk->sfxBeanChomp, false, 255);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Beanstalk_EditorDraw(void)
{
    RSDK_THIS(Beanstalk);

    self->bezCtrlAngle &= 0xFF;
    self->field_74 = -1;
    self->field_78 = 0x200;
    self->field_8C = self->position;
    self->field_94 = self->position;
    self->field_9C = 0;
    self->timer    = 0;
    self->field_A0 = 0;

    switch (self->type) {
        case 0:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 4, &self->animator1, true, 9);
            break;
        case 1:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 2, &self->animator2, true, 0);
            break;
        case 3:
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Beanstalk->aniFrames, 3, &self->animator2, true, 0);
            break;
        default: break;
    }

    Beanstalk_Unknown4();
    Beanstalk_Unknown5();

    self->scale.x = self->field_78;
    self->scale.y = self->field_78;
    self->drawFX  = FX_SCALE | FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->type > 1)
        RSDK.DrawSprite(&self->animator2, NULL, false);
    self->drawFX  = FX_FLIP;
    self->scale.x = 0x200;
    self->scale.y = 0x200;
}

void Beanstalk_EditorLoad(void) { Beanstalk->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Beanstalk.bin", SCOPE_STAGE); }
#endif

void Beanstalk_Serialize(void)
{
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlAngle);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_INT32, bezCtrlLength);
    RSDK_EDITABLE_VAR(Beanstalk, VAR_BOOL, forceEnd);
}

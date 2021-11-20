#include "SonicMania.h"

ObjectDDWrecker *DDWrecker;

void DDWrecker_Update(void)
{
    RSDK_THIS(DDWrecker);
    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);

    if (self->type != DDWRECKER_CORE) {
        if (self->invincible > 0)
            self->invincible--;
        StateMachine_Run(self->lateState);
    }
}

void DDWrecker_LateUpdate(void) {}

void DDWrecker_StaticUpdate(void) {}

void DDWrecker_Draw(void)
{
    RSDK_THIS(DDWrecker);
    if (self->type == DDWRECKER_MAIN_1 || self->type == DDWRECKER_MAIN_2) {
        RSDK.SetActivePalette(1, 0, ScreenInfo->height);
        self->direction = self->animator.frameID >= 16;
        if ((self->invincible & 2) == 0) {
            RSDK.SetLimitedFade(1, 2, 3, self->blendAmount, 1, 28);
        }
        else {
            for (int32 i = 1; i < 0x1C; ++i) {
                RSDK.SetPaletteEntry(1, i, 0xF0F0F0);
            }
        }
    }
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.SetActivePalette(0, 0, ScreenInfo->height);
}

void DDWrecker_Create(void *data)
{
    RSDK_THIS(DDWrecker);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK || globals->gameMode >= MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else if (data) {
            self->type   = voidToInt(data);
            self->active = ACTIVE_NORMAL;
            switch (self->type) {
                case DDWRECKER_MAIN_1: // main body
                case DDWRECKER_MAIN_2:
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
                    self->drawFX        = FX_FLIP | FX_ROTATE;
                    self->hitbox.left   = -20;
                    self->hitbox.top    = -20;
                    self->hitbox.right  = 20;
                    self->hitbox.bottom = 20;
                    self->health        = 3;
                    break;
                case DDWRECKER_CHAIN: // chains
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 4, &self->animator, true, 0);
                    break;
                case DDWRECKER_CORE: // core
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 5, &self->animator, true, 0);
                    break;
                default: break;
            }
            self->visible   = true;
            self->drawOrder = Zone->drawOrderLow;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->visible       = false;
            self->state         = DDWrecker_State_Init;
        }
    }
}

void DDWrecker_StageLoad(void)
{
    DDWrecker->aniFrames   = RSDK.LoadSpriteAnimation("GHZ/DDWrecker.bin", SCOPE_STAGE);
    DDWrecker->sfxBossHit   = RSDK.GetSfx("Stage/BossHit.wav");
    DDWrecker->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    DDWrecker->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    DDWrecker->sfxImpact    = RSDK.GetSfx("Stage/Impact2.wav");
    DDWrecker->sfxAssemble  = RSDK.GetSfx("Stage/Assemble.wav");
    DDWrecker->sfxRotate    = RSDK.GetSfx("Stage/Rotate.wav");
    DDWrecker->sfxSharp     = RSDK.GetSfx("Stage/Sharp.wav");
}

void DDWrecker_State_Init(void)
{
    RSDK_THIS(DDWrecker);
    if (++self->timer >= 8) {
        RSDKScreenInfo *screen             = ScreenInfo;
        self->timer                  = 0;
        Zone->playerBoundActiveL[0]    = true;
        Zone->screenBoundsL1[0]        = (self->position.x >> 0x10) - screen->centerX;
        Zone->playerBoundActiveR[0]    = true;
        Zone->screenBoundsR1[0]        = screen->centerX + (self->position.x >> 0x10);
        Zone->playerBoundActiveB[0]    = true;
        Zone->screenBoundsB1[0]        = (self->position.y >> 0x10);
        DDWrecker->camBoundL           = self->position.x + ((160 - screen->centerX) << 16);
        DDWrecker->camBoundR           = self->position.x + ((screen->centerX - 160) << 16);
        DDWrecker->bossBoundL          = self->position.x + ((32 - screen->centerX) << 16);
        DDWrecker->bossBoundR          = self->position.x + ((screen->centerX - 32) << 16);
        DDWrecker->bossBoundT          = self->position.y - 0xC00000;
        DDWrecker->xVelocityUnknown[0] = screen->centerX - 168;
        if (DDWrecker->xVelocityUnknown[0] < 0) {
            DDWrecker->xVelocityUnknown[0] = 0;
        }
        if (DDWrecker->xVelocityUnknown[0] > 24) {
            DDWrecker->xVelocityUnknown[0] = 24;
        }
        DDWrecker->xVelocityUnknown[0] = (DDWrecker->xVelocityUnknown[0] + 32) << 11;
        DDWrecker->xVelocityUnknown[1] = DDWrecker->xVelocityUnknown[0] >> 2;
        DDWrecker->xVelocityUnknown[2] = 288 * DDWrecker->xVelocityUnknown[0] >> 8;
        self->state                  = DDWrecker_State_InitChildren;
    }
}

void DDWrecker_State_InitChildren(void)
{
    RSDK_THIS(DDWrecker);
    if (self->timer) {
        self->timer++;
        if (self->timer == 60) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_CHAIN));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_CHAIN));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_CHAIN));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 5, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_CHAIN));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_CORE));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x200000;
            child->startPos.x = self->position.x;
            child->startPos.y = self->position.y;
            child->startPos.y -= 0x800000;
            child->velocity.y = -0x98000;
            child->slots[0]   = SceneInfo->entitySlot + 6;
            child->slots[1]   = SceneInfo->entitySlot + 1;
            child->slots[2]   = SceneInfo->entitySlot + 2;
            child->slots[3]   = SceneInfo->entitySlot + 4;
            child->slots[4]   = SceneInfo->entitySlot + 5;
            child->slots[5]   = SceneInfo->entitySlot + 7;
            child->bodyA      = RSDK.GetEntityByID(SceneInfo->entitySlot + 6);
            child->bodyB      = RSDK.GetEntityByID(SceneInfo->entitySlot + 7);
            child->state      = DDWrecker_State_Assemble;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker); // bodyA
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_MAIN_1));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;
            child->slots[0]  = SceneInfo->entitySlot + 1;
            child->slots[1]  = SceneInfo->entitySlot + 2;
            child->slots[2]  = SceneInfo->entitySlot + 3;
            child->slots[3]  = SceneInfo->entitySlot + 4;
            child->slots[4]  = SceneInfo->entitySlot + 5;
            child->slots[5]  = SceneInfo->entitySlot + 7;
            child->bodyA     = RSDK.GetEntityByID(SceneInfo->entitySlot + 6);
            child->bodyB     = RSDK.GetEntityByID(SceneInfo->entitySlot + 7);
            child->arcOffset = 64;

            child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker); // bodyB
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(DDWRECKER_MAIN_2));
            child->position.x = self->position.x;
            child->position.y = self->position.y;
            child->position.y += 0x400000;
            child->slots[0]  = SceneInfo->entitySlot + 1;
            child->slots[1]  = SceneInfo->entitySlot + 2;
            child->slots[2]  = SceneInfo->entitySlot + 3;
            child->slots[3]  = SceneInfo->entitySlot + 4;
            child->slots[4]  = SceneInfo->entitySlot + 5;
            child->slots[5]  = SceneInfo->entitySlot + 6;
            child->bodyB     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker);
            child->bodyA     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker);
            child->arcOffset = 64;

            destroyEntity(self);
        }
    }
    else {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player->position.x > self->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->timer;
        }
    }
}

void DDWrecker_State_Assemble(void)
{
    RSDK_THIS(DDWrecker);
    self->velocity.y += 0x4000;
    self->position.y += self->velocity.y;
    if (self->position.y > self->startPos.y && self->velocity.y > 0) {
        self->position.y = self->startPos.y;
        self->velocity.y = 0;
        self->arcOffset  = 2048;
        self->timer2     = 128;
        self->state      = DDWrecker_State_Unknown2;
        RSDK.PlaySfx(DDWrecker->sfxAssemble, 0, 255);
    }
}
void DDWrecker_State_Unknown2(void)
{
    RSDK_THIS(DDWrecker);
    self->angle3 = (self->angle3 + (self->timer2 >> 1)) & 0x3FF;
    self->arcOffset += (64 - self->arcOffset) >> 4;
    self->angle      = (uint8)(self->angle + 2);
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;
    DDWrecker_Spin();

    if (self->timer2 <= 0) {
        self->timer = 30;
        self->state = DDWrecker_State_Unknown3;
        foreach_active(DDWrecker, child)
        {
            if (child->type == DDWRECKER_MAIN_1 || child->type == DDWRECKER_MAIN_2)
                child->lateState = DDWrecker_LateState_Unknown1;
        }
    }
    else {
        self->timer2--;
    }
}
void DDWrecker_State_Unknown3(void)
{
    RSDK_THIS(DDWrecker);
    self->angle      = (uint8)(self->angle + 2);
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (--self->timer < 1) {
        EntityDDWrecker *entityA = self->bodyA;
        EntityDDWrecker *entityB = self->bodyB;
        if (entityA->position.y - entityB->position.y < 0) {
            entityA->startPos.x = self->startPos.x;
            entityA->startPos.y = self->startPos.y;
            entityA->startPos.y -= 0x400000;
            entityA->velocity.x = 0;
            entityA->angle      = self->angle;
            entityA->angle3     = 0;
            entityA->timer      = 4;
            entityA->state      = DDWrecker_State_Unknown5;
        }
        else {
            entityB->startPos.x = self->startPos.x;
            entityB->startPos.y = self->startPos.y;
            entityB->startPos.y -= 0x400000;
            entityA->velocity.x = 0;
            entityB->angle      = self->angle;
            entityB->angle3     = 512;
            entityB->timer      = 4;
            entityB->state      = DDWrecker_State_Unknown4;
        }
        if (self->flag == 1) {
            entityA->lateState = DDWrecker_LateState_Unknown1;
            entityB->lateState = DDWrecker_LateState_Unknown2;
        }
        else {
            entityA->lateState = DDWrecker_LateState_Unknown2;
            entityB->lateState = DDWrecker_LateState_Unknown1;
        }
        RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
        self->state = StateMachine_None;
    }
}
void DDWrecker_State_Unknown4(void)
{
    RSDK_THIS(DDWrecker);
    if (self->angle3 < 0x200) {
        self->position.x += self->velocity.x;
        if (self->position.x < DDWrecker->camBoundL)
            self->position.x = DDWrecker->camBoundL;
    }
    self->angle3 += 4;
    self->angle3 &= 0x3FF;
    self->angle      = (uint8)(self->angle + 2);
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin2();
    if (self->angle3 >= 768) {
        if (--self->timer <= 0) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(self->slots[2], DDWrecker);
            child->velocity.x      = (child->startPos.x - child->position.x) >> 6;
            child->velocity.y      = (child->startPos.y - child->position.y) >> 6;
            child->timer2          = -2;
            child->angle3          = self->angle3;
            child->angle           = 0;
            child->timer           = 64;
            child->state           = DDWrecker_State_Unknown6;
            if (self == self->bodyB)
                child->angle3 += 0x200;
            self->state = StateMachine_None;

            EntityDDWrecker *bodyA = self->bodyA;
            EntityDDWrecker *bodyB = self->bodyB;
            bodyA->lateState       = DDWrecker_LateState_Unknown2;
            bodyB->lateState       = DDWrecker_LateState_Unknown2;
            RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
        }
        else {
            if (self->velocity.x)
                self->velocity.x = DDWrecker->xVelocityUnknown[0];
            else
                self->velocity.x = DDWrecker->xVelocityUnknown[1];

            self->state = DDWrecker_State_Unknown5;
            if (self->timer == 2) {
                EntityDDWrecker *bodyA = self->bodyA;
                EntityDDWrecker *bodyB = self->bodyB;

                if (bodyA->lateState == DDWrecker_LateState_Unknown1)
                    bodyA->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyA->lateState = DDWrecker_LateState_Unknown1;

                if (bodyB->lateState == DDWrecker_LateState_Unknown1)
                    bodyB->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyB->lateState = DDWrecker_LateState_Unknown1;
                RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
            }
        }
    }
}
void DDWrecker_State_Unknown5(void)
{
    RSDK_THIS(DDWrecker);
    if (self->angle3 > 511) {
        self->position.x += self->velocity.x;
        if (self->position.x > DDWrecker->camBoundR)
            self->position.x = DDWrecker->camBoundR;
    }
    self->angle3 += 4;
    self->angle3 &= 0x3FF;
    self->angle      = (uint8)(self->angle + 2);
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin2();
    if (self->angle3 >= 256 && self->angle3 < 512) {
        if (--self->timer <= 0) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(self->slots[2], DDWrecker);
            child->velocity.x      = (child->startPos.x - child->position.x) >> 6;
            child->velocity.y      = (child->startPos.y - child->position.y) >> 6;
            child->timer2          = 2;
            child->angle3          = self->angle3;
            child->angle           = 0;
            child->timer           = 64;
            child->state           = DDWrecker_State_Unknown6;
            if (self == self->bodyB)
                child->angle3 += 512;
            self->state = StateMachine_None;

            EntityDDWrecker *bodyA = self->bodyA;
            EntityDDWrecker *bodyB = self->bodyB;
            bodyA->lateState       = DDWrecker_LateState_Unknown2;
            bodyB->lateState       = DDWrecker_LateState_Unknown2;
            RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
        }
        else {
            if (self->velocity.x)
                self->velocity.x = -DDWrecker->xVelocityUnknown[0];
            else
                self->velocity.x = -DDWrecker->xVelocityUnknown[1];

            self->state = DDWrecker_State_Unknown4;
            if (self->timer == 2) {
                EntityDDWrecker *bodyA = self->bodyA;
                EntityDDWrecker *bodyB = self->bodyB;

                if (bodyA->lateState == DDWrecker_LateState_Unknown1)
                    bodyA->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyA->lateState = DDWrecker_LateState_Unknown1;

                if (bodyB->lateState == DDWrecker_LateState_Unknown1)
                    bodyB->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyB->lateState = DDWrecker_LateState_Unknown1;
                RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
            }
        }
    }
}
void DDWrecker_State_Unknown6(void)
{
    RSDK_THIS(DDWrecker);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->velocity.x > 0) {
        if (self->position.x > self->startPos.x) {
            self->velocity.x = 0;
            self->position.x = self->startPos.x;
        }
    }
    else if (self->velocity.x < 0) {
        if (self->position.x < self->startPos.x) {
            self->velocity.x = 0;
            self->position.x = self->startPos.x;
        }
    }

    if (self->velocity.y < 0) {
        if (self->position.y < self->startPos.y) {
            self->velocity.y = 0;
            self->position.y = self->startPos.y;
        }
    }
    else if (self->velocity.y > 0) {
        if (self->position.y > self->startPos.y) {
            self->velocity.y = 0;
            self->position.y = self->startPos.y;
        }
    }

    if (!self->velocity.x && !self->velocity.y) {
        self->angle += 2;
        self->angle &= 0xFF;
        self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;
    }

    if (self->timer2 <= 0) {
        if (self->timer2 > -141) {
            self->timer2--;
        }
        else {
            self->state = DDWrecker_State_Unknown7;
            RSDK.PlaySfx(DDWrecker->sfxAssemble, 0, 255);
        }
    }
    else if (self->timer2 >= 144) {
        self->state = DDWrecker_State_Unknown7;
        RSDK.PlaySfx(DDWrecker->sfxAssemble, 0, 255);
    }
    else {
        self->timer2++;
    }

    self->angle3 = (self->angle3 + (self->timer2 >> 2)) & 0x3FF;
    DDWrecker_Spin();
}
void DDWrecker_State_Unknown7(void)
{
    RSDK_THIS(DDWrecker);
    self->angle3 = (self->angle3 + (self->timer2 >> 2)) & 0x3FF;

    self->angle += 2;
    self->angle &= 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (!--self->timer) {
        self->state = DDWrecker_State_Unknown8;

        EntityDDWrecker *bodyA = self->bodyA;
        EntityDDWrecker *bodyB = self->bodyB;
        bodyA->lateState       = DDWrecker_LateState_Unknown1;
        bodyB->lateState       = DDWrecker_LateState_Unknown1;
    }
}
void DDWrecker_State_Unknown8(void)
{
    RSDK_THIS(DDWrecker);
    if (self->timer2 <= 0) {
        if (self->timer2 < -8)
            self->timer2++;
    }
    else {
        if (self->timer2 > 8)
            self->timer2--;
    }
    self->angle3 = (self->angle3 + (self->timer2 >> 2)) & 0x3FF;

    self->angle += 2;
    self->angle &= 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (!(self->angle3 & 0x1FC)) {
        if (abs(self->timer2) <= 8) {
            self->timer2 = 0;
            self->timer  = 30;
            self->angle3 = (self->angle3 + 0x80) & 0x300;
            self->state  = DDWrecker_State_Unknown3;
        }
    }
}
void DDWrecker_State_Unknown9(void)
{
    RSDK_THIS(DDWrecker);
    self->velocity.y -= 0x800;
    self->position.x += self->velocity.x;
    self->startPos.y += self->velocity.y;

    if (self->startPos.y < DDWrecker->bossBoundT) {
        self->startPos.y = DDWrecker->bossBoundT;
        self->velocity.y = 0;
    }

    self->angle += 2;
    self->angle &= 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    self->timer4 = 8;
    if (self->position.x <= DDWrecker->bossBoundL) {
        self->position.x = DDWrecker->bossBoundL;
        self->velocity.x = 0;
        if (self->startPos.y <= DDWrecker->bossBoundT)
            self->state = DDWrecker_State_Unknown10;
    }
    else if (self->position.x >= DDWrecker->bossBoundR) {
        self->position.x = DDWrecker->bossBoundR;
        self->velocity.x = 0;
        if (self->startPos.y <= DDWrecker->bossBoundT)
            self->state = DDWrecker_State_Unknown10;
    }

    if (self->rotation < 0) {
        if (self->rotation + 2 > 0)
            self->rotation = 0;
        else
            self->rotation += 2;
    }
    else if (self->rotation > 0) {
        self->rotation -= 2;
        if (self->rotation < 0)
            self->rotation = 0;
    }

    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else {
        if (self->animator.animationID == 3 && !self->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }
}
void DDWrecker_State_Unknown10(void)
{
    RSDK_THIS(DDWrecker);
    self->angle += 2;
    self->angle &= 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->startPos.y) & 0xFFFF0000;

    if (self->animator.animationID) {
        if (self->animator.animationID == 1) {
            if (self->animator.frameID == self->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 3, &self->animator, true, 0);
        }
        else {
            if (self->animator.animationID == 3) {
                if (self->animator.animationSpeed < 0x100) {
                    self->animator.animationSpeed += 2;
                }
            }
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 1, &self->animator, true, 0);
        RSDK.PlaySfx(DDWrecker->sfxSharp, 0, 255);
    }

    if (++self->timer == 30) {
        self->timer      = 0;
        self->velocity.y = -0x20000;
        self->state      = DDWrecker_State_Unknown11;
        RSDK.PlaySfx(DDWrecker->sfxDrop, 0, 255);
    }
}
void DDWrecker_State_Unknown11(void)
{
    RSDK_THIS(DDWrecker);
    self->position.x += self->velocity.x;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    self->animator.animationSpeed = 0x100 - 32 * self->timer;
    if (self->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x180000, true)) {
            ++self->timer;
            Camera_ShakeScreen(0, 0, 3);
            RSDK.PlaySfx(DDWrecker->sfxImpact, 0, 255);
            self->velocity.y = (self->velocity.y >> 3) - self->velocity.y;
            if (self->timer - 1) {
                if (!(self->timer - 3)) {
                    self->velocity.y = 0;
                    self->timer      = 0;
                    self->startPos.y = self->position.y;
                    self->timer4     = 8;
                    self->state      = DDWrecker_State_Unknown12;
                }
            }
            else {
                if (self->position.x - (DDWrecker->bossBoundL + 0x100000) > 0)
                    self->velocity.x = -DDWrecker->xVelocityUnknown[2];
                else
                    self->velocity.x = DDWrecker->xVelocityUnknown[2];
            }
        }
    }
}
void DDWrecker_State_Unknown12(void)
{
    RSDK_THIS(DDWrecker);
    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else {
        if (self->animator.animationID == 3 && !self->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }

    if (++self->timer == 30) {
        self->timer = 0;
        self->state = DDWrecker_State_Unknown9;
    }
}
void DDWrecker_LateState_Unknown1(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!self->invincible && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            DDWrecker_Hit();
            RSDK.PlaySfx(DDWrecker->sfxBossHit, 0, 255);
        }
    }

    if (self->timer4 > 0) {
        if (self->timer4 > 8)
            self->timer4--;

        self->angle2 += self->timer4;
        if (self->angle2 > 1023) {
            self->angle2 = 0;
            self->timer4 = 0;
        }
    }

    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else if (self->animator.animationID == 3 && !self->animator.frameID) {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }

    if (self->blendAmount > 0) {
        self->blendAmount -= 16;
    }
}
void DDWrecker_LateState_Unknown2(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!self->invincible && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || self->animator.animationID < 3) {
                if (Player_CheckBossHit(player, self)) {
                    DDWrecker_Hit();
                    RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
                }
            }
            else {
                Player_CheckHit(player, self);
            }
        }
    }

    if (self->animator.animationID) {
        if (self->animator.animationID == 1) {
            if (self->animator.frameID == self->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 3, &self->animator, true, 0);
        }
        else if (self->animator.animationID == 3) {
            if (self->animator.animationSpeed < 0x100)
                self->animator.animationSpeed += 2;
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 1, &self->animator, true, 0);
    }

    if (self->timer4 < 48)
        self->timer4 += 2;

    self->angle2 = (self->angle2 + self->timer4) & 0x3FF;
    if (self->blendAmount < 256) {
        self->blendAmount += 16;
    }
}
void DDWrecker_LateState_Unknown3(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!self->invincible && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || self->animator.animationID < 3) {
                if (Player_CheckBossHit(player, self)) {
                    DDWrecker_Hit();
                    RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
                }
            }
            else {
                Player_CheckHit(player, self);
            }
        }
    }

    self->angle2 = (self->angle2 + self->timer4) & 0x3FF;
    if (self->animator.animationID == 0) {
        if (self->blendAmount > 0) {
            self->blendAmount -= 16;
        }
    }
    else if (self->blendAmount < 256) {
        self->blendAmount += 16;
    }
}
void DDWrecker_Hit(void)
{
    RSDK_THIS(DDWrecker);
    if (--self->health <= 0) {
        self->state     = DDWrecker_State_Die;
        self->lateState = StateMachine_None;
        self->timer     = 0;
        foreach_active(DDWrecker, child)
        {
            if (self != child) {
                switch (child->type) {
                    case DDWRECKER_MAIN_1:
                    case DDWRECKER_MAIN_2:
                        if (child->lateState) {
                            child->state      = DDWrecker_State_Unknown12;
                            child->startPos.y = child->position.y;
                            child->velocity.x = -DDWrecker->xVelocityUnknown[2];
                            child->lateState  = DDWrecker_LateState_Unknown3;
                        }
                        break;
                    case DDWRECKER_CHAIN:
                    case DDWRECKER_CORE:
                        if (child->state != DDWrecker_State_Debris) {
                            child->state      = DDWrecker_State_Debris;
                            child->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                            child->velocity.y = RSDK.Rand(-0x20000, 0x20000);
                        }
                        break;
                    default: continue;
                }
            }
        }
    }
    else {
        self->invincible = 48;
        RSDK.PlaySfx(DDWrecker->sfxBossHit, 0, 255);
    }
}
void DDWrecker_Spin(void)
{
    RSDK_THIS(DDWrecker);
    for (int32 i = 0; i < 6; ++i) {
        EntityDDWrecker *child = RSDK_GET_ENTITY(self->slots[i], DDWrecker);
        child->position.x      = DDWrecker->angleOffsets1[i] * RSDK.Sin1024(self->angle3) * self->arcOffset;
        child->position.y      = DDWrecker->angleOffsets1[i] * RSDK.Cos1024(self->angle3) * self->arcOffset;
        child->position.x += self->position.x;
        child->position.y += self->position.y;

        int32 rot = child->rotation;
        if (rot < 0) {
            if (rot < 0) {
                rot += 2;
                if (rot > 0)
                    child->rotation = 0;
                else
                    child->rotation = rot;
            }
        }
        else if (rot > 0) {
            rot -= 2;
            if (rot - 2 < 0)
                child->rotation = 0;
            else
                child->rotation = rot;
        }
    }
}
void DDWrecker_Spin2(void)
{
    RSDK_THIS(DDWrecker);
    int32 angle        = RSDK.Sin1024(self->angle3) >> 2;
    self->rotation = RSDK.Sin1024(-self->angle3) >> 6;
    if (self->timer4 == 0)
        self->angle2 = RSDK.Sin1024(-self->angle3) >> 5;

    EntityDDWrecker *child = NULL;
    for (int32 i = 0; i < 6; ++i) {
        child             = RSDK_GET_ENTITY(self->slots[i], DDWrecker);
        child->position.x = DDWrecker->angleOffsets2[i] * RSDK.Sin1024(angle) * self->arcOffset;
        child->position.y = DDWrecker->angleOffsets2[i] * RSDK.Cos1024(angle) * self->arcOffset;
        child->position.x += self->position.x;
        child->position.y += self->position.y;
    }
    child->rotation = RSDK.Sin1024(-self->angle3) >> 5;
}
void DDWrecker_State_Debris(void)
{
    RSDK_THIS(DDWrecker);
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}
void DDWrecker_State_Die(void)
{
    RSDK_THIS(DDWrecker);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(DDWrecker->sfxExplosion, 0, 255);
        if (Zone->timer & 4) {
            int x = self->position.x + (RSDK.Rand(-20, 20) << 16);
            int y = self->position.y + (RSDK.Rand(-20, 20) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 80) {
        int32 cnt = 0;
        foreach_active(DDWrecker, child) { ++cnt; }
        if (cnt != 1) {
            destroyEntity(self);
        }
        else {
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            self->timer               = 0;
            self->visible             = false;
            self->state               = DDWrecker_State_SpawnSignpost;
            SceneInfo->timeEnabled = false;
            Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        }
    }
}
void DDWrecker_State_SpawnSignpost(void)
{
    RSDK_THIS(DDWrecker);
    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, 0, 255);
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void DDWrecker_EditorDraw(void)
{
    RSDK_THIS(DDWrecker);

    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, false, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 8, -212, -SCREEN_YSIZE, 212, 0);
    }
}

void DDWrecker_EditorLoad(void)
{
    DDWrecker->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DDWrecker.bin", SCOPE_STAGE);
}
#endif

void DDWrecker_Serialize(void) {}

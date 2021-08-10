#include "SonicMania.h"

ObjectDDWrecker *DDWrecker;

void DDWrecker_Update(void)
{
    RSDK_THIS(DDWrecker);
    StateMachine_Run(entity->state);

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->type != 4) {
        if (entity->invincible > 0)
            entity->invincible--;
        StateMachine_Run(entity->lateState);
    }
}

void DDWrecker_LateUpdate(void) {}

void DDWrecker_StaticUpdate(void) {}

void DDWrecker_Draw(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->type == 1 || entity->type == 2) {
        RSDK.SetActivePalette(1, 0, RSDK_screens->height);
        entity->direction = entity->animator.frameID >= 16;
        if ((entity->invincible & 2) == 0) {
            RSDK.SetLimitedFade(1, 2, 3, entity->blendAmount, 1, 28);
        }
        else {
            for (int i = 1; i < 0x1C; ++i) {
                RSDK.SetPaletteEntry(1, i, 0xF0F0F0);
            }
        }
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void DDWrecker_Create(void *data)
{
    RSDK_THIS(DDWrecker);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK || globals->gameMode >= MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else if (data) {
            entity->type   = voidToInt(data);
            entity->active = ACTIVE_NORMAL;
            switch (entity->type) {
                case 1: // main body
                case 2:
                    RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 0, &entity->animator, true, 0);
                    entity->drawFX        = FX_FLIP | FX_ROTATE;
                    entity->hitbox.left   = -20;
                    entity->hitbox.top    = -20;
                    entity->hitbox.right  = 20;
                    entity->hitbox.bottom = 20;
                    entity->health        = 3;
                    break;
                case 3: // chains
                    RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 4, &entity->animator, true, 0);
                    break;
                case 4: // core
                    RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 5, &entity->animator, true, 0);
                    break;
                default: break;
            }
            entity->visible   = true;
            entity->drawOrder = Zone->drawOrderLow;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->visible       = false;
            entity->state         = DDWrecker_State_Init;
        }
    }
}

void DDWrecker_StageLoad(void)
{
    DDWrecker->spriteIndex   = RSDK.LoadSpriteAnimation("GHZ/DDWrecker.bin", SCOPE_STAGE);
    DDWrecker->sfx_BossHit   = RSDK.GetSFX("Stage/BossHit.wav");
    DDWrecker->sfx_Explosion = RSDK.GetSFX("Stage/Explosion2.wav");
    DDWrecker->sfx_Drop      = RSDK.GetSFX("Stage/Drop.wav");
    DDWrecker->sfx_Impact    = RSDK.GetSFX("Stage/Impact2.wav");
    DDWrecker->sfx_Assemble  = RSDK.GetSFX("Stage/Assemble.wav");
    DDWrecker->sfx_Rotate    = RSDK.GetSFX("Stage/Rotate.wav");
    DDWrecker->sfx_Sharp     = RSDK.GetSFX("Stage/Sharp.wav");
}

void DDWrecker_State_Init(void)
{
    RSDK_THIS(DDWrecker);
    if (++entity->timer >= 8) {
        ScreenInfo *screen             = RSDK_screens;
        entity->timer                  = 0;
        Zone->playerBoundActiveL[0]    = true;
        Zone->screenBoundsL1[0]        = (entity->position.x >> 0x10) - screen->centerX;
        Zone->playerBoundActiveR[0]    = true;
        Zone->screenBoundsR1[0]        = screen->centerX + (entity->position.x >> 0x10);
        Zone->playerBoundActiveB[0]    = true;
        Zone->screenBoundsB1[0]        = (entity->position.y >> 0x10);
        DDWrecker->camBoundL           = entity->position.x + ((160 - screen->centerX) << 16);
        DDWrecker->camBoundR           = entity->position.x + ((screen->centerX - 160) << 16);
        DDWrecker->bossBoundL          = entity->position.x + ((32 - screen->centerX) << 16);
        DDWrecker->bossBoundR          = entity->position.x + ((screen->centerX - 32) << 16);
        DDWrecker->bossBoundT          = entity->position.y - 0xC00000;
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
        entity->state                  = DDWrecker_State_InitChildren;
    }
}

void DDWrecker_State_InitChildren(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 60) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(3));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(3));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 4, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(3));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 5, DDWrecker);
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(3));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 3, DDWrecker); // main
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(4));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x200000;
            child->startPos.x = entity->position.x;
            child->startPos.y = entity->position.y;
            child->startPos.y -= 0x800000;
            child->velocity.y = -0x98000;
            child->slots[0]   = RSDK_sceneInfo->entitySlot + 6;
            child->slots[1]   = RSDK_sceneInfo->entitySlot + 1;
            child->slots[2]   = RSDK_sceneInfo->entitySlot + 2;
            child->slots[3]   = RSDK_sceneInfo->entitySlot + 4;
            child->slots[4]   = RSDK_sceneInfo->entitySlot + 5;
            child->slots[5]   = RSDK_sceneInfo->entitySlot + 7;
            child->bodyA      = RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 6);
            child->bodyB      = RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 7);
            child->state      = DDWrecker_State_Assemble;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 6, DDWrecker); // bodyA
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(1));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;
            child->slots[0]  = RSDK_sceneInfo->entitySlot + 1;
            child->slots[1]  = RSDK_sceneInfo->entitySlot + 2;
            child->slots[2]  = RSDK_sceneInfo->entitySlot + 3;
            child->slots[3]  = RSDK_sceneInfo->entitySlot + 4;
            child->slots[4]  = RSDK_sceneInfo->entitySlot + 5;
            child->slots[5]  = RSDK_sceneInfo->entitySlot + 7;
            child->bodyA     = RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 6);
            child->bodyB     = RSDK.GetEntityByID(RSDK_sceneInfo->entitySlot + 7);
            child->arcOffset = 64;

            child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 7, DDWrecker); // bodyB
            RSDK.ResetEntityPtr(child, DDWrecker->objectID, intToVoid(2));
            child->position.x = entity->position.x;
            child->position.y = entity->position.y;
            child->position.y += 0x400000;
            child->slots[0]  = RSDK_sceneInfo->entitySlot + 1;
            child->slots[1]  = RSDK_sceneInfo->entitySlot + 2;
            child->slots[2]  = RSDK_sceneInfo->entitySlot + 3;
            child->slots[3]  = RSDK_sceneInfo->entitySlot + 4;
            child->slots[4]  = RSDK_sceneInfo->entitySlot + 5;
            child->slots[5]  = RSDK_sceneInfo->entitySlot + 6;
            child->bodyB     = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 7, DDWrecker);
            child->bodyA     = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 6, DDWrecker);
            child->arcOffset = 64;

            destroyEntity(entity);
        }
    }
    else {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player->position.x > entity->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++entity->timer;
        }
    }
}

void DDWrecker_State_Assemble(void)
{
    RSDK_THIS(DDWrecker);
    entity->velocity.y += 0x4000;
    entity->position.y += entity->velocity.y;
    if (entity->position.y > entity->startPos.y && entity->velocity.y > 0) {
        entity->position.y = entity->startPos.y;
        entity->velocity.y = 0;
        entity->arcOffset  = 2048;
        entity->timer2     = 128;
        entity->state      = DDWrecker_State_Unknown2;
        RSDK.PlaySFX(DDWrecker->sfx_Assemble, 0, 255);
    }
}
void DDWrecker_State_Unknown2(void)
{
    RSDK_THIS(DDWrecker);
    entity->angle3 = (entity->angle3 + (entity->timer2 >> 1)) & 0x3FF;
    entity->arcOffset += (64 - entity->arcOffset) >> 4;
    entity->angle      = (byte)(entity->angle + 2);
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;
    DDWrecker_Spin();

    if (entity->timer2 <= 0) {
        entity->timer = 30;
        entity->state = DDWrecker_State_Unknown3;
        foreach_active(DDWrecker, child)
        {
            if (child->type == 1 || child->type == 2)
                child->lateState = DDWrecker_LateState_Unknown1;
        }
    }
    else {
        entity->timer2--;
    }
}
void DDWrecker_State_Unknown3(void)
{
    RSDK_THIS(DDWrecker);
    entity->angle      = (byte)(entity->angle + 2);
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (--entity->timer < 1) {
        EntityDDWrecker *entityA = entity->bodyA;
        EntityDDWrecker *entityB = entity->bodyB;
        if (entityA->position.y - entityB->position.y < 0) {
            entityA->startPos.x = entity->startPos.x;
            entityA->startPos.y = entity->startPos.y;
            entityA->startPos.y -= 0x400000;
            entityA->velocity.x = 0;
            entityA->angle      = entity->angle;
            entityA->angle3     = 0;
            entityA->timer      = 4;
            entityA->state      = DDWrecker_State_Unknown5;
        }
        else {
            entityB->startPos.x = entity->startPos.x;
            entityB->startPos.y = entity->startPos.y;
            entityB->startPos.y -= 0x400000;
            entityA->velocity.x = 0;
            entityB->angle      = entity->angle;
            entityB->angle3     = 512;
            entityB->timer      = 4;
            entityB->state      = DDWrecker_State_Unknown4;
        }
        if (entity->flag == 1) {
            entityA->lateState = DDWrecker_LateState_Unknown1;
            entityB->lateState = DDWrecker_LateState_Unknown2;
        }
        else {
            entityA->lateState = DDWrecker_LateState_Unknown2;
            entityB->lateState = DDWrecker_LateState_Unknown1;
        }
        RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
        entity->state = StateMachine_None;
    }
}
void DDWrecker_State_Unknown4(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->angle3 < 0x200) {
        entity->position.x += entity->velocity.x;
        if (entity->position.x < DDWrecker->camBoundL)
            entity->position.x = DDWrecker->camBoundL;
    }
    entity->angle3 += 4;
    entity->angle3 &= 0x3FF;
    entity->angle      = (byte)(entity->angle + 2);
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin2();
    if (entity->angle3 >= 768) {
        if (--entity->timer <= 0) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(entity->slots[2], DDWrecker);
            child->velocity.x      = (child->startPos.x - child->position.x) >> 6;
            child->velocity.y      = (child->startPos.y - child->position.y) >> 6;
            child->timer2          = -2;
            child->angle3          = entity->angle3;
            child->angle           = 0;
            child->timer           = 64;
            child->state           = DDWrecker_State_Unknown6;
            if (entity == entity->bodyB)
                child->angle3 += 0x200;
            entity->state = StateMachine_None;

            EntityDDWrecker *bodyA = entity->bodyA;
            EntityDDWrecker *bodyB = entity->bodyB;
            bodyA->lateState       = DDWrecker_LateState_Unknown2;
            bodyB->lateState       = DDWrecker_LateState_Unknown2;
            RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
        }
        else {
            if (entity->velocity.x)
                entity->velocity.x = DDWrecker->xVelocityUnknown[0];
            else
                entity->velocity.x = DDWrecker->xVelocityUnknown[1];

            entity->state = DDWrecker_State_Unknown5;
            if (entity->timer == 2) {
                EntityDDWrecker *bodyA = entity->bodyA;
                EntityDDWrecker *bodyB = entity->bodyB;

                if (bodyA->lateState == DDWrecker_LateState_Unknown1)
                    bodyA->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyA->lateState = DDWrecker_LateState_Unknown1;

                if (bodyB->lateState == DDWrecker_LateState_Unknown1)
                    bodyB->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyB->lateState = DDWrecker_LateState_Unknown1;
                RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
            }
        }
    }
}
void DDWrecker_State_Unknown5(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->angle3 > 511) {
        entity->position.x += entity->velocity.x;
        if (entity->position.x > DDWrecker->camBoundR)
            entity->position.x = DDWrecker->camBoundR;
    }
    entity->angle3 += 4;
    entity->angle3 &= 0x3FF;
    entity->angle      = (byte)(entity->angle + 2);
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin2();
    if (entity->angle3 >= 256 && entity->angle3 < 512) {
        if (--entity->timer <= 0) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(entity->slots[2], DDWrecker);
            child->velocity.x      = (child->startPos.x - child->position.x) >> 6;
            child->velocity.y      = (child->startPos.y - child->position.y) >> 6;
            child->timer2          = 2;
            child->angle3          = entity->angle3;
            child->angle           = 0;
            child->timer           = 64;
            child->state           = DDWrecker_State_Unknown6;
            if (entity == entity->bodyB)
                child->angle3 += 512;
            entity->state = StateMachine_None;

            EntityDDWrecker *bodyA = entity->bodyA;
            EntityDDWrecker *bodyB = entity->bodyB;
            bodyA->lateState       = DDWrecker_LateState_Unknown2;
            bodyB->lateState       = DDWrecker_LateState_Unknown2;
            RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
        }
        else {
            if (entity->velocity.x)
                entity->velocity.x = -DDWrecker->xVelocityUnknown[0];
            else
                entity->velocity.x = -DDWrecker->xVelocityUnknown[1];

            entity->state = DDWrecker_State_Unknown4;
            if (entity->timer == 2) {
                EntityDDWrecker *bodyA = entity->bodyA;
                EntityDDWrecker *bodyB = entity->bodyB;

                if (bodyA->lateState == DDWrecker_LateState_Unknown1)
                    bodyA->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyA->lateState = DDWrecker_LateState_Unknown1;

                if (bodyB->lateState == DDWrecker_LateState_Unknown1)
                    bodyB->lateState = DDWrecker_LateState_Unknown2;
                else
                    bodyB->lateState = DDWrecker_LateState_Unknown1;
                RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
            }
        }
    }
}
void DDWrecker_State_Unknown6(void)
{
    RSDK_THIS(DDWrecker);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->velocity.x > 0) {
        if (entity->position.x > entity->startPos.x) {
            entity->velocity.x = 0;
            entity->position.x = entity->startPos.x;
        }
    }
    else if (entity->velocity.x < 0) {
        if (entity->position.x < entity->startPos.x) {
            entity->velocity.x = 0;
            entity->position.x = entity->startPos.x;
        }
    }

    if (entity->velocity.y < 0) {
        if (entity->position.y < entity->startPos.y) {
            entity->velocity.y = 0;
            entity->position.y = entity->startPos.y;
        }
    }
    else if (entity->velocity.y > 0) {
        if (entity->position.y > entity->startPos.y) {
            entity->velocity.y = 0;
            entity->position.y = entity->startPos.y;
        }
    }

    if (!entity->velocity.x && !entity->velocity.y) {
        entity->angle += 2;
        entity->angle &= 0xFF;
        entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;
    }

    if (entity->timer2 <= 0) {
        if (entity->timer2 > -141) {
            entity->timer2--;
        }
        else {
            entity->state = DDWrecker_State_Unknown7;
            RSDK.PlaySFX(DDWrecker->sfx_Assemble, 0, 255);
        }
    }
    else if (entity->timer2 >= 144) {
        entity->state = DDWrecker_State_Unknown7;
        RSDK.PlaySFX(DDWrecker->sfx_Assemble, 0, 255);
    }
    else {
        entity->timer2++;
    }

    entity->angle3 = (entity->angle3 + (entity->timer2 >> 2)) & 0x3FF;
    DDWrecker_Spin();
}
void DDWrecker_State_Unknown7(void)
{
    RSDK_THIS(DDWrecker);
    entity->angle3 = (entity->angle3 + (entity->timer2 >> 2)) & 0x3FF;

    entity->angle += 2;
    entity->angle &= 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (!--entity->timer) {
        entity->state = DDWrecker_State_Unknown8;

        EntityDDWrecker *bodyA = entity->bodyA;
        EntityDDWrecker *bodyB = entity->bodyB;
        bodyA->lateState       = DDWrecker_LateState_Unknown1;
        bodyB->lateState       = DDWrecker_LateState_Unknown1;
    }
}
void DDWrecker_State_Unknown8(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->timer2 <= 0) {
        if (entity->timer2 < -8)
            entity->timer2++;
    }
    else {
        if (entity->timer2 > 8)
            entity->timer2--;
    }
    entity->angle3 = (entity->angle3 + (entity->timer2 >> 2)) & 0x3FF;

    entity->angle += 2;
    entity->angle &= 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    DDWrecker_Spin();
    if (!(entity->angle3 & 0x1FC)) {
        if (abs(entity->timer2) <= 8) {
            entity->timer2 = 0;
            entity->timer  = 30;
            entity->angle3 = (entity->angle3 + 0x80) & 0x300;
            entity->state  = DDWrecker_State_Unknown3;
        }
    }
}
void DDWrecker_State_Unknown9(void)
{
    RSDK_THIS(DDWrecker);
    entity->velocity.y -= 0x800;
    entity->position.x += entity->velocity.x;
    entity->startPos.y += entity->velocity.y;

    if (entity->startPos.y < DDWrecker->bossBoundT) {
        entity->startPos.y = DDWrecker->bossBoundT;
        entity->velocity.y = 0;
    }

    entity->angle += 2;
    entity->angle &= 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    entity->timer4 = 8;
    if (entity->position.x <= DDWrecker->bossBoundL) {
        entity->position.x = DDWrecker->bossBoundL;
        entity->velocity.x = 0;
        if (entity->startPos.y <= DDWrecker->bossBoundT)
            entity->state = DDWrecker_State_Unknown10;
    }
    else if (entity->position.x >= DDWrecker->bossBoundR) {
        entity->position.x = DDWrecker->bossBoundR;
        entity->velocity.x = 0;
        if (entity->startPos.y <= DDWrecker->bossBoundT)
            entity->state = DDWrecker_State_Unknown10;
    }

    if (entity->rotation < 0) {
        if (entity->rotation + 2 > 0)
            entity->rotation = 0;
        else
            entity->rotation += 2;
    }
    else if (entity->rotation > 0) {
        entity->rotation -= 2;
        if (entity->rotation < 0)
            entity->rotation = 0;
    }

    if (entity->animator.animationID == 2) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 0, &entity->animator, true, 0);
    }
    else {
        if (entity->animator.animationID == 3 && !entity->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 2, &entity->animator, true, 0);
    }
}
void DDWrecker_State_Unknown10(void)
{
    RSDK_THIS(DDWrecker);
    entity->angle += 2;
    entity->angle &= 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->startPos.y) & 0xFFFF0000;

    if (entity->animator.animationID) {
        if (entity->animator.animationID == 1) {
            if (entity->animator.frameID == entity->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 3, &entity->animator, true, 0);
        }
        else {
            if (entity->animator.animationID == 3) {
                if (entity->animator.animationSpeed < 0x100) {
                    entity->animator.animationSpeed += 2;
                }
            }
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 1, &entity->animator, true, 0);
        RSDK.PlaySFX(DDWrecker->sfx_Sharp, 0, 255);
    }

    if (++entity->timer == 30) {
        entity->timer      = 0;
        entity->velocity.y = -0x20000;
        entity->state      = DDWrecker_State_Unknown11;
        RSDK.PlaySFX(DDWrecker->sfx_Drop, 0, 255);
    }
}
void DDWrecker_State_Unknown11(void)
{
    RSDK_THIS(DDWrecker);
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    entity->animator.animationSpeed = 0x100 - 32 * entity->timer;
    if (entity->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x180000, true)) {
            ++entity->timer;
            Camera_ShakeScreen(0, 0, 3);
            RSDK.PlaySFX(DDWrecker->sfx_Impact, 0, 255);
            entity->velocity.y = (entity->velocity.y >> 3) - entity->velocity.y;
            if (entity->timer - 1) {
                if (!(entity->timer - 3)) {
                    entity->velocity.y = 0;
                    entity->timer      = 0;
                    entity->startPos.y = entity->position.y;
                    entity->timer4     = 8;
                    entity->state      = DDWrecker_State_Unknown12;
                }
            }
            else {
                if (entity->position.x - (DDWrecker->bossBoundL + 0x100000) > 0)
                    entity->velocity.x = -DDWrecker->xVelocityUnknown[2];
                else
                    entity->velocity.x = DDWrecker->xVelocityUnknown[2];
            }
        }
    }
}
void DDWrecker_State_Unknown12(void)
{
    RSDK_THIS(DDWrecker);
    if (entity->animator.animationID == 2) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 0, &entity->animator, true, 0);
    }
    else {
        if (entity->animator.animationID == 3 && !entity->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 2, &entity->animator, true, 0);
    }

    if (++entity->timer == 30) {
        entity->timer = 0;
        entity->state = DDWrecker_State_Unknown9;
    }
}
void DDWrecker_LateState_Unknown1(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!entity->invincible && Player_CheckBadnikHit(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            DDWrecker_Hit();
            RSDK.PlaySFX(DDWrecker->sfx_BossHit, 0, 255);
        }
    }

    if (entity->timer4 > 0) {
        if (entity->timer4 > 8)
            entity->timer4--;

        entity->angle2 += entity->timer4;
        if (entity->angle2 > 1023) {
            entity->angle2 = 0;
            entity->timer4 = 0;
        }
    }

    if (entity->animator.animationID == 2) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 0, &entity->animator, true, 0);
    }
    else if (entity->animator.animationID == 3 && !entity->animator.frameID) {
        RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 2, &entity->animator, true, 0);
    }

    if (entity->blendAmount > 0) {
        entity->blendAmount -= 16;
    }
}
void DDWrecker_LateState_Unknown2(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!entity->invincible && Player_CheckBadnikHit(player, entity, &entity->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || entity->animator.animationID < 3) {
                if (Player_CheckBossHit(player, entity)) {
                    DDWrecker_Hit();
                    RSDK.PlaySFX(DDWrecker->sfx_BossHit, 0, 255);
                }
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }

    if (entity->animator.animationID) {
        if (entity->animator.animationID == 1) {
            if (entity->animator.frameID == entity->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 3, &entity->animator, true, 0);
        }
        else if (entity->animator.animationID == 3) {
            if (entity->animator.animationSpeed < 0x100)
                entity->animator.animationSpeed += 2;
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->spriteIndex, 1, &entity->animator, true, 0);
    }

    if (entity->timer4 < 48)
        entity->timer4 += 2;

    entity->angle2 = (entity->angle2 + entity->timer4) & 0x3FF;
    if (entity->blendAmount < 256) {
        entity->blendAmount += 16;
    }
}
void DDWrecker_LateState_Unknown3(void)
{
    RSDK_THIS(DDWrecker);
    foreach_active(Player, player)
    {
        if (!entity->invincible && Player_CheckBadnikHit(player, entity, &entity->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || entity->animator.animationID < 3) {
                if (Player_CheckBossHit(player, entity)) {
                    DDWrecker_Hit();
                    RSDK.PlaySFX(DDWrecker->sfx_BossHit, 0, 255);
                }
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }

    entity->angle2 = (entity->angle2 + entity->timer4) & 0x3FF;
    if (entity->animator.animationID == 0) {
        if (entity->blendAmount > 0) {
            entity->blendAmount -= 16;
        }
    }
    else if (entity->blendAmount < 256) {
        entity->blendAmount += 16;
    }
}
void DDWrecker_Hit(void)
{
    RSDK_THIS(DDWrecker);
    if (--entity->health <= 0) {
        entity->state     = DDWrecker_State_Die;
        entity->lateState = StateMachine_None;
        entity->timer     = 0;
        foreach_active(DDWrecker, child)
        {
            if (entity != child) {
                switch (child->type) {
                    case 1:
                    case 2:
                        if (child->lateState) {
                            child->state      = DDWrecker_State_Unknown12;
                            child->startPos.y = child->position.y;
                            child->velocity.x = -DDWrecker->xVelocityUnknown[2];
                            child->lateState  = DDWrecker_LateState_Unknown3;
                        }
                        break;
                    case 3:
                    case 4:
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
        entity->invincible = 48;
        RSDK.PlaySFX(DDWrecker->sfx_BossHit, 0, 255);
    }
}
void DDWrecker_Spin(void)
{
    RSDK_THIS(DDWrecker);
    for (int i = 0; i < 6; ++i) {
        EntityDDWrecker *child = RSDK_GET_ENTITY(entity->slots[i], DDWrecker);
        child->position.x      = DDWrecker->angleOffsets1[i] * RSDK.Sin1024(entity->angle3) * entity->arcOffset;
        child->position.y      = DDWrecker->angleOffsets1[i] * RSDK.Cos1024(entity->angle3) * entity->arcOffset;
        child->position.x += entity->position.x;
        child->position.y += entity->position.y;

        int rot = child->rotation;
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
    int angle        = RSDK.Sin1024(entity->angle3) >> 2;
    entity->rotation = RSDK.Sin1024(-entity->angle3) >> 6;
    if (entity->timer4 == 0)
        entity->angle2 = RSDK.Sin1024(-entity->angle3) >> 5;

    EntityDDWrecker *child = NULL;
    for (int i = 0; i < 6; ++i) {
        child             = RSDK_GET_ENTITY(entity->slots[i], DDWrecker);
        child->position.x = DDWrecker->angleOffsets2[i] * RSDK.Sin1024(angle) * entity->arcOffset;
        child->position.y = DDWrecker->angleOffsets2[i] * RSDK.Cos1024(angle) * entity->arcOffset;
        child->position.x += entity->position.x;
        child->position.y += entity->position.y;
    }
    child->rotation = RSDK.Sin1024(-entity->angle3) >> 5;
}
void DDWrecker_State_Debris(void)
{
    RSDK_THIS(DDWrecker);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}
void DDWrecker_State_Die(void)
{
    RSDK_THIS(DDWrecker);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySFX(DDWrecker->sfx_Explosion, 0, 255);
        if (Zone->timer & 4) {
            Entity *explosion = RSDK.CreateEntity(Explosion->objectID, intToVoid((RSDK.Rand(0, 256) > 192) + 2),
                                                  (RSDK.Rand(-20, 20) << 16) + entity->position.x, (RSDK.Rand(-20, 20) << 16) + entity->position.y);

            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 80) {
        int cnt = 0;
        foreach_active(DDWrecker, child) { ++cnt; }
        if (cnt != 1) {
            destroyEntity(entity);
        }
        else {
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            entity->timer               = 0;
            entity->visible             = false;
            entity->state               = DDWrecker_State_SpawnSignpost;
            RSDK_sceneInfo->timeEnabled = false;
            Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        }
    }
}
void DDWrecker_State_SpawnSignpost(void)
{
    RSDK_THIS(DDWrecker);
    if (++entity->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySFX(SignPost->sfx_Twinkle, 0, 255);
        }
        destroyEntity(entity);
    }
}

void DDWrecker_EditorDraw(void) {}

void DDWrecker_EditorLoad(void) {}

void DDWrecker_Serialize(void) {}

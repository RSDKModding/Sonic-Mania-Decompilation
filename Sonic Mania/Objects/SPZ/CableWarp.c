#include "SonicMania.h"

ObjectCableWarp *CableWarp;

void CableWarp_Update(void)
{
    RSDK_THIS(CableWarp);
    StateMachine_Run(entity->state);
}

void CableWarp_LateUpdate(void) {}

void CableWarp_StaticUpdate(void) {}

void CableWarp_Draw(void)
{
    RSDK_THIS(CableWarp);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void CableWarp_Create(void *data)
{
    RSDK_THIS(CableWarp);

    if (!RSDK_sceneInfo->inEditor) {
        if (data)
            entity->type = voidToInt(data);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->alpha         = 0x100;
        switch (entity->type) {
            case CABLEWARP_ENTRY:
                entity->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &entity->animator, true, 0);
                entity->state = CableWarp_State0_Unknown;
                break;
            case CABLEWARP_EXIT:
                entity->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &entity->animator, true, 2);
                entity->state = CableWarp_State1_Unknown;
                break;
            case CABLEWARP_NODE: entity->visible = false; break;
            case CABLEWARP_TRANSPORT:
                entity->visible = false;
                entity->active  = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 1, &entity->animator, true, 0);
                entity->state = CableWarp_State3_Unknown1;
                break;
            default: break;
        }
    }
}

void CableWarp_StageLoad(void)
{
    CableWarp->aniFrames      = RSDK.LoadSpriteAnimation("SPZ2/CableWarp.bin", SCOPE_STAGE);
    CableWarp->sfxCharge      = RSDK.GetSFX("Stage/ElecCharge.wav");
    CableWarp->sfxPulse       = RSDK.GetSFX("Stage/ElecPulse.wav");
    CableWarp->sfxLedgeBreak  = RSDK.GetSFX("Stage/LedgeBreak.wav");
    CableWarp->hitbox1.left   = -16;
    CableWarp->hitbox1.top    = -16;
    CableWarp->hitbox1.right  = 16;
    CableWarp->hitbox1.bottom = 32;
    CableWarp->hitbox2.left   = -16;
    CableWarp->hitbox2.top    = 0;
    CableWarp->hitbox2.right  = 16;
    CableWarp->hitbox2.bottom = 32;
}

void CableWarp_State0_Unknown(void)
{
    RSDK_THIS(CableWarp);

    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &CableWarp->hitbox1) == C_TOP) {
            if (player->playerAnimator.animationID == ANI_JUMP || player->state == Player_State_DropDash
#if RETRO_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
            ) {
                if (player->state == Player_State_DropDash) {
                    player->velocity.y = -0x10000;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->state = Player_State_Air;
                }
#if RETRO_USE_PLUS
                else if (player->state == Player_State_MightyHammerDrop) {
                    player->velocity.y = velY;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->state = Player_State_Air;
                }
#endif
                else {
                    player->velocity.y = -0x10000;
                }
                player->onGround = false;

                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x - 0x80000, entity->position.y - 0x80000);
                debris->velocity.x   = -0x20000;
                debris->velocity.y   = -0x20000;
                debris->gravity      = 0x3800;
                debris->drawOrder    = Zone->playerDrawHigh;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 3);

                debris             = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x + 0x80000, entity->position.y - 0x80000);
                debris->velocity.x = 0x20000;
                debris->velocity.y = -0x20000;
                debris->gravity    = 0x3800;
                debris->drawOrder  = Zone->playerDrawHigh;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 4);
                RSDK.PlaySfx(CableWarp->sfxLedgeBreak, false, 255);
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &entity->animator, true, 1);
                entity->state = CableWarp_State1_Unknown;

                foreach_break;
            }
        }
    }
}

void CableWarp_State1_Unknown(void)
{
    RSDK_THIS(CableWarp);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &CableWarp->hitbox2) == C_TOP && !entity->type) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->tileCollisions  = false;
            player->interaction     = false;
            player->onGround        = false;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);

            EntityCableWarp *warp = CREATE_ENTITY(CableWarp, intToVoid(CABLEWARP_TRANSPORT), entity->position.x, entity->position.y);
            warp->isPermanent     = true;
            warp->playerPtr       = (Entity *)player;
            warp->slotID          = RSDK_sceneInfo->entitySlot;
            if (entity->type) {
                warp->nextSlot            = warp->slotID - 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(warp->nextSlot, CableWarp);
                if (nextNode->objectID != CableWarp->objectID || nextNode->type != CABLEWARP_NODE)
                    warp->nextSlot += 2;
            }
            else {
                warp->nextSlot            = warp->slotID + 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(warp->nextSlot, CableWarp);
                if (nextNode->objectID == CableWarp->objectID) {
                    if (nextNode->type != CABLEWARP_NODE)
                        warp->nextSlot -= 2;
                }
                else {
                    warp->nextSlot -= 2;
                }
            }
        }
    }
}

void CableWarp_State3_Unknown1(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        int y = entity->position.y - 0x200000;
        int x = entity->position.x;
        if (player->playerAnimator.animationSpeed < 0xF0)
            player->playerAnimator.animationSpeed += 8;
        player->velocity.x += ((entity->position.x - player->position.x) >> 5) - (player->velocity.x >> 4);
        player->velocity.y += ((y - player->position.y) >> 5) - (player->velocity.y >> 4);
        if (++entity->timer == 30) {
            player->position.x = x;
            player->position.y = y;
            player->velocity.x = 0;
            player->velocity.y = 0;
            entity->visible    = true;
            entity->state      = CableWarp_State3_Unknown2;
            RSDK.PlaySfx(CableWarp->sfxCharge, false, 255);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void CableWarp_State3_Unknown2(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&entity->animator);

        if (entity->animator.frameID == 20) {
            player->visible    = 0;
            player->velocity.y = 0x60000;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 255);
        }
        else {
            if (entity->animator.frameID == entity->animator.frameCount - 1) {
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 2, &entity->animator, true, 0);
                entity->inkEffect         = INK_ADD;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(entity->nextSlot, CableWarp);
                int angle          = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
                entity->angle      = angle;
                entity->rotation   = 2 * angle;
                player->velocity.x = RSDK.Cos256(entity->angle) << 12;
                player->velocity.y = RSDK.Sin256(entity->angle) << 12;
                entity->drawFX     = FX_ROTATE | FX_FLIP;
                entity->state      = CableWarp_State3_Unknown3;
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void CableWarp_State3_Unknown3(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&entity->animator);
        entity->position.x = player->position.x;
        entity->position.y = player->position.y;
        RSDK.GetEntityByID(entity->slotID);
        EntityCableWarp *nextNode = RSDK_GET_ENTITY(entity->nextSlot, CableWarp);
        int rx                    = (nextNode->position.x - player->position.x) >> 16;
        int ry                    = (nextNode->position.y - player->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            player->position.x = nextNode->position.x;
            player->position.y = nextNode->position.y;

            if (entity->slotID >= entity->nextSlot) {
                entity->nextSlot--;
                entity->slotID--;
            }
            else {
                entity->nextSlot++;
                entity->slotID++;
            }

            nextNode           = RSDK_GET_ENTITY(entity->nextSlot, CableWarp);
            int angle          = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
            entity->angle      = angle;
            entity->rotation   = 2 * angle + 128;
            player->velocity.x = RSDK.Cos256(entity->angle) << 12;
            player->velocity.y = (RSDK.Sin256(entity->angle) << 12);
            if (nextNode->type != CABLEWARP_NODE)
                entity->state = CableWarp_State3_Unknown4;
        }
    }
    else {
        destroyEntity(entity);
    }
}

void CableWarp_State3_Unknown4(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&entity->animator);
        entity->position.x = player->position.x;
        entity->position.y = player->position.y;
        RSDK.GetEntityByID(entity->slotID);
        EntityCableWarp *nextNode = RSDK_GET_ENTITY(entity->nextSlot, CableWarp);
        int rx                    = (nextNode->position.x - player->position.x) >> 16;
        int ry                    = (nextNode->position.y - player->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            player->position.x = nextNode->position.x;
            player->position.y = nextNode->position.y;
            player->velocity.x = 0;
            player->velocity.y = -0x80000;
            if (nextNode->state == CableWarp_State0_Unknown) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, nextNode->position.x - 0x80000, nextNode->position.y - 0x80000);
                debris->velocity.x   = -0x20000;
                debris->velocity.y   = -0x20000;
                debris->gravity      = 0x3800;
                debris->drawOrder    = Zone->playerDrawHigh;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 3);

                debris             = CREATE_ENTITY(Debris, Debris_State_Fall, nextNode->position.x + 0x80000, nextNode->position.y - 0x80000);
                debris->velocity.x = 0x20000;
                debris->velocity.y = -0x20000;
                debris->gravity    = 0x3800;
                debris->drawOrder  = Zone->playerDrawHigh;

                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 4);
                RSDK.PlaySfx(CableWarp->sfxLedgeBreak, false, 255);
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &nextNode->animator, true, 1);
                nextNode->state = CableWarp_State1_Unknown;
            }
            RSDK.SetSpriteAnimation(CableWarp->aniFrames, 3, &entity->animator, true, 0);
            entity->timer     = 0;
            entity->state     = CableWarp_State3_Unknown5;
            entity->inkEffect = INK_NONE;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 255);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void CableWarp_State3_Unknown5(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    RSDK.ProcessAnimation(&entity->animator);
    ++entity->timer;
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        player->state          = Player_State_Air;
        player->visible        = true;
        player->tileCollisions = true;
        player->interaction    = true;
        player->jumpAbility    = 0;
        player->velocity.y     = -0xA0000;
        player->position.y     = entity->position.y - 0x180000;
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void CableWarp_EditorDraw(void)
{
    RSDK_THIS(CableWarp);

    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    switch (entity->type) {
        case CABLEWARP_ENTRY: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &entity->animator, true, 0); break;
        case CABLEWARP_EXIT: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &entity->animator, true, 2); break;
        case CABLEWARP_NODE: RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator, true, 2); break;
        default: break;
    }

    CableWarp_Draw();
}

void CableWarp_EditorLoad(void)
{
    CableWarp->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CableWarp.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CableWarp, type);
    RSDK_ENUM_VAR("Entry", CABLEWARP_ENTRY);
    RSDK_ENUM_VAR("Exit", CABLEWARP_EXIT);
    RSDK_ENUM_VAR("Warp Node", CABLEWARP_NODE);
}
#endif

void CableWarp_Serialize(void) { RSDK_EDITABLE_VAR(CableWarp, VAR_ENUM, type); }

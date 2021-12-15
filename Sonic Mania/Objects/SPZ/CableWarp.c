// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CableWarp Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCableWarp *CableWarp;

void CableWarp_Update(void)
{
    RSDK_THIS(CableWarp);
    StateMachine_Run(self->state);
}

void CableWarp_LateUpdate(void) {}

void CableWarp_StaticUpdate(void) {}

void CableWarp_Draw(void)
{
    RSDK_THIS(CableWarp);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void CableWarp_Create(void *data)
{
    RSDK_THIS(CableWarp);

    if (!SceneInfo->inEditor) {
        if (data)
            self->type = voidToInt(data);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->drawOrder     = Zone->drawOrderHigh;
        self->alpha         = 0x100;
        switch (self->type) {
            case CABLEWARP_ENTRY:
                self->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 0);
                self->state = CableWarp_State0_Unknown;
                break;
            case CABLEWARP_EXIT:
                self->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 2);
                self->state = CableWarp_State1_Unknown;
                break;
            case CABLEWARP_NODE: self->visible = false; break;
            case CABLEWARP_TRANSPORT:
                self->visible = false;
                self->active  = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 1, &self->animator, true, 0);
                self->state = CableWarp_State3_Unknown1;
                break;
            default: break;
        }
    }
}

void CableWarp_StageLoad(void)
{
    CableWarp->aniFrames      = RSDK.LoadSpriteAnimation("SPZ2/CableWarp.bin", SCOPE_STAGE);
    CableWarp->sfxCharge      = RSDK.GetSfx("Stage/ElecCharge.wav");
    CableWarp->sfxPulse       = RSDK.GetSfx("Stage/ElecPulse.wav");
    CableWarp->sfxLedgeBreak  = RSDK.GetSfx("Stage/LedgeBreak.wav");
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
        if (Player_CheckCollisionBox(player, self, &CableWarp->hitbox1) == C_TOP) {
            if (player->animator.animationID == ANI_JUMP || player->state == Player_State_DropDash
#if RETRO_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
            ) {
                if (player->state == Player_State_DropDash) {
                    player->velocity.y = -0x10000;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
#if RETRO_USE_PLUS
                else if (player->state == Player_State_MightyHammerDrop) {
                    player->velocity.y = velY;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
#endif
                else {
                    player->velocity.y = -0x10000;
                }
                player->onGround = false;

                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x - 0x80000, self->position.y - 0x80000);
                debris->velocity.x   = -0x20000;
                debris->velocity.y   = -0x20000;
                debris->gravity      = 0x3800;
                debris->drawOrder    = Zone->playerDrawHigh;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 3);

                debris             = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + 0x80000, self->position.y - 0x80000);
                debris->velocity.x = 0x20000;
                debris->velocity.y = -0x20000;
                debris->gravity    = 0x3800;
                debris->drawOrder  = Zone->playerDrawHigh;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 4);
                RSDK.PlaySfx(CableWarp->sfxLedgeBreak, false, 255);
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 1);
                self->state = CableWarp_State1_Unknown;

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
        if (Player_CheckCollisionBox(player, self, &CableWarp->hitbox2) == C_TOP && !self->type) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->tileCollisions  = false;
            player->interaction     = false;
            player->onGround        = false;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

            EntityCableWarp *warp = CREATE_ENTITY(CableWarp, intToVoid(CABLEWARP_TRANSPORT), self->position.x, self->position.y);
            warp->isPermanent     = true;
            warp->playerPtr       = (Entity *)player;
            warp->slotID          = SceneInfo->entitySlot;
            if (self->type) {
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
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        int y = self->position.y - 0x200000;
        int x = self->position.x;
        if (player->animator.speed < 0xF0)
            player->animator.speed += 8;
        player->velocity.x += ((self->position.x - player->position.x) >> 5) - (player->velocity.x >> 4);
        player->velocity.y += ((y - player->position.y) >> 5) - (player->velocity.y >> 4);
        if (++self->timer == 30) {
            player->position.x = x;
            player->position.y = y;
            player->velocity.x = 0;
            player->velocity.y = 0;
            self->visible    = true;
            self->state      = CableWarp_State3_Unknown2;
            RSDK.PlaySfx(CableWarp->sfxCharge, false, 255);
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_State3_Unknown2(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&self->animator);

        if (self->animator.frameID == 20) {
            player->visible    = 0;
            player->velocity.y = 0x60000;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 255);
        }
        else {
            if (self->animator.frameID == self->animator.frameCount - 1) {
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 2, &self->animator, true, 0);
                self->inkEffect         = INK_ADD;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
                int angle          = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
                self->angle      = angle;
                self->rotation   = 2 * angle;
                player->velocity.x = RSDK.Cos256(self->angle) << 12;
                player->velocity.y = RSDK.Sin256(self->angle) << 12;
                self->drawFX     = FX_ROTATE | FX_FLIP;
                self->state      = CableWarp_State3_Unknown3;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_State3_Unknown3(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&self->animator);
        self->position.x = player->position.x;
        self->position.y = player->position.y;
        RSDK.GetEntityByID(self->slotID);
        EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
        int rx                    = (nextNode->position.x - player->position.x) >> 16;
        int ry                    = (nextNode->position.y - player->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            player->position.x = nextNode->position.x;
            player->position.y = nextNode->position.y;

            if (self->slotID >= self->nextSlot) {
                self->nextSlot--;
                self->slotID--;
            }
            else {
                self->nextSlot++;
                self->slotID++;
            }

            nextNode           = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
            int angle          = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
            self->angle      = angle;
            self->rotation   = 2 * angle + 128;
            player->velocity.x = RSDK.Cos256(self->angle) << 12;
            player->velocity.y = (RSDK.Sin256(self->angle) << 12);
            if (nextNode->type != CABLEWARP_NODE)
                self->state = CableWarp_State3_Unknown4;
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_State3_Unknown4(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.ProcessAnimation(&self->animator);
        self->position.x = player->position.x;
        self->position.y = player->position.y;
        RSDK.GetEntityByID(self->slotID);
        EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
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
            RSDK.SetSpriteAnimation(CableWarp->aniFrames, 3, &self->animator, true, 0);
            self->timer     = 0;
            self->state     = CableWarp_State3_Unknown5;
            self->inkEffect = INK_NONE;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 255);
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_State3_Unknown5(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    RSDK.ProcessAnimation(&self->animator);
    ++self->timer;
    if (self->animator.frameID == self->animator.frameCount - 1) {
        player->state          = Player_State_Air;
        player->visible        = true;
        player->tileCollisions = true;
        player->interaction    = true;
        player->jumpAbility    = 0;
        player->velocity.y     = -0xA0000;
        player->position.y     = self->position.y - 0x180000;
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void CableWarp_EditorDraw(void)
{
    RSDK_THIS(CableWarp);

    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    switch (self->type) {
        case CABLEWARP_ENTRY: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 0); break;
        case CABLEWARP_EXIT: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 2); break;
        case CABLEWARP_NODE: RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 2); break;
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

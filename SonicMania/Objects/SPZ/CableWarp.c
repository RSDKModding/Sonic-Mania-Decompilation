// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CableWarp Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
            self->type = VOID_TO_INT(data);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->alpha         = 0x100;

        switch (self->type) {
            case CABLEWARP_ENTRY:
                self->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 0);
                self->state = CableWarp_State_CablePlug;
                break;

            case CABLEWARP_EXIT:
                self->visible = true;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 2);
                self->state = CableWarp_State_CheckPlayerEntry;
                break;

            case CABLEWARP_NODE: self->visible = false; break;

            case CABLEWARP_TRANSPORT:
                self->visible = false;
                self->active  = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 1, &self->animator, true, 0);
                self->state = CableWarp_StateTransport_BeginEnter;
                break;

            default: break;
        }
    }
}

void CableWarp_StageLoad(void)
{
    CableWarp->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/CableWarp.bin", SCOPE_STAGE);

    CableWarp->sfxCharge     = RSDK.GetSfx("Stage/ElecCharge.wav");
    CableWarp->sfxPulse      = RSDK.GetSfx("Stage/ElecPulse.wav");
    CableWarp->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");

    CableWarp->hitboxPlug.left   = -16;
    CableWarp->hitboxPlug.top    = -16;
    CableWarp->hitboxPlug.right  = 16;
    CableWarp->hitboxPlug.bottom = 32;

    CableWarp->hitboxTransport.left   = -16;
    CableWarp->hitboxTransport.top    = 0;
    CableWarp->hitboxTransport.right  = 16;
    CableWarp->hitboxTransport.bottom = 32;
}

void CableWarp_State_CablePlug(void)
{
    RSDK_THIS(CableWarp);

    foreach_active(Player, player)
    {
#if MANIA_USE_PLUS
        int32 velY = player->velocity.y;
#endif

        if (Player_CheckCollisionBox(player, self, &CableWarp->hitboxPlug) == C_TOP) {
#if MANIA_USE_PLUS
            if (player->animator.animationID == ANI_JUMP || player->state == Player_State_DropDash
                || player->state == Player_State_MightyHammerDrop) {
                if (player->state == Player_State_DropDash) {
                    player->velocity.y = -0x10000;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
                else if (player->state == Player_State_MightyHammerDrop) {
                    player->velocity.y = velY;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
                else {
                    player->velocity.y = -0x10000;
                }
#else
            if (player->animator.animationID == ANI_JUMP) {
                player->velocity.y = -0x10000;
#endif
                player->onGround = false;

                EntityDebris *debris    = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x - 0x80000, self->position.y - 0x80000);
                debris->velocity.x      = -0x20000;
                debris->velocity.y      = -0x20000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->playerDrawGroup[1];
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 3);

                debris                  = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + 0x80000, self->position.y - 0x80000);
                debris->velocity.x      = 0x20000;
                debris->velocity.y      = -0x20000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->playerDrawGroup[1];
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 4);
                RSDK.PlaySfx(CableWarp->sfxLedgeBreak, false, 0xFF);
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 1);
                self->state = CableWarp_State_CheckPlayerEntry;

                foreach_break;
            }
        }
    }
}

void CableWarp_State_CheckPlayerEntry(void)
{
    RSDK_THIS(CableWarp);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &CableWarp->hitboxTransport) == C_TOP && !self->type) {
            player->state           = Player_State_Static;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->tileCollisions  = TILECOLLISION_NONE;
            player->interaction     = false;
            player->onGround        = false;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

            EntityCableWarp *warp = CREATE_ENTITY(CableWarp, INT_TO_VOID(CABLEWARP_TRANSPORT), self->position.x, self->position.y);
            warp->isPermanent     = true;
            warp->player          = player;

            warp->slotID = SceneInfo->entitySlot;
            if (self->type) {
                warp->nextSlot            = warp->slotID - 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(warp->nextSlot, CableWarp);
                if (nextNode->classID != CableWarp->classID || nextNode->type != CABLEWARP_NODE)
                    warp->nextSlot += 2;
            }
            else {
                warp->nextSlot            = warp->slotID + 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(warp->nextSlot, CableWarp);
                if (nextNode->classID == CableWarp->classID) {
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

void CableWarp_StateTransport_BeginEnter(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->player;

    if (player->state == Player_State_Static) {
        int32 x = self->position.x;
        int32 y = self->position.y - 0x200000;

        if (player->animator.speed < 0xF0)
            player->animator.speed += 8;

        player->velocity.x += ((x - player->position.x) >> 5) - (player->velocity.x >> 4);
        player->velocity.y += ((y - player->position.y) >> 5) - (player->velocity.y >> 4);

        if (++self->timer == 30) {
            player->position.x = x;
            player->position.y = y;
            player->velocity.x = 0;
            player->velocity.y = 0;
            self->visible      = true;
            self->state        = CableWarp_StateTransport_Enter;
            RSDK.PlaySfx(CableWarp->sfxCharge, false, 0xFF);
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_StateTransport_Enter(void)
{
    RSDK_THIS(CableWarp);

    EntityPlayer *player = self->player;
    if (player->state == Player_State_Static) {
        RSDK.ProcessAnimation(&self->animator);

        if (self->animator.frameID == 20) {
            player->visible    = false;
            player->velocity.y = 0x60000;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 0xFF);
        }
        else if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(CableWarp->aniFrames, 2, &self->animator, true, 0);
            self->inkEffect           = INK_ADD;
            EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
            int32 angle        = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
            self->angle        = angle;
            self->rotation     = 2 * angle;
            player->velocity.x = RSDK.Cos256(self->angle) << 12;
            player->velocity.y = RSDK.Sin256(self->angle) << 12;
            self->drawFX       = FX_ROTATE | FX_FLIP;
            self->state        = CableWarp_StateTransport_MoveToNextNode;
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_StateTransport_MoveToNextNode(void)
{
    RSDK_THIS(CableWarp);

    EntityPlayer *player = self->player;
    if (player->state == Player_State_Static) {
        RSDK.ProcessAnimation(&self->animator);
        self->position.x = player->position.x;
        self->position.y = player->position.y;
        RSDK_GET_ENTITY(self->slotID, CableWarp);

        EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);
        int32 rx                  = (nextNode->position.x - player->position.x) >> 16;
        int32 ry                  = (nextNode->position.y - player->position.y) >> 16;
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
            int32 angle        = RSDK.ATan2((nextNode->position.x - player->position.x) >> 16, (nextNode->position.y - player->position.y) >> 16);
            self->angle        = angle;
            self->rotation     = 2 * angle + 128;
            player->velocity.x = RSDK.Cos256(self->angle) << 12;
            player->velocity.y = (RSDK.Sin256(self->angle) << 12);
            if (nextNode->type != CABLEWARP_NODE)
                self->state = CableWarp_StateTransport_EndNode;
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_StateTransport_EndNode(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->player;

    if (player->state == Player_State_Static) {
        RSDK.ProcessAnimation(&self->animator);
        self->position.x = player->position.x;
        self->position.y = player->position.y;

        RSDK_GET_ENTITY(self->slotID, CableWarp);
        EntityCableWarp *nextNode = RSDK_GET_ENTITY(self->nextSlot, CableWarp);

        int32 rx = (nextNode->position.x - player->position.x) >> 16;
        int32 ry = (nextNode->position.y - player->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            player->position.x = nextNode->position.x;
            player->position.y = nextNode->position.y;
            player->velocity.x = 0;
            player->velocity.y = -0x80000;
            if (nextNode->state == CableWarp_State_CablePlug) {
                EntityDebris *debris    = CREATE_ENTITY(Debris, Debris_State_Fall, nextNode->position.x - 0x80000, nextNode->position.y - 0x80000);
                debris->velocity.x      = -0x20000;
                debris->velocity.y      = -0x20000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->playerDrawGroup[1];
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 3);

                debris                  = CREATE_ENTITY(Debris, Debris_State_Fall, nextNode->position.x + 0x80000, nextNode->position.y - 0x80000);
                debris->velocity.x      = 0x20000;
                debris->velocity.y      = -0x20000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->playerDrawGroup[1];

                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &debris->animator, true, 4);
                RSDK.PlaySfx(CableWarp->sfxLedgeBreak, false, 255);
                RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &nextNode->animator, true, 1);
                nextNode->state = CableWarp_State_CheckPlayerEntry;
            }
            RSDK.SetSpriteAnimation(CableWarp->aniFrames, 3, &self->animator, true, 0);
            self->timer     = 0;
            self->state     = CableWarp_StateTransport_Exit;
            self->inkEffect = INK_NONE;
            RSDK.PlaySfx(CableWarp->sfxPulse, false, 255);
        }
    }
    else {
        destroyEntity(self);
    }
}

void CableWarp_StateTransport_Exit(void)
{
    RSDK_THIS(CableWarp);
    EntityPlayer *player = (EntityPlayer *)self->player;

    RSDK.ProcessAnimation(&self->animator);
    ++self->timer;
    if (self->animator.frameID == self->animator.frameCount - 1) {
        player->state          = Player_State_Air;
        player->visible        = true;
        player->tileCollisions = TILECOLLISION_DOWN;
        player->interaction    = true;
        player->applyJumpCap   = false;
        player->velocity.y     = -0xA0000;
        player->position.y     = self->position.y - 0x180000;
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void CableWarp_EditorDraw(void)
{
    RSDK_THIS(CableWarp);

    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    switch (self->type) {
        case CABLEWARP_ENTRY: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 0); break;
        case CABLEWARP_EXIT: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 0, &self->animator, true, 2); break;
        case CABLEWARP_NODE: RSDK.SetSpriteAnimation(CableWarp->aniFrames, 4, &self->animator, true, 2); break;
        default: break;
    }

    CableWarp_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->inkEffect = INK_BLEND;

        int32 slotID   = SceneInfo->entitySlot;
        int32 nextSlot = -1;
        switch (self->type) {
            case CABLEWARP_ENTRY: {
                nextSlot                  = slotID + 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(nextSlot, CableWarp);
                if (nextNode && nextNode->classID == CableWarp->classID) {
                    if (nextNode->type != CABLEWARP_NODE)
                        nextSlot -= 2;
                }
                else {
                    nextSlot -= 2;
                }
                break;
            }

            case CABLEWARP_EXIT: {
                nextSlot                  = slotID - 1;
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(nextSlot, CableWarp);
                if (nextNode && (nextNode->classID != CableWarp->classID || nextNode->type != CABLEWARP_NODE))
                    nextSlot += 2;
                break;
            }

            default: break;
        }

        if (nextSlot != -1) {
            EntityCableWarp *lastNode = RSDK_GET_ENTITY(slotID, CableWarp);
            while (lastNode) {
                EntityCableWarp *nextNode = RSDK_GET_ENTITY(nextSlot, CableWarp);
                if (!nextNode)
                    break;

                DrawHelpers_DrawArrow(lastNode->position.x, lastNode->position.y, nextNode->position.x, nextNode->position.y, 0xFFFF00, INK_NONE,
                                      0xFF);

                if (nextNode->type != CABLEWARP_NODE)
                    break;

                if (slotID >= nextSlot) {
                    nextSlot--;
                    slotID--;
                }
                else {
                    nextSlot++;
                    slotID++;
                }

                lastNode = nextNode;
            }
        }

        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
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

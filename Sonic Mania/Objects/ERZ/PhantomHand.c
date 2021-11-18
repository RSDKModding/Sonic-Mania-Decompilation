#include "SonicMania.h"

ObjectPhantomHand *PhantomHand;

void PhantomHand_Update(void)
{
    RSDK_THIS(PhantomHand);
    StateMachine_Run(self->state);
}

void PhantomHand_LateUpdate(void) {}

void PhantomHand_StaticUpdate(void) {}

void PhantomHand_Draw(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.CopyPalette(1, 128, 0, 128, 128);
    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (self->state != PhantomHand_State_Unknown4) {
        self->inkEffect = INK_ALPHA;
        self->alpha     = 0xD0;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->inkEffect = INK_ADD;
        self->alpha     = 0x100;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void PhantomHand_Create(void *data)
{
    RSDK_THIS(PhantomHand);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->parent        = (Entity *)data;
        self->field_6C      = self->position.x;
        self->field_70      = self->position.y;
        self->active        = ACTIVE_NORMAL;
        self->inkEffect     = INK_ADD;
        self->drawFX        = FX_FLIP | FX_SCALE;
        self->alpha         = 0x100;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->field_64      = 64;
        self->state         = PhantomHand_State_Summon;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 0, &self->animator1, true, 0);
    }
}

void PhantomHand_StageLoad(void)
{
    PhantomHand->aniFrames     = RSDK.LoadSpriteAnimation("Phantom/PhantomHand.bin", SCOPE_STAGE);
    PhantomHand->hitbox.left   = -20;
    PhantomHand->hitbox.top    = -20;
    PhantomHand->hitbox.right  = 20;
    PhantomHand->hitbox.bottom = 20;

    int pos = 3;
    for (int i = 0; i < PhantomHand->debrisInfo[0]; ++i) {
        PhantomHand->debrisInfo[pos + 0] = RSDK.Rand(-0x20000, 0x20000);
        PhantomHand->debrisInfo[pos + 1] = RSDK.Rand(-0x10000, -0x40000);

        pos += 4;
    }

    PhantomHand->grabbedPlayers[0] = false;
    PhantomHand->grabbedPlayers[1] = false;
    PhantomHand->grabbedPlayers[2] = false;
    PhantomHand->grabbedPlayers[3] = false;
}

void PhantomHand_CheckPlayerGrab(int playerX, int playerY)
{
    RSDK_THIS(PhantomHand);

    if (self->position.x >= playerX) {
        if (self->position.x > playerX) {
            int dist = (self->position.x - playerX) >> 5;
            if (dist > 0x10000)
                dist = 0x10000;
            self->position.x -= dist;
        }
    }
    else {
        int dist = (playerX - self->position.x) >> 5;
        if (dist > 0x10000)
            dist = 0x10000;
        self->position.x += dist;
    }

    if (self->field_70 < playerY)
        self->field_70 += 0x10000;

    if (playerY <= self->field_70)
        self->position.y += (playerY - self->position.y) >> 3;
    else
        self->position.y = self->field_70;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int rx = (player->position.x - self->position.x) >> 16;
            int ry = (player->position.y - self->position.y) >> 16;
            if (rx * rx + ry * ry < 0x100) {
                if (!PhantomHand->grabbedPlayers[player->playerID]) {
                    PhantomHand->grabbedPlayers[player->playerID] = true;
                    self->state                                 = PhantomHand_State_Unknown5;
                }
            }
        }
    }
}

void PhantomHand_State_Summon(void)
{
    RSDK_THIS(PhantomHand);

    if (!self->timer)
        RSDK.PlaySfx(PhantomEgg->sfxSummon, false, 255);
    self->position.x += self->velocity.x;
    if (self->timer > 0)
        self->field_64 -= 4;
    self->timer += self->field_64;

    if (self->timer < 0x200 && self->field_64 < 0) {
        self->timer = 0x200;
        self->state = PhantomHand_State_Unknown1;
    }
    self->field_68 += 8;
    self->scale.x = self->timer + ((self->timer * RSDK.Sin256(self->field_68)) >> 11);
    self->scale.y = self->timer + ((self->timer * RSDK.Cos256(self->field_68)) >> 11);
}

void PhantomHand_State_Unknown1(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator1);
    self->field_68 += 16;
    self->position.x += self->velocity.x;
    self->scale.x = ((self->timer * RSDK.Sin256(self->field_68)) >> 11) + 0x200;
    self->scale.y = ((self->timer * RSDK.Cos256(self->field_68)) >> 11) + 0x200;
    if (self->timer <= 0) {
        self->drawFX = FX_FLIP;
        if (Player->playerCount < 2)
            self->state = PhantomHand_State_Unknown2;
        else
            self->state = PhantomHand_State_Unknown3;
    }
    else {
        self->timer -= 32;
    }
}

void PhantomHand_State_Unknown2(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator1);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->interaction == true && player1->state == Player_State_None) {
        self->state = PhantomHand_State_Unknown4;
    }
    else {
        if ((self->direction && player1->position.x <= PhantomEgg->boundsM) || (self->direction && player1->position.x >= PhantomEgg->boundsM)) {
            PhantomHand_CheckPlayerGrab(self->parent->position.x, player1->position.y);
        }
        else {
            PhantomHand_CheckPlayerGrab(player1->position.x, player1->position.y);
        }
    }
}

void PhantomHand_State_Unknown3(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator1);

    bool32 flag = true;
    for (int i = SLOT_PLAYER1; i < SLOT_PLAYER3; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        if (player->objectID == Player->objectID)
            flag = flag && player->interaction && player->state == Player_State_None;
    }

    if (flag) {
        self->state = PhantomHand_State_Unknown4;
    }
    else {
        int dist = 0x7FFFFFFF;

        EntityPlayer *playerPtr = NULL;
        if (self->direction == FLIP_NONE) {
            foreach_active(Player, player)
            {
                if (player->state != Player_State_None && self->position.x - player->position.x < dist) {
                    dist      = self->position.x - self->position.x;
                    playerPtr = player;
                }
            }
            if (playerPtr)
                PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
        }
        else {
            foreach_active(Player, player)
            {
                if (player->state != Player_State_None && player->position.x - self->position.x < dist) {
                    dist      = player->position.x - self->position.x;
                    playerPtr = player;
                }
            }
        }
        if (playerPtr)
            PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
    }
}

void PhantomHand_State_Unknown5(void)
{
    RSDK_THIS(PhantomHand);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomHand->hitbox)) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->position.x      = self->position.x;
            player->position.y      = self->position.y;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            player->onGround        = false;
            player->groundVel       = 0;
            if (!player->playerID)
                self->field_78 = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
        }
    }

    if (++self->timer2 == 8) {
        self->timer2 = 0;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 1, &self->animator1, true, 0);
        self->state = PhantomHand_State_Unknown6;
    }
}

void PhantomHand_State_Unknown6(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == 5) {
        self->drawOrder = Zone->drawOrderHigh;
    }
    if (self->animator1.frameID == 10) {
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 2, &self->animator2, true, 0);
        self->state = PhantomHand_State_Unknown7;
    }
}

void PhantomHand_State_Unknown7(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (self->animator2.frameID == self->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 3, &self->animator3, true, 0);
        self->state = PhantomHand_State_Unknown8;
    }
}

void PhantomHand_State_Unknown8(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->animator3);
    if (self->animator3.frameID == self->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator3, true, 0);
        if (self->field_78)
            PhantomEgg_SetupScanlineCB();
        self->state = StateMachine_None;
    }
}

void PhantomHand_State_Unknown9(void)
{
    RSDK_THIS(PhantomHand);
    Debris_FallFlickerAnimSetup(PhantomHand->aniFrames, PhantomHand->debrisInfo, 4);
    RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomHand->hitbox)) {
            player->state                                 = Player_State_Air;
            player->velocity.y                            = -0x40000;
            player->onGround                              = false;
            player->groundVel                             = 0;
            PhantomHand->grabbedPlayers[player->playerID] = false;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
        }
    }
    destroyEntity(self);
}

void PhantomHand_State_Unknown4(void)
{
    RSDK_THIS(PhantomHand);

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 8;
}

#if RETRO_INCLUDE_EDITOR
void PhantomHand_EditorDraw(void) {}

void PhantomHand_EditorLoad(void) {}
#endif

void PhantomHand_Serialize(void) {}

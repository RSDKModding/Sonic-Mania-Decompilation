// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LoveTester Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLoveTester *LoveTester;

void LoveTester_Update(void)
{
    RSDK_THIS(LoveTester);

    StateMachine_Run(self->state);
    StateMachine_Run(self->stateLate);

    for (int i = 0; i < 10; ++i) {
        RSDK.ProcessAnimation(&self->animators[i]);
    }
}

void LoveTester_LateUpdate(void) {}

void LoveTester_StaticUpdate(void)
{
    foreach_active(LoveTester, loveTester) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(loveTester)); }
}

void LoveTester_Draw(void)
{
    RSDK_THIS(LoveTester);

    if (self->state == LoveTester_State2_Unknown)
        RSDK.DrawSprite(&self->animator1, NULL, false);
    else
        LoveTester_DrawSprites();
}

void LoveTester_Create(void *data)
{
    RSDK_THIS(LoveTester);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->storedPos.x   = self->position.x;
    self->storedPos.y   = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    LoveTester_SetupHitboxes();
    if (data) {
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->active        = ACTIVE_NORMAL;
        self->drawOrder     = Zone->drawOrderHigh;
        self->state         = LoveTester_State2_Unknown;
    }
    else {
        self->state = LoveTester_State_Unknown1;
    }
}

void LoveTester_StageLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);
    LoveTester_SetupHitboxes();
    LoveTester_SetupPositions();
    LoveTester->field_20.x = 0;
    LoveTester->field_20.y = -0x300000;
    LoveTester->field_28.x = 0;
    LoveTester->field_28.y = 0x320000;
    LoveTester->active     = ACTIVE_ALWAYS;
    LoveTester->sfxScore   = RSDK.GetSfx("SPZ/Score.wav");
}

void LoveTester_SetupHitboxes(void)
{
    LoveTester->hitbox1.left   = -16;
    LoveTester->hitbox1.top    = -16;
    LoveTester->hitbox1.right  = 16;
    LoveTester->hitbox1.bottom = 16;
    LoveTester->hitbox2.left   = -24;
    LoveTester->hitbox2.top    = -20;
    LoveTester->hitbox2.right  = -18;
    LoveTester->hitbox2.bottom = 20;
    LoveTester->hitbox3.left   = 18;
    LoveTester->hitbox3.top    = -20;
    LoveTester->hitbox3.right  = 24;
    LoveTester->hitbox3.bottom = 20;
}

void LoveTester_SetupPositions(void)
{
    LoveTester->positions[0].x = -0x1C0000;
    LoveTester->positions[0].y = -0x1F0000;
    LoveTester->positions[1].x = 0x1C0000;
    LoveTester->positions[1].y = -0x1F0000;
    LoveTester->positions[2].x = -0x1C0000;
    LoveTester->positions[2].y = -0xF0000;
    LoveTester->positions[3].x = 0x1C0000;
    LoveTester->positions[3].y = -0xF0000;
    LoveTester->positions[4].x = -0x1C0000;
    LoveTester->positions[4].y = 0x10000;
    LoveTester->positions[5].x = 0x1C0000;
    LoveTester->positions[5].y = 0x10000;
    LoveTester->positions[6].x = -0x1C0000;
    LoveTester->positions[6].y = 0x110000;
    LoveTester->positions[7].x = 0x1C0000;
    LoveTester->positions[7].y = 0x110000;
    LoveTester->positions[8].x = -0x1C0000;
    LoveTester->positions[8].y = 0x210000;
    LoveTester->positions[9].x = 0x1C0000;
    LoveTester->positions[9].y = 0x210000;
}

void LoveTester_DrawSprites(void)
{
    RSDK_THIS(LoveTester);

    int storeX = self->position.x;
    int storeY = self->position.y;
    if (SceneInfo->currentDrawGroup != Zone->drawOrderHigh || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 1, &self->animator1, true, 0);
        RSDK.DrawSprite(&self->animator1, NULL, false);

        for (int i = 0; i < 10; ++i) {
            self->position.x = storeX;
            self->position.y = storeY;
            self->position.x = storeX + LoveTester->positions[i].x;
            self->position.y = storeY + LoveTester->positions[i].y;
            RSDK.DrawSprite(&self->animators[i], NULL, false);
        }

        self->position.x = storeX;
        self->position.y = storeY;
        self->position.x = storeX + LoveTester->field_20.x;
        self->position.y = storeY + LoveTester->field_20.y;
        LoveTester_Unknown2(self->playerID, self->field_74, self->field_79);

        self->position.x = storeX;
        self->position.y = storeY;
        self->position.x = storeX + LoveTester->field_28.x;
        self->position.y = storeY + LoveTester->field_28.y;
        LoveTester_Unknown2(self->field_80, self->field_7C, self->field_81);

        self->position.x = storeX;
        self->position.y = storeY;
    }

    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 0, &self->animator1, true, 0);
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void LoveTester_Unknown2(uint8 list, bool32 flag, uint8 frame)
{
    RSDK_THIS(LoveTester);

    RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &self->animator1, true, 0);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (flag) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &self->animator1, true, 1);
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }

    switch (list) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, list + 4, &self->animator1, true, frame);
            RSDK.DrawSprite(&self->animator1, NULL, false);
            break;
        default: break;
    }
}

void LoveTester_CheckPlayerCollisions(void)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int storeX = player->position.x;
        int storeY = player->position.y;
        int velX   = player->velocity.x;
        int velY   = player->velocity.y;
        int side2  = Player_CheckCollisionBox(player, self, &LoveTester->hitbox2);
        int side   = Player_CheckCollisionBox(player, self, &LoveTester->hitbox3) | side2;
        if (side == C_TOP || side == C_BOTTOM) {
            player->onGround   = false;
            player->velocity.x = velX;
            player->velocity.y = velY;
            player->position.x = storeX;
            player->position.y = storeY;
        }
    }
}

void LoveTester_CheckPlayerCollisions2(bool32 flag)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (flag || !player->sidekick) {

            if (!((1 << playerID) & self->activePlayers)) {
                if (Player_CheckBadnikTouch(player, self, &LoveTester->hitbox1)) {
                    self->activePlayers |= 1 << playerID;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                    player->nextGroundState = 0;
                    player->nextAirState    = 0;
                    player->velocity.x >>= 15;
                    player->velocity.y >>= 15;
                    player->onGround = false;
                    player->state    = Player_State_None;
                    if (!self->playerPtr) {
                        self->playerPtr = (Entity *)player;
                        if (player->camera) {
                            player->camera->targetPtr   = (Entity *)self;
                            player->camera->offsetYFlag = false;
                        }
                    }
                }
            }

            if ((1 << playerID) & self->activePlayers) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->position.x += (self->position.x - player->position.x) >> 1;
                player->position.y += (self->position.y + 0x10000 - player->position.y) >> 1;
                player->state = Player_State_None;
                if (self->field_70) {
                    self->activePlayers &= ~(1 << playerID);
                    player->state          = Player_State_Air;
                    player->tileCollisions = true;
                    player->onGround       = false;
                    player->velocity.x     = 0;
                    player->velocity.y     = 0;
                    if (player->camera) {
                        if (self->playerPtr == (Entity *)player) {
                            player->camera->targetPtr   = (Entity *)player;
                            player->camera->offsetYFlag = true;
                        }
                    }
                }
            }
        }
    }
}

void LoveTester_GiveScore(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;

    EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, player->position.x, player->position.y);
    bonus->drawOrder        = Zone->drawOrderHigh;
    bonus->animator.frameID = 0;
    Player_GiveScore(player, 100);
    RSDK.PlaySfx(LoveTester->sfxScore, false, 255);
}

void LoveTester_Unknown6(void)
{
    RSDK_THIS(LoveTester);

    for (int velX = 0, frame = 0; velX < 0x10000; velX += 0x4000, ++frame) {
        EntityLoveTester *child = CREATE_ENTITY(LoveTester, intToVoid(true), self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 4, &child->animator1, true, frame & 1);
        child->velocity.x = velX - 0x6000;
    }
}

void LoveTester_State_Unknown1(void)
{
    RSDK_THIS(LoveTester);

    for (int i = 0; i < 10; ++i) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[i], true, 5);
    }

    self->playerPtr     = NULL;
    self->field_70      = 0;
    self->activePlayers = 0;
    self->state         = LoveTester_State_Unknown2;
    LoveTester_State_Unknown2();
}

void LoveTester_State_Unknown2(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(false);
    if (self->playerPtr) {
        self->timer    = 0;
        self->field_74 = 1;
        self->field_7C = 1;
        self->playerID = 0;
        self->field_79 = 0;
        self->field_80 = 0;
        self->field_81 = 0;
        self->state    = LoveTester_State_Unknown3;
    }
}

void LoveTester_State_Unknown3(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(true);

    if (self->timer >= 8) {
        EntityPlayer *player = (EntityPlayer *)self->playerPtr;
        self->timer        = 0;
        self->field_74     = 1;
        self->field_7C     = 1;

        switch (player->characterID) {
            case ID_SONIC: self->playerID = 1; break;
            case ID_TAILS: self->playerID = 2; break;
            case ID_KNUCKLES: self->playerID = 3; break;
        }
        self->field_82 = self->playerID;

        do {
            self->field_82 = RSDK.Rand(1, 6);
        } while (self->field_82 == self->playerID);
        self->state = LoveTester_State_Unknown4;
    }
    else {
        self->field_74 = !(self->timer & 2);
        self->field_7C = !(self->timer & 2);
        self->timer++;
    }
}

void LoveTester_State_Unknown4(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(true);

    if (self->timer < 168) {
        int val     = self->timer & 0x80000003;
        bool32 flag = !val;
        if (val < 0)
            flag = ((val - 1) | 0xFFFFFFFC) == -1;

        if (flag) {
            int startVal = self->field_80;
            int id       = self->playerID;
            while (true) {
                self->field_80 = id;
                if (id != self->playerID && id != startVal)
                    break;
                id = RSDK.Rand(1, 6);
            }
        }

        if (self->timer >= 128)
            self->field_80 = 0;
        if (self->timer) {
            if (self->timer == 80) {
                self->field_88  = 0;
                self->stateLate = LoveTester_StateLate_Unknown2;
            }
            else {
                if (self->timer == 128) {
                    self->field_88  = 0;
                    self->stateLate = LoveTester_StateLate_Unknown3;
                }
            }
        }
        else {
            self->field_88  = 0;
            self->stateLate = LoveTester_StateLate_Unknown1;
        }
        self->timer++;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)self->playerPtr;
        self->field_88     = 0;
        self->stateLate    = 0;
        switch (player->characterID) {
            case ID_SONIC:
                if (self->field_82 == 4)
                    self->state = LoveTester_State_Unknown7;
                else if (self->field_82 == 5)
                    self->state = LoveTester_State_Unknown5;
                else
                    self->state = LoveTester_State_Unknown6;
                break;
            case ID_TAILS:
                if (self->field_82 == 1)
                    self->state = LoveTester_State_Unknown5;
                else if (self->field_82 == 4)
                    self->state = LoveTester_State_Unknown5;
                else
                    self->state = LoveTester_State_Unknown6;
                break;
            case ID_KNUCKLES:
                if (self->field_82 == 1)
                    self->state = LoveTester_State_Unknown7;
                else if (self->field_82 == 4)
                    self->state = LoveTester_State_Unknown5;
                else
                    self->state = LoveTester_State_Unknown6;
                break;
        }

        if (self->state == LoveTester_State_Unknown5) {
            self->field_79 = 1;
            LoveTester_Unknown6();
        }
        else if (self->state == LoveTester_State_Unknown7) {
            self->field_79 = 2;
        }

        self->field_80 = self->field_82;
        self->timer    = 0;
    }
}

void LoveTester_State_Unknown5(void)
{
    RSDK_THIS(LoveTester);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (self->timer >= 64) {
        self->timer    = 0;
        self->field_80 = self->field_82;
        self->field_70 = 1;
        self->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        int timer = self->timer % 32;
        if (timer) {
            if (timer == 16) {
                self->field_80 = 6;
                LoveTester_GiveScore(player);
            }
            ++self->timer;
            LoveTester_CheckPlayerCollisions();
            LoveTester_CheckPlayerCollisions2(true);
        }
        else {
            self->field_80 = self->field_82;
            LoveTester_GiveScore(player);
            ++self->timer;
            LoveTester_CheckPlayerCollisions();
            LoveTester_CheckPlayerCollisions2(true);
        }
    }
}

void LoveTester_State_Unknown6(void)
{
    RSDK_THIS(LoveTester);

    if (self->timer >= 24) {
        self->timer    = 0;
        self->field_70 = 1;
        self->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        if (!self->timer)
            LoveTester_GiveScore(self->playerPtr);
        ++self->timer;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
}

void LoveTester_State_Unknown7(void)
{
    RSDK_THIS(LoveTester);

    if (self->timer >= 64) {
        self->timer    = 0;
        self->field_80 = self->field_82;
        self->field_70 = 1;
        self->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else if (self->timer % 32) {
        if ((self->timer % 32) == 16)
            self->field_80 = 7;
        self->timer++;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        self->field_80 = self->field_82;
        self->timer++;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
}

void LoveTester_State_Unknown8(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    foreach_active(Player, player)
    {
        if (player == (EntityPlayer *)self->playerPtr) {
            if (!Player_CheckCollisionTouch(player, self, &LoveTester->hitbox1)) {
                self->activePlayers = 0;
                self->state         = LoveTester_State_Unknown1;
            }
        }
    }
}

void LoveTester_State2_Unknown(void)
{
    RSDK_THIS(LoveTester);

    self->position.x += self->velocity.x;
    self->velocity.y -= 0x700;
    self->position.y += self->velocity.y;
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void LoveTester_StateLate_Unknown1(void)
{
    RSDK_THIS(LoveTester);

    switch (self->field_88 % 40) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[8], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[6], true, 0);
            break;
        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[5], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[4], true, 0);
            break;
        case 24:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[7], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[2], true, 0);
            break;
        case 32:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[9], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[0], true, 0);
            break;
    }
    ++self->field_88;
}

void LoveTester_StateLate_Unknown2(void)
{
    RSDK_THIS(LoveTester);

    switch (self->field_88 % 24) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, self->animators, true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[9], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[7], true, 0);
            break;
        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[5], true, 0);
            break;
    }
    ++self->field_88;
}

void LoveTester_StateLate_Unknown3(void)
{
    RSDK_THIS(LoveTester);

    switch (self->field_88 % 20) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[0], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[1], true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[3], true, 0);
            break;
        case 4:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[5], true, 0);
            break;
        case 6:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[7], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[9], true, 0);
            break;
    }
    ++self->field_88;
}

#if RETRO_INCLUDE_EDITOR
void LoveTester_EditorDraw(void)
{
    RSDK_THIS(LoveTester);

    for (int i = 0; i < 10; ++i) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &self->animators[i], true, 5);
    }

    LoveTester_Draw();
}

void LoveTester_EditorLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);

    LoveTester_SetupHitboxes();
    LoveTester_SetupPositions();
}
#endif

void LoveTester_Serialize(void) {}

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Crane Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Crane *PBL_Crane;

void PBL_Crane_Update(void)
{
    RSDK_THIS(PBL_Crane);

    StateMachine_Run(self->state);
}

void PBL_Crane_LateUpdate(void) {}

void PBL_Crane_StaticUpdate(void) {}

void PBL_Crane_Draw(void)
{
    RSDK_THIS(PBL_Crane);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);
    }
}

void PBL_Crane_Create(void *data)
{
    RSDK_THIS(PBL_Crane);

    if (!SceneInfo->inEditor) {
        self->drawGroup = 9;
        self->active    = ACTIVE_NORMAL;
        self->type      = VOID_TO_INT(data);

        switch (self->type) {
            case PBL_CRANE_MANAGER:
                self->position.x = ScreenInfo->center.x << 16;
                self->position.y = 0;

                self->alpha   = 0xFF;
                self->scale.x = 0x600;
                self->scale.y = 0x200;

                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &self->displayAnimator, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 0, &self->machineAnimator, true, 1);

                self->stateDraw = PBL_Crane_Draw_CraneMachine;
                break;

            case PBL_CRANE_CRANE:
                self->visible = true;

                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 3, &self->bgAnimator, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 4, &self->craneBackAnimator, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 5, &self->craneFrontAnimator, true, 0);

                self->scale.x = 0x200;
                self->scale.y = 0x200;

                self->state     = PBL_Crane_StateCrane_Lower;
                self->stateDraw = PBL_Crane_Draw_Crane;
                break;

            case PBL_CRANE_PRIZEDISPLAY:
                self->visible = true;

                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 6, &self->displayAnimator, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &self->bgAnimator, true, 4);

                self->state     = PBL_Crane_StatePrizeDisplay_Move;
                self->stateDraw = PBL_Crane_Draw_PrizeDisplay;
                break;

            case PBL_CRANE_PRIZE:
                self->drawGroup = 12;
                self->visible   = true;
                self->scale.x   = 0x180;
                self->scale.y   = 0x180;
                self->drawFX    = FX_SCALE;

                self->state = PBL_Crane_StatePrize_Bounce;
                break;

            default: break;
        }
    }
}

void PBL_Crane_StageLoad(void)
{
    PBL_Crane->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE);

    PBL_Crane->sfxBackGlass = RSDK.GetSfx("Pinball/Backglass.wav");
    PBL_Crane->sfxCraneMove = RSDK.GetSfx("Pinball/CraneMove.wav");
    PBL_Crane->sfxCraneDrop = RSDK.GetSfx("Pinball/CraneDrop.wav");
    PBL_Crane->sfxCraneGrab = RSDK.GetSfx("Pinball/CraneGrab.wav");
    PBL_Crane->sfxCraneRise = RSDK.GetSfx("Pinball/CraneRise.wav");
    PBL_Crane->sfxPrizeGood = RSDK.GetSfx("Pinball/PrizeGood.wav");
    PBL_Crane->sfxPrizeBad  = RSDK.GetSfx("Pinball/PrizeBad.wav");
}

void PBL_Crane_HandlePrizes(void)
{
    RSDK_THIS(PBL_Crane);

    switch (self->displayAnimator.frameID) {
        case PBL_CRANE_PRIZE_SONIC:
        case PBL_CRANE_PRIZE_TAILS:
        case PBL_CRANE_PRIZE_KNUCKLES:
        case PBL_CRANE_PRIZE_MIGHTY:
        case PBL_CRANE_PRIZE_RAY: {
            int32 playerID = 1 << self->displayAnimator.frameID;
            globals->characterFlags |= playerID;
            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_BUDDY;

            if (!GET_CHARACTER_ID(1))
                globals->playerID |= playerID;
            else if (!GET_CHARACTER_ID(2))
                globals->playerID |= playerID << 8;
            else if (!GET_STOCK_ID(1))
                globals->stock |= playerID;
            else if (!GET_STOCK_ID(2))
                globals->stock |= playerID << 8;
            else if (!GET_STOCK_ID(3))
                globals->stock |= playerID << 16;

            break;
        }

        case PBL_CRANE_PRIZE_EGGMAN:
            PBL_Setup->rings   = 0;
            PBL_Setup->score   = 0;
            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_BAD;
            break;

        case PBL_CRANE_PRIZE_RINGS:
            globals->restartRings += PBL_Setup->rings;
            globals->restartRings = CLAMP(globals->restartRings, 0, 999);

            if (globals->gameMode == MODE_MANIA && globals->restartRings >= globals->restart1UP) {
                PBL_Setup_GiveLife();
                globals->restart1UP += 100;
                if (globals->restart1UP > 300)
                    globals->restart1UP = 1000;
            }

            PBL_Setup->rings   = 0;
            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_RINGS;
            break;

        case PBL_CRANE_PRIZE_SHIELD_BLUE:
        case PBL_CRANE_PRIZE_SHIELD_BUBBLE:
        case PBL_CRANE_PRIZE_SHIELD_FIRE:
        case PBL_CRANE_PRIZE_SHIELD_ELECTRIC:
            globals->restartPowerups &= ~0x3F;
            globals->restartPowerups |= self->displayAnimator.frameID - PBL_CRANE_PRIZE_SHIELD_BLUE + SHIELD_BLUE; // Converts the frame ID to a shield ID

            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_ITEM;
            break;

        case PBL_CRANE_PRIZE_TABLE_RESTORE: {
            foreach_all(PBL_Bumper, bumper)
            {
                if (!bumper->state) {
                    bumper->velocity.y = 32;
                    bumper->state      = PBL_TargetBumper_State_Rise;
                    bumper->active     = ACTIVE_NORMAL;
                }
            }

            foreach_all(PBL_Ring, ring)
            {
                if (ring->active == ACTIVE_NEVER) {
                    ring->active = ACTIVE_BOUNDS;
                    RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 1, &ring->animator, true, 0);
                    ring->state = PBL_Ring_State_Ring;
                }
            }

            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_TBLRESTORE;
            break;
        }

        case PBL_CRANE_PRIZE_1UP:
            if (SaveGame_GetSaveRAM()->lives < 99)
                SaveGame_GetSaveRAM()->lives++;

            if (globals->restartLives[0] < 99)
                globals->restartLives[0]++;

            PBL_Crane->prizeID = PBL_CRANE_PRIZEID_1UP;
            break;

        default: break;
    }
}

void PBL_Crane_Draw_CraneMachine(void)
{
    RSDK_THIS(PBL_Crane);

    self->position.x = ScreenInfo->center.x << 16;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        self->displayAnimator.frameID = 1;
        self->inkEffect               = INK_NONE;
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);
        RSDK.DrawSprite(&self->machineAnimator, NULL, true);

        RSDK.AddDrawListRef(11, SceneInfo->entitySlot);

        RSDK.SetClipBounds(0, ScreenInfo->center.x - 96, 0, ScreenInfo->center.x + 96, (self->position.y >> 16) + 64);
    }
    else {
        RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);

        self->displayAnimator.frameID = 0;
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);

        self->displayAnimator.frameID = 2;
        self->drawFX                  = FX_SCALE;
        self->inkEffect               = INK_ADD;
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);

        self->displayAnimator.frameID = 3;
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);

        self->drawFX = FX_NONE;
    }
}

void PBL_Crane_Draw_PrizeDisplay(void)
{
    RSDK_THIS(PBL_Crane);

    RSDK.DrawSprite(&self->bgAnimator, NULL, true);
    RSDK.DrawSprite(&self->displayAnimator, NULL, true);
}

void PBL_Crane_Draw_Crane(void)
{
    RSDK_THIS(PBL_Crane);

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x1A0000;
    RSDK.DrawSprite(&self->craneBackAnimator, NULL, true);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->displayAnimator, &drawPos, true);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->bgAnimator, NULL, true);
    RSDK.DrawSprite(&self->craneFrontAnimator, NULL, true);
}

void PBL_Crane_State_CreatePrizes(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);

    self->position.y   = 0;
    camera->worldY     = 0x1000000;
    self->cameraTarget = camera->target;
    camera->target     = NULL;
    self->visible      = true;

    int32 spawnX = self->position.x - 0x6C0000;
    int32 spawnY = 0x600000 + self->position.y;
    self->state  = PBL_Crane_State_DisplayPrizes;

    for (int32 i = 1; i < 6; ++i) {
        EntityPBL_Crane *prize = CREATE_ENTITY(PBL_Crane, INT_TO_VOID(PBL_CRANE_PRIZEDISPLAY), spawnX, spawnY);
        if (globals->gameMode == MODE_ENCORE) {
            if (!((1 << i) & globals->characterFlags) || i == PBL_CRANE_PRIZE_EGGMAN) {
                prize->displayAnimator.frameID = i;
            }
            else {
                prize->displayAnimator.frameID = RSDK.Rand(PBL_CRANE_PRIZE_RINGS, PBL_CRANE_PRIZE_TABLE_RESTORE + 1);
            }
        }
        else {
            if (i == 5) {
                prize->displayAnimator.frameID = PBL_CRANE_PRIZE_EGGMAN;
            }
            else {
                prize->displayAnimator.frameID = RSDK.Rand(PBL_CRANE_PRIZE_RINGS, PBL_CRANE_PRIZE_1UP + 1);
            }
        }

        spawnX += 0x280000;
        prize->parent = self;
    }

    CREATE_ENTITY(PBL_Crane, INT_TO_VOID(PBL_CRANE_CRANE), self->position.x, -0x180000)->parent = self;

    if (PBL_Setup->sectorID < PBL_Setup->sectorCount)
        PBL_Setup->sectorID++;

    PBL_Crane->prizeID = PBL_CRANE_PRIZEID_NOTHING;
}

void PBL_Crane_State_DisplayPrizes(void)
{
    RSDK_THIS(PBL_Crane);

    if (self->position.y >= 0x300000) {
        self->timer = 0;
        self->state = StateMachine_None;
    }
    else {
        EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);

        self->position.y += 0x10000;
        camera->worldY += 0x28000;
        camera->position.y -= 0x14000;

        if (!self->timer)
            --camera->rotationY;

        self->timer = (self->timer + 1) & 3;
    }
}

void PBL_Crane_StatePrizeDisplay_Move(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Crane *parent = self->parent;

    self->position.x -= 0x10000;
    int32 dist       = abs(self->position.x - parent->position.x);
    self->position.y = (dist >> 13) * (dist >> 13) + (parent->position.y + 0x300000);

    if (self->position.x < parent->position.x - 0x980000) {
        self->position.x += 0x1100000;
    }
}

void PBL_Crane_StateCrane_Lower(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Crane *parent = self->parent;

    if (!parent->state) {
        if (self->position.y >= 0x100000)
            self->state = PBL_Crane_StateCrane_Move;
        else
            self->position.y += 0x8000;
    }
}

void PBL_Crane_StateCrane_Move(void)
{
    RSDK_THIS(PBL_Crane);

    self->velocity.x = (-((ControllerInfo[CONT_P1].keyLeft.down || AnalogStickInfoL[CONT_P1].keyLeft.down) != 0) & 0xFFFF0000) + 0x8000;

    if (++self->timer == 30) {
        self->timer = 0;
        self->state = PBL_Crane_StateCrane_Controlled;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
}

void PBL_Crane_StateCrane_Controlled(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Crane *parent = self->parent;

    if (AnalogStickInfoL[CONT_P1].keyLeft.press || ControllerInfo[CONT_P1].keyLeft.press) {
        ++self->timer;
        self->velocity.x = -0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 0xFF);
    }
    else if (AnalogStickInfoL[CONT_P1].keyRight.press || ControllerInfo[CONT_P1].keyRight.press) {
        ++self->timer;
        self->velocity.x = 0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 0xFF);
    }
    else if (AnalogStickInfoL[CONT_P1].keyDown.press || ControllerInfo[CONT_P1].keyDown.press) {
        self->timer = 4;
    }

    self->position.x += self->velocity.x;

    if (self->velocity.x <= 0) {
        if (self->position.x <= parent->position.x - 0x600000)
            self->velocity.x = -self->velocity.x;
    }
    else {
        if (self->position.x >= parent->position.x + 0x600000)
            self->velocity.x = -self->velocity.x;
    }

    if (self->timer == 4) {
        self->timer = 0;
        self->state = PBL_Crane_StateCrane_Drop;
        RSDK.PlaySfx(PBL_Crane->sfxCraneDrop, false, 255);
    }
}

void PBL_Crane_StateCrane_Drop(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Crane *parent = self->parent;

    self->position.y += 0x10000;
    if (self->position.y >= parent->position.y + 0x180000) {
        foreach_active(PBL_Crane, crane)
        {
            if (crane->type == PBL_CRANE_PRIZEDISPLAY) {
                if (abs(crane->position.x - self->position.x) < 0x100000) {
                    memcpy(&self->displayAnimator, &crane->displayAnimator, sizeof(Animator));
                    RSDK.SetSpriteAnimation(-1, 0, &crane->displayAnimator, false, 0);
                }
            }
        }

        self->state = PBL_Crane_StateCrane_Grab;
        RSDK.PlaySfx(PBL_Crane->sfxCraneGrab, false, 255);
    }
}

void PBL_Crane_StateCrane_Grab(void)
{
    RSDK_THIS(PBL_Crane);

    ++self->timer;

    if (self->scale.x > 0x180) {
        self->scale.x -= 0x10;
        self->scale.y -= 0x10;
    }

    if (self->timer > 8) {
        self->craneBackAnimator.frameID  = (self->timer - 8) >> 1;
        self->craneFrontAnimator.frameID = (self->timer - 8) >> 1;
    }

    if (self->craneBackAnimator.frameID == self->craneBackAnimator.frameCount - 1) {
        self->timer = 0;
        self->state = PBL_Crane_StateCrane_Rise;
        RSDK.PlaySfx(PBL_Crane->sfxCraneRise, false, 255);
    }
}

void PBL_Crane_StateCrane_Rise(void)
{
    RSDK_THIS(PBL_Crane);

    EntityPBL_Crane *parent = self->parent;

    self->position.y -= 0x10000;

    if (self->scale.x > 0x180) {
        self->scale.x -= 0x10;
        self->scale.y -= 0x10;
    }

    if (self->position.y <= 0x100000) {
        if (self->displayAnimator.animationID == 6) {
            EntityPBL_Crane *crane = CREATE_ENTITY(PBL_Crane, INT_TO_VOID(PBL_CRANE_PRIZE), self->position.x, self->position.y + 0x1A0000);
            crane->displayAnimator = self->displayAnimator;
            crane->parent          = self->parent;
            RSDK.SetSpriteAnimation(-1, 0, &self->displayAnimator, false, 0);

            crane->velocity.x = self->position.x > parent->position.x ? 0x20000 : -0x20000;
            crane->velocity.y = -0x20000;

            if (crane->displayAnimator.frameID != 5) {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeGood, false, 255);
                self->state = StateMachine_None;
            }
            else {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
                self->state = StateMachine_None;
            }
        }
        else {
            parent->state = PBL_Crane_StatePrize_PrizeGet;
            RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
            self->state = StateMachine_None;
        }
    }
}

void PBL_Crane_StatePrize_Bounce(void)
{
    RSDK_THIS(PBL_Crane);

    if (self->scale.x >= 0x200) {
        self->drawFX = FX_NONE;
    }
    else {
        self->scale.x += 0x10;
        self->scale.y += 0x10;
    }

    self->velocity.y += 0x2800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y > 0x500000) {
        self->velocity.y = -(self->velocity.y >> 1);

        if (++self->timer == 2) {
            self->timer = 0;
            self->state = PBL_Crane_StatePrize_Flash;
        }
    }
}

void PBL_Crane_StatePrize_Flash(void)
{
    RSDK_THIS(PBL_Crane);

    self->visible = (++self->timer >> 2) & 1;

    if (self->timer == 60) {
        PBL_Crane_HandlePrizes();
        self->parent->state = PBL_Crane_StatePrize_PrizeGet;
        destroyEntity(self);
    }
}

void PBL_Crane_StatePrize_PrizeGet(void)
{
    RSDK_THIS(PBL_Crane);

    if (++self->timer == 24) {
        foreach_active(PBL_HUD, hud)
        {
            hud->state   = PBL_HUD_State_HideCrane;
            hud->visible = true;
        }
    }

    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    if (self->position.y <= 0) {
        foreach_active(PBL_Crane, crane)
        {
            if (crane != self)
                destroyEntity(crane);
        }

        self->timer = 0;
        self->state = StateMachine_None;

        foreach_all(PBL_Sector, sector)
        {
            if (sector->craneID == PBL_Setup->sectorID)
                sector->active = ACTIVE_NORMAL;
        }

        LogHelpers_PrintInt32("Sector", PBL_Setup->sectorID);
        camera->target    = self->cameraTarget;
        camera->rotationY = -96;

        foreach_active(PBL_HUD, hud)
        {
            switch (PBL_Crane->prizeID) {
                case PBL_CRANE_PRIZEID_NOTHING: PBL_HUD_DisplayMessage(hud, "TOO BAD!", PBL_HUD_MSG_FLASH); continue;
                case PBL_CRANE_PRIZEID_BAD: PBL_HUD_DisplayMessage(hud, "OH NO!", PBL_HUD_MSG_FLASH); continue;
                case PBL_CRANE_PRIZEID_BUDDY: PBL_HUD_DisplayMessage(hud, "NICE SAVE!", PBL_HUD_MSG_SCROLL_RIGHT); continue;
                case PBL_CRANE_PRIZEID_RINGS: PBL_HUD_DisplayMessage(hud, "!RINGS DEPOSITED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case PBL_CRANE_PRIZEID_ITEM: PBL_HUD_DisplayMessage(hud, "!ITEM COLLECTED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case PBL_CRANE_PRIZEID_TBLRESTORE: PBL_HUD_DisplayMessage(hud, "!TABLE RESTORED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case PBL_CRANE_PRIZEID_1UP: PBL_HUD_DisplayMessage(hud, "!EXTRA LIFE!", PBL_HUD_MSG_SCROLL_LEFT); break;
                default: break;
            }
        }
    }
    else {
        self->position.y = self->position.y - 0x10000;
        camera->worldY -= 0x28000;
        camera->position.y += 0x14000;

        if ((self->timer & 3) == 1)
            ++camera->rotationY;
    }
}

#if GAME_INCLUDE_EDITOR
void PBL_Crane_EditorDraw(void)
{
    RSDK_THIS(PBL_Crane);

    RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 4, &self->craneBackAnimator, true, 0);
    RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 5, &self->craneFrontAnimator, true, 0);

    RSDK.DrawSprite(&self->craneBackAnimator, NULL, false);
    RSDK.DrawSprite(&self->craneFrontAnimator, NULL, false);
}

void PBL_Crane_EditorLoad(void) { PBL_Crane->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE); }
#endif

void PBL_Crane_Serialize(void) {}
#endif

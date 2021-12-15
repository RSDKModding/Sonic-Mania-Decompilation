// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Crane Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
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
        RSDK.DrawSprite(&self->animator1, NULL, true);
    }
}

void PBL_Crane_Create(void *data)
{
    RSDK_THIS(PBL_Crane);
    if (!SceneInfo->inEditor) {
        self->drawOrder = 9;
        self->active    = ACTIVE_NORMAL;
        self->type      = voidToInt(data);
        switch (self->type) {
            case 0:
                self->position.x = ScreenInfo->centerX << 16;
                self->position.y = 0;
                self->alpha      = 0xFF;
                self->scale.x    = 0x600;
                self->scale.y    = 0x200;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 0, &self->animator5, true, 1);
                self->stateDraw = PBL_Crane_StateDraw_Unknown1;
                break;
            case 1:
                self->visible = true;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 3, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 4, &self->animator3, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 5, &self->animator4, true, 0);
                self->scale.x   = 0x200;
                self->scale.y   = 0x200;
                self->state     = PBL_Crane_State_Unknown1;
                self->stateDraw = PBL_Crane_StateDraw_Unknown2;
                break;
            case 2:
                self->visible = true;
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 6, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(PBL_Crane->aniFrames, 2, &self->animator2, true, 4);
                self->state     = PBL_Crane_Unknown7;
                self->stateDraw = PBL_Crane_StateDraw_Unknown3;
                break;
            case 3:
                self->drawOrder = 12;
                self->visible   = true;
                self->scale.x   = 0x180;
                self->scale.y   = 0x180;
                self->drawFX    = FX_SCALE;
                self->state     = PBL_Crane_State2_Unknown1;
                break;
            default: break;
        }
    }
}

void PBL_Crane_StageLoad(void)
{
    PBL_Crane->aniFrames    = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE);
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
    switch (self->animator1.frameID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4: {
            int32 playerID = 1 << self->animator1.frameID;
            globals->characterFlags |= playerID;
            PBL_Crane->prizeID = 3;

            if (!globals->playerID) {
                globals->playerID |= playerID;
            }
            else if (!(globals->playerID & 0xFF00)) {
                globals->playerID |= playerID << 8;
            }
            else if (!globals->stock) {
                globals->stock |= playerID;
            }
            else if (!(globals->stock & 0xFF00)) {
                globals->stock |= playerID << 8;
            }
            else if (!(globals->stock & 0xFF0000)) {
                globals->stock |= playerID << 16;
            }
            break;
        }
        case 5:
            PBL_Setup->rings   = 0;
            PBL_Setup->score   = 0;
            PBL_Crane->prizeID = 2;
            break;
        case 6:
            globals->restartRings += PBL_Setup->rings;
            globals->restartRings = clampVal(globals->restartRings, 0, 999);
            if (globals->gameMode == MODE_MANIA && globals->restartRings >= globals->restart1UP) {
                PBL_Setup_GiveLife();
                globals->restart1UP += 100;
                if (globals->restart1UP > 300)
                    globals->restart1UP = 1000;
            }
            PBL_Setup->rings   = 0;
            PBL_Crane->prizeID = 4;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            globals->restartPowerups &= -0x40;
            globals->restartPowerups |= self->animator1.frameID - 6;
            PBL_Crane->prizeID = 5;
            break;
        case 11: {
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
            PBL_Crane->prizeID = 6;
            break;
        }
        case 12:
            if (SaveGame->saveRAM->lives < 99)
                SaveGame->saveRAM->lives++;
            if (globals->restartLives[0] < 99)
                globals->restartLives[0]++;
            PBL_Crane->prizeID = 7;
            break;
        default: break;
    }
}

void PBL_Crane_StateDraw_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);
    self->position.x = ScreenInfo->centerX << 16;
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->animator1.frameID = 1;
        self->inkEffect         = INK_NONE;
        RSDK.DrawSprite(&self->animator1, NULL, true);
        RSDK.DrawSprite(&self->animator5, NULL, true);

        RSDK.AddDrawListRef(11, SceneInfo->entitySlot);
        RSDK.SetClipBounds(0, ScreenInfo->centerX - 96, 0, ScreenInfo->centerX + 96, (self->position.y >> 16) + 64);
    }
    else {
        RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
        self->animator1.frameID = 0;
        RSDK.DrawSprite(&self->animator1, NULL, true);

        self->animator1.frameID = 2;
        self->drawFX            = FX_SCALE;
        self->inkEffect         = INK_ADD;
        RSDK.DrawSprite(&self->animator1, NULL, true);

        self->animator1.frameID = 3;
        RSDK.DrawSprite(&self->animator1, NULL, true);

        self->drawFX = FX_NONE;
    }
}

void PBL_Crane_StateDraw_Unknown3(void)
{
    RSDK_THIS(PBL_Crane);
    RSDK.DrawSprite(&self->animator2, NULL, true);
    RSDK.DrawSprite(&self->animator1, NULL, true);
}

void PBL_Crane_StateDraw_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x1A0000;
    RSDK.DrawSprite(&self->animator3, NULL, true);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->animator1, &drawPos, true);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, true);
    RSDK.DrawSprite(&self->animator4, NULL, true);
}

void PBL_Crane_State_CreatePrizes(void)
{
    RSDK_THIS(PBL_Crane);
    self->position.y       = 0;
    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    camera->worldY           = 0x1000000;
    self->cameraTarget         = camera->targetPtr;
    camera->targetPtr        = NULL;
    self->visible          = true;
    int32 spawnX               = self->position.x - 0x6C0000;
    int32 spawnY               = 0x600000 + self->position.y;
    self->state            = PBL_Crane_Unknown6;

    for (int32 i = 1; i < 6; ++i) {
        EntityPBL_Crane *prize = CREATE_ENTITY(PBL_Crane, intToVoid(2), spawnX, spawnY);
        if (globals->gameMode == MODE_ENCORE) {
            if (!((1 << i) & globals->characterFlags) || i == 5) {
                prize->animator1.frameID = i;
            }
            else {
                prize->animator1.frameID = RSDK.Rand(6, 12);
            }
        }
        else {
            if (i == 5) {
                prize->animator1.frameID = 5;
            }
            else {
                prize->animator1.frameID = RSDK.Rand(6, 13);
            }
        }

        spawnX += 0x280000;
        prize->parent = (Entity *)self;
    }

    CREATE_ENTITY(PBL_Crane, intToVoid(1), self->position.x, -0x180000)->parent = (Entity *)self;
    if (PBL_Setup->sectorID < PBL_Setup->sectorCount)
        PBL_Setup->sectorID++;
    PBL_Crane->prizeID = 1;
}

void PBL_Crane_Unknown6(void)
{
    RSDK_THIS(PBL_Crane);

    if (self->position.y >= 0x300000) {
        self->timer = 0;
        self->state = StateMachine_None;
    }
    else {
        self->position.y += 0x10000;
        EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
        camera->worldY += 0x28000;
        camera->position.y -= 0x14000;
        if (!self->timer)
            --camera->rotationY;
        self->timer = (self->timer + 1) & 3;
    }
}

void PBL_Crane_Unknown7(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)self->parent;

    self->position.x -= 0x10000;
    int32 dist           = abs(self->position.x - parent->position.x);
    self->position.y = (dist >> 13) * (dist >> 13) + (parent->position.y + 0x300000);
    if (self->position.x < parent->position.x - 0x980000) {
        self->position.x += 0x1100000;
    }
}

void PBL_Crane_State_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)self->parent;

    if (!parent->state) {
        if (self->position.y >= 0x100000) {
            self->state = PBL_Crane_State_Unknown2;
        }
        else {
            self->position.y += 0x8000;
        }
    }
}

void PBL_Crane_State_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);

    self->velocity.x = (-((ControllerInfo[1].keyLeft.down || AnalogStickInfoL[1].keyLeft.down) != 0) & 0xFFFF0000) + 0x8000;
    if (++self->timer == 30) {
        self->timer = 0;
        self->state = PBL_Crane_State_Unknown3;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
}

void PBL_Crane_State_Unknown3(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)self->parent;

    if (AnalogStickInfoL[1].keyLeft.press || ControllerInfo[1].keyLeft.press) {
        ++self->timer;
        self->velocity.x = -0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
    else if (AnalogStickInfoL[1].keyRight.press || ControllerInfo[1].keyRight.press) {
        ++self->timer;
        self->velocity.x = 0x18000;
        RSDK.PlaySfx(PBL_Crane->sfxCraneMove, false, 255);
    }
    else if (AnalogStickInfoL[1].keyDown.press || ControllerInfo[1].keyDown.press) {
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
        self->state = PBL_Crane_State_Unknown4;
        RSDK.PlaySfx(PBL_Crane->sfxCraneDrop, false, 255);
    }
}

void PBL_Crane_State_Unknown4(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)self->parent;

    self->position.y += 0x10000;
    if (self->position.y >= parent->position.y + 0x180000) {
        foreach_active(PBL_Crane, crane)
        {
            if (crane->type == 2) {
                if (abs(crane->position.x - self->position.x) < 0x100000) {
                    memcpy(&self->animator1, &crane->animator1, sizeof(Animator));
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &crane->animator1, false, 0);
                }
            }
        }
        self->state = PBL_Crane_State_Unknown5;
        RSDK.PlaySfx(PBL_Crane->sfxCraneGrab, false, 255);
    }
}

void PBL_Crane_State_Unknown5(void)
{
    RSDK_THIS(PBL_Crane);
    ++self->timer;
    if (self->scale.x > 384) {
        self->scale.x -= 16;
        self->scale.y -= 16;
    }

    if (self->timer > 8) {
        self->animator3.frameID = (self->timer - 8) >> 1;
        self->animator4.frameID = (self->timer - 8) >> 1;
    }

    if (self->animator3.frameID == self->animator3.frameCount - 1) {
        self->timer = 0;
        self->state = PBL_Crane_State_Unknown6;
        RSDK.PlaySfx(PBL_Crane->sfxCraneRise, false, 255);
    }
}

void PBL_Crane_State_Unknown6(void)
{
    RSDK_THIS(PBL_Crane);
    EntityPBL_Crane *parent = (EntityPBL_Crane *)self->parent;

    self->position.y -= 0x10000;
    if (self->scale.x > 0x180) {
        self->scale.x -= 16;
        self->scale.y -= 16;
    }

    if (self->position.y <= 0x100000) {
        if (self->animator1.animationID == 6) {
            EntityPBL_Crane *crane = CREATE_ENTITY(PBL_Crane, intToVoid(3), self->position.x, self->position.y + 0x1A0000);
            crane->animator1       = self->animator1;
            crane->parent          = self->parent;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator1, false, 0);

            crane->velocity.y = -0x20000;
            if (self->position.x > parent->position.x)
                crane->velocity.x = 0x20000;
            else
                crane->velocity.x = -0x20000;
            if (crane->animator1.frameID != 5) {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeGood, false, 255);
                self->state = StateMachine_None;
            }
            else {
                RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
                self->state = StateMachine_None;
            }
        }
        else {
            parent->state = PBL_Crane_State2_Unknown3;
            RSDK.PlaySfx(PBL_Crane->sfxPrizeBad, false, 255);
            self->state = StateMachine_None;
        }
    }
}

void PBL_Crane_State2_Unknown1(void)
{
    RSDK_THIS(PBL_Crane);

    if (self->scale.x >= 512) {
        self->drawFX = FX_NONE;
    }
    else {
        self->scale.x += 16;
        self->scale.y += 16;
    }

    self->velocity.y += 0x2800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->position.y > 0x500000) {
        self->velocity.y = -(self->velocity.y >> 1);
        if (++self->timer == 2) {
            self->timer = 0;
            self->state = PBL_Crane_State2_Unknown2;
        }
    }
}

void PBL_Crane_State2_Unknown2(void)
{
    RSDK_THIS(PBL_Crane);
    self->visible = (++self->timer >> 2) & 1;
    if (self->timer == 60) {
        PBL_Crane_HandlePrizes();
        ((EntityPBL_Crane *)self->parent)->state = PBL_Crane_State2_Unknown3;
        destroyEntity(self);
    }
}

void PBL_Crane_State2_Unknown3(void)
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
        RSDK.PrintInteger(PRINT_NORMAL, "Sector", PBL_Setup->sectorID);
        camera->targetPtr = self->cameraTarget;
        camera->rotationY = -96;
        foreach_active(PBL_HUD, hud)
        {
            switch (PBL_Crane->prizeID) {
                case 1: PBL_HUD_DisplayMessage(hud, "TOO BAD!", PBL_HUD_MSG_FLASH); continue;
                case 2: PBL_HUD_DisplayMessage(hud, "OH NO!", PBL_HUD_MSG_FLASH); continue;
                case 3: PBL_HUD_DisplayMessage(hud, "NICE SAVE!", PBL_HUD_MSG_SCROLL_RIGHT); continue;
                case 4: PBL_HUD_DisplayMessage(hud, "!RINGS DEPOSITED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case 5: PBL_HUD_DisplayMessage(hud, "!ITEM COLLECTED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case 6: PBL_HUD_DisplayMessage(hud, "!TABLE RESTORED!", PBL_HUD_MSG_SCROLL_LEFT); break;
                case 7: PBL_HUD_DisplayMessage(hud, "!EXTRA LIFE!", PBL_HUD_MSG_SCROLL_LEFT); break;
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

#if RETRO_INCLUDE_EDITOR
void PBL_Crane_EditorDraw(void) {}

void PBL_Crane_EditorLoad(void) {}
#endif

void PBL_Crane_Serialize(void) {}
#endif

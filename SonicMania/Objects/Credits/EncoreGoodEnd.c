// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EncoreGoodEnd Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectEncoreGoodEnd *EncoreGoodEnd;

void EncoreGoodEnd_Update(void)
{
    RSDK_THIS(EncoreGoodEnd);

    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                CutsceneSeq_StartSequence(self, EncoreGoodEnd_Cutscene_MoveToPlace, EncoreGoodEnd_Cutscene_WaitForMovementFinish,
                                          EncoreGoodEnd_Cutscene_ClinkGlasses, EncoreGoodEnd_Cutscene_KingAppear,
                                          EncoreGoodEnd_Cutscene_ThanksForPlaying, EncoreGoodEnd_Cutscene_FinishCutscene, StateMachine_None);
#if MANIA_USE_PLUS
                CutsceneSeq_SetSkipType(SKIPTYPE_CALLBACK, EncoreGoodEnd_Cutscene_SkipCB);
#endif

                self->activated = true;
            }
        }
    }
}

void EncoreGoodEnd_LateUpdate(void) {}

void EncoreGoodEnd_StaticUpdate(void) {}

void EncoreGoodEnd_Draw(void) {}

void EncoreGoodEnd_Create(void *data)
{
    RSDK_THIS(EncoreGoodEnd);

    if (!SceneInfo->inEditor) {
        INIT_ENTITY(self);
        self->active = ACTIVE_NORMAL;
        CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);

        SceneInfo->timeEnabled = false;
        EncoreGoodEnd->unused1 = 0;
        Player->playerCount    = 4;

        EncoreGoodEnd_SetupDecorations();
    }
}

void EncoreGoodEnd_StageLoad(void) { EncoreGoodEnd->sfxSnap = RSDK.GetSfx("SPZ/ShBugSnap.wav"); }

void EncoreGoodEnd_SetupDecorations(void)
{
    int32 id = 0;
    foreach_all(Decoration, decor)
    {
        if (id >= E_END_COUNT)
            break;

        if (id == E_END_ICECREAM_CONE)
            decor->drawFX = FX_NONE;
        if (id >= E_END_ICECREAM_CONE)
            decor->visible = false;

        EncoreGoodEnd->decorations[id++] = decor;
    }
}

void EncoreGoodEnd_SetupPlayer(int32 playerID)
{
    switch (playerID) {
        default: break;

        case ID_SONIC: EncoreGoodEnd->decorations[E_END_SONIC]->visible = false; break;

        case ID_TAILS:
            EncoreGoodEnd->decorations[E_END_TAILS]->visible = false;
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL_STILL, &EncoreGoodEnd->decorations[E_END_STOOLT]->animator, false, 0);
            break;

        case ID_KNUCKLES:
            EncoreGoodEnd->decorations[E_END_KNUX]->visible   = false;
            EncoreGoodEnd->decorations[E_END_GLASSK]->visible = false;
            break;

        case ID_MIGHTY:
            EncoreGoodEnd->decorations[E_END_MIGHTY]->visible = false;
            EncoreGoodEnd->decorations[E_END_GLASSM]->visible = false;
            break;

        case ID_RAY:
            EncoreGoodEnd->decorations[E_END_RAY]->visible = false;
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL_STILL, &EncoreGoodEnd->decorations[E_END_STOOLR]->animator, false, 0);
            break;
    }
}

void EncoreGoodEnd_StatePlayer_MoveToPos(void)
{
    RSDK_THIS(Player);

    self->velocity.x = 0x18000;
    self->groundVel  = 0x18000;

    int32 playerID = HUD_CharacterIndexFromID(self->characterID);

    if (self->position.x >= EncoreGoodEnd->decorations[playerID + E_END_SONIC]->position.x) {
        self->position.x = EncoreGoodEnd->decorations[playerID + E_END_SONIC]->position.x;

        self->velocity.x = 0;
        self->groundVel  = 0;

        switch (self->characterID) {
            case ID_SONIC:
                self->visible                                               = false;
                EncoreGoodEnd->decorations[playerID + E_END_SONIC]->visible = true;
                RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_SONICLAYDOWN,
                                        &EncoreGoodEnd->decorations[playerID + E_END_SONIC]->animator, true, 0);
                self->velocity.x = 0;
                self->groundVel  = 0;
                self->state      = EncoreGoodEnd_StatePlayer_EndingSonic;
                break;

            case ID_TAILS:
            case ID_KNUCKLES:
            case ID_MIGHTY:
            case ID_RAY:
                self->jumpPress = true;
                Player_State_Ground();
                self->jumpPress       = false;
                self->state           = EncoreGoodEnd_StatePlayer_EndingIdle;
                self->nextAirState    = EncoreGoodEnd_StatePlayer_EndingIdle;
                self->nextGroundState = Player_State_Static;
                break;

            default: break;
        }
    }
    else {
        Player_State_Ground();
    }
}

void EncoreGoodEnd_StatePlayer_EndingIdle(void)
{
    RSDK_THIS(Player);

    Player_State_Air();

    if (self->velocity.y > 0) {
        int32 playerID = HUD_CharacterIndexFromID(self->characterID);

        if (self->position.y >= (EncoreGoodEnd->decorations[playerID + E_END_SONIC]->position.y - 0x80000)) {
            self->position.y = (EncoreGoodEnd->decorations[playerID + E_END_SONIC]->position.y - 0x80000);
            self->velocity.y = 0;
            self->visible    = false;

            EncoreGoodEnd->decorations[playerID + E_END_SONIC]->visible = true;
            switch (self->characterID) {
                default:
                case ID_SONIC: self->state = Player_State_Static; break;

                case ID_TAILS:
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_TAILSSPIN,
                                            &EncoreGoodEnd->decorations[playerID + E_END_SONIC]->animator, true, 0);
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL, &EncoreGoodEnd->decorations[E_END_STOOLT]->animator, true, 0);
                    self->state = Player_State_Static;
                    break;

                case ID_KNUCKLES:
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXSITDOWN,
                                            &EncoreGoodEnd->decorations[playerID + E_END_SONIC]->animator, true, 0);
                    EncoreGoodEnd->decorations[E_END_GLASSK]->visible = true;
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXSITDOWNG, &EncoreGoodEnd->decorations[E_END_GLASSK]->animator,
                                            true, 0);
                    EncoreGoodEnd->decorations[E_END_GLASSK]->animator.speed = 0;
                    self->state                                              = Player_State_Static;
                    break;

                case ID_MIGHTY:
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYSITDOWN,
                                            &EncoreGoodEnd->decorations[playerID + E_END_SONIC]->animator, true, 0);
                    EncoreGoodEnd->decorations[E_END_GLASSM]->visible = true;
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYSITDOWNG, &EncoreGoodEnd->decorations[E_END_GLASSM]->animator,
                                            true, 0);
                    EncoreGoodEnd->decorations[E_END_GLASSM]->animator.speed = 0;
                    self->state                                              = Player_State_Static;
                    break;

                case ID_RAY:
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_RAYSPIN, &EncoreGoodEnd->decorations[playerID + E_END_SONIC]->animator,
                                            true, 0);
                    RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL, &EncoreGoodEnd->decorations[E_END_STOOLR]->animator, true, 0);
                    self->state = Player_State_Static;
                    break;
            }
        }
    }
}

void EncoreGoodEnd_StatePlayer_EndingSonic(void)
{
    RSDK_THIS(Player);

    EntityDecoration *playerDecor = EncoreGoodEnd->decorations[E_END_SONIC];

    switch (self->abilityValues[0]) {
        case 0:
            if (EncoreGoodEnd->decorations[E_END_SONIC]->animator.frameID == 8) {
                self->abilityValues[0]                                      = 1;
                EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->visible    = true;
                EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->velocity.y = -0x40000;
                self->abilityValues[1]                                      = 0;
            }
            break;

        case 1:
            if (++self->abilityValues[1] >= 15) {
                self->abilityValues[1]                                      = 0;
                self->abilityValues[0]                                      = 2;
                EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->visible    = true;
                EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->velocity.y = -0x40000;
            }
            break;

        case 2:
            if (++self->abilityValues[1] >= 15) {
                self->abilityValues[1]                                       = 0;
                self->abilityValues[0]                                       = 3;
                EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->visible    = true;
                EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->velocity.y = -0x40000;
            }
            break;

        case 3:
            if (++self->abilityValues[1] >= 15) {
                self->abilityValues[1]                                       = 0;
                self->abilityValues[0]                                       = 4;
                EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->visible    = true;
                EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->velocity.y = -0x40000;
            }
            break;

        case 4:
            if (EncoreGoodEnd->decorations[E_END_SONIC]->animator.frameID >= 22)
                self->state = Player_State_Static;
            break;

        default: break;
    }

    int32 anim = playerDecor->animator.frameID;
    if (anim == ANI_RUN || anim == ANI_SKID || anim == ANI_DROPDASH)
        playerDecor->animator.speed = 0;

    // Sonic's Ice creams
    if (EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->visible) {
        if (EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->velocity.y < 0) {
            self->direction = FLIP_Y;
        }
        else {
            EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->drawGroup = Zone->playerDrawGroup[1];
            self->direction                                            = FLIP_NONE;
        }

        EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->position.y += EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->velocity.y;
        EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->velocity.y += 0x3800;

        if (EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->position.y >= playerDecor->position.y) {
            EncoreGoodEnd->decorations[E_END_ICECREAM_CONE]->visible = false;

            ++playerDecor->animator.frameID;
            playerDecor->animator.speed = 1;
        }
    }

    if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->visible) {
        if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->velocity.y < 0) {
            self->direction = FLIP_Y;
        }
        else {
            EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->drawGroup = Zone->playerDrawGroup[1];
            self->direction                                            = FLIP_NONE;
        }

        EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->position.y += EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->velocity.y;
        EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->velocity.y += 0x3800;

        if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->position.y >= playerDecor->position.y) {
            EncoreGoodEnd->decorations[E_END_ICECREAM_PINK]->visible = false;

            ++playerDecor->animator.frameID;
            playerDecor->animator.speed = 1;
        }
    }

    if (EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->visible) {
        if (EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->velocity.y < 0) {
            self->direction = FLIP_Y;
        }
        else {
            EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->drawGroup = Zone->playerDrawGroup[1];
            self->direction                                             = FLIP_NONE;
        }

        EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->position.y += EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->velocity.y;
        EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->velocity.y += 0x3800;

        if (EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->position.y >= playerDecor->position.y - 0x40000) {
            EncoreGoodEnd->decorations[E_END_ICECREAM_GREEN]->visible = false;

            ++playerDecor->animator.frameID;
            playerDecor->animator.speed = 1;
        }
    }

    if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->visible == 1) {
        if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->velocity.y < 0) {
            self->direction = FLIP_Y;
        }
        else {
            EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->drawGroup = Zone->playerDrawGroup[1];
            self->direction                                             = FLIP_NONE;
        }

        EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->position.y += EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->velocity.y;
        EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->velocity.y += 0x3800;

        if (EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->position.y >= playerDecor->position.y - 0x80000) {
            EncoreGoodEnd->decorations[E_END_ICECREAM_PINK2]->visible = false;

            ++playerDecor->animator.frameID;
            playerDecor->animator.speed = 1;
        }
    }
}

bool32 EncoreGoodEnd_Cutscene_MoveToPlace(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreGoodEnd);
    MANIA_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        player1->state      = Player_State_Static;
        player1->stateInput = StateMachine_None;

        CutsceneSeq_LockAllPlayerControl();
        EncoreGoodEnd_SetupPlayer(player1->characterID);
        EncoreGoodEnd_SetupPlayer(player2->characterID);

        camera->target   = (Entity *)EncoreGoodEnd->decorations[E_END_SONIC];
        self->position.y = EncoreGoodEnd->decorations[E_END_KING]->position.y;

        EncoreGoodEnd->decorations[E_END_KING]->position.y += 0x500000;
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_SONICLAYDOWN, &EncoreGoodEnd->decorations[E_END_SONIC]->animator, true, 21);

        foreach_all(UICreditsText, label) { label->active = ACTIVE_NEVER; }
    }
    else if (host->timer == 120) {
        player1->position.x = 0x300000;
        player1->velocity.x = 0x30000;
        player1->groundVel  = 0x30000;
        player1->drawGroup  = Zone->playerDrawGroup[1];
        player1->state      = Player_State_Ground;

        if (player2->classID == Player->classID) {
            player2->position.x = -0x800000;
            player2->velocity.x = 0x30000;
            player2->groundVel  = 0x30000;
            player2->drawGroup  = Zone->playerDrawGroup[1];
            player2->state      = Player_State_Ground;
            player2->stateInput = StateMachine_None;
        }
    }
    else if (host->timer > 120 && player1->groundVel <= 0x18000) {
        player1->state = EncoreGoodEnd_StatePlayer_MoveToPos;
        if (player2->classID == Player->classID)
            player2->state = EncoreGoodEnd_StatePlayer_MoveToPos;

        return true;
    }
    return false;
}

bool32 EncoreGoodEnd_Cutscene_WaitForMovementFinish(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    return player1->state == Player_State_Static && (player2->state == Player_State_Static || player2->classID != Player->classID);
}

bool32 EncoreGoodEnd_Cutscene_ClinkGlasses(EntityCutsceneSeq *host)
{
    if (host->timer == 60) {
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXCLINK, &EncoreGoodEnd->decorations[E_END_KNUX]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXCLINKG, &EncoreGoodEnd->decorations[E_END_GLASSK]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYCLINK, &EncoreGoodEnd->decorations[E_END_MIGHTY]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYCLINKG, &EncoreGoodEnd->decorations[E_END_GLASSM]->animator, true, 0);
    }
    else if (host->timer == 120) {
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXBREATHE, &EncoreGoodEnd->decorations[E_END_KNUX]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXSITDOWNG, &EncoreGoodEnd->decorations[E_END_GLASSK]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYBREATHE, &EncoreGoodEnd->decorations[E_END_MIGHTY]->animator, true, 0);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYSITDOWNG, &EncoreGoodEnd->decorations[E_END_GLASSM]->animator, true, 0);
    }
    else if (host->timer == 180) {
        EncoreGoodEnd->decorations[E_END_KING]->velocity.y = -0x60000;
        return true;
    }

    return false;
}

bool32 EncoreGoodEnd_Cutscene_KingAppear(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreGoodEnd);

    EntityDecoration *decor = EncoreGoodEnd->decorations[E_END_KING];
    decor->position.y += decor->velocity.y;
    decor->velocity.y += 0x3800;

    if (decor->velocity.y > 0) {
        if (decor->position.y >= self->position.y) {
            decor->position.y = self->position.y;

            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_SONICSHOCKED, &EncoreGoodEnd->decorations[E_END_SONIC]->animator, true, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_TAILSSHOCKED, &EncoreGoodEnd->decorations[E_END_TAILS]->animator, true, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXSHOCKED, &EncoreGoodEnd->decorations[E_END_KNUX]->animator, true, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYSHOCKED, &EncoreGoodEnd->decorations[E_END_MIGHTY]->animator, true, 0);

            EncoreGoodEnd->decorations[E_END_RAY]->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_RAYSHOCKED, &EncoreGoodEnd->decorations[E_END_RAY]->animator, true, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL_STILL, &EncoreGoodEnd->decorations[E_END_STOOLT]->animator, false, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_STOOL_STILL, &EncoreGoodEnd->decorations[E_END_STOOLR]->animator, false, 0);
            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_KNUXICECREAMSHOCK, &EncoreGoodEnd->decorations[E_END_GLASSK]->animator, true,
                                    0);

            EncoreGoodEnd->decorations[E_END_GLASSK]->drawFX |= FX_SCALE;
            EncoreGoodEnd->decorations[E_END_GLASSK]->position.x += 0x80000;
            EncoreGoodEnd->decorations[E_END_GLASSK]->rotation = -32;

            RSDK.SetSpriteAnimation(Decoration->aniFrames, E_END_DECOR_MIGHTYICECREAMSHOCK, &EncoreGoodEnd->decorations[E_END_GLASSM]->animator, true,
                                    0);
            EncoreGoodEnd->decorations[E_END_GLASSM]->drawFX |= FX_SCALE;
            EncoreGoodEnd->decorations[E_END_GLASSM]->position.x -= 0x80000;
            EncoreGoodEnd->decorations[E_END_GLASSM]->rotation = 32;

            foreach_all(UICreditsText, label)
            {
                label->active    = ACTIVE_NORMAL;
                label->state     = UICreditsText_State_SetupCharPos;
                label->drawGroup = Zone->hudDrawGroup;
            }

            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
            fxFade->speedIn      = 256;
            fxFade->speedOut     = 64;
            fxFade->drawGroup    = Zone->objectDrawGroup[1];
            RSDK.PlaySfx(EncoreGoodEnd->sfxSnap, false, 255);
            return true;
        }
    }

    return false;
}

bool32 EncoreGoodEnd_Cutscene_ThanksForPlaying(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreGoodEnd);

    if (host->timer < 22) {
        foreach_active(Decoration, decor)
        {
            if (decor->type >= 25) {
                decor->drawGroup = Zone->objectDrawGroup[1] + 1;

                if (decor->position.y >= self->position.y)
                    decor->position.y -= 0x20000;
                else
                    decor->position.y += 0x20000;
            }
        }

        RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
        RSDK.SetActivePalette(1, 2 * host->timer, ScreenInfo->size.y - 2 * host->timer);
    }

    return host->timer == 240;
}

bool32 EncoreGoodEnd_Cutscene_FinishCutscene(EntityCutsceneSeq *host)
{
    if (API.CheckDLC(DLC_PLUS))
        RSDK.SetScene("Presentation", "Encore Summary");
    else
        RSDK.SetScene("Presentation", "Menu");

    Zone_StartFadeOut(10, 0x000000);
    Music_FadeOut(0.025);

    return true;
}

void EncoreGoodEnd_Cutscene_SkipCB(void) { EncoreGoodEnd_Cutscene_FinishCutscene(NULL); }

#if GAME_INCLUDE_EDITOR
void EncoreGoodEnd_EditorDraw(void) {}

void EncoreGoodEnd_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Stool", E_END_DECOR_STOOL);
    RSDK_ENUM_VAR("Ice Cream (Pink)", E_END_DECOR_ICECREAM_PINK);
    RSDK_ENUM_VAR("Ice Cream (Green)", E_END_DECOR_ICECREAM_GREEN);
    RSDK_ENUM_VAR("Ice Cream (Cone)", E_END_DECOR_ICECREAM_CONE);
    RSDK_ENUM_VAR("King", E_END_DECOR_KING);
    RSDK_ENUM_VAR("Ray Spin", E_END_DECOR_RAYSPIN);
    RSDK_ENUM_VAR("Ray Shocked", E_END_DECOR_RAYSHOCKED);
    RSDK_ENUM_VAR("Sonic Lay Down", E_END_DECOR_SONICLAYDOWN);
    RSDK_ENUM_VAR("Sonic Shocked", E_END_DECOR_SONICSHOCKED);
    RSDK_ENUM_VAR("Knux Breathe", E_END_DECOR_KNUXBREATHE);
    RSDK_ENUM_VAR("Knux Clink", E_END_DECOR_KNUXCLINK);
    RSDK_ENUM_VAR("Knux Clink (Glass)", E_END_DECOR_KNUXCLINKG);
    RSDK_ENUM_VAR("Knux Shocked", E_END_DECOR_KNUXSHOCKED);
    RSDK_ENUM_VAR("Knux Sit Down", E_END_DECOR_KNUXSITDOWN);
    RSDK_ENUM_VAR("Knux Sit Down (Glass)", E_END_DECOR_KNUXSITDOWNG);
    RSDK_ENUM_VAR("Mighty Breathe", E_END_DECOR_MIGHTYBREATHE);
    RSDK_ENUM_VAR("Mighty Clink", E_END_DECOR_MIGHTYCLINK);
    RSDK_ENUM_VAR("Mighty Clink (Glass)", E_END_DECOR_MIGHTYCLINKG);
    RSDK_ENUM_VAR("Mighty Sit Down", E_END_DECOR_MIGHTYSITDOWN);
    RSDK_ENUM_VAR("Mighty Sit Down (Glass)", E_END_DECOR_MIGHTYSITDOWNG);
    RSDK_ENUM_VAR("Mighty Shocked", E_END_DECOR_MIGHTYSHOCKED);
    RSDK_ENUM_VAR("Tails Shocked", E_END_DECOR_TAILSSHOCKED);
    RSDK_ENUM_VAR("Tails Spin", E_END_DECOR_TAILSSPIN);
    RSDK_ENUM_VAR("Stool (Still)", E_END_DECOR_STOOL_STILL);
    RSDK_ENUM_VAR("Knux Cream Shock", E_END_DECOR_KNUXICECREAMSHOCK);
    RSDK_ENUM_VAR("Mighty Cream Shock", E_END_DECOR_MIGHTYICECREAMSHOCK);
    RSDK_ENUM_VAR("Letterbox 1", E_END_DECOR_LETTERBOX1);
    RSDK_ENUM_VAR("Letterbox 2", E_END_DECOR_LETTERBOX2);
}
#endif

void EncoreGoodEnd_Serialize(void) { RSDK_EDITABLE_VAR(EncoreGoodEnd, VAR_VECTOR2, size); }
#endif

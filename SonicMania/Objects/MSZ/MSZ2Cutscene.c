// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSZ2Cutscene Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSZ2Cutscene *MSZ2Cutscene;

void MSZ2Cutscene_Update(void)
{
    RSDK_THIS(MSZ2Cutscene);

    if (!self->activated) {
        MSZ2Cutscene_SetupCutscene();

        self->activated = true;
    }
}

void MSZ2Cutscene_LateUpdate(void) {}

void MSZ2Cutscene_StaticUpdate(void) {}

void MSZ2Cutscene_Draw(void) {}

void MSZ2Cutscene_Create(void *data)
{
    RSDK_THIS(MSZ2Cutscene);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NEVER;
}

void MSZ2Cutscene_StageLoad(void)
{
    foreach_all(ParallaxSprite, sprite)
    {
        if (sprite->aniID == MSZ_PARALLAXSPRITE_OOZPEEK) {
            MSZ2Cutscene->oozPeek = sprite;
            foreach_break;
        }
    }

    foreach_all(EggPrison, prison)
    {
        MSZ2Cutscene->prison = prison;
        foreach_break;
    }
}

void MSZ2Cutscene_SetupCutscene(void)
{
    RSDK_THIS(MSZ2Cutscene);

    CutsceneSeq_StartSequence(self, MSZ2Cutscene_Cutscene_GoToPistol, MSZ2Cutscene_Cutscene_EnterPistol, MSZ2Cutscene_Cutscene_PistolFired,
                              MSZ2Cutscene_Cutscene_AppearInBG, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
#endif

    HUD_MoveOut();
}

void MSZ2Cutscene_GetPistolPtr(void)
{
    RSDK_THIS(MSZ2Cutscene);

    foreach_all(GiantPistol, pistol)
    {
        if (MathHelpers_PointInHitbox(self->position.x, self->position.y, pistol->position.x, pistol->position.y, self->direction, &self->hitbox)) {
            MSZ2Cutscene->pistol = pistol;
            foreach_break;
        }
    }
}

bool32 MSZ2Cutscene_Cutscene_GoToPistol(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityParallaxSprite *parallaxSprite = MSZ2Cutscene->oozPeek;
    EntityEggPrison *prison              = MSZ2Cutscene->prison;

    if (!host->timer) {
        MSZ2Cutscene_GetPistolPtr();
        parallaxSprite->visible = true;
        parallaxSprite->drawFX  = FX_SCALE;
        parallaxSprite->scale.x = 0x200;
        parallaxSprite->scale.y = 0x200;
        prison->notSolid        = true;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
        Zone->cameraBoundsT[0] = 0;
        Zone->cameraBoundsT[1] = 0;
        Zone->cameraBoundsR[0] = size.x;
        Zone->cameraBoundsR[1] = size.x;

        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->state     = Player_State_Ground;
        player1->groundVel = 0;

        if (player2->classID == Player->classID) {
#if GAME_VERSION != VER_100
            Player->disableP2KeyCheck = true;
#endif
            player2->state      = Player_State_Ground;
            player2->stateInput = Player_Input_P2_AI;
            player2->groundVel  = 0;
        }
    }

    EntityGiantPistol *pistol = MSZ2Cutscene->pistol;

    player1->right = player1->position.x < pistol->position.x - 0xD00000;
    if (player1->position.x < pistol->position.x - 0xD00000) {
        if (player2 && player2->jumpPress) {
            player2->velocity.x = 0x44000;
            player2->velocity.y = -0x80000;
        }
    }
    else {
        player1->position.x = pistol->position.x - 0xD00000;
        Player_Action_Jump(player1);
        player1->velocity.x = 0x44000;
        player1->velocity.y = -0x80000;
        player1->jumpPress  = true;
        player1->jumpHold   = true;
        return true;
    }

    return false;
}

bool32 MSZ2Cutscene_Cutscene_EnterPistol(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    EntityGiantPistol *pistol = MSZ2Cutscene->pistol;

    if (player2->classID == Player->classID) {
        if (player2->state == Player_State_Air && player2->animator.animationID == ANI_JUMP)
            player2->position.x += (player1->position.x - player2->position.x) >> 4;
        player2->position.y += (player1->position.y - player2->position.y) >> 4;
    }

    if (!host->timer)
        player1->jumpPress = false;

    if (pistol->state == GiantPistol_State_CloseChamber) {
        player1->jumpHold = false;
        if (player2->classID == Player->classID) {
            player2->position.x      = player1->position.x;
            player2->position.y      = player1->position.y;
            player2->stateInput      = StateMachine_None;
            player2->state           = Player_State_Static;
            player2->nextAirState    = StateMachine_None;
            player2->nextGroundState = StateMachine_None;
            player2->velocity.x      = 0;
            player2->velocity.y      = 0;
            pistol->activePlayers |= 1 << player2->playerID;
        }
    }

    if (pistol->rotation == 448) {
        player1->jumpPress = true;
        player1->camera    = NULL;
        camera->target     = NULL;
        return true;
    }

    return false;
}

bool32 MSZ2Cutscene_Cutscene_PistolFired(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    Entity *curEntity = host->activeEntity;
    if (!host->timer) {
        player1->jumpPress = false;
        player1->jumpHold  = false;
    }

    if (player1->position.y < (curEntity->position.y - (ScreenInfo->center.y << 16) - 0x100000)) {
        player1->state      = Player_State_Static;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->position.y = 0;
    }

    if (player2->classID == Player->classID && player2->position.y < (curEntity->position.y - (ScreenInfo->center.y << 16) - 0x100000)) {
        player2->state      = Player_State_Static;
        player2->velocity.x = 0;
        player2->velocity.y = 0;
        player2->position.y = 0;
    }

    if (host->timer == 30) {
        host->storedValue = camera->position.x + 0x1000000;
        host->storedTimer = camera->position.y - 0x4000000;
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, host->storedValue, host->storedTimer, 2);
    }

    if (host->timer > 30 && camera->position.x == host->storedValue && camera->position.y == host->storedTimer) {
        EntityShield *shield = RSDK_GET_ENTITY(player1->playerID + Player->playerCount, Shield);
        if (shield->classID == Shield->classID) {
            player1->shield = SHIELD_NONE;
            destroyEntity(shield);
        }
        return true;
    }

    return false;
}

bool32 MSZ2Cutscene_Cutscene_AppearInBG(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        host->storedTimer = 0x4000;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_JUMP, &player1->animator, false, 0);
        player1->drawFX |= FX_SCALE;
        player1->animator.speed = 60;
        player1->scale.x        = 0x40;
        player1->scale.y        = 0x40;
        player1->state          = Player_State_Static;
        player1->position.x     = camera->position.x;
        player1->position.y     = camera->position.y - 0x700000;

        if (player2->classID == Player->classID) {
            player2->drawFX |= FX_SCALE;
            player2->scale.x    = 0x40;
            player2->scale.y    = 0x40;
            player2->state      = Player_State_Static;
            player2->position.x = player1->position.x - 0x63000;
            player2->position.y = player1->position.y - 0x24000;
            player2->velocity.x = 0;
            player2->velocity.y = 0;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_JUMP, &player2->animator, false, 0);
            player1->animator.speed = 60;
        }
    }

    if (host->timer > 0) {
        player1->velocity.x = 0xB000;
        player1->velocity.y = host->storedTimer;

        if (player2->classID == Player->classID) {
            player2->state      = Player_State_Static;
            player2->velocity.x = 0xB000;
            player2->velocity.y = player1->velocity.y;
        }

        host->storedTimer += 144;
    }

    if (host->timer == 120)
        Zone_StartFadeOut(10, 0x000000);

    return false;
}

#if GAME_INCLUDE_EDITOR
void MSZ2Cutscene_EditorDraw(void)
{
    RSDK_THIS(MSZ2Cutscene);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void MSZ2Cutscene_EditorLoad(void) {}
#endif

void MSZ2Cutscene_Serialize(void) { RSDK_EDITABLE_VAR(MSZ2Cutscene, VAR_VECTOR2, size); }

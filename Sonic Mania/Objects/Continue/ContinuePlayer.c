#include "SonicMania.h"

ObjectContinuePlayer *ContinuePlayer;

void ContinuePlayer_Update(void)
{
    RSDK_THIS(ContinuePlayer);
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void ContinuePlayer_LateUpdate(void) {}

void ContinuePlayer_StaticUpdate(void) {}

void ContinuePlayer_Draw(void)
{
    RSDK_THIS(ContinuePlayer);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ContinuePlayer_Create(void *data)
{
    RSDK_THIS(ContinuePlayer);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible   = true;
        entity->drawOrder = 1;
        entity->active    = ACTIVE_NORMAL;
        if (!entity->isPlayer2) {
            switch (globals->playerID & 0xFF) {
                case ID_TAILS:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_TAILS, &entity->animator, true, 0);
                    entity->position.y += 0x40000;
                    break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_KNUX, &entity->animator, true, 0); break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_MIGHTY, &entity->animator, true, 0); break;
                case ID_RAY:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_RAY, &entity->animator, true, 0);
                    entity->position.y += 0x40000;
                    break;
#endif
                default:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_SONIC, &entity->animator, true, 0);
                    if (globals->playerID & ID_TAILS_ASSIST)
                        entity->position.x -= 0x100000;
                    break;
            }
            entity->timer     = 60;
            entity->aniFrames = ContinuePlayer->playerAniFrames;
        }
        else {
            RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_AI, &entity->animator, true, 0);
            if (!(globals->playerID & ID_TAILS_ASSIST))
                entity->active = ACTIVE_NEVER;
            entity->timer     = 76;
            entity->aniFrames = ContinuePlayer->tailAniFrames;
        }
    }
}

void ContinuePlayer_StageLoad(void)
{
    ContinuePlayer->aniFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE);
    if (globals->playerID == ID_NONE) 
        globals->playerID = ID_DEFAULT_PLAYER;

    switch (globals->playerID & 0xFF) {
        case ID_TAILS: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE); break;
        case ID_KNUCKLES: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
        case ID_RAY: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
        default: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE); break;
    }
    ContinuePlayer->tailAniFrames = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
    ContinuePlayer->sfxRoll       = RSDK.GetSFX("Global/Roll.wav");
    ContinuePlayer->sfxCharge     = RSDK.GetSFX("Global/Charge.wav");
    ContinuePlayer->sfxRelease    = RSDK.GetSFX("Global/Release.wav");
}

void ContinuePlayer_Unknown1(void)
{
    RSDK_THIS(ContinuePlayer);

    if (entity->groundVel >= 0x40000) {
        if (entity->groundVel >= 0x60000) {
            if (entity->groundVel >= 0xA0000) {
                RSDK.SetSpriteAnimation(entity->aniFrames, ANI_DASH, &entity->animator, false, 0);
                entity->animator.animationSpeed = 256;
            }
            else {
                RSDK.SetSpriteAnimation(entity->aniFrames, ANI_RUN, &entity->animator, false, 1);
                entity->animator.animationSpeed = (entity->groundVel >> 12) + 96;
                if (entity->animator.animationSpeed > 0x100)
                    entity->animator.animationSpeed = 0x100;
            }
        }
        else {
            RSDK.SetSpriteAnimation(entity->aniFrames, ANI_JOG, &entity->animator, false, 0);
            int32 iVel                        = (entity->groundVel >> 16);
            entity->animator.animationSpeed = 4 * ((iVel << 1) + iVel) + 64;
        }
    }
    else {
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_WALK, &entity->animator, false, 0);
        entity->animator.animationSpeed = (entity->groundVel >> 12) + 48;
    }
}

void ContinuePlayer_Unknown2(void)
{
    RSDK_THIS(ContinuePlayer);

    if (entity->timer <= 0) {
        ContinuePlayer_Unknown1();
        entity->state = ContinuePlayer_Unknown3;
        RSDK.PlaySfx(ContinuePlayer->sfxRoll, 0, 255);
    }
    else {
        entity->timer--;
    }
}

void ContinuePlayer_Unknown3(void)
{
    RSDK_THIS(ContinuePlayer);

    if (entity->groundVel >= 0xC0000) {
        RSDK.PlaySfx(ContinuePlayer->sfxRelease, 0, 255);
        entity->state = ContinuePlayer_Unknown4;
    }
    else {
        entity->groundVel += 0x4000;
    }
    ContinuePlayer_Unknown1();
}

void ContinuePlayer_Unknown4(void)
{
    RSDK_THIS(ContinuePlayer);
    entity->position.x += entity->groundVel;
}

#if RETRO_INCLUDE_EDITOR
void ContinuePlayer_EditorDraw(void) {}

void ContinuePlayer_EditorLoad(void) {}
#endif

void ContinuePlayer_Serialize(void) { RSDK_EDITABLE_VAR(ContinuePlayer, VAR_BOOL, isPlayer2); }

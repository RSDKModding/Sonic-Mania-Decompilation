#include "SonicMania.h"

ObjectHatterkiller *Hatterkiller;

void Hatterkiller_Update(void)
{
    RSDK_THIS(Hatterkiller);

    for (int s = 0; s < Hatterkiller_SegmentCount; ++s) {
        if (entity->delays[s] <= 0) {
            entity->velocities[s].y += 0x3800;
            entity->positions[s].x += entity->velocities[s].x + TornadoPath->field_8.x;
            entity->positions[s].y += entity->velocities[s].y + TornadoPath->field_8.y;
        }
        else {
            entity->delays[s]--;
            entity->positions[s].x += TornadoPath->field_8.x;
            entity->positions[s].y += TornadoPath->field_8.y;
        }
    }

    if (HeavyMystic->curtainLinePos) {
        if (!(Zone->timer & 3))
            HeavyMystic_Unknown1(entity->position.x, entity->position.y);
        if (++entity->field_58 == 40) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            RSDK.PlaySfx(Hatterkiller->sfxTransform2, false, 255);
            RSDK.PlaySfx(Hatterkiller->sfxPowerup, false, 255);
            CREATE_ENTITY(FXSpinRay, NULL, entity->position.x, entity->position.y)->parent = (Entity *)entity;
            EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
            fade->speedIn      = 8;
            fade->speedOut     = 8;
        }
    }
    else {

        foreach_active(Player, player)
        {
            entity->position.x = entity->positions[0].x;
            entity->position.y = entity->positions[0].y;
            if (Player_CheckBadnikTouch(player, entity, &Hatterkiller->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
                for (int s = 1; s < Hatterkiller_SegmentCount; ++s) {
                    Vector2 *pos       = &entity->positions[s];
                    Animator *animator = entity->animators[s];

                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, pos->x, pos->y);
                    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, animator->animationID, &debris->animator, true, animator->frameID);
                    debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                    debris->velocity.y    = RSDK.Rand(-0x20000, -0x10000);
                    debris->gravity       = 0x4800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                }

                destroyEntity(entity);
                entity->active = ACTIVE_NEVER2;
                foreach_break;
            }

            for (int s = 1; s < Hatterkiller_SegmentCount; ++s) {
                entity->position.x = entity->positions[s].x;
                entity->position.y = entity->positions[s].y;

                if (Player_CheckCollisionTouch(player, entity, &Hatterkiller->hitbox)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, entity);
                }
            }
        }
    }

    if (HeavyMystic->curtainLinePos) {
        entity->position = entity->positions[9];
        if (!RSDK.CheckOnScreen(entity, NULL)) {
            if (HeavyMystic->curtainLinePos == 1 && !RSDK.CheckOnScreen(entity, NULL)) {
                foreach_active(ParallaxSprite, sprite) { sprite->visible = false; }
                ++HeavyMystic->curtainLinePos;
            }
            CREATE_ENTITY(UberCaterkiller, NULL, entity->positions[Hatterkiller_SegmentCount - 1].x,
                          (RSDK_screens->position.y + 64 + RSDK_screens->height) << 16);
            destroyEntity(entity);
        }
    }
    else if (!RSDK.CheckOnScreen(entity, NULL)) {
        destroyEntity(entity);
    }

    entity->position.x = entity->positions[0].x;
    entity->position.y = entity->positions[0].y;
}

void Hatterkiller_LateUpdate(void) {}

void Hatterkiller_StaticUpdate(void) {}

void Hatterkiller_Draw(void)
{
    RSDK_THIS(Hatterkiller);

    for (int s = Hatterkiller_SegmentCount - 1; s >= 0; --s) {
        RSDK.DrawSprite(entity->animators[s], &entity->positions[s], false);
    }
}

void Hatterkiller_Create(void *data)
{
    RSDK_THIS(Hatterkiller);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->active        = ACTIVE_NORMAL;
        entity->drawOrder     = Zone->drawOrderLow + 1;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;

        int delay = 0;
        for (int s = 0; s < Hatterkiller_SegmentCount; ++s) {
            entity->velocities[s].x = voidToInt(data);
            entity->velocities[s].y = -0x40000;
            entity->animators[s]    = &entity->animator2;
            entity->delays[s]       = delay;
            entity->positions[s].x  = entity->position.x;
            entity->positions[s].y  = entity->position.y;
            delay += 4;
        }
        entity->animators[0] = &entity->animator1;

        if (voidToInt(data) >= 0)
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 3, &entity->animator1, true, 0);
        else
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 2, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 4, &entity->animator3, true, 0);
    }
}

void Hatterkiller_StageLoad(void)
{
    Hatterkiller->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/RattleKiller.bin", SCOPE_STAGE);
    Hatterkiller->hitbox.left   = -8;
    Hatterkiller->hitbox.top    = -8;
    Hatterkiller->hitbox.right  = 8;
    Hatterkiller->hitbox.bottom = 8;
    Hatterkiller->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    Hatterkiller->sfxTransform2 = RSDK.GetSFX("Stage/Transform2.wav");
    Hatterkiller->sfxPowerup    = RSDK.GetSFX("Stage/PowerUp.wav");
    DEBUGMODE_ADD_OBJ(Hatterkiller);
}

void Hatterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Hatterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Hatterkiller, intToVoid(-0x20000), entity->position.x, entity->position.y);
}

#if RETRO_INCLUDE_EDITOR
void Hatterkiller_EditorDraw(void) {}

void Hatterkiller_EditorLoad(void) {}
#endif

void Hatterkiller_Serialize(void) {}

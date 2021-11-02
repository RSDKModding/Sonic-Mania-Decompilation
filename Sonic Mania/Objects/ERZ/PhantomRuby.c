#include "SonicMania.h"

ObjectPhantomRuby *PhantomRuby;

void PhantomRuby_Update(void)
{
    RSDK_THIS(PhantomRuby);
    StateMachine_Run(entity->state);
    if (entity->animator1.animationID == 1 && entity->animator1.frameID == entity->animator1.frameCount - 1)
        RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &entity->animator1, true, 0);
    if (entity->animator2.animationID == 2 && entity->animator2.frameID == entity->animator2.frameCount - 1)
        RSDK.SetSpriteAnimation(0xFFFF, 0xFFFF, &entity->animator2, true, 0);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void PhantomRuby_LateUpdate(void) {}

void PhantomRuby_StaticUpdate(void) {}

void PhantomRuby_Draw(void)
{
    RSDK_THIS(PhantomRuby);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->animator2.animationID != -1) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 0xFF;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->inkEffect = INK_NONE;
    }
}

void PhantomRuby_Create(void *data)
{
    RSDK_THIS(PhantomRuby);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = PhantomRuby_Unknown3;
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &entity->animator1, true, 0);
}

void PhantomRuby_StageLoad(void)
{
    PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE);
    PhantomRuby->sfx_L[0]  = RSDK.GetSFX("Ruby/Attack1_L.wav");
    PhantomRuby->sfx_R[0]  = RSDK.GetSFX("Ruby/Attack1_R.wav");
    PhantomRuby->sfx_L[1]  = RSDK.GetSFX("Ruby/Attack2_L.wav");
    PhantomRuby->sfx_R[1]  = RSDK.GetSFX("Ruby/Attack2_R.wav");
    PhantomRuby->sfx_L[2]  = RSDK.GetSFX("Ruby/Attack3_L.wav");
    PhantomRuby->sfx_R[2]  = RSDK.GetSFX("Ruby/Attack3_R.wav");
    PhantomRuby->sfx_L[3]  = RSDK.GetSFX("Ruby/Attack4_L.wav");
    PhantomRuby->sfx_R[3]  = RSDK.GetSFX("Ruby/Attack4_R.wav");
    PhantomRuby->sfx_L[4]  = RSDK.GetSFX("Ruby/Attack5_L.wav");
    PhantomRuby->sfx_R[4]  = RSDK.GetSFX("Ruby/Attack5_R.wav");
    PhantomRuby->sfx_L[5]  = RSDK.GetSFX("Ruby/Attack6_L.wav");
    PhantomRuby->sfx_R[5]  = RSDK.GetSFX("Ruby/Attack6_R.wav");
    PhantomRuby->sfx_L[6]  = RSDK.GetSFX("Ruby/RedCube_L.wav");
    PhantomRuby->sfx_R[6]  = RSDK.GetSFX("Ruby/RedCube_R.wav");
}

void PhantomRuby_PlaySFX(uint8 sfxID)
{
    if (sfxID) {
        uint8 sfx    = sfxID - 1;
        int32 channel = RSDK.PlaySfx(PhantomRuby->sfx_L[sfx], 0, 0);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        channel = RSDK.PlaySfx(PhantomRuby->sfx_R[sfx], 0, 0);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
    }
}

void PhantomRuby_Unknown2(EntityPhantomRuby *ruby)
{
    ruby->flag     = false;
    ruby->field_6C = 0;
    ruby->timer    = 0;
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 1, &ruby->animator1, true, 0);
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 2, &ruby->animator2, true, 0);
    ruby->state = PhantomRuby_Unknown4;
}

void PhantomRuby_Unknown3(void)
{
    RSDK_THIS(PhantomRuby);
    if (entity->flag)
        entity->flag = false;
}

void PhantomRuby_Unknown4(void)
{
    RSDK_THIS(PhantomRuby);

    if (entity->timer == 38) {
        PhantomRuby_PlaySFX(entity->sfx);
        entity->flag     = true;
        entity->field_6C = 1;
        entity->timer    = 0;
        entity->state    = PhantomRuby_Unknown3;
    }
    else {
        entity->timer++;
    }
}

void PhantomRuby_Unknown5(void)
{
    RSDK_THIS(PhantomRuby);
    entity->angle += 2;
    entity->position.y = (RSDK.Sin256(entity->angle) << 10) + entity->startPos.y;
}

void PhantomRuby_Unknown6(void)
{
    RSDK_THIS(PhantomRuby);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->position.y > (Zone->screenBoundsB1[0] + 64) << 16)
        entity->state = StateMachine_None;
}

void PhantomRuby_Unknown7(void)
{
    RSDK_THIS(PhantomRuby);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->rotation = (entity->rotation + 5) & 0x1FF;
}

void PhantomRuby_Unknown8(void)
{
    RSDK_THIS(PhantomRuby);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->rotation = (entity->rotation + 5) & 0x1FF;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        entity->velocity.x >>= 1;
        if (-(entity->velocity.y >> 1) > -0x10000)
            entity->velocity.y = 0;
        else
            entity->velocity.y = -(entity->velocity.y >> 1);
    }
}

void PhantomRuby_Unknown9(void)
{
    RSDK_THIS(PhantomRuby);
    int32 rx    = (entity->startPos.x - entity->position.x) >> 16;
    int32 ry    = (entity->startPos.y - entity->position.y) >> 16;
    int32 angle = RSDK.ATan2(rx, ry);
    entity->velocity.x += RSDK.Cos256(angle) << 3;
    entity->velocity.y += RSDK.Sin256(angle) << 3;
    int32 r = rx * rx + ry * ry;
    if (r >= 16) {
        if (r < 2304) {
            entity->velocity.x = (entity->startPos.x - entity->position.x) >> 4;
            entity->velocity.y = (entity->startPos.y - entity->position.y) >> 4;
        }
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }
    else {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->state      = PhantomRuby_Unknown5;
    }
}

void PhantomRuby_Unknown10(void)
{
    RSDK_THIS(PhantomRuby);
    entity->rotation += 6;
    if (entity->rotation > 0x200) {
        entity->rotation = 0;
        entity->drawFX   = FX_NONE;
        entity->state    = StateMachine_None;
    }
}

void PhantomRuby_EditorDraw(void) { PhantomRuby_Draw(); }

void PhantomRuby_EditorLoad(void) { PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE); }

void PhantomRuby_Serialize(void) { RSDK_EDITABLE_VAR(PhantomRuby, VAR_UINT8, sfx); }

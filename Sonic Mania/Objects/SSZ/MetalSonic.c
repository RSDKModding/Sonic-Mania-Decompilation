#include "SonicMania.h"

ObjectMetalSonic *MetalSonic;

void MetalSonic_Update(void)
{
    RSDK_THIS(MetalSonic);
    if (entity->field_94)
        entity->field_94--;
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator2);
    StateMachine_Run(entity->state);
    if (MetalSonic->field_8 > 0)
        MetalSonic->field_8--;
    foreach_active(StarPost, post) { post->starTimer = 0; }
}

void MetalSonic_LateUpdate(void)
{

}

void MetalSonic_StaticUpdate(void)
{

}

void MetalSonic_Draw(void)
{
    RSDK_THIS(MetalSonic);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->position.x < 0x8000000)
        drawPos.x += 0xE000000;
    else
        drawPos.x -= 0xE000000;

    if (entity->field_94 & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 8);
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator, NULL, false);
        RSDK.CopyPalette(1, 240, 0, 240, 8);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void MetalSonic_Create(void* data)
{
    RSDK_THIS(MetalSonic);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            Zone->field_154          = 0;
            entity->active          = ACTIVE_BOUNDS;
            entity->drawFX          = FX_FLIP | FX_ROTATE;
            entity->visible         = false;
            entity->updateRange.x   = 0x800000;
            entity->updateRange.y   = 0x800000;
            entity->tileCollisions  = true;
            entity->collisionLayers = Zone->fgLayers;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &entity->animator, true, 0);
            entity->drawOrder = Zone->drawOrderLow;
            //entity->state          = MetalSonic_Unknown5;
        }
    }
}

void MetalSonic_StageLoad(void)
{
    MetalSonic->aniFrames       = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);
    MetalSonic->hitbox1.left    = -8;
    MetalSonic->hitbox1.top     = -112;
    MetalSonic->hitbox1.right   = 8;
    MetalSonic->hitbox1.bottom  = 128;
    MetalSonic->hitbox2.left    = -16;
    MetalSonic->hitbox2.top     = -112;
    MetalSonic->hitbox2.right   = 16;
    MetalSonic->hitbox2.bottom  = 128;
    MetalSonic->sfxHit          = RSDK.GetSFX("Stage/BossHit.wav");
    MetalSonic->sfxExplosion2   = RSDK.GetSFX("Stage/Explosion2.wav");
    MetalSonic->sfxExplosion3   = RSDK.GetSFX("Stage/Explosion3.wav");
    MetalSonic->sfxRumble       = RSDK.GetSFX("Stage/Rumble.wav");
    MetalSonic->sfxJump2        = RSDK.GetSFX("Stage/Jump2.wav");
    MetalSonic->sfxSpecialRing  = RSDK.GetSFX("Global/SpecialRing.wav");
    MetalSonic->sfxMSElecPulse  = RSDK.GetSFX("SSZ2/MSElecPulse.wav");
    MetalSonic->sfxMSBall       = RSDK.GetSFX("SSZ2/MSBall.wav");
    MetalSonic->sfxMSFireball   = RSDK.GetSFX("SSZ2/MSFireball.wav");
    MetalSonic->sfxBeep3        = RSDK.GetSFX("Stage/Beep3.wav");
    MetalSonic->sfxBeep4        = RSDK.GetSFX("Stage/Beep4.wav");
    MetalSonic->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
    MetalSonic->sfxMSShoot      = RSDK.GetSFX("SSZ2/MSShoot.wav");
    MetalSonic->sfxMSChargeFire = RSDK.GetSFX("SSZ2/MSChargeFire.wav");
    MetalSonic->sfxMSTransform  = RSDK.GetSFX("SSZ2/MSTransform.wav");
    MetalSonic->sfxTransform2   = RSDK.GetSFX("Stage/Transform2.wav");
}

void MetalSonic_EditorDraw(void)
{

}

void MetalSonic_EditorLoad(void)
{

}

void MetalSonic_Serialize(void)
{

}


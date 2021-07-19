#include "../SonicMania.h"

ObjectHeavyMystic *HeavyMystic;

void HeavyMystic_Update(void)
{

}

void HeavyMystic_LateUpdate(void)
{

}

void HeavyMystic_StaticUpdate(void)
{

}

void HeavyMystic_Draw(void)
{

}

void HeavyMystic_Create(void* data)
{

}

void HeavyMystic_StageLoad(void)
{
    HeavyMystic->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->rouguesFrames = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
    HeavyMystic->active        = ACTIVE_ALWAYS;
    HeavyMystic->field_18      = 0;
    HeavyMystic->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    HeavyMystic->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    HeavyMystic->sfxMagicBox   = RSDK.GetSFX("MSZ/MagicBox.wav");
    HeavyMystic->sfxClack      = RSDK.GetSFX("Stage/Clack.wav");
    HeavyMystic->sfxImpact     = RSDK.GetSFX("Stage/Impact3.wav");
    HeavyMystic->sfxPowerDown  = RSDK.GetSFX("Stage/PowerDown.wav");
    HeavyMystic->sfxPon        = RSDK.GetSFX("Stage/Pon.wav");
    HeavyMystic->sfxDrop       = RSDK.GetSFX("Stage/Drop.wav");
    HeavyMystic->sfxImpact2    = RSDK.GetSFX("Stage/Impact3.wav");
    HeavyMystic->sfxAssemble   = RSDK.GetSFX("Stage/Assemble.wav");
    HeavyMystic->sfxPoof       = RSDK.GetSFX("MSZ/MysticPoof.wav");
    HeavyMystic->sfxTwinkle    = RSDK.GetSFX("MSZ/MysticTwinkle.wav");
    HeavyMystic->sfxHat        = RSDK.GetSFX("MSZ/MysticHat.wav");
    HeavyMystic->sfxHatNode    = RSDK.GetSFX("MSZ/MysticHatNode.wav");
    HeavyMystic->sfxBleeps     = RSDK.GetSFX("MSZ/MysticBleeps.wav");
    HeavyMystic->sfxAppear1    = RSDK.GetSFX("MSZ/MysticAppearAct1.wav");
    HeavyMystic->sfxTransform  = RSDK.GetSFX("MSZ/MysticTransform.wav");
}

void HeavyMystic_Unknown1(int x, int y)
{
    int velX = RSDK.Rand(-0xC000, 0xC000);
    int velY = RSDK.Rand(-0xC000, 0xC000);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, x, y);
    debris->drawOrder    = Zone->drawOrderLow;
    debris->drawFX       = FX_FLIP;
    debris->direction    = RSDK.Rand(0, 4);
    debris->velocity.x   = velX;
    debris->velocity.y   = velY;
    debris->inkEffect    = INK_SUB;
    debris->alpha        = 0xE0;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 5, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;

    debris             = CREATE_ENTITY(Debris, Debris_State_LightningSpark, x, y);
    debris->velocity.x = velX;
    debris->velocity.y = velY;
    debris->drawOrder  = Zone->drawOrderLow;
    debris->inkEffect  = INK_ADD;
    debris->alpha      = 0xFF;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 7, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;
}

void HeavyMystic_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 7)) {
        HeavyMystic_Unknown1(entity->position.x + RSDK.Rand(-0x200000, -0x100000), entity->position.y + RSDK.Rand(-0x100000, 0));
        HeavyMystic_Unknown1(entity->position.x + RSDK.Rand(0x100000, 0x200000), entity->position.y + RSDK.Rand(-0x100000, 0));
    }
}

void HeavyMystic_EditorDraw(void)
{

}

void HeavyMystic_EditorLoad(void)
{

}

void HeavyMystic_Serialize(void)
{

}


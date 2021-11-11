#include "SonicMania.h"

ObjectDrillerdroid *Drillerdroid;

void Drillerdroid_Update(void)
{

}

void Drillerdroid_LateUpdate(void)
{

}

void Drillerdroid_StaticUpdate(void)
{

}

void Drillerdroid_Draw(void)
{

}

void Drillerdroid_Create(void* data)
{

}

void Drillerdroid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    Drillerdroid->hitbox1.left   = -25;
    Drillerdroid->hitbox1.top    = -16;
    Drillerdroid->hitbox1.right  = 25;
    Drillerdroid->hitbox1.bottom = 16;

    Drillerdroid->hitbox2.left   = -58;
    Drillerdroid->hitbox2.top    = -74;
    Drillerdroid->hitbox2.right  = -25;
    Drillerdroid->hitbox2.bottom = 16;

    Drillerdroid->hitbox3.left   = 25;
    Drillerdroid->hitbox3.top    = -74;
    Drillerdroid->hitbox3.right  = 58;
    Drillerdroid->hitbox3.bottom = 16;

    Drillerdroid->hitbox4.left   = -58;
    Drillerdroid->hitbox4.top    = 16;
    Drillerdroid->hitbox4.right  = -25;
    Drillerdroid->hitbox4.bottom = 53;

    Drillerdroid->hitbox5.left   = 25;
    Drillerdroid->hitbox5.top    = 16;
    Drillerdroid->hitbox5.right  = 58;
    Drillerdroid->hitbox5.bottom = 53;

    Drillerdroid->field_44     = 4;
    Drillerdroid->field_3C     = 4;
    Drillerdroid->field_48     = 0;
    Drillerdroid->field_40     = 0;
    Drillerdroid->field_34     = 0x100000;
    Drillerdroid->field_38     = 0x100000;
    Drillerdroid->field_2C     = 0;
    Drillerdroid->field_30     = 0;
    Drillerdroid->field_54     = 0;
    Drillerdroid->field_58     = 0;
    Drillerdroid->field_4C     = 0;
    Drillerdroid->field_50     = 0;
    Drillerdroid->field_5C[0]  = 1;
    Drillerdroid->field_5C[1]  = 1;
    Drillerdroid->field_5C[2]  = 1;
    Drillerdroid->field_5C[3]  = 1;
    Drillerdroid->field_60     = 1;
    Drillerdroid->active       = ACTIVE_ALWAYS;
    Drillerdroid->field_66     = 0;
    Drillerdroid->field_68     = 0;
    Drillerdroid->field_6C     = 3;
    Drillerdroid->field_6D     = 0;
    Drillerdroid->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    Drillerdroid->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    Drillerdroid->sfxDrill     = RSDK.GetSFX("LRZ/Drill.wav");
    Drillerdroid->sfxImpact    = RSDK.GetSFX("Stage/Impact4.wav");
    Drillerdroid->sfxJump      = RSDK.GetSFX("LRZ/DrillJump.wav");
    Drillerdroid->sfxTargeting = RSDK.GetSFX("Stage/Targeting1.wav");
    Drillerdroid->sfxSizzle    = RSDK.GetSFX("LRZ/Sizzle.wav");
    Drillerdroid->sfxDrop      = RSDK.GetSFX("Stage/Drop.wav");
    Drillerdroid->sfxFail      = RSDK.GetSFX("Stage/Fail.wav");
    Drillerdroid->sfxClang     = RSDK.GetSFX("Stage/Clang2.wav");
}

#if RETRO_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void)
{

}

void Drillerdroid_EditorLoad(void)
{
    Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
}
#endif

void Drillerdroid_Serialize(void)
{

}


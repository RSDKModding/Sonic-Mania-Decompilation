#include "SonicMania.h"

ObjectAmoebaDroid *AmoebaDroid;

void AmoebaDroid_Update(void)
{

}

void AmoebaDroid_LateUpdate(void)
{

}

void AmoebaDroid_StaticUpdate(void)
{

}

void AmoebaDroid_Draw(void)
{

}

void AmoebaDroid_Create(void* data)
{

}

void AmoebaDroid_StageLoad(void)
{
    AmoebaDroid->aniFrames    = RSDK.LoadSpriteAnimation("CPZ/AmoebaDroid.bin", SCOPE_STAGE);
    AmoebaDroid->waterFrames  = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    AmoebaDroid->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    AmoebaDroid->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    AmoebaDroid->sfxGather    = RSDK.GetSFX("CPZ/DroidGather.wav");
    AmoebaDroid->sfxBounce    = RSDK.GetSFX("CPZ/DroidBounce.wav");
    AmoebaDroid->sfxRelease   = RSDK.GetSFX("CPZ/DroidRelease.wav");
}

void AmoebaDroid_EditorDraw(void)
{

}

void AmoebaDroid_EditorLoad(void)
{

}

void AmoebaDroid_Serialize(void) { RSDK_EDITABLE_VAR(AmoebaDroid, VAR_ENUM, type); }


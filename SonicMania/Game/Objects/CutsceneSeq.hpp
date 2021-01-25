#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCutsceneSeq : Object {

};

// Entity Class
struct EntityCutsceneSeq : Entity {

};

// Object Entity
extern ObjectCutsceneSeq CutsceneSeq;

// Standard Entity Events
void CutsceneSeq_Update();
void CutsceneSeq_LateUpdate();
void CutsceneSeq_StaticUpdate();
void CutsceneSeq_Draw();
void CutsceneSeq_Create(void* data);
void CutsceneSeq_StageLoad();
void CutsceneSeq_EditorDraw();
void CutsceneSeq_EditorLoad();
void CutsceneSeq_Serialize();

// Extra Entity Functions


#endif //!OBJ_CUTSCENESEQ_H

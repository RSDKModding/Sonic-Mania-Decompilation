#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

#include "../../SonicMania.h"

//Object Class
class ObjectCutsceneSeq : public Object {
public:

};

//Entity Class
class EntityCutsceneSeq : public Entity {
public:

};

//Object Entity
ObjectCutsceneSeq CutsceneSeq;

//Entity Functions
void CutsceneSeq_Update();
void CutsceneSeq_EarlyUpdate();
void CutsceneSeq_LateUpdate();
void CutsceneSeq_Draw();
void CutsceneSeq_Setup(void* subtype);
void CutsceneSeq_StageLoad();
void CutsceneSeq_GetAttributes();

#endif //!OBJ_CUTSCENESEQ_H

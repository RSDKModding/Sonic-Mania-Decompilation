#ifndef OBJ_CUTSCENESEQ_H
#define OBJ_CUTSCENESEQ_H

//Object Class
class CutsceneSeq : Object {

};

//Entity Class
class EntityCutsceneSeq : Entity {

};

//Entity Functions
void CutsceneSeq_Update();
void CutsceneSeq_EarlyUpdate();
void CutsceneSeq_LateUpdate();
void CutsceneSeq_Draw();
void CutsceneSeq_Setup(void* subtype);
void CutsceneSeq_StageLoad();
void CutsceneSeq_GetAttributes();

#endif //!OBJ_CUTSCENESEQ_H

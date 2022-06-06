#ifndef SCENE3D_H
#define SCENE3D_H

namespace RSDK
{

#define RSDK_SIGNATURE_MDL (0x4C444D) // "MDL"

#define SCENE3D_COUNT    (0x20)
#define MODEL_COUNT      (0x100)
#define SCENE3D_VERT_COUNT (0x4000)

enum Scene3DDrawTypes {
    S3D_WIREFRAME,
    S3D_SOLIDCOLOR,

    S3D_UNUSED_1,
    S3D_UNUSED_2,

    S3D_WIREFRAME_SHADED,
    S3D_SOLIDCOLOR_SHADED,

    S3D_SOLIDCOLOR_SHADED_BLENDED,

    S3D_WIREFRAME_SCREEN,
    S3D_SOLIDCOLOR_SCREEN,

    S3D_WIREFRAME_SHADED_SCREEN,
    S3D_SOLIDCOLOR_SHADED_SCREEN,

    S3D_SOLIDCOLOR_SHADED_BLENDED_SCREEN,
};

struct ScanEdge {
    int32 start;
    int32 end;

    int32 startR;
    int32 endR;
    int32 startG;
    int32 endG;
    int32 startB;
    int32 endB;
};

struct Matrix {
    int32 values[4][4];
};

struct ModelVertex {
    int32 x;
    int32 y;
    int32 z;

    int32 nx;
    int32 ny;
    int32 nz;
};

struct TexCoord {
    float x;
    float y;
};

struct Model {
    RETRO_HASH_MD5(hash);
    ModelVertex *vertices;
    TexCoord *texCoords;
    Color *colors;
    uint16 *indices;
    uint16 vertCount;
    uint16 indexCount;
    uint16 frameCount;
    uint8 flags;
    uint8 faceVertCount;
    uint8 scope;
};

struct Scene3DVertex {
    int32 x;
    int32 y;
    int32 z;

    int32 nx;
    int32 ny;
    int32 nz;

    int32 tx;
    int32 ty;

    uint32 color;
};

struct Scene3DFace {
    int32 depth;
    int32 index;
};

struct Scene3D {
    RETRO_HASH_MD5(hash);
    Scene3DVertex *vertices;
    Scene3DVertex *normals;
    Scene3DFace *faceBuffer;
    uint8 *faceVertCounts;

    int32 projectionX;
    int32 projectionY;

    int32 diffuseX;
    int32 diffuseY;
    int32 diffuseZ;

    int32 diffuseIntensityX;
    int32 diffuseIntensityY;
    int32 diffuseIntensityZ;

    int32 specularIntensityX;
    int32 specularIntensityY;
    int32 specularIntensityZ;

    uint16 vertLimit;
    uint16 vertexCount;
    uint16 faceCount;
    uint8 drawMode;
    uint8 scope;
};

extern Model modelList[MODEL_COUNT];
extern Scene3D scene3DList[SCENE3D_COUNT];

extern ScanEdge scanEdgeBuffer[SCREEN_YSIZE * 2];

void ProcessScanEdge(int32 x1, int32 y1, int32 x2, int32 y2);
void ProcessScanEdgeClr(uint32 c1, uint32 c2, int32 x1, int32 y1, int32 x2, int32 y2);

void SetIdentityMatrix(Matrix *matrix);
void MatrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
void MatrixTranslateXYZ(Matrix *Matrix, int32 x, int32 y, int32 z, bool32 setIdentity);
void MatrixScaleXYZ(Matrix *matrix, int32 scaleX, int32 scaleY, int32 scaleZ);
void MatrixRotateX(Matrix *matrix, int16 angle);
void MatrixRotateY(Matrix *matrix, int16 angle);
void MatrixRotateZ(Matrix *matrix, int16 angle);
void MatrixRotateXYZ(Matrix *matrix, int16 rotationX, int16 rotationY, int16 rotationZ);
void MatrixInverse(Matrix *dest, Matrix *matrix);
inline void MatrixCopy(Matrix *matDst, Matrix *matSrc) { memcpy(matDst, matSrc, sizeof(Matrix)); }

uint16 LoadMesh(const char *filepath, Scopes scope);
uint16 Create3DScene(const char *name, uint16 faceCnt, Scopes scope);
inline void Prepare3DScene(uint16 sceneID)
{
    if (sceneID < SCENE3D_COUNT) {
        Scene3D *scn = &scene3DList[sceneID];

        scn->vertexCount = 0;
        scn->faceCount   = 0;

        memset(scn->vertices, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->normals, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->faceVertCounts, 0, sizeof(uint8) * scn->vertLimit);
        memset(scn->faceBuffer, 0, sizeof(Scene3DFace) * scn->vertLimit);
    }
}

inline void SetMeshAnimation(uint16 model, Animator *animator, int16 speed, uint8 loopIndex, bool32 forceApply, uint16 frameID)
{
    if (model >= MODEL_COUNT) {
        if (animator)
            animator->frames = NULL;

        return;
    }

    if (!animator)
        return;

    if (animator->animationID == model && !forceApply)
        return;

    animator->frames          = (SpriteFrame *)1;
    animator->timer           = 0;
    animator->frameID         = frameID;
    animator->frameCount      = modelList[model].frameCount;
    animator->speed           = speed;
    animator->prevAnimationID = animator->animationID;
    animator->frameDuration   = 0x100;
    animator->loopIndex       = loopIndex;
    animator->animationID     = model;
}
inline void SetDiffuseColor(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_COUNT) {
        Scene3D *scn  = &scene3DList[sceneID];
        scn->diffuseX = x;
        scn->diffuseY = y;
        scn->diffuseZ = z;
    }
}
inline void SetDiffuseIntensity(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_COUNT) {
        Scene3D *scn           = &scene3DList[sceneID];
        scn->diffuseIntensityX = x;
        scn->diffuseIntensityY = y;
        scn->diffuseIntensityZ = z;
    }
}
inline void SetSpecularIntensity(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_COUNT) {
        Scene3D *scn            = &scene3DList[sceneID];
        scn->specularIntensityX = x;
        scn->specularIntensityY = y;
        scn->specularIntensityZ = z;
    }
}
void AddModelToScene(uint16 modelFrames, uint16 sceneIndex, uint8 drawMode, Matrix *matWorld, Matrix *matView, color color);
void AddMeshFrameToScene(uint16 modelFrames, uint16 sceneIndex, Animator *animator, uint8 drawMode, Matrix *matWorld, Matrix *matView, color color);
void Draw3DScene(uint16 sceneID);

inline void Clear3DScenes()
{
    // Unload Models
    for (int32 m = 0; m < MODEL_COUNT; ++m) {
        if (modelList[m].scope != SCOPE_GLOBAL) {
            MEM_ZERO(modelList[m]);
            modelList[m].scope = SCOPE_NONE;
        }
    }

    // Unload 3D Scenes
    for (int32 s = 0; s < SCENE3D_COUNT; ++s) {
        if (scene3DList[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(scene3DList[s]);
            scene3DList[s].scope = SCOPE_NONE;
        }
    }
}

} // namespace RSDK

#endif

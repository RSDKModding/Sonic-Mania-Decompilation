#include "RSDK/Core/RetroEngine.hpp"
using namespace RSDK;

Model modelList[MODEL_MAX];
Scene3D scene3DList[SCENE3D_MAX];

ScanEdge scanEdgeBuffer[SCREEN_YSIZE * 2];

enum ModelFlags {
    MODEL_NOFLAGS     = 0,
    MODEL_USENORMALS  = 1 << 0,
    MODEL_USETEXTURES = 1 << 1,
    MODEL_USECOLOURS  = 1 << 2,
};

void ProcessScanEdge(int32 x1, int32 y1, int32 x2, int32 y2)
{
    int top = y1 >> 16;
    int iy1 = y1 >> 16;
    int iy2 = y2 >> 16;
    int ix2 = x2 >> 16;
    int ix1 = x1 >> 16;
    if (y1 >> 16 != y2 >> 16) {
        if (y1 >> 16 > y2 >> 16) {
            top = y2 >> 16;
            ix1 = x2 >> 16;
            ix2 = x1 >> 16;
            iy1 = y2 >> 16;
            iy2 = y1 >> 16;
        }

        int bottom = iy2 + 1;
        if (top < currentScreen->clipBound_Y2 && bottom >= currentScreen->clipBound_Y1) {
            if (bottom > currentScreen->clipBound_Y2)
                bottom = currentScreen->clipBound_Y2;
            int scanPos = ix1 << 16;
            int delta   = ((ix2 - ix1) << 16) / (iy2 - iy1);
            if (top < 0) {
                scanPos -= top * delta;
                top = 0;
            }

            ScanEdge *edge = &scanEdgeBuffer[top];
            for (int i = top; i < bottom; ++i) {
                int scanX = scanPos >> 16;
                if (scanX < edge->start)
                    edge->start = scanX;
                if (scanX > edge->end)
                    edge->end = scanX;
                scanPos += delta;
                ++edge;
            }
        }
    }
}

void ProcessScanEdgeClr(uint32 c1, uint32 c2, int32 x1, int32 y1, int32 x2, int32 y2)
{
    int iy1 = y1 >> 16;
    int iy2 = y2 >> 16;
    int ix1 = x1 >> 16;
    int ix2 = x2 >> 16;

    int top   = y1 >> 16;
    uint clr1 = c1;
    uint clr2 = c2;
    if (y1 >> 16 != y2 >> 16) {
        if (y1 >> 16 > y2 >> 16) {
            top  = y2 >> 16;
            ix1  = x2 >> 16;
            ix2  = x1 >> 16;
            iy1  = y2 >> 16;
            iy2  = y1 >> 16;
            clr1 = c2;
            clr2 = c1;
        }

        int bottom = iy2 + 1;
        if (top < currentScreen->clipBound_Y2 && bottom >= currentScreen->clipBound_Y1) {
            if (bottom > currentScreen->clipBound_Y2)
                bottom = currentScreen->clipBound_Y2;

            int size     = iy2 - iy1;
            int scanPosX = ix1 << 16;
            int deltaX   = ((ix2 - ix1) << 16) / size;

            int c1R      = (clr1 & 0xFF0000);
            int c2R      = (clr2 & 0xFF0000);
            int scanPosR = c1R;
            int deltaR   = 0;
            if (c1R != c2R)
                deltaR = (c2R - c1R) / size;

            int c1G      = (clr1 & 0x00FF00) << 8;
            int c2G      = (clr2 & 0x00FF00) << 8;
            int scanPosG = c1G;
            int deltaG   = 0;
            if (c1G != c2G)
                deltaG = (c2G - c1G) / size;

            int c1B      = (clr1 & 0x0000FF) << 16;
            int c2B      = (clr2 & 0x0000FF) << 16;
            int scanPosB = c1B;
            int deltaB   = 0;
            if (c1B != c2B)
                deltaB = (c2B - c1B) / size;

            if (top < 0) {
                scanPosX -= top * deltaX;
                scanPosR -= top * deltaR;
                scanPosG -= top * deltaG;
                scanPosB -= top * deltaB;
                top = 0;
            }

            ScanEdge *edge = &scanEdgeBuffer[top];
            for (int i = top; i < bottom; ++i) {
                int scanX = scanPosX >> 16;
                if (scanX < edge->start) {
                    edge->start  = scanX;
                    edge->startR = scanPosR;
                    edge->startG = scanPosG;
                    edge->startB = scanPosB;
                }
                if (scanX > edge->end) {
                    edge->end  = scanX;
                    edge->endR = scanPosR;
                    edge->endG = scanPosG;
                    edge->endB = scanPosB;
                }
                scanPosX += deltaX;
                scanPosR += deltaR;
                scanPosG += deltaG;
                scanPosB += deltaB;
                ++edge;
            }
        }
    }
}

void setIdentityMatrix(Matrix *matrix)
{
    matrix->values[0][0] = 0x100;
    matrix->values[1][0] = 0;
    matrix->values[2][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[0][1] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[2][1] = 0;
    matrix->values[3][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[1][2] = 0;
    matrix->values[2][2] = 0x100;
    matrix->values[3][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][3] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB)
{
    int result[4][4];
    memset(result, 0, 4 * 4 * sizeof(int));

    for (int i = 0; i < 0x10; ++i) {
        uint rowA          = i / 4;
        uint rowB          = i % 4;
        result[rowB][rowA] = (matrixA->values[3][rowA] * matrixB->values[rowB][3] >> 8) + (matrixA->values[2][rowA] * matrixB->values[rowB][2] >> 8)
                             + (matrixA->values[1][rowA] * matrixB->values[rowB][1] >> 8)
                             + (matrixA->values[0][rowA] * matrixB->values[rowB][0] >> 8);
    }

    for (int i = 0; i < 0x10; ++i) {
        uint rowA                = i / 4;
        uint rowB                = i % 4;
        dest->values[rowB][rowA] = result[rowB][rowA];
    }
}
void matrixTranslateXYZ(Matrix *matrix, int32 x, int32 y, int32 z, bool32 setIdentity)
{
    if (setIdentity) {
        matrix->values[0][0] = 0x100;
        matrix->values[1][0] = 0;
        matrix->values[2][0] = 0;
        matrix->values[0][1] = 0;
        matrix->values[1][1] = 0x100;
        matrix->values[2][1] = 0;
        matrix->values[0][2] = 0;
        matrix->values[1][2] = 0;
        matrix->values[2][2] = 0x100;
        matrix->values[3][0] = 0;
        matrix->values[3][1] = 0;
        matrix->values[3][2] = 0;
        matrix->values[3][3] = 0x100;
    }
    matrix->values[0][3] = x >> 8;
    matrix->values[1][3] = y >> 8;
    matrix->values[2][3] = z >> 8;
}
void matrixScaleXYZ(Matrix *matrix, int32 scaleX, int32 scaleY, int32 scaleZ)
{
    matrix->values[0][0] = scaleX;
    matrix->values[1][0] = 0;
    matrix->values[2][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[0][1] = 0;
    matrix->values[1][1] = scaleY;
    matrix->values[2][1] = 0;
    matrix->values[3][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[1][2] = 0;
    matrix->values[2][2] = scaleZ;
    matrix->values[3][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][3] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateX(Matrix *matrix, int16 rotationX)
{
    int sine   = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosine = cosVal1024[rotationX & 0x3FF] >> 2;

    matrix->values[0][0] = 0x100;
    matrix->values[1][0] = 0;
    matrix->values[2][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[0][1] = 0;
    matrix->values[1][1] = cosine;
    matrix->values[2][1] = sine;
    matrix->values[3][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[1][2] = -sine;
    matrix->values[2][2] = cosine;
    matrix->values[3][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][3] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateY(Matrix *matrix, int16 rotationY)
{
    int sine             = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationY & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[1][0] = 0;
    matrix->values[2][0] = sine;
    matrix->values[3][0] = 0;
    matrix->values[0][1] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[2][1] = 0;
    matrix->values[3][1] = 0;
    matrix->values[0][2] = -sine;
    matrix->values[1][2] = 0;
    matrix->values[2][2] = cosine;
    matrix->values[3][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][3] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateZ(Matrix *matrix, int16 rotationZ)
{
    int sine             = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationZ & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[1][0] = -sine;
    matrix->values[2][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[0][1] = sine;
    matrix->values[1][1] = cosine;
    matrix->values[2][1] = 0;
    matrix->values[3][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[1][2] = 0;
    matrix->values[2][2] = 0x100;
    matrix->values[3][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][3] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateXYZ(Matrix *matrix, int16 rotationX, int16 rotationY, int16 rotationZ)
{
    int sinX = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosX = cosVal1024[rotationX & 0x3FF] >> 2;
    int sinY = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosY = cosVal1024[rotationY & 0x3FF] >> 2;
    int sinZ = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosZ = cosVal1024[rotationZ & 0x3FF] >> 2;

    matrix->values[0][0] = (cosZ * cosY >> 8) + (sinZ * (sinY * sinX >> 8) >> 8);
    matrix->values[0][1] = -(sinZ * cosX) >> 8;
    matrix->values[0][2] = (sinZ * (cosY * sinX >> 8) >> 8) - (cosZ * sinY >> 8);
    matrix->values[0][3] = 0;
    matrix->values[1][0] = (sinZ * cosY >> 8) - (cosZ * (sinY * sinX >> 8) >> 8);
    matrix->values[1][1] = cosZ * cosX >> 8;
    matrix->values[1][2] = (-(sinZ * sinY) >> 8) - (cosZ * (cosY * sinX >> 8) >> 8);
    matrix->values[1][3] = 0;
    matrix->values[2][0] = sinY * cosX >> 8;
    matrix->values[2][1] = sinX;
    matrix->values[2][2] = cosY * cosX >> 8;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixInverse(Matrix *dest, Matrix *matrix)
{
    double inv[16], det;
    double m[16];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            m[(y << 2) + x] = matrix->values[y][x] / 256.0;
        }
    }

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for (int i = 0; i < 0x10; ++i) inv[i] = (int)((inv[i] * det) * 256);
    for (int i = 0; i < 0x10; ++i) dest->values[i / 4][i % 4] = inv[i];
}
void matrixCopy(Matrix *matDst, Matrix *matSrc) { memcpy(matDst, matSrc, sizeof(Matrix)); }

uint16 LoadMesh(const char *filename, Scopes scope)
{
    char buffer[0x100];
    sprintf(buffer, "Data/Meshes/%s", filename);

    uint hash[4];
    GEN_HASH(buffer, hash);

    for (int i = 0; i < MODEL_MAX; ++i) {
        if (HASH_MATCH(hash, modelList[i].hash)) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < MODEL_MAX; ++id) {
        if (modelList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= MODEL_MAX)
        return -1;

    Model *model = &modelList[id];
    FileInfo info;
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        uint sig = ReadInt32(&info, false);

        if (sig != 0x4C444D) {
            CloseFile(&info);
            return -1;
        }

        model->scope = scope;
        HASH_COPY(model->hash, hash);

        model->flags         = ReadInt8(&info);
        model->faceVertCount = ReadInt8(&info);

        model->vertCount  = ReadInt16(&info);
        model->frameCount = ReadInt16(&info);

        RSDK::AllocateStorage(sizeof(ModelVertex) * model->vertCount * model->frameCount, (void **)&model->vertices, RSDK::DATASET_STG, true);
        if (model->flags & MODEL_USETEXTURES)
            RSDK::AllocateStorage(sizeof(TexCoord) * model->vertCount, (void **)&model->texCoords, RSDK::DATASET_STG, true);
        if (model->flags & MODEL_USECOLOURS)
            RSDK::AllocateStorage(sizeof(Colour) * model->vertCount, (void **)&model->colours, RSDK::DATASET_STG, true);

        if (model->flags & MODEL_USETEXTURES) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->texCoords[v].x = ReadSingle(&info);
                model->texCoords[v].y = ReadSingle(&info);
            }
        }

        if (model->flags & MODEL_USECOLOURS) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->colours[v].colour = ReadInt32(&info, false);
            }
        }

        model->indexCount = ReadInt16(&info);
        RSDK::AllocateStorage(sizeof(ushort) * model->indexCount, (void **)&model->indices, RSDK::DATASET_STG, true);
        for (int i = 0; i < model->indexCount; ++i) model->indices[i] = ReadInt16(&info);

        for (int f = 0; f < model->frameCount; ++f) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->vertices[(f * model->vertCount) + v].x = ReadSingle(&info) * 256.0f;
                model->vertices[(f * model->vertCount) + v].y = ReadSingle(&info) * 256.0f;
                model->vertices[(f * model->vertCount) + v].z = ReadSingle(&info) * 256.0f;

                model->vertices[(f * model->vertCount) + v].nx = 0;
                model->vertices[(f * model->vertCount) + v].ny = 0;
                model->vertices[(f * model->vertCount) + v].nz = 0;
                if (model->flags & MODEL_USENORMALS) {
                    model->vertices[(f * model->vertCount) + v].nx = ReadSingle(&info) * 65536.0f;
                    model->vertices[(f * model->vertCount) + v].ny = ReadSingle(&info) * 65536.0f;
                    model->vertices[(f * model->vertCount) + v].nz = ReadSingle(&info) * 65536.0f;
                }
            }
        }

        CloseFile(&info);
        return id;
    }
    return -1;
}
uint16 Create3DScene(const char *name, uint16 vertexLimit, Scopes scope)
{
    uint hash[4];
    GEN_HASH(name, hash);

    for (int i = 0; i < SCENE3D_MAX; ++i) {
        if (HASH_MATCH(hash, scene3DList[i].hash)) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SCENE3D_MAX; ++id) {
        if (scene3DList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SCENE3D_MAX)
        return -1;

    Scene3D *scene = &scene3DList[id];

    if (vertexLimit > SCENE3D_VERT_MAX || !vertexLimit)
        vertexLimit = SCENE3D_VERT_MAX;

    scene->scope = scope;
    HASH_COPY(scene->hash, hash);
    scene->vertLimit   = vertexLimit;
    scene->faceCount   = 6;
    scene->projectionX = 8;
    scene->projectionY = 8;
    RSDK::AllocateStorage(sizeof(Scene3DVertex) * vertexLimit, (void **)&scene->vertices, RSDK::DATASET_STG, true);
    RSDK::AllocateStorage(sizeof(Scene3DVertex) * vertexLimit, (void **)&scene->normals, RSDK::DATASET_STG, true);
    RSDK::AllocateStorage(sizeof(byte) * vertexLimit, (void **)&scene->faceVertCounts, RSDK::DATASET_STG, true);
    RSDK::AllocateStorage(sizeof(FaceBufferEntry) * vertexLimit, (void **)&scene->faceBuffer, RSDK::DATASET_STG, true);

    return id;
}
void AddModelToScene(uint16 modelID, uint16 sceneID, uint8 drawMode, Matrix *matWorld, Matrix *matNormals, colour colour)
{
    if (modelID < MODEL_MAX && sceneID < SCENE3D_MAX) {
        if (matWorld) {
            Model *mdl           = &modelList[modelID];
            Scene3D *scn         = &scene3DList[sceneID];
            ushort *indices      = mdl->indices;
            int vertID           = scn->vertexCount;
            byte *faceVertCounts = &scn->faceVertCounts[scn->faceCount];
            int indCnt           = mdl->indexCount;
            if (scn->vertLimit - vertID >= indCnt) {
                scn->vertexCount += mdl->indexCount;
                scn->drawMode = drawMode;
                scn->faceCount += indCnt / mdl->faceVertCount;

                int i = 0;
                int f = 0;
                switch (mdl->flags) {
                    default:
                    case MODEL_NOFLAGS:
                    case MODEL_USECOLOURS:
                        for (; i < mdl->indexCount;) {
                            faceVertCounts[f++] = mdl->faceVertCount;
                            for (int c = 0; c < mdl->faceVertCount; ++c) {
                                ModelVertex *modelVert = &mdl->vertices[indices[i++]];
                                Scene3DVertex *vertex  = &scn->vertices[vertID++];

                                vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                            + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                            + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                vertex->z = matWorld->values[2][3] + ((modelVert->x * matWorld->values[2][0]) >> 8)
                                            + ((matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));

                                vertex->colour = colour;
                            }
                        }
                        break;
                    case MODEL_USENORMALS:
                        if (matNormals) {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *modelVert = &mdl->vertices[indices[i++]];
                                    Scene3DVertex *vertex  = &scn->vertices[vertID++];

                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (modelVert->x * matWorld->values[0][0] >> 8) + (modelVert->y * matWorld->values[0][1] >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->z * matWorld->values[1][2] >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (matWorld->values[2][2] * modelVert->z >> 8) + (matWorld->values[2][1] * modelVert->y >> 8);

                                    vertex->nx = (modelVert->nz * matNormals->values[0][2] >> 8) + (modelVert->nx * matNormals->values[0][0] >> 8)
                                                 + (matNormals->values[0][1] * modelVert->ny >> 8);
                                    vertex->ny = (modelVert->ny * matNormals->values[1][1] >> 8) + (modelVert->nz * matNormals->values[1][2] >> 8)
                                                 + (modelVert->nx * matNormals->values[1][0] >> 8);
                                    vertex->nz = ((modelVert->ny * matNormals->values[2][1]) >> 8)
                                                 + ((matNormals->values[2][0] * modelVert->nx >> 8) + (modelVert->nz * matNormals->values[2][2] >> 8));

                                    vertex->colour = colour;
                                }
                            }
                        }
                        else {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *modelVert = &mdl->vertices[indices[i++]];
                                    Scene3DVertex *vertex  = &scn->vertices[vertID++];

                                    vertex->x              = matWorld->values[0][3] + (modelVert->z * matWorld->values[0][2] >> 8)
                                                + (matWorld->values[0][0] * modelVert->x >> 8) + (matWorld->values[0][1] * modelVert->y >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->y * matWorld->values[1][1] >> 8)
                                                + (modelVert->z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->z = matWorld->values[2][3] + ((matWorld->values[2][2] * modelVert->z) >> 8)
                                                + ((matWorld->values[2][0] * modelVert->x >> 8) + (matWorld->values[2][1] * modelVert->y >> 8));

                                    vertex->colour = colour;
                                }
                            }
                        }
                        break;
                    case MODEL_USENORMALS | MODEL_USECOLOURS:
                        if (matNormals) {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *modelVert = &mdl->vertices[indices[i]];
                                    Colour *modelColour    = &mdl->colours[indices[i++]];
                                    Scene3DVertex *vertex  = &scn->vertices[vertID++];

                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (matWorld->values[0][0] * modelVert->x >> 8);
                                    vertex->y = matWorld->values[1][3] + (matWorld->values[1][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[1][1] >> 8) + (matWorld->values[1][0] * modelVert->x >> 8);
                                    vertex->z = matWorld->values[2][3] + (modelVert->x * matWorld->values[2][0] >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (matWorld->values[2][2] * modelVert->z >> 8);

                                    vertex->nx = (matNormals->values[0][0] * modelVert->nx >> 8) + (modelVert->ny * matNormals->values[0][1] >> 8)
                                                 + (matNormals->values[0][2] * modelVert->nz >> 8);
                                    vertex->ny = (matNormals->values[1][0] * modelVert->nx >> 8) + (modelVert->ny * matNormals->values[1][1] >> 8)
                                                 + (matNormals->values[1][2] * modelVert->nz >> 8);
                                    vertex->nz = ((matNormals->values[2][2] * modelVert->nz) >> 8)
                                                 + ((modelVert->ny * matNormals->values[2][1] >> 8) + (matNormals->values[2][0] * modelVert->nx >> 8));

                                    vertex->colour = modelColour->colour;
                                }
                            }
                        }
                        else {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *modelVert = &mdl->vertices[indices[i]];
                                    Colour *modelColour    = &mdl->colours[indices[i++]];
                                    Scene3DVertex *vertex  = &scn->vertices[vertID++];

                                    vertex->x              = matWorld->values[0][3] + (matWorld->values[0][0] * modelVert->x >> 8)
                                                + (modelVert->y * matWorld->values[0][1] >> 8) + (modelVert->z * matWorld->values[0][2] >> 8);
                                    vertex->y = matWorld->values[1][3] + (modelVert->z * matWorld->values[1][2] >> 8)
                                                + (matWorld->values[1][0] * modelVert->x >> 8) + (modelVert->y * matWorld->values[1][1] >> 8);
                                    vertex->z = matWorld->values[2][3] + (matWorld->values[2][2] * modelVert->z >> 8)
                                                + (modelVert->y * matWorld->values[2][1] >> 8) + (modelVert->x * matWorld->values[2][0] >> 8);

                                    vertex->colour = modelColour->colour;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}
void AddMeshFrameToScene(uint16 modelID, uint16 sceneID, RSDK::Animator *animator, uint8 drawMode, Matrix *matWorld, Matrix *matNormals,
                         colour colour)
{
    if (modelID < MODEL_MAX && sceneID < SCENE3D_MAX) {
        if (matWorld && animator) {
            Model *mdl           = &modelList[modelID];
            Scene3D *scn         = &scene3DList[sceneID];
            ushort *indices      = mdl->indices;
            int vertID           = scn->vertexCount;
            byte *faceVertCounts = &scn->faceVertCounts[scn->faceCount];
            int indCnt           = mdl->indexCount;
            if (scn->vertLimit - vertID >= indCnt) {
                scn->vertexCount += mdl->indexCount;
                scn->drawMode = drawMode;
                scn->faceCount += indCnt / mdl->faceVertCount;

                int nextFrame = animator->frameID + 1;
                if (nextFrame >= animator->frameCount)
                    nextFrame = animator->loopIndex;
                int frameOffset     = animator->frameID * mdl->vertCount;
                int nextFrameOffset = nextFrame * mdl->vertCount;

                int i           = 0;
                int f           = 0;
                int interpolate = animator->timer;
                switch (mdl->flags) {
                    default:
                    case MODEL_NOFLAGS:
                    case MODEL_USECOLOURS:
                        for (; i < mdl->indexCount;) {
                            faceVertCounts[f++] = mdl->faceVertCount;
                            for (int c = 0; c < mdl->faceVertCount; ++c) {
                                ModelVertex *frameVert     = &mdl->vertices[frameOffset + indices[i]];
                                ModelVertex *nextFrameVert = &mdl->vertices[nextFrameOffset + indices[i]];
                                int x                      = frameVert->x + ((interpolate * (nextFrameVert->x - frameVert->x)) >> 8);
                                int y                      = frameVert->y + ((interpolate * (nextFrameVert->y - frameVert->y)) >> 8);
                                int z                      = frameVert->z + ((interpolate * (nextFrameVert->z - frameVert->z)) >> 8);
                                i++;
                                Scene3DVertex *vertex = &scn->vertices[vertID++];
                                vertex->x             = matWorld->values[0][3] + (z * matWorld->values[0][2] >> 8) + (matWorld->values[0][0] * x >> 8)
                                            + (matWorld->values[0][1] * y >> 8);
                                vertex->y = matWorld->values[1][3] + (y * matWorld->values[1][1] >> 8) + (z * matWorld->values[1][2] >> 8)
                                            + (matWorld->values[1][0] * x >> 8);
                                vertex->z = matWorld->values[2][3] + ((x * matWorld->values[2][0]) >> 8)
                                            + ((matWorld->values[2][2] * z >> 8) + (matWorld->values[2][1] * y >> 8));
                                vertex->colour = colour;
                            }
                        }
                        break;
                    case MODEL_USENORMALS:
                        if (matNormals) {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *frameVert     = &mdl->vertices[frameOffset + indices[i]];
                                    ModelVertex *nextFrameVert = &mdl->vertices[nextFrameOffset + indices[i]];
                                    int x                      = frameVert->x + ((interpolate * (nextFrameVert->x - frameVert->x)) >> 8);
                                    int y                      = frameVert->y + ((interpolate * (nextFrameVert->y - frameVert->y)) >> 8);
                                    int z                      = frameVert->z + ((interpolate * (nextFrameVert->z - frameVert->z)) >> 8);
                                    int nx                     = frameVert->nx + ((interpolate * (nextFrameVert->nx - frameVert->nx)) >> 8);
                                    int ny                     = frameVert->ny + ((interpolate * (nextFrameVert->ny - frameVert->ny)) >> 8);
                                    int nz                     = frameVert->nz + ((interpolate * (nextFrameVert->nz - frameVert->nz)) >> 8);
                                    i++;

                                    Scene3DVertex *vertex = &scn->vertices[vertID++];
                                    vertex->x = matWorld->values[0][3] + (z * matWorld->values[0][2] >> 8) + (x * matWorld->values[0][0] >> 8)
                                                + (y * matWorld->values[0][1] >> 8);
                                    vertex->y = matWorld->values[1][3] + (y * matWorld->values[1][1] >> 8) + (matWorld->values[1][0] * x >> 8)
                                                + (z * matWorld->values[1][2] >> 8);
                                    vertex->z = matWorld->values[2][3] + (x * matWorld->values[2][0] >> 8) + (matWorld->values[2][2] * z >> 8)
                                                + (matWorld->values[2][1] * y >> 8);
                                    vertex->nx =
                                        (nz * matNormals->values[0][2] >> 8) + (nx * matNormals->values[0][0] >> 8) + (matNormals->values[0][1] * ny >> 8);
                                    vertex->ny =
                                        (ny * matNormals->values[1][1] >> 8) + (nz * matNormals->values[1][2] >> 8) + (nx * matNormals->values[1][0] >> 8);
                                    vertex->nz =
                                        ((ny * matNormals->values[2][1]) >> 8) + ((matNormals->values[2][0] * nx >> 8) + (nz * matNormals->values[2][2] >> 8));
                                    vertex->colour = colour;
                                }
                            }
                        }
                        else {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *frameVert     = &mdl->vertices[frameOffset + indices[i]];
                                    ModelVertex *nextFrameVert = &mdl->vertices[nextFrameOffset + indices[i]];
                                    int x                      = frameVert->x + ((interpolate * (nextFrameVert->x - frameVert->x)) >> 8);
                                    int y                      = frameVert->y + ((interpolate * (nextFrameVert->y - frameVert->y)) >> 8);
                                    int z                      = frameVert->z + ((interpolate * (nextFrameVert->z - frameVert->z)) >> 8);
                                    i++;
                                    Scene3DVertex *vertex = &scn->vertices[vertID++];
                                    vertex->x = matWorld->values[0][3] + (z * matWorld->values[0][2] >> 8) + (matWorld->values[0][0] * x >> 8)
                                                + (matWorld->values[0][1] * y >> 8);
                                    vertex->y = matWorld->values[1][3] + (y * matWorld->values[1][1] >> 8) + (z * matWorld->values[1][2] >> 8)
                                                + (matWorld->values[1][0] * x >> 8);
                                    vertex->z = matWorld->values[2][3] + ((matWorld->values[2][2] * z) >> 8)
                                                + ((matWorld->values[2][0] * x >> 8) + (matWorld->values[2][1] * y >> 8));
                                    vertex->colour = colour;
                                }
                            }
                        }
                        break;
                    case MODEL_USENORMALS | MODEL_USECOLOURS:
                        if (matNormals) {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *frameVert     = &mdl->vertices[frameOffset + indices[i]];
                                    ModelVertex *nextFrameVert = &mdl->vertices[nextFrameOffset + indices[i]];
                                    int x                      = frameVert->x + ((interpolate * (nextFrameVert->x - frameVert->x)) >> 8);
                                    int y                      = frameVert->y + ((interpolate * (nextFrameVert->y - frameVert->y)) >> 8);
                                    int z                      = frameVert->z + ((interpolate * (nextFrameVert->z - frameVert->z)) >> 8);
                                    int nx                     = frameVert->nx + ((interpolate * (nextFrameVert->nx - frameVert->nx)) >> 8);
                                    int ny                     = frameVert->ny + ((interpolate * (nextFrameVert->ny - frameVert->ny)) >> 8);
                                    int nz                     = frameVert->nz + ((interpolate * (nextFrameVert->nz - frameVert->nz)) >> 8);

                                    Colour *modelColour   = &mdl->colours[indices[i++]];
                                    Scene3DVertex *vertex = &scn->vertices[vertID++];
                                    vertex->x = matWorld->values[0][3] + (matWorld->values[0][2] * z >> 8) + (y * matWorld->values[0][1] >> 8)
                                                + (matWorld->values[0][0] * x >> 8);
                                    vertex->y = matWorld->values[1][3] + (matWorld->values[1][2] * z >> 8) + (y * matWorld->values[1][1] >> 8)
                                                + (matWorld->values[1][0] * x >> 8);
                                    vertex->z = matWorld->values[2][3] + (x * matWorld->values[2][0] >> 8) + (y * matWorld->values[2][1] >> 8)
                                                + (matWorld->values[2][2] * z >> 8);
                                    vertex->nx =
                                        (matNormals->values[0][0] * nx >> 8) + (ny * matNormals->values[0][1] >> 8) + (matNormals->values[0][2] * nz >> 8);
                                    vertex->ny =
                                        (matNormals->values[1][0] * nx >> 8) + (ny * matNormals->values[1][1] >> 8) + (matNormals->values[1][2] * nz >> 8);
                                    vertex->nz =
                                        ((matNormals->values[2][2] * nz) >> 8) + ((ny * matNormals->values[2][1] >> 8) + (matNormals->values[2][0] * nx >> 8));
                                    vertex->colour = modelColour->colour;
                                }
                            }
                        }
                        else {
                            for (; i < mdl->indexCount;) {
                                faceVertCounts[f++] = mdl->faceVertCount;
                                for (int c = 0; c < mdl->faceVertCount; ++c) {
                                    ModelVertex *frameVert     = &mdl->vertices[frameOffset + indices[i]];
                                    ModelVertex *nextFrameVert = &mdl->vertices[nextFrameOffset + indices[i]];
                                    int x                      = frameVert->x + ((interpolate * (nextFrameVert->x - frameVert->x)) >> 8);
                                    int y                      = frameVert->y + ((interpolate * (nextFrameVert->y - frameVert->y)) >> 8);
                                    int z                      = frameVert->z + ((interpolate * (nextFrameVert->z - frameVert->z)) >> 8);
                                    Colour *modelColour   = &mdl->colours[indices[i++]];
                                    Scene3DVertex *vertex = &scn->vertices[vertID++];
                                    vertex->x = matWorld->values[0][3] + (matWorld->values[0][0] * x >> 8) + (y * matWorld->values[0][1] >> 8)
                                                + (z * matWorld->values[0][2] >> 8);
                                    vertex->y = matWorld->values[1][3] + (z * matWorld->values[1][2] >> 8) + (matWorld->values[1][0] * x >> 8)
                                                + (y * matWorld->values[1][1] >> 8);
                                    vertex->z = matWorld->values[2][3] + (matWorld->values[2][2] * z >> 8) + (y * matWorld->values[2][1] >> 8)
                                                + (x * matWorld->values[2][0] >> 8);
                                    vertex->colour = modelColour->colour;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}
void Draw3DScene(uint16 sceneID)
{
    if (sceneID < SCENE3D_MAX) {
        Entity *entity = sceneInfo.entity;
        Scene3D *scn   = &scene3DList[sceneID];

        byte *vertCnt           = scn->faceVertCounts;
        Scene3DVertex *vertices = scn->vertices;
        int vertID              = 0;
        for (int i = 0; i < scn->faceCount; ++i) {
            scn->faceBuffer[i].depth = 0;
            switch (scn->faceVertCounts[i]) {
                default: break;
                case 1:
                    scn->faceBuffer[i].depth = vertices->z;
                    vertices++;
                    break;
                case 2:
                    scn->faceBuffer[i].depth = vertices[0].z >> 1;
                    scn->faceBuffer[i].depth += vertices[1].z >> 1;
                    vertices += 2;
                    break;
                case 3:
                    scn->faceBuffer[i].depth = vertices[0].z >> 1;
                    scn->faceBuffer[i].depth += vertices[1].z >> 1;
                    scn->faceBuffer[i].depth += vertices[2].z >> 1;
                    vertices += 3;
                    break;
                case 4:
                    scn->faceBuffer[i].depth = vertices[0].z >> 2;
                    scn->faceBuffer[i].depth += vertices[1].z >> 2;
                    scn->faceBuffer[i].depth += vertices[2].z >> 2;
                    scn->faceBuffer[i].depth += vertices[3].z >> 2;
                    vertices += 4;
                    break;
            }
            scn->faceBuffer[i].index = vertID;
            vertID += scn->faceVertCounts[i];
        }

        for (int i = 0; i < scn->faceCount; ++i) {
            for (int j = scn->faceCount - 1; j > i; --j) {
                if (scn->faceBuffer[j].depth > scn->faceBuffer[j - 1].depth) {
                    int index                    = scn->faceBuffer[j].index;
                    int depth                    = scn->faceBuffer[j].depth;
                    scn->faceBuffer[j].index     = scn->faceBuffer[j - 1].index;
                    scn->faceBuffer[j].depth     = scn->faceBuffer[j - 1].depth;
                    scn->faceBuffer[j - 1].index = index;
                    scn->faceBuffer[j - 1].depth = depth;
                }
            }
        }

        vertCnt = scn->faceVertCounts;
        Vector2 vertPos[4];
        uint vertClrs[4];
        switch (scn->drawMode) {
            default: break;
            case S3D_FLATCLR_WIREFRAME:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    for (int v = 0; v < *vertCnt - 1; ++v) {
                        DrawLine(drawVert[v + 0].x << 8, drawVert[v + 0].y << 8, drawVert[v + 1].x << 8, drawVert[v + 1].y << 8, drawVert[0].colour,
                                 entity->alpha, (InkEffects)entity->inkEffect, false);
                    }
                    DrawLine(drawVert[0].x << 8, drawVert[0].y << 8, drawVert[*vertCnt - 1].x << 8, drawVert[*vertCnt - 1].y << 8, drawVert[0].colour,
                             entity->alpha, (InkEffects)entity->inkEffect, false);
                    vertCnt++;
                }
                break;
            case S3D_FLATCLR:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    for (int v = 0; v < *vertCnt; ++v) {
                        vertPos[v].x = (drawVert[v].x << 8) - (currentScreen->position.x << 16);
                        vertPos[v].y = (drawVert[v].y << 8) - (currentScreen->position.y << 16);
                    }
                    DrawFace(vertPos, *vertCnt, (drawVert->colour >> 16) & 0xFF, (drawVert->colour >> 8) & 0xFF, (drawVert->colour >> 0) & 0xFF,
                             entity->alpha, (InkEffects)entity->inkEffect);
                    vertCnt++;
                }
                break;
            case S3D_UNKNOWN_2: break;
            case S3D_UNKNOWN_3: break;
            case S3D_FLATCLR_SHADED_WIREFRAME:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int ny1 = 0;
                    for (int v = 0; v < vertCount; ++v) {
                        ny1 += drawVert[v].ny;
                    }

                    int normal    = ny1 / vertCount;
                    int normalVal = (normal >> 2) * (abs(normal) >> 2);

                    int specular = normalVal >> 6 >> scn->specularIntensityX;
                    specular     = minVal(0xFF, specular);
                    specular     = maxVal(0, specular);
                    int clrR     = specular + ((int)((drawVert->colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX) >> scn->diffuseIntensityX);

                    specular = normalVal >> 6 >> scn->specularIntensityY;
                    specular = minVal(0xFF, specular);
                    specular = maxVal(0, specular);
                    int clrG = specular + ((int)((drawVert->colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY) >> scn->diffuseIntensityY);

                    specular = normalVal >> 6 >> scn->specularIntensityZ;
                    specular = minVal(0xFF, specular);
                    specular = maxVal(0, specular);
                    int clrB = specular + ((int)((drawVert->colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ) >> scn->diffuseIntensityZ);

                    clrR = minVal(0xFF, clrR);
                    clrR = maxVal(0, clrR);
                    clrG = minVal(0xFF, clrG);
                    clrG = maxVal(0, clrG);
                    clrB = minVal(0xFF, clrB);
                    clrB = maxVal(0, clrB);

                    uint colour = (clrR << 16) | (clrG << 8) | (clrB << 0);

                    for (int v = 0; v < vertCount - 1; ++v) {
                        DrawLine(drawVert[v + 0].x << 8, drawVert[v + 0].y << 8, drawVert[v + 1].x << 8, drawVert[v + 1].y << 8, colour,
                                 entity->alpha, (InkEffects)entity->inkEffect, false);
                    }
                    DrawLine(drawVert[vertCount - 1].x << 8, drawVert[vertCount - 1].y << 8, drawVert[0].x << 8, drawVert[0].y << 8, colour,
                             entity->alpha, (InkEffects)entity->inkEffect, false);

                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SHADED:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int ny = 0;
                    for (int v = 0; v < vertCount; ++v) {
                        ny += drawVert[v].ny;
                        vertPos[v].x = (drawVert[v].x << 8) - (currentScreen->position.x << 16);
                        vertPos[v].y = (drawVert[v].y << 8) - (currentScreen->position.y << 16);
                    }

                    int normal    = ny / vertCount;
                    int normalVal = (normal >> 2) * (abs(normal) >> 2);

                    int specular = normalVal >> 6 >> scn->specularIntensityX;
                    specular     = minVal(0xFF, specular);
                    specular     = maxVal(0, specular);
                    int clrR     = specular + ((int)((drawVert->colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX) >> scn->diffuseIntensityX);

                    specular = normalVal >> 6 >> scn->specularIntensityY;
                    specular = minVal(0xFF, specular);
                    specular = maxVal(0, specular);
                    int clrG = specular + ((int)((drawVert->colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY) >> scn->diffuseIntensityY);

                    specular = normalVal >> 6 >> scn->specularIntensityZ;
                    specular = minVal(0xFF, specular);
                    specular = maxVal(0, specular);
                    int clrB = specular + ((int)((drawVert->colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ) >> scn->diffuseIntensityZ);

                    clrR = minVal(0xFF, clrR);
                    clrR = maxVal(0, clrR);
                    clrG = minVal(0xFF, clrG);
                    clrG = maxVal(0, clrG);
                    clrB = minVal(0xFF, clrB);
                    clrB = maxVal(0, clrB);

                    uint colour = (clrR << 16) | (clrG << 8) | (clrB << 0);

                    drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    DrawFace(vertPos, *vertCnt, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, (colour >> 0) & 0xFF, entity->alpha,
                             (InkEffects)entity->inkEffect);

                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SHADED_BLENDED:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    for (int v = 0; v < vertCount; ++v) {
                        vertPos[v].x = (drawVert[v].x << 8) - (currentScreen->position.x << 16);
                        vertPos[v].y = (drawVert[v].y << 8) - (currentScreen->position.y << 16);

                        int normal    = drawVert[v].ny;
                        int normalVal = (normal >> 2) * (abs(normal) >> 2);

                        int specular = (normalVal >> 6) >> scn->specularIntensityX;
                        specular     = minVal(0xFF, specular);
                        specular     = maxVal(0x00, specular);
                        int ambDif   = (int)((drawVert->colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX);
                        int clrR     = specular + (ambDif >> scn->diffuseIntensityX);

                        specular = (normalVal >> 6) >> scn->specularIntensityY;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0x00, specular);
                        ambDif   = (int)((drawVert->colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY);
                        int clrG = specular + (ambDif >> scn->diffuseIntensityY);

                        specular = (normalVal >> 6) >> scn->specularIntensityZ;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0x00, specular);
                        ambDif   = (int)((drawVert->colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ);
                        int clrB = specular + (ambDif >> scn->diffuseIntensityZ);

                        clrR = minVal(0xFF, clrR);
                        clrR = maxVal(0, clrR);
                        clrG = minVal(0xFF, clrG);
                        clrG = maxVal(0, clrG);
                        clrB = minVal(0xFF, clrB);
                        clrB = maxVal(0, clrB);

                        vertClrs[v] = (clrR << 16) | (clrG << 8) | (clrB << 0);
                    }

                    DrawBlendedFace(vertPos, vertClrs, *vertCnt, entity->alpha, (InkEffects)entity->inkEffect);

                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SCREEN_WIREFRAME:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];

                    int v = 0;
                    for (; v < *vertCnt && v < 0xFF; ++v) {
                        int vertZ = drawVert[v].z;
                        if (vertZ < 0x100) {
                            v = 0xFF;
                        }
                        else {
                            vertPos[v].x = currentScreen->center.x + (drawVert[v].x << scn->projectionX) / vertZ;
                            vertPos[v].y = currentScreen->center.y - (drawVert[v].y << scn->projectionY) / vertZ;
                        }
                    }

                    if (v < 0xFF) {
                        for (int v = 0; v < *vertCnt - 1; ++v) {
                            DrawLine(vertPos[v + 0].x, drawVert[v + 0].y, vertPos[v + 1].x, vertPos[v + 1].y, drawVert[0].colour, entity->alpha,
                                     (InkEffects)entity->inkEffect, true);
                        }
                        DrawLine(vertPos[0].x, vertPos[0].y, vertPos[*vertCnt - 1].x, vertPos[*vertCnt - 1].y, drawVert[0].colour, entity->alpha,
                                 (InkEffects)entity->inkEffect, true);
                    }
                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SCREEN:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int v = 0;
                    for (; v < vertCount && v < 0xFF; ++v) {
                        int vertZ = drawVert[v].z;
                        if (vertZ < 0x100) {
                            v = 0xFF;
                        }
                        else {
                            vertPos[v].x = (currentScreen->center.x << 16) + ((drawVert[v].x << scn->projectionX) / vertZ << 16);
                            vertPos[v].y = (currentScreen->center.y << 16) - ((drawVert[v].y << scn->projectionY) / vertZ << 16);
                        }
                    }

                    if (v < 0xFF) {
                        DrawFace(vertPos, *vertCnt, (drawVert[0].colour >> 16) & 0xFF, (drawVert[0].colour >> 8) & 0xFF,
                                 (drawVert[0].colour >> 0) & 0xFF, entity->alpha, (InkEffects)entity->inkEffect);
                    }
                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SHADED_SCREEN_WIREFRAME:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int v   = 0;
                    int ny1 = 0;
                    for (; v < *vertCnt && v < 0xFF; ++v) {
                        int vertZ = drawVert[v].z;
                        if (vertZ < 0x100) {
                            v = 0xFF;
                        }
                        else {
                            vertPos[v].x = currentScreen->center.x + (drawVert[v].x << scn->projectionX) / vertZ;
                            vertPos[v].y = currentScreen->center.y - (drawVert[v].y << scn->projectionY) / vertZ;
                            ny1 += drawVert[v].ny;
                        }
                    }

                    if (v < 0xFF) {
                        int normal    = ny1 / vertCount;
                        int normalVal = (normal >> 2) * (abs(normal) >> 2);

                        int specular = normalVal >> 6 >> scn->specularIntensityX;
                        specular     = minVal(0xFF, specular);
                        specular     = maxVal(0, specular);
                        int clrR = specular + ((int)((drawVert[0].colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX) >> scn->diffuseIntensityX);

                        specular = normalVal >> 6 >> scn->specularIntensityY;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0, specular);
                        int clrG = specular + ((int)((drawVert[0].colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY) >> scn->diffuseIntensityY);

                        specular = normalVal >> 6 >> scn->specularIntensityZ;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0, specular);
                        int clrB = specular + ((int)((drawVert[0].colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ) >> scn->diffuseIntensityZ);

                        clrR = minVal(0xFF, clrR);
                        clrR = maxVal(0, clrR);
                        clrG = minVal(0xFF, clrG);
                        clrG = maxVal(0, clrG);
                        clrB = minVal(0xFF, clrB);
                        clrB = maxVal(0, clrB);

                        uint colour = (clrR << 16) | (clrG << 8) | (clrB << 0);

                        drawVert = &scn->vertices[scn->faceBuffer[i].index];
                        for (int v = 0; v < *vertCnt - 1; ++v) {
                            DrawLine(vertPos[v + 0].x, vertPos[v + 0].y, vertPos[v + 1].x, vertPos[v + 1].y, colour, entity->alpha,
                                     (InkEffects)entity->inkEffect, true);
                        }
                        DrawLine(vertPos[*vertCnt - 1].x, vertPos[*vertCnt - 1].y, vertPos[0].x, vertPos[0].y, colour, entity->alpha,
                                 (InkEffects)entity->inkEffect, true);
                    }

                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SHADED_SCREEN:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int v  = 0;
                    int ny = 0;
                    for (; v < vertCount && v < 0xFF; ++v) {
                        int vertZ = drawVert[v].z;
                        if (vertZ < 0x100) {
                            v = 0xFF;
                        }
                        else {
                            vertPos[v].x = (currentScreen->center.x << 16) + ((drawVert[v].x << scn->projectionX) / vertZ << 16);
                            vertPos[v].y = (currentScreen->center.y << 16) - ((drawVert[v].y << scn->projectionY) / vertZ << 16);
                            ny += drawVert[v].ny;
                        }
                    }

                    if (v < 0xFF) {
                        int normal    = ny / vertCount;
                        int normalVal = (normal >> 2) * (abs(normal) >> 2);

                        int specular = normalVal >> 6 >> scn->specularIntensityX;
                        specular     = minVal(0xFF, specular);
                        specular     = maxVal(0, specular);
                        int clrR = specular + ((int)((drawVert[0].colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX) >> scn->diffuseIntensityX);

                        specular = normalVal >> 6 >> scn->specularIntensityY;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0, specular);
                        int clrG = specular + ((int)((drawVert[0].colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY) >> scn->diffuseIntensityY);

                        specular = normalVal >> 6 >> scn->specularIntensityZ;
                        specular = minVal(0xFF, specular);
                        specular = maxVal(0, specular);
                        int clrB = specular + ((int)((drawVert[0].colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ) >> scn->diffuseIntensityZ);

                        clrR = minVal(0xFF, clrR);
                        clrR = maxVal(0, clrR);
                        clrG = minVal(0xFF, clrG);
                        clrG = maxVal(0, clrG);
                        clrB = minVal(0xFF, clrB);
                        clrB = maxVal(0, clrB);

                        uint colour = (clrR << 16) | (clrG << 8) | (clrB << 0);

                        drawVert = &scn->vertices[scn->faceBuffer[i].index];
                        DrawFace(vertPos, *vertCnt, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, (colour >> 0) & 0xFF, entity->alpha,
                                 (InkEffects)entity->inkEffect);
                    }

                    vertCnt++;
                }
                break;
            case S3D_FLATCLR_SHADED_BLENDED_SCREEN:
                for (int i = 0; i < scn->faceCount; ++i) {
                    Scene3DVertex *drawVert = &scn->vertices[scn->faceBuffer[i].index];
                    int vertCount           = *vertCnt;

                    int v = 0;
                    for (; v < vertCount && v < 0xFF; ++v) {
                        int vertZ = drawVert[v].z;
                        if (vertZ < 0x100) {
                            v = 0xFF;
                        }
                        else {
                            vertPos[v].x = (currentScreen->center.x << 16) + ((drawVert[v].x << scn->projectionX) / vertZ << 16);
                            vertPos[v].y = (currentScreen->center.y << 16) - ((drawVert[v].y << scn->projectionY) / vertZ << 16);

                            int normal    = drawVert[v].ny;
                            int normalVal = (normal >> 2) * (abs(normal) >> 2);

                            int specular = normalVal >> 6 >> scn->specularIntensityX;
                            specular     = minVal(0xFF, specular);
                            specular     = maxVal(0, specular);
                            int clrR =
                                specular + ((int)((drawVert[v].colour >> 16) & 0xFF) * ((normal >> 10) + scn->diffuseX) >> scn->diffuseIntensityX);

                            specular = normalVal >> 6 >> scn->specularIntensityY;
                            specular = minVal(0xFF, specular);
                            specular = maxVal(0, specular);
                            int clrG =
                                specular + ((int)((drawVert[v].colour >> 8) & 0xFF) * ((normal >> 10) + scn->diffuseY) >> scn->diffuseIntensityY);

                            specular = normalVal >> 6 >> scn->specularIntensityZ;
                            specular = minVal(0xFF, specular);
                            specular = maxVal(0, specular);
                            int clrB =
                                specular + ((int)((drawVert[v].colour >> 0) & 0xFF) * ((normal >> 10) + scn->diffuseZ) >> scn->diffuseIntensityZ);

                            clrR = minVal(0xFF, clrR);
                            clrR = maxVal(0, clrR);
                            clrG = minVal(0xFF, clrG);
                            clrG = maxVal(0, clrG);
                            clrB = minVal(0xFF, clrB);
                            clrB = maxVal(0, clrB);

                            vertClrs[v] = (clrR << 16) | (clrG << 8) | (clrB << 0);
                        }
                    }

                    if (v < 0xFF) {
                        drawVert = &scn->vertices[scn->faceBuffer[i].index];
                        DrawBlendedFace(vertPos, vertClrs, *vertCnt, entity->alpha, (InkEffects)entity->inkEffect);
                    }

                    vertCnt++;
                }
                break;
        }
    }
}

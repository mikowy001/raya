#include "./universe.h"
#include <raymath.h>

void atomPhysics() {
	float dt = GetFrameTime();

    rlEnableShader(shaderCompute.id);
    SetShaderValue(shaderCompute, dtLoc, &dt, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shaderCompute, countLoc, &atomCount, SHADER_UNIFORM_INT);
        
    rlBindShaderBuffer(ssbo, 0);
    rlComputeShaderDispatch((atomCount / 64) + 1, 1, 1);
    rlDisableShader();

    rlReadShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);
}

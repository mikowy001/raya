#include "./universe.h"

void atomPhysics() {
  if (atomCount == 0)
    return;

  float dt = GetFrameTime();

  rlEnableShader(shaderCompute.id);

  rlUpdateShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);

  SetShaderValue(shaderCompute, dtLoc, &dt, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shaderCompute, countLoc, &atomCount, SHADER_UNIFORM_INT);

  // TraceLog(LOG_INFO, "dtLock:%d      countLoc:%d",
  // GetShaderLocation(shaderCompute, &dtLoc), GetShaderLocation(shaderCompute,
  // &countLoc));

  rlBindShaderBuffer(ssbo, 0);
  rlComputeShaderDispatch((atomCount / 64) + 1, 1, 1);
  rlReadShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);

  rlDisableShader();

  // TraceLog(LOG_INFO, "shader id: %d", shaderCompute.id);
}

void updateSSBO() {
  rlUnloadShaderBuffer(ssbo);
  ssbo =
      rlLoadShaderBuffer(sizeof(atom) * atomCount, atomsList, RL_DYNAMIC_COPY);
  TraceLog(LOG_INFO, "realocated ssbo !!!!!!!!!!!!!!!!!!!!!!!");
}

void initComputeShader() {
  char *code = LoadFileText("src/compute.glsl");
  if (code != NULL) {
    int shaderData = rlCompileShader(code, RL_COMPUTE_SHADER);

    shaderCompute.id = rlLoadComputeShaderProgram(shaderData);

    UnloadFileText(code);

    if (shaderCompute.id == 0) {
      TraceLog(LOG_ERROR, "Shader error !!!");
    } else {
      TraceLog(LOG_INFO, "[INFO] COMPUTE SHADER LOADED: ID %d",
               shaderCompute.id);
    }
  }
}

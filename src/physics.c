#include "./functions.h"
#include "./coredata.h"

void atomPhysics() {
	if(DATA.Atoms.atomsCount == 0)
		return;

	float dt = GetFrameTime();

	rlEnableShader(DATA.Shaders.shaderCompute.id);

	rlUpdateShaderBuffer(DATA.Shaders.ssbo, DATA.Atoms.atomsList, sizeof(atom) * DATA.Atoms.atomsCount, 0);

	SetShaderValue(DATA.Shaders.shaderCompute, DATA.Shaders.dtLoc, &dt, SHADER_UNIFORM_FLOAT);
	SetShaderValue(DATA.Shaders.shaderCompute, DATA.Shaders.countLoc, &DATA.Atoms.atomsCount, SHADER_UNIFORM_INT);

	// TraceLog(LOG_INFO, "dtLock:%d			countLoc:%d",
	// GetShaderLocation(shaderCompute, &dtLoc), GetShaderLocation(shaderCompute,
	// &countLoc));

	rlBindShaderBuffer(DATA.Shaders.ssbo, 0);
	rlComputeShaderDispatch((DATA.Atoms.atomsCount / 64) + 1, 1, 1);
	rlReadShaderBuffer(DATA.Shaders.ssbo, DATA.Atoms.atomsList, sizeof(atom) * DATA.Atoms.atomsCount, 0);

	rlDisableShader();

	// TraceLog(LOG_INFO, "shader id: %d", shaderCompute.id);
}

void updateSSBO() {
	rlUnloadShaderBuffer(DATA.Shaders.ssbo);
	DATA.Shaders.ssbo =
			rlLoadShaderBuffer(sizeof(atom) * DATA.Atoms.atomsCount, DATA.Atoms.atomsList, RL_DYNAMIC_COPY);
	TraceLog(LOG_INFO, "realocated ssbo !!!!!!!!!!!!!!!!!!!!!!!");
}

void initComputeShader() {
	char *code = LoadFileText("src/compute.glsl");
	if (code != NULL) {
		int shaderData = rlCompileShader(code, RL_COMPUTE_SHADER);

		DATA.Shaders.shaderCompute.id = rlLoadComputeShaderProgram(shaderData);

		UnloadFileText(code);

		if (DATA.Shaders.shaderCompute.id == 0) {
			TraceLog(LOG_ERROR, "Shader error !!!");
		} else {
			TraceLog(LOG_INFO, "[INFO] COMPUTE SHADER LOADED: ID %d",
							 DATA.Shaders.shaderCompute.id);
		}
	}
}

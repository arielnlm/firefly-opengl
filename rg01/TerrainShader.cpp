#include "TerrainShader.h"

void TerrainShader::loadShineVariables(float damper, float reflectivity) {
	setFloat("shineDamper", damper);
	setFloat("reflectivity", reflectivity);
}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix) {
	setMat4("transformationMatrix", matrix);
}

void TerrainShader::loadLight(Light light) {
	setVec3("lightColor", light.getColor());
	setVec3("lightPosition", light.getPosition());
}

void TerrainShader::loadViewMatrix(PlayerCamera camera) {
	setMat4("viewMatrix", Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition()));
}

void TerrainShader::loadProjectionMatrix(glm::mat4 projectionMatrix) {
	setMat4("projectionMatrix", projectionMatrix);
}

void TerrainShader::loadSkyColor(float r, float g, float b) {
	setVec3("skyColor", r, g, b);
}

void TerrainShader::connectTextureUnits() {
	setInt("backgroundTexture", 0);
	setInt("rTexture", 1);
	setInt("gTexture", 2);
	setInt("bTexture", 3);
	setInt("blendMap", 4);
}

void TerrainShader::loadLights(std::vector<Light> lights)
{
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			setVec3("lightPosition[" + std::to_string(i) + "]", lights[i].getPosition());
			setVec3("lightColor[" + std::to_string(i) + "]", lights[i].getColor());
			setVec3("attenuation[" + std::to_string(i) + "]", lights[i].getAttenuation());
		}
		else {
			setVec3("lightPosition[" + std::to_string(i) + "]", glm::vec3(0, 0, 0));
			setVec3("lightColor[" + std::to_string(i) + "]", glm::vec3(0, 0, 0));
			setVec3("attenuation[" + std::to_string(i) + "]", glm::vec3(0, 0, 0));
		}
	}
}

void TerrainShader::loadClipPlane(glm::vec4 plane)
{
	setVec4("plane", plane);
}

void TerrainShader::bindAttributes() {
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoordinates");
	Shader::bindAttribute(2, "normal");
}
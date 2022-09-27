#include "StaticShader.h"

void StaticShader::loadShineVariables(float damper, float reflectivity) {
	setFloat("shineDamper", damper);
	setFloat("reflectivity", reflectivity);
}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix) {
	setMat4("transformationMatrix", matrix);
}

void StaticShader::loadLights(std::vector<Light> lights) {
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			setVec3("lightPosition[" + std::to_string(i) + "]", lights[i].getPosition());
			setVec3("lightColor[" + std::to_string(i) + "]", lights[i].getColor());
			setVec3("attenuation[" + std::to_string(i) + "]", lights[i].getAttenuation());
		}
		else {
			setVec3("lightPosition[" + std::to_string(i) + "]", glm::vec3(0, 0, 0));
			setVec3("lightColor[" + std::to_string(i) + "]", glm::vec3(0, 0, 0));
			setVec3("attenuation[" + std::to_string(i) + "]", glm::vec3(1, 0, 0));
		}
	}
}

void StaticShader::loadLight(Light light) {
	setVec3("lightPosition", light.getPosition());
	setVec3("lightColor", light.getColor());
}

void StaticShader::loadViewMatrix(PlayerCamera camera) {
	setMat4("viewMatrix", Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition()));
}

void StaticShader::loadProjectionMatrix(glm::mat4 projectionMatrix) {
	setMat4("projectionMatrix", projectionMatrix);
}

void StaticShader::loadFakeLighting(bool useFake) {
	setBool("useFakeLighting", useFake);
}
void StaticShader::loadSkyColor(float r, float g, float b) {
	setVec3("skyColor", r, g, b);
}
void StaticShader::loadNumberOfRows(int numberOfRows)
{
	setFloat("numberOfRows", numberOfRows);
}
void StaticShader::loadOffset(float offsetX, float offsetY)
{
	setVec2("offset", offsetX, offsetY);
}

void StaticShader::loadClipPlane(glm::vec4 plane)
{
	setVec4("plane", plane);
}

void StaticShader::bindAttributes() {
	Shader::bindAttribute(0, "position");
	Shader::bindAttribute(1, "textureCoordinates");
	Shader::bindAttribute(2, "normal");
}
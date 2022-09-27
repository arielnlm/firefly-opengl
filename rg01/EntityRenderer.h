#ifndef H_ENTITY_RENDERER
#define H_ENTITY_RENDERER

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "RawModel.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "Shader.h"
#include "Maths.h"
#include <map>
#include <vector>
#include "StaticShader.h"
#include "MasterRendererStatic.h"

/*
	Prepares and renders entities.
*/
class EntityRenderer {

private:

	StaticShader shader;

public:

	EntityRenderer(StaticShader shader, glm::mat4 projectionMatrix) {
		this->shader = shader;
		shader.use();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
	}

	void render(std::map<TexturedModel*, vector<Entity*>> entities) {
		for (const auto& p : entities) {
			prepareTexturedModel(p.first);
			vector<Entity*> batch = p.second;
			for (const auto& entity : batch) {
				prepareInstance(entity);
				glDrawElements(GL_TRIANGLES, p.first->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
			}
				unbindTexturedModel();
		}
	}
private:

	void prepareTexturedModel(TexturedModel* model) {
		RawModel* rawModel = model->getRawModel();
		glBindVertexArray(rawModel->getVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		ModelTexture* texture = model->getTexture();
		shader.loadNumberOfRows(texture->getNumberOfRows());
		if (texture->isHasTransparency()) {
			MasterRendererStatic::disableCulling();
		}
		shader.loadFakeLighting(texture->isUseFakeLighting());
		shader.loadShineVariables(texture->getShineDamper(), texture->getReflectivity());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}

	void unbindTexturedModel() {
		MasterRendererStatic::enableCulling();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void prepareInstance(Entity* entity) {
		glm::mat4 transformationMatrix = Maths::createTransformationMatrix(entity->getPosition(),
			entity->getRotX(), entity->getRotY(), entity->getRotZ(),
			entity->getScale());

		shader.loadTransformationMatrix(transformationMatrix);
		shader.loadOffset(entity->getTextureXOffset(), entity->getTextureYOffset());
	}
};

#endif
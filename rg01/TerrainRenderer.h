#ifndef H_TERRAIN_RENDERER
#define H_TERRAIN_RENDERER

#pragma once
#include "TerrainShader.h"
#include "Terrain.h"

/*
	Prepares and renders terrain
*/
class TerrainRenderer {
private:
	TerrainShader shader;

public:
	TerrainRenderer(TerrainShader shader, glm::mat4 projectionMatrix) {
		this->shader = shader;
		shader.use();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.connectTextureUnits();
		shader.stop();
	}

	void render(vector<Terrain> terrains) {
		for (auto& terrain : terrains) {
			prepareTerrain(terrain);
			loadModelMatrix(terrain);
			glDrawElements(GL_TRIANGLES, terrain.getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
			unbindTexturedModel();
		}
	}

private:

	void prepareTerrain(Terrain terrain) {
		RawModel* model = terrain.getModel();
		glBindVertexArray(model->getVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		bindTextures(terrain);
		shader.loadShineVariables(1, 0);

	}

	void bindTextures(Terrain terrain) {
		TerrainTexturePack texturePack = terrain.getTexturePack();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBackgroundTexture().getTextureID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturePack.getRTexture().getTextureID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texturePack.getGTexture().getTextureID());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texturePack.getBTexture().getTextureID());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, terrain.getBlendMap().getTextureID());
	}

	void unbindTexturedModel() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void loadModelMatrix(Terrain terrain) {
		glm::mat4 transformationMatrix = Maths::createTransformationMatrix(
			glm::vec3(terrain.getX(), 0, terrain.getZ()), 0, 0, 0, 1);

		shader.setMat4("transformationMatrix", transformationMatrix);
	}
};

#endif
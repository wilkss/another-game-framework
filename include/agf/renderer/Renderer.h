#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

namespace agf {

	class Window;
	class Framebuffer;
	class Mesh;
	class ShaderProgram;
	class Texture;
	class Scene;
	class Material;

	class Renderer
	{
	public:
		Renderer(Window* window, Scene* scene);
		~Renderer();

		void Render();
		void RenderDirectionalLightShadowDepthMaps();
		void RenderDirectionalLights();
		void RenderPointLightShadowDepthMaps();
		void RenderPointLights();
		
	private:
		Window* m_Window;
		Scene* m_Scene;

		Mesh* m_ScreenSpaceQuadMesh;
		ShaderProgram* m_CompositeShader;
		ShaderProgram* m_DirectionalLightShader;
		ShaderProgram* m_PointLightShader;
		ShaderProgram* m_GeometryShader;
		ShaderProgram* m_OmnidirectionalShadowShader;
		ShaderProgram* m_DirectionalShadowShader;

		Texture* m_ColorTexture;
		Texture* m_NormalTexture;
		Texture* m_LightAccumulationTexture;
		Texture* m_DepthTexture;
		Framebuffer* m_GeometryBuffer;

		Texture* m_CheckerboardAlbedoMap;
		Material* m_DefaultMaterial;
	};

}
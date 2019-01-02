#include "agf/renderer/Renderer.h"
#include "agf/Entity.h"
#include "agf/Window.h"
#include "agf/Scene.h"
#include "agf/renderer/Material.h"
#include "agf/renderer/Camera.h"
#include "agf/renderer/Mesh.h"
#include "agf/renderer/ShaderProgram.h"
#include "agf/renderer/MeshFactory.h"
#include "agf/renderer/MeshRenderer.h"
#include "agf/renderer/Framebuffer.h"
#include "agf/renderer/Texture.h"
#include "agf/renderer/light/DirectionalLight.h"
#include "agf/renderer/light/PointLight.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace agf {

	Renderer::Renderer(Window* window, Scene* scene)
		: m_Window(window), m_Scene(scene)
	{
		m_ScreenSpaceQuadMesh = MeshFactory::CreateScreenSpaceQuadMesh();
		m_ColorTexture = new Texture(m_Window->GetWidth(), m_Window->GetHeight(), GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		m_NormalTexture = new Texture(m_Window->GetWidth(), m_Window->GetHeight(), GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		m_LightAccumulationTexture = new Texture(m_Window->GetWidth(), m_Window->GetHeight(), GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		m_DepthTexture = new Texture(m_Window->GetWidth(), m_Window->GetHeight(), GL_TEXTURE_2D, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		m_GeometryBuffer = new Framebuffer(m_Window->GetWidth(), m_Window->GetHeight(), { m_ColorTexture, m_NormalTexture, m_LightAccumulationTexture }, m_DepthTexture);

		m_CompositeShader = new ShaderProgram("media/shaders/ScreenSpaceQuad.vertex", "media/shaders/Composite.fragment");
		m_DirectionalLightShader = new ShaderProgram("media/shaders/ScreenSpaceQuad.vertex", "media/shaders/DirectionalLight.fragment");
		m_PointLightShader = new ShaderProgram("media/shaders/ScreenSpaceQuad.vertex", "media/shaders/PointLight.fragment");
		m_GeometryShader = new ShaderProgram("media/shaders/Model.vertex", "media/shaders/Geometry.fragment");
		m_DirectionalShadowShader = new ShaderProgram("media/shaders/Model.vertex", "media/shaders/Empty.fragment");
		m_OmnidirectionalShadowShader = new ShaderProgram("media/shaders/OmnidirectionalShadow.vertex", "media/shaders/OmnidirectionalShadow.fragment", "media/shaders/OmnidirectionalShadow.geometry");

		m_DefaultMaterial = new agf::Material();
		m_CheckerboardAlbedoMap = Texture::LoadTexture("media/textures/checkerboard.png");
		m_DefaultMaterial->SetAlbedoMap(m_CheckerboardAlbedoMap);
	}

	Renderer::~Renderer()
	{
		delete m_GeometryBuffer;
		delete m_ScreenSpaceQuadMesh;

		delete m_ColorTexture;
		delete m_NormalTexture;
		delete m_LightAccumulationTexture;
		delete m_DepthTexture;

		delete m_CompositeShader;
		delete m_DirectionalLightShader;
		delete m_PointLightShader;
		delete m_GeometryShader;
		delete m_DirectionalShadowShader;
		delete m_OmnidirectionalShadowShader;

		delete m_DefaultMaterial;
		delete m_CheckerboardAlbedoMap;
	}

	void Renderer::Render()
	{
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		RenderDirectionalLightShadowDepthMaps();
		RenderPointLightShadowDepthMaps();

		m_GeometryBuffer->Bind();
		{
			GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, drawBuffers);

			m_GeometryShader->Use();
			m_GeometryShader->SetMat4("u_ProjectionMatrix", m_Scene->GetMainCamera()->GetProjectionMatrix());
			m_GeometryShader->SetMat4("u_ViewMatrix", m_Scene->GetMainCamera()->GetViewMatrix());

			for (auto* meshRenderer : m_Scene->GetComponentsOfType<MeshRenderer>())
			{
				m_GeometryShader->SetVec3("u_Albedo", m_DefaultMaterial->GetAlbedo());
				m_GeometryShader->SetBool("u_HasAlbedoMap", m_DefaultMaterial->HasAlbedoMap());

				if (m_DefaultMaterial->HasAlbedoMap())
				{
					m_GeometryShader->SetInt("u_AlbedoMap", 0);
					m_DefaultMaterial->GetAlbedoMap()->Bind(0);
				}

				meshRenderer->Render(m_GeometryShader);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT2);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			RenderDirectionalLights();
			RenderPointLights();

			glDisable(GL_BLEND);
		}
		m_GeometryBuffer->Unbind();

		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
		m_CompositeShader->Use();
		m_CompositeShader->SetTexture("u_ColorTexture", m_ColorTexture, 0);
		m_CompositeShader->SetTexture("u_LightAccumulationTexture", m_LightAccumulationTexture, 1);
		m_CompositeShader->SetTexture("u_DepthTexture", m_DepthTexture, 2);
		m_ScreenSpaceQuadMesh->Render();
	}

	void Renderer::RenderDirectionalLightShadowDepthMaps()
	{
		m_DirectionalShadowShader->Use();

		for (auto* light : m_Scene->GetComponentsOfType<DirectionalLight>())
		{
			for (unsigned i = 0; i < light->GetShadowCascadeCount(); ++i)
			{
				Framebuffer* shadowMapFramebuffer = light->GetShadowMapFramebuffer(i);
				if (!shadowMapFramebuffer)
					continue;

				m_DirectionalShadowShader->SetMat4("u_ProjectionMatrix", light->GetShadowProjectionMatrix(i));
				m_DirectionalShadowShader->SetMat4("u_ViewMatrix", light->GetShadowViewMatrix(i));
				shadowMapFramebuffer->Bind();

				for (auto* meshRenderer : m_Scene->GetComponentsOfType<MeshRenderer>())
					meshRenderer->Render(m_DirectionalShadowShader);

				shadowMapFramebuffer->Unbind();
			}
		}
	}

	void Renderer::RenderDirectionalLights()
	{
		m_DirectionalLightShader->Use();
		m_DirectionalLightShader->SetTexture("u_NormalTexture", m_NormalTexture, 0);
		m_DirectionalLightShader->SetTexture("u_DepthTexture", m_DepthTexture, 1);

		m_DirectionalLightShader->SetMat4("u_ViewMatrix", m_Scene->GetMainCamera()->GetViewMatrix());
		m_DirectionalLightShader->SetMat4("u_InverseViewProjectionMatrix", glm::inverse(m_Scene->GetMainCamera()->GetProjectionMatrix() * m_Scene->GetMainCamera()->GetViewMatrix()));

		for (auto* light : m_Scene->GetComponentsOfType<DirectionalLight>())
		{
			m_DirectionalLightShader->SetTexture("u_ShadowMapTexture", light->GetShadowMapTexture(), 2);
			m_DirectionalLightShader->SetVec3("u_LightDirection", light->GetParent()->GetRotation());
			m_DirectionalLightShader->SetVec3("u_LightColor", light->GetColor());
			m_DirectionalLightShader->SetInt("u_CascadeCount", light->GetShadowCascadeCount());

			for (unsigned i = 0; i < light->GetShadowCascadeCount(); i++)
			{
				m_DirectionalLightShader->SetFloat("u_CascadeSplitDepths[" + std::to_string(i) + "]", light->GetCascadeSplitDepth(i));
				m_DirectionalLightShader->SetMat4("u_CascadeSplitMatrices[" + std::to_string(i) + "]", light->GetShadowProjectionMatrix(i) * light->GetShadowViewMatrix(i));
			}

			m_ScreenSpaceQuadMesh->Render();
		}
	}

	void Renderer::RenderPointLightShadowDepthMaps()
	{
		m_OmnidirectionalShadowShader->Use();

		for (auto* light : m_Scene->GetComponentsOfType<PointLight>())
		{
			Framebuffer* shadowMapFramebuffer = light->GetShadowFramebuffer();
			if (!shadowMapFramebuffer)
				continue;

			shadowMapFramebuffer->Bind();
			{
				m_OmnidirectionalShadowShader->SetMat4("u_ProjectionMatrix", light->GetShadowMapProjectionMatrix());
				for (unsigned i = 0; i < 6; ++i)
					m_OmnidirectionalShadowShader->SetMat4("u_ViewMatrices[" + std::to_string(i) + "]", light->GetShadowMapViewMatrix(i));
				m_OmnidirectionalShadowShader->SetFloat("u_ShadowDistance", light->GetShadowDistance());
				m_OmnidirectionalShadowShader->SetVec3("u_LightPosition", light->GetParent()->GetPosition());

				for (auto* meshRenderer : m_Scene->GetComponentsOfType<MeshRenderer>())
					meshRenderer->Render(m_OmnidirectionalShadowShader);
			}
			shadowMapFramebuffer->Unbind();
		}
	}

	void Renderer::RenderPointLights()
	{
		m_PointLightShader->Use();
		m_PointLightShader->SetTexture("u_NormalTexture", m_NormalTexture, 0);
		m_PointLightShader->SetTexture("u_DepthTexture", m_DepthTexture, 1);

		m_PointLightShader->SetMat4("u_InverseViewProjectionMatrix", glm::inverse(m_Scene->GetMainCamera()->GetProjectionMatrix() * m_Scene->GetMainCamera()->GetViewMatrix()));

		for (auto* light : m_Scene->GetComponentsOfType<PointLight>())
		{
			m_PointLightShader->SetTexture("u_ShadowMapTexture", light->GetShadowMapTexture(), 2);
			m_PointLightShader->SetVec3("u_LightPosition", light->GetParent()->GetPosition());
			m_PointLightShader->SetVec3("u_LightColor", light->GetColor());
			const std::string& lightAttenuationUniformPrefix = "u_Attenuation.";
			m_PointLightShader->SetFloat(lightAttenuationUniformPrefix + "constant", light->GetAttenuation().constant);
			m_PointLightShader->SetFloat(lightAttenuationUniformPrefix + "linear", light->GetAttenuation().linear);
			m_PointLightShader->SetFloat(lightAttenuationUniformPrefix + "quadratic", light->GetAttenuation().quadratic);
			m_PointLightShader->SetFloat("u_ShadowDistance", light->GetShadowDistance());
			m_ScreenSpaceQuadMesh->Render();
		}
	}

}
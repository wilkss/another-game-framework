#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace agf {

	class Texture;

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath = "");
		~ShaderProgram();

		unsigned CompileShader(const std::string& source, unsigned type) const;

		void Use() const;

		void SetInt(const std::string& uniformName, int value);
		void SetBool(const std::string& uniformName, bool value);
		void SetFloat(const std::string& uniformName, float value);
		void SetVec3(const std::string& uniformName, const glm::vec3& value);
		void SetVec4(const std::string& uniformName, const glm::vec4& value);
		void SetMat3(const std::string& uniformName, const glm::mat3& value);
		void SetMat4(const std::string& uniformName, const glm::mat4& value);
		void SetTexture(const std::string& uniformName, Texture* texture, int slot = 0);
		unsigned GetUniformLocation(const std::string& uniformName);

	private:
		unsigned m_Handle;
		std::unordered_map<std::string, unsigned> m_UniformLocations;
	};

}
#include "agf/renderer/ShaderProgram.h"
#include "agf/renderer/Texture.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>

namespace agf {

	ShaderProgram::ShaderProgram(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath)
	{
		m_Handle = glCreateProgram();

		unsigned vertexShader = CompileShader(vertexFilePath, GL_VERTEX_SHADER);
		unsigned fragmentShader = CompileShader(fragmentFilePath, GL_FRAGMENT_SHADER);
		glAttachShader(m_Handle, vertexShader);
		glAttachShader(m_Handle, fragmentShader);

		unsigned geometryShader = 0;
		if (!geometryFilePath.empty())
		{
			geometryShader = CompileShader(geometryFilePath, GL_GEOMETRY_SHADER);
			glAttachShader(m_Handle, geometryShader);
		}

		glLinkProgram(m_Handle);
		glValidateProgram(m_Handle);
		int result;
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetProgramInfoLog(m_Handle, length, &length, message);
			std::cout << "Failed to link shader program" << std::endl;
			std::cout << message << std::endl;
			delete[] message;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (!geometryFilePath.empty())
			glDeleteShader(geometryShader);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_Handle);
	}

	unsigned ShaderProgram::CompileShader(const std::string& path, unsigned type) const
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "Failed to open shader: " << path << std::endl;
			return 0;
		}

		std::string source;
		std::string line;
		while (std::getline(file, line))
			source += line + '\n';

		unsigned handle = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(handle, 1, &src, nullptr);
		glCompileShader(handle);

		int result;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(handle, length, &length, message);
			glDeleteShader(handle);
			std::cout << "Failed to compile shader: " << path << std::endl;
			std::cout << message << std::endl;
			delete[] message;
			return 0;
		}

		return handle;
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(m_Handle);
	}

	void ShaderProgram::SetInt(const std::string& uniformName, int value)
	{
		glUniform1i(GetUniformLocation(uniformName), value);
	}

	void ShaderProgram::SetBool(const std::string& uniformName, bool value)
	{
		glUniform1i(GetUniformLocation(uniformName), value);
	}

	void ShaderProgram::SetFloat(const std::string& uniformName, float value)
	{
		glUniform1f(GetUniformLocation(uniformName), value);
	}

	void ShaderProgram::SetVec3(const std::string& uniformName, const glm::vec3& value)
	{
		glUniform3fv(GetUniformLocation(uniformName), 1, &value[0]);
	}

	void ShaderProgram::SetVec4(const std::string& uniformName, const glm::vec4& value)
	{
		glUniform4fv(GetUniformLocation(uniformName), 1, &value[0]);
	}

	void ShaderProgram::SetMat3(const std::string& uniformName, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(uniformName), 1, false, &value[0][0]);
	}

	void ShaderProgram::SetMat4(const std::string& uniformName, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, &value[0][0]);
	}

	void ShaderProgram::SetTexture(const std::string& uniformName, Texture* texture, int slot)
	{
		glUniform1i(GetUniformLocation(uniformName), slot);
		texture->Bind(slot);
	}

	unsigned ShaderProgram::GetUniformLocation(const std::string& uniformName)
	{
		auto it = m_UniformLocations.find(uniformName);
		if (it != m_UniformLocations.end())
			return m_UniformLocations[uniformName];

		unsigned uniformLocation = glGetUniformLocation(m_Handle, uniformName.c_str());
		if (uniformLocation == -1)
			std::cout << "Warning: Uniform " << uniformName << " does not exist!" << std::endl;
		m_UniformLocations[uniformName] = uniformLocation;
		return uniformLocation;
	}

}
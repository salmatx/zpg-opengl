#pragma once

#include "i_bind.h"

namespace engine {

class IShader : public IBind {
  public:
    virtual ~IShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) = 0;
	virtual void SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) = 0;
};

}
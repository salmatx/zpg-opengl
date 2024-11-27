#pragma once

#include <light.h>

namespace engine {

class DirectionalLight : public Light {
public:
	explicit DirectionalLight(const DirectionalLightParams_t& t_params);

	void Notify() override;
	void InitLight() override;

private:
	DirectionalLightParams_t m_params {};
};
}


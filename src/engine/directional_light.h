#pragma once

#include <light.h>

namespace engine {

class DirectionalLight : public Light {
public:
	explicit DirectionalLight(const DirectionalLightParams_t& t_params);

	void Notify() override;
	void InitLight() override;

	[[nodiscard]] DirectionalLightParams_t GetParams() const {return m_params;}

private:
	DirectionalLightParams_t m_params {};
};
}


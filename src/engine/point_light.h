#pragma once

#include "light.h"

namespace engine {

class PointLight : public Light {
public:
	explicit PointLight(const PointLightParams_t& t_params);

	void Notify() override;
	void InitLight() override;

private:
	PointLightParams_t m_params {};
};

}

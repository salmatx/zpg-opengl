#pragma once

#include "light.h"

namespace engine {

class PointLight : public Light {
public:
	explicit PointLight(const PointLightParams_t& t_params);

	void Notify() override;
	void InitLight() override;

	[[nodiscard]] PointLightParams_t GetParams() const {return m_params;}

private:
	PointLightParams_t m_params {};
};

}

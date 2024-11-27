#include "directional_light.h"

namespace engine {
DirectionalLight::DirectionalLight(const DirectionalLightParams_t& t_params)
	: Light(LightType_t::Directional) {
	m_params.direction = t_params.direction;
	m_params.ambient = t_params.ambient;
	m_params.diffuse = t_params.diffuse;
	m_params.specular = t_params.specular;
}

void DirectionalLight::Notify() {
	auto observers = GetLightObservers();
	for (auto& observer : observers) {
		observer->Update(*this);
	}
}

void DirectionalLight::InitLight() {
	Notify();
}
}

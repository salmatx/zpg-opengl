#include "point_light.h"

namespace engine {
PointLight::PointLight(const PointLightParams_t& t_params)
	: Light(LightType_t::Point){
	m_params.position = t_params.position;
	m_params.constant = t_params.constant;
	m_params.linear = t_params.linear;
	m_params.quadratic = t_params.quadratic;
	m_params.ambient = t_params.ambient;
	m_params.diffuse = t_params.diffuse;
	m_params.specular = t_params.specular;
}

void PointLight::Notify() {
	auto observers = GetLightObservers();
	for (auto& observer : observers) {
		observer->Update(*this);
	}
}

void PointLight::InitLight() {
	Notify();
}
}

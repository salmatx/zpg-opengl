#include "flash_light.h"

namespace engine {
FlashLight::FlashLight(const FlashLightParams_t& t_params, const std::shared_ptr<Camera>& t_camera)
	: Light(LightType_t::Flashlight),
	m_camera(t_camera) {
	m_params.cut_off_angle = t_params.cut_off_angle;
	m_params.outer_cut_off_angle = t_params.outer_cut_off_angle;
	m_params.constant = t_params.constant;
	m_params.linear = t_params.linear;
	m_params.quadratic = t_params.quadratic;
	m_params.ambient = t_params.ambient;
	m_params.diffuse = t_params.diffuse;
	m_params.specular = t_params.specular;

	m_camera->Attach(this);
}

FlashLight::~FlashLight() {
	m_camera->Detach(this);
}

void FlashLight::Notify() {
	auto observers = GetLightObservers();
	for (auto& observer : observers) {
		observer->Update(*this, m_camera_params);
	}
}

void FlashLight::InitLight() {
	Notify();
}

void FlashLight::Update(const glm::mat4& t_projection, const glm::mat4& t_view, const glm::vec3& t_position,
	const glm::vec3& t_front) {
	m_camera_params.position = t_position;
	m_camera_params.direction = t_front;

	Notify();
}
}

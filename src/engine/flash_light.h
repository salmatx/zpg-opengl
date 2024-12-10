#pragma once

#include <camera.h>

#include "light.h"

namespace engine {

class FlashLight : public Light, public ICameraObserver, public IWindowObserver {
public:
	FlashLight(const FlashLightParams_t& t_params, const std::shared_ptr<Camera>& t_camera, Window& t_window);
	~FlashLight();

	void Notify() override;
	void InitLight() override;

	void Update(const glm::mat4& t_projection, const glm::mat4& t_view,
		const glm::vec3& t_position, const glm::vec3& t_front) override;
	void Update(EventType t_event, const std::shared_ptr<const void>& t_event_data) override;

	[[nodiscard]] FlashLightParams_t GetParams() const {return m_params;}

private:
	Window* m_window;
	FlashLightParams_t m_params {};
	CameraParams_t m_camera_params {};
	std::shared_ptr<Camera> m_camera;
};

}

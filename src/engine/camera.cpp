#include "camera.h"

namespace engine {
Camera::Camera(Window& t_window, int t_screen_width, int t_screen_height, CameraDepth t_depth, CameraPosition t_position)
	: m_window(&t_window),
	m_screen_width(t_screen_width),
	m_screen_height(t_screen_height),
	m_yaw(-90.0f),
	m_pitch(0.0f),
	m_last_x(static_cast<float>(t_screen_width) / 2.0f),
	m_last_y(static_cast<float>(t_screen_height) / 2.0f),
	m_fov(45.0f),
	m_first_mouse(true),
	m_near(t_depth.near),
	m_far(t_depth.far),
	m_camera_position(t_position.world_position),
	m_camera_front(t_position.view_front),
	m_camera_up(t_position.view_up) {
	m_projection = glm::perspective(glm::radians(m_fov),
		static_cast<float>(m_screen_width) / static_cast<float>(m_screen_height), m_near, m_far);
	m_view = glm::lookAt(m_camera_position, m_camera_position + m_camera_front, m_camera_up);

	m_window->Attach(EventType::key, this);
	m_window->Attach(EventType::mouse, this);
	m_window->Attach(EventType::scroll, this);
	m_window->Attach(EventType::screen_size, this);
}

Camera::~Camera() {
	m_window->Detach(EventType::key, this);
	m_window->Detach(EventType::mouse, this);
	m_window->Detach(EventType::scroll, this);
	m_window->Detach(EventType::screen_size, this);
}

void Camera::Attach(ICameraObserver* t_observer) {
	m_observers.emplace_back(t_observer);
}

void Camera::Detach(ICameraObserver* t_observer) {
	m_observers.remove(t_observer);
}

void Camera::Notify() {
	m_projection = glm::perspective(glm::radians(m_fov),
		static_cast<float>(m_screen_width) / static_cast<float>(m_screen_height), m_near, m_far);
	m_view = glm::lookAt(m_camera_position, m_camera_position + m_camera_front, m_camera_up);

	for (const auto& observer : m_observers) {
		observer->Update(m_projection, m_view, m_camera_position, m_camera_front);
	}
}

void Camera::Update(EventType t_event, std::shared_ptr<const void> t_event_data) {
	auto y_pos = m_camera_position.y;
	switch (t_event) {
	case EventType::key: {
		auto key_event = std::static_pointer_cast<const KeyEvent>(t_event_data);
		float camera_speed = 10.0f * key_event->time;
		if (key_event->key == GLFW_KEY_W)
			m_camera_position += camera_speed * m_camera_front;
		if (key_event->key == GLFW_KEY_S)
			m_camera_position -= camera_speed * m_camera_front;
		if (key_event->key == GLFW_KEY_A)
			m_camera_position -= glm::normalize(glm::cross(m_camera_front, m_camera_up)) * camera_speed;
		if (key_event->key == GLFW_KEY_D)
			m_camera_position += glm::normalize(glm::cross(m_camera_front, m_camera_up)) * camera_speed;

		m_camera_position.y = y_pos;
		break;
	}
	case EventType::mouse: {
		auto key_event = std::static_pointer_cast<const MouseEvent>(t_event_data);
		float x_pos = static_cast<float>(key_event->x_pos);
		float y_pos = static_cast<float>(key_event->y_pos);

		if (m_first_mouse) {
			m_last_x = x_pos;
			m_last_y = y_pos;
			m_first_mouse = false;
		}

		float x_offset = x_pos - m_last_x;
		float y_offset = m_last_y - y_pos;
		m_last_x = x_pos;
		m_last_y = y_pos;

		float sensitivity = 0.1f;
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		m_yaw += x_offset;
		m_pitch += y_offset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_camera_front = glm::normalize(front);

		break;
	}

	case EventType::scroll: {
		auto key_event = std::static_pointer_cast<const ScrollEvent>(t_event_data);
		m_fov -= static_cast<float>(key_event->y_offset);
		if (m_fov < 1.0f)
			m_fov = 1.0f;
		if (m_fov > 45.0f)
			m_fov = 45.0f;

		break;
	}

	case EventType::screen_size: {
		auto key_event = std::static_pointer_cast<const ScreenSizeEvent>(t_event_data);
		m_screen_width = key_event->width;
		m_screen_height = key_event->height;

		break;
	}

	default:
		break;
	}

	Notify();
}

void Camera::InitCamera() {
	glEnable(GL_DEPTH_TEST);
	Notify();
}

void Camera::RemoveObservation(EventType t_event) {
	m_window->Detach(t_event, this);
}
}

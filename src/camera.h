#pragma once

#include "i_camera_subject.h"
#include "i_window_observer.h"
#include "window.h"

namespace engine {

struct CameraDepth {
  float near;
  float far;
};

struct CameraPosition {
  glm::vec3 world_position;
  glm::vec3 view_front;
  glm::vec3 view_up;
};

class Camera : public ICameraSubject, public IWindowObserver {
public:
  Camera(Window& t_window, int t_screen_width, int t_screen_height, CameraDepth t_depth, CameraPosition t_position);
  ~Camera() override = default;

  void Attach(ICameraObserver* observer) override;
  void Detach(ICameraObserver* observer) override;
  void Notify() override;

  void Update(EventType t_event, std::shared_ptr<const void> t_event_data) override;

  void InitCamera();
  void RemoveObservation(EventType t_event);

private:
  Window& m_window;
  std::list<ICameraObserver*> m_observers;
  glm::mat4 m_projection;
  glm::mat4 m_view;
  glm::vec3 m_camera_position;
  glm::vec3 m_camera_front;
  glm::vec3 m_camera_up;
  int m_screen_width;
  int m_screen_height;
  float m_yaw;
  float m_pitch;
  float m_last_x;
  float m_last_y;
  float m_fov;
  bool m_first_mouse;
  float m_near;
  float m_far;
};

}

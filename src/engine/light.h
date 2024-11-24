#pragma once

#include <i_light_subject.h>

#include "epch.h"

namespace engine {

enum class LightType {
	Point,
	Directional,
	Spot,
	Area
};

struct LightParams {
	LightType type;
	glm::vec3 position; // Point or Spotlight position
	glm::vec3 direction; // For directional or spotlight
	glm::vec3 color;
	float intensity; // The intensity of light (affects all types)
	float cutoff;    // Spotlight cutoff
	float outer_cutoff; // Spotlight outer cutoff
};

class Light : public ILightSubject {
public:
	Light(const LightParams& t_params);

	void SetPosition(const glm::vec3& t_position);
	void SetColor(const glm::vec3& t_color);
	void SetDirection(const glm::vec3& t_direction);
	void SetCutoff(float t_cutoff);
	void SetOuterCutoff(float t_outer_cutoff);
	void SetIntensity(float t_intensity);
	void SetType(LightType t_type);

	const glm::vec3& GetPosition() const {return m_position;}
	const glm::vec3& GetColor() const {return m_color;}
	const glm::vec3& GetDirection() const {return m_direction;}
	float GetCutoff() const {return m_cutoff;}
	float GetOuterCutoff() const {return m_outer_cutoff;}
	float GetIntensity() const {return m_intensity;}
	LightType GetType() const {return m_type;}

	void InitLight();

	void Attach(ILightObserver* observer) override;
	void Detach(ILightObserver* observer) override;

private:
	void Notify() override;

	std::list<ILightObserver*> m_observers;
	LightType m_type;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_color;
	float m_intensity;
	float m_cutoff;
	float m_outer_cutoff;
};
}


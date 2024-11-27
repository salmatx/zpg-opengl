#include "directional_light.h"

namespace engine {
DirectionalLight::DirectionalLight(const DirectionalLightParams_t& t_params)
	: Light({t_params.position, t_params.color}) {}
}

#pragma once

#include "../../nclgl/SceneNode.h"

enum SolarType
{
	SOLAR_TYPE_SUN,
	SOLAR_TYPE_PLANET,
	SOLAR_TYPE_MOON,
	SOLAR_TYPE_RING
};

class SolarObject : public SceneNode
{
public:
	SolarObject(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1), SolarType t = SOLAR_TYPE_PLANET);
	~SolarObject(void);

protected:
	SolarType type;
};


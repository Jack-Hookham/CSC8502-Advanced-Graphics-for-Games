#pragma once

#include "Vector3.h"
#include "Vector4.h"

class Light
{
public:
	Light(Vector3 position = Vector3(), Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), float radius = 10.0f)
	{
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}

	~Light(void) {};

	inline Vector3 GetPosition() const { return position; }
	inline void SetPosition(Vector3 val) { position = val; }

	inline float GetRadius() const { return radius; }
	inline void SetRadius(float val) { radius = val; }

	inline Vector4 GetColour() const { return colour; }
	inline void SetColour(Vector4 val) { colour = val; }

	inline void SetR(float val) { colour.x = val; }
	inline void SetG(float val) { colour.y = val; }
	inline void SetB(float val) { colour.z = val; }
	inline void SetA(float val) { colour.w = val; }

protected:
	Vector3 position;
	Vector4 colour;
	//Vector3 specularColour;
	float radius;
};
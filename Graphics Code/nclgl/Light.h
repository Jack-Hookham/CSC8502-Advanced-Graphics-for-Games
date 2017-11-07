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

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	//Vector4 GetSpecularColour() const { return colour; }
	//void SetSpecularColour(Vector4 val) { colour = val; }


protected:
	Vector3 position;
	Vector4 colour;
	//Vector3 specularColour;
	float radius;
};
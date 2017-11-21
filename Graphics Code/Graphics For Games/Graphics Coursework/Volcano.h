#pragma once

#include "RenderObject.h"
#include "ParticleEmitter.h"
#include "../../nclgl/Camera.h"

class Volcano : public RenderObject
{
public:
	Volcano();
	~Volcano();

	//void Update(const float msec);

	//inline const bool getErupting() const { return erupting; }
	//inline void setErupting(const bool value) { erupting = value; }

protected:
	//bool erupting = false;

	//ParticleEmitter* lavaEmitter;
	//ParticleEmitter* emberEmitter;

};


#pragma once

#include "../../nclgl/Matrix4.h"
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector4.h"
#include "../../nclgl/Mesh.h"
#include <vector>

//Render types determine which shader is used for rendering
enum RenderType
{
	TYPE_NONE,
	TYPE_SUN,
	TYPE_SATELLITE		//planet/moon
};

class RenderObject
{
public:
	RenderObject(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1), RenderType t = RenderType::TYPE_NONE);
	~RenderObject(void);

	void SetTransform(const Matrix4 &matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	void SetSelfTransform(const Matrix4 &matrix) { selfTransform = matrix; }
	const Matrix4& GetSelfTransform() const { return selfTransform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	void AddChild(RenderObject* s);
	RenderObject* GetChild(int n) { return children[n]; }

	virtual void Update(float msec);
	virtual void Draw();
	virtual void DrawPatches();

	std::vector<RenderObject*>::const_iterator GetChildIteratorStart()
	{
		return children.begin();
	}

	std::vector<RenderObject*>::const_iterator GetChildIteratorEnd()
	{
		return children.end();
	}

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	static bool CompareByCameraDistance(RenderObject* a, RenderObject* b)
	{
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

	RenderType getType() const { return type; }

protected:
	RenderObject* parent;
	Mesh* mesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Matrix4 selfTransform;
	Vector3 modelScale;
	Vector4 colour;
	std::vector<RenderObject*> children;

	float distanceFromCamera;
	float boundingRadius;

	RenderType type;
};


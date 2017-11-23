#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Mesh.h"

//#define RAW_WIDTH 257
//#define RAW_HEIGHT 257
//
//#define HEIGHTMAP_X 16.0f
//#define HEIGHTMAP_Z 16.0f
//#define HEIGHTMAP_Y 8.0f
//#define HEIGHTMAP_TEX_X 1.0f / 16.0f
//#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

class HeightMap : public Mesh
{
public:
	HeightMap(std::string name, int rawWidth = 257, int rawHeight = 257, float heightMapX = 16.0f,
		float heightMapZ = 16.0f, float heightMapY = 8.0f,
		float heightMapTexX = 1.0f / 16.0f, float heightMapTexZ = 1.0f / 16.0f);
	~HeightMap(void) {
		delete data;
	};

	unsigned char* data;

	inline const int getRawWidth() const { return mRawWidth; }
	inline const int getRawHeight() const { return mRawHeight; }
	inline const int getHeightMapX() const { return mHeightMapX; }
	inline const int getHeightMapZ() const { return mHeightMapZ; }
	inline const int getHeightMapY() const { return mHeightMapY; }
	inline const int getHeightMapTexX() const { return mHeightMapTexX; }
	inline const int getHeightMapTexZ() const { return mHeightMapTexZ; }

	inline void setHeightMapY(float value) { mHeightMapY = value; }

private:
	int mRawWidth;
	int mRawHeight;

	float mHeightMapX;
	float mHeightMapZ;
	float mHeightMapY;
	float mHeightMapTexX;
	float mHeightMapTexZ;
};


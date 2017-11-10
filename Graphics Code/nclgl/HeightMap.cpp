#include "HeightMap.h"

HeightMap::HeightMap(std::string name, int rawWidth, int rawHeight, float heightMapX, 
	float heightMapZ, float heightMapY,
	float heightMapTexX, float heightMapTexZ)
{
	mRawWidth = rawWidth;
	mRawHeight = rawHeight;
	
	mHeightMapX = heightMapX;
	mHeightMapZ = heightMapZ;
	mHeightMapY = heightMapY;
	mHeightMapTexX = heightMapTexX;
	mHeightMapTexZ = heightMapTexZ;

	std::ifstream file(name.c_str(), ios::binary);
	if (!file)
	{
		return;
	}

	numVertices = mRawWidth * mRawHeight;
	numIndices = (mRawWidth - 1) * (mRawHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	unsigned char* data = new unsigned char[numVertices];
	file.read((char*)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < mRawWidth; ++x)
	{
		for (int z = 0; z < mRawHeight; ++z)
		{
			int offset = (x * mRawWidth) + z;
			vertices[offset] = Vector3(x * mHeightMapX, data[offset] * mHeightMapY, z * mHeightMapZ);
			textureCoords[offset] = Vector2(x * mHeightMapTexX, z * mHeightMapTexZ);
		}
	}

	delete data;

	numIndices = 0;

	for (int x = 0; x < mRawWidth - 1; ++x)
	{
		for (int z = 0; z < mRawHeight - 1; ++z)
		{
			int a = (x * (mRawWidth)) + z;
			int b = ((x + 1) * (mRawWidth)) + z;
			int c = ((x + 1) * (mRawWidth)) + (z + 1);
			int d = (x * (mRawWidth)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	GenerateTangents();
	BufferData();
}

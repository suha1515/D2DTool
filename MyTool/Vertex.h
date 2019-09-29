#pragma once
#ifndef __vertexH__
#define __vertexH__

struct Vertex
{
	Vertex() {}
	Vertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		pos.x = x;  pos.y = y;  pos.z = z;
		normal.x = nx; normal.y= ny; normal.z= nz;
		tex.x = u;  tex.y = v;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex;
	//float _x, _y, _z;
	//float _nx, _ny, _nz;
	//float _u, _v; // texture coordinates
};
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#endif // __vertexH__
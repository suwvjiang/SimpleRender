#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "Vector.h"
#include "Rasterizer.h"
#include "Buffer.h"

using namespace std;

void loadObj(const string& path, std::vector<Vertex>& vertices, std::vector<int>& indexs)
{
	string line;
	ifstream myfile(path);
	std::vector<Vec3f> pointes;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> uvs;

	std::vector<int> index_t;
	std::vector<int> index_n;

	while (getline(myfile, line))
	{
		if (line.substr(0, 2) == "v ")//vertices
		{
			istringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;
			pointes.emplace_back(Vec3f(x, y, z));
		}
		else if (line.substr(0, 2) == "vt")//texture coordinate
		{
			istringstream coord(line.substr(3));
			float u, v;
			coord >> u >> v;
			uvs.emplace_back(Vec2f(u, v));
		}
		else if (line.substr(0, 2) == "vn")//normal
		{
			istringstream nor(line.substr(3));
			float x, y, z;
			nor >> x >> y >> z;
			normals.emplace_back(Vec3f(x, y, z));
		}
		else if (line.substr(0, 2) == "f ")//index
		{
			int i, j, k;
			int n, m, p;
			int x, y, z;
			
			const char* chh = line.c_str();
			sscanf_s(chh, "f %d//%d %d//%d %d//%d", &i, &x, &j, &y, &k, &z);
			indexs.emplace_back(i); index_t.emplace_back(n); index_n.emplace_back(x);
			indexs.emplace_back(j); index_t.emplace_back(m); index_n.emplace_back(y);
			indexs.emplace_back(k); index_t.emplace_back(p); index_n.emplace_back(z);
		}
	}

	int cnt = pointes.size();
	for (int i = 0; i < cnt; ++i)
	{
		vertices.emplace_back(Vertex(pointes[i], normals[i], Vec2f(0), Vec3f(0)));
	}
}
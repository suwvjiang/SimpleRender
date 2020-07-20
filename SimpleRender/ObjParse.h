#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "Vector.h"
#include "Rasterizer.h"

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
			int n = 0, m = 0, p = 0;
			int x, y, z;
			
			const char* chh = line.c_str();
			sscanf_s(chh, "f %d//%d %d//%d %d//%d", &i, &x, &j, &y, &k, &z);
			indexs.emplace_back(i - 1); index_t.emplace_back(n - 1); index_n.emplace_back(x - 1);
			indexs.emplace_back(j - 1); index_t.emplace_back(m - 1); index_n.emplace_back(y - 1);
			indexs.emplace_back(k - 1); index_t.emplace_back(p - 1); index_n.emplace_back(z - 1);
		}
	}

	int cnt = pointes.size();
	for (int i = 0; i < cnt; ++i)
	{
		vertices.emplace_back(Vertex(pointes[i], normals[i], Vec2f(0), Vec3f(0)));
	}
}
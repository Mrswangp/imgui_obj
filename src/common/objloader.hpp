#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include<time.h>
#include <glm/glm.hpp>
namespace obj {
	struct objdata {
		bool v_flag;
		bool vt_flag;
		bool vn_flag;
	};
	bool load_obj(const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);
}

	/*class objloader {
	public:
		objloader(bool ver_flag, bool tex_flag, bool nor_flag) :v_flag(ver_flag), vt_flag(tex_flag), vn_flag(nor_flag) {};
		void verindex_push(unsigned int index)
		{
			vertexIndices.push_back(index);
		};
		void uvindex_push(unsigned int index)
		{
			uvIndices.push_back(index);
		};
	    void norindex_push(unsigned int index)
		{
			normalIndices.push_back(index);
		};
		void temp_ver_push(glm::vec3 vertex)
		{
			temp_vertices.push_back(vertex);
		};
		void temp_uvs_push(glm::vec2 uv)
		{
			temp_uvs.push_back(uv);
		};
	    void temp_normals_push(glm::vec3 normal)
		{
			temp_normals.push_back(normal);
		};
		void set_v_flag(bool flag) {
			v_flag = flag;
		}
		void set_vt_flag(bool flag) {
			vt_flag = flag;
		}
		void set_vn_flag(bool flag) {
			vn_flag = flag;
		}
		bool get_v_flag()
		{
			return v_flag;
		}
		bool get_vt_flag()
		{
			return vt_flag;
		}
		bool get_vn_flag()
		{
			return vn_flag;
		}
		std::vector<unsigned int> get_vertexindex()
		{
			return vertexIndices;
		}
		std::vector<unsigned int> get_uvindex()
		{
			return uvIndices;
		}
		std::vector<unsigned int> get_normalindex()
		{
			return normalIndices;
		}
		std::vector<glm::vec3> get_temp_vertices()
		{
			return temp_vertices;
		}
		std::vector<glm::vec2> get_temp_uvs()
		{
			return temp_uvs;
		}
		std::vector<glm::vec3> get_temp_normals()
		{
			return temp_normals;
		}
	private:
		bool v_flag;
		bool vt_flag;
		bool vn_flag;
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;
	};
	bool load_obj(const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);
}*/

#endif
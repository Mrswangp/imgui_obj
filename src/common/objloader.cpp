
#include "objloader.hpp"
using namespace obj;
bool extract_infor(obj::objdata obj_tar,std::vector<unsigned int> vertexIndices, 
	std::vector<unsigned int>  uvIndices, 
	std::vector<unsigned int> normalIndices, 
	std::vector<glm::vec3> temp_vertices,
	std::vector<glm::vec2> temp_uvs,
	std::vector<glm::vec3> temp_normals,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals)
{   
	if (obj_tar.v_flag && obj_tar.vt_flag && obj_tar.vn_flag) {
		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);

		}
	}
	else {
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			// Get the attributes thanks to the index
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			// Put the attributes in buffers
			out_vertices.push_back(vertex);

		}
	}
	return true;
}
bool opera_face(FILE* file, obj::objdata obj_tar,std::vector<unsigned int> &vertexIndices, std::vector<unsigned int> & uvIndices, std::vector<unsigned int>& normalIndices)
{
	
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	if (obj_tar.v_flag && obj_tar.vt_flag && obj_tar.vn_flag) {
		int matches = fscanf(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
		if (matches != 9) {
			printf("File can't be read by our simple nine parser :-( Try exporting with other options\n");
			fclose(file);
			return false;
		}
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);

		uvIndices.push_back(uvIndex[0]);
		uvIndices.push_back(uvIndex[1]);
		uvIndices.push_back(uvIndex[2]);

		normalIndices.push_back(normalIndex[0]);
		normalIndices.push_back(normalIndex[1]);
		normalIndices.push_back(normalIndex[2]);
	}
	else {
		int matches = fscanf(file, "%d//%d//%d//\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
		if (matches != 3) {
			printf(" File can't be read by our simple three parser :-( Try exporting with other options\n");
			fclose(file);
			return false;
		}
		vertexIndices.push_back(vertexIndex[0]);
		vertexIndices.push_back(vertexIndex[1]);
		vertexIndices.push_back(vertexIndex[2]);

	}
}
bool obj::load_obj(const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals)
{   
	printf("Loading OBJ file %s...\n", path);
	bool v_flag = false;
	bool vt_flag = false;
	bool vn_flag = false;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	//obj::objloader obj_tar(false, false, false);

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}
	obj::objdata obj_tar;
	obj_tar.v_flag = false;
	obj_tar.vt_flag = false;
	obj_tar.vn_flag = false;
	while (1) {

		char lineHeader[2000];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			obj_tar.v_flag = true;
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
			obj_tar.vt_flag = true;
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			obj_tar.vn_flag = true;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			opera_face(file, obj_tar, vertexIndices, uvIndices, normalIndices);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	extract_infor(obj_tar,vertexIndices,uvIndices,normalIndices,temp_vertices,temp_uvs,temp_normals, out_vertices, out_uvs, out_normals);
	fclose(file);
	return true;
}



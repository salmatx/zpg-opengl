#include "model_loader.h"

namespace engine {

std::shared_ptr<ObjectMesh_t> ModelLoader::Load(const std::string& t_path) {
	Assimp::Importer importer;
	unsigned int import_options = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes
		| aiProcess_JoinIdenticalVertices
		| aiProcess_CalcTangentSpace
		| aiProcess_GenNormals
		| aiProcess_GenSmoothNormals;

	const aiScene* scene = importer.ReadFile(t_path, import_options);

	if (scene) {
		auto num_meshes = scene->mNumMeshes;
		auto num_materials = scene->mNumMaterials;

		std::cout << "Loaded Model: " << "(" << t_path
		<< "), Num of meshes: " << num_meshes
		<< ", Num of materials: " << num_materials
		<< std::endl;

		if (num_meshes > 1) {
			std::cerr << "Object must have single mesh" << std::endl;
			return {};
		}

		for (unsigned int i = 0; i < num_materials; ++i) {
			const aiMaterial* material = scene->mMaterials[i];
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			std::cout << "Material [" << i << "] name: " << name.C_Str() << std::endl;
		}

		aiMesh* mesh = scene->mMeshes[0];

		auto mesh_object = std::make_shared<ObjectMesh_t>();

		auto num_mesh = mesh->mNumVertices;
		mesh_object->vertices.reserve(num_mesh);
		mesh_object->vertices.resize(num_mesh);
		for (unsigned int i = 0; i < num_mesh; ++i) {
			if (mesh->HasPositions()) {
				mesh_object->vertices[i].params[0] = mesh->mVertices[i].x;
				mesh_object->vertices[i].params[1] = mesh->mVertices[i].y;
				mesh_object->vertices[i].params[2] = mesh->mVertices[i].z;
			}

			if (mesh->HasNormals()) {
				mesh_object->vertices[i].params[3] = mesh->mNormals[i].x;
				mesh_object->vertices[i].params[4] = mesh->mNormals[i].y;
				mesh_object->vertices[i].params[5] = mesh->mNormals[i].z;
			}

			if (mesh->HasTextureCoords(0)) {
				mesh_object->vertices[i].params[6] = mesh->mTextureCoords[0][i].x;
				mesh_object->vertices[i].params[7] = mesh->mTextureCoords[0][i].y;
			}

			if (mesh->HasTangentsAndBitangents()) {
				mesh_object->vertices[i].params[8] = mesh->mTangents[0].x;
				mesh_object->vertices[i].params[9] = mesh->mTangents[0].y;
				mesh_object->vertices[i].params[10] = mesh->mTangents[0].z;
			}
		}

		if (mesh->HasFaces()) {
			auto num_faces = mesh->mNumFaces;
			mesh_object->indices.reserve(num_faces * 3);
			mesh_object->indices.resize(num_faces * 3);
			for (unsigned int i = 0; i < num_faces; ++i) {
				mesh_object->indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
				mesh_object->indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				mesh_object->indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			}
		}

		return mesh_object;

	}
	else {
		std::cerr << "Error during parsing mesh from (" << t_path << ") " << importer.GetErrorString() << std::endl;
		return {};
	}
}

}

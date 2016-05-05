#include <cstdio>
#include <vector>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>



// ���b�V���f�[�^
struct MeshData
{
	// ���_�t�H�[�}�b�g
	struct VertexFormat
	{
		aiVector3D	m_pos;
		aiVector2D	m_tex;
		aiVector3D	m_normal;
		aiColor3D	m_color;

		VertexFormat()
		{
		}

		VertexFormat(const aiVector3D& pos, const aiVector2D& tex, const aiVector3D& normal, const aiColor3D& color)
			: m_pos(pos)
			, m_tex(tex)
			, m_normal(normal)
			, m_color(color)
		{
		}
	};

	std::vector<VertexFormat>	vertices;		// ���_�z��
	std::vector<uint32_t>		indices;		// �C���f�b�N�X�z��
};

int main()
{
	std::vector<MeshData> mashDatum;	// ���b�V���f�[�^�W����

	Assimp::Importer importer;

	// �t�@�C������V�[���쐬
	const aiScene* pScene = importer.ReadFile("Bandouiruka0.3ds", aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals);

	if(pScene)
	{
		printf_s("�V�[���̓ǂݍ��ݐ���\n");

		mashDatum.resize(pScene->mNumMeshes);

		// ���b�V���f�[�^�����������čs��
		for(unsigned int i = 0; i < mashDatum.size(); ++i)
		{
			const aiMesh* pMesh = pScene->mMeshes[i];

			// �f�B�t���[�Y�J���[�l��
			aiColor3D diffuseColor(0.f, 0.f, 0.f);
			pScene->mMaterials[pMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

			aiVector3D ZERO_3D(0.0f, 0.0f, 0.0f);

			// ���_���i�[
			for(unsigned int j = 0; j < pMesh->mNumVertices; ++j)
			{
				aiVector3D* pTexCoord;
				if(pMesh->HasTextureCoords(0))
				{
					pTexCoord = &pMesh->mTextureCoords[0][j];
				}
				else
				{
					pTexCoord = &ZERO_3D;
				}

				mashDatum[i].vertices.emplace_back(
					pMesh->mVertices[j],
					aiVector2D(pTexCoord->x, pTexCoord->y),
					pMesh->mNormals[j],
					diffuseColor);
			}

			// �ʏ�񂩂�C���f�b�N�X�����l��
			for(unsigned int j = 0; j < pMesh->mNumFaces; j++)
			{
				const aiFace& face = pMesh->mFaces[j];

				// �O�p�����Ă���͂��Ȃ̂�3�ȊO�̓X���[
				if(face.mNumIndices != 3)
				{
					continue;
				}

				mashDatum[i].indices.emplace_back(face.mIndices[0]);
				mashDatum[i].indices.emplace_back(face.mIndices[1]);
				mashDatum[i].indices.emplace_back(face.mIndices[2]);
			}
		}

		printf_s("=====���b�V�����\���J�n=====\n");
		printf_s("���b�V���f�[�^�� = %d\n" , mashDatum.size());

		for(const auto& meshData : mashDatum)
		{
			printf_s("\t=====���_���\���J�n=====\n");
			for(const auto& vertex : meshData.vertices)
			{
				printf_s("\t���W: x = %f y = %f z = %f\n", vertex.m_pos.x, vertex.m_pos.y, vertex.m_pos.z);
				printf_s("\tUV  : x = %f y = %f\n", vertex.m_tex.x, vertex.m_tex.y);
				printf_s("\t�@��: x = %f y = %f z = %f\n", vertex.m_normal.x, vertex.m_normal.y, vertex.m_normal.z);
				printf_s("\t�F  : r = %f g = %f b = %f\n", vertex.m_color.r, vertex.m_color.g, vertex.m_color.b);
			}
			printf_s("\t=====���_���\���I��=====\n");

			printf_s("\t=====�C���f�b�N�X�ɂ��ʏ��\���J�n=====\n");
			for(uint32_t i = 0; i < meshData.indices.size(); i += 3)
			{
				printf_s("\t��: %d %d %d\n", meshData.indices[i], meshData.indices[i + 1], meshData.indices[i + 2]);
			}
			printf_s("\t=====�C���f�b�N�X�ɂ��ʏ��\���I��=====\n");
		}
		printf_s("=====���b�V�����\���I��=====\n");
	}
	else
	{
		printf_s("�V�[���̓ǂݍ��ݎ��s\n");
	}

	getchar();
	return 0;
}

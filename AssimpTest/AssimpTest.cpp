#include <cstdio>
#include <vector>
#include <string>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>



// メッシュデータ
struct MeshData
{
	// 頂点フォーマット
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

	std::vector<VertexFormat>	vertices;		// 頂点配列
	std::vector<uint32_t>		indices;		// インデックス配列
	std::string					textureName;	// テクスチャ名
};

int main()
{
	std::vector<MeshData> mashDatum;	// メッシュデータ集合体

	Assimp::Importer importer;

	// ファイルからシーン作成
	const aiScene* pScene = importer.ReadFile("Bandouiruka0.3ds", aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals);

	if(pScene)
	{
		printf_s("シーンの読み込み成功\n");

		mashDatum.resize(pScene->mNumMeshes);

		// メッシュデータを初期化して行く
		for(unsigned int i = 0; i < mashDatum.size(); ++i)
		{
			const aiMesh* pMesh = pScene->mMeshes[i];

			// ディフューズカラー獲得
			aiColor3D diffuseColor(0.f, 0.f, 0.f);
			pScene->mMaterials[pMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

			// テクスチャパスを獲得
			aiString path;
			pScene->mMaterials[pMesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			mashDatum[i].textureName = path.C_Str();

			aiVector3D ZERO_3D(0.0f, 0.0f, 0.0f);

			// 頂点情報格納
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

			// 面情報からインデックス情報を獲得
			for(unsigned int j = 0; j < pMesh->mNumFaces; j++)
			{
				const aiFace& face = pMesh->mFaces[j];

				// 三角化しているはずなので3以外はスルー
				if(face.mNumIndices != 3)
				{
					continue;
				}

				mashDatum[i].indices.emplace_back(face.mIndices[0]);
				mashDatum[i].indices.emplace_back(face.mIndices[1]);
				mashDatum[i].indices.emplace_back(face.mIndices[2]);
			}
		}

		printf_s("=====メッシュ情報表示開始=====\n");
		printf_s("メッシュデータ数 = %d\n" , mashDatum.size());

		for(const auto& meshData : mashDatum)
		{
			printf_s("\tテクスチャ名 = %s\n" , meshData.textureName.c_str());
			printf_s("\t=====頂点情報表示開始=====\n");
			for(const auto& vertex : meshData.vertices)
			{
				printf_s("\t座標: x = %f y = %f z = %f\n", vertex.m_pos.x, vertex.m_pos.y, vertex.m_pos.z);
				printf_s("\tUV  : x = %f y = %f\n", vertex.m_tex.x, vertex.m_tex.y);
				printf_s("\t法線: x = %f y = %f z = %f\n", vertex.m_normal.x, vertex.m_normal.y, vertex.m_normal.z);
				printf_s("\t色  : r = %f g = %f b = %f\n", vertex.m_color.r, vertex.m_color.g, vertex.m_color.b);
			}
			printf_s("\t=====頂点情報表示終了=====\n");

			printf_s("\t=====インデックスによる面情報表示開始=====\n");
			for(uint32_t i = 0; i < meshData.indices.size(); i += 3)
			{
				printf_s("\t面: %d %d %d\n", meshData.indices[i], meshData.indices[i + 1], meshData.indices[i + 2]);
			}
			printf_s("\t=====インデックスによる面情報表示終了=====\n");
		}
		printf_s("=====メッシュ情報表示終了=====\n");
	}
	else
	{
		printf_s("シーンの読み込み失敗\n");
	}

	getchar();
	return 0;
}

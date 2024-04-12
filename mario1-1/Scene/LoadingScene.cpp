#include"LoadingScene.h"
#include"../Object/Player.h"
#include"../Utility/CSVHandle.h"

LoadingScene::LoadingScene() : count(0)
{

}

LoadingScene::~LoadingScene()
{
	obj.push_back(new Player);
}

void LoadingScene::Initialize()
{
	LoadDivGraph("Resource/1-1image/Mario/mario.png", 9, 9, 1, 32, 32, image);
	LoadDivGraph("Resource/1-1image/UI/num.png", 15, 15, 1, 16, 16, num_img);
}

eSceneType LoadingScene::Update()
{
	count++;

	if (count >= 300)
	{
		return eSceneType::E_MAIN;
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}

void LoadingScene::Draw() const
{
	DrawString(0, 0, "LoadingScene", 0xFFFFFF);
	DrawRotaGraph(288, 240, 1.0, 0.0, image[0], TRUE);
	DrawRotaGraph(320.0f, 240.0f, 1.0f, 0.0f, num_img[11], TRUE);
	DrawRotaGraph(352.0f, 240.0f, 1.0f, 0.0f, num_img[3], TRUE);
}

void LoadingScene::Finalize()
{

}

eSceneType LoadingScene::GetNowScene() const
{
	return eSceneType::E_LOADING;
}

void LoadingScene::LoadDataCSV()
{
	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "r");

	if (result != 0)
	{
		throw("CSVファイルが開けませんでした\n");
	}

	// CSVFile<型名> data;
	CSVFile<int> data;
	// data.csv_read(入力ファイル名, ヘッダーの有無, インデックスの有無, 区切り文字);
	data.csv_read("Resource/dat/data.csv", false, true, ',');
}
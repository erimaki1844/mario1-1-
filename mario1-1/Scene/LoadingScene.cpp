#include"LoadingScene.h"
#include"../Object/Player.h"

LoadingScene::LoadingScene() : count(0)
{
	for (int i = 0; i < 3; i++)
	{
		mario_data[i] = NULL;
	}
}

LoadingScene::~LoadingScene()
{
	obj.push_back(new Player);
}

void LoadingScene::Initialize()
{
	LoadDataCSV();

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
	DrawRotaGraph(352.0f, 240.0f, 1.0f, 0.0f, num_img[life], TRUE);
}

void LoadingScene::Finalize()
{

}

eSceneType LoadingScene::GetNowScene() const
{
	return eSceneType::E_LOADING;
}

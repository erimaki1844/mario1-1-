#include"LoadingScene.h"

LoadingScene::LoadingScene() : count(0)
{

}

LoadingScene::~LoadingScene()
{

}

void LoadingScene::Initialize()
{

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
}

void LoadingScene::Finalize()
{

}

eSceneType LoadingScene::GetNowScene() const
{
	return eSceneType::E_LOADING;
}
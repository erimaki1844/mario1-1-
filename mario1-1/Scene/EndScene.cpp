#include"EndScene.h"

EndScene::EndScene() : count(0)
{

}

EndScene::~EndScene()
{

}

void EndScene::Initialize()
{

}

eSceneType EndScene::Update()
{
	count++;

	if (count >= 300)
	{
		return eSceneType::E_TITLE;
	}

	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}

void EndScene::Draw() const
{
	DrawString(390, 210, "GAMEOVER", 0xFFFFFF);
}

void EndScene::Finalize()
{

}

eSceneType EndScene::GetNowScene() const
{
	return eSceneType::E_END;
}
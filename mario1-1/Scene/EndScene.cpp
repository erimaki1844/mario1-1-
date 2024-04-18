#include"EndScene.h"
#include"../Utility/UI.h"

EndScene::EndScene() : count(0)
{

}

EndScene::~EndScene()
{

}

void EndScene::Initialize()
{
	//文字の読み込み
	LoadDivGraph("Resource/1-1image/UI/string.png", 26, 26, 1, 16, 16, char_img);

	//BGM読み込み
	bgm = LoadSoundMem("Resource/sound/SE_GameOver.wav");
	ChangeVolumeSoundMem(80, bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_BACK, TRUE);
}

eSceneType EndScene::Update()
{
	count++;

	if (count >= 240)
	{
		return eSceneType::E_TITLE;
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}

void EndScene::Draw() const
{
	char gameover[9] = { 'G','A','M','E','O','V','E','R'};
	for (int i = 0; i < 9; i++) 
	{
		DrawRotaGraph(264 + 16 * i, 240, 1.0, 0.0, char_img[UI::STRConversion(gameover[i])], TRUE);
	}
}

void EndScene::Finalize()
{

}

eSceneType EndScene::GetNowScene() const
{
	return eSceneType::E_END;
}
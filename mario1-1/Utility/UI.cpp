#include"UI.h"
#include"DxLib.h"

UI::UI()
{

}

UI::~UI()
{

}

void UI::Initialize()
{
	time = 0;
	score = 0;
	coin = 0;

	anim = 0;
	anim_count = 0;

	LoadDivGraph("Resource/1-1image/UI/num.png", 15, 15, 1, 16, 16, num_img);
	LoadDivGraph("Resource/1-1image/UI/uicoin.png", 4, 4, 1, 16, 16, coin_img);
	str_img[0] = LoadGraph("Resource/1-1image/UI/name_mario.png");
	str_img[1] = LoadGraph("Resource/1-1image/UI/time.png");
	str_img[2] = LoadGraph("Resource/1-1image/UI/top.png");
	str_img[3] = LoadGraph("Resource/1-1image/UI/world.png");
}

void UI::Update(int time, int score, int coin)
{
	this->time = time;
	this->coin = coin;
	this->score = score;

	// coinのアニメーション
	if (anim == 0 && anim_count > 20)
	{
		anim++;
		anim_count = 0;
	}
	else if (anim != 0 && anim_count > 8)
	{
		anim++;
		if (anim >= 4) anim = 0;

		anim_count = 0;
	}

	anim_count++;
}

void UI::Draw()
{
	//スコアの表示
	DrawRotaGraph(100.0f, 10.0f, 1.0f, 0.0f, str_img[0], TRUE);
	for (int i = 0; i < 5; i++)
	{
		DrawRotaGraph(132.0f - 16.0f * i, 28.0f, 1.0f, 0.0f, num_img[Conversion(score, i)], TRUE);
	}

	//コインの表示
	DrawRotaGraph(196.0f, 28.0f, 1.0f, 0.0f, coin_img[anim], TRUE);
	DrawRotaGraph(212.0f, 28.0f, 1.0f, 0.0f, num_img[11], TRUE);
	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraph(244.0f - 16.0f * i, 28.0f, 1.0f, 0.0f, num_img[Conversion(coin, i)], TRUE);
	}

	//現在のステージ表示
	DrawRotaGraph(340.0f, 10.0f, 1.0f, 0.0f, str_img[3], TRUE);
	DrawRotaGraph(326.0f, 28.0f, 1.0f, 0.0f, num_img[1], TRUE);
	DrawRotaGraph(340.0f, 28.0f, 1.0f, 0.0f, num_img[10], TRUE);
	DrawRotaGraph(356.0f, 28.0f, 1.0f, 0.0f, num_img[1], TRUE);

	//タイマーの表示
	DrawRotaGraph(444.0f, 10.0f, 1.0f, 0.0f, str_img[1], TRUE);
	for (int i = 0; i < 3; i++)
	{
		DrawRotaGraph(460.0f - 16.0f * i, 28.0f, 1.0f, 0.0f, num_img[Conversion(time, i)], TRUE);
	}
}

void UI::Finalize()
{

}

int UI::Conversion(int score,int num)
{
	int handle = 0;
	
	switch (num)
	{
	case 0:
		handle = score % 10;
		break;
	case 1:
		handle = (score % 100 - (score % 10)) / 10;
		break;
	case 2:
		handle = (score % 1000 - (score % 100)) / 100;
		break;
	case 3:
		handle = (score % 10000 - (score % 1000)) / 1000;
		break;
	case 4:
		handle = (score % 100000 - (score % 10000)) / 10000;
		break;
	default:
		break;
	}


	return handle;
}
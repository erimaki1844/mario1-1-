#include"ClayPipe.h"

ClayPipe::ClayPipe()
{

}

ClayPipe::~ClayPipe()
{

}

void ClayPipe::Initialize()
{
	box_size = Vector2D(32.0f, 48.0f + 32.0f * (length - 1));
	obj_type = E_CLAYPIPE;
	is_active = false;
	end_flg = false;

	image[0] = LoadGraph("Resource/1-1image/dokan_down.png");
	image[1] = LoadGraph("Resource/1-1image/dokan_up.png");
}

void ClayPipe::Update(Vector2D diff)
{
	//Playerとのズレを加算する
	location.x -= diff.x;

	//画面内に入ってから動くようにする
	if (location.x > 650.0f)
	{
		return;
	}

	//画面外に完全に出た終了する
	if (location.x < -64.0f)
	{
		end_flg = true;
	}
}

void ClayPipe::Draw()
{
	for (int i = 0; i < length; i++)
	{
		DrawRotaGraph(location.x, location.y - i * 32.0f, 1.0f, 0.0f, image[0], FALSE);
	}
	DrawRotaGraph(location.x, location.y - 32.0f * length, 1.0f, 0.0f, image[1], FALSE);
}
int ClayPipe::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		DeleteGraph(image[i]);
	}

	return 0;
}

//土管の長さを設定する
void ClayPipe::SetPreset(int preset)
{
	length = preset;
}
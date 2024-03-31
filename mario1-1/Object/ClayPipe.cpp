#include"ClayPipe.h"

ClayPipe::ClayPipe()
{

}

ClayPipe::~ClayPipe()
{

}

void ClayPipe::Initialize()
{
	location = Vector2D(400.0f, 400.0f);
	box_size = Vector2D(32.0f, 80.0f);
	obj_type = E_CLAYPIPE;
	end_flg = false;

	image[0] = LoadGraph("Resource/1-1image/dokan_down.png");
	image[1] = LoadGraph("Resource/1-1image/dokan_up.png");
}

void ClayPipe::Update()
{

}

void ClayPipe::Draw(Vector2D diff)
{
	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraph(location.x, location.y - i * 32.0f, 1.0f, 0.0f, image[0], FALSE);
	}
	DrawRotaGraph(location.x, location.y - 32.0f * 2, 1.0f, 0.0f, image[1], FALSE);
}
int ClayPipe::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		DeleteGraph(image[i]);
	}

	return 0;
}

//“yŠÇ‚Ì’·‚³‚ðÝ’è‚·‚é
void ClayPipe::SetPreset(int preset)
{
	length = preset;
}
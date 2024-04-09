#include"Bakuha.h"

Bakuha::Bakuha()
{

}

Bakuha::~Bakuha()
{

}

void Bakuha::Initialize()
{
	anim = 0;
	anim_count = 0;
	bakuha_type = 0;
	state = false;
	end_flg = false;
	is_active = false;
	obj_type = E_BAKUHA;

	LoadDivGraph("Resource/1-1image/bakuha.png", 3, 3, 1, 32, 32, image);
	se[0] = LoadSoundMem("Resource/sound/SE_6ConsecutiveFireworks.wav");

	for (int i = 0; i < 10; i++)
	{
		ChangeVolumeSoundMem(100, this->se[i]);
	}
}

void Bakuha::Update(Vector2D diff)
{
	if (anim_count > 60)
	{
		if (anim != 3) anim++;
		else end_flg = true;

		anim_count = 0;
	}

	anim_count++;

	if (bakuha_type == 4)
	{
		if (location.y > 150.0f)
		{
			location.y -= 5.0f;
		}
	}
	if (bakuha_type == 3)
	{
		if (location.y > 180.0f)
		{
			location.y -= 5.0f;
		}
	}
	if (bakuha_type == 2)
	{
		if (location.y > 170.0f)
		{
			location.y -= 5.0f;
		}
	}
	if (bakuha_type == 1)
	{
		if (location.y > 160.0f)
		{
			location.y -= 5.0f;
		}
	}


	if (is_active == false)
	{
		PlaySoundMem(se[0], DX_PLAYTYPE_BACK, TRUE);
		is_active = true;
	}

}

void Bakuha::Draw()
{
	if (bakuha_type == 4 && location.y < 200.0f) DrawRotaGraph(location.x, location.y, 1.0, 0.0, image[anim], TRUE);
	if (bakuha_type == 3 && location.y < 200.0f) DrawRotaGraph(location.x + 96.0f, location.y, 1.0, 0.0, image[anim], TRUE);
	if (bakuha_type == 2 && location.y < 200.0f) DrawRotaGraph(location.x - 32.0f, location.y, 1.0, 0.0, image[anim], TRUE);
	if (bakuha_type == 1 && location.y < 200.0f) DrawRotaGraph(location.x + 64.0f, location.y, 1.0, 0.0, image[anim], TRUE);
}

int Bakuha::Finalize()
{
	return 0;
}

void Bakuha::SetType(int handle)
{
	bakuha_type = handle;
}
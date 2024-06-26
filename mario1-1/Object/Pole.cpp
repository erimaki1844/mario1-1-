#include"Pole.h"

Pole::Pole()
{

}

Pole::~Pole()
{

}

void Pole::Initialize()
{

	box_size = Vector2D(1.0f, 240.0f);
	move = Vector2D(0.0f);
	score = 400;
	anim_count = 0;
	is_active = false;
	end_flg = false;
	obj_type = E_POLE;
	image[0] = LoadGraph("Resource/1-1image/pole_down.png");
	image[1] = LoadGraph("Resource/1-1image/pole.png");
	image[2] = LoadGraph("Resource/1-1image/flag.png");
}

void Pole::Update(Vector2D diff)
{
	//Playerとのズレを加算する
	location.x -= diff.x;

	//画面内に入ってから動くようにする
	if (location.x > 650.0f)
	{
		return;
	}

	if (is_active == true)
	{
		anim_count++;

		if (anim_count > 50 && location.y - 223.0f < location.y - move.y)Movement();
		else end_flg = true;
	}
}

void Pole::Draw()
{
	//Poleの長さは256
	for (int i = 0; i < 8; i++)
	{
		DrawRotaGraph(location.x, location.y - i * 32.0f, 1.0f, 0.0f, image[0], FALSE);
	}
	DrawRotaGraph(location.x, location.y - 256.0f, 1.0f, 0.0f, image[1], FALSE);
	DrawRotaGraph(location.x - 16.0f, location.y - 223.0f + move.y, 1.0f, 0.0f, image[2], FALSE);
	if (is_active == true)
	{
		DrawFormatString(location.x + 50.0f, location.y - move.y, 0xFFFFFF, "%d", score);
	}
}

int Pole::Finalize()
{
	for (int i = 0; i < 3; i++) 
	{
		DeleteGraph(image[0]);
	}

	return 0;
}

void Pole::Movement()
{
	move.y += 5.0f;
}

void Pole::OnHit(ObjectBase* obj)
{
	if (obj->GetObjectType() == E_PLAYER && is_active == false)
	{
		for (int i = 0; i < 8; i++)
		{
			if (obj->GetLocation().y <= location.y - i * 32.0f)
			{
				score +=  i * 400;
			}
		}
		is_active = true;
	}
}
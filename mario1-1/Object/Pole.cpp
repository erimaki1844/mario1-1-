#include"Pole.h"

Pole::Pole()
{

}

Pole::~Pole()
{

}

void Pole::Initialize()
{

	location = Vector2D(600.0f, 356.0f);
	box_size = Vector2D(1.0f, 240.0f);
	move = Vector2D(0.0f);
	score = 400;
	anim_count = 0;
	is_active = false;
	obj_type = E_POLE;
	image[0] = LoadGraph("Resource/1-1image/pole_down.png");
	image[1] = LoadGraph("Resource/1-1image/pole.png");
	image[2] = LoadGraph("Resource/1-1image/flag.png");
}

void Pole::Update()
{
	if (is_active == true)
	{
		anim_count++;

		if (anim_count > 50 && location.y - 223.0f < location.y - move.y)Movement();
	}
}

void Pole::Draw(Vector2D diff)
{
	//Player‚Æ‚ÌƒYƒŒ
	float diff_location = this->location.y - diff.y;

	//Pole‚Ì’·‚³‚Í256
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

void Pole::Finalize()
{
	for (int i = 0; i < 3; i++) 
	{
		DeleteGraph(image[0]);
	}
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
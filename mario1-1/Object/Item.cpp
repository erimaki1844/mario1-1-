#include"Item.h"
#include"../Utility/UI.h"
#include<cmath>

Item::Item() : g_speed(0.0f)
{

}

Item::~Item()
{

}

void Item::Initialize()
{
	anim = 0;
	anim_count=0;
	location = Vector2D(400.0f, 300.0f);
	box_size = Vector2D(16.0f, 16.0f);
	ChangeType(E_SUPER);
	obj_type = E_ITEM;
	direction = E_LEFT;
	is_active = false;
	end_flg = false;
	display_flg = false;
	start_pos = location.y;
}

void Item::Update()
{
	if (display_flg == true)
	{
		anim_count++;
		if (anim_count > 50)
		{
			end_flg = true;
		}

		return;
	}

	if (state == true)
	{
		if (item_type == E_COIN)
		{
			g_speed = 10.0f;
			is_active = true;
			state = false;
		}
		else if (location.y + 32.0f > start_pos)
		{
			location.y -= 1.0f;
		}
		else
		{
			is_active = true;
			state = false;
		}
	}

	if (anim_count / 5 >= 1 && anim_count % 5 == 0)
	{
		anim++;
	}

	Movement();
}

void Item::Draw(Vector2D diff)
{
	if (state == true || is_active == true)
	{
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[anim], TRUE);
	}
	if (display_flg == true)
	{
		for (int i = 0; i < 4; i++)
		{
			DrawRotaGraph((location.x + 80.0f) - 16.0f * i, location.y - 10.0f, 1.0f, 0.0f, num_img[UI::Conversion(score, i)], FALSE);
		}
	}

}

int Item::Finalize()
{
	for (int i = 0; i < 15; i++)
	{
		DeleteGraph(image[i]);
		DeleteGraph(num_img[i]);
	}

	return score;
}

//Itemの移動処理
void Item::Movement()
{
	Vector2D move = Vector2D(0.0f);

	if (is_active == true && item_type != E_COIN)
	{
		//重力
		g_speed -= GRAVITY;
		location.y -= g_speed;
	}

	if (is_active == true)
	{
		if (item_type == E_SUPER)
		{
			//左移動処理
			if (direction == E_LEFT)
			{
				move += Vector2D(2.0f, 0.0f);
			}
			//右移動処理
			if (direction == E_RIGHT)
			{
				move += Vector2D(-2.0f, 0.0f);
			}

			location += move;
		}
		if (item_type == E_COIN)
		{
			if (anim_count < 20)
			{
				//重力
				g_speed -= GRAVITY / 5;
				location.y -= g_speed;
			}
			else
			{
				Finalize();
			}

			anim_count++;
		}
	}
}

void Item::OnHit(ObjectBase* obj)
{
	//PLAYERの場合
	if (obj->GetObjectType() == E_PLAYER)
	{
		if (is_active == true)
		{
			anim_count = 0;
			is_active = false;
			display_flg = true;
		}
	}

	//BLOCKの場合
	if (obj->GetObjectType() == E_BLOCK)
	{
		if (obj->GetIsActive() && is_active != true)
		{
			state = true;
		}

		if (obj->GetIsActive() && is_active == true)
		{
			direction = E_RIGHT;
			location.x -= 20.0f;
			location.y -= 20.0f;
		}

		//このアイテムがアクティブな状態か？
		if (is_active == false)return;

		//ブロッキング処理
		//位置情報の差分を取得
		Vector2D diff_location = location - obj->GetLocation();

		//当たり判定サイズの大きさを取得
		Vector2D box_ex = box_size + obj->GetSize();

		//どれだけ重なっているか？
		Vector2D blocking;
		blocking.x = box_ex.x - fabsf(diff_location.x);
		blocking.y = box_ex.y - fabsf(diff_location.y);

		if (fabsf(diff_location.x) > fabsf(diff_location.y))
		{
			if (diff_location.x > 0)location.x = blocking.x;
			else if (diff_location.x < 0)location.x = -blocking.x;
		}
		else
		{
			if (diff_location.y < 0)
			{
				if (item_type != E_COIN)
				{
					g_speed = 0.0f;
					location.y -= blocking.y;
				}
			}
		}
	}
}

void Item::ChangeType(eItemType type)
{
	if (type == E_1UP)
	{
		score = 0;
		LoadDivGraph("Resource/1-1image/Item/1up_mushroom.png", 1, 1, 1, 32, 32, image);
	}
	if (type == E_SUPER)
	{
		score = 1000;
		LoadDivGraph("Resource/1-1image/Item/mushroom.png", 1, 1, 1, 32, 32, image);
	}
	if (type == E_COIN)
	{
		score = 200;
		LoadDivGraph("Resource/1-1image/Item/coin.png", 4, 4, 1, 32, 32, image);
	}

	item_type = type;
}

int Item::GetPreset()
{
	int i = item_type;
	return i;
}
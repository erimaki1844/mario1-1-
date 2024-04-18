#include"Item.h"
#include"../Utility/UI.h"
#include<cmath>

Item::Item()
{

}

Item::~Item()
{

}

void Item::Initialize()
{
	anim = 0;
	anim_count=0;
	g_speed = 0.0f;
	box_size = Vector2D(16.0f, 16.0f);
	obj_type = E_ITEM;
	direction = E_LEFT;
	state = false;
	is_active = false;
	end_flg = false;
	display_flg = false;
	start_pos = location.y;

	//画像の読み込み
	LoadDivGraph("Resource/1-1image/UI/num.png", 15, 15, 1, 16, 16, num_img);

	//SEの読み込み
	se[0] = LoadSoundMem("Resource/sound/SE_ItemAppearance.wav");
	se[1] = LoadSoundMem("Resource/sound/SE_1Up.wav");
	se[2] = LoadSoundMem("Resource/sound/SE_CoinPickUp.wav");

	for (int i = 0; i < 3; i++)
	{
		ChangeVolumeSoundMem(100, this->se[i]);
	}
}

void Item::Update(Vector2D diff)
{
	//Playerとのズレを加算する
	location.x -= diff.x;

	//画面内に入ってから動くようにする
	if (location.x > 650.0f)
	{
		return;
	}

	//画面外に出たら終了する
	if (location.x < -64.0f || location.y > 500.0f)
	{
		end_flg = true;
	}

	if (display_flg == true)
	{
		anim_count++;
		if (anim_count > 50)
		{
			end_flg = true;
		}

		return;
	}

	//アイテムがブロックがら出てくる処理
	if (state == true)
	{
	
		if (location.y + 22.0f > start_pos)
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

		if (anim == 4)anim = 0;
	}

	Movement();
}

void Item::Draw()
{
	if (state == true || is_active == true)
	{
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[anim], TRUE);
	}
	if (display_flg == true)
	{
		for (int i = 0; i < 4; i++)
		{
			DrawRotaGraph((location.x + 80.0f) - 16.0f * i, location.y - 10.0f, 1.0f, 0.0f, num_img[UI::Conversion(score, i)], TRUE);
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

	//落下処理
	if (is_active == true && item_type == E_COIN)
	{
		if (g_speed > -5.0f)
		{
			//重力
			g_speed -= GRAVITY / 5;
		}
		else
		{
			display_flg = true;
			anim_count = 0;
		}

		anim_count++;

	}
	else if (is_active == true)
	{
		if (g_speed > -12.0f)
		{
			//重力
			g_speed -= GRAVITY;
		}
	}

	move.y -= g_speed;

	if (is_active == true)
	{
		if (item_type == E_SUPER || item_type == E_1UP)
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
		}

		location += move;

	}
}

void Item::OnHit(ObjectBase* obj)
{
	//PLAYERの場合
	if (obj->GetObjectType() == E_PLAYER)
	{
		if (is_active == true && item_type != E_COIN)
		{
			anim_count = 0;
			is_active = false;
			display_flg = true;
			if (item_type == E_1UP)
			{
				PlaySoundMem(se[1], DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}

	//BLOCKの場合
	if (obj->GetObjectType() == E_BLOCK)
	{
		//Playerがブロックに下側から当たったらアイテムが出てくる
		if (obj->GetPreset() == 1 && is_active != true)
		{
			if (item_type == E_COIN)
			{
				g_speed = 10.0f;
				is_active = true;
				PlaySoundMem(se[2], DX_PLAYTYPE_BACK, TRUE);
			}
			else if (state == false)
			{
				state = true;
				PlaySoundMem(se[0], DX_PLAYTYPE_BACK, TRUE);
			}
		}

		if (obj->GetIsActive() && is_active == true && item_type != E_COIN)
		{
			direction = E_RIGHT;
			location.x -= 20.0f;
			location.y -= 20.0f;
		}

		//このアイテムがアクティブな状態か？
		if (is_active == false || item_type == E_COIN)return;

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
			if (diff_location.x > 0)location.x += blocking.x;
			else if (diff_location.x < 0)location.x += -blocking.x;
		}
		else
		{
			if (diff_location.y > 0)
			{
				if (item_type != E_COIN)
				{
					location.y += blocking.y;
				}
			}
			else if (diff_location.y < 0 && g_speed < 0.0f)
			{
				g_speed = 0.0f;
				location.y += -blocking.y;
			}
		}
	}

	if (obj->GetObjectType() == E_CLAYPIPE)
	{
		direction = E_RIGHT;
	}
}

void Item::SetType(int handle)
{
	switch (handle)
	{
	case 0:
		score = 0;
		item_type = E_1UP;
		LoadDivGraph("Resource/1-1image/Item/1up_mushroom.png", 1, 1, 1, 32, 32, image);
		break;
	case 1:
		score = 1000;
		item_type = E_SUPER;
		LoadDivGraph("Resource/1-1image/Item/mushroom.png", 1, 1, 1, 32, 32, image);
		break;
	case 2:
		score = 200;
		item_type = E_COIN;
		LoadDivGraph("Resource/1-1image/Item/coin.png", 4, 4, 1, 32, 32, image);
		break;
	default:
		break;
	}
}

int Item::GetPreset()
{
	int i = item_type;
	return i;
}
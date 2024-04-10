#include"Enemy.h"
#include"../Utility/UI.h"
#include<cmath>

Enemy::Enemy() : speed(0.0f),g_speed(0.0f)
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	anim = 0;
	anim_count = 0;
	count = 0;
	angle = 0.0f;
	obj_type = E_ENEMY;
	now_anim = E_IDOL_ENEMY;
	direction = E_RIGHT;
	is_active = true;
	state = false;
	end_flg = false;

	LoadDivGraph("Resource/1-1image/UI/num.png", 15, 15, 1, 16, 16, num_img);
}

void Enemy::Update(Vector2D diff)
{
	//Playerとのズレを加算する
	location.x -= diff.x;

	// 画面外に完全に出たら終了する
	if (enemy_type == E_KOOPTROOPA_HIDE)
	{
		if (location.x > 680.0f || location.y > 500.0f)
		{
			end_flg = true;
		}
	}

	//画面内に入ってから動くようにする
	if (location.x > 680.0f)
	{
		return;
	}

	// めり込み分ずらす
	location += overlap;
	overlap = Vector2D(0.0f);

	//画面外に出たら終了する
	if (location.x < -64.0f || location.y > 500.0f)
	{
		end_flg = true;
	}

	//アクティブな状態じゃなかったらENEMYの終了アニメーションをする
	if (is_active == false)
	{
		if (now_anim == E_HIT_ENEMY && enemy_type == E_GOOMBA)
		{
			anim_count++;
			if (anim_count > 50)
			{
				end_flg = true;
			}
			return;
		}
		Movement();
		return;
	}

	//ノコノコが甲羅にこもっている時のアニメーション
	if (enemy_type == E_KOOPTROOPA_HIDE)
	{
		count++;

		if (count > 200 && now_anim == E_HIT_ENEMY)
		{
			if (anim_count > 5 - speed)
			{
				anim_count = 0;
				anim++;
				if (anim == 2) anim = 0;
			}
			if (count > 350)
			{
				ChangeType(E_KOOPTROOPA);
				ChangeAnim(E_IDOL_ENEMY);
			}
		}
	}
	else if (anim_count > 5 - speed)
	{
		anim_count = 0;
		anim++;
		if (anim == 2) anim = 0;
	}

	anim_count++;

	Movement();
}

void Enemy::Draw()
{
	if (enemy_type == E_GOOMBA)
	{
		if (now_anim == E_IDOL_ENEMY)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE);
		}
		if (now_anim == E_HIT_ENEMY)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[2], TRUE);
			for (int i = 0; i < 3; i++)
			{
				DrawRotaGraph((location.x + 80.0f) - 16.0f * i, location.y - 10.0f, 1.0f, 0.0f, num_img[UI::Conversion(score, i)], TRUE);
			}
		}
	}
	if (enemy_type == E_KOOPTROOPA)
	{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE,direction);
	}
	if (enemy_type == E_KOOPTROOPA_HIDE)
	{
		DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE);
	}

	DrawFormatString(0, 80, 0xFFFFFF, "%d", count);
}

int Enemy::Finalize()
{
	for (int i=0; i < 15; i++)
	{
		DeleteGraph(image[i]);
		DeleteGraph(num_img[i]);
	}
	return score;
}

void Enemy::ChangeAnim(eEnemyAnim anim)
{
	now_anim = anim;
}

void Enemy::OnHit(ObjectBase* obj)
{
	//PLAYERの場合
	if (obj->GetObjectType() == E_PLAYER && obj->GetIsActive() == true)
	{
		//ノコノコが甲羅に籠っている時に当たったら転がっていく
		if (enemy_type == E_KOOPTROOPA_HIDE && state == false)
		{
			ChangeAnim(E_IDOL_ENEMY);
			speed = 5.0f;
			state = true;
			if (obj->GetLocation().x > location.x)direction = E_RIGHT;
			if (obj->GetLocation().x < location.x)direction = E_LEFT;
		}

		//ENEMYがアクティブな状態か？
		if (is_active == false)
		{
			return;
		}

		//マリオが敵の頭上にいるか判定する
		if (location.y - box_size.y < obj->GetLocation().y + obj->GetSize().y && 
			location.y > obj->GetLocation().y + obj->GetSize().y )
		{
			//ノコノコの時は甲羅にこもる
			if (enemy_type == E_KOOPTROOPA)
			{
				ChangeAnim(E_HIT_ENEMY);
				ChangeType(E_KOOPTROOPA_HIDE);
				anim = 0;
			}
			if (enemy_type == E_GOOMBA)
			{
				ChangeAnim(E_HIT_ENEMY);
				is_active = false;
			}
		}
	}

	//BLOCKとPipeの場合
	if (obj->GetObjectType() == E_BLOCK || obj->GetObjectType() == E_CLAYPIPE)
	{

		//ENEMYがアクティブな状態か？
		if (is_active == false)
		{
			return;
		}

		if (obj->GetIsActive())
		{
			is_active = false;
			location.x -= 20.0f;
			location.y -= 20.0f;
			angle = 3.0f;
			return;
		}
		//ブロッキング処理
		//位置情報の差分を取得
		Vector2D diff_location = location - obj->GetLocation();

		//当たり判定サイズの大きさを取得
		Vector2D box_ex = box_size + obj->GetSize();

		//どれだけ重なっているか？
		Vector2D blocking;
		blocking.x = box_ex.x - fabsf(diff_location.x);
		blocking.y = box_ex.y - fabsf(diff_location.y);

		if (g_speed < 0.0f || blocking.y < 1.0f)
		{
			g_speed = 0.0f;
			overlap.y = -blocking.y;
		}
		else if (g_speed >= 0.0f)
		{
			if (diff_location.x > 0)
			{
				overlap.x = blocking.x;
				direction = E_LEFT;
			}
			else if (diff_location.x < 0)
			{
				overlap.x = -blocking.x;
				direction = E_RIGHT;
			}
		}
	}

	//ノコノコの甲羅に当たった時
	if (obj->GetObjectType() == E_ENEMY)
	{
		//ENEMYがアクティブな状態か？
		if (is_active == false)
		{
			return;
		}

		if (obj->GetPreset() == 2 && obj->GetState())
		{
			is_active = false;
			g_speed = 10.0f;
			speed = -5.0f;
			angle = 3.0f;
			return;
		}
	}
}

//ENEMYの移動処理
void Enemy::Movement()
{
	Vector2D move = Vector2D(0.0f);

	if (now_anim == E_IDOL_ENEMY)
	{
		//左移動処理
		if (direction == E_LEFT)
		{
			move += Vector2D(1.0f + speed, 0.0f);
		}
		//右移動処理
		if (direction == E_RIGHT)
		{
			move += Vector2D(-1.0f - speed, 0.0f);
		}
	}


	//重力
	if (g_speed > -12.0f)
	{
		//重力
		g_speed -= GRAVITY;
	}
	move.y -= g_speed;

	location += move;
}

void Enemy::ChangeType(eEnemyType type)
{
	if (type == E_GOOMBA)
	{
		score = 100;
		enemy_type = type;
		box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Enemy/kuribo.png", 3, 3, 1, 32, 32, image);
	}
	if (type == E_KOOPTROOPA)
	{
		score = 200;
		enemy_type = type;
		box_size = Vector2D(16.0f, 32.0f);
		LoadDivGraph("Resource/1-1image/Enemy/nokonoko.png", 2, 2, 1, 32, 64, image);
	}
	if (type == E_KOOPTROOPA_HIDE)
	{
		enemy_type = type;
		box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Enemy/nokonoko_revival.png", 2, 2, 1, 32, 32, image);
	}
}

//生成するエネミーのタイプを設定する
void Enemy::SetType(int handle)
{
	switch (handle)
	{case 0:
		score = 100;
		enemy_type = E_GOOMBA;
		box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Enemy/kuribo.png", 3, 3, 1, 32, 32, image);
		break;
	case 1:
		score = 200;
		enemy_type = E_KOOPTROOPA;
		box_size = Vector2D(16.0f, 32.0f);
		LoadDivGraph("Resource/1-1image/Enemy/nokonoko.png", 2, 2, 1, 32, 64, image);
		break;
	case 2:
		enemy_type = E_KOOPTROOPA_HIDE;
		box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Enemy/nokonoko_revival.png", 2, 2, 1, 32, 32, image);
		break;
	default:
		break;
	}
}

//Enemyのタイプを取得する
int Enemy::GetPreset()
{
	int type = enemy_type;
	return type;
}
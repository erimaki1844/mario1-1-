#include"Player.h"
#include"../Utility/InputControl.h"
#include<cmath>

#define MAX_SPEED 3.0f

Player::Player() : jump_power(0.0f), speed(0.0f), count(0),start_pos(0.0f), flash_count(0), anim_count2(0)
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	ChangeAnim(E_IDOL);
	direction = E_RIGHT;
	this->location = Vector2D(30.0f, 100.0f);
	overlap = Vector2D(0.0f);
	this->angle = 0.0f;
	anim = 1;
	anim_count = 0;
	this->obj_type = E_PLAYER;
	ChangeType(SUPER);
	is_active = true;
	state = true;
	end_flg = false;

	//SEの読み込み
	se[0] = LoadSoundMem("Resource/sound/SE_Death.wav");
	se[1] = LoadSoundMem("Resource/sound/SE_Degeneration.wav");
	se[2] = LoadSoundMem("Resource/sound/SE_MiniJump.wav");
	se[3] = LoadSoundMem("Resource/sound/SE_SuperJump.wav");
	se[4] = LoadSoundMem("Resource/sound/SE_PowerUp.wav");
	se[5] = LoadSoundMem("Resource/sound/SE_StepOn.wav");
	se[6] = LoadSoundMem("Resource/sound/SE_Touch.wav");
	se[7] = LoadSoundMem("Resource/sound/SE_Kick.wav");
	se[8] = LoadSoundMem("Resource/sound/SE_PoleTouch.wav");
	se[9] = LoadSoundMem("Resource/sound/SE_Goal.wav");

	for (int i = 0; i < 10; i++) 
	{
		ChangeVolumeSoundMem(100, this->se[i]);
	}

	//BGMの読み込み
	bgm = LoadSoundMem("Resource/sound/BGM_MarioGround.wav");

	ChangeVolumeSoundMem(80, bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, TRUE);
}

void Player::Update(Vector2D diff)
{
	//画面外にいたらゲームオーバー扱いとする
	if (location.y > 500.0f)
	{
		end_flg = true;
		return;
	}

	// めり込み分ずらす
	location += overlap;
	overlap = Vector2D(0.0f);

	PlayerAnim();

	// マリオのアニメーション中
	if (state == false)
	{
		speed = 0.0f;
		return;
	}

	//しゃがみ処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) && player_type == SUPER)
	{
		ChangeAnim(E_SQUAT);
	}

	//走っている時のアニメーション
	if (anim_count > 5 - speed)
	{
		anim++;
		if (anim >= 4) anim = 1;

		anim_count = 0;
	}

	anim_count++;

	Movement();
}

void Player::Draw()
{
	//チビマリオの画像表示処理
	if (player_type == NOMAL)
	{
		if (is_active == false)
		{
			if (anim_count2 > 5)
			{
				if (now_anim == E_IDOL)
				{
					DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[0], TRUE, direction);
				}
				if (now_anim == E_RUN || now_anim == E_GOAL)
				{
					DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
				}
				if (now_anim == E_BRAKE)
				{
					DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[4], TRUE, direction);
				}
				if (now_anim == E_JUMP)
				{
					DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[5], TRUE, direction);
				}
				if (now_anim == E_GAMEOVER)
				{
					DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[6], TRUE, direction);
				}
			}
		}
		else
		{
			if (now_anim == E_IDOL)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[0], TRUE, direction);
			}
			if (now_anim == E_RUN || now_anim == E_GOAL)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
			}
			if (now_anim == E_BRAKE)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[4], TRUE, direction);
			}
			if (now_anim == E_JUMP)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[5], TRUE, direction);
			}
			if (now_anim == E_GAMEOVER)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[6], TRUE, direction);
			}
			if (now_anim == E_CLING)
			{
				DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
			}
		}
	}

	//スーパーマリオの画像表示処理
	if (player_type == SUPER)
	{
		if (now_anim == E_IDOL)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[0], TRUE, direction);
		}
		if (now_anim == E_SQUAT)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[1], TRUE, direction);
		}
		if (now_anim == E_RUN || now_anim == E_GOAL)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim + 1], TRUE, direction);
		}
		if (now_anim == E_BRAKE)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[5], TRUE, direction);
		}
		if (now_anim == E_JUMP)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[6], TRUE, direction);
		}
		if (now_anim == E_CLING)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
		}
	}

	//点滅時の画像表示処理
	if (player_type == POWER_DOWN)
	{
		if (anim_count2 > 5)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
		}
	}

	//チビマリオからスーパーマリオになる時の処理
	if (player_type == POWER_UP)
	{
		DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
	}
}

int Player::Finalize()
{
	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(image[i]);
	}

	return 0;
}

void Player::Movement()
{
	move = Vector2D(0.0f);

	//左移動処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT) && now_anim != E_JUMP)
	{
		//ブレーキ処理
		if (direction == E_RIGHT && speed > 0.0f)
		{
			ChangeAnim(E_BRAKE);
			speed -= MAX_SPEED / 20;
			move += Vector2D(speed, 0.0f);
			//location.x += speed;
		}
		else
		{
			speed++;

			if (InputControl::GetButton(XINPUT_BUTTON_B))
			{
				if (MAX_SPEED <= speed)
				{
					speed = MAX_SPEED;
				}
			}
			else
			{
				if (2.0f <= speed)
				{
					speed = 2.0f;
				}
			}

			ChangeAnim(E_RUN);
			direction = E_LEFT;
			move += Vector2D(-speed, 0.0f);
		}
	}
	//右移動処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT) && now_anim != E_JUMP)
	{
		//ブレーキ処理
		if (direction == E_LEFT && speed > 0.0f)
		{
			ChangeAnim(E_BRAKE);
			speed -= MAX_SPEED / 20;
			move += Vector2D(-speed, 0.0f);
			//location.x -= speed;
		}
		else
		{

			speed++;

			if (InputControl::GetButton(XINPUT_BUTTON_B))
			{
				if (MAX_SPEED <= speed)
				{
					speed = MAX_SPEED;
				}
			}
			else
			{
				if (2.0f <= speed)
				{
					speed = 2.0f;
				}
			}

			ChangeAnim(E_RUN);
			direction = E_RIGHT;
			move += Vector2D(speed, 0.0f);
		}
	}

	//慣性
	if (move == 0.0f && now_anim != E_JUMP && now_anim != E_BRAKE) {
		if (speed > 0.0f)
		{
			ChangeAnim(E_RUN);
			speed -= MAX_SPEED / 25;
		}
		else
		{
			speed = 0.0f;
		}
	}

	//重力
	if (jump_power > -12.0f)
	{
		jump_power -= GRAVITY;
	}

	//ジャンプ処理
	if (E_JUMP != now_anim && jump_power > -1.0f)
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
		{
			jump_power = 10.0f;
			ChangeAnim(E_JUMP);
			if (speed >= MAX_SPEED)
			{
				PlaySoundMem(se[3], DX_PLAYTYPE_BACK, TRUE);
			}
			else
			{
				PlaySoundMem(se[2], DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
	else
	{
		if (InputControl::GetButton(XINPUT_BUTTON_A))
		{
			count++;
			if (count < 15)
			{
				if (speed >= MAX_SPEED)
				{
					jump_power += 1.0f;
				}
				else
				{
					jump_power += 0.8f;
				}
			}
		}
	}

	move.y -= jump_power;

	if (direction == E_LEFT)move += Vector2D(-speed, 0.0f);
	if (direction == E_RIGHT)move += Vector2D(speed, 0.0f);

	location += move;

	if (location.x - box_size.x < 0 || location.x + box_size.x > 390)
	{
		location.x -= move.x;
	}
}

//ヒット時処理
void Player::OnHit(ObjectBase* obj)
{
	//再生中のSEをとめる
	StopSoundMem(se[2]);
	StopSoundMem(se[3]);

	//ENEMYの場合
	if (obj->GetObjectType() == E_ENEMY && is_active == true)
	{
		//アクティブな状態か？
		if (is_active == false)return;

		//ENEMYがアクティブな状態か？
		if (obj->GetIsActive() == false)
		{
			return;
		}
		//マリオが敵の頭上にいるか判定する
		if (location.y < obj->GetLocation().y && jump_power < 0.0f)
		{
			//踏みつけていたら少し跳ねる
			jump_power = 10.0f;
			PlaySoundMem(se[5], DX_PLAYTYPE_BACK, TRUE);
		}
		else if (obj->GetIsActive() == true)
		{
			if (obj->GetPreset() == 2)
			{
				PlaySoundMem(se[7], DX_PLAYTYPE_BACK, TRUE);
				return;
			}
			else PlaySoundMem(se[6], DX_PLAYTYPE_BACK, TRUE);
			//ミニマリオの時にエネミーと当たっていたらゲームオーバー
			if (player_type == NOMAL)
			{
				anim = 0;
				jump_power = 10.0f;
				is_active = false;
				state = false;
				StopSoundMem(bgm);
				ChangeAnim(E_GAMEOVER);
			}
			//スーパーマリオの時にエネミーと当たっていたらミニマリオに戻る
			if (player_type == SUPER)
			{
				anim = 0;
				is_active = false;
				state = false;
				ChangeType(POWER_DOWN);
				PlaySoundMem(se[1], DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
	//BLOCKの場合
	if (obj->GetObjectType() == E_BLOCK && now_anim != E_GAMEOVER || obj->GetObjectType() == E_CLAYPIPE)
	{
		//ポールにしがみついて下がりきった時にブロックに当たったら終了
		if (now_anim == E_CLING && direction != E_LEFT)
		{
			anim_count2 = 0;
			location.x += 32.0f;
			direction = E_LEFT;
			StopSoundMem(se[8]);
			return;
		}
		else if (now_anim == E_CLING) return;

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
			if (diff_location.x > 0)overlap.x = blocking.x;
			else if (diff_location.x < 0)overlap.x = -blocking.x;
		}
		else
		{
			if (diff_location.y > 0)
			{
				overlap.y = blocking.y;
				jump_power -= 5.0f;
			}
			else if (diff_location.y < 0 && jump_power < 0.0f)
			{
				ChangeAnim(E_IDOL);
				overlap.y = -blocking.y;
			}
		}
	}
	//ITEMの場合
	if (obj->GetObjectType() == E_ITEM)
	{
		if (obj->GetIsActive())
		{
			if (obj->GetPreset() == 1 && player_type != SUPER)
			{
				ChangeType(POWER_UP);
				anim = 0;
				anim_count2 = 0;
				flash_count = 0;
				state = false;
				is_active = false;
				PlaySoundMem(se[4], DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}

	//Poleの場合
	if (obj->GetObjectType() == E_POLE && now_anim != E_CLING && obj->GetIsActive() != false)
	{
		anim_count2 = 0;
		state = false;
		anim = 8;
		now_anim = E_CLING;
		StopSoundMem(bgm);
		PlaySoundMem(se[8], DX_PLAYTYPE_BACK, TRUE);
	}
}

//PLAYERのアニメーション
void Player::PlayerAnim()
{
	move = Vector2D(0.0f);

	//ゴールした時のアニメーション
	if (now_anim == E_GOAL)
	{
		//重力
		if (jump_power > -12.0f)
		{
			jump_power -= GRAVITY;
		}
		//着地していたら
		if (jump_power <= 0)
		{
			if (anim_count > 5 - speed)
			{
				anim++;
				if (anim >= 4) anim = 1;

				anim_count = 0;
			}

			anim_count++;
		}
		move.x += 3.0f;
		move.y -= jump_power;

		location += move;
	}

	//ポールにしがみついた時のアニメーション
	if (now_anim == E_CLING && anim_count2 > 50)
	{
		//降りきった後の処理
		if (direction == E_LEFT)
		{
			location.x += 10.0f;
			jump_power = 10.0f;
			direction = E_RIGHT;
			anim = 5;
			ChangeAnim(E_GOAL);
			PlaySoundMem(se[9], DX_PLAYTYPE_BACK, TRUE);
		}
		//ポールにしがみついて降下している時の処理
		else
		{
			location.y += 5.0f;
			if (anim_count2 % 5 == 0)
			{
				if (anim == 8)anim = 7;
				else if (anim == 7)anim = 8;
			}
		}
	}

	//ゲームオーバー時のアニメーション
	if (now_anim == E_GAMEOVER)
	{
		if (anim_count2 > 50)
		{
			//重力
			jump_power -= GRAVITY / 2;
			location.y -= jump_power;
		}
		if (anim_count2 > 25 && anim_count2 < 27)
		{
			PlaySoundMem(se[0], DX_PLAYTYPE_BACK, TRUE);
		}
	}
	//点滅処理
	if (anim_count2 > 5 && now_anim != E_GAMEOVER && now_anim != E_CLING && now_anim != E_GOAL)
	{
		if (is_active == false)
		{
			flash_count++;

			if (flash_count > 5 || player_type == POWER_UP)
			{
				anim++;
				if (anim == 3)anim = 0;

				if (flash_count > 12)
				{
					if (player_type == POWER_UP)
					{
						ChangeType(SUPER);
						is_active = true;
						state = true;
						return;
					}
					else
					{
						ChangeType(NOMAL);
						state = true;
					}
				}
			}
			else
			{
				anim = 2;
			}
			if (flash_count >= 30)
			{
				is_active = true;
			}
		}

		anim_count2 = 0;
	}

	anim_count2++;
}

//アニメーションの切り替え処理
void Player::ChangeAnim(ePlayerAnim anim)
{
	//ジャンプして着地した時に呼ばれる想定の処理
	if (anim == E_IDOL)
	{
		count = 0;
		jump_power = 0;
	}

	if (now_anim != E_GAMEOVER && now_anim != E_GOAL)now_anim = anim;
}

//PLAYERのタイプ切り替え処理
void Player::ChangeType(ePlayerType type)
{

	if (player_type != NOMAL && type != POWER_DOWN)
	{
		//位置ズレを直す
		location.y += 16.0f;
	}
	if (player_type != SUPER && type != POWER_DOWN)
	{
		//位置ズレを直す
		location.y -= 16.0f;
	}

	player_type = type;

	if (type == SUPER)
	{
		this->box_size = Vector2D(16.0f, 32.0f);
		LoadDivGraph("Resource/1-1image/Mario/dekamario.png", 10, 10, 1, 32, 64, image);
	}
	if (type == NOMAL)
	{
		this->box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Mario/mario.png", 9, 9, 1, 32, 32, image);
	}
	if (type == POWER_DOWN)
	{
		LoadDivGraph("Resource/1-1image/Mario/dekamarimation.png", 3, 3, 1, 32, 64, image);
	}
	if (type == POWER_UP)
	{
		LoadDivGraph("Resource/1-1image/Mario/dekamarimation.png", 3, 3, 1, 32, 64, image);
	}
}

Vector2D Player::GetOffSet()
{
	if (location.x + box_size.x > 385)
	{
		return move;
	}
	else return 0;
}

int Player::GetPreset()
{
	int i = player_type;
	return i;
}
#include"Player.h"
#include"../Utility/InputControl.h"
#include<cmath>

#define MAX_SPEED 3.0f

Player::Player() : jump_power(0.0f), speed(0.0f), count(0), life(3), start_pos(0.0f), flash_count(0), anim_count2(0),overlap(0.0f)
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	ChangeAnim(E_IDOL);
	direction = E_RIGHT;
	this->location = Vector2D(500.0f, 300.0f);
	this->angle = 0.0f;
	anim = 1;
	anim_count = 0;
	this->obj_type = E_PLAYER;
	ChangeType(NOMAL);
	is_active = true;
	state = true;
	end_flg = false;
}

void Player::Update()
{

	// �߂荞�ݕ����炷
	location += overlap;
	overlap = Vector2D(0.0f);

	PlayerAnim();

	// �}���I�̃T�C�Y�ύX��
	if (state == false)
	{
		speed = 0.0f;
		return;
	}

	//Debug�p����
	if (location.y >= 405)
	{
		location.y = 405;
		jump_power = 0;
		ChangeAnim(E_IDOL);
	}

	//���Ⴊ�ݏ���
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) && player_type == SUPER)
	{
		ChangeAnim(E_SQUAT);
	}

	//�����Ă��鎞�̃A�j���[�V����
	if (anim_count > 5 - speed)
	{
		anim++;
		if (anim >= 4) anim = 1;

		anim_count = 0;
	}

	anim_count++;

	Movement();
}

void Player::Draw(Vector2D diff)
{
	//�`�r�}���I�̉摜�\������
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
				if (now_anim == E_RUN)
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
			if (now_anim == E_RUN)
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

	//�X�[�p�[�}���I�̉摜�\������
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
		if (now_anim == E_RUN)
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

	//�_�Ŏ��̉摜�\������
	if (player_type == FLASH)
	{
		if (anim_count2 > 5)
		{
			DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
		}
	}

	//�`�r�}���I����X�[�p�[�}���I�ɂȂ鎞�̏���
	if (player_type == POWER_UP)
	{
		DrawRotaGraph(location.x, location.y, 1.0f, angle, this->image[anim], TRUE, direction);
	}
}
int Player::Finalize()
{
	return 0;
}

void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);

	//���ړ�����
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT) && now_anim != E_JUMP)
	{
		//�u���[�L����
		if (direction == E_RIGHT && speed > 0.0f)
		{
			ChangeAnim(E_BRAKE);
			speed -= MAX_SPEED / 25;
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
	//�E�ړ�����
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT) && now_anim != E_JUMP)
	{
		//�u���[�L����
		if (direction == E_LEFT && speed > 0.0f)
		{
			ChangeAnim(E_BRAKE);
			speed -= MAX_SPEED / 25;
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

	//����
	if (move == 0.0f && now_anim != E_JUMP && now_anim != E_BRAKE) {
		if (speed > 0.0f)
		{
			ChangeAnim(E_RUN);
			speed -= MAX_SPEED / 50;
		}
		else
		{
			speed = 0.0f;
		}
	}

	//�d��
	if (jump_power > -12.0f)
	{
		jump_power -= GRAVITY;
	}

	//�W�����v����
	if (E_JUMP != now_anim)
	{
		if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
		{
			jump_power = 10.0f;
			ChangeAnim(E_JUMP);
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
}

//�q�b�g������
void Player::OnHit(ObjectBase* obj)
{
	//ENEMY�̏ꍇ
	if (obj->GetObjectType() == E_ENEMY && is_active == true)
	{
		//�A�N�e�B�u�ȏ�Ԃ��H
		if (is_active == false)return;

		//ENEMY���A�N�e�B�u�ȏ�Ԃ��H
		if (obj->GetIsActive() == false)
		{
			return;
		}
		//�}���I���G�̓���ɂ��邩���肷��
		if (location.y < obj->GetLocation().y && jump_power < 0.0f)
		{
			//���݂��Ă����班�����˂�
			jump_power = 10.0f;
			speed = 3.0f;
		}
		else if (obj->GetIsActive() == true)
		{
			if (player_type == NOMAL)
			{
				life--;
				jump_power = 10.0f;
				is_active = false;
				state = false;
				ChangeAnim(E_GAMEOVER);
			}
			if (player_type == SUPER)
			{
				ChangeType(FLASH);
				is_active = false;
				state = false;
			}
		}
	}
	//BLOCK�̏ꍇ
	if (obj->GetObjectType() == E_BLOCK && now_anim != E_GAMEOVER || obj->GetObjectType() == E_CLAYPIPE)
	{
		//�|�[���ɂ����݂��ĉ����肫�������Ƀu���b�N�ɓ���������I��
		if (now_anim == E_CLING)
		{
			anim_count2 = 0;
			location.x += 17.0f;
			direction = E_LEFT;
			return;
		}

		//�u���b�L���O����
		//�ʒu���̍������擾
		Vector2D diff_location = location - obj->GetLocation();

		//�����蔻��T�C�Y�̑傫�����擾
		Vector2D box_ex = box_size + obj->GetSize();

		//�ǂꂾ���d�Ȃ��Ă��邩�H
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
	//ITEM�̏ꍇ
	if (obj->GetObjectType() == E_ITEM)
	{
		if (obj->GetIsActive())
		{
			if (obj->GetPreset() == 0)
			{
				life++;
			}
			if (obj->GetPreset() == 1)
			{
				ChangeType(POWER_UP);
				state = false;
				is_active = false;
			}
		}
	}

	//Pole�̏ꍇ
	if (obj->GetObjectType() == E_POLE && now_anim != E_CLING && obj->GetIsActive() != false)
	{
		state = false;
		anim = 8;
		now_anim = E_CLING;
	}
}

//PLAYER�̃A�j���[�V����
void Player::PlayerAnim()
{
	//�|�[���ɂ����݂������̃A�j���[�V����
	if (now_anim == E_CLING && anim_count2 > 50)
	{
		if (direction == E_LEFT)
		{
			location += 10.0f;
			now_anim = E_JUMP;
			state = true;
		}
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

	//�Q�[���I�[�o�[���̃A�j���[�V����
	if (now_anim == E_GAMEOVER)
	{
		if (anim_count2 > 50)
		{
			//�d��
			jump_power -= GRAVITY / 2;
			location.y -= jump_power;
		}
	}
	//�_�ŏ���
	if (anim_count2 > 5 && now_anim != E_GAMEOVER && now_anim != E_CLING)
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

//�A�j���[�V�����̐؂�ւ�����
void Player::ChangeAnim(ePlayerAnim anim)
{
	if (anim == E_IDOL)
	{
		count = 0;
		jump_power = 0;
	}

	if (now_anim != E_GAMEOVER)now_anim = anim;
}

//PLAYER�̃^�C�v�؂�ւ�����
void Player::ChangeType(ePlayerType type)
{

	if (player_type != NOMAL && type != FLASH)
	{
		//�ʒu�Y���𒼂�
		location.y += 16.0f;
	}
	if (player_type != SUPER && type != FLASH)
	{
		//�ʒu�Y���𒼂�
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
	if (type == FLASH)
	{
		LoadDivGraph("Resource/1-1image/Mario/dekamarimation.png", 3, 3, 1, 32, 64, image);
	}
	if (type == POWER_UP)
	{
		LoadDivGraph("Resource/1-1image/Mario/dekamarimation.png", 3, 3, 1, 32, 64, image);
	}
}

float Player::GetOffSet()
{
	return sizeof(this->location.x);
}

int Player::GetPreset()
{
	int i = player_type;
	return i;
}
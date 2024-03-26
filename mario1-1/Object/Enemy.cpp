#include"Enemy.h"
#include<cmath>

Enemy::Enemy() : speed(0.0f),g_speed(0.0f)
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	location = Vector2D(400.0f, 250.f);
	anim = 0;
	anim_count = 0;
	count = 0;
	angle = 0.0f;
	obj_type = E_ENEMY;
	now_anim = E_IDOL_ENEMY;
	ChangeType(E_KOOPTROOPA);
	direction = E_RIGHT;
	is_active = true;
	
}

void Enemy::Update()
{
	//�A�N�e�B�u�ȏ�Ԃ���Ȃ�������ENEMY�̏I���A�j���[�V����������
	if (is_active == false)
	{
		if (now_anim = E_HIT_ENEMY)
		{
			return;
		}
		//�d��
		g_speed += GRAVITY;
		location.y += g_speed;
		return;
	}

	//�d��
	g_speed += GRAVITY;
	location.y += g_speed;

	//�����Ă��鎞�̃A�j���[�V����
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

void Enemy::Draw(float diff)
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

void Enemy::Finalize()
{
	for (int i=0; i > 3; i++)
	{
		DeleteGraph(image[i]);
	}
}

int Enemy::GetPreset()
{
	int i = enemy_type;
	return i;
}

void Enemy::ChangeAnim(eEnemyAnim anim)
{
	now_anim = anim;
}

void Enemy::OnHit(ObjectBase* obj)
{
	//ENEMY���A�N�e�B�u�ȏ�Ԃ��H
	if (is_active == false)
	{
		return;
	}

	//PLAYER�̏ꍇ
	if (obj->GetObjectType() == E_PLAYER && obj->GetIsActive() == true)
	{
		//�m�R�m�R���b�����Ă��Ă��鎞�ɓ���������]�����Ă���
		if (enemy_type == E_KOOPTROOPA_HIDE)
		{
			ChangeAnim(E_IDOL_ENEMY);
			speed = 10.0f;
			if (obj->GetLocation().x > location.x)direction = E_RIGHT;
			if (obj->GetLocation().x < location.x)direction = E_LEFT;
		}

		//�}���I���G�̓���ɂ��邩���肷��
		if (location.y - box_size.y < obj->GetLocation().y + obj->GetSize().y && 
			location.y > obj->GetLocation().y + obj->GetSize().y )
		{
			if (enemy_type == E_KOOPTROOPA)
			{
				ChangeAnim(E_HIT_ENEMY);
				ChangeType(E_KOOPTROOPA_HIDE);
			}
			if (enemy_type == E_GOOMBA)
			{
				ChangeAnim(E_HIT_ENEMY);
				is_active = false;
			}
		}
	}

	//BLOCK�̏ꍇ
	if (obj->GetObjectType() == E_BLOCK)
	{
		if (obj->GetIsActive())
		{
			is_active = false;
			location.x -= 20.0f;
			location.y -= 20.0f;
			angle = 3.0f;
			return;
		}
		//�u���b�L���O����
		//�ʒu���̍������擾
		Vector2D diff_location = location - obj->GetLocation();

		//�����蔻��T�C�Y�̑傫�����擾
		Vector2D box_ex = box_size + obj->GetSize();

		//�ǂꂾ���d�Ȃ��Ă��邩�H
		Vector2D overlap = box_ex.x - fabsf(diff_location.x);
		overlap.y = box_ex.y - fabsf(diff_location.y);

		if (diff_location.x != 0 || diff_location.y != 0)
		{
			//��ւƉ����o��
			if (diff_location.y < 0 )
			{
				g_speed = 0;
				location.y -= overlap.y;
				return;
			}
			//�E�ւƉ����o��
			if (diff_location.x > 0)
			{
				direction = E_LEFT;
				location.x += overlap.x;
				return;
			}
			//���ւƉ����o��
			if (diff_location.x < 0)
			{
				direction = E_RIGHT;
				location.x -= overlap.x;
				return;
			}
		}
	}
}

//ENEMY�̈ړ�����
void Enemy::Movement()
{
	Vector2D move = Vector2D(0.0f);

	if (now_anim == E_IDOL_ENEMY)
	{
		//���ړ�����
		if (direction == E_LEFT)
		{
			move += Vector2D(2.0f + speed, 0.0f);
		}
		//�E�ړ�����
		if (direction == E_RIGHT)
		{
			move += Vector2D(-2.0f - speed, 0.0f);
		}
	}

	location += move;
}

void Enemy::ChangeType(eEnemyType type)
{
	if (type == E_GOOMBA)
	{
		enemy_type = type;
		box_size = Vector2D(16.0f, 16.0f);
		LoadDivGraph("Resource/1-1image/Enemy/kuribo.png", 3, 3, 1, 32, 32, image);
	}
	if (type == E_KOOPTROOPA)
	{
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
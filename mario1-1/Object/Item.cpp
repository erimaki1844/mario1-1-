#include"Item.h"
#include<cmath>

Item::Item() : g_speed(0.0f)
{

}

Item::~Item()
{

}

void Item::Initialize()
{
	location = Vector2D(300.0f, 300.0f);
	box_size = Vector2D(16.0f, 16.0f);
	ChangeType(E_1UP);
	is_active = false;
	start_pos = location.y;
}

void Item::Update()
{
	if (is_active == true)
	{
		//�d��
		g_speed += GRAVITY;
		location.y += g_speed;
	}
	if (state == true)
	{
		if (location.y + 32.0f >= start_pos)
		{
			location.y -= 4.0f;
		}
		else
		{
			is_active = true;
			state = false;
		}
	}
}

void Item::Draw(float diff)
{
	DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[0], TRUE);
}

void Item::Finalize()
{

}

void Item::Movement()
{

}

void Item::OnHit(ObjectBase* obj)
{
	//���̃A�C�e�����A�N�e�B�u�ȏ�Ԃ��H
	if (is_active == false)return;

	//BLOCK�̏ꍇ
	if (obj->GetObjectType() == E_BLOCK)
	{
		if (obj->GetIsActive())
		{
			state = true;
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
			if (diff_location.y < 0)
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

void Item::ChangeType(eItemType type)
{
	if (type == E_1UP)
	{
		LoadDivGraph("Resource/1-1image/Item/1up_mushroom.png", 1, 1, 1, 32, 32, image);
	}
	if (type == E_SUPER)
	{

	}
	if (type == E_COIN)
	{

	}
}
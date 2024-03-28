#include"Block.h"
#include"Player.h"

Block::Block() :anim_flg(true)
{

}

Block::~Block()
{

}

void Block::Initialize()
{
	if (block_type == E_NONE)
	{
		Finalize();
	}
	state = false;
	is_active = false;
	this->location = Vector2D(400.0f, 300.0f);
	block_type = E_ITEMBLOCK;
	this->box_size = Vector2D(16.0f,16.0f);
	this->obj_type = E_BLOCK;
	pos = location.y;
	//LoadDivGraph("Resource/images/block222.png", 7, 7, 1, 30, 30, image);
	LoadDivGraph("Resource/1-1image/Block/allblock.png", 10, 10, 1, 32, 32, image);
}

void Block::Update()
{
	if (state == true)
	{
		if (pos - 10.0f < location.y && anim_flg == true)
		{
			location.y -= 2.0f;
		}
		else
		{
			anim_flg = false;
		}
		if (pos > location.y && anim_flg == false)
		{
			location.y += 2.0f;
		}
		else if (anim_flg == false)
		{
			anim_flg = true;
			state = false;
			is_active = false;
			location.y = pos;
		}
	}
}

void Block::Draw(Vector2D diff)
{
	float diff_location = this->location.x - diff.x;
	//DrawBox(location.x + diff_location, location.y, location.x + box_size.x + diff_location, location.y + box_size.y, 0x555555, TRUE);
	DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[block_type],FALSE);
	//DrawFormatString(0, 60, 0xFFFFFF, "%f", diff);
}

void Block::Finalize()
{
	DeleteGraph(image[block_type]);
}

void Block::SetLocation(Vector2D location)
{
	this->location = location;
}

void Block::SetType(int handle)
{
	switch (handle)
	{
	case 0:
		 block_type = E_NONE;
		 break;
	case 1:
		block_type = E_NOMAL;
		break;
	case 2:
		block_type = E_BRICK;
		break;
	case 3:
		block_type = E_ITEMBLOCK;
		break;
	case 4:
		block_type = E_HIDEBLOCK;
		break;
	default:
		break;
	}
}

void Block::OnHit(ObjectBase* obj)
{
	if (block_type == E_NOMAL)
	{
		return;
	}
	if (obj->GetObjectType() == E_PLAYER)
	{
		//Player‚ª‰º‘¤‚©‚ç“–‚½‚Á‚½Žž‚Ìˆ—
		if (obj->GetLocation().y - obj->GetSize().y < location.y + box_size.y && obj->GetLocation().y - obj->GetSize().y > location.y)
		{
			if (state == false)
			{
				state = true;
				is_active = true;
			}
			if (block_type == E_BRICK)
			{
				if (obj->GetPreset() == 0)
				{
					DeleteGraph(image[1]);
					block_type = E_NONE;
				}
			}
			if (block_type == E_ITEMBLOCK)
			{
				block_type = E_NOMAL;
			}
			if (block_type == E_HIDEBLOCK)
			{
				block_type = E_NOMAL;
			}
		}
	}
}
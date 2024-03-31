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
	anim = 0;
	anim_count = 0;
	state = false;
	is_active = false;
	end_flg = false;
	//this->location = Vector2D(400.0f, 300.0f);
	//block_type = E_ITEMBLOCK;
	this->box_size = Vector2D(16.0f,16.0f);
	this->obj_type = E_BLOCK;
	pos = location.y;
}

void Block::Update()
{
	if (block_type == E_ITEMBLOCK)
	{
		if (anim == 0 && anim_count > 20)
		{
			anim++;
			anim_count = 0;
		}
		else if (anim != 0 && anim_count > 8)
		{
			anim++;
			if (anim >= 4) anim = 0;

			anim_count = 0;
		}

		anim_count++;
	}

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
	DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[anim],FALSE);
	
}

int Block::Finalize()
{
	DeleteGraph(image[block_type]);
	return 0;
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
		image[0] = LoadGraph("Resource/1-1image/Block/kara_block.png");
		break;
	case 2:
		block_type = E_BRICK;
		image[0] = LoadGraph("Resource/1-1image/Block/block.png");
		break;
	case 3:
		block_type = E_ITEMBLOCK;
		LoadDivGraph("Resource/1-1image/Block/hatena.png", 4, 4, 1, 32, 32, image);
		break;
	case 4:
		block_type = E_HIDEBLOCK;
		image[0] = LoadGraph("Resource/1-1image/Block/block.png");
		break;
	case 5:
		block_type = E_ITEMBRICK;
		image[0] = LoadGraph("Resource/1-1image/Block/block.png");
		break;
	case 6:
		block_type = E_FLOORBLOCK;
		image[0] = LoadGraph("Resource/1-1image/Block/floor.png");
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
		//Playerが下側から当たった時の処理
		if (obj->GetLocation().y - obj->GetSize().y < location.y + box_size.y && obj->GetLocation().y - obj->GetSize().y > location.y)
		{
			if (state == false)
			{
				state = true;
				is_active = true;
			}
			if (block_type == E_BRICK)
			{
				//マリオがスーパーマリオか確認する
				if (obj->GetPreset() == 0)
				{
					DeleteGraph(image[1]);
					
				}
			}
			if (block_type == E_ITEMBLOCK)
			{
				
				SetType(1);
			}
			if (block_type == E_HIDEBLOCK)
			{
				SetType(1);
			}
		}
	}
}
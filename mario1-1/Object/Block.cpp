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
	g_speed = 0.0f;
	state = false;
	is_active = false;
	end_flg = false;
	move = Vector2D(0.0f);
	this->box_size = Vector2D(16.0f,16.0f);
	this->obj_type = E_BLOCK;
	pos = location.y;

	//SEの読み込み
	se[0] = LoadSoundMem("Resource/sound/SE_Block.wav");
	se[1] = LoadSoundMem("Resource/sound/SE_BlockBreak.wav");
	se[2] = LoadSoundMem("Resource/sound/SE_ItemAppearance.wav");

	for (int i = 0; i < 3; i++)
	{
		ChangeVolumeSoundMem(100, this->se[i]);
	}
}

void Block::Update(Vector2D diff)
{
	//Playerとのズレを加算する
	location.x -= diff.x;

	//画面内に入ってから動くようにする
	if (location.x > 650.0f)
	{
		return;
	}

	//画面外に完全に出たら終了する
	if (location.x < -64.0f)
	{
		end_flg = true;
	}

	BlockAnim();
}

void Block::Draw()
{
	if (block_type == E_BRICKBREAK)
	{
		DrawRotaGraph(location.x + move.x, location.y - move.y, 1.0f, 0.0f, this->image[anim], TRUE, E_RIGHT);
		DrawRotaGraph(location.x + move.x - 0.25f, location.y - move.y/2, 1.0f, 0.0f, this->image[anim], TRUE, E_RIGHT);
		DrawRotaGraph(location.x - move.x, location.y - move.y, 1.0f, 0.0f, this->image[anim], TRUE, E_LEFT);
		DrawRotaGraph(location.x - move.x - 0.25f, location.y - move.y /2, 1.0f, 0.0f, this->image[anim], TRUE, E_LEFT);
	}
	else DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, this->image[anim],FALSE);
}

int Block::Finalize()
{
	DeleteGraph(image[block_type]);
	return 0;
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
		image[1] = LoadGraph("Resource/1-1image/Block/bloak_1.png");
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
	case 7:
		block_type = E_KAIBLOCK;
		image[0] = LoadGraph("Resource/1-1image/Block/kai_block.png");
		break;
	default:
		break;
	}
}

void Block::OnHit(ObjectBase* obj)
{
	if (block_type == E_NOMAL || block_type == E_FLOORBLOCK || block_type == E_KAIBLOCK || block_type == E_BRICKBREAK)
	{
		return;
	}
	if (obj->GetObjectType() == E_PLAYER)
	{
		//プレイヤーの状態をチェックする
		if (obj->GetState() == false) return;

		//Playerが下側から当たった時の処理
		if (obj->GetG_SPEED() > 1.0f && obj->GetLocation().y > location.y)
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
					anim = 1;
					g_speed = 10.0f;
					block_type = E_BRICKBREAK;
					PlaySoundMem(se[1], DX_PLAYTYPE_BACK, TRUE);
				}
				else
				{
					PlaySoundMem(se[0], DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (block_type == E_ITEMBLOCK)
			{
				anim = 0;
				SetType(1);
			}
			if (block_type == E_HIDEBLOCK)
			{
				SetType(1);
				PlaySoundMem(se[2], DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
}

void Block::BlockAnim()
{
	//アイテムブロックのアニメーション
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

	//プレイヤーがブロックに当たった時のアニメーション
	if (state == true && block_type != E_BRICKBREAK)
	{
		//移動量
		move = Vector2D(0.0f);

		if (pos - 10.0f < location.y && anim_flg == true)
		{
			move.y -= 2.0f;
		}
		else
		{
			anim_flg = false;
		}
		if (pos > location.y && anim_flg == false)
		{
			move.y += 2.0f;
		}
		else if (anim_flg == false)
		{
			anim_flg = true;
			state = false;
			is_active = false;
			location.y = pos;
		}

		location += move;
	}

	//れんがブロックが破壊された時のアニメーション
	if (block_type == E_BRICKBREAK)
	{
		move.x += 1.0f;
		move.y += g_speed;
		//重力
		if (g_speed > 0.0f)
		{
			g_speed -= GRAVITY / 5;
		}
		else end_flg = true;
	}
}

int Block::GetPreset()
{
	int type = block_type;
	return type;
}
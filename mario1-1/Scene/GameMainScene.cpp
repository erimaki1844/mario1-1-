#include"GameMainScene.h"
#include"../Object/Player.h"
#include"../Object//Enemy.h"
#include"../Object/Block.h"
#include"../Object/Item.h"
#include"../Object/Pole.h"
#include"../Object/ClayPipe.h"
#include"../Object/Bakuha.h"
#include"../Utility/CSVHandle.h"

GameMainScene::GameMainScene()
{

}

GameMainScene::~GameMainScene()
{

}

void GameMainScene::Initialize()
{
	//�����������ŏ��������Ă���A���LoadingScene�ŏ���������
	score = 0;
	coin = 0;
	life = 3;
	time = 400;

	bakuha_count = 4;
	goal_flg = false;
	bakuha_flg = false;
	pos = Vector2D(0.0f);
	flag_pos = 54.0f;
	diff = 0.0f;
	diff_location = Vector2D(0.0f);

	//�摜�̓ǂݍ���
	background_img = LoadGraph("Resource/1-1image/background/background.png");
	siro_img[0] = LoadGraph("Resource/1-1image/siro1.png");
	siro_img[1] = LoadGraph("Resource/1-1image/flag_siro.png");
	se[1] = LoadSoundMem("Resource/sound/SE_Goal.wav");

	//SE�̓ǂݍ���
	bgm = LoadSoundMem("Resource/sound/BGM_MarioGround.wav");

	ChangeVolumeSoundMem(80, bgm);

	obj.push_back(new Player);
	
	LoadStageCSV();

	for (int i = 0; i < obj.size(); i++)
	{
		obj[i]->Initialize();
	}
	
	//obj[4]->SetLocation(Vector2D(600.0f, 388.0f));
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, TRUE);
}

eSceneType GameMainScene::Update()
{
	fps++;
	//�^�C�}�[
	if (fps == 24 && goal_flg == false)
	{
		fps = 0;
		time--;

		if (time == 0)
		{
			if (life == 0)
			{
				return E_END;
			}
			else
			{
				life--;
				return E_LOADING;
			}
		}
	}

	//�S�[������
	//if (obj[0]->GetLocation().x > pos.x)
	//{
	//	obj[0]->Finalize();
	//	
	//	if (time != 0)
	//	{
	//		score += 50;
	//		time--;
	//	}
	//	else if (flag_pos < 96.0f)flag_pos += 2.0f;

	//	if (flag_pos >= 96.0f)
	//	{
	//		if (bakuha_count != 0) bakuha_flg = true;

	//		if (bakuha_flg == true)
	//		{
	//			//�X�e�[�W�N���A��̔����𐶐�����
	//			obj.push_back(new Bakuha);
	//			obj.back()->Initialize();
	//			obj.back()->SetLocation(Vector2D(pos.x,pos.y - flag_pos));
	//			obj.back()->SetType(bakuha_count);
	//			bakuha_flg = false;
	//			bakuha_count--;
	//		}
	//		else
	//		{
	//			if (obj.back()->GetObjectType() == E_BAKUHA)
	//			{
	//				obj.back()->Update(diff);
	//			}
	//			else return E_TITLE;

	//			if (obj.back()->GetObjectType() == E_BAKUHA && obj.back()->GetEndFlg())
	//			{
	//				delete obj.back();
	//				obj.pop_back();
	//			}
	//		}
	//	}

	//	goal_flg = true;

	//	return GetNowScene();
	//}

	//end_flg��ture�ɂȂ��Ă�����̏I�����������Ă�
	for (int i = 0; i < 100; i++)
	{
		//if (obj[i]->GetLocation().x > 800.0f)break;
		if (obj[i]->GetEndFlg())
		{
			if (obj[i]->GetObjectType() == E_ITEM && obj[i]->GetPreset() == 2)
			{
				coin++;
			}
			if (obj[i]->GetObjectType() == E_ITEM && obj[i]->GetPreset() == 0)
			{
				life++;
			}
			if (obj[i]->GetObjectType() == E_PLAYER)
			{
				if (life == 0)
				{
					return E_END;
				}
				else
				{
					life--;
					return E_LOADING;
				}
			}
			else if(obj[i]->GetObjectType() != E_POLE)
			{
				obj[i]->Finalize();
				obj.erase(obj.begin() + i);
			}
		}
	}

	//�����蔻��`�F�b�N
	for (int i = 0; i < obj.size(); i++)
	{
		//if (obj[i]->GetLocation().x > 800.0f)break;
		for (int j = 0; j < 100; j++)
		{
			//if (obj[j]->GetLocation().x > 800.0f)break;
			if (i != j)
			{
				if (IsHitCheck(obj[i], obj[j]))
				{
					obj[i]->OnHit(obj[j]);
					obj[j]->OnHit(obj[i]);

					if (obj[i]->GetEndFlg())
					{
						//score += obj[i]->GetScore();
					}
					if (obj[i]->GetObjectType() == E_PLAYER)
					{
						if (obj[j]->GetObjectType() == E_POLE && obj[j]->GetEndFlg() == false)
						{
							score += obj[j]->GetScore();
						}
					}
				}
			}
		}
	}

	//�I�u�W�F�N�g�̍X�V����
	for (int i = 0; i < obj.size(); i++)
	{
		//if (obj[i]->GetLocation().x > 800.0f)break;

		//PLAYER���A�j���[�V�������̎�����
		if (obj[0]->GetState() == false)
		{
			if (obj[i]->GetObjectType() == E_POLE || obj[i]->GetObjectType() == E_PLAYER)
			{
				obj[i]->Update(diff_location);
			}
		}
		else obj[i]->Update(diff_location);
	}

	//player�Ƃ̃Y���̍��v
	diff += fabsf(obj[0]->GetOffSet().x);
	//1F���Ƃ̃Y��
	diff_location = obj[0]->GetOffSet();
	
	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}

void GameMainScene::Draw() const
{
	DrawGraph(-diff % 1504, 0, background_img, FALSE);
	DrawGraph(-diff % 1504 + 1504, 0, background_img, FALSE);
	DrawRotaGraph(pos.x - diff, pos.y - flag_pos, 1.0f, 0.0, siro_img[1], TRUE);
	//DrawRotaGraph(pos.x - diff, pos.y,1.0f,0.0, siro_img[0], TRUE);
	
	for (int i = 1; i < 100; i++)
	{
		//if (obj[i]->GetLocation().x > 800.0f)break;
		obj[i]->Draw();
	
	}
	//Player�̕`����Ō��
	obj[0]->Draw();
}

void GameMainScene::Finalize()
{
	StopSoundMem(bgm);

	if (goal_flg == true)
	{
		score += coin * 100;
		score += life * 1000;
		if (coin == 13)
		{
			life++;
		}
	}
}
 
eSceneType GameMainScene::GetNowScene() const
{
	return eSceneType::E_MAIN;
}

bool GameMainScene::IsHitCheck(ObjectBase* obj1, ObjectBase* obj2)
{
	//�ʒu���̍������擾
	Vector2D diff_location = obj1->GetLocation() - obj2->GetLocation();

	//�����蔻��T�C�Y�̑傫�����擾
	Vector2D box_ex = obj1->GetSize() + obj2->GetSize();

	//�R���W�����f�[�^���ʒu���̍������������Ȃ�A�q�b�g����Ƃ���
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

void GameMainScene::LoadStageCSV()
{
	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/map_chip.csv", "r");

	if (result != 0)
	{
		throw("CSV�t�@�C�����J���܂���ł���\n");
	}

	// CSVFile<�^��> data;
	CSVFile<int> data;
	// data.csv_read(���̓t�@�C����, �w�b�_�[�̗L��, �C���f�b�N�X�̗L��, ��؂蕶��);
	data.csv_read("Resource/dat/map_chip.csv", true, true, ',');

	Vector2D map_chip;
	
	for (int i = 0; i < data.header.size(); i++)
	{
		for (int j = 0; j < data.index.size(); j++)
		{
			//�u���b�N�ǂݍ���
			if (data.cell.at(j).at(i) != 0 && data.cell.at(j).at(i) != 8 && data.cell.at(j).at(i) / 10 == 0)
			{
				map_chip.x = (i * 32)-16;
				map_chip.y = (j * 32)-16;
				obj.push_back(new Block);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(j).at(i));
			}
			//�A�C�e���ǂݍ���
			if (data.cell.at(j).at(i) / 10 == 1)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new Item);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(j).at(i) - 10);
				obj.push_back(new Block);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(3);
			}
			//�G�l�~�[�ǂݍ���
			if (data.cell.at(j).at(i) / 10 == 2)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new Enemy);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(j).at(i) - 20);
			}
			//�p�C�v�ǂݍ���
			if (data.cell.at(j).at(i) / 10 == 3)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new ClayPipe);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetPreset(data.cell.at(j).at(i) - 30);
			}
			//�|�[���ǂݍ���
			if (data.cell.at(j).at(i) == 8)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new Pole);
				obj.back()->SetLocation(map_chip);
				pos.x = map_chip.x + 192;
				pos.y = map_chip.y - 32;
			}
		}
	}
}

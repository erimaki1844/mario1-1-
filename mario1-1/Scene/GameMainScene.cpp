#include"GameMainScene.h"
#include"Player.h"
#include"Block.h"
#include"Enemy.h"
#include"Item.h"
#include"CSVHandle.h"

GameMainScene::GameMainScene()
{

}

GameMainScene::~GameMainScene()
{

}

void GameMainScene::Initialize()
{
	obj.push_back(new Player);
	//obj.push_back(new Enemy);
	//obj.push_back(new Item);
	obj.push_back(new Block);
	//LoadStageCSV();
	
	for (int i = 0; i < obj.size(); i++)
	{
		obj[i]->Initialize();
	}
}

eSceneType GameMainScene::Update()
{
	for (int i = 0; i < obj.size(); i++)
	{
		for (int j = 0; j < obj.size(); j++)
		{
			if (i != j)
			{
				if (IsHitCheck(obj[i], obj[j]))
				{
					obj[i]->OnHit(obj[j]);
					obj[j]->OnHit(obj[i]);
				}
			}
		}
	}

	for (int i = 0; i < obj.size(); i++)
	{
		if (obj[0]->GetState() == false)
		{
			obj[0]->Update();
			//���݂̃V�[���^�C�v��Ԃ�
			return GetNowScene();
		}
		obj[i]->Update();
	}

	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}

void GameMainScene::Draw() const
{
	for (int i = 0; i < obj.size(); i++)
	{
		obj[i]->Draw(obj[0]->GetOffSet());
	}
}

void GameMainScene::Finalize()
{

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

	for (int i = 0; i < data.cell.size(); i++)
	{
		for (int j = 0; j < data.cell.at(i).size(); j++)
		{
			if (data.cell.at(i).at(j) != 0)
			{
				map_chip.x = i * 32;
				map_chip.y = j * 32;
				obj.push_back(new Block);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(i).at(j));
			}
		}
	}
}
#include"GameMainScene.h"
#include"../Object/Player.h"
#include"../Object//Enemy.h"
#include"../Object/Block.h"
#include"../Object/Item.h"
#include"../Object/Pole.h"
#include"../Object/ClayPipe.h"
#include"../Utility/CSVHandle.h"

GameMainScene::GameMainScene()
{

}

GameMainScene::~GameMainScene()
{

}

void GameMainScene::Initialize()
{
	//今だけここで初期化している、後でLoadingSceneで初期化する
	score = 0;
	coin = 0;

	obj.push_back(new Player);
	obj.push_back(new Enemy);
	obj.push_back(new Item);
	//obj.push_back(new ClayPipe);
	//obj.push_back(new Pole);
	/*obj.push_back(new Block);
	obj.push_back(new Block);
	obj.push_back(new Block);
	obj.push_back(new Block);*/
	LoadStageCSV();

	for (int i = 0; i < obj.size(); i++)
	{
		obj[i]->Initialize();
	}
	
	//obj[4]->SetLocation(Vector2D(600.0f, 388.0f));
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
			obj[2]->Update();
			//現在のシーンタイプを返す
			return GetNowScene();
		}
		obj[i]->Update();
	}

	for (int i = 0; i < obj.size(); i++)
	{
		if (obj[i]->GetEndFlg())
		{
			if (obj[i]->GetObjectType() == E_ITEM && obj[i]->GetPreset() == 2)
			{
				coin++;
			}
			else
			{
				score += obj[i]->Finalize();
				delete obj[i];
				obj.erase(obj.begin() + i);
			}
		}
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}

void GameMainScene::Draw() const
{
	for (int i = 1; i < obj.size(); i++)
	{
		obj[i]->Draw(obj[0]->GetOffSet());
	}
	//Playerの描画を最後に
	obj[0]->Draw(obj[0]->GetOffSet());
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
	//位置情報の差分を取得
	Vector2D diff_location = obj1->GetLocation() - obj2->GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = obj1->GetSize() + obj2->GetSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

void GameMainScene::LoadStageCSV()
{
	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/map_chip.csv", "r");

	if (result != 0)
	{
		throw("CSVファイルが開けませんでした\n");
	}

	// CSVFile<型名> data;
	CSVFile<int> data;
	// data.csv_read(入力ファイル名, ヘッダーの有無, インデックスの有無, 区切り文字);
	data.csv_read("Resource/dat/map_chip.csv", true, true, ',');

	Vector2D map_chip;

	for (int i = 0; i < data.cell.size(); i++)
	{
		for (int j = 0; j < data.cell.at(i).size(); j++)
		{
			if (data.cell.at(i).at(j) != 0)
			{
				map_chip.x = j * 32;
				map_chip.y = i * 32;
				obj.push_back(new Block);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(i).at(j));
			}
		}
	}
}

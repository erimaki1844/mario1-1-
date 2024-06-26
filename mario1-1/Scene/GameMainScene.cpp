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
	LoadDataCSV();
	time = 400;

	bakuha_count = 0;
	goal_flg = false;
	bakuha_flg = false;
	pos = Vector2D(0.0f);
	flag_pos = 54.0f;
	diff_location = Vector2D(0.0f);

	//画像の読み込み
	background_img = LoadGraph("Resource/1-1image/background/background.png");
	siro_img[0] = LoadGraph("Resource/1-1image/siro1.png");
	siro_img[1] = LoadGraph("Resource/1-1image/flag_siro.png");
	se[1] = LoadSoundMem("Resource/sound/SE_Goal.wav");

	obj.push_back(new Player);
	
	LoadStageCSV();

	for (int i = 0; i < obj.size(); i++)
	{
		obj[i]->Initialize();
	}
	
}

eSceneType GameMainScene::Update()
{
	fps++;
	//タイマー
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

	//ゴール処理
	if (obj[0]->GetLocation().x > pos.x - diff_location.x)
	{
		obj[0]->Finalize();
		
		if (time != 0)
		{
			score += 50;
			time--;
		}
		else if (flag_pos < 96.0f)flag_pos += 2.0f;

		if (flag_pos >= 96.0f)
		{
			if (bakuha_count >= 0) bakuha_flg = true;

			if (bakuha_flg == true)
			{
				//ステージクリア後の爆発を生成する
				obj.push_back(new Bakuha);
				obj.back()->Initialize();
				obj.back()->SetLocation(Vector2D(pos.x - diff_location.x,pos.y - flag_pos));
				obj.back()->SetType(bakuha_count);
				bakuha_flg = false;
				bakuha_count--;
			}
			else
			{
				if (obj.back()->GetObjectType() == E_BAKUHA)
				{
					obj.back()->Update(diff_location.x);
				}
				else
				{
					score = 0;
					coin = 0;
					life = 3;
					return E_TITLE;
				}

				if (obj.back()->GetObjectType() == E_BAKUHA && obj.back()->GetEndFlg())
				{
					delete obj.back();
					obj.pop_back();
				}
			}
		}

		goal_flg = true;

		return GetNowScene();
	}

	//end_flgがtureになっているもの終了時処理を呼ぶ
	for (int i = 0; i < obj.size(); i++)
	{
		if (obj[i]->GetObjectType() == E_ENEMY)
		{
			if (obj[i]->GetPreset() == 2 && obj[i]->GetLocation().x > 680.0f)
			{
				obj[i]->Finalize();
				obj.erase(obj.begin() + i);
			}
		}
		if (obj[i]->GetLocation().x > 680.0f)break;
		if (obj[i]->GetEndFlg())
		{
			if (obj[i]->GetObjectType() == E_ITEM && obj[i]->GetHitFlg())
			{
				if (obj[i]->GetPreset() == 2 && obj[i]->GetIsActive())
				{
					coin++;
				}
				if (obj[i]->GetPreset() == 0 && obj[i]->GetHitFlg())
				{
					life++;
				}

				score += obj[i]->GetScore();
			}
			if (obj[i]->GetObjectType() == E_ENEMY && obj[i]->GetIsActive() == false)
			{
				score += obj[i]->GetScore();
			}
			if (obj[i]->GetObjectType() == E_PLAYER)
			{
				if (life == 1)
				{
					score = 0;
					coin = 0;
					life = 3;
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

	//当たり判定チェック
	for (int i = 0; i < obj.size(); i++)
	{
		if (obj[i]->GetLocation().x > 680.0f)break;
		for (int j = 0; j < obj.size(); j++)
		{
			if (obj[i]->GetLocation().x > 680.0f)break;
			if (i != j)
			{
				if (IsHitCheck(obj[i], obj[j]))
				{
					//PLAYERの処理を最優先にする
					if (obj[i]->GetObjectType() == E_PLAYER)
					{
						obj[0]->OnHit(obj[j]);
						obj[j]->OnHit(obj[i]);
					}
					else if (obj[j]->GetObjectType() == E_PLAYER)
					{
						obj[i]->OnHit(obj[j]);
					}
					else
					{
						obj[i]->OnHit(obj[j]);
						obj[j]->OnHit(obj[i]);
					}
					//ゴールした時の処理
					if (obj[i]->GetObjectType() == E_PLAYER)
					{
						if (obj[j]->GetObjectType() == E_POLE && obj[j]->GetEndFlg() == false)
						{
							score += obj[j]->GetScore();
							if (time % 10 == 1)bakuha_count = 1;
							if (time % 10 == 3)bakuha_count = 2;
							if (time % 10 == 6)bakuha_count = 3;
						}
					}
				}
			}
		}
	}

	//オブジェクトの更新処理
	for (int i = 0; i < obj.size(); i++)
	{
		//PLAYERがアニメーション中の時処理
		if (obj[0]->GetState() == false)
		{
			if (obj[i]->GetObjectType() == E_POLE || obj[i]->GetObjectType() == E_PLAYER || obj[i]->GetObjectType() == E_BLOCK)
			{
				obj[i]->Update(obj[0]->GetOffSet());
			}
		}
		else obj[i]->Update(obj[0]->GetOffSet());
	}

	//playerとのズレの合計
	diff_location += obj[0]->GetOffSet().x;
	
	//現在のシーンタイプを返す
	return GetNowScene();
}

void GameMainScene::Draw() const
{
	DrawGraph(-diff_location.x, 0, background_img, FALSE);
	DrawGraph(-diff_location.x + 1504, 0, background_img, FALSE);
	DrawGraph(-diff_location.x + 1504 * 2, 0, background_img, FALSE);
	DrawGraph(-diff_location.x + 1504 * 3, 0, background_img, FALSE);
	DrawGraph(-diff_location.x + 1504 * 4, 0, background_img, FALSE);
	DrawRotaGraph(pos.x - diff_location.x, pos.y - flag_pos, 1.0f, 0.0, siro_img[1], TRUE);
	DrawRotaGraph(pos.x - diff_location.x, pos.y,1.0f,0.0, siro_img[0], TRUE);
	
	//オブジェクトの描画処理
	for (int i = 1; i < obj.size(); i++)
	{
		if (obj[i]->GetLocation().x > 680.0f)break;
		obj[i]->Draw();
	}
	//Playerの描画を最後に
	obj[0]->Draw();
}

void GameMainScene::Finalize()
{
	StopSoundMem(bgm);


	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "w");

	if (result != 0)
	{
		throw("CSVファイルが開けませんでした\n");
	}

	char name[3][10] = { { 'l','i','f','e' },{ 's','c','o','r','e'},{'c','o','i','n'}};
	int data[3] = { life,score,coin };

	for (int  i = 0; i < 3; i++)
	{
		fprintf(fp, "%s,%d\n", name[i], data[i]);
	}
	
	//ファイルクローズ
	fclose(fp);
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
	
	for (int i = 0; i < data.header.size(); i++)
	{
		for (int j = 0; j < data.index.size(); j++)
		{
			//ブロック読み込み
			if (data.cell.at(j).at(i) != 0 && data.cell.at(j).at(i) != 8 && data.cell.at(j).at(i) / 10 == 0)
			{
				map_chip.x = (i * 32)-16;
				map_chip.y = (j * 32)-16;
				obj.push_back(new Block);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(j).at(i));
			}
			//アイテム読み込み
			if (data.cell.at(j).at(i) / 10 == 1)
			{
				if (data.cell.at(j).at(i) == 10)
				{
					map_chip.x = (i * 32) - 16;
					map_chip.y = (j * 32) - 16;
					obj.push_back(new Item);
					obj.back()->SetLocation(Vector2D(map_chip.x, map_chip.y - 10.0f));
					obj.back()->SetType(data.cell.at(j).at(i) - 10);
					obj.push_back(new Block);
					obj.back()->SetLocation(map_chip);
					obj.back()->SetType(4);
				}
				else
				{
					map_chip.x = (i * 32) - 16;
					map_chip.y = (j * 32) - 16;
					obj.push_back(new Item);
					obj.back()->SetLocation(Vector2D(map_chip.x, map_chip.y - 10.0f));
					obj.back()->SetType(data.cell.at(j).at(i) - 10);
					obj.push_back(new Block);
					obj.back()->SetLocation(map_chip);
					obj.back()->SetType(3);
				}
			}
			//エネミー読み込み
			if (data.cell.at(j).at(i) / 10 == 2)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new Enemy);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetType(data.cell.at(j).at(i) - 20);
			}
			//パイプ読み込み
			if (data.cell.at(j).at(i) / 10 == 3)
			{
				map_chip.x = (i * 32) - 16;
				map_chip.y = (j * 32) - 16;
				obj.push_back(new ClayPipe);
				obj.back()->SetLocation(map_chip);
				obj.back()->SetPreset(data.cell.at(j).at(i) - 30);
			}
			//ポール読み込み
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

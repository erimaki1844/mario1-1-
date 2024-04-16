#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"DxLib.h"
#include"../Object/ObjectBase.h"
#include<vector>
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<string.h>

//シーン情報
enum eSceneType
{
	E_TITLE,
	E_LOADING,
	E_MAIN,
	E_END,
	END
};

//基底シーンクラス
class SceneBase
{
protected:
	std::vector<ObjectBase*> obj;
	int score;
	int coin;
	int time;
	int life;

public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//初期化処理
	virtual void Initialize() {}
	//更新処理
	virtual eSceneType Update()
	{
		return GetNowScene();
	}
	//描画処理
	virtual void Draw() const {}
	//終了時処理
	virtual void Finalize() {}

	//現在のシーン情報を取得
	virtual eSceneType GetNowScene() const = 0;

	//スコア
	int GetScore()
	{
		return score;
	}

	//コイン
	int GetCoin()
	{
		return coin;
	}

	//タイマー
	int GetTime()
	{
		return time;
	}

	void LoadDataCSV()
	{
		FILE* fp = nullptr;

		errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "r");

		if (result != 0)
		{
			throw("CSVファイルが開けませんでした\n");
		}

		char buf[512];

		while (fgets(buf, 512, fp) != NULL)
		{
			//lifeの値を読み込む
			if (buf[0] == 'l')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						this->life = buf[i] - '0';
					}
				}
			}
			//スコアの値を読み込む
			else if (buf[0] == 's')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						if (this->score >= 1)
						{
							this->score = this->score * 10 + (buf[i] - '0');
						}
						else this->score = buf[i] - '0';
					}
				}
			}
			//コインの値を読み込む
			else if (buf[0] == 'c')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						if (this->coin >= 1)
						{
							this->coin = this->coin * 10 + (buf[i] - '0');
						}
						else this->coin = buf[i] - '0';
					}
				}
			}
		}

		//ファイルクローズ
		fclose(fp);
	}
};
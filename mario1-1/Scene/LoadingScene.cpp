#include"LoadingScene.h"
#include"../Object/Player.h"

LoadingScene::LoadingScene() : count(0)
{
	for (int i = 0; i < 3; i++)
	{
		mario_data[i] = NULL;
	}
}

LoadingScene::~LoadingScene()
{
	obj.push_back(new Player);
}

void LoadingScene::Initialize()
{
	LoadDataCSV();

	LoadDivGraph("Resource/1-1image/Mario/mario.png", 9, 9, 1, 32, 32, image);
	LoadDivGraph("Resource/1-1image/UI/num.png", 15, 15, 1, 16, 16, num_img);
}

eSceneType LoadingScene::Update()
{
	count++;

	if (count >= 300)
	{
		return eSceneType::E_MAIN;
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}

void LoadingScene::Draw() const
{
	DrawString(0, 0, "LoadingScene", 0xFFFFFF);
	DrawRotaGraph(288, 240, 1.0, 0.0, image[0], TRUE);
	DrawRotaGraph(320.0f, 240.0f, 1.0f, 0.0f, num_img[11], TRUE);
	DrawRotaGraph(352.0f, 240.0f, 1.0f, 0.0f, num_img[mario_data[0]], TRUE);
}

void LoadingScene::Finalize()
{

}

eSceneType LoadingScene::GetNowScene() const
{
	return eSceneType::E_LOADING;
}

void LoadingScene::LoadDataCSV()
{
	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "r");

	if (result != 0)
	{
		throw("CSVファイルが開けませんでした\n");
	}

	char buf[512];
	char life[10] = {};
	char score[20] = {};
	char coin[10] = {};
	int ret;

	while (fgets(buf, 512, fp) != NULL)
	{
		//lifeの値を読み込む
		if (buf[0] == 'l')
		{
			ret = sscanf(buf, "%s", life);

			for (int i = 0; i < 10; i++)
			{
				if (life[i] >= '0' && life[i] <= '9') 
				{
					mario_data[0] = life[i] - '0';
				}
			}
		}
		//スコアの値を読み込む
		else if (buf[0] == 's')
		{
			ret = sscanf(buf, "%s", score);

			for (int i = 0; i < 10; i++)
			{
				if (score[i] >= '0' && score[i] <= '9')
				{
					if (mario_data[1] >= 1)
					{
						mario_data[1] = mario_data[1] * 10 + (score[i] - '0');
					}
					else mario_data[1] = score[i] - '0';
				}
			}

			this->score = mario_data[1];
		}
	}

	//ファイルクローズ
	fclose(fp);
}
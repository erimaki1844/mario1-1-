#include"SceneManager.h"
#include"DxLib.h"
#include"../Utility/InputControl.h"
#include"TitleScene.h"
#include"LoadingScene.h"
#include"GameMainScene.h"
#include"EndScene.h"

SceneManager::SceneManager() : current_scene(nullptr)
{
	
}

SceneManager::~SceneManager()
{

}

//�V�[���}�l�W���[�@�\:����������
void SceneManager::Initialize()
{
	//�E�B���h�E�̃^�C�g����ݒ�
	SetMainWindowText("�}���I");

	//�E�B���h�E���[�h�ŋN��
	if (ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK)
	{
		throw("�E�B���h�E���[�h�ŋN���ł��܂���ł���\n");
	}
	//��16�@�c14
	SetGraphMode(640, 480, 32);

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1)
	{
		throw("Dx���C�u�������������ł��܂���ł���\n");
	}

	//�`���w�菈��
	if (SetDrawScreen(DX_SCREEN_BACK) == -1)
	{
		throw("�`���̎w�肪�ł��܂���ł���\n");
	}

	//�^�C�g���V�[������n�߂�
	ChangeScene(eSceneType::E_MAIN);

	//UI�𐶐�����
	ui = new UI;
	ui->Initialize();
}

//�V�[���}�l�[�W���[�@�\:�X�V����
void SceneManager::Update()
{
	//�t���[���J�n���ԁi�}�C�N���b�j���擾
	LONGLONG start_time = GetNowHiPerformanceCount();
	
	//���C�����[�v
	while (ProcessMessage() != -1)
	{
		//���ݎ��Ԃ��擾
		LONGLONG now_time = GetNowHiPerformanceCount();

		//1�t���[��������̎��Ԃɓ��B������A�X�V����ѕ`�揈�����s��
		if ((now_time - start_time) >= DELTA_SECOND)
		{
			//�t���[���J�n���Ԃ��X�V����
			start_time = now_time;

			//���͋@�\:�X�V����
			InputControl::Update();

			//UI�̍X�V����
			ui->Update(current_scene->GetTime(), current_scene->GetScore(), current_scene->GetCoin());

			//�X�V�����i�߂�l�͎��̃V�[�����j
			eSceneType next = current_scene->Update();

			//�`�揈��
			Draw();

			//�G���h���I������Ă�����A�Q�[�����I������
			if (next == eSceneType::END)
			{
				break;
			}

			//���݂̃V�[���Ǝ��̃V�[��������Ă�����A�؂�ւ��������s��
			if (next != current_scene->GetNowScene())
			{
				ChangeScene(next);
			}
		}

		//ESCAPE�L�[�������ꂽ��A�Q�[�����I������
		if (CheckHitKey(KEY_INPUT_ESCAPE) || InputControl::GetButtonUp(XINPUT_BUTTON_BACK))
		{
			break;
		}
	}
}

//�V�[���}�l�[�W���[�@�\�F�I��������
void SceneManager::Finalize()
{
	//�V�[�����̍폜
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}

	//data.csv�̏�����
	FILE* fp = nullptr;

	errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "w");

	if (result != 0)
	{
		throw("CSV�t�@�C�����J���܂���ł���\n");
	}

	char name[3][10] = { { 'l','i','f','e' },{ 's','c','o','r','e'},{'c','o','i','n'} };
	int data[3] = { 3,0,0 };

	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%s,%d\n", name[i], data[i]);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	//DX���C�u�����̎g�p���I������
	DxLib_End();
}

//�V�[���}�l�[�W���[�@�\�F�`�揈��
void SceneManager::Draw() const
{
	//��ʂ̏�����
	ClearDrawScreen();

	//�V�[���̕`��
	current_scene->Draw();

	//UI�̕`��
	ui->Draw();

	//����ʂ̓��e��\��ʂɔ��f
	ScreenFlip();
}

//�V�[���؂�ւ�����
void SceneManager::ChangeScene(eSceneType scene_type)
{
	//�V�[���𐶐�����iSceneBase���p������Ă��邩�H�j
	SceneBase* new_scene = dynamic_cast<SceneBase*>(CreateScene(scene_type));

	//�G���[�`�F�b�N
	if (new_scene == nullptr)
	{
		throw("�V�[���������ł��܂���ł����B\n");
	}

	//�O��V�[���̏I�����������s��
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	//�V�������������V�[���̏��������s��
	new_scene->Initialize();

	//���݂̃V�[���̍X�V
	current_scene = new_scene;
}

//�V�[����������
SceneBase* SceneManager::CreateScene(eSceneType scene_type)
{
	//�����i�V�[���^�C�v�j�ɂ���āA��������V�[�������肷��
	switch (scene_type)
	{
	case eSceneType::E_TITLE:
		return new TitleScene;
	case eSceneType::E_LOADING:
		return new LoadingScene;
	case eSceneType::E_MAIN:
		return new GameMainScene;
	case eSceneType::E_END:
		return new EndScene;
	default:
		return nullptr;
	}
}
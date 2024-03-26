#include"SceneManager.h"
#include"DxLib.h"
#include"InputControl.h"
#include"TitleScene.h"
#include"LoadingScene.h"
#include"GameMainScene.h"

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
	SetGraphMode(780, 420, 32);

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

			//�`�揈��
			Draw();

			//�X�V�����i�߂�l�͎��̃V�[�����j
			eSceneType next = current_scene->Update();

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
	default:
		return nullptr;
	}
}
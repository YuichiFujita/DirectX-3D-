//======================================================================================================================
//
//	�Q�[����ʏ��� [game.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "game.h"

#include "billboard.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "field.h"
#include "light.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "object.h"
#include "particle.h"
#include "pause.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
GAMESTATE g_gameState;			// �Q�[���̏��
int  g_nCounterGameState;		// ��ԊǗ��J�E���^�[
bool g_bPause;					// �|�[�Y��Ԃ� ON / OFF

//======================================================================================================================
//	�Q�[����ʂ̏���������
//======================================================================================================================
void InitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�Q�[���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �O���[�o���ϐ���������
	g_gameState         = GAMESTATE_NORMAL;		// �ʏ��Ԃɏ�����
	g_nCounterGameState = 0;					// �J�E���^�[��������
	g_bPause            = false;				// �|�[�Y�� OFF �ɂ���

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���V�����_�[�̏�����
	InitMeshCylinder();

#if 0
	// ���̏�����
	InitField();

	// �ǂ̏�����
	InitWall();
#else
	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���E�H�[���̏�����
	InitMeshWall();
#endif

	// �e�̏�����
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�̏�����
	InitEnemy();

	// �I�u�W�F�N�g�̏�����
	InitObject();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �e�̏�����
	InitBullet();

	// �����̏�����
	InitExplosion();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �|�[�Y�̏�����
	InitPause();

#if 0
	// ���̐ݒ�
	SetField(D3DXVECTOR3(-200.0f, 0.0f,  200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetField(D3DXVECTOR3( 200.0f, 0.0f,  200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetField(D3DXVECTOR3( 200.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetField(D3DXVECTOR3(-200.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �ǂ̐ݒ�
	SetWall(D3DXVECTOR3( 0.0f,   0.0f,  400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(0),   0.0f));
	SetWall(D3DXVECTOR3( 400.0f, 0.0f,  0.0f),   D3DXVECTOR3(0.0f, D3DXToRadian(90),  0.0f));
	SetWall(D3DXVECTOR3( 0.0f,   0.0f, -400.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180), 0.0f));
	SetWall(D3DXVECTOR3(-400.0f, 0.0f,  0.0f),   D3DXVECTOR3(0.0f, D3DXToRadian(270), 0.0f));
#else
	//// ���b�V���t�B�[���h�̐ݒ�
	//SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//// ���b�V���E�H�[���̐ݒ�
	//SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
#endif

	// �G�̐ݒ�
	SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(-100.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(   0.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3( 100.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3( 200.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	// �I�u�W�F�N�g�̐ݒ�
	SetObject(D3DXVECTOR3(-100.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-160.0f, 0.0f, 0.0f),  D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-100.0f, 0.0f, 60.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	SetObject(D3DXVECTOR3(-160.0f, 0.0f, 60.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	SetObject(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
	SetObject(D3DXVECTOR3(180.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2);

	// �r���{�[�h�̐ݒ�
	SetBillboard(D3DXVECTOR3( 400.0f, 50.0f, 0.0f));
	SetBillboard(D3DXVECTOR3(-400.0f, 50.0f, 0.0f));
}

//======================================================================================================================
//	�Q�[����ʂ̏I������
//======================================================================================================================
void UninitGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏I��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���b�V���V�����_�[�̏I��
	UninitMeshCylinder();

#if 0
	// ���̏I��
	UninitField();

	// �ǂ̏I��
	UninitWall();
#else
	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���E�H�[���̏I��
	UninitMeshWall();
#endif

	// �e�̏I��
	UninitShadow();

	// �v���C���[�̏I��
	UninitPlayer();

	// �G�̏I��
	UninitEnemy();

	// �I�u�W�F�N�g�̏I��
	UninitObject();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �e�̏I��
	UninitBullet();

	// �����̏I��
	UninitExplosion();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �p�[�e�B�N���̏I��
	UninitParticle();

	// �|�[�Y�̏I��
	UninitPause();
}

//======================================================================================================================
//	�Q�[����ʂ̍X�V����
//======================================================================================================================
void UpdateGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�Q�[���̍X�V
	//------------------------------------------------------------------------------------------------------------------
	switch (g_gameState)
	{ // ��Ԃ��Ƃ̏���
	case GAMESTATE_NORMAL:

		if (GetFade() == FADE_NONE)
		{ // �t�F�[�h���Ă��Ȃ��ꍇ

			if (GetKeyboardTrigger(DIK_P) == true || GetJoyKeyTrigger(JOYKEY_START, 0) == true)
			{ // �|�[�Y�̑��삪�s��ꂽ�ꍇ

				// �T�E���h�̍Đ�
				//PlaySound(SOUND_LABEL_SE_DECISION_01);		// ���艹01 (�V�X�e��)

				// �|�[�Y�̑I�����ڂ̍Đݒ�
				ResetPauseSelect();

				// �ϐ� (g_bPause) �^�U�������� (�O�����Z�q)	// ���������^�Ȃ� true �A�U�Ȃ� false ����
				g_bPause = (g_bPause == false) ? true : false;	// �� �������F() �̒�
			}
		}

#ifdef _DEBUG	// �f�o�b�O����
		if (GetKeyboardTrigger(DIK_F4) == true)
		{ // [F4] �������ꂽ�ꍇ

			// �Q�[����ʂ��I����Ԃɂ���
			g_gameState = GAMESTATE_END;
		}
#endif

		// �����𔲂���
		break;

	case GAMESTATE_END:

		// �ϐ� (g_nCounterGameState) �����Z
		g_nCounterGameState--;

		if (g_nCounterGameState <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// ���[�h�ݒ� (�Q�[����ʂɈڍs)
			SetFade(MODE_TITLE);
		}

		// �����𔲂���
		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̍X�V
	//------------------------------------------------------------------------------------------------------------------
	if (g_bPause == false)
	{ // �|�[�Y��Ԃł͂Ȃ��ꍇ

		// ���C�g�̍X�V
		UpdateLight();

		// ���b�V���V�����_�[�̍X�V
		UpdateMeshCylinder();

#if 0
		// ���̍X�V
		UpdateField();
	
		// �ǂ̍X�V
		UpdateWall();
#else
		// ���b�V���t�B�[���h�̍X�V
		UpdateMeshField();

		// ���b�V���E�H�[���̍X�V
		UpdateMeshWall();
#endif
		// �e�̍X�V
		UpdateShadow();

		// �v���C���[�̍X�V
		UpdatePlayer();

		// �G�̍X�V
		UpdateEnemy();

		// �I�u�W�F�N�g�̍X�V
		UpdateObject();

		// �J�����̍X�V
		UpdateCamera();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		// �e�̍X�V
		UpdateBullet();

		// �����̍X�V
		UpdateExplosion();

		// �G�t�F�N�g�̍X�V
		UpdateEffect();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();
	}
	else
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̍X�V
		UpdatePause();
	}
}

//======================================================================================================================
//	�Q�[����ʂ̕`�揈��
//======================================================================================================================
void DrawGame(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera();

	// ���b�V���V�����_�[�̕`��
	DrawMeshCylinder();

#if 0
	// ���̕`��
	DrawField();

	// �ǂ̕`��
	DrawWall();
#else
	// ���b�V���t�B�[���h�̕`��
	DrawMeshField();

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();
#endif
	// �e�̕`��
	DrawShadow();

	// �v���C���[�̕`��
	DrawPlayer();

	// �G�̕`��
	DrawEnemy();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �p�[�e�B�N���̕`��
	DrawParticle();

	// �e�̕`��
	DrawBullet();

	// �����̕`��
	DrawExplosion();

	if (g_bPause == true)
	{ // �|�[�Y��Ԃ̏ꍇ

		// �|�[�Y�̕`��
		DrawPause();
	}
}

//======================================================================================================================
//	�Q�[����ʂ̏�Ԑݒ菈��
//======================================================================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	// �ϐ� (g_gameState) �Ɉ��� (state) ����
	g_gameState = state;				// ��Ԃ�ݒ�

	// �ϐ� (g_nCounterGameState) �Ɉ��� (nCounter) ����
	g_nCounterGameState = nCounter;		// �]�C�̒�����ݒ�
}

//======================================================================================================================
//	�|�[�Y��Ԃ̐ݒ菈��
//======================================================================================================================
void SetEnablePause(bool bPause)
{
	// ���݂̃|�[�Y��ԂɈ����̃|�[�Y��Ԃ���
	g_bPause = bPause;
}

//======================================================================================================================
//	�Q�[����ʂ̏�Ԏ擾����
//======================================================================================================================
GAMESTATE GetGameState(void)
{
	// �ϐ� (g_gameState) �̒l��Ԃ�
	return g_gameState;
}

//======================================================================================================================
//	�|�[�Y��Ԃ̎擾����
//======================================================================================================================
bool GetPause(void)
{
	// �|�[�Y��Ԃ̐^�U��Ԃ�
	return g_bPause;
}
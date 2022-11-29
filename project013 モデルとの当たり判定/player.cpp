//======================================================================================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "bullet.h"
#include "camera.h"
#include "object.h"
#include "player.h"
#include "shadow.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define SETUP_TXT			"data\\TXT\\motion.txt"		// �Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X

#define MAX_STRING			(128)		// �Z�b�g�A�b�v���̃e�L�X�g�ǂݍ��ݕ�����̍Œ�

#define MOVE_PLAYER			(1.2f)		// �v���C���[�̈ړ���
#define DELETE_MOVE			(0.01f)		// �v���C���[�̈ړ��ʂ��c��Œ�l
#define SUB_MOVE_PLAYER		(0.25f)		// �v���C���[�̈ړ��ʂ̌����W��
#define REV_ROT_PLAYER		(0.1f)		// �v���C���[�̉�]�ʂ̕␳�W��

#define POS_BULL_PLAYER_X	(30.0f)		// �v���C���[�̒e���ˈʒu�̉��Z�� (x)
#define POS_BULL_PLAYER_Y	(17.0f)		// �v���C���[�̒e���ˈʒu�̉��Z�� (y)
#define POS_BULL_PLAYER_Z	(30.0f)		// �v���C���[�̒e���ˈʒu�̉��Z�� (z)

#define MOVE_BULLET			(6.0f)		// �e�̈ړ���
#define SHADOW_PLAYER		(35.0f)		// �v���C���[�̉e�̔��a

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apModelPlayer[] =			// ���f���̑��΃p�X
{
	"data\\CHARA_MODEL\\body.x",		// �̂̃��f�����΃p�X
	"data\\CHARA_MODEL\\head.x",		// ���̃��f�����΃p�X
	"data\\CHARA_MODEL\\sleeve_l.x",	// ���r�̃��f�����΃p�X
	"data\\CHARA_MODEL\\sleeve_r.x",	// �E�r�̃��f�����΃p�X
	"data\\CHARA_MODEL\\wrist_l.x",		// �����̃��f�����΃p�X
	"data\\CHARA_MODEL\\wrist_r.x",		// �E���̃��f�����΃p�X
	"data\\CHARA_MODEL\\hand_l.x",		// ����̃��f�����΃p�X
	"data\\CHARA_MODEL\\hand_r.x",		// �E��̃��f�����΃p�X
	"data\\CHARA_MODEL\\pants_l.x",		// �����̃��f�����΃p�X
	"data\\CHARA_MODEL\\pants_r.x",		// �E���̃��f�����΃p�X
	"data\\CHARA_MODEL\\leg_l.x",		// ���ڂ̃��f�����΃p�X
	"data\\CHARA_MODEL\\leg_r.x",		// �E�ڂ̃��f�����΃p�X
	"data\\CHARA_MODEL\\shoes_l.x",		// ���C�̃��f�����΃p�X
	"data\\CHARA_MODEL\\shoes_r.x",		// �E�C�̃��f�����΃p�X
	"data\\CHARA_MODEL\\knife.x",		// �i�C�t�̃��f�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (MODEL_PLAYER)
//**********************************************************************************************************************
typedef enum
{
	MODEL_PLAYER_BODY = 0,				// ��
	MODEL_PLAYER_HEAD,					// ��
	MODEL_PLAYER_SLEEVE_L,				// ���r
	MODEL_PLAYER_SLEEVE_R,				// �E�r
	MODEL_PLAYER_WRIST_L,				// �����
	MODEL_PLAYER_WRIST_R,				// �E���
	MODEL_PLAYER_HAND_L,				// ����
	MODEL_PLAYER_HAND_R,				// �E��
	MODEL_PLAYER_PANTS_L,				// ����
	MODEL_PLAYER_PANTS_R,				// �E��
	MODEL_PLAYER_LEG_L,					// ����
	MODEL_PLAYER_LEG_R,					// �E��
	MODEL_PLAYER_SHOES_L,				// ���C
	MODEL_PLAYER_SHOES_R,				// �E�C
	MODEL_PLAYER_KNIFE,					// �i�C�t
	MODEL_PLAYER_MAX,					// ���̗񋓌^�̑���
} MODEL_PLAYER;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void TxtSetPlayer(void);				// �v���C���[�̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Player g_aPlayer[MODEL_PLAYER_MAX];		// �v���C���[�̏��

//======================================================================================================================
//	�v���C���[�̏���������
//======================================================================================================================
void InitPlayer(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL      *pMat;					// �}�e���A���ւ̃|�C���^

	// �v���C���[�̏��̏�����
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		g_aPlayer[nCntPlayer].apTexture[TEXTURE_PLAYER_MAX] = {};								// �e�N�X�`���ւ̃|�C���^
		g_aPlayer[nCntPlayer].pMesh                         = NULL;								// ���b�V�� (���_���) �ւ̃|�C���^
		g_aPlayer[nCntPlayer].pBuffMat                      = NULL;								// �}�e���A���ւ̃|�C���^
		g_aPlayer[nCntPlayer].dwNumMat                      = 0;								// �}�e���A���̐�
		g_aPlayer[nCntPlayer].pos                           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���݂̈ʒu
		g_aPlayer[nCntPlayer].oldPos                        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
		g_aPlayer[nCntPlayer].move                          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aPlayer[nCntPlayer].rot                           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aPlayer[nCntPlayer].nCameraBackCounter            = 0;								// �J�����̉�荞�݃J�E���^�[
		g_aPlayer[nCntPlayer].nParentID                     = NONE_PLAYER_PARENT;				// �v���C���[�̐e�̃C���f�b�N�X
		g_aPlayer[nCntPlayer].nShadowID                     = NONE_SHADOW;						// �e�̃C���f�b�N�X
	}

	// �v���C���[�̃Z�b�g�A�b�v
	TxtSetPlayer();

	// �e�̃C���f�b�N�X��ݒ�
	g_aPlayer[0].nShadowID = SetShadow(SHADOW_PLAYER, NULL);	// �e�ɂ̂ݐݒ�

	// x�t�@�C���̓ǂݍ���
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		D3DXLoadMeshFromX
		( // ����
			apModelPlayer[nCntPlayer],			// ���f�� (�p�[�c) �̑��΃p�X
			D3DXMESH_SYSTEMMEM,
			pDevice,							// �f�o�C�X�ւ̃|�C���^
			NULL,
			&g_aPlayer[nCntPlayer].pBuffMat,	// �}�e���A���ւ̃|�C���^
			NULL,
			&g_aPlayer[nCntPlayer].dwNumMat,	// �}�e���A���̐�
			&g_aPlayer[nCntPlayer].pMesh		// ���b�V�� (���_���) �ւ̃|�C���^
		);
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // �e�N�X�`���t�@�C�������݂���ꍇ

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aPlayer[nCntPlayer].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	�v���C���[�̏I������
//======================================================================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		for (int nCntMat = 0; nCntMat < TEXTURE_PLAYER_MAX; nCntMat++)
		{ // �g�p����e�N�X�`�������J��Ԃ�

			if (g_aPlayer[nCntPlayer].apTexture[nCntMat] != NULL)
			{ // �ϐ� (g_aPlayer[nCntPlayer].apTexture) ��NULL�ł͂Ȃ��ꍇ

				g_aPlayer[nCntPlayer].apTexture[nCntMat]->Release();
				g_aPlayer[nCntPlayer].apTexture[nCntMat] = NULL;
			}
		}
	}

	// ���b�V���̔j��
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		if (g_aPlayer[nCntPlayer].pMesh != NULL)
		{ // �ϐ� (g_aPlayer[nCntPlayer].pMesh) ��NULL�ł͂Ȃ��ꍇ

			g_aPlayer[nCntPlayer].pMesh->Release();
			g_aPlayer[nCntPlayer].pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		if (g_aPlayer[nCntPlayer].pBuffMat != NULL)
		{ // �ϐ� (g_aPlayer[nCntPlayer].pBuffMat) ��NULL�ł͂Ȃ��ꍇ

			g_aPlayer[nCntPlayer].pBuffMat->Release();
			g_aPlayer[nCntPlayer].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	�v���C���[�̍X�V����
//======================================================================================================================
void UpdatePlayer(void)
{
	// �ϐ���錾
	float fDestRot = g_aPlayer[0].rot.y;	// �v���C���[�̖ڕW��������p
	float fDiffRot = 0.0f;					// �v���C���[�̌����v�Z����p

	// �|�C���^��錾
	Camera *pCamera = GetCamera();			// �J�����̏��

	//--------------------------------------------------------------------------------------------------------------
	//	�O��ʒu�̍X�V
	//--------------------------------------------------------------------------------------------------------------
	g_aPlayer[0].oldPos = g_aPlayer[0].pos;

	//------------------------------------------------------------------------------------------------------------------
	//	�ړ��ʂ̍X�V (�O�㍶�E)
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_UP) == true)
	{ // [��] �������ꂽ�ꍇ

		if (GetKeyboardPress(DIK_LEFT) == true)
		{ // [��] �������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(135) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{ // [��] �������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(225) + pCamera->rot.y;
		}
		else
		{ // [��] �����������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x += sinf(pCamera->rot.y) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(180) + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{ // [��] �������ꂽ�ꍇ

		if (GetKeyboardPress(DIK_LEFT) == true)
		{ // [��] �������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(45) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{ // [��] �������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(315) + pCamera->rot.y;
		}
		else
		{ // [��] �����������ꂽ�ꍇ

			// �ʒu���X�V
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y) * MOVE_PLAYER;

			// �ڕW�������X�V
			fDestRot = D3DXToRadian(0) + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{ // [��] �������ꂽ�ꍇ

		// �ʒu���X�V
		g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;

		// �ڕW�������X�V
		fDestRot = D3DXToRadian(90) + pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{ // [��] �������ꂽ�ꍇ

		// �ʒu���X�V
		g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;

		// �ڕW�������X�V
		fDestRot = D3DXToRadian(270) + pCamera->rot.y;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�ړ��ʂ̍X�V (�㉺)
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_I) == true)
	{ // [I] �������ꂽ�ꍇ

		// �ʒu���X�V
		g_aPlayer[0].move.y += MOVE_PLAYER;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{ // [K] �������ꂽ�ꍇ

		// �ʒu���X�V
		g_aPlayer[0].move.y -= MOVE_PLAYER;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�ʒu�̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// �ʒu�̍X�V
	g_aPlayer[0].pos += g_aPlayer[0].move;

	//------------------------------------------------------------------------------------------------------------------
	//	�ړ��ʂ̌����E�폜
	//------------------------------------------------------------------------------------------------------------------
	// �ړ��ʂ̌���
	g_aPlayer[0].move.x += (0.0f - g_aPlayer[0].move.x) * SUB_MOVE_PLAYER;
	g_aPlayer[0].move.y += (0.0f - g_aPlayer[0].move.y) * SUB_MOVE_PLAYER;
	g_aPlayer[0].move.z += (0.0f - g_aPlayer[0].move.z) * SUB_MOVE_PLAYER;

	// �ړ��ʂ̍폜
	if (g_aPlayer[0].move.x <  DELETE_MOVE
	&&  g_aPlayer[0].move.x > -DELETE_MOVE)
	{ // �v���C���[�̈ړ��� (x) ���폜����͈͓��̏ꍇ

		// �v���C���[�̈ړ��� (x) ���폜����
		g_aPlayer[0].move.x = 0.0f;
	}

	if (g_aPlayer[0].move.y <  DELETE_MOVE
	&&  g_aPlayer[0].move.y > -DELETE_MOVE)
	{ // �v���C���[�̈ړ��� (y) ���폜����͈͓��̏ꍇ

		// �v���C���[�̈ړ��� (y) ���폜����
		g_aPlayer[0].move.y = 0.0f;
	}

	if (g_aPlayer[0].move.z <  DELETE_MOVE
	&&  g_aPlayer[0].move.z > -DELETE_MOVE)
	{ // �v���C���[�̈ړ��� (z) ���폜����͈͓��̏ꍇ

		// �v���C���[�̈ړ��� (z) ���폜����
		g_aPlayer[0].move.z = 0.0f;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�����̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// �ڕW�����̐��K��
	if      (fDestRot >  D3DX_PI) { fDestRot -= D3DX_PI * 2; }
	else if (fDestRot < -D3DX_PI) { fDestRot += D3DX_PI * 2; }

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = fDestRot - g_aPlayer[0].rot.y;

	// ���������̐��K��
	if      (fDiffRot >  D3DX_PI) { fDiffRot -= D3DX_PI * 2; }
	else if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2; }

	// �����̍X�V
	g_aPlayer[0].rot.y += fDiffRot * REV_ROT_PLAYER;

	// �����̐��K��
	if      (g_aPlayer[0].rot.y >  D3DX_PI) { g_aPlayer[0].rot.y -= D3DX_PI * 2; }
	else if (g_aPlayer[0].rot.y < -D3DX_PI) { g_aPlayer[0].rot.y += D3DX_PI * 2; }

	//------------------------------------------------------------------------------------------------------------------
	//	�J�����̉�荞�݃J�E���^�[�̍X�V
	//------------------------------------------------------------------------------------------------------------------
	if (g_aPlayer[0].move == D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	&&  g_aPlayer[0].nCameraBackCounter  < CNT_ROT_FOLLOW)
	{ // �ړ����Ă��Ȃ����A�J�E���^�[�����l�����̏ꍇ

		// �J�E���^�[�����Z
		g_aPlayer[0].nCameraBackCounter++;
	}
	else if (g_aPlayer[0].move != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{ // �ړ����Ă���ꍇ

		// �J�E���^�[��������
		g_aPlayer[0].nCameraBackCounter = 0;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�ʒu�̕␳
	//------------------------------------------------------------------------------------------------------------------
	if (g_aPlayer[0].pos.z > STAGE_LIMIT_NEAR)
	{ // �͈͊O�̏ꍇ (��O)

		// ��O�Ɉʒu��␳
		g_aPlayer[0].pos.z = STAGE_LIMIT_NEAR;
	}
	if (g_aPlayer[0].pos.z < STAGE_LIMIT_FAR)
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_aPlayer[0].pos.z = STAGE_LIMIT_FAR;
	}
	if (g_aPlayer[0].pos.x > STAGE_LIMIT_RIGHT)
	{ // �͈͊O�̏ꍇ (�E)

		// �E�Ɉʒu��␳
		g_aPlayer[0].pos.x = STAGE_LIMIT_RIGHT;
	}
	if (g_aPlayer[0].pos.x < STAGE_LIMIT_LEFT)
	{ // �͈͊O�̏ꍇ (��)

		// ���Ɉʒu��␳
		g_aPlayer[0].pos.x = STAGE_LIMIT_LEFT;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�����蔻��
	//------------------------------------------------------------------------------------------------------------------
	// �I�u�W�F�N�g�Ƃ̓����蔻��
	CollisionObject(&g_aPlayer[0]);

	//------------------------------------------------------------------------------------------------------------------
	//	�e�̍X�V
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_SPACE) == true)
	{ // [SPACE] �������ꂽ�ꍇ

		// �e�̐ݒ�
		SetBullet
		( // ����
			D3DXVECTOR3
			( // ����
				g_aPlayer[0].pos.x + sinf(g_aPlayer[0].rot.y + D3DX_PI) * POS_BULL_PLAYER_X,	// �ʒu (x)
				g_aPlayer[0].pos.y + POS_BULL_PLAYER_Y,											// �ʒu (y)
				g_aPlayer[0].pos.z + cosf(g_aPlayer[0].rot.y + D3DX_PI) * POS_BULL_PLAYER_Z		// �ʒu (z)
			),
			D3DXVECTOR3
			( // ����
				sinf(g_aPlayer[0].rot.y + D3DX_PI) * MOVE_BULLET,	// �ړ��� (x)
				0.0f,												// �ړ��� (y)
				cosf(g_aPlayer[0].rot.y + D3DX_PI) * MOVE_BULLET	// �ړ��� (z)
			),
			BULLETTYPE_PLAYER	// ���
		);
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�e�̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// �e�̈ʒu�ݒ�
	SetPositionShadow(g_aPlayer[0].nShadowID, g_aPlayer[0].pos);
}

//======================================================================================================================
//	�v���C���[�̕`�揈��
//======================================================================================================================
void DrawPlayer(void)
{
	// �ϐ���錾
	int          nCntParts = 0;					// �p�[�c�̐e�̊m�F�p
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �ϐ��z���錾
	D3DXMATRIX mtxRot[MODEL_PLAYER_MAX];		// �����̌v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans[MODEL_PLAYER_MAX];		// �ʒu�̌v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	// �����E�ʒu�̎擾
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

		// �������擾
		D3DXMatrixRotationYawPitchRoll(&mtxRot[nCntPlayer], g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);

		// �ʒu���擾
		D3DXMatrixTranslation(&mtxTrans[nCntPlayer], g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
	}

	// �����E�ʒu�̔��f
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		// �����𔽉f
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot[nCntPlayer]);

		// �ʒu�𔽉f
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans[nCntPlayer]);

		// ���݂̃��f�� (�p�[�c) �̃C���f�b�N�X����
		nCntParts = nCntPlayer;

		while (g_aPlayer[nCntParts].nParentID != NONE_PLAYER_PARENT)
		{ // ���݂̃p�[�c�ɐe�����݂���ꍇ

			// �����𔽉f
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot[g_aPlayer[nCntParts].nParentID]);

			// �ʒu�𔽉f
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans[g_aPlayer[nCntParts].nParentID]);

			// ���݂̃p�[�c�̐e�̃C���f�b�N�X����
			nCntParts = g_aPlayer[nCntParts].nParentID;
		}
	}

	// �}�e���A���E�e�N�X�`���̐ݒ�A�`��
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // �v���C���[�̃p�[�c�̍ő吔���J��Ԃ�

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aPlayer[nCntPlayer].apTexture[nCntMat]);

			// ���f�� (�p�[�c) �̕`��
			g_aPlayer[nCntPlayer].pMesh->DrawSubset(nCntMat);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================================================
//	�v���C���[�̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetPlayer(void)
{
	// �ϐ���錾
	int nID  = 0;				// �C���f�b�N�X�̑���p
	int nEnd = 0;				// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			//----------------------------------------------------------------------------------------------------------
			//	�u���b�N�̐ݒ�
			//----------------------------------------------------------------------------------------------------------
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);						// ���f�� (�p�[�c) �̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &g_aPlayer[nID].nParentID);	// ���f�� (�p�[�c) �̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ
								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.z);		// Z������ǂݍ���
							}

						} while (strcmp(&aString[0], "END_PARTSSET") != 0);		// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);			// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);													// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//======================================================================================================================
//	�v���C���[�̎擾����
//======================================================================================================================
Player *GetPlayer(void)
{
	// �v���C���[�̐e�p�[�c�̏��A�h���X��Ԃ�
	return &g_aPlayer[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�v���C���[�̍��W�̎擾����
//**********************************************************************************************************************
D3DXVECTOR3 GetPlayerPos(void)
{
	// ���W��Ԃ�
	return g_aPlayer[0].pos;
}

//**********************************************************************************************************************
//	�v���C���[�̌����̎擾����
//**********************************************************************************************************************
D3DXVECTOR3 GetPlayerRot(void)
{
	// ���W��Ԃ�
	return g_aPlayer[0].rot;
}

//**********************************************************************************************************************
//	�v���C���[�̍Đݒ菈��
//**********************************************************************************************************************
void ResetPlayer(void)
{
	// �O���[�o���ϐ����Đݒ�
	g_aPlayer[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	g_aPlayer[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
}
#endif
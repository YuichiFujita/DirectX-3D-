//======================================================================================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "enemy.h"
#include "shadow.h"

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apModelEnemy[] =		// ���f���̑��΃p�X
{
	"data\\MODEL\\flower000.x",		// �Ԃ̃��f�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (ENEMYTYPE)
//**********************************************************************************************************************
typedef enum
{
	ENEMYTYPE_FLOWER = 0,			// ��
	ENEMYTYPE_MAX,					// ���̗񋓌^�̑���
} ENEMYTYPE;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
ModelEnemy g_aModelEnemy[ENEMYTYPE_MAX];		// �G�̃��f�����
Enemy      g_aEnemy[MAX_ENEMY];					// �G�̏��

//======================================================================================================================
//	�G�̏���������
//======================================================================================================================
void InitEnemy(void)
{
	// �ϐ���錾
	int         nNumVtx;			// ���f���̒��_��
	DWORD       dwSizeFVF;			// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE        *pVtxBuff;			// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;				// ���f���̒��_���W

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL      *pMat;					// �}�e���A���ւ̃|�C���^

	// �G�̃��f�����̏�����
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		g_aModelEnemy[nCntEnemy].apTexture[TEXTURE_ENEMY_MAX] = {};			// �e�N�X�`���ւ̃|�C���^
		g_aModelEnemy[nCntEnemy].pMesh    = NULL;							// ���b�V�� (���_���) �ւ̃|�C���^
		g_aModelEnemy[nCntEnemy].pBuffMat = NULL;							// �}�e���A���ւ̃|�C���^
		g_aModelEnemy[nCntEnemy].dwNumMat = 0;								// �}�e���A���̐�
		g_aModelEnemy[nCntEnemy].vtxMin   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̒��_���W
		g_aModelEnemy[nCntEnemy].vtxMax   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ő�̒��_���W
	}

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		g_aEnemy[nCntEnemy].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aEnemy[nCntEnemy].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aEnemy[nCntEnemy].state         = ENEMYSTATE_NONE;				// ���
		g_aEnemy[nCntEnemy].nType         = 0;								// ���
		g_aEnemy[nCntEnemy].nCounterState = 0;								// ��ԊǗ��J�E���^�[
		g_aEnemy[nCntEnemy].nShadowID     = NONE_SHADOW;					// �e�̃C���f�b�N�X
		g_aEnemy[nCntEnemy].bUse          = false;							// �g�p��
	}

	// x�t�@�C���̓ǂݍ���
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		D3DXLoadMeshFromX
		( // ����
			apModelEnemy[nCntEnemy],			// ���f���̑��΃p�X
			D3DXMESH_SYSTEMMEM,
			pDevice,							// �f�o�C�X�ւ̃|�C���^
			NULL,
			&g_aModelEnemy[nCntEnemy].pBuffMat,	// �}�e���A���ւ̃|�C���^
			NULL,
			&g_aModelEnemy[nCntEnemy].dwNumMat,	// �}�e���A���̐�
			&g_aModelEnemy[nCntEnemy].pMesh		// ���b�V�� (���_���) �ւ̃|�C���^
		);
	}

	// �����蔻��̍쐬
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		// ���f���̒��_�����擾
		nNumVtx = g_aModelEnemy[nCntEnemy].pMesh->GetNumVertices();

		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelEnemy[nCntEnemy].pMesh->GetFVF());

		// ���f���̒��_�o�b�t�@�����b�N
		g_aModelEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���f���̒��_�����J��Ԃ�

			// ���f���̒��_���W����
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// �ŏ����_���W�̊m�F
			if (vtx.x < g_aModelEnemy[nCntEnemy].vtxMin.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����������ꍇ

				// ����̒��_��� (x) ����
				g_aModelEnemy[nCntEnemy].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_aModelEnemy[nCntEnemy].vtxMin.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����������ꍇ

				// ����̒��_��� (y) ����
				g_aModelEnemy[nCntEnemy].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_aModelEnemy[nCntEnemy].vtxMin.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����������ꍇ

				// ����̒��_��� (z) ����
				g_aModelEnemy[nCntEnemy].vtxMin.z = vtx.z;
			}

			// �ő咸�_���W�̊m�F
			if (vtx.x > g_aModelEnemy[nCntEnemy].vtxMax.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����傫���ꍇ

				// ����̒��_��� (x) ����
				g_aModelEnemy[nCntEnemy].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_aModelEnemy[nCntEnemy].vtxMax.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����傫���ꍇ

				// ����̒��_��� (y) ����
				g_aModelEnemy[nCntEnemy].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_aModelEnemy[nCntEnemy].vtxMax.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����傫���ꍇ

				// ����̒��_��� (z) ����
				g_aModelEnemy[nCntEnemy].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		// ���f���̒��_�o�b�t�@���A�����b�N
		g_aModelEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModelEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelEnemy[nCntEnemy].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // �e�N�X�`���t�@�C�������݂���ꍇ

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelEnemy[nCntEnemy].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	�G�̏I������
//======================================================================================================================
void UninitEnemy(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		for (int nCntMat = 0; nCntMat < TEXTURE_ENEMY_MAX; nCntMat++)
		{ // �g�p����e�N�X�`�������J��Ԃ�

			if (g_aModelEnemy[nCntEnemy].apTexture[nCntMat] != NULL)
			{ // �ϐ� (g_aModelEnemy[nCntEnemy].apTexture) ��NULL�ł͂Ȃ��ꍇ

				g_aModelEnemy[nCntEnemy].apTexture[nCntMat]->Release();
				g_aModelEnemy[nCntEnemy].apTexture[nCntMat] = NULL;
			}
		}
	}

	// ���b�V���̔j��
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		if (g_aModelEnemy[nCntEnemy].pMesh != NULL)
		{ // �ϐ� (g_aModelEnemy[nCntEnemy].pMesh) ��NULL�ł͂Ȃ��ꍇ

			g_aModelEnemy[nCntEnemy].pMesh->Release();
			g_aModelEnemy[nCntEnemy].pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // �G�̎�ޕ��J��Ԃ�

		if (g_aModelEnemy[nCntEnemy].pBuffMat != NULL)
		{ // �ϐ� (g_aModelEnemy[nCntEnemy].pBuffMat) ��NULL�ł͂Ȃ��ꍇ

			g_aModelEnemy[nCntEnemy].pBuffMat->Release();
			g_aModelEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	�G�̍X�V����
//======================================================================================================================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // �G���g�p����Ă���ꍇ

			if (g_aEnemy[nCntEnemy].nCounterState > 0)
			{ // �J�E���^�[�� 0���傫���ꍇ

				// �J�E���^�[�����Z
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{ // �J�E���^�[�� 0�ȉ��̏ꍇ

					// �ʏ��Ԃɂ���
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
			}
		}
	}
}

//======================================================================================================================
//	�G�̕`�揈��
//======================================================================================================================
void DrawEnemy(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL      redMat;					// �}�e���A���f�[�^ (��)

	// �\���̗̂v�f���N���A
	ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

	// �g�U���E������Ԃɂ���
	redMat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	redMat.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // �G���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModelEnemy[g_aEnemy[nCntEnemy].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelEnemy[g_aEnemy[nCntEnemy].nType].dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				switch (g_aEnemy[nCntEnemy].state)
				{ // ��Ԃ��Ƃ̏���
				case ENEMYSTATE_NORMAL:

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �ʏ�

					// �����𔲂���
					break;

				case ENEMYSTATE_DAMAGE:

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&redMat.MatD3D);			// ��

					// �����𔲂���
					break;
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aModelEnemy[g_aEnemy[nCntEnemy].nType].apTexture[nCntMat]);

				// ���f���̕`��
				g_aModelEnemy[g_aEnemy[nCntEnemy].nType].pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�G�̐ݒ菈��
//======================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		if (g_aEnemy[nCntEnemy].bUse == false)
		{ // �G���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aEnemy[nCntEnemy].pos   = pos;		// �ʒu
			g_aEnemy[nCntEnemy].rot   = rot;		// ����
			g_aEnemy[nCntEnemy].nType = nType;		// ���

			// �G�̏���������
			g_aEnemy[nCntEnemy].state         = ENEMYSTATE_NORMAL;	// ���
			g_aEnemy[nCntEnemy].nCounterState = 0;					// ��ԊǗ��J�E���^�[

			// �g�p���Ă����Ԃɂ���
			g_aEnemy[nCntEnemy].bUse = true;

			// �e�̃C���f�b�N�X��ݒ�
			g_aEnemy[nCntEnemy].nShadowID = SetShadow
			( // ����
				fabsf(g_aModelEnemy[g_aEnemy[nCntEnemy].nType].vtxMax.x - g_aModelEnemy[g_aEnemy[nCntEnemy].nType].vtxMin.x),	// ���a
				&g_aEnemy[nCntEnemy].bUse																						// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aEnemy[nCntEnemy].nShadowID, g_aEnemy[nCntEnemy].pos);

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�G�̃_���[�W����
//======================================================================================================================
void HitEnemy(Enemy *pEnemy)
{
	// �_���[�W��Ԃɂ���
	pEnemy->state = ENEMYSTATE_DAMAGE;

	// �J�E���^�[��ݒ�
	pEnemy->nCounterState = DAMAGE_TIME_ENEMY;
}

//======================================================================================================================
//	�G�̎擾����
//======================================================================================================================
Enemy *GetEnemy(void)
{
	// �G�̏��̐擪�A�h���X��Ԃ�
	return &g_aEnemy[0];
}

//======================================================================================================================
//	�G�̃��f�����̎擾����
//======================================================================================================================
ModelEnemy *GetModelEnemy(void)
{
	// �G�̃��f�����̐擪�A�h���X��Ԃ�
	return &g_aModelEnemy[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�G�̑����擾����
//**********************************************************************************************************************
int GetNumEnemy(void)
{
	// �ϐ���錾
	int nNumEnemy = 0;	// �G�̑����̊m�F�p

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // �G���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumEnemy++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumEnemy;	// �G�̑���
}
#endif
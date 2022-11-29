//======================================================================================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "object.h"
#include "shadow.h"

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apModelObject[] =		// ���f���̑��΃p�X
{
	"data\\MODEL\\pillar000.x",		// ���̃��f�����΃p�X
	"data\\MODEL\\save000.x",		// ���̃��f�����΃p�X
	"data\\MODEL\\box000.x",		// ���̃��f�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (OBJECTTYPE)
//**********************************************************************************************************************
typedef enum
{
	OBJECTTYPE_PILLAR = 0,			// ��
	OBJECTTYPE_STAR,				// ��
	OBJECTTYPE_BOX,					// ��
	OBJECTTYPE_MAX,					// ���̗񋓌^�̑���
} OBJECTTYPE;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
ModelObject g_aModelObject[OBJECTTYPE_MAX];		// �I�u�W�F�N�g�̃��f�����
Object      g_aObject[MAX_OBJECT];				// �I�u�W�F�N�g�̏��

//======================================================================================================================
//	�I�u�W�F�N�g�̏���������
//======================================================================================================================
void InitObject(void)
{
	// �ϐ���錾
	int         nNumVtx;			// ���f���̒��_��
	DWORD       dwSizeFVF;			// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y
	BYTE        *pVtxBuff;			// ���f���̒��_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;				// ���f���̒��_���W

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL      *pMat;					// �}�e���A���ւ̃|�C���^

	// �I�u�W�F�N�g�̃��f�����̏�����
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		g_aModelObject[nCntObject].apTexture[TEXTURE_OBJECT_MAX] = {};			// �e�N�X�`���ւ̃|�C���^
		g_aModelObject[nCntObject].pMesh    = NULL;								// ���b�V�� (���_���) �ւ̃|�C���^
		g_aModelObject[nCntObject].pBuffMat = NULL;								// �}�e���A���ւ̃|�C���^
		g_aModelObject[nCntObject].dwNumMat = 0;								// �}�e���A���̐�
		g_aModelObject[nCntObject].vtxMin   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ŏ��̒��_���W
		g_aModelObject[nCntObject].vtxMax   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ő�̒��_���W
	}

	// �I�u�W�F�N�g�̏��̏�����
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		g_aObject[nCntObject].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aObject[nCntObject].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_aObject[nCntObject].nType     = 0;									// ���
		g_aObject[nCntObject].nShadowID = NONE_SHADOW;							// �e�̃C���f�b�N�X
		g_aObject[nCntObject].bUse      = false;								// �g�p��
	}

	// x�t�@�C���̓ǂݍ���
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		D3DXLoadMeshFromX
		( // ����
			apModelObject[nCntObject],				// ���f���̑��΃p�X
			D3DXMESH_SYSTEMMEM,
			pDevice,								// �f�o�C�X�ւ̃|�C���^
			NULL,
			&g_aModelObject[nCntObject].pBuffMat,	// �}�e���A���ւ̃|�C���^
			NULL,
			&g_aModelObject[nCntObject].dwNumMat,	// �}�e���A���̐�
			&g_aModelObject[nCntObject].pMesh		// ���b�V�� (���_���) �ւ̃|�C���^
		);
	}

	// �����蔻��̍쐬
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		// ���f���̒��_�����擾
		nNumVtx = g_aModelObject[nCntObject].pMesh->GetNumVertices();

		// ���f���̒��_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelObject[nCntObject].pMesh->GetFVF());

		// ���f���̒��_�o�b�t�@�����b�N
		g_aModelObject[nCntObject].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // ���f���̒��_�����J��Ԃ�

			// ���f���̒��_���W����
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// �ŏ����_���W�̊m�F
			if (vtx.x < g_aModelObject[nCntObject].vtxMin.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����������ꍇ

				// ����̒��_��� (x) ����
				g_aModelObject[nCntObject].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_aModelObject[nCntObject].vtxMin.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����������ꍇ

				// ����̒��_��� (y) ����
				g_aModelObject[nCntObject].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_aModelObject[nCntObject].vtxMin.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����������ꍇ

				// ����̒��_��� (z) ����
				g_aModelObject[nCntObject].vtxMin.z = vtx.z;
			}

			// �ő咸�_���W�̊m�F
			if (vtx.x > g_aModelObject[nCntObject].vtxMax.x)
			{ // ����̒��_���W (x) ���A����̒��_���W (x) �����傫���ꍇ

				// ����̒��_��� (x) ����
				g_aModelObject[nCntObject].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_aModelObject[nCntObject].vtxMax.y)
			{ // ����̒��_���W (y) ���A����̒��_���W (y) �����傫���ꍇ

				// ����̒��_��� (y) ����
				g_aModelObject[nCntObject].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_aModelObject[nCntObject].vtxMax.z)
			{ // ����̒��_���W (z) ���A����̒��_���W (z) �����傫���ꍇ

				// ����̒��_��� (z) ����
				g_aModelObject[nCntObject].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		// ���f���̒��_�o�b�t�@���A�����b�N
		g_aModelObject[nCntObject].pMesh->UnlockVertexBuffer();
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModelObject[nCntObject].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelObject[nCntObject].dwNumMat; nCntMat++)
		{ // �}�e���A���̐����J��Ԃ�

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // �e�N�X�`���t�@�C�������݂���ꍇ

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelObject[nCntObject].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̏I������
//======================================================================================================================
void UninitObject(void)
{
	// �e�N�X�`���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		for (int nCntMat = 0; nCntMat < TEXTURE_OBJECT_MAX; nCntMat++)
		{ // �g�p����e�N�X�`�������J��Ԃ�

			if (g_aModelObject[nCntObject].apTexture[nCntMat] != NULL)
			{ // �ϐ� (g_aModelObject[nCntObject].apTexture) ��NULL�ł͂Ȃ��ꍇ

				g_aModelObject[nCntObject].apTexture[nCntMat]->Release();
				g_aModelObject[nCntObject].apTexture[nCntMat] = NULL;
			}
		}
	}

	// ���b�V���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		if (g_aModelObject[nCntObject].pMesh != NULL)
		{ // �ϐ� (g_aModelObject[nCntObject].pMesh) ��NULL�ł͂Ȃ��ꍇ

			g_aModelObject[nCntObject].pMesh->Release();
			g_aModelObject[nCntObject].pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // �I�u�W�F�N�g�̎�ޕ��J��Ԃ�

		if (g_aModelObject[nCntObject].pBuffMat != NULL)
		{ // �ϐ� (g_aModelObject[nCntObject].pBuffMat) ��NULL�ł͂Ȃ��ꍇ

			g_aModelObject[nCntObject].pBuffMat->Release();
			g_aModelObject[nCntObject].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̍X�V����
//======================================================================================================================
void UpdateObject(void)
{

}

//======================================================================================================================
//	�I�u�W�F�N�g�̕`�揈��
//======================================================================================================================
void DrawObject(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	D3DXMATERIAL     *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModelObject[g_aObject[nCntObject].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelObject[g_aObject[nCntObject].nType].dwNumMat; nCntMat++)
			{ // �}�e���A���̐����J��Ԃ�

				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aModelObject[g_aObject[nCntObject].nType].apTexture[nCntMat]);

				// ���f���̕`��
				g_aModelObject[g_aObject[nCntObject].nType].pMesh->DrawSubset(nCntMat);
			}

			// �ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == false)
		{ // �I�u�W�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aObject[nCntObject].pos   = pos;		// �ʒu
			g_aObject[nCntObject].rot   = rot;		// ����
			g_aObject[nCntObject].nType = nType;	// ���

			// �g�p���Ă����Ԃɂ���
			g_aObject[nCntObject].bUse = true;

			// �e�̃C���f�b�N�X��ݒ�
			g_aObject[nCntObject].nShadowID = SetShadow
			( // ����
				fabsf(g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x - g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x),	// ���a
				&g_aObject[nCntObject].bUse																							// �e�̐e�̎g�p��
			);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�Ƃ̓����蔻��
//======================================================================================================================
void CollisionObject(Player *pPlayer)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �O��̓����蔻��
			if (pPlayer->pos.x + PLAY_WIDTH > g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x
			&&  pPlayer->pos.x - PLAY_WIDTH < g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x)
			{ // �u���b�N�̍��E�͈͓̔��̏ꍇ

				if (pPlayer->pos.z    + PLAY_WIDTH >  g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z
				&&  pPlayer->oldPos.z + PLAY_WIDTH <= g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z)
				{ // �O����̓����蔻��

					// �ʒu��␳
					pPlayer->pos.z = g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z - PLAY_WIDTH;

					// �ړ��ʂ�������
					pPlayer->move.z = 0.0f;
				}
				else if (pPlayer->pos.z    - PLAY_WIDTH <  g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z
				     &&  pPlayer->oldPos.z - PLAY_WIDTH >= g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z)
				{ // ��납��̓����蔻��

					// �ʒu��␳
					pPlayer->pos.z = g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z + PLAY_WIDTH;

					// �ړ��ʂ�������
					pPlayer->move.z = 0.0f;
				}
			}

			// ���E�̓����蔻��
			if (pPlayer->pos.z + PLAY_WIDTH > g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z
			&&  pPlayer->pos.z - PLAY_WIDTH < g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z)
			{ // �u���b�N�̑O��͈͓̔��̏ꍇ

				if (pPlayer->pos.x    + PLAY_WIDTH >  g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x
				&&  pPlayer->oldPos.x + PLAY_WIDTH <= g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x)
				{ // ������̓����蔻��

					// �ʒu��␳
					pPlayer->pos.x = g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x - PLAY_WIDTH;

					// �ړ��ʂ�������
					pPlayer->move.x = 0.0f;
				}
				else if (pPlayer->pos.x    - PLAY_WIDTH <  g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x
				     &&  pPlayer->oldPos.x - PLAY_WIDTH >= g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x)
				{ // �E����̓����蔻��

					// �ʒu��␳
					pPlayer->pos.x = g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x + PLAY_WIDTH;

					// �ړ��ʂ�������
					pPlayer->move.x = 0.0f;
				}
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�̎擾����
//======================================================================================================================
Object *GetObjectData(void)
{
	// �I�u�W�F�N�g�̏��̐擪�A�h���X��Ԃ�
	return &g_aObject[0];
}

//======================================================================================================================
//	�I�u�W�F�N�g�̃��f�����̎擾����
//======================================================================================================================
ModelObject *GetModelObject(void)
{
	// �I�u�W�F�N�g�̃��f�����̐擪�A�h���X��Ԃ�
	return &g_aModelObject[0];
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�I�u�W�F�N�g�̑����擾����
//**********************************************************************************************************************
int GetNumObject(void)
{
	// �ϐ���錾
	int nNumObject = 0;	// �I�u�W�F�N�g�̑����̊m�F�p

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aObject[nCntObject].bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumObject++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumObject;	// �I�u�W�F�N�g�̑���
}
#endif
//======================================================================================================================
//
//	���b�V���V�����_�[���� [meshcylinder.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "meshcylinder.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_MESHCYLINDER	(1)			// �g�p����|���S���� (���b�V���V�����_�[�̍ő吔)
#define PARTITION_WIDTH		(8)			// ���̕�����
#define PARTITION_HEIGHT	(1)			// �c�̕�����
//#define MESHCYLINDER_WIDTH	(800)	// ���b�V���V�����_�[�̉���
//#define MESHCYLINDER_HEIGHT	(200)	// ���b�V���V�����_�[�̏c��

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureMeshCylinder[] =	// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\mountain000.png",	// ���b�V���V�����_�[�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_MESHCYLINDER)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHCYLINDER_NORMAL = 0,	// ���b�V���V�����_�[ (�ʏ�)
	TEXTURE_MESHCYLINDER_MAX,			// ���̗񋓌^�̑���
} TEXTURE_MESHCYLINDER;

//**********************************************************************************************************************
//	�\���̒�` (MeshCylinder)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXMATRIX  mtxWorld;				// ���[���h�}�g���b�N�X
	float       fRadius;				// ���a
	float       fHeight;				// �c��
	int         nPartWidth;				// ���̕�����
	int         nPartHeight;			// �c�̕�����
	int         nNumVtx;				// �K�v���_��
	int         nNumIdx;				// �K�v�C���f�b�N�X��
	bool        bUse;					// �g�p��
} MeshCylinder;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight);	// ���b�V���V�����_�[�̐ݒ菈��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshCylinder[TEXTURE_MESHCYLINDER_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshCylinder = NULL;							// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

MeshCylinder g_aMeshCylinder[MAX_MESHCYLINDER];	// ���b�V���V�����_�[�̏��
int          g_nNeedVtxCylinder;				// �K�v���_��
int          g_nNeedIdxCylinder;				// �K�v�C���f�b�N�X��

//======================================================================================================================
//	���b�V���V�����_�[�̏���������
//======================================================================================================================
void InitMeshCylinder(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;							// ���_�ʒu�̌v�Z�p
	int         nNumVtx = 0;					// ���_���̌v���p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^
	WORD      *pIdx;							// �C���f�b�N�X���ւ̃|�C���^
	
	// �O���[�o���ϐ��̏�����
	g_nNeedVtxCylinder = 0;						// �K�v���_�̑���
	g_nNeedIdxCylinder = 0;						// �K�v�C���f�b�N�X�̑���

	// ���b�V���V�����_�[�̏��̏�����
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		g_aMeshCylinder[nCntMeshCylinder].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aMeshCylinder[nCntMeshCylinder].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
		g_aMeshCylinder[nCntMeshCylinder].fRadius     = 0.0f;							// ���a
		g_aMeshCylinder[nCntMeshCylinder].fHeight     = 0.0f;							// �c��
		g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = 0;								// ���̕�����
		g_aMeshCylinder[nCntMeshCylinder].nPartHeight = 0;								// �c�̕�����
		g_aMeshCylinder[nCntMeshCylinder].nNumVtx     = 0;								// �K�v���_��
		g_aMeshCylinder[nCntMeshCylinder].nNumIdx     = 0;								// �K�v�C���f�b�N�X��
		g_aMeshCylinder[nCntMeshCylinder].bUse        = false;							// �g�p��
	}

	// �e�N�X�`���̓ǂݍ���
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureMeshCylinder[nCntMeshCylinder], &g_apTextureMeshCylinder[nCntMeshCylinder]);
	}

	// ���b�V���V�����_�[�̐ݒ�
	SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000.0f, 800.0f, 16, 1);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * g_nNeedVtxCylinder,	// �K�v���_��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pVtxBuffMeshCylinder,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL
	);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	( // ����
		sizeof(WORD) * g_nNeedIdxCylinder,		// �K�v�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,						// �g�p���@
		D3DFMT_INDEX16,							// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
		D3DPOOL_MANAGED,						// �������̎w��
		&g_pIdxBuffMeshCylinder,				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			for (int nCntHeight = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight + 1; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					// ���_���W�̕�����ݒ�
					vecPos = D3DXVECTOR3
					( // ����
						sinf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius,						// x
						-(nCntHeight * (g_aMeshCylinder[nCntMeshCylinder].fHeight / (float)g_aMeshCylinder[nCntMeshCylinder].nPartHeight)) + g_aMeshCylinder[nCntMeshCylinder].fHeight,	// y
						cosf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius							// z
					);

					// ���_���W�̐ݒ�
					pVtx[0].pos = g_aMeshCylinder[nCntMeshCylinder].pos + vecPos;

					//// �@���x�N�g���̕�����ݒ�
					//pos = D3DXVECTOR3
					//(
					//	sinf(D3DXToRadian(fabsf(180 - nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth)))),
					//	0.0f,
					//	cosf(D3DXToRadian(fabsf(180 - nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))))
					//);
					//D3DXVec3Normalize(&pos, &pos);	// �x�N�g���𐳋K��
					//// �@���x�N�g���̐ݒ�
					//pVtx[0].nor = pos;

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//// �e�N�X�`�����W�̐ݒ�
					//pVtx[0].tex = D3DXVECTOR2
					//( // ����
					//	nCntWidth * (1.0f / (float)g_aMeshField[nCntMeshField].nPartWidth) - (1.0f * 0.5f),		// u
					//	nCntHeight * (1.0f / (float)g_aMeshField[nCntMeshField].nPartHeight) - (1.0f * 0.5f)	// v
					//);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(1.0f * (nCntWidth % 2), 1.0f * nCntHeight);

					// ���_�f�[�^�̃|�C���^�� 1���i�߂�
					pVtx += 1;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�C���f�b�N�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight; nCntHeight++)
			{ // �c�̕����� +1��J��Ԃ�

				for (nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // ���̕����� +1��J��Ԃ�

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth);

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshCylinder[nCntMeshCylinder].nPartHeight - 1)
				{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 2));

					// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
					pIdx += 2;
				}
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();
}

//======================================================================================================================
//	���b�V���V�����_�[�̏I������
//======================================================================================================================
void UninitMeshCylinder(void)
{
	// �e�N�X�`���̔j��
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{ // �ϐ� (g_apTextureMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_apTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{ // �ϐ� (g_pVtxBuffMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{ // �ϐ� (g_pIdxBuffMeshCylinder) ��NULL�ł͂Ȃ��ꍇ

		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//======================================================================================================================
//	���b�V���V�����_�[�̍X�V����
//======================================================================================================================
void UpdateMeshCylinder(void)
{

}

//======================================================================================================================
//	���b�V���V�����_�[�̕`�揈��
//======================================================================================================================
void DrawMeshCylinder(void)
{
	// �ϐ���錾
	int        nNumIdx = 0;						// �C���f�b�N�X���̌v���p
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // ���b�V���V�����_�[�̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // ���b�V���V�����_�[���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshCylinder[TEXTURE_MESHCYLINDER_NORMAL]);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,							// �v���~�e�B�u�̎��
				0,
				0,
				g_aMeshCylinder[nCntMeshCylinder].nNumVtx,		// �g�p���钸�_��
				nNumIdx,										// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				g_aMeshCylinder[nCntMeshCylinder].nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
			);

			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumIdx += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	���b�V���V�����_�[�̐ݒ菈��
//======================================================================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // �ǂ̍ő�\�������J��Ԃ�

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == false)
		{ // �ǂ��g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aMeshCylinder[nCntMeshCylinder].pos         = pos;			// �ʒu
			g_aMeshCylinder[nCntMeshCylinder].rot         = rot;			// ����
			g_aMeshCylinder[nCntMeshCylinder].fRadius     = fRadius;		// ���a
			g_aMeshCylinder[nCntMeshCylinder].fHeight     = fHeight;		// �c��
			g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = nPartWidth;		// ���̕�����
			g_aMeshCylinder[nCntMeshCylinder].nPartHeight = nPartHeight;	// �c�̕�����

			// �g�p���Ă����Ԃɂ���
			g_aMeshCylinder[nCntMeshCylinder].bUse = true;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̕K�v����ݒ�
			g_aMeshCylinder[nCntMeshCylinder].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshCylinder[nCntMeshCylinder].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̑��������Z
			g_nNeedVtxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
			g_nNeedIdxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;

			// �����𔲂���
			break;
		}
	}
}
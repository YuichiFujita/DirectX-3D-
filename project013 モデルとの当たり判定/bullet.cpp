//======================================================================================================================
//
//	�e���� [bullet.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "meshwall.h"
#include "object.h"
#include "particle.h"
#include "shadow.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_BULLET		(128)		// �g�p����|���S���� (�e�̍ő吔)

#define BULL_WIDTH		(18.0f)		// �e�̏c�� / 2
#define BULL_HEIGHT		(18.0f)		// �e�̉��� / 2

#define LIFE_BULLET		(80)		// �e�̎���
#define SHADOW_BULL		(30.0f)		// �e�̉e�̔��a

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureBullet[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\bullet000.png",	// �e�̃e�N�X�`���̑��΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_BULLET)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_BULLET_NORMAL = 0,		// �e (�ʏ�)
	TEXTURE_BULLET_MAX,				// ���̗񋓌^�̑���
} TEXTURE_BULLET;

//**********************************************************************************************************************
//	�\���̒�` (Bullet)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXMATRIX  mtxWorld;			// ���[���h�}�g���b�N�X
	BULLETTYPE  type;				// ���
	int         nLife;				// ����
	int         nShadowID;			// �e�̃C���f�b�N�X
	bool        bUse;				// �g�p��
}Bullet;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void CollisionEnemy(Bullet *pBullet);			// �G�ƒe�̓����蔻��
void CollisionObject(Bullet *pBullet);			// �I�u�W�F�N�g�ƒe�̓����蔻��
void CollisionMeshWall(Bullet *pBullet);		// ���b�V���E�H�[���ƒe�̓����蔻��

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureBullet[TEXTURE_BULLET_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Bullet g_aBullet[MAX_BULLET];		// �e�̏��

//======================================================================================================================
//	�e�̏���������
//======================================================================================================================
void InitBullet(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		D3DXCreateTextureFromFile(pDevice, apTextureBullet[nCntBullet], &g_apTextureBullet[nCntBullet]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	// �e�̏��̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		g_aBullet[nCntBullet].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aBullet[nCntBullet].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aBullet[nCntBullet].type      = BULLETTYPE_PLAYER;				// ���
		g_aBullet[nCntBullet].nLife     = 0;								// ����
		g_aBullet[nCntBullet].nShadowID = NONE_SHADOW;						// �e�̃C���f�b�N�X
		g_aBullet[nCntBullet].bUse      = false;							// �g�p��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULL_WIDTH, -BULL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( BULL_WIDTH, -BULL_HEIGHT, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//======================================================================================================================
//	�e�̏I������
//======================================================================================================================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureBullet[nCntBullet] != NULL)
		{ // �ϐ� (g_apTextureBullet) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureBullet[nCntBullet]->Release();
			g_apTextureBullet[nCntBullet] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{ // �ϐ� (g_pVtxBuffBullet) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//======================================================================================================================
//	�e�̍X�V����
//======================================================================================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aBullet[nCntBullet].bUse == true)
		{ // �e���g�p����Ă���ꍇ

			//------------------------------------------------------------------------------------------------------------------
			//	�ʒu�̍X�V
			//------------------------------------------------------------------------------------------------------------------
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

			//------------------------------------------------------------------------------------------------------------------
			//	�����蔻��
			//------------------------------------------------------------------------------------------------------------------
			switch (g_aBullet[nCntBullet].type)
			{ // ��ނ��Ƃ̏���
			case BULLETTYPE_PLAYER:		// �u���C���[�̒e

				// �G�ƒe�̓����蔻��
				CollisionEnemy(&g_aBullet[nCntBullet]);

				// �����𔲂���
				break;
			}

			// �I�u�W�F�N�g�ƒe�̓����蔻��
			CollisionObject(&g_aBullet[nCntBullet]);

			// ���b�V���E�H�[���ƒe�̓����蔻��
			CollisionMeshWall(&g_aBullet[nCntBullet]);

			//------------------------------------------------------------------------------------------------------------------
			//	�G�t�F�N�g�̍X�V
			//------------------------------------------------------------------------------------------------------------------
			// �G�t�F�N�g�̐ݒ�
			SetEffect
			( // ����
				g_aBullet[nCntBullet].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// �ړ���
				D3DXCOLOR(0.3f, 0.6f, 1.0f, 1.0f),	// �F
				30,									// ����
				30.0f,								// ���a
				1.0f								// ���Z�� (���a)
			);

			// �G�t�F�N�g�̐ݒ�
			SetEffect
			( // ����
				g_aBullet[nCntBullet].pos,			// �ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// �ړ���
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),	// �F
				30,									// ����
				30.0f,								// ���a
				2.0f								// ���Z�� (���a)
			);

			//------------------------------------------------------------------------------------------------------------------
			//	�e�̍X�V
			//------------------------------------------------------------------------------------------------------------------
			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			//------------------------------------------------------------------------------------------------------------------
			//	�e�̍폜
			//------------------------------------------------------------------------------------------------------------------
			// ���������炷
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{ // �������s�����ꍇ

				// �g�p���Ă��Ȃ���Ԃɂ���
				g_aBullet[nCntBullet].bUse = false;

				// �����̐ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos);

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					g_aBullet[nCntBullet].pos,	// �ʒu
					PARTICLETYPE_EXPLOSION,		// ���
					SPAWN_PARTICLE_EXPLOSION,	// �G�t�F�N�g��
					2							// ����
				);
			}
		}
	}
}

//======================================================================================================================
//	�e�̕`�揈��
//======================================================================================================================
void DrawBullet(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ���e�X�g�̎Q�ƒl�ݒ�

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aBullet[nCntBullet].bUse == true)
		{ // �e���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//======================================================================================================================
//	�e�̐ݒ菈��
//======================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aBullet[nCntBullet].bUse == false)
		{ // �e���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aBullet[nCntBullet].pos  = pos;	// �ʒu
			g_aBullet[nCntBullet].move = move;	// �ړ���
			g_aBullet[nCntBullet].type = type;	// ���

			// ������ݒ�
			g_aBullet[nCntBullet].nLife = LIFE_BULLET;

			// �g�p���Ă����Ԃɂ���
			g_aBullet[nCntBullet].bUse = true;

			// �e�̃C���f�b�N�X��ݒ�
			g_aBullet[nCntBullet].nShadowID = SetShadow(SHADOW_BULL, &g_aBullet[nCntBullet].bUse);

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�G�ƒe�̓����蔻��
//======================================================================================================================
void CollisionEnemy(Bullet *pBullet)
{
	// �|�C���^��錾
	Enemy      *pEnemy      = GetEnemy();		// �G�̏��
	ModelEnemy *pModelEnemy = GetModelEnemy();	// �G�̃��f�����

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{ // �G�̍ő�\�������J��Ԃ�

		if (pEnemy->bUse == true)
		{ // �G���g�p����Ă���ꍇ

			if (pBullet->pos.x + BULL_WIDTH  > pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMax.z)
			{ // �e���G�ɓ������Ă���ꍇ

				// �G�̃_���[�W����
				HitEnemy(pEnemy);

				//// �����̐ݒ�
				//SetExplosion(pBullet->pos);

				//// �p�[�e�B�N���̐ݒ�
				//SetParticle
				//( // ����
				//	pBullet->pos,				// �ʒu
				//	PARTICLETYPE_EXPLOSION,		// ���
				//	SPAWN_PARTICLE_EXPLOSION,	// �G�t�F�N�g��
				//	2							// ����
				//);

				// �g�p���Ă��Ȃ���Ԃɂ���
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	�I�u�W�F�N�g�ƒe�̓����蔻��
//======================================================================================================================
void CollisionObject(Bullet *pBullet)
{
	// �|�C���^��錾
	Object      *pObject      = GetObjectData();	// �I�u�W�F�N�g�̏��
	ModelObject *pModelObject = GetModelObject();	// �I�u�W�F�N�g�̃��f�����

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (pObject->bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			if (pBullet->pos.x + BULL_WIDTH  > pObject->pos.x + pModelObject[pObject->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pObject->pos.y + pModelObject[pObject->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pObject->pos.z + pModelObject[pObject->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pObject->pos.x + pModelObject[pObject->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pObject->pos.y + pModelObject[pObject->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pObject->pos.z + pModelObject[pObject->nType].vtxMax.z)
			{ // �e���I�u�W�F�N�g�ɓ������Ă���ꍇ

				// �����̐ݒ�
				SetExplosion(pBullet->pos);

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					pBullet->pos,				// �ʒu
					PARTICLETYPE_EXPLOSION,		// ���
					SPAWN_PARTICLE_EXPLOSION,	// �G�t�F�N�g��
					2							// ����
				);

				// �g�p���Ă��Ȃ���Ԃɂ���
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	���b�V���E�H�[���ƒe�̓����蔻��
//======================================================================================================================
void CollisionMeshWall(Bullet *pBullet)
{
	// �ϐ���錾
	D3DXVECTOR3 posLeft, posRight, vecLine, vecToPos;	// �����蔻��̌v�Z�p

	// �|�C���^��錾
	MeshWall *pMeshWall = GetMeshWall();				// ���b�V���E�H�[���̏��

	for (int nCntObject = 0; nCntObject < MAX_MESHWALL; nCntObject++, pMeshWall++)
	{ // ���b�V���E�H�[���̍ő�\�������J��Ԃ�

		if (pMeshWall->bUse == true)
		{ // ���b�V���E�H�[�����g�p����Ă���ꍇ
	
			// ���b�V���E�H�[���̍��[�̍��W�����߂�
			posLeft = D3DXVECTOR3
			( // ����
				pMeshWall->pos.x - sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z - cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// ���b�V���E�H�[���̉E�[�̍��W�����߂�
			posRight = D3DXVECTOR3
			( // ����
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// ���E���x�N�g�������߂�
			vecLine  = posRight - posLeft;

			// ���b�V���E�H�[���̍��[�ƒe�̈ʒu�̃x�N�g�������߂�
			vecToPos = pBullet->pos - posLeft;

			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0
			&&  pBullet->pos.y - BULL_HEIGHT < pMeshWall->pos.y + pMeshWall->fHeight
			&&  pBullet->pos.y + BULL_HEIGHT > pMeshWall->pos.y)
			{ // �e�����b�V���E�H�[���ɓ������Ă���ꍇ

				// �����̐ݒ�
				SetExplosion(pBullet->pos);

				// �p�[�e�B�N���̐ݒ�
				SetParticle
				( // ����
					pBullet->pos,				// �ʒu
					PARTICLETYPE_EXPLOSION,		// ���
					SPAWN_PARTICLE_EXPLOSION,	// �G�t�F�N�g��
					2							// ����
				);

				// �g�p���Ă��Ȃ���Ԃɂ���
				pBullet->bUse = false;
			}
		}
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�e�̑����擾����
//**********************************************************************************************************************
int GetNumBullet(void)
{
	// �ϐ���錾
	int nNumBullet = 0;	// �e�̑����̊m�F�p

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // �e�̍ő�\�������J��Ԃ�

		if (g_aBullet[nCntBullet].bUse == true)
		{ // �e���g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumBullet++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumBullet;	// �e�̑���
}
#endif
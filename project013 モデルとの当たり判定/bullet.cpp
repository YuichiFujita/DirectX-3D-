//======================================================================================================================
//
//	e [bullet.cpp]
//	AuthorF‘cEκ
//
//======================================================================================================================
//**********************************************************************************************************************
//	CN[ht@C
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
//	}Nθ`
//**********************************************************************************************************************
#define MAX_BULLET		(128)		// gp·ι|S (eΜΕε)

#define BULL_WIDTH		(18.0f)		// eΜc / 2
#define BULL_HEIGHT		(18.0f)		// eΜ‘ / 2

#define LIFE_BULLET		(80)		// eΜυ½
#define SHADOW_BULL		(30.0f)		// eΜeΜΌa

//**********************************************************************************************************************
//	RXgθ`
//**********************************************************************************************************************
const char *apTextureBullet[] =		// eNX`ΜΞpX
{
	"data\\TEXTURE\\bullet000.png",	// eΜeNX`ΜΞpX
};

//**********************************************************************************************************************
//	ρ^θ` (TEXTURE_BULLET)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_BULLET_NORMAL = 0,		// e (Κν)
	TEXTURE_BULLET_MAX,				// ±Μρ^Μ
} TEXTURE_BULLET;

//**********************************************************************************************************************
//	\’Μθ` (Bullet)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// Κu
	D3DXVECTOR3 move;				// Ϊ?Κ
	D3DXMATRIX  mtxWorld;			// [h}gbNX
	BULLETTYPE  type;				// νή
	int         nLife;				// υ½
	int         nShadowID;			// eΜCfbNX
	bool        bUse;				// gpσ΅
}Bullet;

//**********************************************************************************************************************
//	vg^CvιΎ
//**********************************************************************************************************************
void CollisionEnemy(Bullet *pBullet);			// GΖeΜ½θ»θ
void CollisionObject(Bullet *pBullet);			// IuWFNgΖeΜ½θ»θ
void CollisionMeshWall(Bullet *pBullet);		// bVEH[ΖeΜ½θ»θ

//**********************************************************************************************************************
//	O[oΟ
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureBullet[TEXTURE_BULLET_MAX] = {};	// eNX`ΦΜ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					// Έ_obt@ΦΜ|C^

Bullet g_aBullet[MAX_BULLET];		// eΜξρ

//======================================================================================================================
//	eΜϊ»
//======================================================================================================================
void InitBullet(void)
{
	// |C^πιΎ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// foCXΦΜ|C^
	VERTEX_3D *pVtx;							// Έ_ξρΦΜ|C^

	// eNX`ΜΗέέ
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // gp·ιeNX`ͺJθΤ·

		D3DXCreateTextureFromFile(pDevice, apTextureBullet[nCntBullet], &g_apTextureBullet[nCntBullet]);
	}

	// Έ_obt@ΜΆ¬
	pDevice->CreateVertexBuffer
	( // ψ
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,		// KvΈ_
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// Έ_tH[}bg
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	// eΜξρΜϊ»
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		g_aBullet[nCntBullet].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Κu
		g_aBullet[nCntBullet].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Ϊ?Κ
		g_aBullet[nCntBullet].type      = BULLETTYPE_PLAYER;				// νή
		g_aBullet[nCntBullet].nLife     = 0;								// υ½
		g_aBullet[nCntBullet].nShadowID = NONE_SHADOW;						// eΜCfbNX
		g_aBullet[nCntBullet].bUse      = false;							// gpσ΅
	}

	//------------------------------------------------------------------------------------------------------------------
	//	Έ_ξρΜϊ»
	//------------------------------------------------------------------------------------------------------------------
	// Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		// Έ_ΐWΜέθ
		pVtx[0].pos = D3DXVECTOR3(-BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULL_WIDTH, -BULL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( BULL_WIDTH, -BULL_HEIGHT, 0.0f);

		// @όxNgΜέθ
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// Έ_J[Μέθ
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// eNX`ΐWΜέθ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// Έ_f[^Μ|C^π 4Βͺiίι
		pVtx += 4;
	}

	// Έ_obt@πAbN·ι
	g_pVtxBuffBullet->Unlock();
}

//======================================================================================================================
//	eΜIΉ
//======================================================================================================================
void UninitBullet(void)
{
	// eNX`Μjό
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // gp·ιeNX`ͺJθΤ·

		if (g_apTextureBullet[nCntBullet] != NULL)
		{ // Ο (g_apTextureBullet) ͺNULLΕΝΘ’κ

			g_apTextureBullet[nCntBullet]->Release();
			g_apTextureBullet[nCntBullet] = NULL;
		}
	}

	// Έ_obt@Μjό
	if (g_pVtxBuffBullet != NULL)
	{ // Ο (g_pVtxBuffBullet) ͺNULLΕΝΘ’κ

		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//======================================================================================================================
//	eΜXV
//======================================================================================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		if (g_aBullet[nCntBullet].bUse == true)
		{ // eͺgp³κΔ’ικ

			//------------------------------------------------------------------------------------------------------------------
			//	ΚuΜXV
			//------------------------------------------------------------------------------------------------------------------
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

			//------------------------------------------------------------------------------------------------------------------
			//	½θ»θ
			//------------------------------------------------------------------------------------------------------------------
			switch (g_aBullet[nCntBullet].type)
			{ // νή²ΖΜ
			case BULLETTYPE_PLAYER:		// uC[Μe

				// GΖeΜ½θ»θ
				CollisionEnemy(&g_aBullet[nCntBullet]);

				// π²―ι
				break;
			}

			// IuWFNgΖeΜ½θ»θ
			CollisionObject(&g_aBullet[nCntBullet]);

			// bVEH[ΖeΜ½θ»θ
			CollisionMeshWall(&g_aBullet[nCntBullet]);

			//------------------------------------------------------------------------------------------------------------------
			//	GtFNgΜXV
			//------------------------------------------------------------------------------------------------------------------
			// GtFNgΜέθ
			SetEffect
			( // ψ
				g_aBullet[nCntBullet].pos,			// Κu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// Ϊ?Κ
				D3DXCOLOR(0.3f, 0.6f, 1.0f, 1.0f),	// F
				30,									// υ½
				30.0f,								// Όa
				1.0f								// ΈZΚ (Όa)
			);

			// GtFNgΜέθ
			SetEffect
			( // ψ
				g_aBullet[nCntBullet].pos,			// Κu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// Ϊ?Κ
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),	// F
				30,									// υ½
				30.0f,								// Όa
				2.0f								// ΈZΚ (Όa)
			);

			//------------------------------------------------------------------------------------------------------------------
			//	eΜXV
			//------------------------------------------------------------------------------------------------------------------
			// eΜΚuέθ
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			//------------------------------------------------------------------------------------------------------------------
			//	eΜν
			//------------------------------------------------------------------------------------------------------------------
			// υ½πΈη·
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{ // υ½ͺs«½κ

				// gp΅Δ’Θ’σΤΙ·ι
				g_aBullet[nCntBullet].bUse = false;

				// ­Μέθ
				SetExplosion(g_aBullet[nCntBullet].pos);

				// p[eBNΜέθ
				SetParticle
				( // ψ
					g_aBullet[nCntBullet].pos,	// Κu
					PARTICLETYPE_EXPLOSION,		// νή
					SPAWN_PARTICLE_EXPLOSION,	// GtFNg
					2							// υ½
				);
			}
		}
	}
}

//======================================================================================================================
//	eΜ`ζ
//======================================================================================================================
void DrawBullet(void)
{
	// ΟπιΎ
	D3DXMATRIX mtxTrans;	// vZp}gbNX
	D3DXMATRIX mtxView;		// r[}gbNXζΎp

	// |C^πιΎ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// foCXΦΜ|C^

	// CeBOπ³ψΙ·ι
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ΏeXgπLψΙ·ι
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ΏeXgΜLψ / ³ψΜέθ
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ΏeXgΜέθ
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ΏeXgΜQΖlέθ

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		if (g_aBullet[nCntBullet].bUse == true)
		{ // eͺgp³κΔ’ικ

			// [h}gbNXΜϊ»
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			// r[}gbNXπζΎ
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// |SπJΙΞ΅Δ³ΚΙό―ι
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);	// tsρπίι
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			// Κuπ½f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			// [h}gbNXΜέθ
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// Έ_obt@πf[^Xg[Ιέθ
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// Έ_tH[}bgΜέθ
			pDevice->SetFVF(FVF_VERTEX_3D);

			// eNX`Μέθ
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].type]);

			// |SΜ`ζ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}

	// CeBOπLψΙ·ι
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ΏeXgπ³ψΙ·ι
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ΏeXgΜLψ / ³ψΜέθ
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ΏeXgΜέθ
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ΏeXgΜQΖlέθ
}

//======================================================================================================================
//	eΜέθ
//======================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		if (g_aBullet[nCntBullet].bUse == false)
		{ // eͺgp³κΔ’Θ’κ

			// ψπγό
			g_aBullet[nCntBullet].pos  = pos;	// Κu
			g_aBullet[nCntBullet].move = move;	// Ϊ?Κ
			g_aBullet[nCntBullet].type = type;	// νή

			// υ½πέθ
			g_aBullet[nCntBullet].nLife = LIFE_BULLET;

			// gp΅Δ’ισΤΙ·ι
			g_aBullet[nCntBullet].bUse = true;

			// eΜCfbNXπέθ
			g_aBullet[nCntBullet].nShadowID = SetShadow(SHADOW_BULL, &g_aBullet[nCntBullet].bUse);

			// eΜΚuέθ
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			// π²―ι
			break;
		}
	}
}

//======================================================================================================================
//	GΖeΜ½θ»θ
//======================================================================================================================
void CollisionEnemy(Bullet *pBullet)
{
	// |C^πιΎ
	Enemy      *pEnemy      = GetEnemy();		// GΜξρ
	ModelEnemy *pModelEnemy = GetModelEnemy();	// GΜfξρ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{ // GΜΕε\¦ͺJθΤ·

		if (pEnemy->bUse == true)
		{ // Gͺgp³κΔ’ικ

			if (pBullet->pos.x + BULL_WIDTH  > pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMax.z)
			{ // eͺGΙ½ΑΔ’ικ

				// GΜ_[W»θ
				HitEnemy(pEnemy);

				//// ­Μέθ
				//SetExplosion(pBullet->pos);

				//// p[eBNΜέθ
				//SetParticle
				//( // ψ
				//	pBullet->pos,				// Κu
				//	PARTICLETYPE_EXPLOSION,		// νή
				//	SPAWN_PARTICLE_EXPLOSION,	// GtFNg
				//	2							// υ½
				//);

				// gp΅Δ’Θ’σΤΙ·ι
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	IuWFNgΖeΜ½θ»θ
//======================================================================================================================
void CollisionObject(Bullet *pBullet)
{
	// |C^πιΎ
	Object      *pObject      = GetObjectData();	// IuWFNgΜξρ
	ModelObject *pModelObject = GetModelObject();	// IuWFNgΜfξρ

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // IuWFNgΜΕε\¦ͺJθΤ·

		if (pObject->bUse == true)
		{ // IuWFNgͺgp³κΔ’ικ

			if (pBullet->pos.x + BULL_WIDTH  > pObject->pos.x + pModelObject[pObject->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pObject->pos.y + pModelObject[pObject->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pObject->pos.z + pModelObject[pObject->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pObject->pos.x + pModelObject[pObject->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pObject->pos.y + pModelObject[pObject->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pObject->pos.z + pModelObject[pObject->nType].vtxMax.z)
			{ // eͺIuWFNgΙ½ΑΔ’ικ

				// ­Μέθ
				SetExplosion(pBullet->pos);

				// p[eBNΜέθ
				SetParticle
				( // ψ
					pBullet->pos,				// Κu
					PARTICLETYPE_EXPLOSION,		// νή
					SPAWN_PARTICLE_EXPLOSION,	// GtFNg
					2							// υ½
				);

				// gp΅Δ’Θ’σΤΙ·ι
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	bVEH[ΖeΜ½θ»θ
//======================================================================================================================
void CollisionMeshWall(Bullet *pBullet)
{
	// ΟπιΎ
	D3DXVECTOR3 posLeft, posRight, vecLine, vecToPos;	// ½θ»θΜvZp

	// |C^πιΎ
	MeshWall *pMeshWall = GetMeshWall();				// bVEH[Μξρ

	for (int nCntObject = 0; nCntObject < MAX_MESHWALL; nCntObject++, pMeshWall++)
	{ // bVEH[ΜΕε\¦ͺJθΤ·

		if (pMeshWall->bUse == true)
		{ // bVEH[ͺgp³κΔ’ικ
	
			// bVEH[ΜΆ[ΜΐWπίι
			posLeft = D3DXVECTOR3
			( // ψ
				pMeshWall->pos.x - sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z - cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// bVEH[ΜE[ΜΐWπίι
			posRight = D3DXVECTOR3
			( // ψ
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// «EόxNgπίι
			vecLine  = posRight - posLeft;

			// bVEH[ΜΆ[ΖeΜΚuΜxNgπίι
			vecToPos = pBullet->pos - posLeft;

			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0
			&&  pBullet->pos.y - BULL_HEIGHT < pMeshWall->pos.y + pMeshWall->fHeight
			&&  pBullet->pos.y + BULL_HEIGHT > pMeshWall->pos.y)
			{ // eͺbVEH[Ι½ΑΔ’ικ

				// ­Μέθ
				SetExplosion(pBullet->pos);

				// p[eBNΜέθ
				SetParticle
				( // ψ
					pBullet->pos,				// Κu
					PARTICLETYPE_EXPLOSION,		// νή
					SPAWN_PARTICLE_EXPLOSION,	// GtFNg
					2							// υ½
				);

				// gp΅Δ’Θ’σΤΙ·ι
				pBullet->bUse = false;
			}
		}
	}
}

#ifdef _DEBUG	// fobO
//======================================================================================================================
//	fobOκ
//======================================================================================================================
//**********************************************************************************************************************
//	eΜζΎ
//**********************************************************************************************************************
int GetNumBullet(void)
{
	// ΟπιΎ
	int nNumBullet = 0;	// eΜΜmFp

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // eΜΕε\¦ͺJθΤ·

		if (g_aBullet[nCntBullet].bUse == true)
		{ // eͺgp³κΔ’ικ

			// JE^[πΑZ
			nNumBullet++;
		}
	}

	// ΟΜlπΤ·
	return nNumBullet;	// eΜ
}
#endif
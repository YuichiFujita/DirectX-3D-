//======================================================================================================================
//
//	弾処理 [bullet.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
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
//	マクロ定義
//**********************************************************************************************************************
#define MAX_BULLET		(128)		// 使用するポリゴン数 (弾の最大数)

#define BULL_WIDTH		(18.0f)		// 弾の縦幅 / 2
#define BULL_HEIGHT		(18.0f)		// 弾の横幅 / 2

#define LIFE_BULLET		(80)		// 弾の寿命
#define SHADOW_BULL		(30.0f)		// 弾の影の半径

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureBullet[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\bullet000.png",	// 弾のテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_BULLET)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_BULLET_NORMAL = 0,		// 弾 (通常)
	TEXTURE_BULLET_MAX,				// この列挙型の総数
} TEXTURE_BULLET;

//**********************************************************************************************************************
//	構造体定義 (Bullet)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 move;				// 移動量
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	BULLETTYPE  type;				// 種類
	int         nLife;				// 寿命
	int         nShadowID;			// 影のインデックス
	bool        bUse;				// 使用状況
}Bullet;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void CollisionEnemy(Bullet *pBullet);			// 敵と弾の当たり判定
void CollisionObject(Bullet *pBullet);			// オブジェクトと弾の当たり判定
void CollisionMeshWall(Bullet *pBullet);		// メッシュウォールと弾の当たり判定

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureBullet[TEXTURE_BULLET_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					// 頂点バッファへのポインタ

Bullet g_aBullet[MAX_BULLET];		// 弾の情報

//======================================================================================================================
//	弾の初期化処理
//======================================================================================================================
void InitBullet(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureBullet[nCntBullet], &g_apTextureBullet[nCntBullet]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	// 弾の情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		g_aBullet[nCntBullet].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBullet[nCntBullet].move      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aBullet[nCntBullet].type      = BULLETTYPE_PLAYER;				// 種類
		g_aBullet[nCntBullet].nLife     = 0;								// 寿命
		g_aBullet[nCntBullet].nShadowID = NONE_SHADOW;						// 影のインデックス
		g_aBullet[nCntBullet].bUse      = false;							// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( BULL_WIDTH,  BULL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULL_WIDTH, -BULL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( BULL_WIDTH, -BULL_HEIGHT, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データのポインタを 4つ分進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//======================================================================================================================
//	弾の終了処理
//======================================================================================================================
void UninitBullet(void)
{
	// テクスチャの破棄
	for (int nCntBullet = 0; nCntBullet < TEXTURE_BULLET_MAX; nCntBullet++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureBullet[nCntBullet] != NULL)
		{ // 変数 (g_apTextureBullet) がNULLではない場合

			g_apTextureBullet[nCntBullet]->Release();
			g_apTextureBullet[nCntBullet] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{ // 変数 (g_pVtxBuffBullet) がNULLではない場合

		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//======================================================================================================================
//	弾の更新処理
//======================================================================================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		if (g_aBullet[nCntBullet].bUse == true)
		{ // 弾が使用されている場合

			//------------------------------------------------------------------------------------------------------------------
			//	位置の更新
			//------------------------------------------------------------------------------------------------------------------
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

			//------------------------------------------------------------------------------------------------------------------
			//	当たり判定
			//------------------------------------------------------------------------------------------------------------------
			switch (g_aBullet[nCntBullet].type)
			{ // 種類ごとの処理
			case BULLETTYPE_PLAYER:		// ブレイヤーの弾

				// 敵と弾の当たり判定
				CollisionEnemy(&g_aBullet[nCntBullet]);

				// 処理を抜ける
				break;
			}

			// オブジェクトと弾の当たり判定
			CollisionObject(&g_aBullet[nCntBullet]);

			// メッシュウォールと弾の当たり判定
			CollisionMeshWall(&g_aBullet[nCntBullet]);

			//------------------------------------------------------------------------------------------------------------------
			//	エフェクトの更新
			//------------------------------------------------------------------------------------------------------------------
			// エフェクトの設定
			SetEffect
			( // 引数
				g_aBullet[nCntBullet].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 移動量
				D3DXCOLOR(0.3f, 0.6f, 1.0f, 1.0f),	// 色
				30,									// 寿命
				30.0f,								// 半径
				1.0f								// 減算量 (半径)
			);

			// エフェクトの設定
			SetEffect
			( // 引数
				g_aBullet[nCntBullet].pos,			// 位置
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 移動量
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),	// 色
				30,									// 寿命
				30.0f,								// 半径
				2.0f								// 減算量 (半径)
			);

			//------------------------------------------------------------------------------------------------------------------
			//	影の更新
			//------------------------------------------------------------------------------------------------------------------
			// 影の位置設定
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			//------------------------------------------------------------------------------------------------------------------
			//	弾の削除
			//------------------------------------------------------------------------------------------------------------------
			// 寿命を減らす
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{ // 寿命が尽きた場合

				// 使用していない状態にする
				g_aBullet[nCntBullet].bUse = false;

				// 爆発の設定
				SetExplosion(g_aBullet[nCntBullet].pos);

				// パーティクルの設定
				SetParticle
				( // 引数
					g_aBullet[nCntBullet].pos,	// 位置
					PARTICLETYPE_EXPLOSION,		// 種類
					SPAWN_PARTICLE_EXPLOSION,	// エフェクト数
					2							// 寿命
				);
			}
		}
	}
}

//======================================================================================================================
//	弾の描画処理
//======================================================================================================================
void DrawBullet(void)
{
	// 変数を宣言
	D3DXMATRIX mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックス取得用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// αテストの参照値設定

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		if (g_aBullet[nCntBullet].bUse == true)
		{ // 弾が使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].type]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストの有効 / 無効の設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// αテストの参照値設定
}

//======================================================================================================================
//	弾の設定処理
//======================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		if (g_aBullet[nCntBullet].bUse == false)
		{ // 弾が使用されていない場合

			// 引数を代入
			g_aBullet[nCntBullet].pos  = pos;	// 位置
			g_aBullet[nCntBullet].move = move;	// 移動量
			g_aBullet[nCntBullet].type = type;	// 種類

			// 寿命を設定
			g_aBullet[nCntBullet].nLife = LIFE_BULLET;

			// 使用している状態にする
			g_aBullet[nCntBullet].bUse = true;

			// 影のインデックスを設定
			g_aBullet[nCntBullet].nShadowID = SetShadow(SHADOW_BULL, &g_aBullet[nCntBullet].bUse);

			// 影の位置設定
			SetPositionShadow(g_aBullet[nCntBullet].nShadowID, g_aBullet[nCntBullet].pos);

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	敵と弾の当たり判定
//======================================================================================================================
void CollisionEnemy(Bullet *pBullet)
{
	// ポインタを宣言
	Enemy      *pEnemy      = GetEnemy();		// 敵の情報
	ModelEnemy *pModelEnemy = GetModelEnemy();	// 敵のモデル情報

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{ // 敵の最大表示数分繰り返す

		if (pEnemy->bUse == true)
		{ // 敵が使用されている場合

			if (pBullet->pos.x + BULL_WIDTH  > pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pEnemy->pos.x + pModelEnemy[pEnemy->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pEnemy->pos.y + pModelEnemy[pEnemy->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pEnemy->pos.z + pModelEnemy[pEnemy->nType].vtxMax.z)
			{ // 弾が敵に当たっている場合

				// 敵のダメージ判定
				HitEnemy(pEnemy);

				//// 爆発の設定
				//SetExplosion(pBullet->pos);

				//// パーティクルの設定
				//SetParticle
				//( // 引数
				//	pBullet->pos,				// 位置
				//	PARTICLETYPE_EXPLOSION,		// 種類
				//	SPAWN_PARTICLE_EXPLOSION,	// エフェクト数
				//	2							// 寿命
				//);

				// 使用していない状態にする
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトと弾の当たり判定
//======================================================================================================================
void CollisionObject(Bullet *pBullet)
{
	// ポインタを宣言
	Object      *pObject      = GetObjectData();	// オブジェクトの情報
	ModelObject *pModelObject = GetModelObject();	// オブジェクトのモデル情報

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (pObject->bUse == true)
		{ // オブジェクトが使用されている場合

			if (pBullet->pos.x + BULL_WIDTH  > pObject->pos.x + pModelObject[pObject->nType].vtxMin.x
			&&  pBullet->pos.y + BULL_HEIGHT > pObject->pos.y + pModelObject[pObject->nType].vtxMin.y
			&&  pBullet->pos.z + BULL_WIDTH  > pObject->pos.z + pModelObject[pObject->nType].vtxMin.z
			&&  pBullet->pos.x - BULL_WIDTH  < pObject->pos.x + pModelObject[pObject->nType].vtxMax.x
			&&  pBullet->pos.y - BULL_HEIGHT < pObject->pos.y + pModelObject[pObject->nType].vtxMax.y
			&&  pBullet->pos.z - BULL_WIDTH  < pObject->pos.z + pModelObject[pObject->nType].vtxMax.z)
			{ // 弾がオブジェクトに当たっている場合

				// 爆発の設定
				SetExplosion(pBullet->pos);

				// パーティクルの設定
				SetParticle
				( // 引数
					pBullet->pos,				// 位置
					PARTICLETYPE_EXPLOSION,		// 種類
					SPAWN_PARTICLE_EXPLOSION,	// エフェクト数
					2							// 寿命
				);

				// 使用していない状態にする
				pBullet->bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	メッシュウォールと弾の当たり判定
//======================================================================================================================
void CollisionMeshWall(Bullet *pBullet)
{
	// 変数を宣言
	D3DXVECTOR3 posLeft, posRight, vecLine, vecToPos;	// 当たり判定の計算用

	// ポインタを宣言
	MeshWall *pMeshWall = GetMeshWall();				// メッシュウォールの情報

	for (int nCntObject = 0; nCntObject < MAX_MESHWALL; nCntObject++, pMeshWall++)
	{ // メッシュウォールの最大表示数分繰り返す

		if (pMeshWall->bUse == true)
		{ // メッシュウォールが使用されている場合
	
			// メッシュウォールの左端の座標を求める
			posLeft = D3DXVECTOR3
			( // 引数
				pMeshWall->pos.x - sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z - cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// メッシュウォールの右端の座標を求める
			posRight = D3DXVECTOR3
			( // 引数
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f),	// x
				0.0f,																						// y
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + (D3DX_PI * 0.5f)) * (pMeshWall->fWidth * 0.5f)	// z
			);

			// 境界線ベクトルを求める
			vecLine  = posRight - posLeft;

			// メッシュウォールの左端と弾の位置のベクトルを求める
			vecToPos = pBullet->pos - posLeft;

			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0
			&&  pBullet->pos.y - BULL_HEIGHT < pMeshWall->pos.y + pMeshWall->fHeight
			&&  pBullet->pos.y + BULL_HEIGHT > pMeshWall->pos.y)
			{ // 弾がメッシュウォールに当たっている場合

				// 爆発の設定
				SetExplosion(pBullet->pos);

				// パーティクルの設定
				SetParticle
				( // 引数
					pBullet->pos,				// 位置
					PARTICLETYPE_EXPLOSION,		// 種類
					SPAWN_PARTICLE_EXPLOSION,	// エフェクト数
					2							// 寿命
				);

				// 使用していない状態にする
				pBullet->bUse = false;
			}
		}
	}
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	弾の総数取得処理
//**********************************************************************************************************************
int GetNumBullet(void)
{
	// 変数を宣言
	int nNumBullet = 0;	// 弾の総数の確認用

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{ // 弾の最大表示数分繰り返す

		if (g_aBullet[nCntBullet].bUse == true)
		{ // 弾が使用されている場合

			// カウンターを加算
			nNumBullet++;
		}
	}

	// 変数の値を返す
	return nNumBullet;	// 弾の総数
}
#endif
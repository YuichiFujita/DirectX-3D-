//======================================================================================================================
//
//	敵処理 [enemy.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "enemy.h"
#include "shadow.h"

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apModelEnemy[] =		// モデルの相対パス
{
	"data\\MODEL\\flower000.x",		// 花のモデル相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (ENEMYTYPE)
//**********************************************************************************************************************
typedef enum
{
	ENEMYTYPE_FLOWER = 0,			// 花
	ENEMYTYPE_MAX,					// この列挙型の総数
} ENEMYTYPE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
ModelEnemy g_aModelEnemy[ENEMYTYPE_MAX];		// 敵のモデル情報
Enemy      g_aEnemy[MAX_ENEMY];					// 敵の情報

//======================================================================================================================
//	敵の初期化処理
//======================================================================================================================
void InitEnemy(void)
{
	// 変数を宣言
	int         nNumVtx;			// モデルの頂点数
	DWORD       dwSizeFVF;			// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;			// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;				// モデルの頂点座標

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL      *pMat;					// マテリアルへのポインタ

	// 敵のモデル情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		g_aModelEnemy[nCntEnemy].apTexture[TEXTURE_ENEMY_MAX] = {};			// テクスチャへのポインタ
		g_aModelEnemy[nCntEnemy].pMesh    = NULL;							// メッシュ (頂点情報) へのポインタ
		g_aModelEnemy[nCntEnemy].pBuffMat = NULL;							// マテリアルへのポインタ
		g_aModelEnemy[nCntEnemy].dwNumMat = 0;								// マテリアルの数
		g_aModelEnemy[nCntEnemy].vtxMin   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最小の頂点座標
		g_aModelEnemy[nCntEnemy].vtxMax   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最大の頂点座標
	}

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // 敵の最大表示数分繰り返す

		g_aEnemy[nCntEnemy].pos           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aEnemy[nCntEnemy].rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aEnemy[nCntEnemy].state         = ENEMYSTATE_NONE;				// 状態
		g_aEnemy[nCntEnemy].nType         = 0;								// 種類
		g_aEnemy[nCntEnemy].nCounterState = 0;								// 状態管理カウンター
		g_aEnemy[nCntEnemy].nShadowID     = NONE_SHADOW;					// 影のインデックス
		g_aEnemy[nCntEnemy].bUse          = false;							// 使用状況
	}

	// xファイルの読み込み
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		D3DXLoadMeshFromX
		( // 引数
			apModelEnemy[nCntEnemy],			// モデルの相対パス
			D3DXMESH_SYSTEMMEM,
			pDevice,							// デバイスへのポインタ
			NULL,
			&g_aModelEnemy[nCntEnemy].pBuffMat,	// マテリアルへのポインタ
			NULL,
			&g_aModelEnemy[nCntEnemy].dwNumMat,	// マテリアルの数
			&g_aModelEnemy[nCntEnemy].pMesh		// メッシュ (頂点情報) へのポインタ
		);
	}

	// 当たり判定の作成
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		// モデルの頂点数を取得
		nNumVtx = g_aModelEnemy[nCntEnemy].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelEnemy[nCntEnemy].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		g_aModelEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 最小頂点座標の確認
			if (vtx.x < g_aModelEnemy[nCntEnemy].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				g_aModelEnemy[nCntEnemy].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_aModelEnemy[nCntEnemy].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				g_aModelEnemy[nCntEnemy].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_aModelEnemy[nCntEnemy].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				g_aModelEnemy[nCntEnemy].vtxMin.z = vtx.z;
			}

			// 最大頂点座標の確認
			if (vtx.x > g_aModelEnemy[nCntEnemy].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				g_aModelEnemy[nCntEnemy].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_aModelEnemy[nCntEnemy].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				g_aModelEnemy[nCntEnemy].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_aModelEnemy[nCntEnemy].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				g_aModelEnemy[nCntEnemy].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		g_aModelEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
	}

	// テクスチャの読み込み
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aModelEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelEnemy[nCntEnemy].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelEnemy[nCntEnemy].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	敵の終了処理
//======================================================================================================================
void UninitEnemy(void)
{
	// テクスチャの破棄
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		for (int nCntMat = 0; nCntMat < TEXTURE_ENEMY_MAX; nCntMat++)
		{ // 使用するテクスチャ数分繰り返す

			if (g_aModelEnemy[nCntEnemy].apTexture[nCntMat] != NULL)
			{ // 変数 (g_aModelEnemy[nCntEnemy].apTexture) がNULLではない場合

				g_aModelEnemy[nCntEnemy].apTexture[nCntMat]->Release();
				g_aModelEnemy[nCntEnemy].apTexture[nCntMat] = NULL;
			}
		}
	}

	// メッシュの破棄
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		if (g_aModelEnemy[nCntEnemy].pMesh != NULL)
		{ // 変数 (g_aModelEnemy[nCntEnemy].pMesh) がNULLではない場合

			g_aModelEnemy[nCntEnemy].pMesh->Release();
			g_aModelEnemy[nCntEnemy].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{ // 敵の種類分繰り返す

		if (g_aModelEnemy[nCntEnemy].pBuffMat != NULL)
		{ // 変数 (g_aModelEnemy[nCntEnemy].pBuffMat) がNULLではない場合

			g_aModelEnemy[nCntEnemy].pBuffMat->Release();
			g_aModelEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	敵の更新処理
//======================================================================================================================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // 敵の最大表示数分繰り返す

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // 敵が使用されている場合

			if (g_aEnemy[nCntEnemy].nCounterState > 0)
			{ // カウンターが 0より大きい場合

				// カウンターを減算
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{ // カウンターが 0以下の場合

					// 通常状態にする
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
			}
		}
	}
}

//======================================================================================================================
//	敵の描画処理
//======================================================================================================================
void DrawEnemy(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL      redMat;					// マテリアルデータ (赤)

	// 構造体の要素をクリア
	ZeroMemory(&redMat, sizeof(D3DXMATERIAL));

	// 拡散光・環境光を赤にする
	redMat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	redMat.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // 敵の最大表示数分繰り返す

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // 敵が使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aModelEnemy[g_aEnemy[nCntEnemy].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelEnemy[g_aEnemy[nCntEnemy].nType].dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				switch (g_aEnemy[nCntEnemy].state)
				{ // 状態ごとの処理
				case ENEMYSTATE_NORMAL:

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// 通常

					// 処理を抜ける
					break;

				case ENEMYSTATE_DAMAGE:

					// マテリアルの設定
					pDevice->SetMaterial(&redMat.MatD3D);			// 赤

					// 処理を抜ける
					break;
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_aModelEnemy[g_aEnemy[nCntEnemy].nType].apTexture[nCntMat]);

				// モデルの描画
				g_aModelEnemy[g_aEnemy[nCntEnemy].nType].pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	敵の設定処理
//======================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // 敵の最大表示数分繰り返す

		if (g_aEnemy[nCntEnemy].bUse == false)
		{ // 敵が使用されていない場合

			// 引数を代入
			g_aEnemy[nCntEnemy].pos   = pos;		// 位置
			g_aEnemy[nCntEnemy].rot   = rot;		// 向き
			g_aEnemy[nCntEnemy].nType = nType;		// 種類

			// 敵の情報を初期化
			g_aEnemy[nCntEnemy].state         = ENEMYSTATE_NORMAL;	// 状態
			g_aEnemy[nCntEnemy].nCounterState = 0;					// 状態管理カウンター

			// 使用している状態にする
			g_aEnemy[nCntEnemy].bUse = true;

			// 影のインデックスを設定
			g_aEnemy[nCntEnemy].nShadowID = SetShadow
			( // 引数
				fabsf(g_aModelEnemy[g_aEnemy[nCntEnemy].nType].vtxMax.x - g_aModelEnemy[g_aEnemy[nCntEnemy].nType].vtxMin.x),	// 半径
				&g_aEnemy[nCntEnemy].bUse																						// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aEnemy[nCntEnemy].nShadowID, g_aEnemy[nCntEnemy].pos);

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	敵のダメージ判定
//======================================================================================================================
void HitEnemy(Enemy *pEnemy)
{
	// ダメージ状態にする
	pEnemy->state = ENEMYSTATE_DAMAGE;

	// カウンターを設定
	pEnemy->nCounterState = DAMAGE_TIME_ENEMY;
}

//======================================================================================================================
//	敵の取得処理
//======================================================================================================================
Enemy *GetEnemy(void)
{
	// 敵の情報の先頭アドレスを返す
	return &g_aEnemy[0];
}

//======================================================================================================================
//	敵のモデル情報の取得処理
//======================================================================================================================
ModelEnemy *GetModelEnemy(void)
{
	// 敵のモデル情報の先頭アドレスを返す
	return &g_aModelEnemy[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	敵の総数取得処理
//**********************************************************************************************************************
int GetNumEnemy(void)
{
	// 変数を宣言
	int nNumEnemy = 0;	// 敵の総数の確認用

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ // 敵の最大表示数分繰り返す

		if (g_aEnemy[nCntEnemy].bUse == true)
		{ // 敵が使用されている場合

			// カウンターを加算
			nNumEnemy++;
		}
	}

	// 変数の値を返す
	return nNumEnemy;	// 敵の総数
}
#endif
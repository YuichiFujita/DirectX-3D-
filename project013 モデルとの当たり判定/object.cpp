//======================================================================================================================
//
//	オブジェクト処理 [object.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "object.h"
#include "shadow.h"

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apModelObject[] =		// モデルの相対パス
{
	"data\\MODEL\\pillar000.x",		// 柱のモデル相対パス
	"data\\MODEL\\save000.x",		// 星のモデル相対パス
	"data\\MODEL\\box000.x",		// 箱のモデル相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (OBJECTTYPE)
//**********************************************************************************************************************
typedef enum
{
	OBJECTTYPE_PILLAR = 0,			// 柱
	OBJECTTYPE_STAR,				// 星
	OBJECTTYPE_BOX,					// 箱
	OBJECTTYPE_MAX,					// この列挙型の総数
} OBJECTTYPE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
ModelObject g_aModelObject[OBJECTTYPE_MAX];		// オブジェクトのモデル情報
Object      g_aObject[MAX_OBJECT];				// オブジェクトの情報

//======================================================================================================================
//	オブジェクトの初期化処理
//======================================================================================================================
void InitObject(void)
{
	// 変数を宣言
	int         nNumVtx;			// モデルの頂点数
	DWORD       dwSizeFVF;			// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;			// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;				// モデルの頂点座標

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL      *pMat;					// マテリアルへのポインタ

	// オブジェクトのモデル情報の初期化
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		g_aModelObject[nCntObject].apTexture[TEXTURE_OBJECT_MAX] = {};			// テクスチャへのポインタ
		g_aModelObject[nCntObject].pMesh    = NULL;								// メッシュ (頂点情報) へのポインタ
		g_aModelObject[nCntObject].pBuffMat = NULL;								// マテリアルへのポインタ
		g_aModelObject[nCntObject].dwNumMat = 0;								// マテリアルの数
		g_aModelObject[nCntObject].vtxMin   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最小の頂点座標
		g_aModelObject[nCntObject].vtxMax   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最大の頂点座標
	}

	// オブジェクトの情報の初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		g_aObject[nCntObject].pos       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aObject[nCntObject].rot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aObject[nCntObject].nType     = 0;									// 種類
		g_aObject[nCntObject].nShadowID = NONE_SHADOW;							// 影のインデックス
		g_aObject[nCntObject].bUse      = false;								// 使用状況
	}

	// xファイルの読み込み
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		D3DXLoadMeshFromX
		( // 引数
			apModelObject[nCntObject],				// モデルの相対パス
			D3DXMESH_SYSTEMMEM,
			pDevice,								// デバイスへのポインタ
			NULL,
			&g_aModelObject[nCntObject].pBuffMat,	// マテリアルへのポインタ
			NULL,
			&g_aModelObject[nCntObject].dwNumMat,	// マテリアルの数
			&g_aModelObject[nCntObject].pMesh		// メッシュ (頂点情報) へのポインタ
		);
	}

	// 当たり判定の作成
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		// モデルの頂点数を取得
		nNumVtx = g_aModelObject[nCntObject].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelObject[nCntObject].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		g_aModelObject[nCntObject].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 最小頂点座標の確認
			if (vtx.x < g_aModelObject[nCntObject].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				g_aModelObject[nCntObject].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_aModelObject[nCntObject].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				g_aModelObject[nCntObject].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_aModelObject[nCntObject].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				g_aModelObject[nCntObject].vtxMin.z = vtx.z;
			}

			// 最大頂点座標の確認
			if (vtx.x > g_aModelObject[nCntObject].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				g_aModelObject[nCntObject].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_aModelObject[nCntObject].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				g_aModelObject[nCntObject].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_aModelObject[nCntObject].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				g_aModelObject[nCntObject].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		g_aModelObject[nCntObject].pMesh->UnlockVertexBuffer();
	}

	// テクスチャの読み込み
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aModelObject[nCntObject].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelObject[nCntObject].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelObject[nCntObject].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの終了処理
//======================================================================================================================
void UninitObject(void)
{
	// テクスチャの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		for (int nCntMat = 0; nCntMat < TEXTURE_OBJECT_MAX; nCntMat++)
		{ // 使用するテクスチャ数分繰り返す

			if (g_aModelObject[nCntObject].apTexture[nCntMat] != NULL)
			{ // 変数 (g_aModelObject[nCntObject].apTexture) がNULLではない場合

				g_aModelObject[nCntObject].apTexture[nCntMat]->Release();
				g_aModelObject[nCntObject].apTexture[nCntMat] = NULL;
			}
		}
	}

	// メッシュの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		if (g_aModelObject[nCntObject].pMesh != NULL)
		{ // 変数 (g_aModelObject[nCntObject].pMesh) がNULLではない場合

			g_aModelObject[nCntObject].pMesh->Release();
			g_aModelObject[nCntObject].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{ // オブジェクトの種類分繰り返す

		if (g_aModelObject[nCntObject].pBuffMat != NULL)
		{ // 変数 (g_aModelObject[nCntObject].pBuffMat) がNULLではない場合

			g_aModelObject[nCntObject].pBuffMat->Release();
			g_aModelObject[nCntObject].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	オブジェクトの更新処理
//======================================================================================================================
void UpdateObject(void)
{

}

//======================================================================================================================
//	オブジェクトの描画処理
//======================================================================================================================
void DrawObject(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aModelObject[g_aObject[nCntObject].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModelObject[g_aObject[nCntObject].nType].dwNumMat; nCntMat++)
			{ // マテリアルの数分繰り返す

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aModelObject[g_aObject[nCntObject].nType].apTexture[nCntMat]);

				// モデルの描画
				g_aModelObject[g_aObject[nCntObject].nType].pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================================================
//	オブジェクトの設定処理
//======================================================================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == false)
		{ // オブジェクトが使用されていない場合

			// 引数を代入
			g_aObject[nCntObject].pos   = pos;		// 位置
			g_aObject[nCntObject].rot   = rot;		// 向き
			g_aObject[nCntObject].nType = nType;	// 種類

			// 使用している状態にする
			g_aObject[nCntObject].bUse = true;

			// 影のインデックスを設定
			g_aObject[nCntObject].nShadowID = SetShadow
			( // 引数
				fabsf(g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x - g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x),	// 半径
				&g_aObject[nCntObject].bUse																							// 影の親の使用状況
			);

			// 影の位置設定
			SetPositionShadow(g_aObject[nCntObject].nShadowID, g_aObject[nCntObject].pos);

			// 処理を抜ける
			break;
		}
	}
}

//======================================================================================================================
//	オブジェクトとの当たり判定
//======================================================================================================================
void CollisionObject(Player *pPlayer)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// 前後の当たり判定
			if (pPlayer->pos.x + PLAY_WIDTH > g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x
			&&  pPlayer->pos.x - PLAY_WIDTH < g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x)
			{ // ブロックの左右の範囲内の場合

				if (pPlayer->pos.z    + PLAY_WIDTH >  g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z
				&&  pPlayer->oldPos.z + PLAY_WIDTH <= g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z)
				{ // 前からの当たり判定

					// 位置を補正
					pPlayer->pos.z = g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z - PLAY_WIDTH;

					// 移動量を初期化
					pPlayer->move.z = 0.0f;
				}
				else if (pPlayer->pos.z    - PLAY_WIDTH <  g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z
				     &&  pPlayer->oldPos.z - PLAY_WIDTH >= g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z)
				{ // 後ろからの当たり判定

					// 位置を補正
					pPlayer->pos.z = g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z + PLAY_WIDTH;

					// 移動量を初期化
					pPlayer->move.z = 0.0f;
				}
			}

			// 左右の当たり判定
			if (pPlayer->pos.z + PLAY_WIDTH > g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.z
			&&  pPlayer->pos.z - PLAY_WIDTH < g_aObject[nCntObject].pos.z + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.z)
			{ // ブロックの前後の範囲内の場合

				if (pPlayer->pos.x    + PLAY_WIDTH >  g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x
				&&  pPlayer->oldPos.x + PLAY_WIDTH <= g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x)
				{ // 左からの当たり判定

					// 位置を補正
					pPlayer->pos.x = g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMin.x - PLAY_WIDTH;

					// 移動量を初期化
					pPlayer->move.x = 0.0f;
				}
				else if (pPlayer->pos.x    - PLAY_WIDTH <  g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x
				     &&  pPlayer->oldPos.x - PLAY_WIDTH >= g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x)
				{ // 右からの当たり判定

					// 位置を補正
					pPlayer->pos.x = g_aObject[nCntObject].pos.x + g_aModelObject[g_aObject[nCntObject].nType].vtxMax.x + PLAY_WIDTH;

					// 移動量を初期化
					pPlayer->move.x = 0.0f;
				}
			}
		}
	}
}

//======================================================================================================================
//	オブジェクトの取得処理
//======================================================================================================================
Object *GetObjectData(void)
{
	// オブジェクトの情報の先頭アドレスを返す
	return &g_aObject[0];
}

//======================================================================================================================
//	オブジェクトのモデル情報の取得処理
//======================================================================================================================
ModelObject *GetModelObject(void)
{
	// オブジェクトのモデル情報の先頭アドレスを返す
	return &g_aModelObject[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	オブジェクトの総数取得処理
//**********************************************************************************************************************
int GetNumObject(void)
{
	// 変数を宣言
	int nNumObject = 0;	// オブジェクトの総数の確認用

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{ // オブジェクトの最大表示数分繰り返す

		if (g_aObject[nCntObject].bUse == true)
		{ // オブジェクトが使用されている場合

			// カウンターを加算
			nNumObject++;
		}
	}

	// 変数の値を返す
	return nNumObject;	// オブジェクトの総数
}
#endif
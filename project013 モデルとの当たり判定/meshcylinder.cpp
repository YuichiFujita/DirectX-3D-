//======================================================================================================================
//
//	メッシュシリンダー処理 [meshcylinder.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "meshcylinder.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_MESHCYLINDER	(1)			// 使用するポリゴン数 (メッシュシリンダーの最大数)
#define PARTITION_WIDTH		(8)			// 横の分割数
#define PARTITION_HEIGHT	(1)			// 縦の分割数
//#define MESHCYLINDER_WIDTH	(800)	// メッシュシリンダーの横幅
//#define MESHCYLINDER_HEIGHT	(200)	// メッシュシリンダーの縦幅

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureMeshCylinder[] =	// テクスチャの相対パス
{
	"data\\TEXTURE\\mountain000.png",	// メッシュシリンダーのテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_MESHCYLINDER)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_MESHCYLINDER_NORMAL = 0,	// メッシュシリンダー (通常)
	TEXTURE_MESHCYLINDER_MAX,			// この列挙型の総数
} TEXTURE_MESHCYLINDER;

//**********************************************************************************************************************
//	構造体定義 (MeshCylinder)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX  mtxWorld;				// ワールドマトリックス
	float       fRadius;				// 半径
	float       fHeight;				// 縦幅
	int         nPartWidth;				// 横の分割数
	int         nPartHeight;			// 縦の分割数
	int         nNumVtx;				// 必要頂点数
	int         nNumIdx;				// 必要インデックス数
	bool        bUse;					// 使用状況
} MeshCylinder;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight);	// メッシュシリンダーの設定処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureMeshCylinder[TEXTURE_MESHCYLINDER_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;							// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffMeshCylinder = NULL;							// インデックスバッファへのポインタ

MeshCylinder g_aMeshCylinder[MAX_MESHCYLINDER];	// メッシュシリンダーの情報
int          g_nNeedVtxCylinder;				// 必要頂点数
int          g_nNeedIdxCylinder;				// 必要インデックス数

//======================================================================================================================
//	メッシュシリンダーの初期化処理
//======================================================================================================================
void InitMeshCylinder(void)
{
	// 変数を宣言
	D3DXVECTOR3 vecPos;							// 頂点位置の計算用
	int         nNumVtx = 0;					// 頂点数の計測用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	WORD      *pIdx;							// インデックス情報へのポインタ
	
	// グローバル変数の初期化
	g_nNeedVtxCylinder = 0;						// 必要頂点の総数
	g_nNeedIdxCylinder = 0;						// 必要インデックスの総数

	// メッシュシリンダーの情報の初期化
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // メッシュシリンダーの最大表示数分繰り返す

		g_aMeshCylinder[nCntMeshCylinder].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aMeshCylinder[nCntMeshCylinder].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aMeshCylinder[nCntMeshCylinder].fRadius     = 0.0f;							// 半径
		g_aMeshCylinder[nCntMeshCylinder].fHeight     = 0.0f;							// 縦幅
		g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = 0;								// 横の分割数
		g_aMeshCylinder[nCntMeshCylinder].nPartHeight = 0;								// 縦の分割数
		g_aMeshCylinder[nCntMeshCylinder].nNumVtx     = 0;								// 必要頂点数
		g_aMeshCylinder[nCntMeshCylinder].nNumIdx     = 0;								// 必要インデックス数
		g_aMeshCylinder[nCntMeshCylinder].bUse        = false;							// 使用状況
	}

	// テクスチャの読み込み
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureMeshCylinder[nCntMeshCylinder], &g_apTextureMeshCylinder[nCntMeshCylinder]);
	}

	// メッシュシリンダーの設定
	SetMeshCylinder(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000.0f, 800.0f, 16, 1);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * g_nNeedVtxCylinder,	// 必要頂点数
		D3DUSAGE_WRITEONLY,						// 使用方法
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,						// メモリの指定
		&g_pVtxBuffMeshCylinder,				// 頂点バッファへのポインタ
		NULL
	);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * g_nNeedIdxCylinder,		// 必要インデックス数
		D3DUSAGE_WRITEONLY,						// 使用方法
		D3DFMT_INDEX16,							// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,						// メモリの指定
		&g_pIdxBuffMeshCylinder,				// インデックスバッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // メッシュシリンダーの最大表示数分繰り返す

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // メッシュシリンダーが使用されている場合

			for (int nCntHeight = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight + 1; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					// 頂点座標の方向を設定
					vecPos = D3DXVECTOR3
					( // 引数
						sinf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius,						// x
						-(nCntHeight * (g_aMeshCylinder[nCntMeshCylinder].fHeight / (float)g_aMeshCylinder[nCntMeshCylinder].nPartHeight)) + g_aMeshCylinder[nCntMeshCylinder].fHeight,	// y
						cosf(D3DXToRadian(nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))) * g_aMeshCylinder[nCntMeshCylinder].fRadius							// z
					);

					// 頂点座標の設定
					pVtx[0].pos = g_aMeshCylinder[nCntMeshCylinder].pos + vecPos;

					//// 法線ベクトルの方向を設定
					//pos = D3DXVECTOR3
					//(
					//	sinf(D3DXToRadian(fabsf(180 - nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth)))),
					//	0.0f,
					//	cosf(D3DXToRadian(fabsf(180 - nCntWidth * (360 / (float)g_aMeshCylinder[nCntMeshCylinder].nPartWidth))))
					//);
					//D3DXVec3Normalize(&pos, &pos);	// ベクトルを正規化
					//// 法線ベクトルの設定
					//pVtx[0].nor = pos;

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//// テクスチャ座標の設定
					//pVtx[0].tex = D3DXVECTOR2
					//( // 引数
					//	nCntWidth * (1.0f / (float)g_aMeshField[nCntMeshField].nPartWidth) - (1.0f * 0.5f),		// u
					//	nCntHeight * (1.0f / (float)g_aMeshField[nCntMeshField].nPartHeight) - (1.0f * 0.5f)	// v
					//);

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * (nCntWidth % 2), 1.0f * nCntHeight);

					// 頂点データのポインタを 1つ分進める
					pVtx += 1;
				}
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // メッシュシリンダーの最大表示数分繰り返す

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // メッシュシリンダーが使用されている場合

			for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < g_aMeshCylinder[nCntMeshCylinder].nPartHeight; nCntHeight++)
			{ // 縦の分割数 +1回繰り返す

				for (nCntWidth = 0; nCntWidth < g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1; nCntWidth++)
				{ // 横の分割数 +1回繰り返す

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 1) + nCntWidth);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth);

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}

				if (nCntHeight != g_aMeshCylinder[nCntMeshCylinder].nPartHeight - 1)
				{ // 一番手前の分割場所ではない場合

					pIdx[0] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * nCntHeight + nCntWidth - 1);
					pIdx[1] = nNumVtx + ((g_aMeshCylinder[nCntMeshCylinder].nPartWidth + 1) * (nCntHeight + 2));

					// インデックスデータのポインタを 2つ分進める
					pIdx += 2;
				}
			}

			// 頂点バッファの開始地点を必要数分ずらす
			nNumVtx += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshCylinder->Unlock();
}

//======================================================================================================================
//	メッシュシリンダーの終了処理
//======================================================================================================================
void UninitMeshCylinder(void)
{
	// テクスチャの破棄
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < TEXTURE_MESHCYLINDER_MAX; nCntMeshCylinder++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureMeshCylinder[nCntMeshCylinder] != NULL)
		{ // 変数 (g_apTextureMeshCylinder) がNULLではない場合

			g_apTextureMeshCylinder[nCntMeshCylinder]->Release();
			g_apTextureMeshCylinder[nCntMeshCylinder] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{ // 変数 (g_pVtxBuffMeshCylinder) がNULLではない場合

		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{ // 変数 (g_pIdxBuffMeshCylinder) がNULLではない場合

		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//======================================================================================================================
//	メッシュシリンダーの更新処理
//======================================================================================================================
void UpdateMeshCylinder(void)
{

}

//======================================================================================================================
//	メッシュシリンダーの描画処理
//======================================================================================================================
void DrawMeshCylinder(void)
{
	// 変数を宣言
	int        nNumIdx = 0;						// インデックス数の計測用
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // メッシュシリンダーの最大表示数分繰り返す

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == true)
		{ // メッシュシリンダーが使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder[nCntMeshCylinder].rot.y, g_aMeshCylinder[nCntMeshCylinder].rot.x, g_aMeshCylinder[nCntMeshCylinder].rot.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder[nCntMeshCylinder].pos.x, g_aMeshCylinder[nCntMeshCylinder].pos.y, g_aMeshCylinder[nCntMeshCylinder].pos.z);
			D3DXMatrixMultiply(&g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder[nCntMeshCylinder].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshCylinder[TEXTURE_MESHCYLINDER_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive
			( // 引数
				D3DPT_TRIANGLESTRIP,							// プリミティブの種類
				0,
				0,
				g_aMeshCylinder[nCntMeshCylinder].nNumVtx,		// 使用する頂点数
				nNumIdx,										// インデックスバッファの開始地点
				g_aMeshCylinder[nCntMeshCylinder].nNumIdx - 2	// プリミティブ (ポリゴン) 数
			);

			// インデックスバッファの開始地点を必要数分ずらす
			nNumIdx += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	メッシュシリンダーの設定処理
//======================================================================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nPartWidth, int nPartHeight)
{
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{ // 壁の最大表示数分繰り返す

		if (g_aMeshCylinder[nCntMeshCylinder].bUse == false)
		{ // 壁が使用されていない場合

			// 引数を代入
			g_aMeshCylinder[nCntMeshCylinder].pos         = pos;			// 位置
			g_aMeshCylinder[nCntMeshCylinder].rot         = rot;			// 向き
			g_aMeshCylinder[nCntMeshCylinder].fRadius     = fRadius;		// 半径
			g_aMeshCylinder[nCntMeshCylinder].fHeight     = fHeight;		// 縦幅
			g_aMeshCylinder[nCntMeshCylinder].nPartWidth  = nPartWidth;		// 横の分割数
			g_aMeshCylinder[nCntMeshCylinder].nPartHeight = nPartHeight;	// 縦の分割数

			// 使用している状態にする
			g_aMeshCylinder[nCntMeshCylinder].bUse = true;

			// 頂点バッファとインデックスバッファの必要数を設定
			g_aMeshCylinder[nCntMeshCylinder].nNumVtx = (nPartWidth + 1) * (nPartHeight + 1);
			g_aMeshCylinder[nCntMeshCylinder].nNumIdx = (nPartWidth + 1) * (((nPartHeight + 1) * 2) - 2) + (nPartHeight * 2) - 2;

			// 頂点バッファとインデックスバッファの総数を加算
			g_nNeedVtxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumVtx;
			g_nNeedIdxCylinder += g_aMeshCylinder[nCntMeshCylinder].nNumIdx;

			// 処理を抜ける
			break;
		}
	}
}
//======================================================================================================================
//
//	影処理 [shadow.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "shadow.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_SHADOW		(64)		// 使用するポリゴン数 (影の最大数)

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureShadow[] =		// テクスチャの相対パス
{
	"data\\TEXTURE\\shadow000.jpg",	// 影のテクスチャの相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_SHADOW)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_SHADOW_NORMAL = 0,		// 影 (通常)
	TEXTURE_SHADOW_MAX,				// この列挙型の総数
} TEXTURE_SHADOW;

//**********************************************************************************************************************
//	構造体定義 (Shadow)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	D3DXMATRIX  mtxWorld;			// ワールドマトリックス
	bool       *pUseParent;			// 影の親の使用状況
	bool        bUse;				// 使用状況
} Shadow;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureShadow[TEXTURE_SHADOW_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;					// 頂点バッファへのポインタ

Shadow g_aShadow[MAX_SHADOW];		// 影の情報

//======================================================================================================================
//	影の初期化処理
//======================================================================================================================
void InitShadow(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// テクスチャの読み込み
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // 使用するテクスチャ数分繰り返す

		D3DXCreateTextureFromFile(pDevice, apTextureShadow[nCntShadow], &g_apTextureShadow[nCntShadow]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,		// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	// 影の情報の初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		g_aShadow[nCntShadow].pos        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aShadow[nCntShadow].rot        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aShadow[nCntShadow].pUseParent = NULL;							// 影の親の使用状況
		g_aShadow[nCntShadow].bUse       = false;							// 使用状況
	}

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffShadow->Unlock();
}

//======================================================================================================================
//	影の終了処理
//======================================================================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	for (int nCntShadow = 0; nCntShadow < TEXTURE_SHADOW_MAX; nCntShadow++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureShadow[nCntShadow] != NULL)
		{ // 変数 (g_apTextureShadow) がNULLではない場合

			g_apTextureShadow[nCntShadow]->Release();
			g_apTextureShadow[nCntShadow] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{ // 変数 (g_pVtxBuffShadow) がNULLではない場合

		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//======================================================================================================================
//	影の更新処理
//======================================================================================================================
void UpdateShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == true && g_aShadow[nCntShadow].pUseParent != NULL)
		{ // 影が使用されている且つ、影の親アドレスが設定されている場合

			if (*g_aShadow[nCntShadow].pUseParent == false)
			{ // 影の親が使用されていない場合

				// 影を使用していない状態にする
				g_aShadow[nCntShadow].bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	影の描画処理
//======================================================================================================================
void DrawShadow(void)
{
	// 変数を宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == true)
		{ // 影が使用されている場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureShadow[TEXTURE_SHADOW_NORMAL]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
//	影の設定処理
//======================================================================================================================
int SetShadow(float fRadius, bool *pUse)
{
	// ポインタを宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{ // 影の最大表示数分繰り返す

		if (g_aShadow[nCntShadow].bUse == false)
		{ // 影が使用されていない場合

			// 引数のアドレスを代入
			g_aShadow[nCntShadow].pUseParent = pUse;	// 影の親の使用状況

			// 使用している状態にする
			g_aShadow[nCntShadow].bUse = true;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを使用する影のポインタまで進める
			pVtx += nCntShadow * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f,  fRadius);
			pVtx[1].pos = D3DXVECTOR3( fRadius, 0.0f,  fRadius);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
			pVtx[3].pos = D3DXVECTOR3( fRadius, 0.0f, -fRadius);

			// 頂点バッファをアンロックする
			g_pVtxBuffShadow->Unlock();

			// 使用できる影のIDを返す
			return nCntShadow;
		}
	}

	// 影の総数が足りない場合 -1を返す
	return -1;
}

//======================================================================================================================
//	影の位置の設定処理
//======================================================================================================================
void SetPositionShadow(int nShadowID, D3DXVECTOR3 pos)
{
	if (nShadowID >= 0)
	{ // IDが 0以上の場合

		// 引数の影の位置を設定
		g_aShadow[nShadowID].pos.x = pos.x;
		g_aShadow[nShadowID].pos.z = pos.z;
	}
}
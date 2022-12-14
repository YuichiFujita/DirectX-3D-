//======================================================================================================================
//
//	タイトル画面処理 [title.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "title.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_TITLE		(2)				// 使用するポリゴン数

#define TITLE_POS_X		(640.0f)		// タイトルの停止時の絶対座標 (x)
#define TITLE_POS_Y		(220.0f)		// タイトルの停止時の絶対座標 (y)
#define TITLE_WIDTH		(440.0f)		// タイトルの幅 / 2 (横)
#define TITLE_HEIGHT	(120.0f)		// タイトルの幅 / 2 (高さ)

#define START_HEIGHT_UP		(500.0f)						// スタートの相対座標 上 (y)
#define START_HEIGHT_DOWN	(START_HEIGHT_UP + 110.0f)		// スタートの相対座標 下 (y)

#define TITLE_MOVE			(2.0f)		// タイトルの移動量
#define TIT_FIRST_ALPHA		(0.65f)		// 背景の初期の透明度
#define TIT_CHANGE_ALPHA	(0.003f)	// 背景の透明度の変更値
#define TIT_ALPHA_HIGH		(0.8f)		// 不透明度の最大値
#define TIT_ALPHA_LOW		(0.5f)		// 透明度の最大値

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apTextureTitle[] =			// テクスチャの相対パス
{
	"data\\TEXTURE\\title000.png",		// タイトルのテクスチャ相対パス
	"data\\TEXTURE\\title001.png",		// スタートのテクスチャ相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TITLE_TITLE = 0,			// タイトル
	TEXTURE_TITLE_START,				// スタート
	TEXTURE_TITLE_MAX,					// この列挙型の総数
} TEXTURE_TITLE;

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureTitle[TEXTURE_TITLE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// 頂点バッファへのポインタ

D3DXVECTOR3 g_posTitle;		// タイトルの絶対座標
TITLESTATE  g_stateTitle;	// タイトルの状態
float g_fAlphaTitle;		// 背景の透明度
float g_fChangeTitle;		// 透明度の変更値

//======================================================================================================================
//	タイトル画面の初期化処理
//======================================================================================================================
void InitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの初期化
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // 使用するテクスチャ数分繰り返す

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, apTextureTitle[nCntTitle], &g_apTextureTitle[nCntTitle]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,	// 必要頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	// グローバル変数を初期化
	g_posTitle     = D3DXVECTOR3(TITLE_POS_X, 0.0f, 0.0f);	// タイトルの絶対座標を初期化
	g_stateTitle   = TITLESTATE_MOVE;						// タイトルの状態を移動に初期化
	g_fAlphaTitle  = TIT_FIRST_ALPHA;						// 背景の透明度を初期化
	g_fChangeTitle = TIT_CHANGE_ALPHA;						// 透明度の変更値を初期化

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	タイトルアイコンの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

	// rhw の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	//------------------------------------------------------------------------------------------------------------------
	//	スタートの初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点座標を設定
	pVtx[4].pos = D3DXVECTOR3(0.0f        , START_HEIGHT_UP,   0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, START_HEIGHT_UP,   0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f        , START_HEIGHT_DOWN, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, START_HEIGHT_DOWN, 0.0f);

	// rhw の設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);

	// テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//======================================================================================================================
//	タイトル画面の終了処理
//======================================================================================================================
void UninitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの終了
	//------------------------------------------------------------------------------------------------------------------
	// テクスチャの破棄
	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // 使用するテクスチャ数分繰り返す

		if (g_apTextureTitle[nCntTitle] != NULL)
		{ // 変数 (g_apTextureTitle) がNULLではない場合

			g_apTextureTitle[nCntTitle]->Release();
			g_apTextureTitle[nCntTitle] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{ // 変数 (g_pVtxBuffTitle) がNULLではない場合

		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	サウンドの終了
	//------------------------------------------------------------------------------------------------------------------
	// サウンドの停止
	StopSound();
}

//======================================================================================================================
//	タイトル画面の更新処理
//======================================================================================================================
void UpdateTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの更新
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 演出の更新
	switch (g_stateTitle)
	{ // 状態の確認
	case TITLESTATE_MOVE:	// 移動状態の場合

		// タイトルの絶対座標を移動 (下)
		g_posTitle.y += TITLE_MOVE;

		if (g_posTitle.y >= TITLE_POS_Y)
		{ // タイトルが下がり切った場合

			// y座標を補正
			g_posTitle.y = TITLE_POS_Y;

			// 状態を何もしないに設定
			g_stateTitle = TITLESTATE_NONE;

			//// サウンドの再生
			//PlaySound(SOUND_LABEL_BGM000);	// タイトル・ステージセレクト画面のBGM
		}

		// 処理を抜ける
		break;

	case TITLESTATE_NONE:	// 何もしていない状態の場合

		if (GetFade() == FADE_NONE)
		{ // フェードしていない場合

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
			||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
			||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
			{ // 決定の操作が行われた場合

				//// サウンドの再生
				//PlaySound(SOUND_LABEL_SE_DECISION_00);	// 決定音00 (システム)

				// モード選択 (ステージセレクト画面に移行)
				SetFade(MODE_GAME);
			}

			if (g_fAlphaTitle >= TIT_ALPHA_HIGH || g_fAlphaTitle <= TIT_ALPHA_LOW)
			{ // 透明度が不透明、または透明になった場合

				// 変動量を反転させる
				g_fChangeTitle *= -1.0f;	// ±を逆転させる
			}

			// 透明度を加算
			g_fAlphaTitle += g_fChangeTitle;

			// 頂点カラーの更新
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
		}
		else
		{ // フェード中の場合

			// 頂点カラーの更新
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
		}

		// 処理を抜ける
		break;
	}

	// 演出のスキップ
	if (g_stateTitle != TITLESTATE_NONE && GetFade() == FADE_NONE)
	{ // タイトルが何もしない状態ではない且つ、フェードしていない場合

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
		{ // スキップ操作が行われた場合

			// y座標を補正
			g_posTitle.y = TITLE_POS_Y;

			// 状態を何もしないに設定
			g_stateTitle = TITLESTATE_NONE;

			//// サウンドの再生
			//PlaySound(SOUND_LABEL_BGM000);	// タイトル・ステージセレクト画面のBGM
		}
	}

	// 頂点座標を更新
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//======================================================================================================================
//	タイトル画面の描画処理
//======================================================================================================================
void DrawTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	タイトルの描画
	//------------------------------------------------------------------------------------------------------------------
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ (ポインタの宣言だが * はいらない)

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{ // 使用するポリゴン数分繰り返す

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}
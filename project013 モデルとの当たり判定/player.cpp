//======================================================================================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"

#include "bullet.h"
#include "camera.h"
#include "object.h"
#include "player.h"
#include "shadow.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define SETUP_TXT			"data\\TXT\\motion.txt"		// セットアップ用のテキストファイルの相対パス

#define MAX_STRING			(128)		// セットアップ時のテキスト読み込み文字列の最長

#define MOVE_PLAYER			(1.2f)		// プレイヤーの移動量
#define DELETE_MOVE			(0.01f)		// プレイヤーの移動量が残る最低値
#define SUB_MOVE_PLAYER		(0.25f)		// プレイヤーの移動量の減速係数
#define REV_ROT_PLAYER		(0.1f)		// プレイヤーの回転量の補正係数

#define POS_BULL_PLAYER_X	(30.0f)		// プレイヤーの弾発射位置の加算量 (x)
#define POS_BULL_PLAYER_Y	(17.0f)		// プレイヤーの弾発射位置の加算量 (y)
#define POS_BULL_PLAYER_Z	(30.0f)		// プレイヤーの弾発射位置の加算量 (z)

#define MOVE_BULLET			(6.0f)		// 弾の移動量
#define SHADOW_PLAYER		(35.0f)		// プレイヤーの影の半径

//**********************************************************************************************************************
//	コンスト定義
//**********************************************************************************************************************
const char *apModelPlayer[] =			// モデルの相対パス
{
	"data\\CHARA_MODEL\\body.x",		// 体のモデル相対パス
	"data\\CHARA_MODEL\\head.x",		// 頭のモデル相対パス
	"data\\CHARA_MODEL\\sleeve_l.x",	// 左腕のモデル相対パス
	"data\\CHARA_MODEL\\sleeve_r.x",	// 右腕のモデル相対パス
	"data\\CHARA_MODEL\\wrist_l.x",		// 左手首のモデル相対パス
	"data\\CHARA_MODEL\\wrist_r.x",		// 右手首のモデル相対パス
	"data\\CHARA_MODEL\\hand_l.x",		// 左手のモデル相対パス
	"data\\CHARA_MODEL\\hand_r.x",		// 右手のモデル相対パス
	"data\\CHARA_MODEL\\pants_l.x",		// 左足のモデル相対パス
	"data\\CHARA_MODEL\\pants_r.x",		// 右足のモデル相対パス
	"data\\CHARA_MODEL\\leg_l.x",		// 左腿のモデル相対パス
	"data\\CHARA_MODEL\\leg_r.x",		// 右腿のモデル相対パス
	"data\\CHARA_MODEL\\shoes_l.x",		// 左靴のモデル相対パス
	"data\\CHARA_MODEL\\shoes_r.x",		// 右靴のモデル相対パス
	"data\\CHARA_MODEL\\knife.x",		// ナイフのモデル相対パス
};

//**********************************************************************************************************************
//	列挙型定義 (MODEL_PLAYER)
//**********************************************************************************************************************
typedef enum
{
	MODEL_PLAYER_BODY = 0,				// 体
	MODEL_PLAYER_HEAD,					// 頭
	MODEL_PLAYER_SLEEVE_L,				// 左腕
	MODEL_PLAYER_SLEEVE_R,				// 右腕
	MODEL_PLAYER_WRIST_L,				// 左手首
	MODEL_PLAYER_WRIST_R,				// 右手首
	MODEL_PLAYER_HAND_L,				// 左手
	MODEL_PLAYER_HAND_R,				// 右手
	MODEL_PLAYER_PANTS_L,				// 左足
	MODEL_PLAYER_PANTS_R,				// 右足
	MODEL_PLAYER_LEG_L,					// 左腿
	MODEL_PLAYER_LEG_R,					// 右腿
	MODEL_PLAYER_SHOES_L,				// 左靴
	MODEL_PLAYER_SHOES_R,				// 右靴
	MODEL_PLAYER_KNIFE,					// ナイフ
	MODEL_PLAYER_MAX,					// この列挙型の総数
} MODEL_PLAYER;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void TxtSetPlayer(void);				// プレイヤーのセットアップ処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
Player g_aPlayer[MODEL_PLAYER_MAX];		// プレイヤーの情報

//======================================================================================================================
//	プレイヤーの初期化処理
//======================================================================================================================
void InitPlayer(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL      *pMat;					// マテリアルへのポインタ

	// プレイヤーの情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		g_aPlayer[nCntPlayer].apTexture[TEXTURE_PLAYER_MAX] = {};								// テクスチャへのポインタ
		g_aPlayer[nCntPlayer].pMesh                         = NULL;								// メッシュ (頂点情報) へのポインタ
		g_aPlayer[nCntPlayer].pBuffMat                      = NULL;								// マテリアルへのポインタ
		g_aPlayer[nCntPlayer].dwNumMat                      = 0;								// マテリアルの数
		g_aPlayer[nCntPlayer].pos                           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の位置
		g_aPlayer[nCntPlayer].oldPos                        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
		g_aPlayer[nCntPlayer].move                          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aPlayer[nCntPlayer].rot                           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aPlayer[nCntPlayer].nCameraBackCounter            = 0;								// カメラの回り込みカウンター
		g_aPlayer[nCntPlayer].nParentID                     = NONE_PLAYER_PARENT;				// プレイヤーの親のインデックス
		g_aPlayer[nCntPlayer].nShadowID                     = NONE_SHADOW;						// 影のインデックス
	}

	// プレイヤーのセットアップ
	TxtSetPlayer();

	// 影のインデックスを設定
	g_aPlayer[0].nShadowID = SetShadow(SHADOW_PLAYER, NULL);	// 親にのみ設定

	// xファイルの読み込み
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		D3DXLoadMeshFromX
		( // 引数
			apModelPlayer[nCntPlayer],			// モデル (パーツ) の相対パス
			D3DXMESH_SYSTEMMEM,
			pDevice,							// デバイスへのポインタ
			NULL,
			&g_aPlayer[nCntPlayer].pBuffMat,	// マテリアルへのポインタ
			NULL,
			&g_aPlayer[nCntPlayer].dwNumMat,	// マテリアルの数
			&g_aPlayer[nCntPlayer].pMesh		// メッシュ (頂点情報) へのポインタ
		);
	}

	// テクスチャの読み込み
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != NULL)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aPlayer[nCntPlayer].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================================================
//	プレイヤーの終了処理
//======================================================================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		for (int nCntMat = 0; nCntMat < TEXTURE_PLAYER_MAX; nCntMat++)
		{ // 使用するテクスチャ数分繰り返す

			if (g_aPlayer[nCntPlayer].apTexture[nCntMat] != NULL)
			{ // 変数 (g_aPlayer[nCntPlayer].apTexture) がNULLではない場合

				g_aPlayer[nCntPlayer].apTexture[nCntMat]->Release();
				g_aPlayer[nCntPlayer].apTexture[nCntMat] = NULL;
			}
		}
	}

	// メッシュの破棄
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		if (g_aPlayer[nCntPlayer].pMesh != NULL)
		{ // 変数 (g_aPlayer[nCntPlayer].pMesh) がNULLではない場合

			g_aPlayer[nCntPlayer].pMesh->Release();
			g_aPlayer[nCntPlayer].pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		if (g_aPlayer[nCntPlayer].pBuffMat != NULL)
		{ // 変数 (g_aPlayer[nCntPlayer].pBuffMat) がNULLではない場合

			g_aPlayer[nCntPlayer].pBuffMat->Release();
			g_aPlayer[nCntPlayer].pBuffMat = NULL;
		}
	}
}

//======================================================================================================================
//	プレイヤーの更新処理
//======================================================================================================================
void UpdatePlayer(void)
{
	// 変数を宣言
	float fDestRot = g_aPlayer[0].rot.y;	// プレイヤーの目標向き代入用
	float fDiffRot = 0.0f;					// プレイヤーの向き計算代入用

	// ポインタを宣言
	Camera *pCamera = GetCamera();			// カメラの情報

	//--------------------------------------------------------------------------------------------------------------
	//	前回位置の更新
	//--------------------------------------------------------------------------------------------------------------
	g_aPlayer[0].oldPos = g_aPlayer[0].pos;

	//------------------------------------------------------------------------------------------------------------------
	//	移動量の更新 (前後左右)
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_UP) == true)
	{ // [↑] が押された場合

		if (GetKeyboardPress(DIK_LEFT) == true)
		{ // [←] も押された場合

			// 位置を更新
			g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(135) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{ // [→] も押された場合

			// 位置を更新
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(225) + pCamera->rot.y;
		}
		else
		{ // [↑] だけが押された場合

			// 位置を更新
			g_aPlayer[0].move.x += sinf(pCamera->rot.y) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(180) + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{ // [↓] が押された場合

		if (GetKeyboardPress(DIK_LEFT) == true)
		{ // [←] も押された場合

			// 位置を更新
			g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(45) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{ // [→] も押された場合

			// 位置を更新
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(315) + pCamera->rot.y;
		}
		else
		{ // [↓] だけが押された場合

			// 位置を更新
			g_aPlayer[0].move.x -= sinf(pCamera->rot.y) * MOVE_PLAYER;
			g_aPlayer[0].move.z -= cosf(pCamera->rot.y) * MOVE_PLAYER;

			// 目標向きを更新
			fDestRot = D3DXToRadian(0) + pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{ // [←] が押された場合

		// 位置を更新
		g_aPlayer[0].move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		g_aPlayer[0].move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;

		// 目標向きを更新
		fDestRot = D3DXToRadian(90) + pCamera->rot.y;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{ // [→] が押された場合

		// 位置を更新
		g_aPlayer[0].move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		g_aPlayer[0].move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;

		// 目標向きを更新
		fDestRot = D3DXToRadian(270) + pCamera->rot.y;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	移動量の更新 (上下)
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_I) == true)
	{ // [I] が押された場合

		// 位置を更新
		g_aPlayer[0].move.y += MOVE_PLAYER;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{ // [K] が押された場合

		// 位置を更新
		g_aPlayer[0].move.y -= MOVE_PLAYER;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	位置の更新
	//------------------------------------------------------------------------------------------------------------------
	// 位置の更新
	g_aPlayer[0].pos += g_aPlayer[0].move;

	//------------------------------------------------------------------------------------------------------------------
	//	移動量の減衰・削除
	//------------------------------------------------------------------------------------------------------------------
	// 移動量の減衰
	g_aPlayer[0].move.x += (0.0f - g_aPlayer[0].move.x) * SUB_MOVE_PLAYER;
	g_aPlayer[0].move.y += (0.0f - g_aPlayer[0].move.y) * SUB_MOVE_PLAYER;
	g_aPlayer[0].move.z += (0.0f - g_aPlayer[0].move.z) * SUB_MOVE_PLAYER;

	// 移動量の削除
	if (g_aPlayer[0].move.x <  DELETE_MOVE
	&&  g_aPlayer[0].move.x > -DELETE_MOVE)
	{ // プレイヤーの移動量 (x) が削除する範囲内の場合

		// プレイヤーの移動量 (x) を削除する
		g_aPlayer[0].move.x = 0.0f;
	}

	if (g_aPlayer[0].move.y <  DELETE_MOVE
	&&  g_aPlayer[0].move.y > -DELETE_MOVE)
	{ // プレイヤーの移動量 (y) が削除する範囲内の場合

		// プレイヤーの移動量 (y) を削除する
		g_aPlayer[0].move.y = 0.0f;
	}

	if (g_aPlayer[0].move.z <  DELETE_MOVE
	&&  g_aPlayer[0].move.z > -DELETE_MOVE)
	{ // プレイヤーの移動量 (z) が削除する範囲内の場合

		// プレイヤーの移動量 (z) を削除する
		g_aPlayer[0].move.z = 0.0f;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	向きの更新
	//------------------------------------------------------------------------------------------------------------------
	// 目標向きの正規化
	if      (fDestRot >  D3DX_PI) { fDestRot -= D3DX_PI * 2; }
	else if (fDestRot < -D3DX_PI) { fDestRot += D3DX_PI * 2; }

	// 目標向きまでの差分を計算
	fDiffRot = fDestRot - g_aPlayer[0].rot.y;

	// 差分向きの正規化
	if      (fDiffRot >  D3DX_PI) { fDiffRot -= D3DX_PI * 2; }
	else if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2; }

	// 向きの更新
	g_aPlayer[0].rot.y += fDiffRot * REV_ROT_PLAYER;

	// 向きの正規化
	if      (g_aPlayer[0].rot.y >  D3DX_PI) { g_aPlayer[0].rot.y -= D3DX_PI * 2; }
	else if (g_aPlayer[0].rot.y < -D3DX_PI) { g_aPlayer[0].rot.y += D3DX_PI * 2; }

	//------------------------------------------------------------------------------------------------------------------
	//	カメラの回り込みカウンターの更新
	//------------------------------------------------------------------------------------------------------------------
	if (g_aPlayer[0].move == D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	&&  g_aPlayer[0].nCameraBackCounter  < CNT_ROT_FOLLOW)
	{ // 移動していない且つ、カウンターが一定値未満の場合

		// カウンターを加算
		g_aPlayer[0].nCameraBackCounter++;
	}
	else if (g_aPlayer[0].move != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{ // 移動している場合

		// カウンターを初期化
		g_aPlayer[0].nCameraBackCounter = 0;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	位置の補正
	//------------------------------------------------------------------------------------------------------------------
	if (g_aPlayer[0].pos.z > STAGE_LIMIT_NEAR)
	{ // 範囲外の場合 (手前)

		// 手前に位置を補正
		g_aPlayer[0].pos.z = STAGE_LIMIT_NEAR;
	}
	if (g_aPlayer[0].pos.z < STAGE_LIMIT_FAR)
	{ // 範囲外の場合 (奥)

		// 奥に位置を補正
		g_aPlayer[0].pos.z = STAGE_LIMIT_FAR;
	}
	if (g_aPlayer[0].pos.x > STAGE_LIMIT_RIGHT)
	{ // 範囲外の場合 (右)

		// 右に位置を補正
		g_aPlayer[0].pos.x = STAGE_LIMIT_RIGHT;
	}
	if (g_aPlayer[0].pos.x < STAGE_LIMIT_LEFT)
	{ // 範囲外の場合 (左)

		// 左に位置を補正
		g_aPlayer[0].pos.x = STAGE_LIMIT_LEFT;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	当たり判定
	//------------------------------------------------------------------------------------------------------------------
	// オブジェクトとの当たり判定
	CollisionObject(&g_aPlayer[0]);

	//------------------------------------------------------------------------------------------------------------------
	//	弾の更新
	//------------------------------------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_SPACE) == true)
	{ // [SPACE] が押された場合

		// 弾の設定
		SetBullet
		( // 引数
			D3DXVECTOR3
			( // 引数
				g_aPlayer[0].pos.x + sinf(g_aPlayer[0].rot.y + D3DX_PI) * POS_BULL_PLAYER_X,	// 位置 (x)
				g_aPlayer[0].pos.y + POS_BULL_PLAYER_Y,											// 位置 (y)
				g_aPlayer[0].pos.z + cosf(g_aPlayer[0].rot.y + D3DX_PI) * POS_BULL_PLAYER_Z		// 位置 (z)
			),
			D3DXVECTOR3
			( // 引数
				sinf(g_aPlayer[0].rot.y + D3DX_PI) * MOVE_BULLET,	// 移動量 (x)
				0.0f,												// 移動量 (y)
				cosf(g_aPlayer[0].rot.y + D3DX_PI) * MOVE_BULLET	// 移動量 (z)
			),
			BULLETTYPE_PLAYER	// 種類
		);
	}

	//------------------------------------------------------------------------------------------------------------------
	//	影の更新
	//------------------------------------------------------------------------------------------------------------------
	// 影の位置設定
	SetPositionShadow(g_aPlayer[0].nShadowID, g_aPlayer[0].pos);
}

//======================================================================================================================
//	プレイヤーの描画処理
//======================================================================================================================
void DrawPlayer(void)
{
	// 変数を宣言
	int          nCntParts = 0;					// パーツの親の確認用
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// 変数配列を宣言
	D3DXMATRIX mtxRot[MODEL_PLAYER_MAX];		// 向きの計算用マトリックス
	D3DXMATRIX mtxTrans[MODEL_PLAYER_MAX];		// 位置の計算用マトリックス

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	// 向き・位置の取得
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

		// 向きを取得
		D3DXMatrixRotationYawPitchRoll(&mtxRot[nCntPlayer], g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);

		// 位置を取得
		D3DXMatrixTranslation(&mtxTrans[nCntPlayer], g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
	}

	// 向き・位置の反映
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		// 向きを反映
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot[nCntPlayer]);

		// 位置を反映
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans[nCntPlayer]);

		// 現在のモデル (パーツ) のインデックスを代入
		nCntParts = nCntPlayer;

		while (g_aPlayer[nCntParts].nParentID != NONE_PLAYER_PARENT)
		{ // 現在のパーツに親が存在する場合

			// 向きを反映
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot[g_aPlayer[nCntParts].nParentID]);

			// 位置を反映
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans[g_aPlayer[nCntParts].nParentID]);

			// 現在のパーツの親のインデックスを代入
			nCntParts = g_aPlayer[nCntParts].nParentID;
		}
	}

	// マテリアル・テクスチャの設定、描画
	for (int nCntPlayer = 0; nCntPlayer < MODEL_PLAYER_MAX; nCntPlayer++)
	{ // プレイヤーのパーツの最大数分繰り返す

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aPlayer[nCntPlayer].apTexture[nCntMat]);

			// モデル (パーツ) の描画
			g_aPlayer[nCntPlayer].pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================================================
//	プレイヤーのセットアップ処理
//======================================================================================================================
void TxtSetPlayer(void)
{
	// 変数を宣言
	int nID  = 0;				// インデックスの代入用
	int nEnd = 0;				// テキスト読み込み終了の確認用

	// 変数配列を宣言
	char aString[MAX_STRING];	// テキストの文字列の代入用

	// ポインタを宣言
	FILE *pFile;				// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			//----------------------------------------------------------------------------------------------------------
			//	ブロックの設定
			//----------------------------------------------------------------------------------------------------------
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &nID);						// モデル (パーツ) のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%d", &g_aPlayer[nID].nParentID);	// モデル (パーツ) の親のインデックスを読み込む
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.x);		// X座標を読み込む
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.y);		// Y座標を読み込む
								fscanf(pFile, "%f", &g_aPlayer[nID].pos.z);		// Z座標を読み込む
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.x);		// X向きを読み込む
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.y);		// Y向きを読み込む
								fscanf(pFile, "%f", &g_aPlayer[nID].rot.z);		// Z向きを読み込む
							}

						} while (strcmp(&aString[0], "END_PARTSSET") != 0);		// 読み込んだ文字列が END_PARTSSET ではない場合ループ
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);			// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}
		} while (nEnd != EOF);													// 読み込んだ文字列が EOF ではない場合ループ
		
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(NULL, "ステージファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

//======================================================================================================================
//	プレイヤーの取得処理
//======================================================================================================================
Player *GetPlayer(void)
{
	// プレイヤーの親パーツの情報アドレスを返す
	return &g_aPlayer[0];
}

#ifdef _DEBUG	// デバッグ処理
//======================================================================================================================
//	デバッグ処理一覧
//======================================================================================================================
//**********************************************************************************************************************
//	プレイヤーの座標の取得処理
//**********************************************************************************************************************
D3DXVECTOR3 GetPlayerPos(void)
{
	// 座標を返す
	return g_aPlayer[0].pos;
}

//**********************************************************************************************************************
//	プレイヤーの向きの取得処理
//**********************************************************************************************************************
D3DXVECTOR3 GetPlayerRot(void)
{
	// 座標を返す
	return g_aPlayer[0].rot;
}

//**********************************************************************************************************************
//	プレイヤーの再設定処理
//**********************************************************************************************************************
void ResetPlayer(void)
{
	// グローバル変数を再設定
	g_aPlayer[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	g_aPlayer[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
}
#endif
//======================================================================================================================
//
//	オブジェクトヘッダー [object.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _OBJECT_H_						// このマクロ定義がされていない場合
#define _OBJECT_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "player.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_OBJECT			(128)		// 使用するモデル数 (オブジェクトの最大数)
#define TEXTURE_OBJECT_MAX	(10)		// テクスチャの最大数

//**********************************************************************************************************************
//	構造体定義 (ModelObject)
//**********************************************************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[TEXTURE_OBJECT_MAX];			// テクスチャへのポインタ
	LPD3DXMESH         pMesh;									// メッシュ (頂点情報) へのポインタ
	LPD3DXBUFFER       pBuffMat;								// マテリアルへのポインタ
	DWORD              dwNumMat;								// マテリアルの数
	D3DXVECTOR3        vtxMin;									// 最小の頂点座標
	D3DXVECTOR3        vtxMax;									// 最大の頂点座標
}ModelObject;

//**********************************************************************************************************************
//	構造体定義 (Object)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX  mtxWorld;				// ワールドマトリックス
	int         nType;					// 種類
	int         nShadowID;				// 影のインデックス
	bool        bUse;					// 使用状況
}Object;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitObject(void);											// オブジェクトの初期化処理
void UninitObject(void);										// オブジェクトの終了処理
void UpdateObject(void);										// オブジェクトの更新処理
void DrawObject(void);											// オブジェクトの描画処理
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);	// オブジェクトの設定処理
void CollisionObject(Player *pPlayer);							// オブジェクトとの当たり判定
Object *GetObjectData(void);									// オブジェクトの取得処理
ModelObject *GetModelObject(void);								// オブジェクトのモデル情報の取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumObject(void);					// オブジェクトの総数取得処理

#endif
//======================================================================================================================
//
//	弾ヘッダー [bullet.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _BULLET_H_			// このマクロ定義がされていない場合
#define _BULLET_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	列挙型定義 (BULLETTYPE)
//**********************************************************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,	// プレイヤーの弾
	BULLETTYPE_MAX			// この列挙型の総数
} BULLETTYPE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitBullet(void);		// 弾の初期化処理
void UninitBullet(void);	// 弾の終了処理
void UpdateBullet(void);	// 弾の更新処理
void DrawBullet(void);		// 弾の描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BULLETTYPE type);	// 弾の設定処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumBullet(void);		// 弾の総数取得処理

#endif
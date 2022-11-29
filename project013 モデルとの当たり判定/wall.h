//======================================================================================================================
//
//	壁ヘッダー [wall.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _WALL_H_			// このマクロ定義がされていない場合
#define _WALL_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitWall(void);		// 壁の初期化処理
void UninitWall(void);		// 壁の終了処理
void UpdateWall(void);		// 壁の更新処理
void DrawWall(void);		// 壁の描画処理
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 壁の設定処理

#endif
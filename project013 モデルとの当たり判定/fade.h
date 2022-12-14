//======================================================================================================================
//
//	フェードヘッダー [fade.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _FADE_H_				// このマクロ定義がされていない場合
#define _FADE_H_				// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	列挙型定義 (FADE)
//**********************************************************************************************************************
typedef enum
{
	FADE_NONE = 0,				// 何もしていない状態
	FADE_IN,					// フェードイン状態
	FADE_OUT,					// フェードアウト状態
	FADE_MAX					// この列挙型の総数
} FADE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitFade(MODE modeNext);	// フェードの初期化処理
void UninitFade(void);			// フェードの終了処理
void UpdateFade(void);			// フェードの更新処理
void DrawFade(void);			// フェードの描画処理
void SetFade(MODE modeNext);	// フェードの設定処理
FADE GetFade(void);				// フェードの取得処理

#endif
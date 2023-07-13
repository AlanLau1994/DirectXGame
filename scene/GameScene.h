#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void PlayerUpdate();
	void EnemyUpdate();
	void BeamUpdate();
	void EnemyMove();
	void EnemyBorn();
	void BeamMove();
	void BeamBorn();

	// crash
	void Collision();
	void CollisionPlayerEnemy();
	void CollisionBeamEnemy();

	// Arrangement
	void GamePlayUpdate();
	void GamePlayDraw3D();
	void GamePlayDraw2DBack();
	void GamePlayDraw2DNear();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ビュープロじぇくしょん（共通）
	ViewProjection viewProjection_;

	/// BG（スプライト）
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// STAGE
	uint32_t textureHandleStage_ = 0;
	Model* modelstage_ = nullptr;
	WorldTransform worldTransformStage_;

	// PLAY
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	int playerlife_ = 1;

	// BEAM
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	int beamflag_ = 0;

	//// ENEMY
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10] = {};
	int enemyflag_[10] = {};
	float enemyspeed_= 0.1f;

	// Multiply

	// Debug
	DebugText* debugText_ = nullptr;

	// Score
	int gamescore_ = 0;

	// scenechange
	int sceneMode_ = 1;

	// title
	void TitleUpdate();
	void TitleDraw2DNear();
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;
	int gameTimer_ = 0;

	// GameOver
	void GameOverUpdate();
	void GameOverDraw2DNear();
	void GameOver3D();
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;
	uint32_t textureHandleGOEnter_ = 0;
	Sprite* spriteGOEnter_ = nullptr;

	void GamePlayStart();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

#pragma once

#include "Audio.h"
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
	void BeamUpdate();
	void BeamMove();
	void BeamBorn();

	/// <summary>
	/// 描画
	/// </summary>
	
	
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	//ビュープロじぇくしょん（共通）
	ViewProjection viewProjection_;

	/// BG（スプライト）
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	//STAGE
	uint32_t textureHandleStage_ = 0;
	Model* modelstage_ = nullptr;
	WorldTransform worldTransformStage_;

	//PLAY
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//BEAM
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	int beamflag_ = 0;

	//// ENEMY
	//uint32_t textureHandleBeam_ = 0;
	//Model* modelBeam_ = nullptr;
	//WorldTransform worldTransformBeam_;
	//int beamflag_ = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
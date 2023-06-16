#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete spriteBG_;
	delete modelstage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
}

#pragma region // 基本情報
void GameScene::PlayerUpdate() {
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	worldTransformPlayer_.TransferMatrix();

	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	if (worldTransformPlayer_.translation_.x > 4) {
		worldTransformPlayer_.translation_.x = 4;
	}
	if (worldTransformPlayer_.translation_.x < -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
}

void GameScene::BeamUpdate() {
	BeamBorn();
	BeamMove();
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	worldTransformBeam_.TransferMatrix();
}

void GameScene::BeamMove() {
	if (beamflag_ == 1) {
		worldTransformBeam_.rotation_.x += 0.1f;
		worldTransformBeam_.translation_.z += 0.1f;
	}
}

void GameScene::BeamBorn() {
	if (input_->PushKey(DIK_SPACE) /*&&()*/) {
		beamflag_ = 1;
		worldTransformBeam_.translation_ = worldTransformPlayer_.translation_;
	}
	if (worldTransformBeam_.translation_.z > 40) {
		beamflag_ = 0;
	}
}

void GameScene::EnemyUpdate() {
	EnemyBorn();
	EnemyMove();
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);
	worldTransformEnemy_.TransferMatrix();
};
void GameScene::EnemyMove() {

	worldTransformEnemy_.translation_.z -= enemyspeed_;

	worldTransformEnemy_.rotation_.x -= 0.1f;
	if (worldTransformEnemy_.translation_.z < -5) {
		enemyflag_ = 0;
	}
	// enemy移動
	/*worldTransformEnemy_.translation_.x += enemyspeed_;
	if (worldTransformEnemy_.translation_.x > 4) {
	    enemyspeed_ *= -1;
	}
	if (worldTransformEnemy_.translation_.x < -4) {
	    enemyspeed_ *= -1.0f;
	}*/
}

void GameScene::EnemyBorn() {

	if (enemyflag_ == 1) {
	
		reborntime_ = 50;
	}

	if (enemyflag_ == 0) {
		reborntime_--;
		worldTransformEnemy_.translation_.z = 40;
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
	}
	if (reborntime_ == 0) {
		enemyflag_ = 1;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	srand((unsigned int)time(NULL));
	// ビュープロじぇくしょん（共通）
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	// BG
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// STAGE
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelstage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);
	worldTransformStage_.TransferMatrix();

	// PLAYER
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// Beam
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();

	// Enemy
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.translation_.z = 40;
	worldTransformEnemy_.Initialize();

	// debug
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

// Crash
void GameScene::CollisionPlayerEnemy() {
	if (enemyflag_ == 1) {
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);

		if (dx < 1 && dz < 1) {
			enemyflag_ = 0;
			playerlife_ -= 1;
		}
	}
}
void GameScene::CollisionBeamEnemy() {
	if (enemyflag_ == 1) {
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

		if (dx < 1 && dz < 1) {
			enemyflag_ = 0;
			gamescore_ += 1;
		}
	}
}

void GameScene::Collision() {
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}
void GameScene::GamePlayDraw3D() {
	modelstage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	if (enemyflag_ == 1) {

		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
	if (beamflag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
}
void GameScene::GamePlayDraw2DBack() { spriteBG_->Draw(); }
void GameScene::GamePlayDraw2DNear() {
	debugText_->Print("AAA", 10, 10, 2);

	char str[100];
	sprintf_s(str, "SCORE %d", gamescore_);
	debugText_->Print(str, 200.f, 10.f, 2.f);

	sprintf_s(str, "LIFE %d", playerlife_);
	debugText_->Print(str, 900.f, 10.f, 2.f);
};
void GameScene::GamePlayUpdate() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
}

#pragma endregion
void GameScene::Update() {
	switch (sceneMode_) {
	case 0:
		GamePlayUpdate(); 
	break;
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	///
	switch (sceneMode_) {
	case 0:
	GamePlayDraw2DBack();
	break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) {
	case 0:
	GamePlayDraw3D();
	break;
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	debugText_->DrawAll();
	
	switch (sceneMode_) {
	case 0:
	GamePlayDraw2DNear();
	break;
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
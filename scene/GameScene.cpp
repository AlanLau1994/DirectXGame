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
	delete spriteTitle_;
	delete spriteGameOver_;
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
#pragma region // beam
void GameScene::BeamUpdate() {
	BeamBorn();
	BeamMove();
	for (int i = 0; i < 10; i++) {
		worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		    worldTransformBeam_[i].translation_);

		worldTransformBeam_[i].TransferMatrix();
	}
}

void GameScene::BeamMove() {
	for (int i = 0; i < 10; i++) {
		if (beamflag_[i] == 1) {
			worldTransformBeam_[i].rotation_.x += 0.1f;
			worldTransformBeam_[i].translation_.z += 1.0f;
		}
	}
}

void GameScene::BeamBorn() {

	if (beamtimer_ == 0) {
		for (int i = 0; i < 10; i++) {
			if (input_->TriggerKey(DIK_SPACE) && beamflag_[i] == 0) {
				beamflag_[i] = 1;
				worldTransformBeam_[i].translation_ = worldTransformPlayer_.translation_;
				beamtimer_ = 1;
				break;
			}
			if (worldTransformBeam_[i].translation_.z > 40) {
				beamflag_[i] = 0;
			}
		}
	} else {
		beamtimer_++;
		if (beamtimer_ > 10) {
			beamtimer_ = 0;
		}
	}
}
#pragma endregion

#pragma region // enemy
void GameScene::EnemyUpdate() {
	EnemyBorn();
	EnemyJump();
	EnemyMove();
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);
		worldTransformEnemy_[i].TransferMatrix();
	}
};
void GameScene::EnemyMove() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag_[i] == 1) {
			worldTransformEnemy_[i].translation_.z -= 0.3f;
			worldTransformEnemy_[i].translation_.z -= gameTimer_ / 1000.0f;
			worldTransformEnemy_[i].translation_.x += enemyspeed_[i];
			if (worldTransformEnemy_[i].translation_.x > 4) {
				worldTransformEnemy_[i].translation_.x *= -1;
			} else if (worldTransformEnemy_[i].translation_.x < -4) {
				worldTransformEnemy_[i].translation_.x *= -1;
			}

			worldTransformEnemy_[i].rotation_.x -= 0.1f;
			
		}

		if (worldTransformEnemy_[i].translation_.z < -5) {

			enemyflag_[i] = 0;
		}
	}
}
void GameScene::EnemyBorn() {

	if (rand() % 10 == 0) {
		for (int e = 0; e < 10; e++) {
			if (enemyflag_[e] == 0) {
				worldTransformEnemy_[e].translation_.z = 40;
				int x = rand() % 80;
				float x2 = (float)x / 10 - 4;
				worldTransformEnemy_[e].translation_.x = x2;
				enemyflag_[e] = 1;
				if (rand() % 2 == 0) {
					enemyspeed_[e] = 0.1f;

				} else {
					enemyspeed_[e] = -0.1f;
				}
				break;
			}
		}
	}
}
void GameScene::EnemyJump() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag_[i] == 2) {
			worldTransformEnemy_[i].translation_.y += enemyJumpSpeed_[i] * 2;
			enemyJumpSpeed_[i] -= 0.1f;
			worldTransformEnemy_[i].translation_.x += enemyspeed_[i] * 4;
		}
		if (worldTransformEnemy_[i].translation_.y < -3) {
			enemyflag_[i] = 0;
			worldTransformEnemy_[i].translation_.y = 0;
		}
	}
}
#pragma endregion

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
	textureHandleStage_ = TextureManager::Load("stage2.jpg");
	modelstage_ = Model::Create();
	for (int i = 0; i < 20; i++) {
		worldTransformStage_[i].Initialize();
	}
	for (int i = 0; i < 20; i++) {
		worldTransformStage_[i].translation_ = {0, -1.5f, 2.0f * i - 5};
		worldTransformStage_[i].scale_ = {4.5f, 1, 1};
		worldTransformStage_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformStage_[i].scale_, worldTransformStage_[i].rotation_,
		    worldTransformStage_[i].translation_);
		worldTransformStage_[i].TransferMatrix();
	}


	// PLAYER
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// Beam
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	for (int e = 0; e < 10; e++) {
		worldTransformBeam_[e].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformBeam_[e].Initialize();
	}

	// Enemy
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int i = 0; i < 10; i++) {

		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].translation_.z = 40;
		worldTransformEnemy_[i].Initialize();
		enemyspeed_[i] = 0.1f;
		enemyJumpSpeed_[i] = 0.1f;
	}

	// debug
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	// title
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});

	// GameOver
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});

	textureHandleGOEnter_ = TextureManager::Load("enter.png");
	spriteGOEnter_ = Sprite::Create(textureHandleGOEnter_, {400, 400});

	// sound
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitBGM_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitBGM_ = audio_->LoadWave("Audio/tada.wav");
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
}

// Crash
void GameScene::CollisionPlayerEnemy() {
	for (int i = 0; i < 10; i++) {

		if (enemyflag_[i] == 1 && playerlife_ > 0) {
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);
			if (dx < 1 && dz < 1) {
				enemyflag_[i] = 0;
				playerlife_ -= 1;
				audio_->PlayWave(soundDataHandlePlayerHitBGM_);
			}
		}
	}
}
void GameScene::CollisionBeamEnemy() {
	for (int i = 0; i < 10; i++) {
		if (enemyflag_[i] == 1) {
			for (int e = 0; e < 10; e++) {
				if (beamflag_[e] == 1) {
					float dx =
					    abs(worldTransformBeam_[e].translation_.x -
					        worldTransformEnemy_[i].translation_.x);
					float dz =
					    abs(worldTransformBeam_[e].translation_.z -
					        worldTransformEnemy_[i].translation_.z);

					if (dx < 1 && dz < 1) {
						enemyflag_[i] = 2;
						beamflag_[e] = 0;
						audio_->PlayWave(soundDataHandleEnemyHitBGM_);
						gamescore_ += 1;
						enemyJumpSpeed_[i] = 1;
					}
				}
			}
		}
	}
}
void GameScene::Collision() {
	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

// scroll
void GameScene::StageUpdate() {
	for (int i = 0; i < 20; i++) {
		worldTransformStage_[i].translation_.z -= 0.1f;
		if (worldTransformStage_[i].translation_.z < -5) {
			worldTransformStage_[i].translation_.z += 40;
		}

		worldTransformStage_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformStage_[i].scale_, worldTransformStage_[i].rotation_,
		    worldTransformStage_[i].translation_);
		worldTransformStage_[i].TransferMatrix();
	}
}

void GameScene::GamePlayDraw3D() {
	for (int i = 0; i < 20; i++) {
		modelstage_->Draw(worldTransformStage_[i], viewProjection_, textureHandleStage_);
	}

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	for (int i = 0; i < 10; i++) {
		if (enemyflag_[i] != 0) {

			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		}
	}

	for (int e = 0; e < 10; e++) {
		if (beamflag_[e] == 1) {
			modelBeam_->Draw(worldTransformBeam_[e], viewProjection_, textureHandleBeam_);
		}
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

// SCENE CHANGE
void GameScene::TitleUpdate() {

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 0;
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);
		GamePlayStart();
	}
}
void GameScene::TitleDraw2DNear() {
	spriteTitle_->Draw();
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

void GameScene::GameOverUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 1;
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
	}
}
void GameScene::GameOverDraw2DNear() {

	spriteGameOver_->Draw();

	if (gameTimer_ % 40 >= 20) {
		spriteGOEnter_->Draw();
	}
}
void GameScene::GameOver3D() {
	for (int i = 0; i < 20; i++) {
		modelstage_->Draw(worldTransformStage_[i], viewProjection_, textureHandleStage_);
	}

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}

void GameScene::GamePlayStart() {
	playerlife_ = 3;
	gamescore_ = 0;
	gameTimer_ = 0;
	for (int i = 0; i < 10; i++) {
		enemyflag_[i] = 0;
	}
	worldTransformPlayer_.translation_.x = 0;
}

void GameScene::GamePlayUpdate() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
	StageUpdate();
	if (playerlife_ == 0) {
		sceneMode_ = 2;
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGameOverBGM_, true);
	}
}

#pragma endregion
void GameScene::Update() {
	gameTimer_ += 1;
	switch (sceneMode_) {
	case 0:
		GamePlayUpdate();

		break;
	case 1:
		TitleUpdate();

		break;
	case 2:
		GameOverUpdate();

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
	case 1:

		break;
	case 2:
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
	case 1:

		break;
	case 2:
		GameOver3D();
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
	case 1:
		TitleDraw2DNear();
		break;
	case 2:
		GameOverDraw2DNear();
		break;
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
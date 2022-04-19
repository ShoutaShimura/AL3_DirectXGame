﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, { 100,50 });
	model_ = Model::Create();


	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		//スケーリングを設定
		worldTransform_[i].scale_ = { 10.0f,10.0f,10.0f };

		////X,Y,Z軸周りの回転角を設定
		//worldTransform_[i].rotation_ = { 0,0,0 };

		//オブジェクトを置き始めるX座標を100と仮定
		int viewMax = 100;


		if (i < 50) {
			//X,Y,Z軸周りの平行移動を設定
			worldTransform_[i].translation_ = { (float)viewMax - (i * 20.0f),40.0f,50.0f };
		}
		else if (i >= 50) {
			worldTransform_[i].translation_ = { (float)viewMax - ((i - 50.0f) * 20.0f),-40.0f,50.0f };
		}

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}

	viewProjection_.Initialize();


	////サウンドデータの読み込み
	//soundDataHandle_ = audio_->LoadWave("se_sad03.wav");

	////音声再生
	//voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {





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

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();



#pragma endregion
}

#include "GameScene.h"
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
		worldTransform_[i].scale_ = { 1.0f,1.0f,1.0f };

		////X,Y,Z軸周りの回転角を設定
		//worldTransform_[i].rotation_ = { 0,0,0 };

		//半径
		int h = 10;

		angle[i] = 36 * i;

		x = h * cos((angle[i]) * 3.1415f / 180) - sin((angle [i]) * 3.1415f / 180);

		y = h * sin((angle[i]) * 3.1415f / 180) + cos((angle[i]) * 3.1415f / 180);

		//X,Y,Z軸周りの平行移動を設定
		worldTransform_[i].translation_ = { x,y ,0 };



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

	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		angle[i]++;
		int h = 10;

		worldTransform_[i].translation_.x = h * cos((angle[i]) * 3.1415f / 180) - sin((angle[i]) * 3.1415f / 180);
		worldTransform_[i].translation_.y = h * sin((angle[i]) * 3.1415f / 180) + cos((angle[i]) * 3.1415f / 180);

		worldTransform_[i].UpdateMatrix();

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

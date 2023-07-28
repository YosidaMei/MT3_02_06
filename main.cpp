#include <Novice.h>
#include "Function.h"
#include <imgui.h>

const char kWindowTitle[] = "LD2A_11_ヨシダメイ";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate = {};
	Vector3 translate = { 0,0,0 };
	Vector3 cameraPosition = { 0,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0,0 };

	AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.0f,0.0f,0.0f},
	};

	Sphere sphere{
		{0.5f,0.5f,0.5f},
		0.7f
	};
	uint32_t color = WHITE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		//カメラの移動
		if (keys[DIK_UP]) {
			cameraPosition.y += 0.02f;
		}
		if (keys[DIK_DOWN]) {
			cameraPosition.y -= 0.02f;
		}
		if (keys[DIK_LEFT]) {
			cameraPosition.x -= 0.02f;
		}
		if (keys[DIK_RIGHT]) {
			cameraPosition.x += 0.02f;
		}
		if (keys[DIK_A]) {
			cameraPosition.z -= 0.02f;
		}
		if (keys[DIK_D]) {
			cameraPosition.z += 0.02f;
		}
		if (keys[DIK_Q]) {
			cameraRotate.y += 0.02f;
		}
		if (keys[DIK_E]) {
			cameraRotate.y -= 0.02f;
		}

		//球とaabbの当たり判定
		if (isCollisionAB2S(aabb1,sphere)) {
			color = RED;
		}
		else {
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, WHITE);
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, color);
		//デバッグ
		ImGui::Begin("window");
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);
		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

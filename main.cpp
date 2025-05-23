#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LC1D_02_イセダ_コテツ_タイトル";
struct Vector3
{
	float x;
	float y;
	float z;

};
struct Matrix4x4
{
	float m[4][4];
};
//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float asprectRatio, float nearClip, float farclip)
{
	float cot = 1.0f / std::tanf(fovY / 2.0f);
	Matrix4x4 result = {};
	result.m[0][0] = cot / asprectRatio;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = cot;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farclip / (farclip - nearClip);
	result.m[2][3] = 1;
	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = (-nearClip * farclip) / (farclip - nearClip);
	result.m[3][3] = 0;
	return result;
}
//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result = {};
	result.m[0][0] =2 / (right-left);
	result.m[0][1] =0;
	result.m[0][2] =0;
	result.m[0][3] =0;
	result.m[1][0] =0;
	result.m[1][1] = 2 / (top-bottom);
	result.m[1][2] =0;
	result.m[1][3] =0;
	result.m[2][0] =0;
	result.m[2][1] =0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] =0;
	result.m[3][0] =(left + right) / (left - right);
	result.m[3][1] =(top+ bottom) / (bottom - top);
	result.m[3][2] =nearClip / (nearClip - farClip);
	result.m[3][3] =1;
	return result;
}
//ビューポート変換行列
Matrix4x4 MakeViewporMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result = {};
	result.m[0][0] = width / 2;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = -height / 2;
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;
	return result;
}
static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column], label);
		}
	}
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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
		Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);
		Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
		Matrix4x4 vieportMatrix = MakeViewporMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		MatrixScreenPrintf(0, 0, orthographicMatrix, "orthographicMatrix");
		Novice::ScreenPrintf(0, kRowHeight + 60, "orthographicMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix, "perspectiveFovMatrix");
		Novice::ScreenPrintf(0, kRowHeight + 160, "perspectiveFovMatrix");
		MatrixScreenPrintf(0, kRowHeight * 10, vieportMatrix, "vieportMatrix");
		Novice::ScreenPrintf(0, kRowHeight + 255, "vieportMatrix");


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

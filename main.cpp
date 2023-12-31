#include "hit.h"
#include <Novice.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const char kWindowTitle[] = "1143_えびbeats";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//Novice::SetWindowMode(kFullscreen);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	//キャラクター
	struct Vector2 {
		float x;
		float y;
		float w;
		float h;
	};
	//Ball
	struct Ball {
		Vector2 position;
		float radius;
		float speed;
	};
	//敵
	struct Enemy {
		Vector2 position;
		float radius;
		float speed;
	};

	//敵用変数
	struct EasyEnemy {
		Vector2 position[200];
		float radius[200];
		float speed[200];
		bool isEenemyAlive[200];
	};

	//初期値設定
	Ball ball = {
		{100.0f, 250.0f, 64.0f, 64.0f},
		32.0f,
		180.0f
	};
	Enemy enemy = {
		{1000.0f, 270.0f, 64.0f, 64.0f},
		32.0f,
		3.0f
	};

	// 敵変数の初期化
	EasyEnemy Eenemy = { 0 };
	srand((unsigned int)time(NULL));
	int num[200] = { 0 }; // 乱数用変数
	for (int j = 0; j < 200; j++) {
		for (int i = 0; i < 200; i++) {
			Eenemy.position[i].x = { 1000.0f + i * 600.0f };
			Eenemy.radius[i] = { 64.0f };
			Eenemy.speed[i] = { 3.0f };
			Eenemy.isEenemyAlive[i] = true;

			// Y座標用乱数
			num[i] = rand() % 3;
			if (num[i] == 0) {
				Eenemy.position[i].y = { 270.0f };
			}
			else if (num[i] == 1) {
				Eenemy.position[i].y = { 90.0f };
			}
			else if (num[i] == 2) {
				Eenemy.position[i].y = { 450.0f };
			}
		}
	}

	//リスポーンタイマー
	int resporntimer = 50;

	int score = 0;

	//スコア
	int kazu = 12345;
	int waru_kazu = 10000;
	int x = 550, y = 450;
	int keta = 0;

	//カーソル移動
	int MoveC = 0;

	// スプライトの縦幅
	float playerw = 64.0f;
	float enemyw = 64.0f;

	// スプライト横幅
	float playerh = 64.0f;
	float enemyh = 64.0f;

	//モーション
	int frameCount = 0;
	int playerAnimCount = 0;

	//カウントキャラクター
	float pos_x = 1200.0f, pos_y = 580.0f;

	//長押しカウント
	int count = 0;

	//曲の時間
	int time_count = 0;
	int time_AnimCount = 0;

	//背景画像
	int bg1X = 0, bg2X = 1280;
	int bg3X = 0, bg4X = 1280;
	int bg5X = 0, bg6X = 1280;
	int bg_speed = 4;

	//アニメーションカウント
	int AnimframeCount = 0;
	int AnimframeCount1 = 0;
	int AnimframeCount2 = 0;
	int AnimCount = 0;

	//フラグ管理
	bool Upflag = true;  //アップフラグ
	bool Downflag = false;  //ダウンフラグ
	bool Startflag = true;  //スタートフラグ
	bool Gameflag = true;  //ゲームスタート用フラグ
	bool Tutorialflag = false;  //チュートリアルフラグ
	bool TutorialGameflag = true;  //チュートリアル用ゲームスタートフラグ
	bool Moveflag = false;  //チュートリアル用動かすフラグ
	bool Stage_Easyflag = false;  //難易度EASYフラグ
	bool Stage_Easy_Clearflag = false;  //難易度EASYクリアフラグ
	bool Stage_Normalflag = false;  //難易度NORMALフラグ
	bool Stage_Normal_Clearflag = false;  //難易度Normalクリアフラグ
	bool Stage_Hardflag = false;  //難易度HARDフラグ
	bool Stage_Hard_Clearflag = false;  //難易度Hardクリアフラグ
	bool Switchingflag = false;  //画面切り替えフラグ
	bool Push1flag = false;  //押した1判定用
	bool Push2flag = false;  //押した2判定用
	bool onceflag = false;  //押されたか用フラグ
	bool L_push = false;  //長押しフラグ
	bool S_push = false;  //短押しフラグ 
	bool isEnemyAlive = true;  //敵生存フラグ
	bool isHittrue = false;  //確認用変数
	bool Clearflag = false;  //クリアフラグ
	bool isddd = false;

	//画像切り替え
	enum Scene {
		eScene_START,
		eScene_TUTORIAL,
		eScene_SELECT,
		eScene_STAGE_EASY,
		eScene_STAGE_NORMAL,
		eScene_STAGE_HARD,
		eScene_CLEAR,
	};
	Scene scene_no = eScene_START;

	//BGM・SE
	int GameStart = Novice::LoadAudio("./Resources/BGM/GameStart.mp3");
	int BGM1 = Novice::LoadAudio("./Resources/BGM/BGM1.mp3");  
	int BGM2 = Novice::LoadAudio("./Resources/BGM/BGM2.mp3");
	int BGM3 = Novice::LoadAudio("./Resources/BGM/BGM3.mp3");
	int ResultBGM = Novice::LoadAudio("./Resources/BGM/Result.mp3");
	int Kill = Novice::LoadAudio("./Resources/SE/Kill.mp3");
	int Success = Novice::LoadAudio("./Resources/SE/Success.mp3");
	int voiceHandle1 = -1;
	int voiceHandle2 = -1;
	int voiceHandle3 = -1;
	int voiceHandle4 = -1;
	int voiceHandle5 = -1;
	int voiceHandle6 = -1;
	int voiceHandle7 = -1;

	//画像読み込み
	int Start[6] = {
		Novice::LoadTexture("./Resources/Start1.png"),
		Novice::LoadTexture("./Resources/Start2.png"),
		Novice::LoadTexture("./Resources/Start3.png"),
		Novice::LoadTexture("./Resources/Start4.png"),
		Novice::LoadTexture("./Resources/Start5.png"),
		Novice::LoadTexture("./Resources/Start6.png"),
	};
	int Select[3] = {
		Novice::LoadTexture("./Resources/Select1.png"),
		Novice::LoadTexture("./Resources/Select2.png"),
		Novice::LoadTexture("./Resources/Select3.png")
	};
	int Arrow = Novice::LoadTexture("./Resources/arrow.png");
	int Ebi = Novice::LoadTexture("./Resources/ebi.png");
	int Ika = Novice::LoadTexture("./Resources/ika.png");
	int Same = Novice::LoadTexture("./Resources/same.png");
	int Maguro = Novice::LoadTexture("./Resources/maguro.png");
	int Haikei[3] = {
		Novice::LoadTexture("./Resources/Haikei1.png"),
		Novice::LoadTexture("./Resources/Haikei2.png"),
		Novice::LoadTexture("./Resources/Haikei3.png")
	};
	int Result = Novice::LoadTexture("./Resources/Result.png");
	int graph[10] = {
	Novice::LoadTexture("./Resources/0.png"),
	Novice::LoadTexture("./Resources/1.png"),
	Novice::LoadTexture("./Resources/2.png"),
	Novice::LoadTexture("./Resources/3.png"),
	Novice::LoadTexture("./Resources/4.png"),
	Novice::LoadTexture("./Resources/5.png"),
	Novice::LoadTexture("./Resources/6.png"),
	Novice::LoadTexture("./Resources/7.png"),
	Novice::LoadTexture("./Resources/8.png"),
	Novice::LoadTexture("./Resources/9.png"),
	};
	int HukidasiUI = Novice::LoadTexture("./Resources/UI/Hukidasi.png");
	int TutorialUI[10] = {
		Novice::LoadTexture("./Resources/UI/tutorial1.png"),
		Novice::LoadTexture("./Resources/UI/tutorial2.png"),
		Novice::LoadTexture("./Resources/UI/tutorial3.png"),
		Novice::LoadTexture("./Resources/UI/tutorial4.png"),
		Novice::LoadTexture("./Resources/UI/tutorial5.png"),
		Novice::LoadTexture("./Resources/UI/tutorial6.png"),
		Novice::LoadTexture("./Resources/UI/tutorial7.png"),
		Novice::LoadTexture("./Resources/UI/tutorial8.png"),
		Novice::LoadTexture("./Resources/UI/tutorial9.png"),
		Novice::LoadTexture("./Resources/UI/tutorial10.png")
	};
	int ReadyUI = Novice::LoadTexture("./Resources/UI/Ready.png");
	int StartUI = Novice::LoadTexture("./Resources/UI/Start.png");
	int CountUI = Novice::LoadTexture("./Resources/UI/Count.png");
	int ClearUI = Novice::LoadTexture("./Resources/UI/Clear.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// ↓更新処理ここから

		switch (scene_no) {
		case eScene_START:
			//シーンの切り替え

			if (!Novice::IsPlayingAudio(voiceHandle1) || voiceHandle1 == -1) {
				voiceHandle1 = Novice::PlayAudio(GameStart, true, 0.5f);
			}

			//カーソル移動

			if (keys[DIK_W] && !preKeys[DIK_W] || keys[DIK_UP] && !preKeys[DIK_UP]) {  //↑押したとき
				Upflag = true;
				Downflag = false;
			}

			if (keys[DIK_S] && !preKeys[DIK_S] || keys[DIK_DOWN] && !preKeys[DIK_DOWN]) {  //↓押したとき
				Upflag = false;
				Downflag = true;
			}

			if (keys[DIK_RETURN] && !preKeys[DIK_RETURN]) {  //ENTER押したとき
				if (Upflag) {
					Novice::StopAudio(voiceHandle1);
					scene_no = eScene_TUTORIAL;
					Startflag = false;
					frameCount = 0;
					playerAnimCount = 0;
				}
				if (Downflag) {
					Novice::Finalize();
					return 0;
				}
			}

			//ゲームスタート画面

			if (Startflag) {
				frameCount++;
				if (frameCount == 15) {
					playerAnimCount += 1;
				}
				if (frameCount == 30) {
					playerAnimCount += 1;
				}
				if (frameCount == 45) {
					playerAnimCount += 1;
				}
				if (frameCount == 60) {
					playerAnimCount += 1;
				}
				if (frameCount == 75) {
					playerAnimCount += 1;
				}
			}

			break;
		case eScene_TUTORIAL:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (TutorialGameflag) {
				AnimframeCount++;
				if (AnimframeCount == 60) {
					AnimCount += 1;
				}
				if (AnimframeCount == 120) {
					Tutorialflag = true;
				}
				if (AnimframeCount == 380) {
					Tutorialflag = false;
					AnimCount += 1;
				}
				if (AnimframeCount == 440) {
					Tutorialflag = true;
				}
				if (score == 300) {
					Tutorialflag = false;
					AnimCount += 1;
					enemy.speed = 0;
					score = 400;
				}
				if (score == 400) {
					AnimframeCount1++;
				}
				if (AnimframeCount1 == 60) {
					AnimCount += 1;
				}
				if (AnimframeCount1 == 120) {
					Tutorialflag = true;
					Moveflag = true;
					AnimCount += 1;
				}
				if (AnimframeCount1 == 420) {
					Tutorialflag = false;
					AnimCount += 1;
				}
				if (AnimframeCount1 == 480) {
					AnimCount += 1;
				}
				if (AnimframeCount1 == 600) {
					Tutorialflag = true;
					Switchingflag = true;
					AnimCount += 1;
				}
				if (Push1flag) {
					AnimframeCount2++;
				}
				if (AnimframeCount2 == 30) {
					AnimCount += 1;
				}
				if (AnimframeCount2 == 120) {
					AnimframeCount = 0;
					AnimframeCount1 = 0;
					AnimframeCount2 = 0;
					AnimCount = 0;
					score = 0;
					enemy.speed = 3.0f;
					TutorialGameflag = false;
					Push1flag = false;
					Tutorialflag = false;
					Moveflag = false;
					scene_no = eScene_SELECT;
				}
			}

			//チュートリアル
			if (Tutorialflag) {

				// チュートリアルスキップ
				if (preKeys[DIK_F] == 0 && keys[DIK_F] != 0) {
					AnimframeCount = 0;
					AnimframeCount1 = 0;
					AnimframeCount2 = 0;
					AnimCount = 0;
					score = 0;
					TutorialGameflag = false;
					Push1flag = false;
					Tutorialflag = false;
					Moveflag = false;
					scene_no = eScene_SELECT;
				}

				//移動

				if (keys[DIK_SPACE]) {  //スペースを押されたら時間を計測
					count++;
					onceflag = true;
					L_push = false;
					S_push = false;
				}

				if (Moveflag) {
					if (keys[DIK_SPACE] && onceflag == true && count >= 20) {  //スペースを長押しした時に移動
						onceflag = false;
						count = 0;
						L_push = true;
					}
				}
				else
				{
					count = 0;
				}

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else {
					S_push = false;
				}

				if (L_push == true) {  //長押し判定
					ball.position.y -= ball.speed;
					if (ball.position.y == 70) {  //上に移動した時
						ball.speed -= ball.speed + 180;
					}
					else if (ball.position.y == 430) {  //下に移動した時
						ball.speed -= ball.speed - 180;
					}
				}

				if (count == 0) {  //バグ対策
					L_push = false;
				}

				// 敵の移動

				if (isEnemyAlive) {
					enemy.position.x -= enemy.speed;
				}

				//敵の当たり判定

				if (S_push) {
					if (isEnemyAlive) {
						if (isHit(
							ball.position.x, ball.position.y, playerw, playerh, enemy.position.x,
							enemy.position.y, enemyw, enemyh, isHittrue)) {
							if (isEnemyAlive == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								score += 100;
								enemy.position.x = 1000.0f;
								enemy.position.y = 266.0f;
								isEnemyAlive = false;
							}
						}
					}
				}

				//リスポーンタイマー

				if (isEnemyAlive == false) {
					resporntimer--;
				}
				if (resporntimer == 0 && isEnemyAlive == false) {
					resporntimer = 50;
					isEnemyAlive = true;
				}

				//曲が変わったら切り替え
	
				if (Switchingflag) {
					if (preKeys[DIK_SPACE]) {
						if (!Novice::IsPlayingAudio(voiceHandle7) || voiceHandle7 == -1) {
							voiceHandle7 = Novice::PlayAudio(Success, false, 0.5f);
						}
						Push1flag = true;
						Switchingflag = false;
					}
				}
				else {
					Novice::StopAudio(voiceHandle7);
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}
			}

			break;
		case eScene_SELECT:

			if (!Novice::IsPlayingAudio(voiceHandle1) || voiceHandle1 == -1) {
				voiceHandle1 = Novice::PlayAudio(GameStart, true, 0.5f);
			}

			// タイトルに戻る
			if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
				Startflag = true;
				frameCount = 0;
				playerAnimCount = 0;
				enemy.speed = 3.0f;
				TutorialGameflag = true;
				scene_no = eScene_START;
			}

			//カーソル移動

			if (keys[DIK_A] && !preKeys[DIK_A] || keys[DIK_LEFT] && !preKeys[DIK_LEFT]) {  //←押したとき
				MoveC -= 1;
			}

			if (keys[DIK_D] && !preKeys[DIK_D] || keys[DIK_RIGHT] && !preKeys[DIK_RIGHT]) {  //→押したとき
				MoveC += 1;
			}

			if (keys[DIK_RETURN] && !preKeys[DIK_RETURN]) {  //ENTER押したとき
				Novice::StopAudio(voiceHandle1);
				if (MoveC == 0) {
					scene_no = eScene_STAGE_EASY;
				}
				if (MoveC == 1) {
					scene_no = eScene_STAGE_NORMAL;
				}
				if (MoveC == 2) {
					scene_no = eScene_STAGE_HARD;
				}
			}

			//カーソル移動の制限
			if (MoveC < 0) {
				MoveC = 0;
			}
			if (MoveC > 2) {
				MoveC = 2;
			}

			break;
		case eScene_STAGE_EASY:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				AnimframeCount++;
				time_count = 0;
				if (AnimframeCount == 60) {
					AnimCount += 1;
				}
				if (AnimframeCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Easyflag = true;
				}
			}

			//難易度簡単

			if (Stage_Easyflag) {

				if (preKeys[DIK_P] == 0 && keys[DIK_P] != 0) {
					scene_no = eScene_CLEAR;
				}
				
				//時間制限

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle2) || voiceHandle2 == -1) {
					voiceHandle2 = Novice::PlayAudio(BGM1, false, 0.5f);
				}

				//移動

				if (keys[DIK_SPACE]) {  //スペースを押されたら時間を計測
					count++;
					onceflag = true;
					L_push = false;
					S_push = false;
				}

				if (keys[DIK_SPACE] && onceflag == true && count >= 20) {  //スペースを長押しした時に移動
					onceflag = false;
					count = 0;
					L_push = true;
				}

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else{
					S_push = false;
				}

				if (L_push == true) {  //長押し判定
					ball.position.y -= ball.speed;
					if (ball.position.y == 70) {  //上に移動した時
						ball.speed -= ball.speed + 180;
					}
					else if (ball.position.y == 430) {  //下に移動した時
						ball.speed -= ball.speed - 180;
					}
				}

				if (count == 0) {  //バグ対策
					L_push = false;
				}

				// 敵の移動

				for (int j = 0; j < 40; j++) {
					if (Eenemy.isEenemyAlive[j] == true) {

						Eenemy.position[j].x -= Eenemy.speed[j];
					}
				}

				//敵の当たり判定

				if (S_push) {

					for (int j = 0; j < 40; j++) {
						if (isHit(
							ball.position.x, ball.position.y, ball.radius, ball.radius,
							Eenemy.position[j].x, Eenemy.position[j].y, Eenemy.radius[j],
							Eenemy.radius[j], isddd)) {
							if (Eenemy.isEenemyAlive[j] == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								score += 100;
							}
							Eenemy.isEenemyAlive[j] = false;
						}
					}
				}

				//カウントダウン
				if (time_count > 0) {
					pos_x -= 0.18f;
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}

				//曲が終わったら終了させる
				
				if (time_AnimCount > 115) {
					Novice::StopAudio(voiceHandle2);
					Clearflag = true;
				}

				if (time_AnimCount == 118) {
					time_count = 0;
					Stage_Easyflag = false;
					Stage_Easy_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}
			break;
		case eScene_STAGE_NORMAL:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				AnimframeCount++;
				time_count = 0
					;
				if (AnimframeCount == 60) {
					AnimCount += 1;
				}
				if (AnimframeCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Normalflag = true;
				}
			}

			//難易度普通

			if (Stage_Normalflag) {

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle3) || voiceHandle3 == -1) {
					voiceHandle3 = Novice::PlayAudio(BGM2, false, 0.5f);
				}

				//移動

				if (keys[DIK_SPACE]) {  //スペースを押されたら時間を計測
					count++;
					onceflag = true;
					L_push = false;
					S_push = false;
				}

				if (keys[DIK_SPACE] && onceflag == true && count >= 20) {  //スペースを長押しした時に移動
					onceflag = false;
					count = 0;
					L_push = true;
				}

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else {
					S_push = false;
				}

				if (L_push == true) {  //長押し判定
					ball.position.y -= ball.speed;
					if (ball.position.y == 70) {  //上に移動した時
						ball.speed -= ball.speed + 180;
					}
					else if (ball.position.y == 430) {  //下に移動した時
						ball.speed -= ball.speed - 180;
					}
				}

				if (count == 0) {  //バグ対策
					L_push = false;
				}

				// 敵の移動
				
				for (int j = 0; j < 90; j++) {
					if (Eenemy.isEenemyAlive[j] == true) {

						Eenemy.position[j].x -= Eenemy.speed[j] + 3.0f;
					}
				}

				//敵の当たり判定

				if (S_push) {

					for (int j = 0; j < 90; j++) {
						if (isHit(
							ball.position.x, ball.position.y, ball.radius, ball.radius,
							Eenemy.position[j].x, Eenemy.position[j].y, Eenemy.radius[j],
							Eenemy.radius[j], isddd)) {
							if (Eenemy.isEenemyAlive[j] == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								score += 100;
							}
							Eenemy.isEenemyAlive[j] = false;
						}
					}
				}

				//曲が変わったら切り替え

				//1回目の切り替え

				if (time_AnimCount > 22) { 
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = true;
							Switchingflag = false;
						}
					}
				}

				//2回目の切り替え

				if (time_AnimCount > 48) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Switchingflag = false;
						}
					}
				}

				//3回目の切り替え

				if (time_AnimCount > 82) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = true;
							Switchingflag = false;
						}
					}
				}

				//4回目切り替え

				if (time_AnimCount > 107) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Switchingflag = false;
						}
					}
				}

				//カウントダウン

				if (time_count > 0) {
					pos_x -= 0.16f;
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				bg3X = bg3X - bg_speed;
				bg4X = bg4X - bg_speed;

				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}
				if (bg3X < -1270 && bg4X < 0) {
					bg3X = 1270;
				}
				if (bg4X < -1270 && bg3X < 0) {
					bg4X = 1270;
				}

				//曲が終わったら終了させる

				if (time_AnimCount > 130) {
					Novice::StopAudio(voiceHandle3);
					Clearflag = true;
				}

				if (time_AnimCount == 134) {
					time_count = 0;
					Stage_Normalflag = false;
					Stage_Normal_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}

			break;
		case eScene_STAGE_HARD:
			//マウスカーソル削除

			Novice::SetMouseCursorVisibility(0);

			//ゲームスタートアニメーション

			if (Gameflag) {
				AnimframeCount++;
				time_count = 0;
				if (AnimframeCount == 60) {
					AnimCount += 1;
				}
				if (AnimframeCount == 120) {
					AnimCount = 0;
					Gameflag = false;
					Stage_Hardflag = true;
				}
			}

			//難易度難しい

			if (Stage_Hardflag) {

				time_count++;

				if (time_count == 60) {
					time_AnimCount += 1;
					time_count = 0;
				}

				if (!Novice::IsPlayingAudio(voiceHandle4) || voiceHandle4 == -1) {
					voiceHandle4 = Novice::PlayAudio(BGM3, false, 0.5f);
				}

				//移動

				if (keys[DIK_SPACE]) {  //スペースを押されたら時間を計測
					count++;
					onceflag = true;
					L_push = false;
					S_push = false;
				}

				if (keys[DIK_SPACE] && onceflag == true && count >= 20) {  //スペースを長押しした時に移動
					onceflag = false;
					count = 0;
					L_push = true;
				}

				if (preKeys[DIK_SPACE] && !keys[DIK_SPACE] && onceflag == true && count <= 20) {  //スペースを長押しない時に攻撃
					onceflag = false;
					count = 0;
					S_push = true;
				}
				else {
					S_push = false;
				}

				if (L_push == true) {  //長押し判定
					ball.position.y -= ball.speed;
					if (ball.position.y == 70) {  //上に移動した時
						ball.speed -= ball.speed + 180;
					}
					else if (ball.position.y == 430) {  //下に移動した時
						ball.speed -= ball.speed - 180;
					}
				}

				if (count == 0) {  //バグ対策
					L_push = false;
				}

				// 敵の移動

				for (int j = 0; j < 200; j++) {
					if (Eenemy.isEenemyAlive[j] == true) {

						Eenemy.position[j].x -= Eenemy.speed[j] + 6.0f;
					}
				}

				//敵の当たり判定

				if (S_push) {

					for (int j = 0; j < 200; j++) {
						if (isHit(
							ball.position.x, ball.position.y, ball.radius, ball.radius,
							Eenemy.position[j].x, Eenemy.position[j].y, Eenemy.radius[j],
							Eenemy.radius[j], isddd)) {
							if (Eenemy.isEenemyAlive[j] == true) {
								if (!Novice::IsPlayingAudio(voiceHandle6) || voiceHandle6 == -1) {
									voiceHandle6 = Novice::PlayAudio(Kill, false, 20.0f);
								}
								score += 100;
							}
							Eenemy.isEenemyAlive[j] = false;
						}
					}
				}

				//曲が変わったら切り替え

				//1回目の切り替え

				if (time_AnimCount > 14) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = true;
							Switchingflag = false;
						}
					}
				}

				//2回目の切り替え

				if (time_AnimCount > 36) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Push2flag = true;
							Switchingflag = false;
						}
					}
				}

				//3回目の切り替え

				if (time_AnimCount > 48) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Push2flag = false;
							Switchingflag = false;
						}
					}
				}

				//4回目切り替え

				if (time_AnimCount > 61) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Push2flag = true;
							Switchingflag = false;
						}
					}
				}

				//5回目切り替え

				if (time_AnimCount > 79) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = false;
							Push2flag = false;
							Switchingflag = false;
						}
					}
				}

				//6回目切り替え

				if (time_AnimCount > 99) {
					Switchingflag = true;
					if (Switchingflag) {
						if (preKeys[DIK_SPACE]) {
							Push1flag = true;
							Push2flag = false;
							Switchingflag = false;
						}
					}
				}

				//カウントダウン

				if (time_count > 0) {
					pos_x -= 0.17f;
				}

				//画像スクロール

				bg1X = bg1X - bg_speed;
				bg2X = bg2X - bg_speed;
				bg3X = bg3X - bg_speed;
				bg4X = bg4X - bg_speed;
				bg5X = bg5X - bg_speed;
				bg6X = bg6X - bg_speed;

				if (bg1X < -1270 && bg2X < 0) {
					bg1X = 1270;
				}
				if (bg2X < -1270 && bg1X < 0) {
					bg2X = 1270;
				}
				if (bg3X < -1270 && bg4X < 0) {
					bg3X = 1270;
				}
				if (bg4X < -1270 && bg3X < 0) {
					bg4X = 1270;
				}
				if (bg5X < -1270 && bg6X < 0) {
					bg5X = 1270;
				}
				if (bg6X < -1270 && bg5X < 0) {
					bg6X = 1270;
				}

				//曲が終わったら終了させる

				if (time_AnimCount > 121) {
					Novice::StopAudio(voiceHandle4);
					Clearflag = true;
				}

				if (time_AnimCount == 125){
					time_count = 0;
					Stage_Hardflag = false;
					Stage_Hard_Clearflag = true;
					scene_no = eScene_CLEAR;
				}
			}

			break;
		case eScene_CLEAR:
			if (Clearflag) {
				if (!Novice::IsPlayingAudio(voiceHandle5) || voiceHandle5 == -1) {
					voiceHandle5 = Novice::PlayAudio(ResultBGM, true, 0.5f);
				}
				if (Stage_Easy_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					num[0] = { 0 };
					for (int j = 0; j < 40; j++) {
						for (int i = 0; i < 40; i++) {
							Eenemy.position[i].x = { 1000.0f + i * 600.0f };
						}
					}
					pos_x = 1200.0f;
					AnimframeCount = 0;
					time_AnimCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						Clearflag = false;
						isEnemyAlive = true;
						Stage_Easy_Clearflag = false;
						Push1flag = false;
						Switchingflag = false;
						score = 0;
						scene_no = eScene_SELECT;
					}
				}
				if (Stage_Normal_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					enemy.position.x = 1000.0f; 
					enemy.position.y = 270.0f;
					pos_x = 1200.0f;
					AnimframeCount = 0;
					time_AnimCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						isEnemyAlive = true;
						Clearflag = false;
						Stage_Normal_Clearflag = false;
						Push1flag = false;
						Switchingflag = false;
						score = 0;
						scene_no = eScene_SELECT;
					}
				}
				if (Stage_Hard_Clearflag) {
					ball.position.x = 100.0f;
					ball.position.y = 250.0f;
					enemy.position.x = 1000.0f;
					enemy.position.y = 270.0f;
					pos_x = 1200.0f;
					AnimframeCount = 0;
					time_AnimCount = 0;
					if (keys[DIK_R]) {
						Gameflag = true;
						isEnemyAlive = true;
						Clearflag = false;
						Stage_Hard_Clearflag = false;
						Push1flag = false;
						Push2flag = false;
						Switchingflag = false;
						score = 0;
						scene_no = eScene_SELECT;
					}
				}
			}
			if (!Clearflag) {
				Novice::StopAudio(voiceHandle5);
			}
			break;
		}
		/// ↑更新処理ここまで

		/// ↓描画処理ここから
		switch (scene_no) {
		case eScene_START:
			if (playerAnimCount == 0) {
				Novice::DrawSprite(0, 0, Start[0], 1, 1, 0.0f, WHITE);
			}
			if (playerAnimCount == 1) {
				Novice::DrawSprite(0, 0, Start[1], 1, 1, 0.0f, WHITE);
			}
			if (playerAnimCount == 2) {
				Novice::DrawSprite(0, 0, Start[2], 1, 1, 0.0f, WHITE);
			}
			if (playerAnimCount == 3) {
				Novice::DrawSprite(0, 0, Start[3], 1, 1, 0.0f, WHITE);
			}
			if (playerAnimCount == 4) {
				Novice::DrawSprite(0, 0, Start[4], 1, 1, 0.0f, WHITE);
			}
			if (playerAnimCount == 5) {
				Novice::DrawSprite(0, 0, Start[5], 1, 1, 0.0f, WHITE);
				if (Upflag) {
					Novice::DrawSprite(550, 450, Arrow, 1, 1, 0.0f, WHITE);
				}
				if (Downflag) {
					Novice::DrawSprite(550, 580, Arrow, 1, 1, 0.0f, WHITE);
				}
			}
			break;
		case eScene_SELECT:
			if (MoveC == 0) {
				Novice::DrawSprite(0, 0, Select[0], 1, 1, 0.0f, WHITE);
			}
			if (MoveC == 1) {
				Novice::DrawSprite(0, 0, Select[1], 1, 1, 0.0f, WHITE);
			}
			if (MoveC == 2) {
				Novice::DrawSprite(0, 0, Select[2], 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_TUTORIAL:
			if (!Push1flag) {
				Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			}
			if (Push1flag) {
				Novice::DrawSprite(bg3X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg4X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
			}
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 0, HukidasiUI, 1, 1, 0.0f, WHITE);
			if (AnimCount == 0) {
				Novice::DrawSprite(100, -10, TutorialUI[0], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 1) {
				Novice::DrawSprite(100, -10, TutorialUI[1], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 2) {
				Novice::DrawSprite(100, -10, TutorialUI[2], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 3) {
				Novice::DrawSprite(100, -10, TutorialUI[3], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 4) {
				Novice::DrawSprite(100, -10, TutorialUI[4], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 5) {
				Novice::DrawSprite(100, -10, TutorialUI[5], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 6) {
				Novice::DrawSprite(100, -10, TutorialUI[6], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 7) {
				Novice::DrawSprite(100, -10, TutorialUI[7], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 8) {
				Novice::DrawSprite(100, -10, TutorialUI[8], 1, 1, 0.0f, WHITE);
			}
			if (AnimCount == 9) {
				Novice::DrawSprite(100, -10, TutorialUI[9], 1, 1, 0.0f, WHITE);
			}
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			if (Eenemy.isEenemyAlive) {
				Novice::DrawSprite(int(enemy.position.x), int(enemy.position.y), Ika, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_EASY:
			Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			for (int i = 0; i < 40; i++) {
				if (Eenemy.isEenemyAlive[i] == true) {
					Novice::DrawSprite(int(Eenemy.position[i].x), int(Eenemy.position[i].y), Ika, 1, 1, 0.0f,WHITE);
				}
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if(Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_NORMAL:
			if (!Push1flag) {
				Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
			}
			if (Push1flag) {
				Novice::DrawSprite(bg3X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
				Novice::DrawSprite(bg4X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
			}
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			for (int i = 0; i < 90; i++) {
				if (Eenemy.isEenemyAlive[i] == true) {
					Novice::DrawSprite(int(Eenemy.position[i].x), int(Eenemy.position[i].y) + 50, Maguro, 1, 1, 0.0f,WHITE);
				}
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if (Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_STAGE_HARD:
			if (!Push1flag) {
				if (!Push2flag) {
					Novice::DrawSprite(bg1X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(bg2X, 0, Haikei[0], 1, 1, 0.0f, WHITE);
				}
			}
			if (Push1flag) {
				if (!Push2flag) {
					Novice::DrawSprite(bg3X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(bg4X, 0, Haikei[1], 1, 1, 0.0f, WHITE);
				}
			}
			if (!Push1flag) {
				if (Push2flag) {
					Novice::DrawSprite(bg5X, 0, Haikei[2], 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(bg6X, 0, Haikei[2], 1, 1, 0.0f, WHITE);
				}
			}
			Novice::DrawLine(100, 180, 1200, 180, WHITE);
			Novice::DrawLine(100, 360, 1200, 360, WHITE);
			Novice::DrawLine(100, 540, 1200, 540, WHITE);
			Novice::DrawSprite(0, 650, CountUI, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)pos_x, (int)pos_y, Ebi, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(ball.position.x), int(ball.position.y), Ebi, 1, 1, 0.0f, WHITE);
			for (int i = 0; i < 200; i++) {
				if (Eenemy.isEenemyAlive[i] == true) {
					Novice::DrawSprite(int(Eenemy.position[i].x), int(Eenemy.position[i].y) + 50, Same, 1, 1, 0.0f, WHITE);
				}
			}
			if (Gameflag) {
				if (AnimCount == 0) {
					Novice::DrawSprite(260, 180, ReadyUI, 1, 1, 0.0f, WHITE);
				}
				if (AnimCount == 1) {
					Novice::DrawSprite(260, 180, StartUI, 1, 1, 0.0f, WHITE);
				}
			}
			if (Clearflag) {
				Novice::DrawSprite(260, 180, ClearUI, 1, 1, 0.0f, WHITE);
			}
			break;
		case eScene_CLEAR:
			Novice::DrawSprite(0, 0, Result, 1, 1, 0.0f, WHITE);
			while (kazu > 0) {
				int ans = kazu / waru_kazu;
				Novice::DrawSprite(x + (keta * 142), y, graph[ans], 1, 1, 0.0f, WHITE);
				keta = keta + 1;
				kazu = kazu - waru_kazu * ans;
				waru_kazu = waru_kazu / 10;
			}
			if (score == 0) {
				kazu = 12345;
				waru_kazu = 10000;
				keta = 0;
			}
			break;
		}
		/// ↑描画処理ここまで

		// フレームの終了
		Novice::EndFrame();

	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
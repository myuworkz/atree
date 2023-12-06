#include "ofApp.h"


//宣言
#pragma region definition

int sceneWhere;
float alphaMoji;
bool FirstTime = true;
static const int LEAFNUM = 300;
float gamenWidth;
float gamenHeight;
float scene3Bef;
float xLeaf[LEAFNUM];
float yLeaf[LEAFNUM];
float rLeaf[LEAFNUM];
float argXLeaf[LEAFNUM];
float argYLeaf[LEAFNUM];
float argRLeaf[LEAFNUM];
int seasonTimer;
int seasonDur[4];
int orderLeaf;
float satLeaf;
float hueLeaf;
float valLeaf;
bool MenuTab;
bool TabHover[3];
int popTimer;
bool CharaPop;
unsigned int cNum[10];
int cTime[10];
unsigned int cImgCount;
bool BackReset;
bool DrawIllust;
int penType;
int penSize;
int penColor;
int eraserSize;
char fileName[255];
bool PrintIllust;
bool DrawMove;
unsigned int drawCounter = 1;
int moveCount;
static const int cMaxTime = 300;
float cPosX[99][cMaxTime];
float cPosY[99][cMaxTime];
float setSize[99];
bool ClickButton[3][5];
bool DrawFinish;
bool TakeSnap;
unsigned int snapCounter = 0;
int snapWidth[6];
int snapHeight[6];


#pragma endregion


//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(255, 255, 255);
	ofSetCircleResolution(64);
	ofSetLineWidth(1);
	ofSetFrameRate(30);
	ofEnableAlphaBlending();

	//画面サイズ
	gamenWidth = ofGetWidth();
	gamenHeight = ofGetHeight();

	//ロード
	#pragma region Load

	//文字設定
	ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 40));
	set.addRanges(ofAlphabet::Latin);
	font.load(set);

	//画像
	imgOrg.allocate(600, 450, OF_IMAGE_COLOR_ALPHA);
	imgCmp.allocate(600, 450, OF_IMAGE_COLOR_ALPHA);
	title.load("title.png");
	intro.load("intro.png");

	//音楽
	player.load("ATreeMusic.mp3");
	player.setLoop(true);
	player.play();

	#pragma endregion


	//初期化
	#pragma region Initialize

	sceneWhere = 1;
	alphaMoji = 0;
	scene3Bef = 0;
	for (int i = 0; i < LEAFNUM; i++) {

		argXLeaf[i] = ofRandom(0, 5000);
		argYLeaf[i] = ofRandom(0, 5000);
		argRLeaf[i] = ofRandom(0, 5000);
		
		xLeaf[i] = ofRandom(-300, 300) * (gamenHeight / 800);
		yLeaf[i] = gamenHeight / 3 + ofRandom(- 80, 350) * (gamenHeight / 800);
		rLeaf[i] = ofRandom(0, 360);		
	}

	seasonTimer = 0;
	seasonDur[0] = ofRandom(300, 700);
	seasonDur[1] = ofRandom(600, 1200);
	seasonDur[2] = ofRandom(600, 1000);
	seasonDur[3] = ofRandom(600, 1200);
	orderLeaf = 0;
	satLeaf = 0.25;
	hueLeaf = 120;
	valLeaf = 100;
	MenuTab = false;

	for (int i = 0; i < 3; i++) {
	
		TabHover[i] = false;
	}

	popTimer = 0;
	CharaPop = false;
	
	for (int i = 0; i < 10; i++) {
		
		cNum[i] = 0;
		cTime[i] = -1;
	}

	cImgCount = 0;
	BackReset = false;
	DrawIllust = false;
	penType = 1;
	penSize = 20;
	penColor = 0;
	eraserSize = 20;
	PrintIllust = false;
	DrawMove = 0;
	moveCount = 0;

	if (FirstTime == true) {

		for (int i = 0; i < 99; i++) {

			for (int j = 0; j < cMaxTime; j++) {

				cPosX[i][j] = 0;
				cPosY[i][j] = 0;
			}

			setSize[i] = 1;
		}

		FirstTime = false;
	}
	
	for (int i = 0; i < 3; i++) {

		for (int h = 0; h < 5; h++) {

			ClickButton[i][h] = false;
		}
	}

	DrawFinish = false;
	TakeSnap = false;

	for (int i = 0; i < 6; i++) {

		snapWidth[i] = 0;
		snapHeight[i] = 0;
	}

	#pragma endregion

}

//--------------------------------------------------------------
void ofApp::update(){
	
	//画面サイズ設定
	#pragma region size

	float befHeight = gamenHeight;

	//幅
	if (ofGetWidth() >= 800) {

		gamenWidth = ofGetWidth();
	}
	else {
		
		gamenWidth = 800;
	}

	//高さ
	if (ofGetHeight() >= 600) {

		gamenHeight = ofGetHeight();
	}
	else {
	
		gamenHeight = 600;
	}

	#pragma endregion 


	//ホーム&説明
	#pragma region Home&Introduction

	if (sceneWhere == 1 || sceneWhere == 2) {

		if (befHeight != gamenHeight) {

			//葉の位置再計算
			for (int i = 0; i < LEAFNUM; i++) {

				xLeaf[i] = xLeaf[i] / (befHeight / 800) * (gamenHeight / 800);
				yLeaf[i] = yLeaf[i] / (befHeight / 800) * (gamenHeight / 800);
			}

			//文字サイズ変更
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 40));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);
		}

		//点滅
		alphaMoji += 0.1;
	}

	#pragma endregion


	//鑑賞シーン
	#pragma region Main

	if (sceneWhere == 3) {

		//背景
		#pragma region Back

			//葉動き
			#pragma region LeafMove

			//葉の位置再計算
			if (befHeight != gamenHeight) {

				for (int i = 0; i < LEAFNUM; i++) {

					xLeaf[i] = xLeaf[i] / (befHeight / 800) * (gamenHeight / 800);
					yLeaf[i] = yLeaf[i] / (befHeight / 800) * (gamenHeight / 800);
				}
			}

			//ノイズ引数
			for (int i = 0; i < LEAFNUM; i++) {

				//落ちる葉
				if ((seasonDur[0] + seasonDur[1] + seasonDur[2] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2] + seasonDur[3]) && (i < orderLeaf)) {
		
					argXLeaf[i] += 0.01;
					argYLeaf[i] += 0.01;
					argRLeaf[i] += 0.1;
				}

				else {

					argXLeaf[i] += 0.0049;
					argYLeaf[i] += 0.0049;
					argRLeaf[i] += 0.0049;
				}
			}
			
			#pragma endregion


			//季節
			#pragma region LeafColor

			//タイムカウント
			seasonTimer++;

			//春
			if (0 <= seasonTimer && seasonTimer < seasonDur[0]) {

				orderLeaf += 10;

				if (80 <= seasonTimer && seasonTimer < 100) {

					satLeaf = 0.5;
				}

				if (seasonTimer >= 100) {

					satLeaf = 1;
				}
			}

			//夏
			if (seasonDur[0] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1]) {

				//初期化
				if (seasonTimer == seasonDur[0]) {

					orderLeaf = 0;
				}
				else {

					orderLeaf += 2;
				}
			}

			//秋
			if (seasonDur[0] + seasonDur[1] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2]) {
		
				//初期化
				if (seasonTimer == seasonDur[0] + seasonDur[1]) {

					orderLeaf = 0;
				}

				//だんだん色づく
				if (hueLeaf > 0) {

					hueLeaf -= 120 / (float)seasonDur[2];
				}

				if (valLeaf > 80) {

					valLeaf -= 120 / (float)seasonDur[2];
				}

			}

			//冬
			if (seasonDur[0] + seasonDur[1] + seasonDur[2] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2] + seasonDur[3]) {
				
				//初期化
				if (seasonTimer == seasonDur[0] + seasonDur[1] + seasonDur[2]) {

					for (int i = 0; i < LEAFNUM; i++) {

						//範囲外を一掃
						if (InCircle(-20 * (gamenHeight / 600), gamenHeight / 3 + 120 * (gamenHeight / 600), 150 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) == false && InCircle(-140 * (gamenHeight / 600), gamenHeight / 3 + 10 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) == false && InCircle(140 * (gamenHeight / 600), gamenHeight / 3 + 30 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) == false) {

							yLeaf[i] = -3000;
						}
					}
				}

				//葉が落ちる
				else {

					orderLeaf++;
					
					for (int i = 0; i < LEAFNUM; i++) {
						
						if (i < orderLeaf) {

							yLeaf[i] -= 5;
						}
					}
				}

			}

			//ループ
			if (seasonTimer >= seasonDur[0] + seasonDur[1] + seasonDur[2] + seasonDur[3]) {
			
				for (int i = 0; i < LEAFNUM; i++) {

					argXLeaf[i] = ofRandom(0, 5000);
					argYLeaf[i] = ofRandom(0, 5000);
					argRLeaf[i] = ofRandom(0, 5000);

					xLeaf[i] = ofRandom(-300, 300) * (gamenHeight / 800);
					yLeaf[i] = gamenHeight / 3 + ofRandom(-80, 350) * (gamenHeight / 800);
					rLeaf[i] = ofRandom(0, 360);
				}

				seasonTimer = 0;
				seasonDur[0] = ofRandom(300, 700);
				seasonDur[1] = ofRandom(600, 1200);
				seasonDur[2] = ofRandom(600, 1000);
				seasonDur[3] = ofRandom(600, 1200);
				orderLeaf = 0;
				satLeaf = 0.25;
				hueLeaf = 120;
				valLeaf = 100;
			}

			//cout << seasonTimer << "\n";

			#pragma endregion

		#pragma endregion


		//メニュータブ
		#pragma region Menu

		//タブの文字サイズ
		if (befHeight != gamenHeight) {

			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);
		}

		//タブのホバー
		if (MenuTab == true) {

			//タブ1
			if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), gamenHeight / 20, gamenHeight / 4, gamenHeight / 12, mouseX, mouseY)) {
			
				TabHover[0] = true;
			}
			else {

				TabHover[0] = false;
			}

			//タブ2
			if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 4, gamenHeight / 12, mouseX, mouseY)) {

				TabHover[1] = true;
			}
			else {

				TabHover[1] = false;
			}

			//タブ3
			if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12 * 2), gamenHeight / 4, gamenHeight / 12, mouseX, mouseY)) {

				TabHover[2] = true;
			}
			else {

				TabHover[2] = false;
			}
		}

		#pragma endregion


		//生物
		#pragma region Creature

		//湧き間隔
		popTimer++;

		//描画時間
		for(int i = 0; i < 10; i++){

			if (cTime[i] >= 0) {

				cTime[i]++;
			
				//生き物帰る
				if ((cTime[i] >= cMaxTime) || (cPosX[cNum[i]][cTime[i]] == 0 && cPosY[cNum[i]][cTime[i]] == 0)) {

					cTime[i] = -1;
				}
			}
		}

		//抽選
		if (popTimer >= 60) {

			popTimer = 0;
			int ran = ofRandom(0, 10);

			if (ran == 0) {

				CharaPop = true;
				cNum[cImgCount] = ofRandom(0, drawCounter);
				cout << "pop : creature_" << cNum[cImgCount] << "\n";
			}
		}

		//生き物出力
		if (CharaPop == true) {
			
			sprintf(fileName, "picCreature_%.2u.png", cNum[cImgCount]);
			cImg[cImgCount].load(fileName);
			cTime[cImgCount] = 0;
			
			cImgCount++;
			CharaPop = false;
		}

		//画像インスタンスリセット
		if (cImgCount >= 10) {

			cImgCount = 0;
		}

		#pragma endregion 

	}

	#pragma endregion	


	//お絵描きシーン
	#pragma region Drawing

	if (sceneWhere == 4) {

		if (befHeight != gamenHeight) {

			BackReset = true;

			//文字サイズ
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);
		}

		if (DrawFinish == true) {

			alphaMoji += 0.1;
		}
	}

	#pragma endregion


	//動作設定シーン
	#pragma region Moveing

	if (sceneWhere == 5) {

		if (befHeight != gamenHeight) {

			//文字サイズ
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);
		}

		if (DrawFinish == true) {

			alphaMoji += 0.2;
		}

		//動作設定
		#pragma region Move

		if (moveCount>= cMaxTime) {

			DrawMove = false;
		}

		if (DrawMove == true) {

			moveCount++;
			cPosX[drawCounter - 1][moveCount - 1] = mouseX - gamenWidth / 2;
			cPosY[drawCounter - 1][moveCount - 1] = mouseY - gamenHeight / 2;
		}

		#pragma endregion
	
	}

	#pragma endregion

}

//--------------------------------------------------------------
void ofApp::draw(){

	//ホーム
	#pragma region Home

	if (sceneWhere == 1) {

		//背景地
		float alphaBack = 255;

		for (int y = 0; y <= gamenHeight * 3 / 4; y++) {
			
			ofSetColor(150, 150, 150, alphaBack);
			ofDrawLine(0, y, gamenWidth, y);

			alphaBack -= 255 / (gamenHeight * 3 / 4);

		}

		//木
		#pragma region Wood

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight);

		//幹
		ofFill();
		ofSetColor(220, 220, 220);
		drawWood(0, gamenHeight / 3, gamenHeight / 800);
		
		//葉
		ofScale(1, -1);

		//葉の数だけ繰り返す
		for (int i = 0; i < LEAFNUM; i++) {

			if (InCircle(-20 * (gamenHeight / 600), gamenHeight / 3 + 120 * (gamenHeight / 600), 150 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(-140 * (gamenHeight / 600), gamenHeight / 3 + 10 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(140 * (gamenHeight / 600), gamenHeight / 3 + 30 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i])) {

				ofFill();
				ofSetColor(200, 200, 200, 200);
				drawLeaf(xLeaf[i], yLeaf[i], rLeaf[i], gamenHeight / 1200);
			}
		}

		ofPopMatrix();

		#pragma endregion

		//タイトル
		ofSetColor(255, 255, 255);
		title.setAnchorPercent(0.5, 0.5);
		title.draw(gamenWidth / 2, gamenHeight / 2, gamenHeight * 4 / 3, gamenHeight);

		//START
		ofSetColor(80, 80, 80, 255 * (0.3 * sin(alphaMoji) + 0.7));
		float wMoji = font.stringWidth("PRESS SOMEWHERE TO START");
		float hMoji = font.stringHeight("PRESS SOMEWHERE TO START");

		font.drawString("PRESS SOMEWHERE TO START", gamenWidth / 2 - wMoji / 2, gamenHeight * 3 / 4 + hMoji / 2);
	}

	#pragma endregion


	//説明
	#pragma region Introduction

	if (sceneWhere == 2) {

		//背景地
		float alphaBack = 255;

		for (int y = 0; y <= gamenHeight * 3 / 4; y++) {

			ofSetColor(150, 150, 150, alphaBack);
			ofDrawLine(0, y, gamenWidth, y);

			alphaBack -= 255 / (gamenHeight * 3 / 4);
		}

		//木
		#pragma region Wood

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight);

		//幹
		ofFill();
		ofSetColor(220, 220, 220);
		drawWood(0, gamenHeight / 3, gamenHeight / 800);

		//葉
		ofScale(1, -1);

		//葉の数だけ繰り返す
		for (int i = 0; i < LEAFNUM; i++) {

			if (InCircle(-20 * (gamenHeight / 600), gamenHeight / 3 + 120 * (gamenHeight / 600), 150 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(-140 * (gamenHeight / 600), gamenHeight / 3 + 10 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(140 * (gamenHeight / 600), gamenHeight / 3 + 30 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i])) {

				ofFill();
				ofSetColor(200, 200, 200, 200);
				drawLeaf(xLeaf[i], yLeaf[i], rLeaf[i], gamenHeight / 1200);
			}
		}

		ofPopMatrix();

		#pragma endregion

		//説明
		ofSetColor(255, 255, 255, alphaMoji * 100);
		intro.draw(gamenWidth / 2, gamenHeight / 2, gamenHeight * 4 / 3, gamenHeight);
	
		//NEXT
		ofSetColor(80, 80, 80, 255 * (0.3 * sin(alphaMoji) + 0.7));
		float wMoji = font.stringWidth("PRESS SOMEWHERE TO NEXT");
		float hMoji = font.stringHeight("PRESS SOMEWHERE TO NEXT");

		font.drawString("PRESS SOMEWHERE TO NEXT", gamenWidth / 2 - wMoji / 2, gamenHeight - gamenHeight / 10 + hMoji / 2);
	}

	#pragma endregion


	//鑑賞シーン
	#pragma region Main

	if(sceneWhere == 3){

		//背景地
		float alphaBack = 255;

		for (int y = 0; y <= gamenHeight * 2 / 3; y++) {

			ofSetColor(150, 150, 150, alphaBack);
			ofDrawLine(0, y, gamenWidth, y);

			alphaBack -= 255 / (gamenHeight * 2 / 3);
		}

		//背景
		#pragma region Back

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight);

			//背景幹
			#pragma region Wood

			ofFill();
			ofSetColor(100, 100, 100);
			drawWood(0, gamenHeight / 3, gamenHeight / 800);

			#pragma endregion
	

			//背景葉
			#pragma region Leaf

			ofScale(1, -1);

			//葉の数だけ繰り返す
			for(int i = 0; i < LEAFNUM; i++){
		
				float xL;
				float yL;
				float rL;

				//状態計算代入
				//落葉
				if ((seasonDur[0] + seasonDur[1] + seasonDur[2] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2] + seasonDur[3]) && (i < orderLeaf)) {

					xL = myNoise(xLeaf[i] - 100 * (gamenHeight / 600), xLeaf[i] + 100 * (gamenHeight / 600), argXLeaf[i]);
					yL = myNoise(yLeaf[i] - 100 * (gamenHeight / 600), yLeaf[i] + 100 * (gamenHeight / 600), argYLeaf[i]);
					rL = myNoise(rLeaf[i] - 180, rLeaf[i] + 180, argRLeaf[i]);
				}
				//通常
				else {

					xL = myNoise(xLeaf[i] - 20 * (gamenHeight / 600), xLeaf[i] + 20 * (gamenHeight / 600), argXLeaf[i]);
					yL = myNoise(yLeaf[i] - 20 * (gamenHeight / 600), yLeaf[i] + 20 * (gamenHeight / 600), argYLeaf[i]);
					rL = myNoise(rLeaf[i] - 180, rLeaf[i] + 180, argRLeaf[i]);
				}

				//描画領域判定
				if (InCircle(-20 * (gamenHeight / 600), gamenHeight / 3 + 120 * (gamenHeight / 600), 150 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(-140 * (gamenHeight / 600), gamenHeight / 3 + 10 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i]) || InCircle(140 * (gamenHeight / 600), gamenHeight / 3 + 30 * (gamenHeight / 600), 90 * (gamenHeight / 600), xLeaf[i], yLeaf[i])) {
			
					ofFill();

					//春
					if (0 <= seasonTimer && seasonTimer < seasonDur[0]) {
				
						ofSetColor(255, 0, 255, 100);

						if (i < orderLeaf) {

							drawLeaf(xL, yL, rL, gamenHeight / 1200 * satLeaf);
						}
					}

					//夏
					else if (seasonDur[0] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1]) {
				
						//だんだん花が落ちる
						if (i < orderLeaf) {

							ofSetColor(0, 255, 0, 100);
						}
						else {

							ofSetColor(255, 0, 255, 100);
						}

						drawLeaf(xL, yL, rL, gamenHeight / 1200);
					}

					//秋
					else if(seasonDur[0] + seasonDur[1] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2]){
			
						//だんだん色づく
						ofSetHSVColor(hueLeaf, 100, valLeaf, 100);
						drawLeaf(xL, yL, rL, gamenHeight / 1200);
					}
				}

				//冬
				if (seasonDur[0] + seasonDur[1] + seasonDur[2] <= seasonTimer && seasonTimer < seasonDur[0] + seasonDur[1] + seasonDur[2] + seasonDur[3]) {

					//葉が落ちる
					if (yL >= gamenHeight / 12) {

						ofSetColor(204, 0, 0, 100);
						drawLeaf(xL, yL, rL, gamenHeight / 1200);
					}
				}
			}

			#pragma endregion

		ofPopMatrix();

		#pragma endregion


		//生物
		#pragma region Creature	

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight / 2);
		ofSetColor(255, 255, 255);
			
		for (int i = 0; i < 10; i++) {

			if (cTime[i] >= 0) {

				cImg[i].setAnchorPercent(0.5, 0.5);
				cImg[i].draw(cPosX[cNum[i]][cTime[i]] * (gamenWidth / 600), cPosY[cNum[i]][cTime[i]] * (gamenHeight / 450), (gamenHeight / 450) * 200 * setSize[cNum[i]], (gamenHeight / 450) * 150 * setSize[cNum[i]]);
			}
		}

		ofPopMatrix();

		#pragma endregion 


		//カメラ
		#pragma region Camera
		
			//スクショ撮影
			#pragma region Take

			if (TakeSnap == true) {

				int w = ofGetWidth();
				int h = ofGetHeight();

				snapCounter++;
				imgSnap.grabScreen(0, 0, w, h);

				//ピクセル用設定
				unsigned char * pixels;
				unsigned char * pixels2;
				pixels = (unsigned char*)malloc(w * h * 4);
				pixels2 = (unsigned char*)malloc(w * h * 4);

				//ピクセル記録
				pixels = imgSnap.getPixels().getData();

				//画像の幅
				int widthOfLine = w;

				for (int y = 0; y < h; y++) {

					for (int x = 0; x < w; x++) {

						//rb色置き換え
						int red = pixels[(widthOfLine * y + x) * 4];
						int green = pixels[(widthOfLine * y + x) * 4 + 1];
						int blue = pixels[(widthOfLine * y + x) * 4 + 2];

						pixels2[(widthOfLine * y + x) * 4] = blue;
						pixels2[(widthOfLine * y + x) * 4 + 1] = green;
						pixels2[(widthOfLine * y + x) * 4 + 2] = red;

					}
				}

				//構成
				imgSnap.setFromPixels(pixels2, w, h, OF_IMAGE_COLOR_ALPHA);

				//出力
				sprintf(fileName, "snapshot_%.2u.jpg", snapCounter);
				imgSnap.saveImage(fileName);

				TakeSnap = false;
			}

			#pragma endregion


			//カメラボタン
			#pragma region Camera

			ofFill();
			ofClickColor(ClickButton[0][1]);
			ofDrawRectangle(gamenHeight / 20, gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight / 20, gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);

			ofFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight * 7 / 120, gamenHeight / 15 + gamenHeight / 200, gamenHeight / 20, gamenHeight / 30);
			ofDrawRectangle(gamenHeight / 15, gamenHeight / 15, gamenHeight / 60, gamenHeight / 120);

			ofFill();
			ofSetColor(255, 255, 255);
			ofDrawCircle(gamenHeight / 12, gamenHeight / 12 + gamenHeight / 200, gamenHeight / 80);

			ofFill();
			ofSetColor(0, 0, 0);
			ofDrawCircle(gamenHeight / 12, gamenHeight / 12 + gamenHeight / 200, gamenHeight / 100);

			#pragma endregion

		#pragma endregion


		//メニュータブ
		#pragma region Menu

		float wMoji;
		float hMoji;

		//メニューボタン
		if (MenuTab == false) {

			ofFill();
			ofClickColor(ClickButton[0][0]);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 20, gamenHeight / 12, gamenHeight / 12);

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 20, gamenHeight / 12, gamenHeight / 12);

			ofFill();
			ofSetColor(120, 120, 120);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 2 / 30), gamenHeight * 2 / 30, gamenHeight / 20, gamenHeight / 60);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 2 / 30), gamenHeight / 10, gamenHeight / 20, gamenHeight / 60);
		}

		//タブ
		else {

			//タブ1
			ofFill();

			if (TabHover[0] == true) {

				ofSetColor(200, 200, 200);
			}
			else {

				ofSetColor(255, 255, 255);
			}

			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), gamenHeight / 20, gamenHeight / 4, gamenHeight / 12);

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), gamenHeight / 20, gamenHeight / 4, gamenHeight / 12);

			wMoji = font.stringWidth("Drawing");
			hMoji = font.stringHeight("Drawing");
			ofSetColor(120, 120, 120);
			font.drawString("Drawing", gamenWidth - (gamenHeight / 20 + gamenHeight / 8) - wMoji / 2, (gamenHeight / 20 + gamenHeight / 24) + hMoji / 2);


			//タブ2
			ofFill();

			if (TabHover[1] == true) {

				ofSetColor(200, 200, 200);
			}
			else {

				ofSetColor(255, 255, 255);
			}

			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 4, gamenHeight / 12);

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 4, gamenHeight / 12);

			wMoji = font.stringWidth("Gallery");
			hMoji = font.stringHeight("Gallery");
			ofSetColor(120, 120, 120);
			font.drawString("Gallery", gamenWidth - (gamenHeight / 20 + gamenHeight / 8) - wMoji / 2, (gamenHeight / 20 + gamenHeight / 12 + gamenHeight / 24) + hMoji / 2);
		

			//タブ3
			ofFill();

			if (TabHover[2] == true) {

				ofSetColor(200, 200, 200);
			}
			else {

				ofSetColor(255, 255, 255);
			}
			
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12 * 2), gamenHeight / 4, gamenHeight / 12);

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12 * 2), gamenHeight / 4, gamenHeight / 12);

			wMoji = font.stringWidth("Home");
			hMoji = font.stringHeight("Home");
			ofSetColor(120, 120, 120);
			font.drawString("Home", gamenWidth - (gamenHeight / 20 + gamenHeight / 8) - wMoji / 2, (gamenHeight / 20 + gamenHeight / 12 * 2 + gamenHeight / 24) + hMoji / 2);

		}

		#pragma endregion

	}

	#pragma endregion


	//お絵描きシーン
	#pragma region Drawing

	if (sceneWhere == 4) {
		
		//キャンバス
		#pragma region Canvas

		if (BackReset == true) {

			ofFill();
			ofSetColor(200, 200, 200);
			ofDrawRectangle(0, 0, gamenWidth, gamenHeight);

			BackReset = false;
		}

		#pragma endregion


		//絵筆
		#pragma region DrawIllust

		if (DrawIllust == true) {

			ofFill();

			if (penType == 1) {

				ofSetColor(penColor, penColor, penColor);
				ofDrawCircle(mouseX, mouseY, penSize);
			}

			else if (penType == 2) {

				ofSetColor(200, 200, 200);
				ofDrawCircle(mouseX, mouseY, eraserSize);
			}
		}

		#pragma endregion


		//ボード
		#pragma region Board

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight / 2);
		ofScale(1, -1);
		
		//塗りつぶし
		ofFill();
		ofSetColor(255, 255, 255);
		ofDrawRectangle(-gamenWidth / 2, gamenHeight / 2, gamenWidth, 226 - gamenHeight / 2);
		ofDrawRectangle(-gamenWidth / 2, 226, gamenWidth / 2 - 300, -451);
		ofDrawRectangle(300, 226, gamenWidth / 2 - 300, -451);
		ofDrawRectangle(-gamenWidth / 2, -225, gamenWidth, 225 - gamenHeight / 2);

		//枠
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(-301, 226, 602, -452);
		
		ofPopMatrix();

		#pragma endregion


		//説明
		float wMoji = font.stringWidth("Draw your character.");
		float hMoji = font.stringHeight("Draw your character.");
		ofSetColor(0, 0, 0);
		font.drawString("Draw your character.", gamenWidth / 2 - wMoji / 2, gamenHeight / 30 + hMoji / 2);


		//戻るボタン
		#pragma region Return

		ofFill();
		ofClickColor(ClickButton[1][0]);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);
		
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight * 7 / 120), gamenHeight / 12);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 7 / 120);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 13 / 120);
		
		#pragma endregion


		//消去ボタン
		#pragma region Delete

		ofFill();
		ofClickColor(ClickButton[1][1]);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15);
		
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15);

		ofFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 24), (gamenHeight / 20 + gamenHeight * 7 / 40), gamenHeight / 60, gamenHeight / 200);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 7 / 120), (gamenHeight / 20 + gamenHeight * 9 / 50), gamenHeight / 20, gamenHeight / 120);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 11 / 200), (gamenHeight / 20 + gamenHeight * 23 / 120), gamenHeight * 13 / 300, gamenHeight / 30);

		#pragma endregion


		//決定ボタン
		#pragma region Enter
		
		ofFill();
		ofClickColor(ClickButton[1][2]);

		if (DrawFinish == true) {

			ofSetColor(80, 80, 80, 255 * (0.5 * sin(alphaMoji) + 0.3));
		}

		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15);
		
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12);
		ofDrawLine(gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight - gamenHeight * 7 / 120);
		ofDrawLine(gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight - gamenHeight * 13 / 120);
	
		#pragma endregion


		//入力中ボタン表示
		#pragma region NowButton

			//ボタンリセット
			#pragma region ResetButton

			ofFill();
			ofSetColor(255, 255, 255);

			//ペンサイズリセット
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);


			//ペン色リセット
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);


			//消しゴムリセット
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);

			#pragma endregion


			//ボタンオン
			#pragma region turnButton

			ofFill();
			ofSetColor(200, 200, 200);

			//ペン
			if (penType == 1) {

				//ペンサイズ
				if (penSize == 20) {
				
					ofDrawRectangle(gamenHeight / 40, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
				}

				else if (penSize == 10) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
				}

				else if (penSize == 5) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
				}

				else if (penSize == 1) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);
				}


				//ペン色
				if(penColor == 0){

					ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
				}

				else if (penColor == 150) {

					ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
				}

				else if (penColor == 255) {

					ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
				}
			}

			//消しゴム
			else if (penType == 2) {

				if (eraserSize == 20) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 2, gamenHeight / 20, gamenHeight / 20);
				}

				else if (eraserSize == 10) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
				}

				else if (eraserSize == 5) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
				}

				else if (eraserSize == 1) {

					ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);
				}
			}

			#pragma endregion

		#pragma endregion


		//ペンボタン
		#pragma region Pen

		//ペン
		ofFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenHeight / 20, gamenHeight / 6, gamenHeight / 20, gamenHeight / 100);
		ofDrawTriangle(gamenHeight * 31 / 300, gamenHeight / 6, gamenHeight * 31 / 300, gamenHeight / 6 + gamenHeight / 100, gamenHeight * 67 / 600, gamenHeight / 6 + gamenHeight / 200);


			//ペンサイズボタン
			#pragma region PenSize

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);
		
			ofFill();
			ofDrawCircle(gamenHeight / 20, gamenHeight * 9 / 40, 10);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 9 / 40 + gamenHeight / 20, 5);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 9 / 40 + gamenHeight / 20 * 2, 3);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 9 / 40 + gamenHeight / 20 * 3, 1);

			#pragma endregion


			//ペン色ボタン
			#pragma region PenColor

			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);

			//黒
			ofFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight / 10, gamenHeight * 25 / 120, gamenHeight / 30, gamenHeight / 30);
		
			//灰
			ofFill();
			ofSetColor(150, 150, 150);
			ofDrawRectangle(gamenHeight / 10, gamenHeight * 25 / 120 + gamenHeight / 20, gamenHeight / 30, gamenHeight / 30);
		
			//白
			ofNoFill();
			ofSetColor(0, 0, 0);
			ofDrawRectangle(gamenHeight / 10, gamenHeight * 25 / 120 + gamenHeight / 20 * 2, gamenHeight / 30, gamenHeight / 30);
			ofFill();
			ofSetColor(255, 255, 255);
			ofDrawRectangle(gamenHeight / 10, gamenHeight * 25 / 120 + gamenHeight / 20 * 2, gamenHeight / 30, gamenHeight / 30);

			#pragma endregion

		#pragma endregion


		//消しゴムボタン
		#pragma region Eraser

		//消しゴム
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawCircle(gamenHeight / 10, gamenHeight / 2 - gamenHeight / 30, gamenHeight / 60 - 1);
		ofFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenHeight / 20, gamenHeight / 2 - gamenHeight / 20, gamenHeight / 20, gamenHeight / 30);


			//消しゴムサイズ
			#pragma region EraserSize

			ofNoFill();
			ofSetColor(0, 0, 0);

			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20);
			ofDrawRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20);

			ofDrawCircle(gamenHeight / 20, gamenHeight * 21 / 40, 10);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 21 / 40 + gamenHeight / 20, 5);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 21 / 40 + gamenHeight / 20 * 2, 3);
			ofDrawCircle(gamenHeight / 20, gamenHeight * 21 / 40 + gamenHeight / 20 * 3, 1);

			#pragma endregion

		#pragma endregion
	
	}

	#pragma endregion


	//動作設定シーン
	#pragma region Moveing

	if (sceneWhere == 5) {

		//木
		#pragma region Wood

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight / 2);

		//幹
		ofFill();
		ofSetColor(220, 220, 220);
		drawWood(0, -70, 0.55);

		ofPopMatrix();

		#pragma endregion


		//動作設定
		#pragma region Move

		ofFill();
		ofSetColor(0, 0, 0);

		for (int i = 0; i < moveCount; i++) {

			ofDrawRectangle(cPosX[drawCounter - 1][i] + gamenWidth / 2 - 1, cPosY[drawCounter - 1][i] + gamenHeight / 2 - 1, 2, 2);
		}

		imgMove.setAnchorPercent(0.5, 0.5);
		ofSetColor(255, 255, 255, 150);
		imgMove.draw(mouseX, mouseY, 200 * setSize[drawCounter - 1], 150 * setSize[drawCounter - 1]);

		#pragma endregion


		//ボード
		#pragma region Board

		ofPushMatrix();
		ofTranslate(gamenWidth / 2, gamenHeight / 2);
		ofScale(1, -1);

		//塗りつぶし
		ofFill();
		ofSetColor(255, 255, 255);
		ofDrawRectangle(-gamenWidth / 2, gamenHeight / 2, gamenWidth, 226 - gamenHeight / 2);
		ofDrawRectangle(-gamenWidth / 2, 226, gamenWidth / 2 - 300, -451);
		ofDrawRectangle(300, 226, gamenWidth / 2 - 300, -451);
		ofDrawRectangle(-gamenWidth / 2, -225, gamenWidth, 225 - gamenHeight / 2);

		//枠
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(-301, 226, 602, -452);

		ofPopMatrix();

		#pragma endregion
	

		//時間ゲージ
		#pragma region Time

		ofFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth / 2 - 300, gamenHeight - gamenHeight / 20, 600, gamenHeight / 60);
		ofSetColor(255, 255, 255);
		ofDrawRectangle((gamenWidth / 2 - 299) + (600 / (float)cMaxTime * moveCount), gamenHeight - gamenHeight / 20 + 1, 600 - (600 / (float)cMaxTime * moveCount) - 2, gamenHeight / 60 - 2);

		#pragma endregion


		//説明
		float wMoji = font.stringWidth("Draw character's moving.");
float hMoji = font.stringHeight("Draw character's moving.");
ofSetColor(0, 0, 0);
font.drawString("Draw character's moving.", gamenWidth / 2 - wMoji / 2, gamenHeight / 30 + hMoji / 2);


//戻るボタン
#pragma region Return

ofFill();
ofClickColor(ClickButton[2][0]);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);

ofNoFill();
ofSetColor(0, 0, 0);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);
ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight * 7 / 120), gamenHeight / 12);
ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 7 / 120);
ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 13 / 120);

#pragma endregion


//消去ボタン
#pragma region Delete

ofFill();
ofClickColor(ClickButton[2][1]);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15);

ofNoFill();
ofSetColor(0, 0, 0);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15);

ofFill();
ofSetColor(0, 0, 0);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 24), (gamenHeight / 20 + gamenHeight * 7 / 40), gamenHeight / 60, gamenHeight / 200);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 7 / 120), (gamenHeight / 20 + gamenHeight * 9 / 50), gamenHeight / 20, gamenHeight / 120);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight * 11 / 200), (gamenHeight / 20 + gamenHeight * 23 / 120), gamenHeight * 13 / 300, gamenHeight / 30);

#pragma endregion


//決定ボタン
#pragma region Enter

ofFill();
ofClickColor(ClickButton[2][2]);

if (DrawFinish == true) {

	ofSetColor(80, 80, 80, 255 * (0.5 * sin(alphaMoji) + 0.3));
}

ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15);

ofNoFill();
ofSetColor(0, 0, 0);
ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15);
ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12);
ofDrawLine(gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight - gamenHeight * 7 / 120);
ofDrawLine(gamenWidth - (gamenHeight * 7 / 120), gamenHeight - gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight - gamenHeight * 13 / 120);

#pragma endregion


//サイズボタン
#pragma region Size

ofFill();
ofClickColor(ClickButton[2][3]);
ofDrawRectangle(gamenHeight / 20, gamenHeight / 4, gamenHeight / 15, gamenHeight / 15);

ofFill();
ofClickColor(ClickButton[2][4]);
ofDrawRectangle(gamenHeight / 20, gamenHeight / 4 + gamenHeight / 15, gamenHeight / 15, gamenHeight / 15);;

ofNoFill();
ofSetColor(0, 0, 0);
ofDrawRectangle(gamenHeight / 20, gamenHeight / 4, gamenHeight / 15, gamenHeight / 15);
ofDrawRectangle(gamenHeight / 20, gamenHeight / 4 + gamenHeight / 15, gamenHeight / 15, gamenHeight / 15);
ofDrawTriangle(gamenHeight / 12, gamenHeight * 31 / 120, gamenHeight * 7 / 120, gamenHeight * 3 / 10, gamenHeight * 13 / 120, gamenHeight * 3 / 10);
ofDrawTriangle(gamenHeight / 15, gamenHeight / 3, gamenHeight / 10, gamenHeight / 3, gamenHeight / 12, gamenHeight * 11 / 30);

#pragma endregion

	}

#pragma endregion


	//ギャラリー
	#pragma region Gallery

	if (sceneWhere == 6) {

		//スクショ展示
		#pragma region Picture

		//枠
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12, gamenHeight / 2 - gamenHeight / 4, gamenHeight / 3, gamenHeight / 4);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12, gamenHeight / 2 - gamenHeight / 4, gamenHeight / 3, gamenHeight / 4);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12 * 2, gamenHeight / 2 - gamenHeight / 4, gamenHeight / 3, gamenHeight / 4);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12, gamenHeight / 2 + gamenHeight / 12, gamenHeight / 3, gamenHeight / 4);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12, gamenHeight / 2 + gamenHeight / 12, gamenHeight / 3, gamenHeight / 4);
		ofDrawRectangle(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12 * 2, gamenHeight / 2 + gamenHeight / 12, gamenHeight / 3, gamenHeight / 4);

		//画像
		int wI[6];
		int hI[6];

		for (int i = 0; i < 6; i++) {

			if(snapWidth[i] >= snapHeight[i]){

				wI[i] = ((gamenHeight / 3) / (float)snapWidth[i]) * (float)snapWidth[i];
				hI[i] = ((gamenHeight / 3) / (float)snapWidth[i]) * (float)snapHeight[i];
			}
			else {

				wI[i] = ((gamenHeight / 4) / (float)snapHeight[i]) * (float)snapWidth[i];
				hI[i] = ((gamenHeight / 4) / (float)snapHeight[i]) * (float)snapHeight[i];
			}
		}
	
		ofSetColor(255, 255, 255);
		galleryImg[0].draw(gamenWidth / 2 - gamenHeight * 7 / 12, gamenHeight / 2 - gamenHeight / 4, wI[0], hI[0]);
		galleryImg[1].draw(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12, gamenHeight / 2 - gamenHeight / 4, wI[1], hI[1]);
		galleryImg[2].draw(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12 * 2, gamenHeight / 2 - gamenHeight / 4, wI[2], hI[2]);
		galleryImg[3].draw(gamenWidth / 2 - gamenHeight * 7 / 12, gamenHeight / 2 + gamenHeight / 12, wI[3], hI[3]);
		galleryImg[4].draw(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12, gamenHeight / 2 + gamenHeight / 12, wI[4], hI[4]);
		galleryImg[5].draw(gamenWidth / 2 - gamenHeight * 7 / 12 + gamenHeight * 5 / 12 * 2, gamenHeight / 2 + gamenHeight / 12, wI[5], hI[5]);

		#pragma endregion


		//戻るボタン
		#pragma region Return

		ofFill();
		ofClickColor(ClickButton[3][0]);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);

		ofNoFill();
		ofSetColor(0, 0, 0);
		ofDrawRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight * 7 / 120), gamenHeight / 12);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 7 / 120);
		ofDrawLine(gamenWidth - (gamenHeight * 13 / 120), gamenHeight / 12, gamenWidth - (gamenHeight / 12), gamenHeight * 13 / 120);

		#pragma endregion

	}

	#pragma endregion

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//出現デバッグ用
	if (sceneWhere == 3) {

		if (key == 'c') {

			cin >> cNum[cImgCount];
			CharaPop = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
	//ホーム
	#pragma region Home

	if (sceneWhere == 1) {
		
		sceneWhere = 2;
		alphaMoji = 0;
		intro.setAnchorPercent(0.5, 0.5);

		//文字
		ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
		set.addRanges(ofAlphabet::Latin);
		font.load(set);
	}

	#pragma endregion


	//説明
	#pragma region Introduction

	else if (sceneWhere == 2) {

		sceneWhere = 3;
		alphaMoji = 0;

		//文字
		ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
		set.addRanges(ofAlphabet::Latin);
		font.load(set);
	}

	#pragma endregion


	//鑑賞シーン
	#pragma region Main

	else if (sceneWhere == 3) {

		//タブが開いてる
		#pragma region Tab

		if (MenuTab == true) {

			//タブ1をクリック
			#pragma region Tab1

			if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), gamenHeight / 20, gamenHeight / 4, gamenHeight / 12, x, y)) {

				sceneWhere = 4;
				ofSetBackgroundAuto(false);
				BackReset = true;
				ofBackground(200, 200, 200);
				ofSetFrameRate(60);
				player.stop();
				MenuTab = false;

				//再計算用
				scene3Bef = gamenHeight;
			}

			#pragma endregion


			//タブ2をクリック
			#pragma region Tab2

			else if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 4, gamenHeight / 12, x, y)) {

				sceneWhere = 6;
				MenuTab = false;

				//新しい画像6つをロード
				int snapNum = snapCounter;

				for (int i = 0; i < 6; i++) {

					sprintf(fileName, "snapshot_%.2u.jpg", snapNum);
					galleryImg[i].load(fileName);
					snapWidth[i] = galleryImg[i].getWidth();
					snapHeight[i] = galleryImg[i].getHeight();
					snapNum--;
				}

				//再計算用
				scene3Bef = gamenHeight;
			}

			#pragma endregion


			//タブ3をクリック
			#pragma region Tab3

			else if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 4), (gamenHeight / 20 + gamenHeight / 12 * 2), gamenHeight / 4, gamenHeight / 12, mouseX, mouseY)) {

				setup();
			}

			#pragma endregion

		}

		#pragma endregion


		//メニュー
		#pragma region Menu

		else if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 20, gamenHeight / 12, gamenHeight / 12, x, y)) {

			ClickButton[0][0] = true;
		}
		else {

			ClickButton[0][0] = false;
		}

		#pragma endregion


		//カメラ
		#pragma region Camera

		if (InRectangle(gamenHeight / 20, gamenHeight / 20, gamenHeight / 12, gamenHeight / 12, x, y)) {

			ClickButton[0][1] = true;
		}
		else {

			ClickButton[0][1] = false;
		}

		#pragma endregion

	}

	#pragma endregion


	//お絵描きシーン
	#pragma region Drawing

	else if (sceneWhere == 4) {

		//キャンバス内
		#pragma region Canvas

		if (InRectangle(gamenWidth / 2 - 300, gamenHeight / 2 - 225, 600, 450, x, y)) {

			DrawIllust = true;
			DrawFinish = true;
		}

		#pragma endregion


		//戻るボタン
		#pragma region Return

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[1][0] = true;
		}
		else {

			ClickButton[1][0] = false;
		}

		#pragma endregion 


		//消去ボタン
		#pragma region Delete

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[1][1] = true;
		}
		else {

			ClickButton[1][1] = false;
		}

		#pragma endregion


		//決定ボタン
		#pragma region Enter

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[1][2] = true;
		}
		else {

			ClickButton[1][2] = false;
		}

		#pragma endregion
	

		//ペンサイズボタン
		#pragma region PenSize

		if (InRectangle(gamenHeight / 40, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20, x, y)) {
		
			penType = 1;
			penSize = 20;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 1;
			penSize = 10;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20, x ,y)) {

			penType = 1;
			penSize = 5;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 5 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 1;
			penSize = 1;
		}

		#pragma endregion 


		//ペン色ボタン
		#pragma region PenColor

		if (InRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 1;
			penColor = 0;
		}
		else if (InRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 1;
			penColor = 150;
		}
		else if (InRectangle(gamenHeight / 40 + gamenHeight / 15, gamenHeight / 5 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 1;
			penColor = 255;
		}

		#pragma endregion 


		//消しゴムボタン
		#pragma region Eraser

		if (InRectangle(gamenHeight / 40, gamenHeight / 2, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 2;
			eraserSize = 20;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 2;
			eraserSize = 10;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 2, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 2;
			eraserSize = 5;
		}
		else if (InRectangle(gamenHeight / 40, gamenHeight / 2 + gamenHeight / 20 * 3, gamenHeight / 20, gamenHeight / 20, x, y)) {

			penType = 2;
			eraserSize = 1;
		}

		#pragma endregion

	}

	#pragma endregion


	//動作設定シーン
	#pragma region Moveing

	else if (sceneWhere == 5) {

		//キャンバス内
		#pragma region Canvas

		if (InRectangle(gamenWidth / 2 - 300, gamenHeight / 2 - 225, 600, 450, x, y)) {

			DrawMove = true;
			DrawFinish = true;
		}

		#pragma endregion	


		//戻るボタン
		#pragma region Return

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[2][0] = true;
		}
		else {

			ClickButton[2][0] = false;
		}

		#pragma endregion 


		//消去ボタン
		#pragma region Delete

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[2][1] = true;
		}
		else {

			ClickButton[2][1] = false;
		}

		#pragma endregion


		//決定ボタン
		#pragma region Enter

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[2][2] = true;
		}
		else {

			ClickButton[2][2] = false;
		}

		#pragma endregion


		//サイズボタン
		#pragma region Size

		//大
		if (InRectangle(gamenHeight / 20, gamenHeight / 4, gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[2][3] = true;
		}
		else {

			ClickButton[2][3] = false;
		}

		//小
		if (InRectangle(gamenHeight / 20, gamenHeight / 4 + gamenHeight / 15, gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[2][4] = true;
		}
		else {

			ClickButton[2][4] = false;
		}

		#pragma endregion

	}

	#pragma endregion


	//ギャラリー
	#pragma region Gallery

	else if (sceneWhere == 6) {

		//戻るボタン
		#pragma region Return

		if (InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			ClickButton[3][0] = true;
		}
		else {

			ClickButton[3][0] = false;
		}

		#pragma endregion 

	}

	#pragma endregion

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	//鑑賞シーン
	#pragma region Main

	if (sceneWhere == 3) {

		//メニュー
		if (ClickButton[0][0] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 12), gamenHeight / 20, gamenHeight / 12, gamenHeight / 12, x, y)) {

			MenuTab = true;
			
		}
		else {

			MenuTab = false;
		}

		ClickButton[0][0] = false;

		//カメラ
		if (ClickButton[0][1] == true && InRectangle(gamenHeight / 20, gamenHeight / 20, gamenHeight / 12, gamenHeight / 12, x, y)) {

			TakeSnap = true;
		}

		ClickButton[0][1] = false;

	}

	#pragma endregion


	//お絵描きシーン
	#pragma region Drawing

	else if (sceneWhere == 4) {

		DrawIllust = false;

		//戻るボタン
		#pragma region Return

		if (ClickButton[1][0] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			sceneWhere = 3;
			DrawFinish = false;
			alphaMoji = 0;
			ofSetBackgroundAuto(true);
			ofBackground(255, 255, 255);
			ofSetFrameRate(30);
			player.play();

			//文字
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);

			//葉再計算
			if (scene3Bef != gamenHeight) {

				for (int i = 0; i < LEAFNUM; i++) {

					xLeaf[i] = xLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
					yLeaf[i] = yLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
				}
			}
		}

		ClickButton[1][0] = false;

		#pragma endregion


		//消去ボタン
		#pragma region Delete

		if (ClickButton[1][1] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15, x, y)) {

			BackReset = true;
		}
		
		ClickButton[1][1] = false;

		#pragma endregion
	
	
		//決定ボタン
		#pragma region Enter

		if (ClickButton[1][2] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15, x, y)) {

			//画像出力
			#pragma region Take

			//撮影
			glReadBuffer(GL_FRONT);
			imgOrg.grabScreen(gamenWidth / 2 - 300, gamenHeight / 2 - 225, 600, 450);

			//ピクセル用設定
			unsigned char * pixels;
			unsigned char * pixels2;
			pixels = (unsigned char*)malloc(600 * 450 * 4);
			pixels2 = (unsigned char*)malloc(600 * 450 * 4);

			//ピクセル記録
			pixels = imgOrg.getPixels().getData();

			//画像の幅
			int widthOfLine = 600;

			for (int y = 0; y < 450; y++) {

				for (int x = 0; x < 600; x++) {

					int red = pixels[(widthOfLine * y + x) * 4];
					int green = pixels[(widthOfLine * y + x) * 4 + 1];
					int blue = pixels[(widthOfLine * y + x) * 4 + 2];

					pixels2[(widthOfLine * y + x) * 4] = blue;
					pixels2[(widthOfLine * y + x) * 4 + 1] = green;
					pixels2[(widthOfLine * y + x) * 4 + 2] = red;

					//アルファを変更
					if (red == 200 && green == 200 && blue == 200) {

						pixels2[(widthOfLine * y + x) * 4 + 3] = 0;
					}
					else {

						pixels2[(widthOfLine * y + x) * 4 + 3] = 255;
					}

				}
			}

			//アルファ用に格納して構成
			imgCmp.setFromPixels(pixels2, 600, 450, OF_IMAGE_COLOR_ALPHA);

			//出力
			sprintf(fileName, "picCreature_%.2u.png", drawCounter);
			imgCmp.saveImage(fileName);

			#pragma endregion 


			imgMove.load(fileName);
			drawCounter++;
			DrawFinish = false;
			alphaMoji = 0;
			sceneWhere = 5;
			ofSetBackgroundAuto(true);
			ofSetFrameRate(30);
		}

		ClickButton[1][2] = false;

		#pragma endregion
	
	}

	#pragma endregion


	//動作設定シーン
	#pragma region Moving

	else if (sceneWhere == 5) {

		//戻るボタン
		#pragma region Return

		if (ClickButton[2][0] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			drawCounter--;
			sceneWhere = 4;
			DrawFinish = false;
			alphaMoji = 0;
			ofSetBackgroundAuto(false);
			BackReset = true;
			ofSetFrameRate(60);
		}

		ClickButton[2][0] = false;

		#pragma endregion


		//消去ボタン
		#pragma region Delete

		if (ClickButton[2][1] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), (gamenHeight / 20 + gamenHeight / 6), gamenHeight / 15, gamenHeight / 15, x, y)) {

			for (int i = 0; i < moveCount; i++) {

				cPosX[drawCounter - 1][i] = 0;
				cPosY[drawCounter - 1][i] = 0;
			}

			moveCount = 0;
		}

		ClickButton[2][1] = false;

		#pragma endregion


		//決定ボタン
		#pragma region Enter

		if (ClickButton[2][2] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 15, gamenHeight / 15, x, y)) {

			sceneWhere = 3;
			moveCount = 0;
			DrawFinish = false;
			alphaMoji = 0;
			ofSetBackgroundAuto(true);
			ofBackground(255, 255, 255);
			ofSetFrameRate(30);
			player.play();

			//文字
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);

			//葉再計算
			if (scene3Bef != gamenHeight) {

				for (int i = 0; i < LEAFNUM; i++) {

					xLeaf[i] = xLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
					yLeaf[i] = yLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
				}
			}
		}

		ClickButton[2][2] = false;

		#pragma endregion


		//サイズボタン
		#pragma region Size

		//大
		if (ClickButton[2][3] == true && InRectangle(gamenHeight / 20, gamenHeight / 4, gamenHeight / 15, gamenHeight / 15, x, y)) {

			setSize[drawCounter - 1] *= 1.1;
		}

		ClickButton[2][3] = false;

		//小
		if (ClickButton[2][4] == true && InRectangle(gamenHeight / 20, gamenHeight / 4 + gamenHeight / 15, gamenHeight / 15, gamenHeight / 15, x, y)) {

			setSize[drawCounter - 1] *= 0.9;
		}

		ClickButton[2][4] = false;

		#pragma endregion


		DrawMove = false;
	}

	#pragma endregion


	//ギャラリー
	#pragma region Gallery

	else if (sceneWhere == 6) {

		//戻るボタン
		#pragma region Return

		if (ClickButton[3][0] == true && InRectangle(gamenWidth - (gamenHeight / 20 + gamenHeight / 15), gamenHeight / 20, gamenHeight / 15, gamenHeight / 15, x, y)) {

			sceneWhere = 3;

			//文字
			ofTrueTypeFontSettings set("cambria.ttc", (gamenHeight / 30));
			set.addRanges(ofAlphabet::Latin);
			font.load(set);

			//葉再計算
			if (scene3Bef != gamenHeight) {

				for (int i = 0; i < LEAFNUM; i++) {

					xLeaf[i] = xLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
					yLeaf[i] = yLeaf[i] / (scene3Bef / 800) * (gamenHeight / 800);
				}
			}
		}

		ClickButton[3][0] = false;

		#pragma endregion

	}

	#pragma endregion

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::drawWood(float x, float y, float size) {

	ofPushMatrix();
	ofScale(1, -1);

	ofBeginShape();
		ofVertex(x + 30 * size, y - 250 * size);
		ofVertex(x + 35 * size, y - 230 * size);
		ofVertex(x + 45 * size, y - 220 * size);
		ofVertex(x + 45 * size, y - 220 * size);
		ofVertex(x + 100 * size, y - 240 * size);
		ofVertex(x + 60 * size, y - 200 * size);//根
		ofVertex(x + 35 * size, y - 150 * size);
		ofVertex(x + 35 * size, y - 20 * size);//幹
		ofVertex(x + 148 * size, y + 38 * size);
		ofVertex(x + 208 * size, y + 50 * size);
		ofVertex(x + 228 * size, y + 35 * size);
		ofVertex(x + 216 * size, y + 51 * size);
		ofVertex(x + 250 * size, y + 60 * size);
		ofVertex(x + 145 * size, y + 50 * size);
		ofVertex(x + 160 * size, y + 90 * size);
		ofVertex(x + 130 * size, y + 48 * size);
		ofVertex(x + 35 * size, y + 20 * size);//枝右
		ofVertex(x + 30 * size, y + 30 * size);
		ofVertex(x + 30 * size, y + 115 * size);//幹
		ofVertex(x + 47 * size, y + 135 * size);
		ofVertex(x + 95 * size, y + 122 * size);
		ofVertex(x + 140 * size, y + 125 * size);
		ofVertex(x + 105 * size, y + 130 * size);
		ofVertex(x + 100 * size, y + 132 * size);
		ofVertex(x + 112 * size, y + 155 * size);
		ofVertex(x + 90 * size, y + 135 * size);
		ofVertex(x + 60 * size, y + 145 * size);
		ofVertex(x + 65 * size, y + 150 * size);
		ofVertex(x + 110 * size, y + 240 * size);
		ofVertex(x + 50 * size, y + 160 * size);
		ofVertex(x, y + 120 * size);//枝右上
		ofVertex(x - 15 * size, y + 125 * size);
		ofVertex(x - 50 * size, y + 185 * size);
		ofVertex(x - 76 * size, y + 250 * size);
		ofVertex(x - 62 * size, y + 180 * size);
		ofVertex(x - 100 * size, y + 190 * size);
		ofVertex(x - 60 * size, y + 170 * size);
		ofVertex(x - 40 * size, y + 120 * size);//枝左上
		ofVertex(x - 30 * size, y + 78 * size);
		ofVertex(x - 100 * size, y + 115 * size);
		ofVertex(x - 120 * size, y + 148 * size);
		ofVertex(x - 115 * size, y + 125 * size);
		ofVertex(x - 190 * size, y + 170 * size);
		ofVertex(x - 130 * size, y + 118 * size);
		ofVertex(x - 155 * size, y + 113 * size);
		ofVertex(x - 110 * size, y + 105 * size);
		ofVertex(x - 32 * size, y + 57 * size);
		ofVertex(x - 35 * size, y + 25 * size);//枝真ん中
		ofVertex(x - 40 * size, y + 20 * size);
		ofVertex(x - 140 * size, y + 28 * size);
		ofVertex(x - 155 * size, y + 65 * size);
		ofVertex(x - 150 * size, y + 30 * size);
		ofVertex(x - 250 * size, y + 45 * size);
		ofVertex(x - 148 * size, y + 15 * size);
		ofVertex(x - 60 * size, y);
		ofVertex(x - 35 * size, y - 15 * size);//枝左下
		ofVertex(x - 35 * size, y - 140 * size);
		ofVertex(x - 50 * size, y - 185 * size);//幹
		ofVertex(x - 85 * size, y - 240 * size);
		ofVertex(x - 25 * size, y - 220 * size);
		ofVertex(x + 30 * size, y - 250 * size);//根
	ofEndShape();

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawLeaf(float x, float y, float rotate, float size) {
	
	float xOnLine = 0;
	float yOnLine = 0;

	ofPushMatrix();
	ofTranslate(x, y);
	ofRotateZDeg(rotate);
	ofScale(size, size);

	ofBeginShape();

		for (int theta = 90; theta <= 450; theta += 10) {
			
			yOnLine = 50 * sin(ofDegToRad(theta));

			if(40 + 50 * cos(ofDegToRad(theta)) <= 0){
				xOnLine = 40 + 60 * cos(ofDegToRad(theta));
				ofVertex(xOnLine, yOnLine);
			}

			if (theta == 270) {
				ofVertex(0, -36);
			}

			if (-40 + 50 * cos(ofDegToRad(theta)) > 0) {
				xOnLine = -40 + 60 * cos(ofDegToRad(theta));
				ofVertex(xOnLine, yOnLine);
			}

			if (theta == 450) {
				ofVertex(0, 36);
			}
		}

	ofEndShape();

	ofPopMatrix();
}

//--------------------------------------------------------------
bool ofApp::InCircle(float xCenter, float yCenter, float radius, float xPoint, float yPoint) {

	if ((xPoint - xCenter)*(xPoint - xCenter) + (yPoint - yCenter)*(yPoint - yCenter) <= radius * radius) {
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------
bool ofApp::InRectangle(float x, float y, float haba, float takasa, float xPoint, float yPoint) {

	if ((x <= xPoint && xPoint <= x + haba) && (y <= yPoint && yPoint <= y + takasa)) {
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------
float ofApp::myNoise(float lower, float upper, float arg1, float arg2, float arg3) {
	
	float dif = upper - lower;
	float result = 0;

	if (dif < 0) {
		dif *= -1;
	}

	if (arg3 != 0) {
		result = dif * ofNoise(arg1, arg2, arg3) + lower;
	}
	else if (arg2 != 0) {
		result = dif * ofNoise(arg1, arg2) + lower;
	}
	else {
		result = dif * ofNoise(arg1) + lower;
	}

	return result;
}

//--------------------------------------------------------------
void ofApp::ofSetHSVColor(int hue, int sat, int val, int alp) {

	float s = (float)sat / 100, v = (float)val / 100;
	
	if (s == 0) {
		ofSetColor(255 * v, 255 * v, 255 * v);
		return;
	}

	float r, g, b;
	int hi = (int)(floor((double)hue / 60)) % 6;
	float f = (float)hue / 60 - (float)hi;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f)*s);
	
	switch (hi) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		case 5:
			r = v;
			g = p;
			b = q;
			break;
		default:
			break;
	}
	ofSetColor(255 * r, 255 * g, 255 * b, alp);
}

//--------------------------------------------------------------
void ofApp::ofClickColor(bool Press) {

	if (Press == true) {

		ofSetColor(200, 200, 200);
	}
	else {

		ofSetColor(255, 255, 255);
	}
}

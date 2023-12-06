#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void drawWood(float x, float y, float size);
		void drawLeaf(float x, float y, float rotate, float size);
		bool InCircle(float xCenter, float yCenter, float radius, float xPoint, float yPoint);
		bool InRectangle(float x, float y, float haba, float takasa, float xPoint, float yPoint);
		float myNoise(float lower, float upper, float arg1, float arg2 = 0, float arg3 = 0);
		void ofSetHSVColor(int hue, int sat, int val, int alp = 255);
		void ofClickColor(bool Press);

		ofTrueTypeFont font;
		ofImage title;
		ofImage intro;
		ofImage imgOrg;
		ofImage imgCmp;
		ofImage imgMove;
		ofImage cImg[10];
		ofImage imgSnap;
		ofImage galleryImg[6];
		ofPixels pixels;
		ofSoundPlayer player;
};

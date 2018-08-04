#pragma once

#include "ofMain.h"
#include "ofxBenG.h"

class ofApp : public ofBaseApp {
public:
    ofApp();~ofApp();
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
	void audioOut(float* output, int bufferSize, int nChannels);

	// App
	void log(float beat, const string &message) const;
    void onTelephoneStarted();
    float startBeat = -1;
    ofxBenG::telephone* telephone = nullptr;
	ofxBenG::syphon* syphon;
    ofxBenG::twister* twister;
    ofxBenG::playmodes* playModes;
	ofxBenG::timeline* timeline = nullptr;
    ofxBenG::property_bag propertyBag;
    ofxBenG::ableton* ableton;
    ofxBenG::audio* audio;
	ofxMaxiSample* sample;
    ofxMaxiSample* click;
    ofxBenG::property<float> beatsPerMinute = {"beatsPerMinute", 60, 0, 480};
    ofxBenG::property<float> loopLengthSeconds = {"loopLengthSeconds", 15.0, 0.0, 16.0};
    ofxBenG::property<float> videoHeaderY = {"videoHeaderY", 0.0, 0.0, ofGetHeight() / 2};
	ofxBenG::property<float> volume = {"videoHeaderY", 0.0, 0.0, ofGetHeight() / 2};
    static float constexpr width = 1280;
    static float constexpr height = width / (1920.0/1080.0);
    bool inFullscreen = false;
    ofTrueTypeFont myFont;
};

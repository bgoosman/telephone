#include "ofApp.h"

ofApp::ofApp() {}
ofApp::~ofApp() {
    ofSoundStreamClose();
    propertyBag.saveToXml();
    delete ableton;
    delete twister;
    delete timeline;
    delete audio;
}

void ofApp::setup() {
    myFont.load("open-sans/OpenSans-Regular.ttf", 64);
    ableton = new ofxBenG::ableton();
    ableton->setupLink(beatsPerMinute, 8.0);
    const float desiredWidth = ofApp::width;
    const float desiredHeight = ofApp::height;
    const int defaultBufferSize = 450;
    playModes = new ofxBenG::playmodes(ofxBenG::playmodes::c615, desiredWidth, desiredHeight, 30, defaultBufferSize);
    ofSetWindowShape(playModes->getWidth(), playModes->getHeight());
    beatsPerMinute.addSubscriber([&]() { ableton->setBeatsPerMinute((int)beatsPerMinute); });
    audio = new ofxBenG::audio();
    ofSoundStreamSetup(2, 0, this, ofxBenG::audio::rate, ofxBenG::audio::bufferSize, 4);
    sample = new ofxMaxiSample();
    sample->load(ofToDataPath("26777__junggle__btn402.wav"));
    click = new ofxMaxiSample();
    click->load(ofToDataPath("320181__dland__hint.wav"));
    timeline = new ofxBenG::timeline();
    syphon = new ofxBenG::syphon(playModes->getBufferCount());
    propertyBag.add(CAST_PROPERTY(beatsPerMinute));
    propertyBag.add(CAST_PROPERTY(loopLengthSeconds));
    propertyBag.add(CAST_PROPERTY(videoHeaderY));
    propertyBag.add(CAST_PROPERTY(audio->getVolumeReference()));
    propertyBag.loadFromXml();
    twister = new ofxBenG::twister();
    twister->bindToMultipleEncoders(&propertyBag);
}

void ofApp::update() {
    float const currentBeat = ableton->getBeat();
    propertyBag.update();

    if (!playModes->isInitialized()) {
        playModes->setup();
    }

    if (playModes->isInitialized()) {
        playModes->update();
//        for (int i = 0; i < playModes->getBufferCount(); i++) {
//            syphon->publishTexture(i, &playModes->getBufferTexture(i));
//        }
    }

    timeline->update(currentBeat);
}

void ofApp::draw() {
    ofBackground(0);

    if (playModes->isInitialized()) {
        if (telephone != nullptr) {
            auto scaledWidth = ofGetWidth() / 2;
            auto scaledHeight = scaledWidth * (9.0 / 16.0);

            int recordingPlusOne = (telephone->getRecordingIndex() + playModes->getBufferCount() - 1) % playModes->getBufferCount();
            playModes->drawHeader(recordingPlusOne, scaledWidth, videoHeaderY, -scaledWidth, scaledHeight);

            int recordingPlusTwo = (telephone->getRecordingIndex() + playModes->getBufferCount() - 2) % playModes->getBufferCount();
            playModes->drawHeader(recordingPlusTwo, ofGetWidth(), videoHeaderY, -scaledWidth, scaledHeight);

            if (startBeat >= 0.0) {
                float second = (ableton->getBeat() - startBeat) * (1 / beatsPerMinute) * 60;
                std::string s = ofToString(second);
                //myFont.drawString(s, ofGetWidth() / 2 + ofGetWidth() / 4, ofGetHeight() / 2 + ofGetHeight() / 4);
            }
        }
    }

    if (!inFullscreen) {
        float y = 15;
        ofxBenG::utilities::drawLabelValue("beat", ableton->getBeat(), y);
        ofxBenG::utilities::drawLabelValue("bpm", ableton->getTempo(), y += 20);
        ofxBenG::utilities::drawLabelValue("fps", ofGetFrameRate(), y += 20);
        ofxBenG::utilities::drawLabelValue("loopLengthSeconds", loopLengthSeconds, y += 20);
        ofxBenG::utilities::drawLabelValue("videoHeaderY", videoHeaderY, y += 20);
        ofxBenG::utilities::drawLabelValue("volume", audio->getVolume(), y += 20);
        ofxBenG::utilities::drawLabelValue("[0] fps", playModes->getBuffer(0)->getFps(), y += 20);
        ofxBenG::utilities::drawLabelValue("[0] real fps", playModes->getBuffer(0)->getRealFPS(), y += 20);
        ofxBenG::utilities::drawLabelValue("[1] fps", playModes->getBuffer(1)->getFps(), y += 20);
        ofxBenG::utilities::drawLabelValue("[1] real fps", playModes->getBuffer(1)->getRealFPS(), y += 20);
        if (telephone != nullptr) {
            ofxBenG::utilities::drawLabelValue("recording", (float)telephone->getRecordingIndex(), y += 20);
        }
    }
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++) {
        float mix = audio->getMix();
        output[nChannels * i] = mix;
        output[nChannels * i + 1] = mix;
    }
}

void ofApp::onTelephoneStarted() {
    startBeat = ableton->getBeat();
}

void ofApp::log(float beat, const string &message) const {
    cout << beat << ": " << message << endl;
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {
    if (key == 'f') {
        ofToggleFullscreen();
        inFullscreen = !inFullscreen;
    }

    if (key == 's') {
        audio->playSample(sample);
    }

    if (key == 'd') {
        audio->playSample(click);
    }

    if (key == ' ') {
        auto nextWholeBeat = ableton->getNextWholeBeat();
        telephone = new ofxBenG::telephone(nextWholeBeat, beatsPerMinute, loopLengthSeconds, playModes, audio, sample, click);
        ofAddListener(telephone->onTelephoneStarted, this, &ofApp::onTelephoneStarted);
        timeline->schedule(telephone, ableton->getBeat(), nextWholeBeat - ableton->getBeat());
    }
}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

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

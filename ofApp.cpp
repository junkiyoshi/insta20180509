#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(239);
	ofSetWindowTitle("Insta");

	// Initialize ofxBox2d
	this->box2d.init();
    this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	// Add Circle
	for (int i = 0; i < 256; i++) {

		float radius = ofRandom(8.0, 15.0);
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(1.0, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), radius);

		ofColor color;
		color.setHsb(ofRandom(255), 239, 239);

		this->circles.push_back(make_pair(circle, color));
	}

	this->force_field_radius = 200;
}

//--------------------------------------------------------------
void ofApp::update() {

	// Field of Force on Center
	for (int i = 0; i < this->circles.size(); i++) {

		shared_ptr<ofxBox2dCircle> circle = get<0>(this->circles[i]);
		ofPoint point = get<0>(this->circles[i])->getPosition();

		float distance = point.distance(ofPoint(ofGetWidth() / 2, ofGetHeight() / 2));
		if(distance < this->force_field_radius) {

			ofPoint p = ofPoint(this->force_field_radius * cos(ofGetFrameNum() * DEG_TO_RAD), this->force_field_radius * sin(ofGetFrameNum() * DEG_TO_RAD));
			circle->addForce(p, ofMap(distance, 0, 200, 10, 1));
		}
	}

	// Compute box2d
	this->box2d.update();
}


//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Filed of Force
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	ofNoFill();
	ofSetColor(39);
	ofSetLineWidth(0.5);
	ofBeginShape();
	for (int deg = 0; deg < 360; deg += 1) {

		ofVertex(this->force_field_radius * cos(deg * DEG_TO_RAD), this->force_field_radius * sin(deg * DEG_TO_RAD));
	}
	ofEndShape(true);

	ofFill();
	ofBeginShape();
	for (int deg = ofGetFrameNum(); deg < ofGetFrameNum() + 360; deg += 120) {

		ofVertex(this->force_field_radius * cos(deg * DEG_TO_RAD), this->force_field_radius * sin(deg * DEG_TO_RAD));
	}
	ofEndShape(true);

	ofPopMatrix();

	// Draw Circles
	ofSetLineWidth(3);
	for (int i = 0; i < this->circles.size(); i++) {

		ofSetColor(get<1>(this->circles[i]));
		float radius = get<0>(this->circles[i])->getRadius();
		ofPoint point = get<0>(this->circles[i])->getPosition();

		ofDrawCircle(point, radius);
		ofDrawLine(point, point - get<0>(this->circles[i])->getVelocity());
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
#pragma once

#include "ofMain.h"
#include "Offence.hpp"
#include "Defence.hpp"
#define OFFENCE_NUM 6
#define DEFENCE_NUM 10
#define MAX_GENERATION 100

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
    void setupPeople();
    void restart();
    void inheritGene();
    //void inheritGene_d();
    void updateDefence();
    void ballHitTo(int index);
    void updateOffence();
    void letThrowBall(int thrower);
    void setConditionOf(int off_index);
    void mutate();
    void drawGraph();
    void drawDef();
    void drawOff();
    void drawBackground();
    
    int getClosestOffence();
    int selectParent();
    float getDistanceBetweenPoints(ofVec2f point1, ofVec2f point2);
    bool doesExistLeftOf(int defIndex, int offIndex);
    bool doesExistFrontOf(int defIndex, int offIndex);
    
    Offence offence[OFFENCE_NUM];
    Defence defence[DEFENCE_NUM];
};

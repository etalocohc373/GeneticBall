#include "ofApp.h"

int ballThrower = 0;
bool didKill = false;
ofVec2f ballPosition = ofVec2f(0, 0);
ofVec2f ballSpeed = ofVec2f(0, 0);
float fromTheta = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetCircleResolution(32);
    ofSetFrameRate(20);
    for (int i = 0; i < OFFENCE_NUM; i++) offence[i].init();
    
    int first = rand() % OFFENCE_NUM;
    offence[first].haveBall = true;
    offence[first].denomi++;
    
    for (int i = 0; i < DEFENCE_NUM; i++) defence[i].init();
}

//--------------------------------------------------------------
void ofApp::update(){
    bool ballCaught = ballCaughtWhenUpdatedOffence();
    bool defeated = true;
    for (int i = 0; i < DEFENCE_NUM; i++){
        defence[i].update(fromTheta);
        if (getDistanceBetweenPoints(defence[i].pos, ballPosition) < 18 && !defence[i].isDead && !didKill){
            defence[i].isDead = true;
            didKill = true;
            offence[ballThrower].score++;
            cout << ballThrower << ": " << float(offence[ballThrower].score) / float(offence[ballThrower].denomi) << endl;
        }
        if (!defence[i].isDead) defeated = false;
    }
    if (defeated){
        inheritGene();
        defeated = false;
        return;
    }
    
    if (getDistanceBetweenPoints(ballPosition, ofVec2f(0, 0)) > 250 + 12)
        offence[getClosestOffence()].haveBall = true;
    
    if (!ballCaught) ballPosition += ballSpeed;
}

bool ofApp::ballCaughtWhenUpdatedOffence(){
    bool ballCaught = false;
    for (int i = 0; i < OFFENCE_NUM; i++){
        if (offence[i].haveBall){
            int leftCount = 0;
            for (int j = 0; j < DEFENCE_NUM; j++){
                if (doesExistLeftOf(j, i)) leftCount++;
            }
            //cout << leftCount << endl;
            if (leftCount > DEFENCE_NUM / 2) offence[i].condition = 0;
            if (leftCount == DEFENCE_NUM / 2) offence[i].condition = 1;
            if (leftCount < DEFENCE_NUM / 2) offence[i].condition = 2;
            
            ballCaught = true;
            ballPosition = offence[i].pos;
            fromTheta = offence[i].theta2;
        }
        if (offence[i].willThrowBall){
            ballThrower = i;
            ballPosition = offence[i].pos;
            offence[i].willThrowBall = false;
            didKill = false;
            offence[i].denomi++;
            
            /*int aim = rand() % DEFENCE_NUM;
             while (defence[aim].isDead && !defeated) aim = rand() % DEFENCE_NUM;
             
             ofVec2f ballVec = ofVec2f(defence[aim].pos.x - ballPosition.x, defence[aim].pos.y - ballPosition.y);
             float scalar = pow(pow(double(ballVec.x), 2) + pow(double(ballVec.y), 2), 0.5);
             ballVec = ofVec2f(ballVec.x /= (scalar / offence[i].ballSpeed), ballVec.y /= (scalar / offence[i].ballSpeed));
             ballSpeed = ballVec;*/ // 狙うやつ
            ballSpeed = ofVec2f(-offence[i].pos.x / offence[i].ballSpeed, -offence[i].pos.y / offence[i].ballSpeed);
        }
        offence[i].update();
    }
    return ballCaught;
}

void ofApp::inheritGene(){
    sort(offence, offence + OFFENCE_NUM, Offence());
    
    int parent1 = selectParent();
    int parent2 = selectParent();
    while (parent2 == parent1) parent2 = selectParent();
    float gene[3][GENE_NUM];
    for (int j = 0; j < 3; j++){
        for (int i = 0; i < GENE_NUM; i++){
            if (i < GENE_NUM / 2) gene[j][i] = offence[parent1].gene[j][i];
            else gene[j][i] = offence[parent2].gene[j][i];
        }
    }
    
    didKill = false;
    fromTheta = 0;
    for (int i = 0; i < OFFENCE_NUM; i++) offence[i].initWithGene(offence[i].gene);
    offence[OFFENCE_NUM - 1].initWithGene(gene);
    int first = rand() % OFFENCE_NUM;
    offence[first].haveBall = true;
    offence[first].denomi++;
    for (int i = 0; i < DEFENCE_NUM; i++) defence[i].init();
    cout << "success: " << parent1 << ", " << parent2 << endl;
}

int ofApp::selectParent(){
    int roulette = rand() % 100;
    int selectNum = 0;
    if (roulette >= 50) selectNum = 0;
    else if (roulette >= 25) selectNum = 1;
    else if (roulette >= 12) selectNum = 2;
    else if (roulette >= 5) selectNum = 3;
    else /*if (roulette >= 1)*/ selectNum = 4;
    //else selectNum = 5;
    
    return selectNum;
}

bool ofApp::doesExistLeftOf(int defIndex, int offIndex){
    bool result = false;
    if (offence[offIndex].theta2 < pi){
        if (offence[offIndex].theta2 > defence[defIndex].theta || offence[offIndex].theta2 + 2 * pi < defence[defIndex].theta) result = true;
    }else{
        if (offence[offIndex].theta2 - pi < defence[defIndex].theta && offence[offIndex].theta2 > defence[defIndex].theta) result = true;
    }
    if (defence[defIndex].isDead) result = false;
    
    
    return result;
}

float ofApp::getDistanceBetweenPoints(ofVec2f point1, ofVec2f point2){
    float result = pow(double(pow(double(point1.x - point2.x), 2) + pow(double(point1.y- point2.y), 2)), 0.5);
    return result;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofSetColor(255);
    ofNoFill();
    ofCircle(0, 0, 250);
    
    ofFill();
    for (int i = 0; i < OFFENCE_NUM; i++){
        ofSetColor(255);
        if (offence[i].haveBall) ofSetColor(255, 50, 50);
        ofCircle(offence[i].pos, 10);
    }
    
    ofSetColor(0, 255, 0);
    for (int i = 0; i < DEFENCE_NUM; i++)
        if (!defence[i].isDead) {
            if (defence[i].isEscaping) ofSetColor(255, 0, 0);
            else ofSetColor(0, 255, 0);
            ofCircle(defence[i].pos, 10);
            //ofDrawBitmapStringHighlight(ofToString(defence[i].acc_theta), defence[i].pos);
            //ofLine(defence[i].pos,defence[i].pos+20*ofVec2f(cos(defence[i].acc_theta),sin(defence[i].acc_theta)));
        }
    
    ofSetColor(0, 0, 255);
    ofCircle(ballPosition, 8);
}

int ofApp::getClosestOffence(){
    float distance = 800;
    int closest = 0;
    for (int i = 0; i < OFFENCE_NUM; i++){
        if (distance > getDistanceBetweenPoints(ballPosition, offence[i].pos)){
            distance = getDistanceBetweenPoints(ballPosition, offence[i].pos);
            closest = i;
        }
    }
    return closest;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

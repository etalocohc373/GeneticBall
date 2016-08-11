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
    ofSetFrameRate(10);
    for (int i = 0; i < OFFENCE_NUM; i++) offence[i].init();
    
    int first = rand() % OFFENCE_NUM;
    offence[first].haveBall = true;
    
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
            
            cout << ballThrower << ": " << float(offence[ballThrower].score) << ", " << float(offence[ballThrower].denomi) << endl;
        }
        if (!defence[i].isDead) defeated = false;
    }
    if (defeated){
        inheritGene();
        //inheritGene_d();
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
            int aliveNum = 0;
            int leftCount = 0;
            int frontCount = 0;
            for (int j = 0; j < DEFENCE_NUM; j++){
                if (doesExistLeftOf(j, i)) leftCount++;
                if (doesExistFrontOf(j, i)) frontCount++;
                if (!defence[i].isDead) aliveNum ++;
            }
            //cout << leftCount << endl;
            if (leftCount > aliveNum / 2) offence[i].condition_l = 0;
            if (leftCount == aliveNum / 2) offence[i].condition_l = 1;
            if (leftCount < aliveNum / 2) offence[i].condition_l = 2;
            
            if (frontCount > aliveNum / 2) offence[i].condition_p = 0;
            if (frontCount == aliveNum / 2) offence[i].condition_p = 1;
            if (frontCount < aliveNum / 2) offence[i].condition_p = 2;
            
            ballCaught = true;
            ballPosition = offence[i].pos;
            fromTheta = offence[i].theta2;
        }
        if (offence[i].willThrowBall){
            ballThrower = i;
            ballPosition = offence[i].pos;
            offence[i].willThrowBall = false;
            didKill = false;
            
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
    
    int crosspoint1 = rand() % (GENE_NUM - 2);
    int crosspoint2 = rand() % (GENE_NUM - crosspoint1 - 2) + crosspoint1 + 1;
    
    float child1[3][3][GENE_NUM];
    float child2[3][3][GENE_NUM];
    for (int k = 0; k < 3; k++){
        for (int j = 0; j < 3; j++){
            for (int i = 0; i < GENE_NUM; i++){//二点交叉
                if (i < crosspoint1 || i > crosspoint2){
                    child1[k][j][i] = offence[parent1].gene[k][j][i];
                    child2[k][j][i] = offence[parent2].gene[k][j][i];
                }else{
                    child1[k][j][i] = offence[parent2].gene[k][j][i];
                    child2[k][j][i] = offence[parent1].gene[k][j][i];
                }
                /*int index = rand() % 2;
                 if (index == 0){
                 child1[j][i] = offence[parent1].gene[j][i];
                 child2[j][i] = offence[parent2].gene[j][i];
                 }else{
                 child1[j][i] = offence[parent2].gene[j][i];
                 child2[j][i] = offence[parent1].gene[j][i];
                 }*///一様交叉
            }
        }
    }
    
    for (int i = 0; i < OFFENCE_NUM; i++){//突然変異
        offence[i].initWithGene(offence[i].gene);
        int mutate = rand() % 200;
        if (mutate <= 1){
            int mutateIndex = rand() % 3;
            int mutateIndex2 = rand() % 3;
            int mutateIndex3 = rand() % GENE_NUM;
            int sign = pow(-1.0, rand());
            offence[i].gene[mutateIndex][mutateIndex2][mutateIndex3] = ofRandom(.01, .05) * sign;
            cout << "mutated" << endl;
        }
    }
    
    didKill = false;
    fromTheta = 0;
    offence[OFFENCE_NUM - 1].initWithGene(child1);
    offence[OFFENCE_NUM - 2].initWithGene(child2);
    int first = rand() % OFFENCE_NUM;
    offence[first].haveBall = true;
    offence[first].denomi = 1;
    for (int i = 0; i < DEFENCE_NUM; i++) defence[i].init();
    cout << "success: " << parent1 << ", " << parent2 << endl;
}

void ofApp::inheritGene_d(){
    sort(defence, defence + DEFENCE_NUM, Defence());
    
    int parent1 = selectParent();
    int parent2 = selectParent();
    while (parent2 == parent1) parent2 = selectParent();
    
    int crosspoint1 = rand() % (DGENE_NUM - 2);
    int crosspoint2 = rand() % (DGENE_NUM - crosspoint1 - 2) + crosspoint1 + 1;
    
    float child1[DGENE_NUM];
    float child2[DGENE_NUM];
    for (int i = 0; i < DGENE_NUM; i++){//二点交叉
        /*if (i < crosspoint1 || i > crosspoint2){
         child1[j][i] = offence[parent1].gene[j][i];
         child2[j][i] = offence[parent2].gene[j][i];
         }else{
         child1[j][i] = offence[parent2].gene[j][i];
         child2[j][i] = offence[parent1].gene[j][i];
         }*/
        
        int index = rand() % 2;
        if (index == 0){
            child1[i] = defence[parent1].gene[i];
            child2[i] = defence[parent2].gene[i];
        }else{
            child1[i] = defence[parent2].gene[i];
            child2[i] = defence[parent1].gene[i];
        }//一様交叉
    }
    
    for (int i = 0; i < DEFENCE_NUM; i++){//突然変異
        defence[i].initWithGene(defence[i].gene);
        int mutate = rand() % 1000;
        if (mutate <= 1){
            int mutateIndex = rand() % DGENE_NUM;
            int sign = pow(-1.0, rand());
            defence[i].gene[mutateIndex] = ofRandom(0, 2 * pi);
            cout << "mutated" << endl;
        }
    }
    defence[DEFENCE_NUM - 1].initWithGene(child1);
    defence[DEFENCE_NUM - 2].initWithGene(child2);
    int first = rand() % DEFENCE_NUM;
    for (int i = 0; i < DEFENCE_NUM; i++) defence[i].init();
    cout << "d_success: " << parent1 << ", " << parent2 << endl;
}

int ofApp::selectParent(){
    int hoge = int(pow(2.0, OFFENCE_NUM));
    int roulette = rand() % hoge;
    int selectNum = 0;
    
    for (int i = 0; i < OFFENCE_NUM; i++){
        if (roulette >= hoge / pow(2.0, i + 1)){
            selectNum = i;
        }
    }
    /*if (roulette >= 50) selectNum = 0;
    else if (roulette >= 25) selectNum = 1;
    else if (roulette >= 12) selectNum = 2;
    else if (roulette >= 5) selectNum = 3;
    else if (roulette >= 2) selectNum = 4;
    else if (roulette >= 0) selectNum = 5;*/
    
    return selectNum;
}

bool ofApp::doesExistFrontOf(int defIndex, int offIndex){
    bool result = false;
    if (offence[offIndex].theta2 < pi / 2){
        if (defence[defIndex].theta < offence[offIndex].theta2 + pi / 2 || defence[defIndex].theta > offence[offIndex].theta2 + pi * 3/2) result = true;
        else result = false;
    }else if (offence[offIndex].theta2 < pi * 3/2){
        if (offence[offIndex].theta2 - pi / 2 < defence[defIndex].theta && defence[defIndex].theta < offence[offIndex].theta2 + pi / 2) result = true;
        else result = false;
    }else{
        if (defence[defIndex].theta < offence[offIndex].theta2 + pi/2 - 2*pi || defence[defIndex].theta > offence[offIndex].theta2 - pi / 2) result = true;
        else result = false;
    }
    if (defence[defIndex].isDead) result = false;
    return result;
}

bool ofApp::doesExistLeftOf(int defIndex, int offIndex){
    bool result = false;
    float hoge = defence[defIndex].theta + pi /2;
    if (hoge >= 2 * pi) hoge -= 2 * pi;
    if (offence[offIndex].theta2 < pi){
        if (offence[offIndex].theta2 > hoge || offence[offIndex].theta2 + 2 * pi < hoge) result = true;
    }else{
        if (offence[offIndex].theta2 - pi < hoge && offence[offIndex].theta2 > hoge) result = true;
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
    ofSetColor(0, 0, 255);
    ofCircle(ballPosition, 8);
    for (int i = 0; i < OFFENCE_NUM; i++){
        ofSetColor(255);
        if (offence[i].haveBall) ofSetColor(255, 50, 50);
        ofCircle(offence[i].pos, 10);
        
        if (offence[i].haveBall){
            ofDrawBitmapStringHighlight(ofToString(offence[i].condition_p), offence[i].pos);
            ofLine(offence[i].pos.x, offence[i].pos.y, -offence[i].pos.x, -offence[i].pos.y);
        }
    }
    
    ofSetColor(0, 255, 0);
    for (int i = 0; i < DEFENCE_NUM; i++){
        if (!defence[i].isDead) {
            if (defence[i].isEscaping) ofSetColor(255, 0, 0);
            else ofSetColor(0, 255, 0);
            ofCircle(defence[i].pos, 10);
            //ofDrawBitmapStringHighlight(ofToString(defence[i].acc_theta), defence[i].pos);
            //ofLine(defence[i].pos,defence[i].pos+20*ofVec2f(cos(defence[i].acc_theta),sin(defence[i].acc_theta)));
        }
    }
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

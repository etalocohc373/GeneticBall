//
//  Defence.cpp
//  GeneticTest
//
//  Created by Minami Aramaki on 2016/08/09.
//
//

#include "Defence.hpp"

Defence::Defence(){
    this -> index = 0;
    this -> isDead = false;
    for (int i = 0; i < DGENE_NUM; i++) gene[i] = ofRandom(0, 2 * pi);
}

void Defence::init(){
    float radius = ofRandom(RADIUS_MIN + .1, RADIUS - .1);
    float pos_theta = ofRandom(0, 2 * pi);
    this -> pos = ofVec2f(radius * sin(pos_theta), radius * cos(pos_theta));
    this -> isDead = false;
    this -> speed = ofRandom(10.0, 12.0);
    this -> elapsedTime = 0;
}

void Defence::initWithGene(float gotGene[DGENE_NUM]){
    this -> init();
    for (int i = 0; i < DGENE_NUM; i++) this -> gene[i] = gotGene[i];
}

void Defence::update(float ballTheta){
    this -> theta = ofMap(360 - (ofVec2f(-1, 0).angleRad(pos) + PI) / PI * 180, 0, 360, 0, 2 * PI);
    
    float acc_theta = ofRandom(0, 2 * pi);
    this -> isEscaping = willEscapeFrom(ballTheta);
    if (isEscaping){
        acc_theta = ofRandom(ballTheta + pi/2, ballTheta + pi * 3/2);
        acc_theta += PI/2;
        while (acc_theta >= 2*pi) acc_theta -= 2 * pi;
    }//自動
    /*acc_theta = gene[index];
    this -> index++;
    if (index >= DGENE_NUM) index = 0;*///進化
    
    if (willExistInFieldWhenMoved(acc_theta))
        this -> pos += ofVec2f(speed * sin(acc_theta), speed * cos(acc_theta));
    
    if (!isDead) this -> elapsedTime += 0.1;
}

bool Defence::willExistInFieldWhenMoved(float acc_theta){
    bool result = true;
    float distMoved = pow(pow(double(pos.x) + speed * sin(acc_theta), 2) + pow(double(pos.y) + speed * cos(acc_theta), 2), 0.5);
    if (distMoved >= RADIUS - DEF_RADIUS || distMoved <= RADIUS_MIN + DEF_RADIUS) result = false;
    
    return result;
}

bool Defence::willEscapeFrom(float ballTheta){
    bool result = false;
    if (ballTheta < pi / 2){//1
        if (theta < ballTheta + pi / 2 || theta > ballTheta + pi * 3/2) result = true;
        else result = false;
    }else if (ballTheta < pi * 3/2){//2
        if (ballTheta - pi / 2 < theta && theta < ballTheta + pi / 2) result = true;
        else result = false;
    }else{//3
        if (theta < ballTheta + pi/2 - 2*pi || theta > ballTheta - pi / 2) result = true;
        else result = false;
    }
    
    return result;
}


//
//  Offence.cpp
//  GeneticTest
//
//  Created by Minami Aramaki on 2016/08/09.
//
//

#include "Offence.hpp"

Offence::Offence(){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            float sign = pow(double(-1), rand());
            for (int k = 0; k < GENE_NUM; k++){
                this -> gene[i][j][k] = ofRandom(.02, .07)/* * sign*/;
            }
        }
    }
    this -> denomi = 0;
    this -> score = 0;
}

void Offence::init(){
    this -> theta = ofRandom(0, 2 * pi);
    this -> pos = ofVec2f(0,0);
    
    this -> haveBall = false;
    this -> willThrowBall = false;
    
    this -> speed = ofRandom(.04, .045) * (int)pow(double(-1), rand());
    this -> ballSpeed = ofRandom(8, 9);
    
    this -> condition_p = 0;
    this -> condition_l = 0;
    // landscape --- 0: 左 1: 同じ 2: 右
    // portrait --- 0: 手前 1: 同じ 2: 奥
    this -> denomi = 0;
    this -> score = 0;
}

void Offence::initWithGene(float gotGene[2][2][GENE_NUM]){
    this -> init();
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < GENE_NUM; k++) this -> gene[i][j][k] = gotGene[i][j][k];
}

void Offence::update(){
    if (haveBall){
        this -> theta += gene[condition_p][condition_l][index];
        this -> index++;
        if (index >= GENE_NUM){
            this -> denomi++;
            this -> willThrowBall = true;
            this -> haveBall = false;
            this -> index = 0;
        }
    }else{
        this -> theta += speed;
    }
    while (theta >= 2 * pi) this -> theta -= 2 * pi;
    this -> pos = ofVec2f(sin(theta) * (RADIUS + 10), cos(theta) * (RADIUS + 10));
    theta2 = ofMap(360 - (ofVec2f(-1, 0).angleRad(pos) + PI) / PI * 180, 0, 360, 0, 2 * PI);
}

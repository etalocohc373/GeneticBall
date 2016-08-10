//
//  Defence.hpp
//  GeneticTest
//
//  Created by Minami Aramaki on 2016/08/09.
//
//

#include "ofMain.h"

class Defence {
public:
    //unsigned int gene[GENE_NUM];
    
    float speed;
    float theta;
    ofVec2f pos;
    bool isDead;
    bool isEscaping;
    
    Defence();
    void init();
    //void initWithGene(unsigned int gene[GENE_NUM]);
    void update(float ballTheta);
    bool willEscapeFrom(float ballTheta);
};

//
//  Defence.hpp
//  GeneticTest
//
//  Created by Minami Aramaki on 2016/08/09.
//
//

#include "ofMain.h"
#define DGENE_NUM 100
#define RADIUS 150
#define RADIUS_MIN 15
#define DEF_RADIUS 8

class Defence {
public:
    bool operator()(const Defence& a, const Defence& b) const {
        return a.elapsedTime < b.elapsedTime;
    }
    
    float speed;
    float theta;
    float elapsedTime;
    ofVec2f pos;
    float gene[DGENE_NUM];
    bool isDead;
    bool isEscaping;
    bool willExistInFieldWhenMoved(float theta);
    
    Defence();
    void init();
    void initWithGene(float gene[DGENE_NUM]);
    void update(float ballTheta);
    bool willEscapeFrom(float ballTheta);
    
private:
    int index;
};

//
//  Offence.hpp
//  GeneticTest
//
//  Created by Minami Aramaki on 2016/08/09.
//
//

#include "ofMain.h"
#define GENE_NUM 100

class Offence {
public:
    bool operator()(const Offence& a, const Offence& b) const {
        float score_a = 0;
        if (a.denomi != 0)
            score_a = float(a.score) / float(a.denomi);
        
        float score_b = 0;
        if (b.denomi != 0)
            score_b = float(b.score) / float(b.denomi);
        
        
        
        return score_a > score_b;
    }
    
    int denomi;
    int score;
    int condition;
    float gene[3][GENE_NUM];
    ofVec2f pos;
    
    float theta2;
    float speed;
    float ballSpeed;
    
    bool haveBall;
    bool willThrowBall;
    
    Offence();
    void init();
    void initWithGene(float gene[3][GENE_NUM]);
    void update();
    
private:
    float theta;
    int index;
};
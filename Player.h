//
// Created by keren on 01/01/2022.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <exception>
#include <iostream>

using  namespace std;

struct keyPlayerLevel{
    int id;
    int level;
    keyPlayerLevel(int id,int level):id(id),level(level){};
    ~keyPlayerLevel()=default;
    keyPlayerLevel(const keyPlayerLevel & other)=default;
    keyPlayerLevel& operator=(const keyPlayerLevel &other)=default;
    bool operator<(const keyPlayerLevel &other)const{
        if(level==other.level){
            return id<other.id;
        }
        if(level<other.level){
            return true;
        }
        return false;
    }
    bool operator>(const keyPlayerLevel &other)const{
        if(level==other.level){
            return id>other.id;
        }
        if(level>other.level){
            return true;
        }
        return false;
    };
    bool operator==(const keyPlayerLevel &other)const{
        return id==other.id;
    };

};
struct keyPlayerScore{
    int id;
    int level;
    int score;
    keyPlayerScore(int id,int score,int level):id(id),score(score),level(level){};
    ~keyPlayerScore()=default;
    keyPlayerScore(const keyPlayerScore & other)=default;
    keyPlayerScore& operator=(const keyPlayerScore &other)=default;
    bool operator<(const keyPlayerScore &other)const{
        if(score==other.score){
            if(level==other.level){
                return id<other.id;
            }
            if(level<other.level){
                return true;
            }
            return false;
        }
        if(score<other.score){
            return true;
        }
        return false;
    }
    bool operator>(const keyPlayerScore &other)const{
        if(score==other.score){
            if(level==other.level){
                return id>other.id;
            }
            if(level>other.level){
                return true;
            }
            return false;
        }
        if(score>other.score){
            return true;
        }
        return false;
    };
    bool operator==(const keyPlayerScore &other)const{
        return id==other.id;
    };

};



class Player{
    keyPlayerLevel key_level;
    keyPlayerScore key_score;
    int group;

public:
    Player(int id,int score,int group,int level=0);
    Player(const Player & other);
    void increaseLevel(int add_level);
    void decreaseLevel(int level);
    void changeScore(int new_score);
    ~Player()=default;
    bool operator<(const Player &other)const;
    bool operator>(const Player &other)const;
    bool operator==(const Player &other)const;
    Player& operator=(const Player &other);
    int getId()const;
    int getLevel()const;
    int getScore()const;
    int getGroup()const;
    keyPlayerLevel& getKeyLevel();
    keyPlayerScore& getKeyScore();
    friend std::ostream& operator<<(std::ostream& os, const Player& player);


};



#endif //WET2_PLAYER_H

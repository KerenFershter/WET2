//
// Created by keren on 01/01/2022.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <exception>
#include <iostream>

using  namespace std;

struct keyPlayer{
    int id;
    int level;
    keyPlayer(int id,int level):id(id),level(level){};
    ~keyPlayer()=default;
    keyPlayer(const keyPlayer & other)=default;
    keyPlayer& operator=(const keyPlayer &other)=default;
    bool operator<(const keyPlayer &other)const{
        if(level==other.level){
            return id<other.id;
        }
        if(level<other.level){
            return true;
        }
        return false;
    }
    bool operator>(const keyPlayer &other)const{
        if(level==other.level){
            return id>other.id;
        }
        if(level>other.level){
            return true;
        }
        return false;
    };
    bool operator==(const keyPlayer &other)const{
        return id==other.id;
    };

};

class Player{
    keyPlayer key;
    int score;
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
    keyPlayer& getKey();
    friend std::ostream& operator<<(std::ostream& os, const Player& player);


};



#endif //WET2_PLAYER_H

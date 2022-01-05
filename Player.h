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
        if(level == other.level){
            return id < other.id;
        }
        return level < other.level;
    }

    bool operator>(const keyPlayer &other)const{
        if(level == other.level){
            return id > other.id;
        }
        return level > other.level;
    }

    bool operator==(const keyPlayer &other)const{
        return id == other.id;
    }

    bool operator<=(const keyPlayer &other)const{
//        return this->operator<(other) || this->operator==(other);
        return *this < other || *this == other;
    }

    bool operator>=(const keyPlayer &other)const{
//        return this->operator>(other) || this->operator==(other);
        return *this > other || *this == other;
    }

    friend std::ostream& operator<<(std::ostream& os, const keyPlayer& player){
        os << "id:" << player.id << "|lvl:" << player.level;
        return os;
    }

};

class Player{
    keyPlayer key;
    int score;
    int group;

    public:
    Player(int id,int score,int group,int level=0);
    Player(const Player & other);
    ~Player()=default;
    void increaseLevel(int add_level);
    void decreaseLevel(int level);
    void changeScore(int new_score);
    bool operator<(const Player &other)const;
    bool operator>(const Player &other)const;
    bool operator==(const Player &other)const;
    Player& operator=(const Player &other);
    int getId()const;
    int getLevel()const;
    int getScore()const;
    int getGroup()const;
    keyPlayer& getKey();
    void setGroup(int new_group_id);
    static void setGroup(Player& player, int new_group_id);
    friend std::ostream& operator<<(std::ostream& os, const Player& player);


};



#endif //WET2_PLAYER_H

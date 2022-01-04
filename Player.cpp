//
// Created by keren on 01/01/2022.
//
#include "Player.h"

using namespace std;

Player::Player(int id,int score,int group,int level):score(score),key(id,level),group(group){};

Player::Player(const Player & other):
key(other.key),score(other.score){};

bool Player::operator<(const Player &other) const{
    return key<other.key;
}
bool Player::operator>(const Player &other) const{
    return key>other.key;
}

void Player::increaseLevel(int add_level) {
    this->key.level+=add_level;
}

void Player::decreaseLevel(int level) {
    this->key.level-=level;
}

void Player::changeScore(int new_score) {
    this->score=new_score;
}

bool Player::operator==(const Player &other) const {
    return key==other.key;
}

Player &Player::operator=(const Player &other) {
    this->key=other.key;
    this->score=other.score;
}

 std::ostream& operator<<(std::ostream& os, const Player& player){
    os<<"player id="<<player.key.id<<", player level="<<player.key.level<<", player score="<<player.score<<endl;
}

int Player::getId() const {
    return key.id;
}

int Player::getLevel() const {
    return key.level;
}

int Player::getScore() const {
    return score;
}

keyPlayer &Player::getKey() {
    return key;
}

int Player::getGroup() const {
    return group;
}



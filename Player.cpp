//
// Created by keren on 01/01/2022.
//
#include "Player.h"

using namespace std;

Player::Player(int id, int score, int group, int level) :
    key_level(id, level),
    key_score(id, score, level),
    group(group){}

Player::Player(const Player & other):
        key_level(other.key_level),
        key_score(other.key_score),
        group(other.group){}

bool Player::operator<(const Player &other) const{
    return key_level.id < other.key_level.id;
}
bool Player::operator>(const Player &other) const{
    return key_level.id > other.key_level.id;
}

void Player::increaseLevel(int add_level) {
    this->key_level.level += add_level;
    this->key_score.level += add_level;
}

void Player::decreaseLevel(int level) {
    this->key_level.level -= level;
    this->key_score.level -= level;
}

void Player::changeScore(int new_score) {
    this->key_score.score = new_score;
}

bool Player::operator==(const Player &other) const {
    return key_level.id == other.key_level.id;
}

Player &Player::operator=(const Player &other){
    this->key_level = other.key_level;
    this->key_score = other.key_score;
    this->group = other.group;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& player){
    os << "player id=" << player.key_score.id
    << ", player level=" << player.key_score.level
    << ", player score=" << player.key_score.score << endl;
    return os;
}

int Player::getId() const {
    return key_level.id;
}

int Player::getLevel() const {
    return key_level.level;
}

int Player::getScore() const {
    return key_score.score;
}

keyPlayerLevel &Player::getKeyLevel() {
    return key_level;
}

int Player::getGroup() const {
    return group;
}

void Player::setGroup(int new_group_id){
    this->group = new_group_id;
}

void Player::setGroup(Player &player, int new_group_id) {
    player.setGroup(new_group_id);
}

keyPlayerScore &Player::getKeyScore() {
    return key_score;
}

Player::operator int() const{
    return this->getLevel();
}

Player::operator double() const{
    return (double)(this->getLevel());
}



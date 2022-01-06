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

    keyPlayerLevel(int id, int level) : id(id), level(level){};
    ~keyPlayerLevel() = default;
    keyPlayerLevel(const keyPlayerLevel & other) = default;
    keyPlayerLevel& operator=(const keyPlayerLevel &other) = default;

    bool operator<(const keyPlayerLevel &other) const{
        if(level == other.level){
            return id < other.id;
        }
        return level < other.level;
    }

    bool operator>(const keyPlayerLevel &other) const{
        if(level == other.level){
            return id > other.id;
        }
        return level > other.level;
    }

    bool operator==(const keyPlayerLevel &other) const{
        return id == other.id;
    }

    bool operator<=(const keyPlayerLevel &other) const{
//        return this->operator<(other) || this->operator==(other);
        return *this < other || *this == other;
    }

    bool operator>=(const keyPlayerLevel &other) const{
//        return this->operator>(other) || this->operator==(other);
        return *this > other || *this == other;
    }

    friend std::ostream& operator<<(std::ostream& os, const keyPlayerLevel& player){
        os << "id:" << player.id << "|lvl:" << player.level;
        return os;
    }

};

struct keyPlayerScore{
    int id;
    int level;
    int score;

    keyPlayerScore(int id, int score, int level) : id(id), score(score), level(level){};
    ~keyPlayerScore() = default;
    keyPlayerScore(const keyPlayerScore & other) = default;
    keyPlayerScore& operator=(const keyPlayerScore &other) = default;

    bool operator<(const keyPlayerScore &other) const{
        if(score == other.score){
            if(level == other.level){
                return id < other.id;
            }
            return level < other.level;
        }

        return score < other.score;
    }

    bool operator>(const keyPlayerScore &other) const{
        if(score == other.score){
            if(level == other.level){
                return id > other.id;
            }
            return level > other.level;
        }
        return score > other.score;
    };

    bool operator==(const keyPlayerScore &other)const{
        return id == other.id;
    };

    bool operator<=(const keyPlayerScore &other) const{
//        return this->operator<(other) || this->operator==(other);
        return *this < other || *this == other;
    }

    bool operator>=(const keyPlayerScore &other) const{
//        return this->operator>(other) || this->operator==(other);
        return *this > other || *this == other;
    }

    friend std::ostream& operator<<(std::ostream& os, const keyPlayerScore& player){
        os << "id:" << player.id << "|lvl:" << player.level << "|score:" << player.score;
        return os;
    }
};



class Player{
    keyPlayerLevel key_level;
    keyPlayerScore key_score;
    int group;

public:
    Player(int id, int score, int group, int level = 0);
    Player(const Player & other);
    void increaseLevel(int add_level);
    void decreaseLevel(int level);
    void changeScore(int new_score);
    ~Player() = default;
    bool operator<(const Player &other) const;
    bool operator>(const Player &other) const;
    bool operator==(const Player &other) const;
    Player& operator=(const Player &other);
    int getId() const;
    int getLevel() const;
    int getScore() const;
    int getGroup() const;

    keyPlayerLevel& getKeyLevel();
    keyPlayerScore& getKeyScore();
    void setGroup(int new_group_id);
    static void setGroup(Player& player, int new_group_id);
    operator int() const;
    operator double() const;

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};



#endif //WET2_PLAYER_H

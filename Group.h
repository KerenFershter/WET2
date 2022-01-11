//
// Created by keren on 01/01/2022.
//

#ifndef WET2_GROUP_H
#define WET2_GROUP_H


#include <exception>
#include <iostream>
#include <memory>
#include "Player.h"
#include "DynamicArray.h"
#include "AvlTree_sum.h"
#include "Utils.h"


class NoSuchScore : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "no such score";
    }
};
class NoSuchPlayer : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "no such player";
    }
};


class Group {
    typedef shared_ptr<Player> ptr_player;
    int id;
    int scale;
    int * hist_scores;
    int * hist_scores_0;
    int num_level_0;
    DynamicArray<ptr_player> all_players;
    AVLTree<keyPlayerLevel> players_by_level;
    AVLTree<keyPlayerScore> players_by_score;
    int player_count;

    static void _give_id(ptr_player& player, void* new_id);

public:
    Group(int id, int scale);
    ~Group();
    void addPlayer(ptr_player & player);
    void removePlayer( ptr_player & player);
    bool playerExist( ptr_player & player);
    void onIncreasePlayerLevel(ptr_player player, int level_increase);
    void onChangePlayerScore(ptr_player player, int new_score);
    bool getPercentOfPlayersWithScoreInBounds(int max_id, int score, int lowerLevel, int higherLevel, double* players);
    void merge(Group& other);
    double averageHighestPlayerLevelByGroup(int m);

    int getSize();

    bool operator==(const Group& other);

};


#endif //WET2_GROUP_H

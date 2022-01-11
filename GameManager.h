//
// Created by keren on 03/01/2022.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <memory>
#include <iostream>
#include "library2.h"
#include "AVLTree_sum.h"
#include "UnionFind.h"
#include "Group.h"
#include "Utils.h"

class LevelNotChanged : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "level didn't changed";
    }
};
class ScoreNotChanged : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "score didn't changed";
    }
};
class GameManager {
    typedef shared_ptr<Player> ptr_player;
    typedef shared_ptr<Group> ptr_group;

    int k;
    int scale;
    int max_id;
    int * hist_scores;
    int * hist_scores_0;
    int num_level_0;
    DynamicArray<ptr_player> all_players;
    AVLTree<keyPlayerLevel> players_by_level;
    AVLTree<keyPlayerScore> players_by_score;
    UnionFind<ptr_group> * groups;
    int player_count;

public:
    GameManager(int k,int scale);
    ~GameManager();
    StatusType mergeGroups(int GroupID1, int GroupID2);
    StatusType addPlayer(int PlayerID, int GroupID, int score);
    StatusType removePlayer( int PlayerID);
    bool playerExist(int player_id);
    StatusType increasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType changePlayerIDScore( int PlayerID, int NewScore);
    StatusType getPercentOfPlayersWithScoreInBounds(int GroupID, int score,
                                                    int lowerLevel, int higherLevel, double * players);
    StatusType averageHighestPlayerLevelByGroup(int groupID, int m, double* avgLevel);

    StatusType getPlayersBound(int GroupID, int score, int m, int * LowerBoundPlayers, int * HigherBoundPlayers);
};


#endif //GAMEMANAGER_H

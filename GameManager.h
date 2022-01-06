//
// Created by keren on 03/01/2022.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "memory"
#include "Group.h"
#include "iostream"
#include "UnionFind.h"
#include "library.h"

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
    int * hist_scores;
    int * hist_scores_0;
    int num_level_0;
    DynamicArray<ptr_player> players_level_0;
    AVLTree<keyPlayer,ptr_player> players;
    UnionFind<ptr_group> * groups;

    public:
    GameManager(int k,int scale);
    ~GameManager();
    //void* init(int k,int scale);
    StatusType mergeGroups(int GroupID1, int GroupID2);//TODO:merge between the players

    StatusType addPlayer(int PlayerID, int GroupID, int score);
    StatusType removePlayer( int PlayerID);
    bool playerExist(int player_id);
    StatusType increasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType changePlayerIDScore( int PlayerID, int NewScore);

    StatusType averageHighestPlayerLevelByGroup(int groupID, int m, double* avgLevel);

    friend std::ostream& operator<<(std::ostream& os, const GameManager& game_manager);

};


#endif //GAMEMANAGER_H

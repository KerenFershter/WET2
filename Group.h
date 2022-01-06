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
#include "AvlTree.h"


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
    DynamicArray<ptr_player> players_level_0;
    AVLTree<keyPlayer,ptr_player> players;


    static void _give_id(ptr_player& player, void* new_id);

public:
    Group(int id,int scale);
    ~Group();
    void addPlayer(ptr_player & player);
    void removePlayer( ptr_player & player);
    bool playerExist( ptr_player & player);
    void increasePlayerLevel(ptr_player player,int level_increase);
    void changePlayerScore(ptr_player player, int new_score);
    void merge(Group& other);
    int getSize();

    friend std::ostream& operator<<(std::ostream& os, const Group& group);


};


#endif //WET2_GROUP_H

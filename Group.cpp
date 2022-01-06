//
// Created by keren on 01/01/2022.
//

#include "Group.h"
////avl with key player, dynamic array with id

Group::Group(int id, int scale):id(id),scale(scale),num_level_0(0){
   hist_scores = new int[scale + 1];//TODO: check if need to do scale+1
   hist_scores_0 = new int[scale + 1];

   for(int i = 0; i < scale + 1; i++){
       hist_scores_0[i] = 0;
       hist_scores[i] = 0;
   }
}

Group::~Group() {
    delete [] hist_scores;
    delete [] hist_scores_0;
}

void Group::addPlayer( Group::ptr_player & player) {
    int score=player->getScore();
    if(score>scale || score<=0){
        throw NoSuchScore();
    }
    hist_scores_0[score]++;
    num_level_0++;
    players_level_0.insert(player->getId(),player);
}

void Group::removePlayer( Group::ptr_player & player) {
    players_level_0.remove(player->getId());
    if(!player->getLevel()) {
        hist_scores_0[player->getScore()]--;
        num_level_0--;
    }else{
        players.remove(player->getKey());
        hist_scores[player->getScore()]--;
    }

}

bool Group::playerExist( Group::ptr_player& player) {
        return players_level_0.exists(player->getId());
}

void Group::increasePlayerLevel(Group::ptr_player player,int level_increase) {
        if(!playerExist(player)){
            throw NoSuchPlayer();
        }
        Group::ptr_player tmp=players_level_0.find(player->getId());
        if(!player->getLevel()){
            hist_scores_0[player->getScore()]--;
            num_level_0--;
            tmp->increaseLevel(level_increase);//TODO: check increased our side
            players.insert(tmp->getKey(),player);
            hist_scores[tmp->getScore()]++;
            tmp->decreaseLevel(level_increase);/////out side will be done increase

        }else{
            players.remove(tmp->getKey());
            tmp->increaseLevel(level_increase);//TODO: check increased out side
            players.insert(tmp->getKey(),player);
            tmp->decreaseLevel(level_increase);/////out side will be done increase
        }
}

void Group::changePlayerScore(Group::ptr_player player, int new_score) {
        if(!playerExist(player)){
            throw NoSuchPlayer();
        }
        if(new_score>scale || new_score<0){
            throw NoSuchScore();
        }
        int old_score=player->getScore();
        if(!player->getLevel()) {
            hist_scores_0[old_score]--;
            hist_scores_0[new_score]++;
        }else{
             hist_scores[old_score]--;
             hist_scores[new_score]++;
        }
        //tmp->changeScore(new_score);
}

void Group::merge(Group& other){
    for(int i = 1; i <= this->scale; i++){
        this->hist_scores[i] += other.hist_scores[i];
        this->hist_scores_0[i] += other.hist_scores_0[i];
    }

    this->num_level_0 += other.num_level_0;

    other.players_level_0.apply(Group::_give_id, (void*)(this->id));

    this->players_level_0.merge(other.players_level_0);
    this->players.merge(other.players);
}


std::ostream &operator<<(ostream &os, const Group &group) {
    os<<"Group Id:"<<group.id<<endl;
    os<<"his_0: by num of players_0: "<<group.num_level_0<<endl;
    for(int i=0;i<group.scale;i++){
        os<<group.hist_scores_0[i]<<", ";
    }
    os<<endl;
    os<<"his_not_0:"<<endl;
    for(int i=0;i<group.scale;i++){
        os<<group.hist_scores[i]<<", ";
    }
    os<<endl;
    os<<"dynamic:"<<endl;
    os<<group.players_level_0<<endl;
   // os<<"avl:"<<endl;
   // group.players.print();//TODO:print avl

}

void Group::_give_id(Group::ptr_player& player, void* new_id) {
    Player::setGroup(*player, (int)(unsigned long)new_id);
}

int Group::getSize() {
    return this->players_level_0.getCount();
}



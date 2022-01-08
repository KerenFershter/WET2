//
// Created by keren on 01/01/2022.
//

#include "Group.h"


Group::Group(int id, int scale) : id(id), scale(scale), num_level_0(0){
    hist_scores = new int[scale + 1];
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
    all_players.insert(player->getId(),player);
}

void Group::removePlayer( Group::ptr_player & player) {
    all_players.remove(player->getId());
    if(!player->getLevel()) {
        hist_scores_0[player->getScore()]--;
        num_level_0--;
    }
    else {
        players_by_score.remove(player->getKeyScore());
        players_by_level.remove(player->getKeyLevel());
        hist_scores[player->getScore()]--;
    }

}

bool Group::playerExist( Group::ptr_player& player) {
    return all_players.exists(player->getId());
}

void Group::onIncreasePlayerLevel(Group::ptr_player player, int level_increase) {
    if(!playerExist(player)){
        throw NoSuchPlayer();
    }
    if(!player->getLevel()){
        hist_scores_0[player->getScore()]--;
        num_level_0--;
        player->increaseLevel(level_increase);//TODO: check increased our side
        players_by_level.insert(player->getKeyLevel(), player);
        players_by_score.insert(player->getKeyScore(), player);
        hist_scores[player->getScore()]++;
        player->decreaseLevel(level_increase);/////out side will be done increase

    }
    else {
        players_by_level.remove(player->getKeyLevel());
        players_by_score.remove(player->getKeyScore());
        player->increaseLevel(level_increase);//TODO: check increased out side
        players_by_level.insert(player->getKeyLevel(), player);
        players_by_score.insert(player->getKeyScore(), player);
        player->decreaseLevel(level_increase);/////out side will be done increase
    }
}

void Group::onChangePlayerScore(Group::ptr_player player, int new_score) {
    if(!playerExist(player)){
        throw NoSuchPlayer();
    }
    if(new_score > scale || new_score < 0){
        throw NoSuchScore();
    }

    int old_score=player->getScore();
    if(!player->getLevel()) {
        hist_scores_0[old_score]--;
        hist_scores_0[new_score]++;
    }
    else {
        players_by_score.remove(player->getKeyScore());
        player->changeScore(new_score);
        players_by_score.insert(player->getKeyScore(), player);
        player->changeScore(old_score);
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

    other.all_players.apply(Group::_give_id, (void*)(unsigned long)(this->id));

    this->all_players.merge(other.all_players);
    this->players_by_level.merge(other.players_by_level);
    this->players_by_score.merge(other.players_by_score);
}

bool Group::getPercentOfPlayersWithScoreInBounds(int max_id, int score, int lowerLevel, int higherLevel,
                                                 double *players) {

    keyPlayerScore min_tmp_score(0, score, lowerLevel);
    keyPlayerScore max_tmp_score(max_id + 1, score, higherLevel);
    keyPlayerLevel min_tmp_level(0, lowerLevel);
    keyPlayerLevel max_tmp_level(max_id + 1, higherLevel);

    int range_score = players_by_score.rangeCount(min_tmp_score, max_tmp_score);
    int range_level = players_by_level.rangeCount(min_tmp_level, max_tmp_level);

    if(lowerLevel == 0){
        range_score += hist_scores_0[score];
        range_level += num_level_0;
    }

    if(range_level <= 0){
        return false;
    }

    *players = *players = Utils::percent(range_score, range_level);
    return true;
}

bool Group::operator==(const Group& other){
    return this->id == other.id;
}

std::ostream &operator<<(ostream &os, const Group &group) {
    os << "Group Id:" << group.id << endl;
    os << "his_0: by num of players_0: " << group.num_level_0 << endl;
    for(int i = 0; i < group.scale; i++){
        os << group.hist_scores_0[i] << ", ";
    }
    os << endl;
    os << "his_not_0:" << endl;
    for(int i = 0; i < group.scale; i++){
        os << group.hist_scores[i] << ", ";
    }
    os << endl;
    os << "dynamic:" << endl;
    os << group.all_players << endl;
    // os<<"avl:"<<endl;
    // group.players.print();//TODO:print avl

    return os;

}

void Group::_give_id(Group::ptr_player& player, void* new_id) {
    Player::setGroup(*player, (int)(unsigned long)new_id);
}

int Group::getSize() {
    return this->all_players.getCount();
}

double Group::averageHighestPlayerLevelByGroup(int m) {
    double sum = this->players_by_level.max_m_sum(m);
    return sum / (double)m;
}





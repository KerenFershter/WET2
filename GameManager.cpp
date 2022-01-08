//
// Created by keren on 03/01/2022.
//

#include "GameManager.h"

double cast_player_ptr_to_double(shared_ptr<Player>& player_ptr){
    return (double)(*player_ptr);
}

GameManager::GameManager(int k, int scale) : scale(scale), k(k), num_level_0(0), max_id(0) {
    hist_scores = new int[scale + 1];//TODO: check if need to do scale+1
    hist_scores_0 = new int[scale + 1];
    for(int i = 0; i < scale + 1; i++){
        hist_scores_0[i] = 0;
        hist_scores[i] = 0;
    }
    groups = new  UnionFind<shared_ptr<Group>>(k + 1);
    for(int i = 1; i < k + 1; i++){
        groups->MakeSet(i,shared_ptr<Group>(new Group(i,scale)));
    }

    players_by_level = new AVLTree<keyPlayerLevel,ptr_player>(
            reinterpret_cast<function<double(shared_ptr<Player>&)>&>(cast_player_ptr_to_double));
    players_by_score = new AVLTree<keyPlayerScore, ptr_player>(
            reinterpret_cast<function<double(shared_ptr<Player> &)>&>(cast_player_ptr_to_double)
    );
}

GameManager::~GameManager() {
    delete [] hist_scores;
    delete [] hist_scores_0;
    delete groups;
}

StatusType GameManager::mergeGroups(int GroupID1, int GroupID2) {
    if(GroupID1 <= 0 || GroupID1 > k || GroupID2 <= 0 || GroupID2 > k){
        return INVALID_INPUT;
    }

    ptr_group g1 = groups->getKeyData(GroupID1);
    ptr_group g2 = groups->getKeyData(GroupID2);

    if(*g1 == *g2){
        return SUCCESS;
    }

    if(groups->getSizeByKey(GroupID1) > groups->getSizeByKey(GroupID2)){
        g1->merge(*g2);
    }
    else {
        g2->merge(*g1);
    }

    groups->Union(groups->find(GroupID1), groups->find(GroupID2));

    return SUCCESS;
}

StatusType GameManager::addPlayer(int PlayerID, int GroupID, int score) {
    if(GroupID <= 0 || GroupID >k  || PlayerID <= 0 || score > scale || score <= 0){
        return INVALID_INPUT;
    }

    if(playerExist(PlayerID)){
        return FAILURE;
    }
    if(PlayerID > max_id){
        max_id = PlayerID;
    }
    shared_ptr<Player> player = make_shared <Player>(PlayerID,score,GroupID) ;
    shared_ptr<Group> group = groups->getKeyData(GroupID);
    group->addPlayer(player);
    hist_scores_0[player->getScore()]++;
    num_level_0++;
    all_players.insert(player->getId(),player);
    return SUCCESS;
}

StatusType GameManager::removePlayer(int PlayerID) {
    if( PlayerID <= 0 ){
        return INVALID_INPUT;
    }

    if(!playerExist(PlayerID)){
        return FAILURE;
    }

    shared_ptr<Player> player = all_players.find(PlayerID) ;
    shared_ptr<Group> group = groups->getKeyData(player->getGroup());
    group->removePlayer(player);

    all_players.remove(player->getId());
    if(!player->getLevel()) {
        hist_scores_0[player->getScore()]--;
        num_level_0--;
    }
    else {
        players_by_level->remove(player->getKeyLevel());
        players_by_score->remove(player->getKeyScore());
        hist_scores[player->getScore()]--;
    }
    return SUCCESS;
}

StatusType GameManager::increasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if(PlayerID <= 0 || LevelIncrease <= 0){
        return INVALID_INPUT;
    }
    if(!playerExist(PlayerID)){
        return FAILURE;
    }
    shared_ptr<Player> player = all_players.find(PlayerID) ;
    shared_ptr<Group> group = groups->getKeyData(player->getGroup());
    group->onIncreasePlayerLevel(player, LevelIncrease);

    if(!player->getLevel()){
        //players_level_0.remove(player->getId());
        hist_scores_0[player->getScore()]--;
        num_level_0--;
        player->increaseLevel(LevelIncrease);//TODO: check increaseLevel
        players_by_level->insert(player->getKeyLevel(), player);
        players_by_score->insert(player->getKeyScore(), player);
        hist_scores[player->getScore()]++;

    }
    else {
        players_by_level->remove(player->getKeyLevel());
        players_by_score->remove(player->getKeyScore());
        player->increaseLevel(LevelIncrease);//TODO: check increaseLevel
        players_by_level->insert(player->getKeyLevel(), player);
        players_by_score->insert(player->getKeyScore(), player);
    }

    return SUCCESS;

}

bool GameManager::playerExist(int player_id) {
    return all_players.exists(player_id);
}

StatusType GameManager::changePlayerIDScore(int PlayerID, int NewScore) {
    if(PlayerID <= 0 || NewScore <= 0 || NewScore > scale){
        return INVALID_INPUT;
    }
    if(!playerExist(PlayerID)){
        return FAILURE;
    }
    shared_ptr<Player> player = all_players.find(PlayerID);
    shared_ptr<Group> group = groups->getKeyData(player->getGroup());
    group->onChangePlayerScore(player, NewScore);
    int old_score = player->getScore();
    if(!player->getLevel()) {
        hist_scores_0[old_score]--;
        player->changeScore(NewScore);
        hist_scores_0[NewScore]++;
    }
    else{
        hist_scores[old_score]--;
        players_by_score.remove(player->getKeyScore());
        player->changeScore(NewScore);
        players_by_score.insert(player->getKeyScore(), player);
        hist_scores[NewScore]++;
    }


    return SUCCESS;
}

StatusType GameManager::getPercentOfPlayersWithScoreInBounds(int GroupID, int score,
                                                             int lowerLevel, int higherLevel, double *players){
    if(GroupID < 0 || GroupID > k){
        return INVALID_INPUT;
    }
    if(lowerLevel>higherLevel){
        return FAILURE;
    }
    if(score <= 0 || score > scale){
        *players = 0;
        return SUCCESS;
    }


    if(GroupID){
        shared_ptr<Group> group = groups->getKeyData(GroupID);
        bool success = group->getPercentOfPlayersWithScoreInBounds(max_id, score, lowerLevel, higherLevel, players);
        return success ? SUCCESS : FAILURE;
    }

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
        return FAILURE;
    }

    *players = Utils::percent(range_score, range_level);
    return SUCCESS;
}

StatusType GameManager::averageHighestPlayerLevelByGroup(int groupID, int m, double* avgLevel){
    if(groupID < 0 || groupID > this->k || m <= 0 || !avgLevel){
        return INVALID_INPUT;
    }

    if(groupID == 0){
        if(m > this->all_players.getCount()){
            return FAILURE;
        }


    }
    else {
        ptr_group group = this->groups->getKeyData(groupID);
        if(m > group->getSize()){
            return FAILURE;
        }

    }
    *avgLevel = -1.0;
    return SUCCESS;
}

StatusType GameManager::getPlayersBound(int GroupID, int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers){
    return FAILURE;
}

std::ostream &operator<<(ostream &os, const GameManager &game_manager) {
    os << "groups arr:" << endl;
    for(int i = 1; i <= game_manager.k; i++){
        shared_ptr<Group> group = game_manager.groups->getKeyData(i);
        os << (*group);
        os << "--------------------------" << endl;
    }
    os << "--------------------------" << endl;
    os << "manager is:" << endl;
    os << "his_0: by num of players_0: " << game_manager.num_level_0 << endl;
    for(int i = 0; i < game_manager.scale; i++){
        os << game_manager.hist_scores_0[i] << ", ";
    }

    os << endl;
    os << "his_not_0:" << endl;
    for(int i = 0; i < game_manager.scale; i++){
        os << game_manager.hist_scores[i] << ", ";
    }
    os << endl;
    os << "dynamic:" << endl;
    os << game_manager.all_players << endl;

    return os;
}



//
// Created by keren on 03/01/2022.
//

#include "GameManager.h"
#define DEFAULT 1

GameManager::GameManager(int k, int scale):scale(scale),k(k),num_level_0(0) {
    hist_scores = new int[scale+1];//TODO: check if need to do scale+1
    hist_scores_0 = new int[scale+1];

    for(int i = 0; i < scale + 1; i++){
        hist_scores_0[i] = 0;
        hist_scores[i] = 0;
    }

    groups = new  UnionFind<shared_ptr<Group>>(k+1);

    for(int i = 1; i < k + 1; i++){
        groups->MakeSet(i,shared_ptr<Group>(new Group(i,scale)));
    }
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

        if(groups->getSizeByKey(GroupID1) > groups->getSizeByKey(GroupID2)){
            g1->merge(*g2);
        }
        else {
            g2->merge(*g1);
        }

        groups->Union(GroupID1, GroupID2);//TODO:merge between players group & change players id group
        return SUCCESS;
}

StatusType GameManager::addPlayer(int PlayerID, int GroupID, int score) {
    if(GroupID<=0 || GroupID>k || PlayerID<=0 || score>scale || score<=0){
        return INVALID_INPUT;
    }

    if(playerExist(PlayerID)){
        return FAILURE;
    }

    shared_ptr<Player> player= make_shared <Player>(PlayerID,score,GroupID) ;
    shared_ptr<Group> group=groups->getKeyData(GroupID);
    group->addPlayer(player);
    hist_scores_0[player->getScore()]++;
    num_level_0++;
    players_level_0.insert(player->getId(),player);
    return SUCCESS;
}

StatusType GameManager::removePlayer(int PlayerID) {
    if( PlayerID<=0 ){
        return INVALID_INPUT;
    }

    if(!playerExist(PlayerID)){
        return FAILURE;
    }

    shared_ptr<Player> player=players_level_0.find(PlayerID) ;
    shared_ptr<Group> group=groups->getKeyData(player->getGroup());
    group->removePlayer(player);

    players_level_0.remove(player->getId());
    if(!player->getLevel()) {
        hist_scores_0[player->getScore()]--;
        num_level_0--;
    }else{
        players.remove(player->getKey());
        hist_scores[player->getScore()]--;
    }
    return SUCCESS;
}

StatusType GameManager::increasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if(PlayerID<=0 || LevelIncrease<=0){
        return INVALID_INPUT;
    }
    if(!playerExist(PlayerID)){
        return FAILURE;
    }
    shared_ptr<Player> player=players_level_0.find(PlayerID) ;
    shared_ptr<Group> group=groups->getKeyData(player->getGroup());
    group->increasePlayerLevel(player,LevelIncrease);

    if(!player->getLevel()){
        //players_level_0.remove(player->getId());
        hist_scores_0[player->getScore()]--;
        num_level_0--;
        player->increaseLevel(LevelIncrease);//TODO: check increaseLevel
        players.insert(player->getKey(),player);
        hist_scores[player->getScore()]++;

    }else{
        players.remove(player->getKey());
        player->increaseLevel(LevelIncrease);//TODO: check increaseLevel
        players.insert(player->getKey(),player);
    }

}

bool GameManager::playerExist(int player_id) {
    return players_level_0.exists(player_id);
}

StatusType GameManager::changePlayerIDScore(int PlayerID, int NewScore) {
    if(PlayerID<=0 || NewScore<=0 || NewScore>scale){
        return INVALID_INPUT;
    }
    if(!playerExist(PlayerID)){
        return FAILURE;
    }
    shared_ptr<Player> player=players_level_0.find(PlayerID);
    shared_ptr<Group> group=groups->getKeyData(player->getGroup());
    group->changePlayerScore(player,NewScore);
    int old_score=player->getScore();
    if(!player->getLevel()) {
        hist_scores_0[old_score]--;
        hist_scores_0[NewScore]++;
    }else{
        hist_scores[old_score]--;
        hist_scores[NewScore]++;
    }
    player->changeScore(NewScore);

    return SUCCESS;
}

//StatusType GameManager::averageHighestPlayerLevelByGroup(int groupID, int m, double* avgLevel){
//    if(groupID < 0 || groupID > this->k || m <= 0 || !avgLevel){
//        return INVALID_INPUT;
//    }
//
//    if(groupID == 0){
//        if(m > this->players_level_0.getCount()){
//            return FAILURE;
//        }
//
//
//    }
//    else {
//        ptr_group group = this->groups->getKeyData(groupID);
//        if(m > group->getSize()){
//            return FAILURE;
//        }
//
//    }
//}

std::ostream &operator<<(ostream &os, const GameManager &game_manager) {
    os<<"groups arr:"<<endl;
    for(int i=1;i<=game_manager.k;i++){
        shared_ptr<Group> group=game_manager.groups->getKeyData(i);
        os<<(*group);
        os<<"--------------------------"<<endl;
    }
    os<<"--------------------------"<<endl;
    os<<"manager is:"<<endl;
    os<<"his_0: by num of players_0: "<<game_manager.num_level_0<<endl;
    for(int i=0;i<game_manager.scale;i++){
        os<<game_manager.hist_scores_0[i]<<", ";
    }
    os<<endl;
    os<<"his_not_0:"<<endl;
    for(int i=0;i<game_manager.scale;i++){
        os<<game_manager.hist_scores[i]<<", ";
    }
    os<<endl;
    os<<"dynamic:"<<endl;
    os<<game_manager.players_level_0<<endl;

}




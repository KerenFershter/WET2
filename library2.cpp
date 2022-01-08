//
// Created by keren on 07/01/2022.
//
#include "library2.h"
#include "GameManager.h"



void* Init(int k, int scale) {
    GameManager *DS = new GameManager(k,scale);
    return (void*) DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    return ((GameManager*) DS)->mergeGroups(GroupID1, GroupID2);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level){
    return ((GameManager*) DS)->addPlayer(PlayerID, GroupID, Level);
}

StatusType RemovePlayer(void *DS, int PlayerID){
    return ((GameManager*) DS)->removePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    return ((GameManager*) DS)->increasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    return ((GameManager*) DS)->changePlayerIDScore(PlayerID, NewScore);
}
StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players){
    return ((GameManager*) DS)->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    return ((GameManager*) DS)->averageHighestPlayerLevelByGroup(GroupID, m, level);
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    return ((GameManager*) DS)->getPlayersBound(GroupID, score, m, LowerBoundPlayers, HigherBoundPlayers);
}

void Quit(void** DS){
    delete((GameManager*) *DS);
    *DS= NULL;
}


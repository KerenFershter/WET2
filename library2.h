//
// Created by keren on 03/01/2022.
//

#ifndef WET2_LIBRARY2_H
#define WET2_LIBRARY2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init(int k, int scale);

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2);

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score);

StatusType RemovePlayer(void *DS, int PlayerID);

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease);

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore);

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players);

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level);

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif //WET2_LIBRARY2_H

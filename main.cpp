
//#include <iostream>
//#include "GameManager.h"
//#include "memory"
//
//using namespace std;
//
//int main() {
//    ////------------GameManager-----------
//    GameManager *gm = new GameManager(3, 6);
//    /////check addPlayer
//    gm->addPlayer(1, 1, 1);
//    gm->addPlayer(2, 1, 1);
//    gm->addPlayer(3, 1, 1);
//    gm->addPlayer(4, 1, 1);
//    gm->addPlayer(5, 1, 1);
//    gm->addPlayer(6, 1, 1);
//    gm->addPlayer(7, 1, 1);
//    gm->addPlayer(8, 1, 1);
//    gm->addPlayer(31, 1, 1);
//    gm->addPlayer(21, 1, 1);
//    gm->addPlayer(11, 1, 1);
//    cout << (*gm);
//
//    /////check removePlayer
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    gm->removePlayer(21);
//
//    cout << (*gm);
//    /////check increasePlayerIDLevel
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    gm->increasePlayerIDLevel(1, 3);
//    cout << (*gm);
//    /////check changePlayerIDScore
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    cout << "--------------------------" << endl;
//    gm->changePlayerIDScore(6, 4);
//
//    cout << (*gm);
//
//
//    ////-----------Groups--------------
////    shared_ptr<Player> player1(new Player(1,1,1));
////    shared_ptr<Player> player2(new Player(2,1,1));
////    shared_ptr<Player> player3(new Player(3,1,1));
////    shared_ptr<Player> player4(new Player(4,2,1));
////    shared_ptr<Player> player5(new Player(5,1,1));
////    shared_ptr<Player> player6(new Player(6,4,1));
////    shared_ptr<Player> player7(new Player(7,1,1));
////    shared_ptr<Player> player8(new Player(8,3,1));
////    shared_ptr<Player> player9(new Player(9,1,1));
////    shared_ptr<Player> player10(new Player(10,1,1));
////    shared_ptr<Player> player11(new Player(11,1,1));
////    shared_ptr<Player> player12(new Player(12,1,1));
////    shared_ptr<Player> player13(new Player(13,1,1));
////    shared_ptr<Player> player14(new Player(14,1,1));
////    shared_ptr<Player> player15(new Player(15,1,1));
////    shared_ptr<Player> player16(new Player(16,1,1));
////    shared_ptr<Player> player17(new Player(17,1,1));
////    shared_ptr<Player> player18(new Player(18,1,1));
////    shared_ptr<Player> player19(new Player(19,1,1));
////    shared_ptr<Player> player20(new Player(20,1,1));
////
////////check addPlayer
////    Group * group=new Group(1,5);
////    group->addPlayer(player1);
////    group->addPlayer(player2);
////    group->addPlayer(player3);
////    group->addPlayer(player4);
////    group->addPlayer(player5);
////    group->addPlayer(player6);
////    group->addPlayer(player7);
////    group->addPlayer(player8);
////    group->addPlayer(player9);
////    group->addPlayer(player10);
////    group->addPlayer(player11);
////    group->addPlayer(player12);
////    group->addPlayer(player13);
////    group->addPlayer(player14);
////    group->addPlayer(player15);
////    group->addPlayer(player16);
////    group->addPlayer(player17);
////    group->addPlayer(player18);
////    group->addPlayer(player19);
////    group->addPlayer(player20);
////    cout<<(*group);
//
//////check removePlayer
////    cout<<"------------------------------------"<<endl;
////    group->removePlayer(player11);
////    group->removePlayer(player12);
////    group->removePlayer(player13);
////    group->removePlayer(player14);
////    group->removePlayer(player15);
////    group->removePlayer(player16);
////    group->removePlayer(player17);
////    group->removePlayer(player18);
////    group->removePlayer(player19);
////    group->removePlayer(player20);
////    cout<<(*group);
//
//
//////check increasePlayerLevel
////    cout<<"------------------------------------"<<endl;
////    group->increasePlayerLevel(player10,2);
////    group->increasePlayerLevel(player11,2);
////    group->increasePlayerLevel(player12,2);
////    group->increasePlayerLevel(player13,2);
////    group->increasePlayerLevel(player14,2);
////    group->increasePlayerLevel(player15,2);
////    group->increasePlayerLevel(player16,2);
////    group->increasePlayerLevel(player17,2);
////    group->increasePlayerLevel(player18,2);
////    group->increasePlayerLevel(player19,2);
////    group->increasePlayerLevel(player20,2);
////    cout<<(*group);
//
//////check changePlayerScore
////    cout<<"------------------------------------"<<endl;
////    group->changePlayerScore(player10,2);
////    group->changePlayerScore(player11,2);
////    group->changePlayerScore(player12,2);
////    group->changePlayerScore(player13,2);
////    group->changePlayerScore(player14,2);
////    group->changePlayerScore(player15,2);
////    group->changePlayerScore(player16,2);
////    group->changePlayerScore(player17,2);
////    group->changePlayerScore(player18,2);
////    group->changePlayerScore(player19,2);
////    group->changePlayerScore(player20,2);
////      cout<<(*group);
//
//
//    //delete group;
//    delete gm;
//
//}
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "AvlTree.h"
//#include "Player.h"
#include <memory>

using namespace std;

//double player_to_double(Player &player){
//    return (double)player;
//}

double int_to_double(int n){
    return (double)n;
}

int main(){
//    auto t = new AVLTree<int,int>(int_to_double);
    auto t = new AVLTree<int,int>();
    t->insert(4, 1);
    t->insert(3, 0);
    t->insert(2, 1);
    t->insert(1, 0);
    t->insert(0, 1);

    t->print(true, true);
    cout << "----------------------------------------------------------------" << endl;

    t->insert(5, 0);

    t->print(true, true);
    cout << "----------------------------------------------------------------" << endl;

    t->insert(6, 1);
    t->insert(60, 0);
    t->insert(600, 1);

    t->print(true, true);
    cout << "----------------------------------------------------------------" << endl;

//    t->remove(3);
//    t->remove(4);
    t->remove(0);
    t->remove(3);
    t->remove(4);

    t->print(true, true);
    cout << "----------------------------------------------------------------" << endl;



//    auto t2 = new AVLTree<int,int>(int_to_double);
    auto t2 = new AVLTree<int,int>();
    t2->insert(40, 8);
    t2->insert(30, 9);
    t2->insert(20, 8);
    t2->insert(10, 9);
    t2->insert(-10, 8);

    t2->print(true, true);
    cout << "----------------------------------------------------------------" << endl;


    t->merge(*t2);

    t->print(true, true);
    cout << "----------------------------------------------------------------" << endl;

    for(int r = 1; r <= t->getSize(); r++){
        int rk = t->select(r);
        int rks = t->rank(rk);

        if(r == rks){
            cout << "rank(select(" << r << ")) = rank(" << rk << ") = " << rks << endl;
        }
    }

    cout << t->find(5) << endl;
    cout << t->find(60) << endl;
    cout << t->find(600) << endl;
    cout << t->find(20) << endl;

    delete t;
    delete t2;

    return 0;
}

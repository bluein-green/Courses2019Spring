#include<iostream>
#include<vector>
#include<stdio.h>
#include<set>
using namespace std;
int board[8][8];//0 means empty, 1 means white, 2 means dark
int dark, white, chess;
int self_side;
int dir[8][2] = {1,-1,1,1,-1,1,-1,1,1,0,-1,0,0,1,0,-1};
int minSearch(int depth, int alpha, int Player);
int maxSearch(int depth, int beta, int Player);
void getLegalMove(int nowPlayer, set<int>&legal_move);//generate legal moves for nowPlayer
bool check(int i, int j, int nowPlayer);//check whether legal
int whether_end();//whether the game is over and who wins. 0: not end 1:white wins 2:dark wins 3: draw
int evaluation(int self_side);//get the evaluation value based on different side
void changeColor(int i, int j, int nowPlayer, set<int>&colorchange);//get the color changed positions
void computer_move(int Player);
void person_move(int Player);
void visulize();
//改变颜色后是否需要重新修改，一直到棋盘不发生变化？
int whether_end()
{
    int dark_n = 0;
    int white_n = 0;
    int n=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]==1)
            {
                white_n+=1;
                n+=1;
            }
            else if(board[i][j]==2)
            {
                dark_n+=1;
                n+=1;
            }
        }
    }
    if(dark_n==0||white_n==0||n==64){
        if(dark_n<white_n){
            return 1;
        }
        else if(dark_n>white_n)
        {
            return 2;
        }
        else{
            return 3;
        }
    }
    else return 0;
}


int evaluation(int self_side)
{
    int white_n = 0;
    int dark_n = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]==1)
            {
                white_n+=1;
            }
            else if(board[i][j]==2)
            {
                dark_n+=1;
            }
        }
    }
    if(self_side==1)//white
    {
        return white_n-dark_n;
    }
    else
    {
        return dark_n-white_n;
    }
}

bool check(int i, int j, int nowPlayer)
{
    if(i<0||i>=8||j<0||j>=8)return false;
    if(board[i][j]!=0)return false;
    for(int d=0;d<8;d++){
        int tempi = i;
        int tempj = j;
        bool ifdifferent = false;
        bool ifsame = false;
        while(true)
        {
            tempi += dir[d][0];
            tempj += dir[d][1];
            if (tempi<0||tempi>=8||tempj<0||tempj>=8)break;
            if(board[tempi][tempj]==0){
                break;
            }
            else if(board[tempi][tempj]==nowPlayer){
                if(ifdifferent){
                    ifsame = true;
                    return true;
                }
            }
            else if(board[tempi][tempj]!=nowPlayer){
                ifdifferent = true;
            }
        }
    }
    return false;
}


void getLegalMove(int nowPlayer, set<int>&legal_move)
{
    //set<int>legal_move;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]!=nowPlayer && board[i][j]!=0)//maybe legal around it
            {
                for(int d=0;d<8;d++)
                {
                    int tempi = i+dir[d][0];
                    int tempj = j+dir[d][1];
                    if(check(tempi, tempj, nowPlayer))
                    {
                        legal_move.insert(tempi*10+tempj);
                    }
                }
            }

        }

    }

}

void changeColor(int i, int j, int nowPlayer, set<int>&colorchange)
{
    board[i][j] = nowPlayer;
    for(int d=0;d<8;d++)
    {
        int tempi = i;
        int tempj = j;
        vector<int>dir_target;//different colors in this direction
        while(true)
        {
            tempi+=dir[d][0];
            tempj+=dir[d][1];
            if (tempi<0||tempi>=8||tempj<0||tempj>=8)break;
            if(board[tempi][tempj]==0)break;
            else if(board[tempi][tempj]!=nowPlayer)
            {
                dir_target.push_back(tempi*10+tempj);
            }
            else{//change color
                vector<int>::iterator ptr = dir_target.begin();
                while(ptr!=dir_target.end())
                {
                    colorchange.insert(*ptr);
                    ptr++;
                }
                dir_target.clear();
            }
        }
    }
    set<int>::iterator ptr = colorchange.begin();
    while(ptr!=colorchange.end())
    {
        board[*ptr/10][*ptr%10]=nowPlayer;
        ptr++;
    }
}

int main()
{
    board[3][3]=1;
    board[4][4]=1;
    board[3][4]=2;
    board[4][3]=2;
    dark = 2;
    white = 2;
    chess = 4;
    cout<<"This is the Reversi Game."<<endl;
    cout<<"Please select dark or white player for the search algorithm. 1:white 2:black"<<endl;
    cin>>self_side;
    cout<<"please predefine the time limit"<<endl;
    int time = -1;
    cin>>time;
    if (self_side==1){
        cout<<"The computer is white player and you are the dark player. You move first."<<endl;
    }
    else{
        cout<<"The computer is dark player and you are the white player. The computer moves first"<<endl;
    }
    while(1){
        int current = whether_end();
        if(current!=0)
        {
            if(current==1)
            {
                cout<<"white wins!"<<endl;
            }
            else if(current==2)
            {
                cout<<"dark wins!"<<endl;
            }
            else
            {
                cout<<"draw!"<<endl;
            }
            break;
        }
        if(self_side==1)//you input first, you are dark and computer is white
        {
            person_move(2);
            computer_move(1);//computer is white
            visulize();
        }
        else//you are white and computer is dark
        {
            computer_move(2);
            person_move(1);
            visulize();
        }
    }
    return 0;
}
int minSearch(int depth, int alpha, int Player)
{
    if(depth==0 ||whether_end()!=0)
    {
        return evaluation(self_side);
    }
    int beta = 99999999;
    set<int>generate_node;
    getLegalMove(Player, generate_node);
    set<int>::iterator iter = generate_node.begin();
    int nextPlayer=-1;
    if(Player==1)nextPlayer=2;
    else nextPlayer=1;
    while(iter!=generate_node.end())
    {
        int child_i = *iter/10;
        int child_j = *iter%10;
        set<int>change;
        changeColor(child_i, child_j, Player, change);
        beta = min(beta, maxSearch(depth-1, beta, nextPlayer));
        set<int>::iterator ptr = change.begin();
        while(ptr!=change.end())
        {
            board[*ptr/10][*ptr%10]=nextPlayer;
            ptr++;
        }
        board[child_i][child_j]=0;
        if(alpha>=beta)return beta;
        iter++;
    }
}

int maxSearch(int depth, int beta, int Player)
{
    if(depth==0 ||whether_end()!=0)
    {
        return evaluation(self_side);
    }
    int alpha = -999999999;
    set<int>generate_node;
    getLegalMove(Player, generate_node);
    set<int>::iterator iter = generate_node.begin();
    int nextPlayer=-1;
    if(Player==1)nextPlayer=2;
    else nextPlayer=1;
    while(iter!=generate_node.end())
    {
        int child_i = *iter/10;
        int child_j = *iter%10;
        set<int>change;
        changeColor(child_i, child_j, Player, change);
        alpha = max(alpha, minSearch(depth-1, alpha, nextPlayer));
        set<int>::iterator ptr = change.begin();
        while(ptr!=change.end())
        {
            board[*ptr/10][*ptr%10]=nextPlayer;
            ptr++;
        }
        board[child_i][child_j]=0;
        if(alpha>=beta)return alpha;
        iter++;
    }


}


void person_move(int Player)
{
    set<int>dark_legal_move;
    int nexti,nextj;
    getLegalMove(Player, dark_legal_move);
    if (dark_legal_move.empty())return;
    set<int>::iterator iter = dark_legal_move.begin();
    cout<<"Your legal move: ";
    while(iter!=dark_legal_move.end()){
        cout<<"("<<*iter/10<<" "<<*iter%10<<") ";
        nexti = *iter/10;
        nextj = *iter%10;
        iter++;
    }

    //cout<<"please enter your move"<<endl;
    //cin>>nexti>>nextj;
    //cout<<nexti<<nextj;
    set<int>white2dark;
    changeColor(nexti, nextj, Player, white2dark);
}

void computer_move(int Player)
{
    set<int>white_legal_move;//电脑执白能走的
    getLegalMove(Player, white_legal_move);
    set<int>::iterator iter = white_legal_move.begin();
    int best = -99999999;
    int nextMove = -1;
    int depth = 6;
    int nextPlayer = -1;
    if(Player==1)nextPlayer=2;
    else nextPlayer=1;
    while(iter!=white_legal_move.end()){
        //cout<<"("<<*iter/10<<" "<<*iter%10<<") ";
        int child_i = *iter/10;
        int child_j = *iter%10;
        set<int>dark2white;
        changeColor(child_i, child_j, Player, dark2white);

        int tmp = minSearch(depth, best, nextPlayer);

        set<int>::iterator ptr = dark2white.begin();
        while(ptr!=dark2white.end())
        {
            board[*ptr/10][*ptr%10]=nextPlayer;
            ptr++;
        }
        board[child_i][child_j]=0;
        if(tmp>best)
        {
            best=tmp;
            nextMove=*iter;
        }
        iter++;
    }
    if (nextMove>=0){
        set<int>dark2white;
        changeColor(nextMove/10, nextMove%10, Player, dark2white);
        cout<<"computer move ("<<nextMove/10<<" "<<nextMove%10<<")"<<endl;
    }
}


void visulize()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]==0)
                cout<<"#";
            else if(board[i][j]==1)
                cout<<"w";
            else
                cout<<"d";
        }
        cout<<endl;
    }
}

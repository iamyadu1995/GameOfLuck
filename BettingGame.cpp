#include <iostream>
#include <cstdlib> // Needed to use random numbers
#include <fstream>//Needed for File Handling
#include <string.h>//For string operations
#define SIZE 30
using namespace std;

//output manipulators
ostream & newMessage( ostream & out)
{
    out<<"\n";
    out.width(30);
    out.setf(ios::left);
    return out;
}

ostream & space(ostream & out)
{
    out<<"\t";
    return out;
}

void drawLine()
{
    cout << "\n" ;
    for(int i=0; i<60; i++)
        cout << "_";
    cout << "\n" ;

}

//Forward declaration of class
class FileManager;

//Entity Player
class Player
{
    char playerName[20];
    int playerAmout;
public:
    Player();
    void set(char nm[] , int amt);
    void display();
    char * getPlayerName();
    int getPlayerAmount();
    void updatePlayerAmount(int);
};

Player :: Player()
{
    strcpy(playerName, "");
    playerAmout = 0;
}

void Player :: set(char nm[], int amt)
{
    strcpy(playerName , nm);
    playerAmout = amt;
}

void Player :: display()
{
    cout<<newMessage<<playerName<<space<<playerAmout;
}

char * Player :: getPlayerName()
{
    return playerName;
}

int Player :: getPlayerAmount()
{
    return playerAmout;
}

void Player :: updatePlayerAmount(int q)
{
    playerAmout = q;
}

//Entity Store Manager

        cout<<newMessage<<"Player Name Already In Use";
        return;
    }

    
}

void FileManager :: modifyRecord(Player* p)
{
    fstream fio;
    fio.open(fileName, ios::binary|ios::in|ios::out);

    if(!fio)
    {
        cout<<newMessage<<"Modif return;
    }

    Player temp;
    int cnt = 0;
    int flag = 0;

    while(fio.read((char*)&temp , sizeof(Player)) )
    {
        if(strcmpi(temp.getPlayerName(), p->getPlayerName()) ==0)
        {
            fio.seekp(cnt*sizeof(Player), ios::beg);
            fio.write((char*)p, sizeof(Player));
            flag = 1;
            break;
        }
        cnt++;
    }

    if(flag == 1)
        cout<<newMessage<<"Record Updated";
    else
        cout<<newMessage<<"Record Updation Failed";

    fio.flush();
    fio.close();

}

Player* FileManager :: searchRecord(char nm[])
{
    ifstream fin;
    fin.open(fileName, ios::binary|ios::in);

    if(!fin)
    {
        return NULL;
    }

    Player *p = new Player();
    int flag = 0;

    while(fin.read((char*)p , sizeof(Player)) )
    {
        if(strcmpi(nm, p->getPlayerName()) ==0)
        {
            flag = 1;
            break;
        }
    }//while

    fin.close();

    if(flag == 1)
        return p;
    else
        return NULL;

}

//Entity Game
class Game
{
    FileManager *fm;
public :
    Game(FileManager *f);
    void rules();
    void play();
    Player * setPlayer();

};

Game :: Game(FileManager * f)
{
    fm = f;
}

Player * Game :: setPlayer()
{
    int flag =0;
    char pName[20];
    cout<<newMessage<<" Enter Player Name";
    cin>>pName;
    return fm->searchRecord(pName);
}

void Game ::rules()
{
    drawLine();
    cout <<newMessage <<"GAME of LUCK\n";
    cout <<newMessage<<"RULES OF THE GAME\n";
    cout <<newMessage<< "Choose any number between 1 to 10";
    cout <<newMessage<< "If you win you will get 10 times of money you bet";
    cout <<newMessage<< "If you bet on wrong number you will lose your betting amount";
    drawLine();
}

void Game :: play()
{
    int guess;
    int dice;
    int betAmount;
    int flag = 1;
    char choice ;

    rules();

    //get player information
    Player *p = setPlayer();

    if(p == NULL)
    {
        //unregistered player
        cout<<newMessage<<"Player Authentication Failed";
        return ;
    }

    //show player account information
    p->display();

    //start the game
    do
    {
        //accept bet amount
        flag = 1;
        do
        {
            cout<<newMessage<<"Enter bet amount ";
            cin>>betAmount;
            if(betAmount > p->getPlayerAmount())
                cout<<newMessage<<"Maximum amount may be : "<<p->getPlayerAmount();
            else if(betAmount <=0)
                cout<<newMessage<<"Minimum amount may be > 0 ";
            else
                flag = 0;

        }while(flag == 1);

        //accept the number to bet on
        flag = 1;
        do
        {
            cout<<newMessage<< "Enter number to bet on (1 to 10) : ";
            cin >> guess;
            if(guess <= 0 || guess > 10)
                cout <<newMessage<< "Please bet on a number between 1 to 10";
            else
                flag = 0;
        }while(flag == 1);

        //generate a random number in range 1 to 10
        dice = rand()%10 + 1;

        //match
        if(dice == guess)
        {
            cout <<newMessage<<"Good Luck!! You won Rs." << betAmount * 10;
            p->updatePlayerAmount(p->getPlayerAmount() + betAmount * 10);
        }
        else
        {
            cout <<newMessage <<"Bad Luck this time !! You lost Rs. "<< betAmount;
            cout <<newMessage << "The winning number was : " << dice <<"\n";
            p->updatePlayerAmount(p->getPlayerAmount() - betAmount);
        }

        //show account status
        cout <<newMessage<<p->getPlayerName()<<", You have Rs. " << p->getPlayerAmount()<< " in your account";

        //terminate the game if account balance is zero
        if(p->getPlayerAmount() == 0)
        {
            cout <<newMessage<< "You have no money to play ";
            break;
        }

        //replay choice
        cout <<newMessage<< "Do you want to play again (y/n)? ";
        cin >> choice;
    }while(choice =='Y'|| choice=='y');

    //update the account information in store
    fm->modifyRecord(p);

    drawLine();
    cout << "\nThanks for playing game. Your balance amount is Rs. " << p->getPlayerAmount() << "\n";
    drawLine();

    //deallocate the player
    delete p;
}


int main()
{
    drawLine();
    cout << "\nWELCOME TO GAME of LUCK\n";
    drawLine();

    //declarations
    char nm[SIZE];
    int amt;
    int choice;
    char fileName[SIZE];

    //objects to operate
    strcpy(fileName, "e:\\game.txt" );
    FileManager fmgr(fileName);
    Player pl;
    Game g(&fmgr);

    //menu
    do
    {
        cout<<newMessage<<" 1. Add Player";
        cout<<newMessage<<" 2. Activate Game";
        cout<<newMessage<<" 3. Quit ";

        cout<<newMessage<<" Enter Choice ";
        cin>>choice;

        switch(choice)
        {
            case 1:
                cout<<newMessage<<"Enter Player Name";
                cin>>nm;
                cout<<newMessage<<"Enter Player Deposit Amount";
                cin>>amt;

                pl.set(nm, amt);
                
                break;
            case 2:
                g.play();
                break;
            case 3:
                break;
            default :
                cout<<"\n Wrong Choice";
                break;
        }
    }while(choice != 3);

    return 0;
}

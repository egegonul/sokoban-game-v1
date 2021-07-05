#include <iostream>
#include <fstream>
using namespace std;

const int stackqueue_size=150; //stack size is declared globally as 150

class Sokoban{            //Sokoban class defined
private:
   char mgame_map[6][8];  // game map is defined as a 6x8 char array
   int player_row,player_column;  //player location is stored as integers
public:
    Sokoban(char game_map[6][8]);
   Sokoban(const Sokoban &otherSokoban);
    Sokoban& operator=(const Sokoban &anotherSokoban);
    Sokoban();
    bool move_up();
   bool move_down();
    bool move_left();
    bool move_right();
   bool is_solved();
   void print_puzzle();

};

template <class T>
class StackQueue{    //stackqueue class defined
private:
    T stackqueue_list[stackqueue_size];  //the stackqueue can store 150 elements
    int front,rear;
public:
    StackQueue();
    void push_front(T item);
    T pop_front();
    T pop_rear();
    bool is_empty();
    bool is_full();
    void display(); // for debugging purposes

};



int main()
{



   char key,prev_key;
    Sokoban s,current_sokoban;
    StackQueue<Sokoban> s_stack;
    s.print_puzzle();       //initial state of the map is printed
    s_stack.push_front(s);   //initial state of the map is pushed into the stackqueue

while(!s.is_solved())  //game loop. continues until the game is solved
{
    cin>>key;
  switch (key){           //depending on the key, related movement is performed and after each movement, new state of the map is pushed into the stackqueue
        case 'w':if(s.move_up()) {s_stack.push_front(s);s.print_puzzle();}break;
        case 's':if(s.move_down()){s_stack.push_front(s);s.print_puzzle();}break;
        case 'a':if(s.move_left()) {s_stack.push_front(s);s.print_puzzle();}break;
        case 'd':if(s.move_right()) {s_stack.push_front(s);s.print_puzzle();}break;
        case 'z':        //to undo last move, last state of sokoban is popped out, previous state is also popped out to print the puzzle then pushed back in
            if(prev_key=='z'){
                cout<<"\nCan't undo twice\n";break;}  //if z is pressed twice error message is displayed
            s_stack.pop_front();
            s=s_stack.pop_front();
            s.print_puzzle();
            s_stack.push_front(s);

            break;
            s_stack.pop_front();
            break;
        case 'r':           //the chronological order of the states of the map is from rear to the front in the list so when r is entered stakcqueue pops all the elemnts from rear to the front and displays  them
            while(!s_stack.is_empty()){
                current_sokoban=s_stack.pop_rear();
                current_sokoban.print_puzzle();
            }
            exit(1); //pressing r also finishes the game
            break;
   }
    prev_key=key; //previous movement is stored


}
cout<<"\n\nCongratulations, the game is solved!";

}





//STACKQUEUE METHODS-----------------------------------------------------------------

template <class T>
StackQueue<T>::StackQueue():front(stackqueue_size-1),rear(stackqueue_size-1){}; //constructor initializes front and rear to the end of the list

template <class T>
bool StackQueue<T>::is_empty(){       //returns 1 if the list is empty
    return front==rear;
}
template <class T>
bool StackQueue<T>::is_full(){     // returns 1 if the list is full
    return front==0;
}
template <class T>
void StackQueue<T>::push_front(T item){        // whenever a new item is pushed it is stored in front place
    if (is_full())                              // and front points shifts one step closer to the beginning of the list
        cerr<<"StackQueue overflow"<<endl;   //gives error if the list is already full
    front--;
    stackqueue_list[front]=item;

}
template <class T>
T StackQueue<T>::pop_front(){       //returns the item stored in the front and shifts front one step to the end
    if (is_empty())                 // gives error if the list is empty
        cerr<<"Stackqueue empty"<<endl;
    T temp=stackqueue_list[front];
    front++;
    return temp;
}
 template<class T>
T StackQueue<T>::pop_rear(){             //returns the item stored in rear and shifts rear towards the beginning of the list
    if (is_empty())                          // gives error if the list is empty
        cerr<<"Stackqueue empty"<<endl;
        rear--;
    T temp=stackqueue_list[rear];
    return temp;
}
template<class T>
void StackQueue<T>::display(){   //displays the stackquee: for debugging purposes
    if (is_empty())
        cout<<"Stackqueue empty"<<endl;
    for(int i=front;i<rear;i++)
        cout<<stackqueue_list[i]<<" ";
    cout<<endl;

}

// SOKOBAN METHODS------------------------------------------------------------------

Sokoban::Sokoban(char game_map[6][8]){   //the constructor that initializes from a char array
for(int i=0; i<6; ++i)
    for(int j=0; j<8; ++j)
        mgame_map[i][j]=game_map[i][j];
}
Sokoban::Sokoban(const Sokoban &othersokoban){  // copy constructor defined
    for(int i=0; i<6; ++i)
        for(int j=0; j<8; ++j)
            mgame_map[i][j]=othersokoban.mgame_map[i][j];
    player_row=othersokoban.player_row;
    player_column=othersokoban.player_column;


}
Sokoban& Sokoban::operator=(const Sokoban &anothersokoban){ //copy assignment op. defined
 for(int i=0; i<6; ++i)
        for(int j=0; j<8; ++j)
            mgame_map[i][j]=anothersokoban.mgame_map[i][j];
    player_row=anothersokoban.player_row;
    player_column=anothersokoban.player_column;


}

Sokoban::Sokoban(){                     // the constructor that initializes from file
string path = "Sample_puzzle.txt";
char dummy;
ifstream file;
file.open(path);
for(int i=0; i<6; ++i){
 for(int j=0; j<8; ++j){
 file >> noskipws >> mgame_map[i][j];    // characters in file assigned to game_map one by one
   if (mgame_map[i][j]=='@'||mgame_map[i][j]=='+'){    //locate the player
    player_row=i;
    player_column=j;}
 }
 file >> noskipws >> dummy;
}
file.close();
}

bool Sokoban::move_down(){
    bool move_valid;                      // boolean variable that takes 1 if the move is valid
    char current_block=mgame_map[player_row][player_column];
    char next_block=mgame_map[player_row+1][player_column];
    char two_next_block=mgame_map[player_row+2][player_column];

    if(next_block==' '||next_block=='.')   // checks the case if the below block is a space or a target
    {
        move_valid=true;
        if(current_block=='+')           // if the player is on a target, when it moves the block becomes a '.',if it is on space block becomes space
            mgame_map[player_row][player_column]='.';
        else
            mgame_map[player_row][player_column]=' ';
        if(next_block=='.')                // if the below block is a target, when the player moves onto it, it is shown as '+'
            mgame_map[player_row+1][player_column]='+';
        else                                // if the below block is free space, when the player moves onto it, it is whown as '@'
            mgame_map[player_row+1][player_column]='@';
        player_row++;
    }

    else if (next_block=='$'||next_block=='*'){  //if there is a box below,the box behind the box is checked
        if(two_next_block==' '||two_next_block=='.')   // if the block behind the box is movable into, the box and player moves, otherwise not
        {
            move_valid= true;
            if (current_block=='+')
                mgame_map[player_row][player_column]='.';
            else
                mgame_map[player_row][player_column]=' ';

            if (next_block=='*')            //if the box at the next block is on target, when it is moved, the player moves there and shown by '+'
                mgame_map[player_row+1][player_column]='+';
            else                            // if the box at the next block is on empty space, when it is moved , the player moves there and shown by '@'
                mgame_map[player_row+1][player_column]='@';

            if(two_next_block==' ')        //finally the block behind the box is checked, if it is empty when the block is moved there it is shown as '$'
              mgame_map[player_row+2][player_column]='$';
            else                          // if the block is a target, when the block moves there, it is shown as '*'
                mgame_map[player_row+2][player_column]='*';
            player_row++;
        }
        else
        move_valid=false;
    }
    else
        move_valid=false;

   return move_valid;
}

bool Sokoban::move_up(){
    bool move_valid;                      // boolean variable that takes 1 if the move is valid
    char current_block=mgame_map[player_row][player_column];
    char next_block=mgame_map[player_row-1][player_column];
    char two_next_block=mgame_map[player_row-2][player_column];

    if(next_block==' '||next_block=='.')   // checks the case if the above block is a space or a target
    {
        move_valid=true;
        if(current_block=='+')           // if the player is on a target, when it moves the block becomes a '.',if it is on space block becomes space
            mgame_map[player_row][player_column]='.';
        else
            mgame_map[player_row][player_column]=' ';
        if(next_block=='.')                // if the above block is a target, when the player moves onto it, it is shown as '+'
            mgame_map[player_row-1][player_column]='+';
        else                                // if the above block is free space, when the player moves onto it, it is shown as '@'
            mgame_map[player_row-1][player_column]='@';
        player_row--;
    }

    else if (next_block=='$'||next_block=='*'){  //if there is a box above,the box behind the box is checked
        if(two_next_block==' '||two_next_block=='.')   // if the block behind the box is movable into, the box and player moves, otherwise not
        {
            move_valid= true;
            if (current_block=='+')
                mgame_map[player_row][player_column]='.';
            else
                mgame_map[player_row][player_column]=' ';

            if (next_block=='*')            //if the box at the next block is on target, when it is moved, the player moves there and shown by '+'
                mgame_map[player_row-1][player_column]='+';
            else                            // if the box at the next block is on empty space, when it is moved , the player moves there and shown by '@'
                mgame_map[player_row-1][player_column]='@';

            if(two_next_block==' ')        //finally the block behind the box is checked, if it is empty when the block is moved there it is shown as '$'
              mgame_map[player_row-2][player_column]='$';
            else                          // if the block is a target, when the block moves there, it is shown as '*'
                mgame_map[player_row-2][player_column]='*';
            player_row--;
        }
        else
        move_valid=false;
    }
    else
        move_valid=false;

   return move_valid;
}

bool Sokoban::move_right(){
    bool move_valid;                      // boolean variable that takes 1 if the move is valid
    char current_block=mgame_map[player_row][player_column];
    char next_block=mgame_map[player_row][player_column+1];
    char two_next_block=mgame_map[player_row][player_column+2];

    if(next_block==' '||next_block=='.')   // checks the case if the right block is a space or a target
    {
        move_valid=true;
        if(current_block=='+')           // if the player is on a target, when it moves the block becomes a '.',if it is on space block becomes space
            mgame_map[player_row][player_column]='.';
        else
            mgame_map[player_row][player_column]=' ';
        if(next_block=='.')                // if the right block is a target, when the player moves onto it, it is shown as '+'
            mgame_map[player_row][player_column+1]='+';
        else                                // if the right block is free space, when the player moves onto it, it is whown as '@'
            mgame_map[player_row][player_column+1]='@';
        player_column++;
    }

    else if (next_block=='$'||next_block=='*'){  //if there is a box on the right,the box behind the box is checked
        if(two_next_block==' '||two_next_block=='.')   // if the block behind the box is movable into, the box and player moves, otherwise not
        {
            move_valid= true;
            if (current_block=='+')
                mgame_map[player_row][player_column]='.';
            else
                mgame_map[player_row][player_column]=' ';

            if (next_block=='*')            //if the box at the next block is on target, when it is moved, the player moves there and shown by '+'
                mgame_map[player_row][player_column+1]='+';
            else                            // if the box at the next block is on empty space, when it is moved , the player moves there and shown by '@'
                mgame_map[player_row][player_column+1]='@';

            if(two_next_block==' ')        //finally the block behind the box is checked, if it is empty when the block is moved there it is shown as '$'
              mgame_map[player_row][player_column+2]='$';
            else                          // if the block is a target, when the block moves there, it is shown as '*'
              mgame_map[player_row][player_column+2]='*';
            player_column++;
        }
        else
        move_valid=false;
    }
    else
        move_valid=false;

   return move_valid;
}

bool Sokoban::move_left(){
    bool move_valid;                      // boolean variable that takes 1 if the move is valid
    char current_block=mgame_map[player_row][player_column];
    char next_block=mgame_map[player_row][player_column-1];
    char two_next_block=mgame_map[player_row][player_column-2];

    if(next_block==' '||next_block=='.')   // checks the case if the left block is a space or a target
    {
        move_valid=true;
        if(current_block=='+')           // if the player is on a target, when it moves the block becomes a '.',if it is on space block becomes space
            mgame_map[player_row][player_column]='.';
        else
            mgame_map[player_row][player_column]=' ';
        if(next_block=='.')                // if the left block is a target, when the player moves onto it, it is shown as '+'
            mgame_map[player_row][player_column-1]='+';
        else                                // if the left block is free space, when the player moves onto it, it is whown as '@'
            mgame_map[player_row][player_column-1]='@';
        player_column--;
    }

    else if (next_block=='$'||next_block=='*'){  //if there is a box on the left,the box behind the box is checked
        if(two_next_block==' '||two_next_block=='.')   // if the block behind the box is movable into, the box and player moves, otherwise not
        {
            move_valid= true;
            if (current_block=='+')
                mgame_map[player_row][player_column]='.';
            else
                mgame_map[player_row][player_column]=' ';

            if (next_block=='*')            //if the box at the next block is on target, when it is moved, the player moves there and shown by '+'
                mgame_map[player_row][player_column-1]='+';
            else                            // if the box at the next block is on empty space, when it is moved , the player moves there and shown by '@'
                mgame_map[player_row][player_column-1]='@';

            if(two_next_block==' ')        //finally the block behind the box is checked, if it is empty when the block is moved there it is shown as '$'
              mgame_map[player_row][player_column-2]='$';
            else                          // if the block is a target, when the block moves there, it is shown as '*'
              mgame_map[player_row][player_column-2]='*';
            player_column--;
        }
        else
        move_valid=false;
    }
    else
        move_valid=false;

   return move_valid;
}



bool Sokoban::is_solved(){   // swipes the map to check if there is any boxes out of target left. If there isn't, the game is solved and 1 is returned.
for(int i=0; i<6; ++i)
 for(int j=0; j<8; ++j)
    if(mgame_map[i][j]=='$')
        return 0;
cout<<"Game solved";
 return 1;
}

void Sokoban::print_puzzle(){   //prints the puzzle as a 6x8 map on the console
for(int i=0; i<6; ++i){
 for(int j=0; j<8; ++j)
    cout<<mgame_map[i][j];
    cout<<endl;
}
cout<<endl;
}









#include <iostream>
#include <deque>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

enum MAPSIZE{
    height = 40,
    width = 120
};

void gotoxy(short x,short y){
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x,y};
    SetConsoleCursorPosition(hOut,pos);
};

void HideCursor(){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(handle,&CursorInfo);
}

class Snake{
private:
    char body; //a char flag of body or head
    char direction;//next time's direction
    int x,y;// position of each part of body
public:
    Snake(int x1,int y1,char dir){
        body = '*';
        direction = dir;
        x = x1;
        y = y1;
    }// constructor
    Snake(char b,int x1,int y1,char dir){
        body = b;
        direction = dir;
        x = x1;
        y = y1;
    }
    Snake(char b,char dir){
        body = b;
        direction = dir;
        x = 60;
        y = 20;
    }
    void move(){
        switch(direction){
            case 'w':y--;break;
            case 'a':x--;break;
            case 's':y++;break;
            case 'd':x++;break;
            default:break;
        }
    }//control the snack's moves by time.
    void change_dir(char dir){
        direction = dir;
    }//change the direction

    void change_body(){
        body = '*';
    }

    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }
    char get_dir(){
        return direction;
    }
    char get_body(){
        return  body;
    }


};

class wall{
private:
    short h;
    short w;
public:
    wall(){
        h = height;
        w = width;
    }
    void print_wall(){
        for(int i = 0;i<w;i++)
            cout<<"#";
        gotoxy(0,1);
        for(int i = 1;i<height;i++)
            cout<<"#"<<endl;
        for(int i = 1;i<height;i++){
            gotoxy(width-1,i);
            cout<<"#";
        }
        gotoxy(0,height-1);
        for(int i = 0;i<width;i++)
            cout<<"#";
        cout<<endl;
        cout<<"snake: 1.press 'w''a''s''d' to start or move;  2.'$' is food;  3.press space to pause; 4.press 'v' to speed up; 5.press 'b' to speed down;"<<endl;
    }

};

int wrongpos(int x,int y,deque<Snake>& s){
    for(auto snake : s){
        if(snake.get_x()==x&&snake.get_y()==y)
            return 1;
    }
    return 0;
}// judge if the food is overlapped with the snake.

class food{
private:
    bool flag;// a flag of if the food has been ate ,if not,true.
    int x,y;
public:
    food(){
        flag = false;
        x = 0;
        y = 0;
    }// constructor
    void create_food(deque<Snake>& s){
        do{
            srand((int)time(0));
            x = rand()%(width -3) +2;
            y = rand()%(height -3) +2;
        }while(wrongpos(x,y,s));
        gotoxy(x-1,y);
        cout<<"$";
        flag = true;
    }// create a new food.(food can't be in the same position with snack's body)
    void eat_food(){
        flag = false;
    }
    bool get_flag(){
        return flag;
    }
    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }

};


void print_snake(deque<Snake>& s){
    for(auto snake :s){
        gotoxy(snake.get_x()-1,snake.get_y());
        cout<<snake.get_body();
    }
}

void clear_snake(deque<Snake>& s){
    for(auto snake :s){
        gotoxy(snake.get_x()-1,snake.get_y());
        cout<<" ";
    }
}

int is_eat_food(deque<Snake>& s,int x,int y){
    if(s[0].get_x()==x&&s[0].get_y()==y){
        return 1;
    }
    return 0;
}

bool eat_self(deque<Snake>& s){
    deque<Snake>::iterator it = s.begin()+1;
    int x = s.front().get_x();
    int y = s.front().get_y();
    for(;it<s.end();it++){
        if(it->get_x()==x&&it->get_y()==y)
            return true;
    }
    return false;
}

double speed_up(double s){
    return s * 0.5;
};
double speed_down(double s){
    return s * 2.0;
};


int main(){
    system("mode con cols=120 lines=43");
    wall w;
    food f;
    deque<Snake> s;
    double speed = 100.0;//speed
    int score = 0;
    bool first_time = true;
    char dir;//receive the direction
    w.print_wall();
    HideCursor();
    while(true){
        while(first_time) {
            dir = getch();
            if(dir=='w'||dir=='s'||dir=='a'||dir=='d'){
                Snake head('@', dir);
                //init snake
                s.push_front(head);
                for (int i = 0; i < 4; i++) {
                    Snake body(s[i].get_x() - 1, s[i].get_y(), 'd');
                    s.push_back(body);
                }
                first_time = false;
            }else continue;
        }
        print_snake(s);
        //print score
        gotoxy(0,height+2);
        cout<<"your score: "<<score;
        //sleep some seconds
        Sleep(speed);
        //check if dead
        if(s[0].get_x()==1||s[0].get_x()==width-1||s[0].get_y()==1||s[0].get_y()==height-1) {
            gotoxy(width/2,height/2);
            cout<<"hit the wall,game over.."<<endl;
            gotoxy(width/2,height/2+2);
            cout<<"your score:"<<score<<endl;
            while(1){
                dir = getch();
                if(dir == '/n') break;
            }
            return 0;
        }
        if(eat_self(s)) {
            gotoxy(width/2,height/2);
            cout<<"eat yourself,game over.."<<endl;
            gotoxy(width/2,height/2+2);
            cout<<"your score:"<<score<<endl;
            while(1){
                dir = getch();
                if(dir == '/n') break;
            }
            return 0;
        }

        if(is_eat_food(s,f.get_x(),f.get_y())){
            f.eat_food();
            Snake body(s.back().get_x()-1,s.back().get_y(),s.back().get_dir());
            s.push_back(body);
            score ++;
        }//eat food and grow body
        if(kbhit()){
            dir = getch();
            if(dir == 'w'&&s[0].get_dir()!='w'&&s[0].get_dir()!='s') s[0].change_dir('w');
            if(dir == 'a'&&s[0].get_dir()!='a'&&s[0].get_dir()!='d') s[0].change_dir('a');
            if(dir == 's'&&s[0].get_dir()!='w'&&s[0].get_dir()!='s') s[0].change_dir('s');
            if(dir == 'd'&&s[0].get_dir()!='a'&&s[0].get_dir()!='d') s[0].change_dir('d');
            if(dir == ' ') {
                gotoxy(width/2,height/2);
                cout<<"game pause,press whatever to continue..."<<endl;
                getch();
                gotoxy(width/2,height/2);
                cout<<"                                          "<<endl;
            }
            if(dir == 'v'&&speed>=50.0) speed = speed_up(speed);
            if(dir == 'b'&&speed<=200.0) speed = speed_down(speed);
        }
         //create food
        if(!f.get_flag()){
            f.create_food(s);
        }
        clear_snake(s);
        //snake move
        Snake temp = s.front();
        s.front().change_body();
        temp.move();
        s.push_front(temp);
        s.pop_back();
    }

}
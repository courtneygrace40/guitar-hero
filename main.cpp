#include <curses.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <vector>
#include <thread>
#include <unistd.h>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
using namespace std;

//Class definitions

class Note {
    public:
        int colorText;
        int colorBack;
        int length;
        int column;
        char key;
        WINDOW * noteWin;

};

class aNote: public Note{
    public: 
        aNote(int noteLength){
            colorText = 7;
            colorBack = 8;
            column = 5;
            key = 'a';
            length = noteLength;
        }
};
class sNote: public Note{
    public: 
        sNote(int noteLength){
            colorText = 9;
            colorBack = 10;
            column = 13;
            key = 's';
            length = noteLength;
        }

};
class dNote: public Note{
    public: 
        dNote(int noteLength){
            colorText = 2;
            colorBack = 3;
            column = 21;
            key = 'd';
            length = noteLength;
        }
};
class fNote: public Note{
    public: 
        fNote(int noteLength){
            colorText = 4;
            colorBack = 6;
            column = 29;
            key = 'f';
            length = noteLength;
        }
    
};

class Song{
    public:
        int songNum;
        string songName;
        string artist;
        vector <Note> songNotes;
        string musicPath;
        int speed;
};


class User{
    public: 
        Song songChoice;
};


//Function Definitions
//Menu Functions

double stringPosFinder(string myString){
        double position = 40 - myString.length()/2;
        return(position);
    }

void screenClear(){
    clear();
    refresh();
    sleep(1);
}

Song songChoice(User user, Song songChoice){
    clear();
    refresh();
    sleep(1);
    string selectString = "Welcome to " + songChoice.songName + " by " + songChoice.artist;
    double songPos = stringPosFinder(selectString);
    user.songChoice = songChoice;
    mvprintw(5, songPos, selectString.c_str());
    refresh();
    sleep(2);
    return(songChoice);
}

//Gameplay Functions

void updateScore(int * scorePtr){
    *scorePtr += 50;
}

void userAccuracyThread(vector <chrono::system_clock::time_point*> timerPoints, int * scorePtr, char * currentNotePtr, char currentKey){
    chrono::duration <double> differenceStart = *timerPoints[0] - *timerPoints[1];
    double differenceStartActual = differenceStart.count();
    if (differenceStartActual < 100 && *currentNotePtr == currentKey){
        updateScore(scorePtr);
    }
}

void lengthAccuracyThread(vector <chrono::system_clock::time_point*> timerPoints, int * scorePtr, char * currentNotePtr, char currentKey){
    chrono::duration <double> differenceStart = *timerPoints[2] - *timerPoints[3];
    double differenceStartActual = differenceStart.count();
    if (differenceStartActual < 100 && *currentNotePtr == currentKey){
        updateScore(scorePtr);
    }
}


//animation for a single note
void singleNote(WINDOW * playerWin, Note note, vector <chrono::system_clock::time_point*> timerPoints, char* currentNote, int speed){
    int column = note.column;
    int length = note.length;
    int colorPair1 = note.colorText;
    int colorPair2 = note.colorBack;
    *currentNote = note.key;
    WINDOW * noteWin = note.noteWin;
    chrono::time_point<chrono::system_clock> compStart, compEnd; //creates clock for timing

    //majority of animations with the o as the guide
    for (int i = 1; i < 15; ++i){

        wattron(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, i, column, "o");

        if (i > 1 && length >= 2){
            mvwprintw(playerWin, i-1, column, "|"); //length 2
        } 
        if (i > 2 && length >= 3){
            mvwprintw(playerWin, i-2, column, "|"); //length 3
        }
        if (i > 3 && length >= 4){
            mvwprintw(playerWin, i-3, column, "|"); //length 3
        }
    
        wrefresh(playerWin);
        wattroff(playerWin, COLOR_PAIR(colorPair1));
        wattron(playerWin, COLOR_PAIR(1));

    
        refresh();
        wrefresh(playerWin);
        
        usleep(speed);
        //j must change based on length of note

        for (int j = length + 1; j < 15; ++j){ // every time it rewrites every line as an | 
            mvwprintw(playerWin, j-length, column, "|"); //j-1 or begins as 1
          
            //mvwprintw(win, row+i-1, column4, "|");
        }

        wrefresh(playerWin);
        wattroff(playerWin, COLOR_PAIR(1));

    }

    int y = length;

    
    
    
    compStart = chrono::system_clock::now(); //this is when the note has to be pressed
    *timerPoints[1] = compStart;

    //ending animations to clear the screen correctly but continue the length of the note
    if (length >= 1){
        wbkgd(noteWin, COLOR_PAIR(colorPair2));
        wrefresh(noteWin);
        wrefresh(playerWin);
    }
    if (length >= 4){
        mvwprintw(playerWin, 11, column, "|"); 
        wattron(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 14, column, "|");
        mvwprintw(playerWin, 13, column, "|");
        mvwprintw(playerWin, 12, column, "|");
        wrefresh(noteWin);
        wrefresh(playerWin);
        usleep(speed);
        wattroff(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 12, column, "|");
        wrefresh(playerWin);
    } 

    if (length >= 3){
        mvwprintw(playerWin, 12, column, "|"); 
        wattron(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 14, column, "|");
        mvwprintw(playerWin, 13, column, "|");
        wrefresh(noteWin);
        wrefresh(playerWin);
        usleep(speed);
        wattroff(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 13, column, "|");
        wrefresh(playerWin);
    }  

    if (length >=2){
        wattron(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 14, column, "|");
        wrefresh(playerWin);
        usleep(speed);
        wattroff(playerWin, COLOR_PAIR(colorPair1));
        mvwprintw(playerWin, 14, column, "|");
        wrefresh(playerWin);
    }   
        
    
    
    
    if (length >= 1){
        mvwprintw(playerWin, 14, column, "|");
    }
    if (length >= 2){
        mvwprintw(playerWin, 13, column, "|");
    } 
    if (length >= 3){
        mvwprintw(playerWin, 12, column, "|"); 
    }
    if (length >= 4){
        mvwprintw(playerWin, 11, column, "|");
    }

    if (length == 1){
        wbkgd(noteWin, COLOR_PAIR(colorPair2));
        wrefresh(noteWin);
        wrefresh(playerWin);
        usleep(speed);
    }

    wbkgd(noteWin, COLOR_PAIR(5));
    compEnd = chrono::system_clock::now(); //ends the clock 
    *timerPoints[3] = compStart;
    wrefresh(noteWin);
    wrefresh(playerWin);
    usleep(1000000);
    
}

//turns on attribute 
void attrOn(WINDOW * win, int colorPair1, int colorPair2){
    wbkgd(win,COLOR_PAIR(colorPair1));
    wattron(win, COLOR_PAIR(colorPair2));
    wrefresh(win);
}

//creates columns in the windows
void columnCreator(WINDOW * win, int columnVal){
    int height = 16;
    for (int i = 0; i < height; ++i){
        mvwprintw(win, i, columnVal, "|"); //increments row by 1, beginning at row 0 in relation to the window 
    }
    wrefresh(win);
}

//creates boxs
void boxCreator(WINDOW * win){
    box(win, 0, 0);
    wrefresh(win);
}

//function called when a key is pressed
void keyPress(Note note, WINDOW * win){
    WINDOW * noteWin = note.noteWin;
    wbkgd(noteWin, COLOR_PAIR(11));
    wattron(win, COLOR_PAIR(12));
    int column = note.column; //gets the note column
    for (int i = 1; i < 15; ++i){
        mvwprintw(win, i, column-1, "|"); //increments row by 1, beginning at row 0 in relation to the window 
    }
    for (int i = 1; i < 15; ++i){
        mvwprintw(win, i, column+1, "|"); //increments row by 1, beginning at row 0 in relation to the window 
    }
    wrefresh(win);
    wrefresh(noteWin);

    wattroff(win, COLOR_PAIR(12));
}

//function called when key is released, erases every possible highlight at once
void keyRelease(WINDOW * win, vector <WINDOW *> windows){
    for (WINDOW * win: windows){
        wbkgd(win, COLOR_PAIR(5)); //resets the windows 
    }
    vector <int> columns = {5, 13, 21, 29}; //column vector to reset all of the columns
    for (int column: columns){
        for (int i = 1; i < 15; ++i){
            mvwprintw(win, i, column-1, " "); //increments row by 1, beginning at row 0 in relation to the window 
        }
        for (int i = 1; i < 15; ++i){
            mvwprintw(win, i, column+1, " "); //increments row by 1, beginning at row 0 in relation to the window 
        }
    }
    wrefresh(win);
    for (WINDOW * win: windows){
        wrefresh(win);
    }
    
}

//the thread for animation
void animationThread(WINDOW * win, vector<Note> notes, vector <chrono::system_clock::time_point*> timerPoints, bool* threadFinishPtr, int* scorePtr, char* currentNotePtr, int speed){
    for (Note note: notes){ //loops through the vector of notes
        singleNote(win, note, timerPoints, currentNotePtr, speed);
    }
    *threadFinishPtr = true;
}

//the thread for user input 
void userInputThread(vector <WINDOW *> windows, WINDOW * win, vector <chrono::system_clock::time_point*> timerPoints, int * scorePtr, bool* threadFinishPtr, char* currentNotePtr){
    const int timeoutMS = 200;
    bool keyHeld = false;

    //random lengths, just need the note key but length is needed to create note
    aNote aNote1(1);
    aNote1.noteWin = windows[0];
    sNote sNote1(1);
    sNote1.noteWin = windows[1];
    dNote dNote1(1);
    dNote1.noteWin = windows[2];
    fNote fNote1(1);
    fNote1.noteWin = windows[3];

    //checks to see if the animation is still running 
    //tracks which key is pressed through this loop 
    while (*threadFinishPtr == false){
        chrono::time_point<chrono::system_clock> userStart, userEnd;
        int ch = wgetch(stdscr);
        timeout(500);
        if(ch ==  aNote1.key){ //s
            userStart = chrono::system_clock::now();
            keyPress(aNote1, win);
            *timerPoints[0] = userStart;
            userAccuracyThread(timerPoints, scorePtr, currentNotePtr, aNote1.key);
        }else if (ch == sNote1.key){
            keyPress(sNote1, win);
            userAccuracyThread(timerPoints, scorePtr, currentNotePtr, sNote1.key);
        }else if (ch == dNote1.key){
            keyPress(dNote1, win);
            userAccuracyThread(timerPoints, scorePtr, currentNotePtr, dNote1.key);
        }else if (ch == fNote1.key){
            keyPress(fNote1, win);
            userAccuracyThread(timerPoints, scorePtr, currentNotePtr, fNote1.key);
        }
        else if (ch == ERR){ //
            keyRelease(win, windows);
            userEnd = chrono::system_clock::now();
            *timerPoints[2] = userEnd;
        }
           
    }
}

//the thread to stream music 
void streamMusic(const string& filePath, bool* isPlayingPtr){
    sf::Music music;

   if (!music.openFromFile(filePath)) {
        std::cerr << "Error loading music file: " << filePath << std::endl;
        return;
    }
    
    music.setLoop(true); //keep music playing 
    music.setVolume(50);

    music.play();

    while(*isPlayingPtr){ //prevents lag 
        sf::sleep(sf::milliseconds(100)); 
    }

    music.stop();

}

//displays the song as the user plays 
void songDisplayThread(Song song){
    string songName = song.songName;
    string artistName = song.artist;
    mvprintw(4, 51, songName.c_str());
    mvprintw(5, 51, artistName.c_str());
}



//terminal is 80x24
int main() {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    cbreak();

    //Section to create all of the variables and their pointers 
    chrono::system_clock::time_point userStartTime;
    chrono::system_clock::time_point compStartTime;
    chrono::system_clock::time_point userEndTime;
    chrono::system_clock::time_point compEndTime;

    chrono::system_clock::time_point* ptrToUStartTime = &userStartTime;
    chrono::system_clock::time_point* ptrToCStartTime = &compStartTime;
    chrono::system_clock::time_point* ptrToUEndTime = &userEndTime;
    chrono::system_clock::time_point* ptrToCEndTime = &compEndTime;

    int score = 0;
    int* scorePtr = &score;

    bool threadFinish = false;
    bool* threadFinishPtr = &threadFinish;

    char currentNote;
    char* currentNotePtr = &currentNote;

    bool isPlaying = true;
    bool* isPlayingPtr = &isPlaying;


    vector <chrono::system_clock::time_point*> timerPoints = {ptrToUStartTime, ptrToCStartTime, ptrToUEndTime, ptrToCEndTime};


    //Color Pairs
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));


    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
    
    //d key
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);

    //f key
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLUE);

    //a key 
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
    init_pair(8, COLOR_GREEN, COLOR_GREEN);

    //s key 
    init_pair(9, COLOR_CYAN, COLOR_BLACK);
    init_pair(10, COLOR_CYAN, COLOR_CYAN);

    init_pair(12, COLOR_WHITE, COLOR_BLACK);
    init_pair(11, COLOR_WHITE, COLOR_WHITE);

    clear();

    attron(COLOR_PAIR(1));


    //logo created by https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
    const char* logo = R"(
        ________      .__  __                   ___ ___                       
       /  _____/ __ __|__|/  |______ _______   /   |   \   ___________  ____  
      /   \  ___|  |  \  \   __\__  \\_  __ \ /    ~    \_/ __ \_  __ \/  _ \ 
     \    \_\  \  |  /  ||  |  / __ \|  | \/ \    Y    /\  ___/|  | \(  <_> )
      \______  /____/|__||__| (____  /__|     \___|_  /  \___  >__|   \____/ 
             \/                    \/               \/       \/              
    )";

   
    double welcome1pos = stringPosFinder("Are you ready to ROCK?");
    double welcome2pos = stringPosFinder("Press RETURN to begin.");


    mvprintw(0, 25, "%s", logo);

    mvprintw(8, welcome1pos, "Are you ready to ROCK?");
    mvprintw(10, welcome2pos, "Press RETURN to begin.");

    int ch = wgetch(stdscr);


    if (ch == '\n' or ch == '\r'){
        screenClear();
    }

    User user;
    Song pinkPonyClub;
    Song sparksFly;


    //SONG SETUP 

    pinkPonyClub.songNum = 1;
    sparksFly.songNum = 2;
    pinkPonyClub.songName = "Pink Pony Club";
    sparksFly.songName = "Sparks Fly";
    pinkPonyClub.artist = "Chappell Roan";
    sparksFly.artist = "Taylor Swift";

    pinkPonyClub.musicPath = "/Users/courtneysweeney/CS330/GuitarHeroGame/assets/audio/pinkponyclub.wav";
    sparksFly.musicPath = "/Users/courtneysweeney/CS330/GuitarHeroGame/assets/audio/sparksfly.wav";

    pinkPonyClub.speed = 150000;
    sparksFly.speed = 60000;
    
    double menu1pos = stringPosFinder("CHOOSE SONG");
    double menu2pos = stringPosFinder("Pink Pony Club by Chappell Roan [EASY] (1)");
    double menu3pos = stringPosFinder("Sparks Fly by Taylor Swift [HARD] (2)");

    mvprintw(5, menu1pos, "CHOOSE SONG");
    mvprintw(7, menu2pos, "Pink Pony Club by Chappell Roan [EASY] (1)");
    mvprintw(8, menu3pos, "Sparks Fly by Taylor Swift [HARD] (2)");

    int ch2 = wgetch(stdscr);
    Song song;
  
    //gets user song choice
    if (ch2 == '1'){
        song = songChoice(user, pinkPonyClub);
    } else if (ch2 == '2'){
        song = songChoice(user, sparksFly);
    }


    clear();
    refresh();

    //displays the instructions before the game begins 
    mvprintw(5, 5, "INSTRUCTIONS");
    mvprintw(7, 5, "Use the A, S, D, F keys.");
    mvprintw(8, 5, "The keys match where the strings are.");
    mvprintw(9, 5, "Time your key press with the note and hold for the length of the note.");
    
    user.songChoice = song;
    

    refresh();

    usleep(10000000);

    clear();
    refresh();


    //GAME BEGINS HERE 

    //windows are drawn 
    WINDOW * win = newwin(20, 35, 1, 3); //big window
    WINDOW * win3 = newwin(3, 7, 16, 5); //5 is where it starts, width of 4?? i think, so line at 7? 
    WINDOW * win4 = newwin(3, 7, 16, 13); // 13 is where it starts 
    WINDOW * win5 = newwin(3, 7, 16, 21);
    WINDOW * win6 = newwin(3, 7, 16, 29);
    

    vector <WINDOW *> bigWindows = {win};
    vector <WINDOW *> windows = {win3, win4, win5, win6};

    int row = 0;
    int column1 = 5;
    int column2 = 13;
    int column3 = 21;
    int column4 = 29;
    int height = 16;

    for (WINDOW * win: bigWindows){
        attrOn(win, 1, 1);
        columnCreator(win, column1);
        columnCreator(win, column2);
        columnCreator(win, column3);
        columnCreator(win, column4);
        boxCreator(win);
    }

    

    for (WINDOW * win: windows){
        attrOn(win, 5, 5);
        boxCreator(win);
    }

    refresh();

    //NOTE SETUP 
    aNote a1(1);
    a1.noteWin = win3;
    aNote a2(2);
    a2.noteWin = win3;
    aNote a3(3);
    a3.noteWin = win3;
    aNote a4(4);
    a4.noteWin = win3;

    sNote s1(1);
    s1.noteWin = win4;
    sNote s2(2);
    s2.noteWin = win4;
    sNote s3(3);
    s3.noteWin = win4;
    sNote s4(4);
    s4.noteWin = win4;
    
    dNote d1(1);
    d1.noteWin = win5;
    dNote d2(2);
    d2.noteWin = win5;
    dNote d3(3);
    d3.noteWin = win5;
    dNote d4(4);
    d4.noteWin = win5;

    fNote f1(1);
    f1.noteWin = win6;
    fNote f2(2);
    f2.noteWin = win6;
    fNote f3(3);
    f3.noteWin = win6;
    fNote f4(4);
    f4.noteWin = win6;
    

    vector <Note> notesList = {a1, a2, a3, a4, s1, s2, s3, s4, d1, d2, d3, d4, f1, f2, f3, f4};

    //a = win3, s = win4, d = win5, = win6

    vector <Note> sparksNotes = {d4, f1, d4, f1, d4, f1, a2, s3, d3, f1, d1, s1, a1, a4, f4, d1, f1, s1, a1, f3, d3, s3, f4, d4, s4, a1, f1, s1, d1, s2, a2, f1, s3, f2, f4, f3, d2, a1, f1, s1, d1, a1, f1, s1, d1, a1, f1, s1, d1};
    vector <Note> ppCNotes = {f4, d4, s4, a4, f4, d4, s4, a4, a1, a1, s1, s1, d1, d1, f1, f1, d4, s3, d4, f4, a3, s4, a4, f4, f4, d3, a4};

    vector <Note> notes;
  
    if (ch2 == '1'){
        notes = ppCNotes;
    } else if (ch2 == '2'){
        notes = sparksNotes;
    }
    
    //all of the threads happening simultaneously 
    thread animationThread1(animationThread, win, notes, timerPoints, threadFinishPtr, scorePtr, currentNotePtr, song.speed);
    thread userInputThread1(userInputThread, windows, win, timerPoints, scorePtr, threadFinishPtr, currentNotePtr);
    thread musicThread(streamMusic, song.musicPath, isPlayingPtr);
    thread songNameThread(songDisplayThread, song);

    animationThread1.join();
    userInputThread1.join();
    songNameThread.join();
    
    //ends the music thread 
    *isPlayingPtr = false;

    //end screen with user score 
    clear();
    refresh();
    mvprintw(2, 30, "Thank you for playing!");
    mvprintw(4, 30, "User Score:");
    mvprintw(5, 30, "%d", score);

    refresh();


    this_thread::sleep_for(chrono::milliseconds(7000));


    getch();
    endwin();

    return 0;

}
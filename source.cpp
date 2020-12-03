#include "oreganTrail.cpp"

using namespace std;


// One piece theme!
int main(){
    string sInput;
    bool playThisSession = true, debug = false;
    ErrorHandler oopsMain("main()");

    cls();
    cout << "Press Enter To Play\n";
    getline(cin, sInput);
    oopsMain.checkResponse(sInput);
    if(sInput == "debug"){
        debug = true;
    }

    // we loop through the game until the user no longer wants to play
    while(playThisSession){
        cout << "Feel free to type '!play' at any time to exit the game!\nThis game displays dates as dd, mm, yyyy\n";
        // most of the game is housed in the core function becuase I think it looks nicer :)
        core(debug);
        while(1){
            cout << "Play again? > ";
            cin >> sInput;
            if(oopsMain.checkResponse(sInput) == 2) cout << "Log was cleared\n";
            else if(oopsMain.checkResponse(sInput) != -1) break;
            cout << "Please input a valid yes or no response.\n";
        }
        if(oopsMain.checkResponse(sInput) == 0) continue;
        if(oopsMain.checkResponse(sInput) == 1) break;
    }
    return 0;
}
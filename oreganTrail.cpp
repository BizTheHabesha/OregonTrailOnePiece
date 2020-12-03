#include "oreganTrail.hpp"
#include "usefulFunctions.hpp"

#ifdef _WIN32
    #include "wait/win.cpp"
#endif
#ifdef __unix__
    #include "wait/uni.cpp"
#endif

using namespace std;
string STDEXCEPTIONLOGGER = "Exception caught: ";

// Error Handler Class
ErrorHandler::ErrorHandler(){
    location = "_undefined";
}
ErrorHandler::ErrorHandler(string thisLocation){
    location = thisLocation;
}
void ErrorHandler::setLocation(string thisLocation){
    location = thisLocation;
}
void ErrorHandler::logError(string error){
    logThis("from "+location+" > "+error);
    if(error == "fatal") cout << "a fatal error has occured in " << location << endl;
}
int ErrorHandler::validString(string aString){
    // if we find a charcter from the list of invalid character in the given string we log it and return -1
    if(aString.find_first_of(invalidchars) != string::npos){
        logError("invalid string");
        return -1;
    }else if(aString.empty()){
        logError("empty string");
        return -2;
    }else return 0;
}
int ErrorHandler::validStartingDay(int aMonth, int aDay){
    // if the monthy is March or May then the day must be between 1 and 31 days
    if(aMonth == 3 || aMonth == 5){
        if(aDay >= 1 && aDay <= 31) return 0;
        else cout << "Please pick a day between 1 and 31\n";
    // if the month is April, the day must be between 1 and 30
    }else if(aMonth == 4){
        if(aDay >= 1 && aDay <= 30) return 0;
        else cout << "Please pick a day between 1 and 30\n";
    // if the month is incorrect, then we've had an incorrect month (shouldn't be possible)
    }else{
        logError("invalid month in ErrorHandler::validStartingDay()");
        return -2;
    }
    // if the day is incorrect then we log it and return -1
    logError("invalid starting day");
    return -1;
}
int ErrorHandler::validStartingMonth(int aMonth){
    // the month must be between 3 and 5, else log and return -1
    if(aMonth >= 3 && aMonth <= 5) return 0;
    else{
        logError("invalid starting month");
        return -1;
    }
}
int ErrorHandler::checkResponse(string sInput){
    // check for all of the yes responses
    for(int i = 0; i < 6; i++){
        if(lowercase(sInput) == validYes[i]) return 0;
    }
    // check for all of the no responses
    for(int i = 0; i < 7; i++){
        if(lowercase(sInput) == validNo[i]) return 1;
    }
    // check if the user is trying to clear the log
    if(sInput == "clearlog"){
        clearLog(0);
        return 2;
    }
    if(sInput == "cls()"){
        cls();
        return -1;
    }
    // check if the response is a valid integer
    if(trySTOI(sInput) != -1){
        return 3;
    }
    if(sInput == "hunt" || sInput == "rest" || sInput == "continue"){
        return 4;
    }
    // for for all of the exit responses
    for(int i = 0; i < 4; i++){
        if(sInput == validExit[i]){
            logThis("User ended the game abruptly.");
            exit(EXIT_SUCCESS);
        }
    }
    return -1;
}
int ErrorHandler::trySTOI(string sInput){//holds the try catch block for a standard stoi operation, returns input integer
    int num;
    ErrorHandler oopsTrySTOI("trySTOI()");
    try{
        num = stoi(sInput);
        return num;
    }catch(const invalid_argument& ia){
        oopsTrySTOI.logError("not a valid integer!");
        return -1;
    }catch(const out_of_range& oor){
        oopsTrySTOI.logError("out of range");
        return -1;
    }catch(exception& ex){ 
        oopsTrySTOI.logError("Undefined error: "+string(ex.what()));
        cout << "Sorry, something unexpected happend! Please check the gamelog\n";
        return -1;
    }catch(...){
        cerr << "(trySTOI) Error: --Unexpected-Throw catch_all abort()\n";
        oopsTrySTOI.logError("Unexpected Error. The game was aborted (exit status: EXIT_FAILURE)");
        exit(EXIT_FAILURE);
    }
    return -1;
}
string ErrorHandler::getInvalidChars(){
    return invalidchars;
}
string ErrorHandler::getMonth(int aMonth){
    return months[aMonth-1];
}
void ErrorHandler::codeHandler(string aCode){
    if(aCode == "fatal" || aCode == "fail"){
        logError("fatal error, aborting");
        exit(EXIT_FAILURE);
    }
}
void ErrorHandler::codeHandler(int aCode){
    logError("error code: "+to_string(aCode));
}

// Traveler Class
Traveler::Traveler(){
        name = "_undefined";
        health = 100;
        alive = true;
        oopsTraveler.setLocation("Traveler::Traveler()");
    }
Traveler::Traveler(string aName){
        name = aName;
        health = 100;
        alive = true;
        oopsTraveler.setLocation("Traveler::Traveler()");
    }
string Traveler::getName(){
    return name;
}
void Traveler::resurection(){
    health = 1;
    alive = true;
}
void Traveler::kill(){
    health = 0;
    alive = false;
}
bool Traveler::getLiving(){
    return alive;
}
int Traveler::setName(string aName){
    oopsTraveler.setLocation("Traveler::setName()");
    int checkString = oopsTraveler.validString(aName);
    if(checkString == 0){
        name = aName;
        logThis("Welcome, "+aName+", to the squad!");
    }
    if(checkString == -1) oopsTraveler.logError("couldn't set name because the string was invalid");
    if(checkString == -2) oopsTraveler.logError("couldn't set name because the string was empty");

    return checkString;
}
int Traveler::setHealth(char operand, int increment){
        if(operand == '+') health+=increment;
        else if(operand == '-') health-=increment;
        else if(operand == '*') health*=increment;
        else if(operand == '/') health/=increment;
        else return -1;
        return 0;
    }
int Traveler::getHealth(){
        return health;
    }

// Store class
Store::Store(){
    storeName = "_undefined";
    oopsStore.setLocation("Store::Store()");
    logThis("store constructed");
}
Store::Store(string aName){
    storeName = aName;
    oopsStore.setLocation("Store::Store() (parameterized)");
    logThis("store constructed");
}
int Store::buy(int item, int quantity, int money){
    double finalAmount = costs[item]*quantity;
    vector <int> stats;
    if(money > finalAmount){
        if(item == 1) centerThis("cout", to_string(quantity)+" pound(s) of food will cost you:");
        else centerThis("cout", to_string(quantity)+" "+items[item]+"(s) will cost you: ");
        centerThis("cout", "~ "+to_string(finalAmount)+" ~");
    }else{
        centerThis("cout", "You're too broke to buy "+to_string(quantity)+" "+items[item]);
        return -1;
    }
    return 0;
}
double Store::getCost(int index){
    return costs[index];
}
string Store::getItem(int index){
    return items[index];
}
vector <int> Store::visit(vector <int> stats){
    storesVisited++;
    string sInput, numSpaces;
    int iInput;
    oopsStore.setLocation("Store::visit()");

    logThis("Player visited "+storeName);

    while(1){
        centerThis("cout", "~~~ Welcome to "+storeName+" ~~~");
        centerThis("cout", "~~~ What would you like to buy? ~~~");
        numSpaces = centerThis("numspaces", "~~~ What would you like to buy? ~~~");
        cout << endl;
        if((sizeof(items)/sizeof(items[0])) != (sizeof(costs)/sizeof(costs[0]))){
            oopsStore.logError("Fatal: items[] and costs[] are not of equal size");
            oopsStore.codeHandler("fatal");
        }
        cout << numSpaces << " [1] " << items[0] << endl;
        cout << numSpaces << " [2] " << items[1] << endl;
        cout << numSpaces << " [3] " << items[2] << endl;
        cout << numSpaces << " [4] Miscellaneous Supplies" << endl;
        cout << numSpaces << " [5] Leave" << endl;
        cout << numSpaces << "In your wallet: ¥" << stats[7] << endl;
        cout << " > ";
        getline(cin, sInput);
        oopsStore.checkResponse(sInput);
        if(sInput == "1"){

            continue;
        }
        if(sInput == "2") continue;
        if(sInput == "3") continue;
        if(sInput == "4") continue;
        if(sInput == "5"){
            cout << "Goodbye, please visit " << storeName << " again soon.\n";
            break;
        }
        cout << "Please input a valid option!\n";
    }
    return stats;
}

// Event class
Event::Event(){
    eventTitle = "_undefined";
}
Event::Event(string aTitle){
    eventTitle = aTitle;
}

// useful functions
string lowercase(string input){
    for(int i = 0; i < input.length(); i++){
        if(input[i] <= 90 && input[i] >= 65) input[i]+=32;
    }
    return input;
};
string centerThis(string dothis, string input){
    // std centering of 88 spaces
    string spaces;
    int strLength = input.length();
    int numSpaces = 96;

    // if the length is even, divide by two
    if(strLength % 2 == 0) numSpaces-=(strLength/2);
    // if its odd then subtract one then divide by two (cant divide an odd integer by 2 properly)
    else{
        strLength++;
        numSpaces-=(strLength/2);
    }
    // add spaces
    for(int i = 0; i < numSpaces; i++) spaces.insert(0, " ");
    input.insert(0, spaces);
    // if we specify cout, then cout
    if(dothis == "cout") cout << input << endl;
    // if we specify for the number of spaces, then return that
    else if(dothis == "numspaces") return spaces;
    // if we specify return then return
    else if(dothis == "return") return input;
    else if(!dothis.empty()){
        return " ! invalid operator in centerThis !\n";
        exit(EXIT_FAILURE);
    }
    // by default, return an empty string
    return "";
}

// crucial game mechanics
int logThis(string toLog){
    string holder;
    time_t currenttime;
    fstream gamelog, gamelogmd, gamelogtxt;
    ErrorHandler oopsLogger("logThis()");
    int linesInMd = 0;
    struct tm * ti; 

    try{
        time (&currenttime);
        ti = localtime(&currenttime);
    }catch(exception& ex){
        cerr << "from logThis > " << ex.what() << endl;
        cout << "from logThis > fatal error.\nAborting..." << endl;
        abort();
    }
    // open the markdown game log in append mode
    gamelog.open("gamelog.md", fstream::app);
    // if the file failed to open, reuturn -1
    if(gamelog.fail()){
        cout << "failed to log md.\n";
        return -1;
    // if the file failed to open, reuturn -1
    }else if(!gamelog.is_open()){
        cout << "failed to log md.\n";
        return -1;
    }
    // if the log item is empty, we say that there is nothing to log
    if(toLog.empty() || toLog.size() == 0){
        gamelog << " - nothing to log\n";
        return 0;
    }
    // append the time followed by the log message
    gamelog << '\n' << asctime(ti) << " - " << toLog << endl;
    gamelog.close();

    // same as above but for the text file
    gamelog.open("gamelog.txt", fstream::app);
    if(gamelog.fail()){
        cout << "failed to log txt.\n";
        return -1;
    }else if(!gamelog.is_open()){
        cout << "failed to log txt.\n";
        return -1;
    }
    if(toLog.empty()){
        gamelog << " - nothing to log\n";
        return 0;
    }
    gamelog << '\n' << asctime(ti) << "\t" << toLog << endl;
    gamelog.close();

    return 0;
}
void clearLog(int when){
    struct tm * ti;
    time_t currenttime;
    ofstream gamelog;
    ErrorHandler oopsClearLog("clearLog()");

    try{   
        time (&currenttime);   
        ti = localtime(&currenttime);
    }catch(exception& ex){
        cerr << STDEXCEPTIONLOGGER << ex.what() << endl;
        logThis("from clearLog() > "+STDEXCEPTIONLOGGER+ex.what());
    }
    // open the markdown log file in truncation mode, which will delete everything when something is written
    gamelog.open("gamelog.md", ofstream::out | ofstream::trunc);
    // entering clearLog with code 0 means it was done by the user via there own input
    if(when == 0) gamelog << "**Log was cleared**: " << asctime(ti);
    // any other code means it is already in the code (usually code -1)
    else gamelog << "**Log was automatically cleared**: " << asctime(ti);
    // close the markdown file
    gamelog.close();

    // same as above but for the text file
    gamelog.open("gamelog.txt", ofstream::out | ofstream::trunc);
    if(when == 0) gamelog << "Log was cleared: " << asctime(ti);
    else gamelog << "Log was automatically cleared: " << asctime(ti);
    gamelog.close();
}
void initializeLog(){
    string holder;
    int numEntries = 0;
    struct tm * ti;
    ErrorHandler oopsInitLog("initializeLog()");

    try{
        time_t currenttime;   
        time (&currenttime);   
        ti = localtime(&currenttime);
    }catch(exception& ex){
        cerr << STDEXCEPTIONLOGGER << ex.what() << endl;
        logThis(STDEXCEPTIONLOGGER+ex.what());
    }
    
    fstream gamelog;
    gamelog.open("gamelog.md");
    while(getline(gamelog, holder, '\n')){
        numEntries++;
    }
    if(numEntries >= 200) clearLog(1);
    gamelog.close();
    // log the initialization in md
    gamelog.open("gamelog.md", ofstream::app);
    gamelog << "\n**Game started**: " << asctime(ti);
    gamelog.close();
    // log the initialization in txt
    gamelog.open("gamelog.txt", ofstream::app);
    gamelog << "\n**Game started**: " << asctime(ti);
    gamelog.close();
}
void spLoad(){
    for(int i = 0; i < 6; i++){
        cout << '.';
        wait(0.5);
    }
    cout << endl;
    return;
}
int restThisTurn(){
    cout << "rested\n";
    return 0;
}
int huntThisTurn(){
    cout << "food time\n";
    return 0;
}
int howManyAlive(Traveler aTravelers[]){
    int numAlive = 0;
    for(int i = 0; i < 5; i++){
        if(aTravelers[i].getLiving()) numAlive++;
    }
    return numAlive;
}
void getName(int index, Traveler aTravelers[]){
    ErrorHandler oopsGetName("getName()");
    string invalidChars = oopsGetName.getInvalidChars();
    bool firstTime = true;
    while(1){
        string sInput;
        if(!firstTime) cout << " > ";
        getline(cin, sInput);
        oopsGetName.checkResponse(sInput);
        int intVR = aTravelers[index].setName(sInput);
        if(intVR == 0) break;
        else if(intVR == -2) cout << "Please input a name\n";
        else cout << "That name contains invalid character(s). These include: \"" << invalidChars << "\"\n";
        firstTime = false;
        cls();
    }
    cls();
}
string getInGameDate(int numdays){
    // 275 days including november 30th
    ErrorHandler oopsGIGD("getInGameDate()");
    string month, sDay, year = "1847";
    int iDay;
    if(numdays < 1){
        oopsGIGD.logError("invalid numdays");
        return "_error";
    }
    if(numdays <= 31){
        month = "March";
        iDay = numdays;
    }else if(numdays <= 61){
        month = "April";
        iDay = numdays - 31;
    }else if(numdays <= 92){
        month = "May";
        iDay = numdays - 61;
    }else if(numdays <= 122){
        month = "June";
        iDay = numdays - 92;
    }else if(numdays <= 153){
        month = "July";
        iDay = numdays - 122;
    }else if(numdays <= 184){
        month = "August";
        iDay = numdays - 153;
    }else if(numdays <= 214){
        month = "September";
        iDay = numdays - 184;
    }else if(numdays <= 245){
        month = "October";
        iDay = numdays - 214;
    }else if(numdays <= 275){
        month = "November";
        iDay = numdays - 245;
    }else if(numdays <= 275){
        month = "December";
        iDay = numdays - 275;
    }

    sDay = to_string(iDay);
    if((iDay - 1) % 10 == 0) sDay+="st";
    else if((iDay - 2) % 10 == 0) sDay+="nd";
    else if((iDay - 3) % 10 == 0) sDay+="rd";
    else sDay+="th";
    
    return sDay+" of "+month+", "+year;
}
int statusUpdate(vector <int> stats, int numdays){
    ErrorHandler oopsStatusUpdate("statusUpdate()");
    string holder = "    ";
    string sSpacesRequired;
    int iSpacesRequired;
    int spacesRequiredArr[8] = {20,32,20,18,11,11,16, 9};

    cout << "Here are your current stats:\n";
    centerThis("cout", "It is currently "+getInGameDate(numdays));
    centerThis("cout", "Miles Traveled:     Distance to Fishman Island:     Food (in lbs.):     Cannon Balls:     Money:     Sails:     Ship Parts:     Med Kits:");
    for(int i = 0; i < 7; i++){
        iSpacesRequired = spacesRequiredArr[i] - to_string(stats[i]).length();
        for(int i = 0; i < iSpacesRequired; i++){
            sSpacesRequired.push_back(' ');
        }
        holder+=to_string(stats[i+3]);
        holder+=sSpacesRequired;
        //cout << stats[i+3] << sSpacesRequired;
        sSpacesRequired.clear();
    }
    centerThis("cout", holder);
    cout << "\n\n";
    centerThis("cout", "~~~ Press enter when you're ready to continue ~~~");
    getline(cin, holder);
    oopsStatusUpdate.setLocation("Store::unspecified");
    return 0;
};
int core(bool debug){
    srand((unsigned) time(0));
    initializeLog();

    Traveler travelers[5];
    Store stores[15];
    bool visited[15], defaultDate = false;
    int intResponse = -1, month, day, numdays = 275;
    ErrorHandler oopsCore("core() stats");
    string milestoneNames[8] = {"Shell Town", "Cocoyasi Village", "Syrup Village", "The Baratie", "Drum Island"};
    string invalidChars = oopsCore.getInvalidChars();
    string sInput, crewName;
    // stores miles traveled, miles left to Fishman Island, food, cannon balls, money, sails, ship parts, medical kits
    vector <int> vecStats = {0, 2040, 100, 0, 1600, 0, 0, 0};

    // initialize the boolean array
    for(int i = 0; i < 15; i++) visited[i] = false;

    oopsCore.setLocation("core() pregame");
    centerThis("cout", "Several years ago, the infamous pirate Gold D Roger was captured and put to death by the world governemnt.");
    centerThis("cout", "The world government's goal was to discourage pirates from ever sailing the sea again, but during his execution, Roger would say something that would change the world.");
    centerThis("cout", "\"You want my treausre? You can have it!\". These riviting last words would bring about a new era:");
    wait(1.01);
    centerThis("cout", "~~~ THE GREAT PIRATE ERA! ~~~\n");
    wait(0.5);
    centerThis("cout", "Whether or not he knew it, Roger had just set in motion the greatest events of our lifetime.");
    wait(0.5);
    centerThis("cout", "Touched by Rogers' words, you band together a rag tag group of pirates and set out to sea!");
    wait(0.5);
    centerThis("cout", "Your goal is the new world, but first you must survive");
    wait(0.5);
    centerThis("cout", "~~~ THE GRAND LINE ~~~");
    wait(0.5);
    
    // get a valid starting month
    oopsCore.setLocation("core() starting date");
    do{
        cout << "You can pick any date between 01, 03, 1847 and 01, 05, 1847 to start your journey\n";
        cout << "In what month will your crew be heading out? > ";
        getline(cin, sInput);
        oopsCore.checkResponse(sInput);

        if(oopsCore.validString(sInput) == -2){
            cout << "Default date of 28, 03, 1847 selected\n";
            month = 3;
            defaultDate = true;
            wait(5);
            cls();
            break;
        }else if(oopsCore.trySTOI(sInput) == -1){
            cout << "Please input a valid number\n";
        }else if(oopsCore.validStartingMonth(stoi(sInput)) == -1){
            cout << "Please input a valid month\n";
        }else{
            month = stoi(sInput);
            wait(5);
            cls();
            break;
        }
        cls();
    }while(1);
    // get a valid starting day
    day = 28;
    while(!defaultDate){
        cout << "On which day will your crew be heading out? > ";
        getline(cin, sInput);
        oopsCore.checkResponse(sInput);
        if(oopsCore.trySTOI(sInput) == -1){
            cout << "Please input a valid number\n";
        }else if(oopsCore.validStartingDay(month, stoi(sInput)) == -1){
            cout << "Please input a valid day\n";
        }else{
            day = stoi(sInput);
            wait(5);
            cls();
            break;
        }
        cls();
    }
    // convert the starting date to numdays
    if(month == 3) numdays == day;
    else if(month == 4) numdays == day + 31;
    else if(month == 5) numdays == day + 30; 

    //getting names
    cout << "\nPositions and their coresponding names are arbitrary and only serve as a story telling piece\n";
    while(1){
        cout << "\nWhat is your crew's name? > ";
        getline(cin, sInput);
        intResponse = oopsCore.validString(sInput);
        if(intResponse == -1){
            cout << "That name contains invalid strings which include:";
            for(int i = 0; i < oopsCore.getInvalidChars().length(); i++){
                cout << "  " << oopsCore.getInvalidChars()[i];
            }
            cout << endl;
        }else if(intResponse == -2) cout << "Please enter a name.\n";
        else{
            wait(5);
            cls();
            break;
        }
        cls();
    }
    cout << "\nWhat be the Captain's name? (Your name) > ";
    getName(0, travelers);
    cout << "\nWhat is your navigator's name? > ";
    getName(1, travelers);
    cout << "\nWhat is your shipwright's name? > ";
    getName(2, travelers);
    cout << "\nWhat is your doctor's name? > ";
    getName(3, travelers);
    cout << "\nWhat is your musician's name? > ";
    getName(4, travelers);
    cout << "\n\n";
    logThis("Squad is set, lets head out!");

    cout << "Time to head out! First thing's first though, lets get some supplies!\n";
    spLoad();
    statusUpdate(vecStats, numdays);

    //the first store's visit has to be written differently because there are some requirements that not all the stores have
    while(1){
        centerThis("cout", "Hello captain "+travelers[0].getName()+", let's get you ready for your journey!");
        stores[0];
        centerThis("cout", "You and you're crew have managed to save ¥1600");
        break;
    }
    cls();

    //turn handling
    oopsCore.setLocation("core() turn handling");
    while(1){
        string sTurnInput, turnNumspaces;
        int iTurnInput, milesTraveled;
        // check if the user has reached Kansas
        if(vecStats[1] <= 0){
            return 0;
        } 


        // check if the user is out of time
        if(numdays > 275){ 
            centerThis("cout", "It is now the "+getInGameDate(numdays));
            centerThis("cout", "The world government has taken a keen eye to you and your crew.");
            centerThis("cout", "You took too long getting to the New World at Fishmen Island, and the Navy has caught you\n");
            centerThis("cout", "Better luck next time!");
            return 1;
        }
        // check if the user has reached any milestones


        // what the user wants to do
        centerThis("cout", "The date is "+getInGameDate(numdays));
        centerThis("cout", "What do you want to do?");
        turnNumspaces = centerThis("numspaces", "[1] Rest");
        cout << turnNumspaces << "[1] Rest\n";
        cout << turnNumspaces << "[2] Hunt\n";
        cout << turnNumspaces << "[3] Continue\n";
        cout << turnNumspaces << "[4] Status Update\n\n";
        cout << turnNumspaces << "    > ";
        getline(cin, sTurnInput);
        oopsCore.checkResponse(sTurnInput);

        iTurnInput = oopsCore.trySTOI(sTurnInput);
        if(lowercase(sTurnInput) == "rest" || iTurnInput == 1){// resting takes between 1 and 3 days. Resting consumes 3 lbs. of food, per person, per day. logged
            numdays+=((rand() % 4));
            vecStats[2]-=(3*howManyAlive(travelers));
        }else if(lowercase(sTurnInput) == "hunt" || iTurnInput == 2){// Hunting takes 1 day. logged
            numdays++;
        }else if(lowercase(sTurnInput) == "continue" || iTurnInput == 3){// takes 2 weeks. Consumes 3 lbs of food, per person, per day (42 lbs.). Typically travel between 70 and 140 miles per turn. logged
            numdays+=14;
            vecStats[2]-=(3*howManyAlive(travelers));
            milesTraveled = (rand() % 140) + 69;
        }else if(lowercase(sTurnInput) == "continue" || iTurnInput == 4){

        }else if(iTurnInput == -1 || iTurnInput > 4) cout << "Please input a number 1-4 or their corresponding phrases.\n";
        else if(debug && sInput == "numdays") cout << " numdays: " << numdays;
        else cout << "Please input \"rest\", \"continue\", \"hunt\", \"status update\", or their corresponding numbers.\n";

        // if(sTurnInput == "rest") restThisTurn();
        // else if(sTurnInput == "continue") continueThisTurn();
        // else if(sTurnInput == "hunt") huntThisTurn();
        // else if(sTurnInput == "status update") statusUpdate(vecStats, numdays);
        // else if(oopsCore.trySTOI(sTurnInput) != -1){
        //     iTurnInput = stoi(sTurnInput);
        //     if(iTurnInput == 1) restThisTurn();
        //     else if(iTurnInput == 2) huntThisTurn();
        //     else if(iTurnInput == 3) continueThisTurn();
        //     else if(iTurnInput == 4) statusUpdate(vecStats, numdays);
        //     else cout << "Please input a number 1-4 or their corresponding phrases.\n";
        // }
        // else cout << "Please input \"rest\", \"continue\", \"hunt\", \"status update\", or their corresponding numbers.\n";
    }
    cls();
    cout << endl;
    return 0;
}


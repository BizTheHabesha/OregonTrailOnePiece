#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <ctime>
#include <stdlib.h>

using namespace std;

class ErrorHandler{
    private:
        // the location of the error
        string location = "location undefined";
        // months in chronological order
        string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        // invalid characters that cannot go inside a string
        string invalidchars = "|`\\<>";
        // valid inputs for a yes reponse
        string validYes[6] = {"yes", "y", "absolutly", "yea", "yeah", "yup"};
        // valid inputs for a no response
        string validNo[7] = {"no", "n", "nah", "na", "nope", "no, thanks", "no thanks"};
        // valid inputs to exit the app
        string validExit[4] = {"abortnow", "abort()", "leavenow", "!play"};
    public:
        // default constructer thats uses a stander errorHandlerPhrase
        ErrorHandler();
        // uses the given phrase as the errorHandlerPhrase
        ErrorHandler(string);
        // sets the location to the given location
        void setLocation(string);
        // checks if the given day fits within the given month
        int validStartingDay(int, int);
        // checks if the given month is a valid starting month
        int validStartingMonth(int);
        // checks if the given string has any invalid character in it.
        int validString(string);
        // logs based on the given code
        void codeHandler(int);
        void codeHandler(string);
        // returns 0 for a valid yes, 1 for a valid no, and -1 for an invalid input.
        int validResponse(string);
        // returns the list of invalid characters
        string getInvalidChars();
        // log this error using the Excpetion Handler phrase and location
        void logError(string);
        // convert the month from integer to string.
        string getMonth(int);
        int trySTOI(string);
};
class Traveler{
    private:
        // wether or not this traveler is alive
        bool alive;
        // the helath of this traveler
        int health;
        // the name of the traveler
        string name;
    public:
        // and exception hanlder for the entire class
        ErrorHandler oopsTraveler;
        // defalut constructer will leave an empty name and set 'alive' to false (by extension health should be zero). logged
        Traveler();
        // using this constructer will set the name to the given name, set alive to true and set health to 100. logged
        Traveler(string);
        // change the travelers name. logged
        int setName(string);
        // increment the health with the given opearand (switch). logged
        int setHealth(char, int);
        // return the health of the traveler.
        int getHealth();
        // return the name of the traveler.
        string getName();
        // bring a dead traveler back to life by setting 'alive' to true and 'health' to a positve non-zero integer. logged
        void resurection();
        // set 'alive' to false and health to 0. logged
        void kill();
        // return living status
        bool getLiving();
};
class Event{
    private:
        string eventTitle;
    public:
        Event();
        Event(string);
};
class Store{
    private:
        // contains all of the statistics of the user that are not specific to the traveler.
        // the name of the milestones
        string milestoneNames[8] = {"Shell Town", "Cocoyasi Village", "Syrup Village", "The Baratie", "Drum Island", };
        string storeName;
        int distance;
        // the number of stores that have been visted so far.
        int storesVisited = 0;
        // an array of item names
        string items[5] = {"log pose", "pound of food", "cannon ball", "ship part", "med kit"};
        // an array of item costs
        double costs[5] = {40.00, .50, 2.00, 20.00, 25.00};
    public:
        Store();
        Store(string);
        // exception handler for the store class
        ErrorHandler oopsStore;
        // will take the item as an index, use that index to find the price from the the 'prices' index above, then check firstly: if the user has enough money, and secondly: if the user already has
        // the max amount for said item.
        int buy(int, int);
        // display the current statistics. log this as an md table
        void visit();
};

// log the current date and time to the gamelog.txt file. (will contain logs from the code to indicate errors and whatnot)
int logThis(string);
void clearLog(int);
void initializeLog();
// spoof a loading screen utilizing wait() from usefulFunctions.hpp
void spLoad();
// get the names for the users
void getName();
// center a string with a certain number of spaces
string centerThis(string, string);
// resting takes between 1 and 3 days. Resting consumes 3 lbs. of food, per person, per day. logged
int restThisTurn();
// takes 2 weeks. Consumes 3 lbs of food, per person, per day (42 lbs.). Typically travel between 70 and 140 miles per turn. logged
int continueThisTurn();
// Hunting takes 1 day. logged
int huntThisTurn();
// Loop through the traveler array and returns how many are alive
int howManyAlive(Traveler*);
// convert the number o' days into an understandable date.
string getInGameDate(int);
int statusUpdate();
// handles the turn by turn gameplay, random occurences, status updates, etc.
int core();

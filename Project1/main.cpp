/*
 *  Spam Filter Progam.
 *  Created by Mariya Pasheva on 01.20.20
 *  U. of Illinois, Chicago
 *  CS 251: Spring 2020
 *
 *  Program functionality:
 *  1. load, accpets one argument allowing the user to load a text file consisting of spam emails.
 *  2. check can be used only after a spam emails txt file has been load. It accepts one argument, which is an email address to test against.
 *  3. filter requires two arguments, an input file with email addresses and an output file where the non-spam emails will be written to/appended.
 *  3. dislay requires no arguments and let's the user to display all spam emails from the loaded text file.
 *
*/
#include <iostream>
#include<fstream>
#include <string>
#include "ourvector.h"
using namespace std;

//Declarations.
void userInput();
bool fileHandler(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& filename, int& numOfProccessedIn, int& numOfProccessedOut, const string& outputFilename = " ");
void load(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& filename);
void filter(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& inputFile, const string& outputFile);
bool binarySearch(ourvector<string>& spamEmailsVec, const string& emailDomain, const string& emailUsername);
void display(ourvector<string>& spamEmailsVec);
bool check(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, string& emailAddrToCheck, const char& separator);
void getDomainUsername(string& email, string& domain, string& username, const char& separator);


int main() {
    cout << "** Welcome to spam filtering app **" << endl;
    userInput();

    return 0;
}
/**
 *  The function initializes all necessary variables and prompts the user
 *  for commands, looping until # is reached to display the statistics at the
 *  end and exit the program.
 */
void userInput()
{
    string userCommand; //load,filter,display or check
    string filename; // Either email or spam.
    string outputFilename; //Necessary for the filter() function.
    string emailAddrToCheck; // Necessary for the check() function.
    string asteriskUsrDomain; //Storing all spam domains which usernames could be anything(*).

    ourvector<string> spamEmailsVec; //The vector storing all the spam emails.

    while(userCommand != "#")
    {
        cout <<  "\nEnter command or #  to exit> ";
        cin >> userCommand;

        if(userCommand == "load") //Handling the spamlist. One more arguemnt expceted afterwards.
        {
            asteriskUsrDomain.clear(); //Making sure there are no any left domains stored from a previously loaded spamlist.
            cin >> filename;
            load(spamEmailsVec, asteriskUsrDomain, filename);
        }
        else if(userCommand == "filter")
        {
            cin >> filename;
            cin >> outputFilename;
            filter(spamEmailsVec, asteriskUsrDomain, filename, outputFilename);
        }
        else if(userCommand == "check") //Checking the spamlist.txt
        {
            cin >> emailAddrToCheck; //offers@groupon.com
            cout << emailAddrToCheck << (check(spamEmailsVec, asteriskUsrDomain, emailAddrToCheck, '@') ? " is spam" : " is not spam")
                 << endl;
        }
        else if(userCommand == "display")
        {
            display(spamEmailsVec);
        }
        else if(userCommand != "#")
        {
            cout << "**invalid command" << endl;
        }
    }// while(userCommand...
}// userInput(...

/**
 *
 * The function both read and writes to files, in case an output file has been provided with the filter function.
 * When a spam input file is provided it pushes the emails with O(n) time complexity to the vector.
 * There is also a stirng, which adds the domain to a string if the username is anything that it cn be "*"
 * This allows later to faster check if the username matters or not.
 *
 * In case the input file provided is an email. The emails is checked whether is a spam.
 * If not then it is written to the provided output file.
 *
 * @param spamEmailsVec The vector containig all spam emails.
 * @param asteriskUsrDomain Contains, if existent, domains which have "*" as their username.
 * @param filename The input filename provided by the user.
 * @param numOfProccessedIn The number of read lines from the input file.
 * @param numOfProccessedOut The number of written lines to the output file.
 * @param outputFilename
 *
 * @return If the file has been opened successfully it returns true, otherwise false.
 */
bool fileHandler(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& filename, int& numOfProccessedIn, int& numOfProccessedOut, const string& outputFilename)
{
    ifstream inFileStream;
    ofstream outFileStream;

    string username;
    string domain;
    //The type of file let's me decide later whether to handle as spam emails and add it to a vector ("spam")
    // or filter it and write to the provided output file. ("emai")
    string typeOfFile = filename.substr(0,4);

    inFileStream.open(filename);
    outFileStream.open(outputFilename, ios_base::app); //appending to the output file, instead of overwriting.

    string rowFile;

    while((!inFileStream.eof()) && inFileStream.is_open())
    {
        getline(inFileStream, rowFile); //Getting the whole line including any existing spaces.
        if(!inFileStream.fail())
        {
            if(typeOfFile == "spam")
            {
                spamEmailsVec.push_back(rowFile);
                //separates the username and domain in their separate strings.
                getDomainUsername(rowFile, username, domain, ':');
                //In case the username is * the domain is concatonaited to the string.
                //The comas on both sides are used as dilimiters to prevetnt partial string recognition such as:
                // "outlook.com" for "look.co"
                if(username == "*"){
                    asteriskUsrDomain += ("," + domain + ",");
                }
                numOfProccessedIn++;
            }
            else if(typeOfFile == "emai")
            {
                //Checking whether the email is a spam.
                //If not, then the email and the subject are written to the output file.
                if(!check(spamEmailsVec, asteriskUsrDomain, rowFile, ' '))
                {
                    outFileStream << rowFile << endl;
                    numOfProccessedOut++;
                }
                numOfProccessedIn++;
            }
        }
    }//while(!inFileStream.eof()...

    //In case the user has provided us with an input file which does not exist in the current directory,
    //therefore cannot be opened.
    if(!inFileStream.is_open()){
        return false;
    }

    inFileStream.close();
    outFileStream.close();
    return true;
}// void fileHandler(ourvector...

/**
 * The function first makes sure that everything is set back to its initial states,
 * ready to handle the new spam file.
 * Based on the output of the fileHandle it output the status or error messages.
 *
 * @param spamEmailsVec The vector containig all spam emails.
 * @param asteriskUsrDomain Contains, if existent, domains which have "*" as their username.
 * @param filename The spam file provided by the user.
 */
void load(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& filename)
{
    int numOfProccessedIn = 0;
    int tempPlaceholder = 0;
    spamEmailsVec.clear(); //Making sure there are no any left domains stored from a previously loaded spamlist.

    if(fileHandler(spamEmailsVec, asteriskUsrDomain, filename, numOfProccessedIn,tempPlaceholder)){
        cout << "Loading \'" << filename << "\'" << endl;
        cout << "# of spam entries: " << numOfProccessedIn << endl;
    }else{
        cout << "**Error, unable to open \'" << filename << "\'"<<endl;
    }
}

/**
 *  The function sets the number of proccessed lines back to 0 and based on the fileHandler function,
 *  if the file has been read successfully it output the status messages, otherwise the error message.
 *
 * @param spamEmailsVec The vector containig all spam emails.
 * @param asteriskUsrDomain Contains, if existent, domains which have "*" as their username.
 * @param inputFile The email file provided by the user.
 * @param outputFile
 */
void filter(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, const string& inputFile, const string& outputFile)
{
    int numOfProccessedIn = 0;
    int numOfProccessedOut = 0;
    if(fileHandler(spamEmailsVec, asteriskUsrDomain, inputFile, numOfProccessedIn, numOfProccessedOut, outputFile)){
        cout << "# emails processed: " << numOfProccessedIn << endl;
        cout << "# non-spam emails: " << numOfProccessedOut << endl;
    }else{
        cout << "**Error, unable to open \'" << inputFile << "\'"<<endl;
    }
}

/**
 *  The function first separates the given input email address by username and domain. Then it checks if the domain is
 *  already in the list of domains containg asterisks as their username. The comma is used as a delimiter.
 *  If not then a binary search is performed over the vector. I did not store those domain in an another vector because
 *  the porject seemed to imply for us to have only one vector.
 *
 * @param spamEmailsVec The vector containig all spam emails.
 * @param asteriskUsrDomain Contains, if existent, domains which have "*" as their username.
 * @param emailAddrToCheck The email address to check against.
 * @param separator specifies whether the separator is ":" as in the spam.txt
 *
 * @return If email is a spam the function return true, otherwise results in false, which in the second case depends on the binarySearch of the vector.
 */
bool check(ourvector<string>& spamEmailsVec, string& asteriskUsrDomain, string& emailAddrToCheck, const char& separator)
{
    string emailUsername;
    string emailDomain;

    getDomainUsername(emailAddrToCheck, emailDomain, emailUsername, separator);

    string find = "," + emailDomain + ",";
    if(asteriskUsrDomain.find(find) != string::npos){
        return true;
    }else{
        return binarySearch(spamEmailsVec, emailDomain, emailUsername);
    }
}

/**
 * The function first checks against the domain, if the domains match it checks whether the username matches.
 * I used the .compare() function instead of the < operator, since I had issues on my machine and was always evaluating
 * the comparisons of the strings incorrectly.
 *
 *
 * @param spamEmailsVec The vector containig all spam emails.
 * @param emailDomain The domain of the email we are searching against.
 * @param emailUsername The username of the email we are searching against.
 *
 * @return If both the domain and username match the function returns true, otherwise false.
 */
bool binarySearch(ourvector<string>& spamEmailsVec, const string& emailDomain, const string& emailUsername)
{

    int middle = 0;
    int lowEnd = 0;
    int highEnd = spamEmailsVec.size() - 1;
    string spamUsername;
    string spamDomain;

    middle = lowEnd + ((highEnd - lowEnd) / 2);

    while(lowEnd <= highEnd)
    {
        string spamEmail = spamEmailsVec.at(middle); //Getting the value of the mid point, in order to separate it by email and domain.
        getDomainUsername(spamEmail, spamUsername, spamDomain, ':'); // Switching order of passing the arguments because of the flipped format in the spamlist.

        if(emailDomain == spamDomain){
            if(spamUsername == emailUsername){
                return true;
            }
            else if(emailUsername.compare(spamUsername) > 0) //emailUsername < spamUsername
            {
                lowEnd = middle + 1;
            }
            else
            {
                highEnd = middle - 1;
            }
        }
        else if(emailDomain.compare(spamDomain) > 0) { //emailUsername < spamUsername
            lowEnd = middle + 1;
        }
        else
        {
            highEnd = middle - 1;
        }
        middle = lowEnd + ((highEnd - lowEnd) / 2);
    }
    return false;
}

/**
 * The function is separating the emails by domain and username based on the saparator.
 * The special case of when we have a whole line with spaacesn included is handle by the first case;
 * Ex: 9 jhummel2@uic.edu Question about grading...
 *
 *
 * @param email The full line from the spam or emai txt files.
 * @param domain The string that the domain will get assigned to.
 * @param username The string the username will get assigned to.
 * @param separator The separator which helped to decided what case is handled.
 */
void getDomainUsername(string& email, string& domain, string& username, const char& separator) {
    unsigned int posSeparator; //The position of the initial separator.
    unsigned int separatorHelper; //Used for the email txt file.

    posSeparator = email.find(separator);

    //In case the line has spaces such as email.txt, the block is executed.
    if (separator == ' ') {
        //Looking for the first case of a letter(all emails start with lowercase, but upper case can be handeled as well)
        //, since there are case where there are two or three spaces, one after another.
        posSeparator = email.find_first_of("abcdefghijklmnopqrstuvwxyz");
        separatorHelper = email.find('@');
        username = email.substr(posSeparator, separatorHelper - (posSeparator));
        posSeparator = email.find(' ', separatorHelper);
        domain = email.substr(separatorHelper + 1, posSeparator - (separatorHelper + 1));
    } else{
        username = email.substr(0, posSeparator);
        domain = email.substr(posSeparator + 1, email.length() - 1);
    }
}

/**
 *
 * Looping through the spam emails and displaying them for the user
 * without any modification.
 *
 * @param spamEmailsVec The vector containig all spam emails.
 */
void display(ourvector<string>& spamEmailsVec){
    for(const string& email : spamEmailsVec)
    {
        cout << email << endl;
    }
}

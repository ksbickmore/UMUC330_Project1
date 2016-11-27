/**
 * @file Lexer.cpp
 * @brief Contains the Lexer class source code, which handles grammar lexing utilities.
 * @author Kristopher Bickmore
 * @date November 21, 2016
 * @bug No known bugs at this time
 */

#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "stringhelper.h"


using namespace std;

/**
 * Utility function used to check if a string is a valid number
 * @param number the string that may be a number
 * @return true if a number, false otherwise
 */
bool checkNumber(string number) {
    long val;
    string check;
    try {
        val = std::stoi(number);
        check = std::to_string(val);
        if (check != number) {
            return false;
        }
    }

    catch (invalid_argument& e) {
        return false;
    }
    catch (out_of_range& e) {
        return false;
    }
    return true;
}

Lexer::Lexer(std::experimental::filesystem::path filename) throw(runtime_error) :
    fileReader(filename),
    currentLexeme(""),
    lastLexeme(""),
    punctuation("():;.,"),
    fileString("")
{
    currentToken = NONE;
    previousToken = NONE;
    index = 0;
    if (!fileReader.is_open()) {
        throw runtime_error("Invalid path to input file");
    }
    string line("");

    while (!fileReader.bad() && !fileReader.eof()) {
        getline(fileReader, line);
        if (line == "") continue;
        fileString.append(line);
    }
    fileReader.close();
}

Token Lexer::getNextToken()
{
    lastLexeme = currentLexeme;
    currentLexeme = getNextLexeme();
    previousToken = currentToken;
    return currentToken;
}

std::string Lexer::getCurrentLexeme() {
    return currentLexeme;
}

std::string Lexer::getNextLexeme()
{
    string possibleLexeme("");
    char c;
    bool checkquotes = false;
    bool checknumber = false;

    // this is all a little convoluted but it works.
    for(index; index < fileString.length(); ++index){
        c = fileString[index];
        // check newline characters for linux mainly.
        if (c == '\n' || c == '\r' || (c == ' ' && !checkquotes && !checknumber)) continue;
        
        if (c == '"') {
            checkquotes = !checkquotes;
            if (!checkquotes) {
                index++;
                currentToken = STRING;
                break;
            }
            else {
                continue;
            }
        }
        else if (isdigit(c) && !checkquotes) {
            possibleLexeme.append(StringHelper::toString(c));
            checknumber = true;
            continue;
        }
        else if (checknumber) {
            checknumber = !checknumber;
            currentToken = NUMBER;
            break;
        }

        if (std::find(punctuation.begin(), punctuation.end(), c) != punctuation.end()) {

            index++;

            currentToken = checkLexeme(StringHelper::toString(c));
            possibleLexeme = StringHelper::toString(c);
            break;
        }
        else if (isalpha(c) || checkquotes) {
            possibleLexeme.append(StringHelper::toString(c));
            if (checkquotes) continue;
            if ((currentToken = checkLexeme(possibleLexeme)) != NONE) {
                index++;
                break;
            }
            else {
                continue;
            }
        }
        else {
            currentToken = NONE;
            if (!possibleLexeme.empty()) {
                return possibleLexeme.append(StringHelper::toString(c));
            }
            else
            {
                return StringHelper::toString(c);
            }
        }
    }
    return possibleLexeme;
}

Token Lexer::checkLexeme(string lexeme)
{
    Token nexttoken = NONE;
    if (lexeme == "(")
    {
        nexttoken = OPENPAREN;
    }
    else if (lexeme == ")")
    {
        nexttoken = CLOSEPAREN;
    }
    else if (lexeme == "Window")
    {
        nexttoken = WINDOW;
    }
    else if (lexeme == "Layout")
    {
        nexttoken = LAYOUT;
    }
    else if (lexeme == "Flow")
    {
        nexttoken = FLOW;
    }
    else if (lexeme == "Border")
    {
        nexttoken = BORDER;
    }
    else if (lexeme == "Grid")
    {
        nexttoken = GRID;
    }
    else if (lexeme == "LEFT")
    {
        nexttoken = LEFT;
    }
    else if (lexeme == "RIGHT")
    {
        nexttoken = RIGHT;
    }
    else if (lexeme == "CENTER")
    {
        nexttoken = CENTER;
    }
    else if (lexeme == "Button")
    {
        nexttoken = BUTTON;
    }
    else if (lexeme == "Group")
    {
        nexttoken = GROUP;
    }
    else if (lexeme == "Label")
    {
        nexttoken = LABEL;
    }
    else if (lexeme == "Panel")
    {
        nexttoken = PANEL;
    }
    else if (lexeme == "Textfield")
    {
        nexttoken = TEXTFIELD;
    }
    else if (lexeme == "Radio")
    {
        nexttoken = RADIO;
    }
    else if (lexeme == ";")
    {
        nexttoken = SEMICOLON;
    }
    else if (lexeme == ":")
    {
        nexttoken = COLON;
    }
    else if (lexeme == "Flow")
    {
        nexttoken = FLOW;
    }
    else if (lexeme == ",")
    {
        nexttoken = COMMA;
    }
    else if(lexeme == "."){
        nexttoken = PERIOD;
    }
    else if(lexeme == "End"){
        nexttoken = END;
    }
    else if (checkNumber(lexeme)) {
        nexttoken = NUMBER;
    }
    return nexttoken;
}

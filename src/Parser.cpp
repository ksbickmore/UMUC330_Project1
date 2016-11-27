/**
 * @file Parser.cpp
 * @brief Contains the source code for the Parser class
 * @author Kristopher Bickmore
 * @date November 20, 2016
 */

#include <fstream>
#include <iostream>
#include <sstream>
//#include <string>

#include "Lexer.h"
#include "Parser.h"
#include "stringhelper.h"

using namespace std;

/**
 * @def WRITE_LINE
 * Writes a string to to the parser's ofstream, also prints to stdout if print is true.
 * @param writevalue
 */
#define WRITE_LINE(writevalue) \
    if(print){\
        cout << writevalue << endl;\
    }\
    outfile << writevalue << endl;

/**
 * @def PARSER_CHECK
 * takes a conditional that compares the current token to the expected token value. If the token is valid syntactically
 * then the process continues by writing the values and getting the next token. Otherwise the process fails.
 */
#define PARSER_CHECK(COND) \
    if(COND){ \
        writeTokenLexeme(token, lexer.getCurrentLexeme());\
        token = lexer.getNextToken(); \
    } \
    else{ \
        ret = false;\
        goto cleanup;\
    }

/**
 * @def PRODUCTION_CHECK
 * takes a conditional that compares whether a production function succeeded or not.
 */
#define PRODUCTION_CHECK(COND)\
    if(!COND){\
        ret = false;\
        goto cleanup;\
    }

void Parser::writeTokenLexeme(Token token, string lexeme){
    string writeline("Next Token is: " + std::to_string(token) + "; Next Lexeme is: " + lexeme);
    if (print) {
        cout << writeline << endl;
    }
    outfile << writeline << endl;
}

Parser::Parser(std::experimental::filesystem::path infilename, std::string outfilename, bool printval) throw(runtime_error): 
    outfile(outfilename),
    lexer(infilename)
{
    lexer.getCurrentLexeme();
    token = NONE;
    print = printval;
    if (!outfile.is_open()) {
        throw runtime_error("Invalid path to output file");
    }
}

void Parser::file() {
    token = lexer.getNextToken();
    bool finaltest = gui_production();
}

bool Parser::gui_production(){
    bool ret = true;
    WRITE_LINE("Entering GUI Production");

    PARSER_CHECK(token == WINDOW);

    PARSER_CHECK(token == STRING);

    PARSER_CHECK(token == OPENPAREN);

    PARSER_CHECK(token == NUMBER);

    PARSER_CHECK(token == COMMA);

    PARSER_CHECK(token == NUMBER);

    PARSER_CHECK(token == CLOSEPAREN);

    PRODUCTION_CHECK(layout_production());

    widgets_production();

    PARSER_CHECK(token == END);

    // At end of file so we can't use PARSER_CHECK which tries to get another token
    // should fix this in case a file has more after the end of the production
    if (token == PERIOD) {
        writeTokenLexeme(token, lexer.getCurrentLexeme()); 
    } 
    else {
        ret = false; 
        goto cleanup; 
    }
cleanup:
    if (ret == false){
        if (token == NONE) {
            
            WRITE_LINE("******** Lexical Error!! ********");
            writeTokenLexeme(token, lexer.getCurrentLexeme());
        }
        else {
            WRITE_LINE("******** Syntax Error!! ********");
            writeTokenLexeme(token, lexer.getCurrentLexeme());
        }
    }
    WRITE_LINE("Exiting GUI Production");
    return ret;
}

bool Parser::layout_production(){
    bool ret = true;
    WRITE_LINE("Entering Layout Production");
    PARSER_CHECK(token == LAYOUT);

    PRODUCTION_CHECK(layout_type_production());

    PARSER_CHECK(token == COLON);

cleanup:
    WRITE_LINE("Exiting Layout Production");
    return ret;
}

bool Parser::layout_type_production(){
    bool ret = true;
    WRITE_LINE("Entering Layout Type Production")

    Token type = token;
    PARSER_CHECK(token == FLOW || token == BORDER || token == GRID);

    PARSER_CHECK(token == OPENPAREN);

    switch(type){
        case FLOW: {
            if (token != CLOSEPAREN) {
                PRODUCTION_CHECK(align_production());
            }
            break;
        }
        case BORDER:{
            if(token != CLOSEPAREN) {
                PARSER_CHECK(token == NUMBER);

                PARSER_CHECK(token == COMMA);

                PARSER_CHECK(token == NUMBER);
            }
            break;
        }
        case GRID:{
            PARSER_CHECK(token == NUMBER);

            PARSER_CHECK(token == COMMA);

            PARSER_CHECK(token == NUMBER);
            if(token != CLOSEPAREN) {
                PARSER_CHECK(token == COMMA);

                PARSER_CHECK(token == NUMBER);

                PARSER_CHECK(token == COMMA);

                PARSER_CHECK(token == NUMBER);
            }
            break;
        }
    }

    PARSER_CHECK(token == CLOSEPAREN);

cleanup:
    WRITE_LINE("Exiting Layout Type Production");
    return ret;
}

bool Parser::align_production(){
    bool ret = true;
    WRITE_LINE("Entering Align Production");

    PARSER_CHECK(token == LEFT || token == RIGHT || token == CENTER);

cleanup:
    WRITE_LINE("Exiting Align Production");
    return ret;
}

bool Parser::widget_production(){
    bool ret = true;
    WRITE_LINE("Entering Widget Production");

    switch(token){
        case BUTTON:{
            PARSER_CHECK(token == BUTTON);
            PARSER_CHECK(token == STRING);
            break;
        }
        case LABEL:{
            PARSER_CHECK(token == LABEL);
            PARSER_CHECK(token == STRING);
            break;
        }
        case GROUP:{
            PARSER_CHECK(token == GROUP);
            radio_buttons_production();
            PARSER_CHECK(token == END);
            break;
        }
        case PANEL:{
            PARSER_CHECK(token == PANEL);
            PRODUCTION_CHECK(layout_production());
            widgets_production();
            PARSER_CHECK(token == END);
            break;
        }
        case TEXTFIELD:{
            PARSER_CHECK(token == TEXTFIELD);
            PARSER_CHECK(token == NUMBER);
            break;
        }
        default:{
            ret = false;
            goto cleanup;
        }
    }

    PARSER_CHECK(token == SEMICOLON);

cleanup:
    WRITE_LINE("Exiting Widget Production");
    return ret;
}

bool Parser::widgets_production(){
    bool ret = true;

    WRITE_LINE("Entering Widgets Production");
    PRODUCTION_CHECK(widget_production());
    PRODUCTION_CHECK(widgets_production());

cleanup:
    WRITE_LINE("Exiting Widgets Production");
    return ret;
}

bool Parser::radio_buttons_production(){
    bool ret = true;
    WRITE_LINE("Entering Radio Buttons Production");
    PRODUCTION_CHECK(radio_button_production());
    radio_buttons_production();


cleanup:
    WRITE_LINE("Exiting Radio Buttons Production");
    return ret;
}

bool Parser::radio_button_production(){
    bool ret = true;
    WRITE_LINE("Entering Radio Button Production" );
    PARSER_CHECK(token == RADIO);
    PARSER_CHECK(token == STRING);
    PARSER_CHECK(token == SEMICOLON);

cleanup:
    WRITE_LINE("Exiting Radio Button Production" );
    return ret;
}
/**
 * @file Lexer.h
 * @brief Contains the Lexer class definition.
 * @author Kristopher Bickmore
 * @date November 11, 2016
 */
#ifndef PROJECT1_LEXER_H_H
#define PROJECT1_LEXER_H_H

#pragma once

#include <algorithm>
#ifdef _WIN32
#include <experimental\filesystem>
#elif __linux__
#include <experimental/filesystem>
#endif
#include <functional>
#include <list>
#include <string>
#include <vector>

/**
 * This enumeration contains all Tokens used in the associated grammar.
 */
enum Token
{
    WINDOW, STRING,	LAYOUT,	OPENPAREN, CLOSEPAREN, COLON, SEMICOLON, FLOW, BORDER, GRID, LEFT, RIGHT, CENTER,
    BUTTON,	GROUP, LABEL, PANEL, TEXTFIELD,	RADIO, END,	PERIOD,	NONE, ENDOFLINE, ENDOFFILE, NUMBER,	COMMA
};

/**
 * @brief This class is used to Lex a specific grammar:
 * @details This class is used to Lex a specific grammar:\n
 *  gui ::= Window STRING '(' NUMBER ',' NUMBER ')' layout widgets End '.'\n
    layout ::= Layout layout_type ':'\n
    layout_type ::=\n
        Flow '(' [ align ] ')'|\n
        Border '(' [ NUMBER ',' NUMBER ] ')'|\n
        Grid '(' NUMBER ',' NUMBER [',' NUMBER ',' NUMBER] ')'\n
    align ::= LEFT | RIGHT | CENTER\n
    widgets ::= widget widgets | widget\n
    widget ::=\n
        Button STRING ';' |\n
        Group radio_buttons End ';' |\n
        Label STRING ';' |\n
        Panel layout widgets End ';' |\n
        Textfield NUMBER ';'\n
    radio_buttons ::= radio_button radio_buttons | radio_button\n
    radio_button ::= Radio STRING ';'\n
 */
class Lexer
{
private:
    /// A file stream reading from the current file to be parsed.
    std::ifstream fileReader;
    /// String which contains the lexeme currently being parsed/lexed
    std::string currentLexeme;
    /// The string containing the lexeme that was previously looked at
    std::string lastLexeme;
    /// A string containing punctuation values which are delimiters in some way.
    std::string punctuation;
    /// String containing the text of the file
    std::string fileString;
    /// The token that is related to the current lexeme.
    Token currentToken;
    /// The token that is related to the prior lexeme.
    Token previousToken;
    /// Index into the file.
    unsigned int index;

    /**
     * Validates that the retrieved lexeme is a valid token.
     * @param lexeme a string to be compared against valid tokens
     * @return The corresponding token
     */
    Token checkLexeme(std::string lexeme);

    /**
	 * Retrieves the next lexeme from the file.
	 * @return
	 */
    std::string getNextLexeme();

public:

	/**
	 * Lexer Constructor
	 * @param filename the path to an input file to be lexed
	 * @return A Lexer object
	 * @throw runtime_error
	 */
	Lexer(std::experimental::filesystem::path filename) throw(std::runtime_error);

	/**
	 * Gets the lexeme that is currently being looked at.
	 * @return the lexeme
	 */
    std::string getCurrentLexeme();

	/**
	 * Retrieves the next token in the current line
	 * @return The Token
	 */
	Token getNextToken();
};

#endif
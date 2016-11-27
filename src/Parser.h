/**
 * @file Parser.h
 * @brief Contains the Parser Class definition
 * @author Kristopher Bickmore
 * @date November 20, 2016
 */

#ifndef PROJECT1_PARSER_H_H
#define PROJECT1_PARSER_H_H

#include <fstream>
#include "Lexer.h"

/**
 * @brief The parser class parses a specific grammar.
 * @details The Parser class is used to parse each Token created by the Lexer class and to parse the overall syntax of
 * the grammar:\n
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
class Parser{
    /// The output file stream associated with the current Lexer input file stream.
    std::ofstream outfile;
    /// The lexer which will provide tokens and lexemes
    Lexer lexer;
    /// A token whose value will be proviced by the Lexer class
    Token token;
    /// This value indicates whether or not the parser will print its output
    bool print;

public:
    /**
     * The Parser constructor
     * @param inFilename a path to the current file to be parsed and lexed
     * @param outfile the name of the file which will contain the output of the parser
     * @param print Print output or not
     * @return A parser object
     * @throw runtime_error
     */
    Parser(std::experimental::filesystem::path inFilename, std::string outfile, bool print) throw(std::runtime_error);

    /**
     * Begins the process of parsing the input file
     */
    void file();

private:

    /**
     * Validates the gui production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool gui_production();

    /**
     * Validates the layout production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool layout_production();

    /**
     * Validates the layout type production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool layout_type_production();

    /**
     * Validates the align production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool align_production();

    /**
     * Validates the widget production syntax.
     * @return true if syntax is valid, false otherwise
     */

    bool widget_production();

    /**
    * Validates the widgets production syntax.
    * @return true if syntax is valid, false otherwise
    */
    bool widgets_production();

    /**
     * Validates the radio buttons production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool radio_buttons_production();

    /**
     * Validates the radio button production syntax.
     * @return true if syntax is valid, false otherwise
     */
    bool radio_button_production();

    /**
     * Writes the next token and lexeme to the output file, prints the output to screen if print is true.
     * @param token the available token
     * @param lexeme the available lexeme
     */
    void writeTokenLexeme(Token token, std::string lexeme);
};
#endif //PROJECT1_PARSER_H_H

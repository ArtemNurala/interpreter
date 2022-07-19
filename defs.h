#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <fstream>
#include <string>
#include <iostream>

#define LetterSmall(p) ( (p>='a') && (p<='z') )
#define LetterBig(p) ( (p>='A') && (p<='Z') )
#define Number(p) ( (p>='0') && (p<='9') )

#define Special(p) ( (p==' ') || (p=='!') || (p=='@') || (p=='#') || (p=='$') || (p=='%') || (p=='^') || (p=='&') || (p=='*') || (p=='(') || (p=='_') || (p=='-') || (p=='+') || (p=='=') || (p=='/') || (p=='<') || (p=='>') || (p==',') || (p=='.') || (p==':') || (p==';') || (p=='|') )
#define Special2(p) ( (p=='r') || (p=='n') || (p=='t') || (p=='v') || (p=='Ф') || (p=='\'') || (p=='\\') || (p=='0') || (p=='a') )
#define Ignor(p) ( (p==' ') || (p=='\n') || (p=='\t') )


#define defineS 33
#define key_words_quantity 7
#define error_quantity 5

#define RETURN  1
#define MAIN  2
#define WHILE  3
#define INT  4
#define SHORT  5
#define FLOAT  7

#define TYPE_IDENT  8

#define TYPE_GT  9
#define TYPE_GE  10
#define TYPE_LT  11
#define TYPE_LE  12
#define TYPE_LS  13
#define TYPE_GS  14
#define TYPE_EQ  15
#define TYPE_NEQ  16
#define TYPE_PLUS  17
#define TYPE_MINUS  18
#define TYPE_DIV  19
#define TYPE_MUL  20
#define TYPE_CONST_10  22
#define TYPE_CONST_CHAR  25
#define TYPE_BRACE_OPEN  26
#define TYPE_BRACE_CLOSED  27
#define TYPE_CUR_BR_OPEN  28
#define TYPE_CUR_BR_CLOSED  29
#define TYPE_DOT  30
#define TYPE_COMMA  31
#define TYPE_SEMICOLON  32
#define TYPE_ASSIGN  33

#define TYPE_ERROR 100
#define TYPE_END 200

#define ERROR_OPEN_COMMENT_IN_THE_END  1
#define ERROR_UNKNOWN_LEKSEMA  2
#define ERROR_OPEN_CHAR_CONST  3
#define ERROR_WRONG_CHAR_CONST  4
#define ERROR_LEXEMA_IS_TOO_LONG  5

#define ERROR_EXPECTED_SEMICOLON 1
#define ERROR_MAIN_IS_FUNC 2
#define ERROR_EXPECTED_BRACE_OPEN 3
#define ERROR_EXPECTED_BRACE_CLOSED 4
#define ERROR_EXPECTED_CUR_BRACE_OPEN 5
#define ERROR_EXPECTED_CUR_BRACE_CLOSED 6
#define ERROR_EXPECTED_IDENT 7
#define ERROR_EXPECTED_TYPE 8 //ожидалось short или float
#define ERROR_EXPECTED_INT 9
#define ERROR_EXPECTED_FUNCTION_OR_ASSIGNING 10
#define ERROR_EXPECTED_OPERATOR 11
#define ERROR_EXPECTED_EXPRESSION 12
#define ERROR_EXPECTED_SEMICOLON_OR_ASSIGN 13
#define ERROR_EXPECTED_OPER_OR_VAR 14
#define ERROR_FUNC_NO_RETURNS 15
#define ERROR_MAIN_ISNT_DEF 16

#define ERROR_FUNC_DOES_NOT_EXIST 17
#define ERROR_FUNC_HAS_MORE_PARAMS 18
#define ERROR_FUNC_HAS_LESS_PARAMS 19
#define ERROR_TYPES_DONT_MATCH 20
#define ERROR_VAR_DOES_NOT_EXIST 21
#define ERROR_VAR_IS_NOT_INIT 22
#define ERROR_TYPES_DONT_MATCH_RET 23

#define T_FUNC_ALREADY_EXISTS 24
#define T_VAR_ALREADY_EXISTS 25

#define ERROR_OPERAND_IS_FLOAT 26

#define ERROR_DIV_ZERO_EXCEPTION 27

#define MaxText 10000 //максимальна€ длина текста »ћ
#define MaxLex 50 //максимальна€ длина лексемы

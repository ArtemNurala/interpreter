#pragma once
#include "defs.h"
#include "scaner.h"
#include "semantic.h"

using namespace std;
class   TDiagram {
private:    // сканер и все функции СД, кроме аксиомы
	TScaner *sc;
	Tree*  functionDef1(bool needToInterpret, LEXs nameIdent, TypeData dt);
	void  listInFuncDef14(TreeControl *listInFuncTree, Node *funcNode);
	TypeData  type2();
	void  list3(TypeData dt);
	Tree* variable4(TypeData dt, bool init, bool *hereIsNewVarAssign);
	void  parameters6(Tree *T);
	Tree* while7(Tree *funcNode);
	Tree* expression9();
	Tree* shift_res10();
	Tree* addendum11();
	Tree* multiplier12();
	Tree* simple_expression13();
	Tree* return_operator16(Tree *funcNode);
	Tree* parameter17(bool *hereIsNewVarAssign, bool *isNewVar);

	Tree*  operator5(bool makeChild, Tree *funcNode);
	Tree*  opers_desrps8(Tree *funcNode);
	Tree*  sostavnoi_operator15(Tree *funcNode);

	Tree * callFunc(Tree * T);

	void PrintError(int i);

	LEXs *lex;
	int type;
	int uk, ukLine, ukInLine;
	int ukTemp, ukLineTemp, ukInLineTemp;
	TreeControl *TC;

	void showInfo(string message, Node *id, bool needToPrintValue);
	void showInfo(string message, Node *id);
	void showInfoParameter(string message, Node *id, Tree * param, bool needEndl);
	int tabs;

	map <int, string> ErrorMessages;

public:     //   из СД   –  только  аксиома
	TDiagram(TScaner * s) : sc(s), lex(new LEXs()), tabs(-1), TC(new TreeControl())
	{
		ErrorMessages[ERROR_EXPECTED_SEMICOLON] = "Ожидалось ;";
		ErrorMessages[ERROR_EXPECTED_BRACE_OPEN] = "Ожидалось (";
		ErrorMessages[ERROR_EXPECTED_BRACE_CLOSED] = "Ожидалось )";
		ErrorMessages[ERROR_EXPECTED_CUR_BRACE_OPEN] = "Ожидалось {";
		ErrorMessages[ERROR_EXPECTED_CUR_BRACE_CLOSED] = "Ожидалось }";
		ErrorMessages[ERROR_EXPECTED_IDENT] = "Ожидался идентификатор";
		ErrorMessages[ERROR_EXPECTED_TYPE] = "Ожидался тип (разрешены типы: short int, float)";
		ErrorMessages[ERROR_EXPECTED_INT] = "Разрешены типы: short int";
		ErrorMessages[ERROR_EXPECTED_FUNCTION_OR_ASSIGNING] = "Ожидалась функция или присваивание";
		ErrorMessages[ERROR_EXPECTED_OPERATOR] = "Ожидался оператор";
		ErrorMessages[ERROR_EXPECTED_EXPRESSION] = "Ожидалось выражение";
		ErrorMessages[ERROR_EXPECTED_SEMICOLON_OR_ASSIGN] = "Ожидалось присваивание или ;";
		ErrorMessages[ERROR_FUNC_NO_RETURNS] = "Функция не возвращает значений";
		ErrorMessages[ERROR_MAIN_IS_FUNC] = "Идентификатор main может использоваться только для описания главной функции программы";
		ErrorMessages[ERROR_MAIN_ISNT_DEF] = "Не объявлена главная функция main";
		ErrorMessages[ERROR_EXPECTED_OPER_OR_VAR] = "Ожидался оператор или переменная";
		ErrorMessages[T_FUNC_ALREADY_EXISTS] = "Функция уже объявлена";
		ErrorMessages[T_VAR_ALREADY_EXISTS] = "Переменная уже объявлена";		 
		ErrorMessages[ERROR_FUNC_DOES_NOT_EXIST] = "Функция не объявлена";
		ErrorMessages[ERROR_VAR_DOES_NOT_EXIST] = "Переменная не объявлена";
		ErrorMessages[ERROR_VAR_IS_NOT_INIT] = "Переменная не инициализирована";
		ErrorMessages[ERROR_FUNC_HAS_MORE_PARAMS] = "Слишком мало параметров";
		ErrorMessages[ERROR_FUNC_HAS_LESS_PARAMS] = "Слишком много параметров";
		ErrorMessages[ERROR_TYPES_DONT_MATCH] = "Приведение типов невозможно";
		ErrorMessages[ERROR_TYPES_DONT_MATCH_RET] = "Тип возвращаемого значения невозможно привести к типу функции";
	}
	~TDiagram() {}
	void maincpp(); // программа  - аксиома КСГ
};

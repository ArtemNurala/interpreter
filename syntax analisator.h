#pragma once
#include "defs.h"
#include "scaner.h"
#include "semantic.h"

using namespace std;
class   TDiagram {
private:    // ������ � ��� ������� ��, ����� �������
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

public:     //   �� ��   �  ������  �������
	TDiagram(TScaner * s) : sc(s), lex(new LEXs()), tabs(-1), TC(new TreeControl())
	{
		ErrorMessages[ERROR_EXPECTED_SEMICOLON] = "��������� ;";
		ErrorMessages[ERROR_EXPECTED_BRACE_OPEN] = "��������� (";
		ErrorMessages[ERROR_EXPECTED_BRACE_CLOSED] = "��������� )";
		ErrorMessages[ERROR_EXPECTED_CUR_BRACE_OPEN] = "��������� {";
		ErrorMessages[ERROR_EXPECTED_CUR_BRACE_CLOSED] = "��������� }";
		ErrorMessages[ERROR_EXPECTED_IDENT] = "�������� �������������";
		ErrorMessages[ERROR_EXPECTED_TYPE] = "�������� ��� (��������� ����: short int, float)";
		ErrorMessages[ERROR_EXPECTED_INT] = "��������� ����: short int";
		ErrorMessages[ERROR_EXPECTED_FUNCTION_OR_ASSIGNING] = "��������� ������� ��� ������������";
		ErrorMessages[ERROR_EXPECTED_OPERATOR] = "�������� ��������";
		ErrorMessages[ERROR_EXPECTED_EXPRESSION] = "��������� ���������";
		ErrorMessages[ERROR_EXPECTED_SEMICOLON_OR_ASSIGN] = "��������� ������������ ��� ;";
		ErrorMessages[ERROR_FUNC_NO_RETURNS] = "������� �� ���������� ��������";
		ErrorMessages[ERROR_MAIN_IS_FUNC] = "������������� main ����� �������������� ������ ��� �������� ������� ������� ���������";
		ErrorMessages[ERROR_MAIN_ISNT_DEF] = "�� ��������� ������� ������� main";
		ErrorMessages[ERROR_EXPECTED_OPER_OR_VAR] = "�������� �������� ��� ����������";
		ErrorMessages[T_FUNC_ALREADY_EXISTS] = "������� ��� ���������";
		ErrorMessages[T_VAR_ALREADY_EXISTS] = "���������� ��� ���������";		 
		ErrorMessages[ERROR_FUNC_DOES_NOT_EXIST] = "������� �� ���������";
		ErrorMessages[ERROR_VAR_DOES_NOT_EXIST] = "���������� �� ���������";
		ErrorMessages[ERROR_VAR_IS_NOT_INIT] = "���������� �� ����������������";
		ErrorMessages[ERROR_FUNC_HAS_MORE_PARAMS] = "������� ���� ����������";
		ErrorMessages[ERROR_FUNC_HAS_LESS_PARAMS] = "������� ����� ����������";
		ErrorMessages[ERROR_TYPES_DONT_MATCH] = "���������� ����� ����������";
		ErrorMessages[ERROR_TYPES_DONT_MATCH_RET] = "��� ������������� �������� ���������� �������� � ���� �������";
	}
	~TDiagram() {}
	void maincpp(); // ���������  - ������� ���
};

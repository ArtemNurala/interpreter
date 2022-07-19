#include "syntax analisator.h"
#include "operators.h"

void TDiagram::showInfo(string message, Node *id, bool needToPrintValue)
{
	for (int i = 0; i < tabs; i++)
		cout << '\t';
	cout << message;
	switch (id->getDataType())
	{
	case TypeData::TypeShortInt:
		cout << " (short) ";
		break;
	case TypeData::TypeFloat:
		cout << " (float) ";
		break;
	}
	cout << id->getID() << ";" << endl;
}

void TDiagram::showInfo(string message, Node *id)
{
	for (int i = 0; i < tabs; i++)
		cout << '\t';
	cout << message;
	switch (id->getDataType())
	{
	case TypeData::TypeShortInt:
		cout << " (short) ";
		break;
	case TypeData::TypeFloat:
		cout << " (float) ";
		break;
	}
	cout << id->getID() << " = ";
	switch (id->getDataType())
	{
	case TypeData::TypeShortInt:
		cout << id->getDataValue().dataAsShortInt << ";" << endl;
		break;
	case TypeData::TypeFloat:
		cout << id->getDataValue().dataAsFloat << ";" << endl;
		break;
	default:
		cout << id->getDataValue().dataAsShortInt << ";" << endl;
		break;
	}
}

void TDiagram::showInfoParameter(string message, Node *id, Tree * param, bool needEndl)
{
	for (int i = 0; i < tabs; i++)
		cout << '\t';
	cout << message;
	switch (param->getDataType())
	{
	case TypeData::TypeShortInt:
		cout << " (short) ";
		break;
	case TypeData::TypeFloat:
		cout << " (float) ";
		break;
	}
	cout << param->getID() << " = ";

	if (id->getID() != "")
		cout << id->getID() << " = ";
	else
	{
		switch (id->getDataType())
		{
		case TypeData::TypeConstInt:
			cout << "ConstInt" << " = ";
			break;
		case TypeData::TypeConstChar:
			cout << "ConstChar" << " = ";
			break;
		default:
			cout << "Const" << " = ";
			break;
		}
	}
	switch (id->getDataType())
	{
	case TypeData::TypeShortInt:
		cout << id->getDataValue().dataAsShortInt << ";" << endl;
		break;
	case TypeData::TypeFloat:
		cout << id->getDataValue().dataAsFloat << ";" << endl;
		break;
	default:
		cout << id->getDataValue().dataAsShortInt << ";" << endl;
		break;
	}
	if (!needEndl)
		cout << ")";
	cout << endl;
}

// в функции callFunc копируем узел функции T
// слева от самого себя и выполняем функцию
Tree * TDiagram::callFunc(Tree *T)
{
	showInfo("CALL FUNC: \t\t", T->getData(),false);
	tabs++;
	int ukLocal = sc->GetUK(), ukLineLocal = sc->GetUKLine(), ukInLineLocal = sc->GetUKInLine();
	Tree * localTree = TC->getTemp();

	Tree * l = T->getLeft();
	T->SetLeft(TC->copyTree(T));
	if (l != nullptr)
	{
		T->getLeft()->SetLeft(l);
		l->SetUp(T->getLeft());
	}

	T->getLeft()->SetUp(T);

	uk = T->getUk(); ukLine = T->getUkLine(); ukInLine = T->getUkInLine();
	sc->SetUK(uk, ukLine, ukInLine);
	TC->setTemp(T->getLeft());

	Tree * funcData = functionDef1(true,T->getID(),T->getDataType());
	TC->CleanAfterFuncCalls(T);
	
	uk = ukLocal;ukLine = ukLineLocal ;ukInLine = ukInLineLocal;
	sc->SetUK(uk, ukLine, ukInLine);
	TC->setTemp(localTree);
	tabs--;
	return funcData;
}

void TDiagram::PrintError(int er_type)
{
	cout << endl << "________________________________________________" << endl;
	cout << "Ошибка: " << endl;
	type = sc->GetTypeOfNextLex(lex);
	cout << "\"" << *lex << "\": " << endl;
	cout << " строка: " << sc->GetUKLine() << " столбец: " << sc->GetUKInLine() << endl;
	map <int, string>::iterator it;
	it = ErrorMessages.find(er_type);
	if (it->first != ERROR_FUNC_NO_RETURNS)
		cout << it->second << endl;
	else
		cout << "Функция \"" << *lex << "\" не возвращает значений" << endl;
	cout << "________________________________________________" << endl;
	//cout << "Введите какой-нибудь символ для завершения программы" << endl;
	//_getche();
	exit(1);
}

void TDiagram::maincpp()
{
	bool mainIsDef = false;
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type != TYPE_END)
	{
		//проверяем сначала на пустой оператор ";"
		if (type == TYPE_SEMICOLON) { ; }
		else
		{
			sc->SetUK(uk, ukLine, ukInLine);
			TypeData dt = type2();
			int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();

			ukTemp = sc->GetUK(); ukLineTemp = sc->GetUKLine(); ukInLineTemp = sc->GetUKInLine();
			type = sc->scan(lex);

			LEXs nameIdent = *lex;

			//если это объявление main
			if (type == MAIN)
			{
				mainIsDef = true;
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				//если считали скобочку - это объявление функции
				if (type == TYPE_BRACE_OPEN)
				{
					functionDef1(false, nameIdent, dt);
					tabs++;
					callFunc(TC->Sem3_0(nameIdent));
				}
				else
				{ sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_MAIN_IS_FUNC); }
			}
			else if (type == TYPE_IDENT)
			{
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				//если считали скобочку - это объявление функции
				if (type == TYPE_BRACE_OPEN)
				{ functionDef1(false, nameIdent, dt); }
				//если считали запятую или равно - это данные
				else if (type == TYPE_COMMA || type == TYPE_ASSIGN)
				{
					sc->SetUK(ukT, ukTl, ukTil);
					//считываем список
					list3(dt);
					//список должен заканчиваться точкой с запятой
					uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
					type = sc->scan(lex);
					if (type != TYPE_SEMICOLON)
					{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }
				}
				//если считали точку с запятой - это объявление одной переменной, просто считываем дальше
				else if (type == TYPE_SEMICOLON)
				{
					int er = TC->Sem1(nameIdent, dt, false, tabs);
					if (TC->flagInterpret)
					{
						showInfo("CREATE VAR: \t\t", TC->getTemp()->getData(), false);
						if (TC->getTemp()->getFlagInit())
						{
							showInfo("INIT VAR: \t\t", TC->getTemp()->getData());
						}
					}
					if (er)
					{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(er); }
				}
				//иначе примем, что это объявление одного идентификатора, после которого ожидается точка с запятой
				else
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }
			}
			else
			{ sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_EXPECTED_IDENT); }
			//если считали идентификатор - это список данных или объявление функции	

		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	if (!mainIsDef)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_MAIN_ISNT_DEF); }
	cout << "синтаксис программы верен" << endl;
	TC->printTC(0);
}

Tree * TDiagram::functionDef1(bool needToInterpret, LEXs nameIdent, TypeData dt)
{
	bool localFlagInterpret = TC->flagInterpret;
	Tree *funcNode;

	if (!needToInterpret)
	{
		TC->flagInterpret = false;
		Node *newnode = new Node(nameIdent, dt, NULL, 0);
		int er = TC->Sem2_0(newnode);
		if (er)
		{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(er); }
		//у функции может не быть параметров
		//но мы должны проверить, есть ли они
		TreeControl *listInFuncTree = new TreeControl();
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		sc->SetUK(uk, ukLine, ukInLine);
		//если скобка не закрылась сразу, то дальше должны идти параметры
		if (type != TYPE_BRACE_CLOSED)
		{   listInFuncDef14(listInFuncTree, newnode); }

		//после параметров точно должна закрываться скобка
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		if (type != TYPE_BRACE_CLOSED)
		{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }

		TC->AddNeighbour(newnode, tabs);
		funcNode = TC->getTemp();
		funcNode->initVar();
		TDataValue k; k.dataAsFloat = 0; k.dataAsShortInt = 0;
		funcNode->setDataValue(k, funcNode->getDataType());

		showInfo("INIT FUNC: \t\t", funcNode->getData());
		//после скобок должен идти составной оператор, который начинается
		//с фигурной скобки
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		if (type != TYPE_CUR_BR_OPEN)
		{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_CUR_BRACE_OPEN); }
		//вот здесь сохраняем в узел функции указатель в тексте
		funcNode->getData()->setUks(sc->GetUK(),sc->GetUKLine(),sc->GetUKInLine());
	}
	else
	{
		funcNode = TC->Sem3_0(TC->getRoot(), nameIdent);
		funcNode = funcNode->getLeft();
	}
	TC->AddChild(new Node(), tabs); //открытие составного оператора
	Tree* funcData = sostavnoi_operator15(funcNode);
	TC->deleteUpToEmpty();
	
	if (
		funcData->getDataType() != TypeData::TypeUnreturned &&
		TC->flagInterpret)
	{ return funcData; }
	else if (funcData->getDataType() == TypeData::TypeUnreturned && TC->flagInterpret)
	{ sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);	PrintError(ERROR_FUNC_NO_RETURNS); }
	TC->flagInterpret = localFlagInterpret;
	funcData = new Tree(nullptr,nullptr,nullptr,new Node(TypeData::TypeUnreturned));
	return funcData;
	//кстати, восстановление указателей в тексте и в дереве происходят уже в callFunc
	//как и удаление копий функции
}

TypeData TDiagram::type2()
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	TypeData ret;
	if (type == SHORT || type == FLOAT)
	{
		if (type == SHORT)
		{
			ret = TypeData::TypeShortInt;
			type = sc->scan(lex);
		}
		else
			ret = TypeData::TypeFloat;			
		if (type != INT && type != FLOAT)
		{
			sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);
		}
	}
	else
	{
		sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE);
	}
	return ret;
}

void TDiagram::list3(TypeData dt)
{
	bool h;
	Tree* var = variable4(dt, false, &h);
	if (TC->flagInterpret)
	{
		showInfo("CREATE VAR: \t\t", var->getData(), false);
		if (var->getFlagInit())
		{ showInfo("INIT VAR: \t\t", var->getData()); }
	}
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_COMMA)
	{
		bool h;
		var = variable4(dt, false, &h);
		if (TC->flagInterpret)
		{
			showInfo("CREATE VAR: \t\t", var->getData(), false);
			if (var->getFlagInit())
			{ showInfo("INIT VAR: \t\t", var->getData()); }
		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
}

Tree *TDiagram::variable4(TypeData dt, bool init, bool *hereIsAssign)
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_IDENT)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_IDENT); }
	int er = TC->Sem1(*lex, dt, init, tabs);
	if (er)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(er); }

	Tree *var = TC->getTemp();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type == TYPE_ASSIGN)
	{
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *tr = expression9();
		if (TC->flagInterpret)
		{
			if (var->checkIfTypesOK(tr))
			{
				*hereIsAssign = true;
				var->initVar();
				var->setDataValue(tr->getDataValue(), tr->getDataType());
			}
			else
			{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH); }
		}
		else
		{ *hereIsAssign = true; }
	}
	else
	{ sc->SetUK(uk, ukLine, ukInLine); }

	return var;
}

Tree *TDiagram::parameter17(bool *hereIsNewVarAssign, bool *isNewVar)
{
	Tree *param;
	*hereIsNewVarAssign = false;
	*isNewVar = false;

	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	TypeData dt;

	if (type == FLOAT || type == SHORT)
	{
		if (type == SHORT)
		{
			dt = TypeData::TypeShortInt;
			type = sc->scan(lex);
		}
		else
			dt = TypeData::TypeFloat;
		if (type != INT && type != FLOAT)
		{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE); }
		int uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		*isNewVar = true;
		param = variable4(dt, false, hereIsNewVarAssign);
		if (TC->flagInterpret)
		{
			showInfo("CREATE VAR: \t\t", param->getData(), false);
			if (param->getFlagInit())
			{ showInfo("INIT VAR: \t\t", param->getData()); }
			if (!param->getFlagInit())
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_VAR_IS_NOT_INIT); }
		}
	}
	else if (type == TYPE_IDENT || type == MAIN)
	{
		LEXs name = *lex;
		Tree *T = TC->Sem3_0(name);

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);

		//если считали скобку - то это функция
		if (type == TYPE_BRACE_OPEN)
		{
			if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
			{ sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST); }
			//у функции может не быть параметров
			//но мы должны проверить, есть ли они
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);

			//если скобка не закрылась сразу, то дальше должны идти параметры
			if (type != TYPE_BRACE_CLOSED)
			{
				if (T->getParamsQuantity() == 0)
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS); }

				sc->SetUK(uk, ukLine, ukInLine);
				parameters6(T);

				//после параметров точно должна закрываться скобка
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_BRACE_CLOSED)
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }
			}
			else if (T->getParamsQuantity() != 0)
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS); }
			if (TC->flagInterpret)
			{
				tabs++;
				param = callFunc(T);
				tabs--;
			}
			else
				param = T;
		}
		//иначе это может быть присваивание
		else if (type == TYPE_ASSIGN)
		{
			if (T == NULL || T->getObjectType() != TypeObject::ObjVar)
			{ sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_VAR_DOES_NOT_EXIST); }

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			param = expression9();

			if (TC->flagInterpret)
			{
				if (!T->checkIfTypesOK(param))
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_TYPES_DONT_MATCH); }

				T->setDataValue(param->getDataValue(), param->getDataType());
				if (T->getFlagInit())
				{ showInfo("ASSIGN VAR: \t\t", T->getData()); }
				else
				{ T->initVar(); showInfo("INIT VAR: \t\t", T->getData()); }
			}
		}
		//иначе это одна переменная
		else
		{
			sc->SetUK(ukT, ukTl, ukTil);
			param = expression9();
		}
	}
	//иначе это выражение
	else
	{
		//восстанавливаем указатели до считанной лексемы
		sc->SetUK(uk, ukLine, ukInLine);
		//считываем лексему
		param = expression9();
	}
	if (TC->flagInterpret && !param->getFlagInit())
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_VAR_IS_NOT_INIT); }
	return param;
}

void TDiagram::parameters6(Tree *T)
{
	if (TC->flagInterpret)
	{
		for (int i = 0; i < tabs; i++)
			cout << '\t';
		cout << "  PARAMETERS FOR FUNCTION (" << T->getID() << ") :" << endl;
		tabs++;
	}
	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	Tree *tempParamInFunc = T->getParams();
	bool isNewVar, isAssignToNewVar;

	Tree *param = parameter17(&isAssignToNewVar,&isNewVar);
	if (TC->flagInterpret)
	{
		if (!tempParamInFunc->checkIfTypesOK(param))
		{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH); }
		else
		{
			bool needEndl = false;
			if (tempParamInFunc->getLeft() != nullptr)
			{ needEndl = true; }
			tempParamInFunc->setDataValue(param->getDataValue(), param->getDataType());
			showInfoParameter("( INIT PARAMETER: \t", param->getData(), tempParamInFunc, needEndl);
		}
	}
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_COMMA)
	{
		ukT = sc->GetUK(); ukTl = sc->GetUKLine(); ukTil = sc->GetUKInLine();

		tempParamInFunc = tempParamInFunc->getLeft();
		if (tempParamInFunc == nullptr)
		{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_FUNC_HAS_LESS_PARAMS); }
		param = parameter17(&isAssignToNewVar, &isNewVar);
		if (TC->flagInterpret)
		{
			if (!tempParamInFunc->checkIfTypesOK(param))
			{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_TYPES_DONT_MATCH); }
			else
			{ 
				bool needEndl = false;
				if (tempParamInFunc->getLeft() != nullptr)
				{ needEndl = true; }
				tempParamInFunc->setDataValue(param->getDataValue(), param->getDataType()); 
				showInfoParameter("  INIT PARAMETER: \t", param->getData(), tempParamInFunc, needEndl);
			}
		}

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);

	//когда закончатся переданные параметры, надо проверить, остались ли в
	//объявлении функции еще параметры.
	tempParamInFunc = tempParamInFunc->getLeft();
	if (tempParamInFunc != NULL)
	{ PrintError(ERROR_FUNC_HAS_MORE_PARAMS); }
	if (TC->flagInterpret) { tabs--; }
}

Tree * TDiagram::while7(Tree *funcNode)
{
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	//уже считали "while", дальше должно идти "("
	if (type != TYPE_BRACE_OPEN)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_OPEN); }

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	int ukLocal = uk, ukLineLocal = ukLine, ukInLineLocal = ukInLine;
	//Tree * localTree = TC->getTemp();
	bool localFlagInterpret = TC->flagInterpret;

	//дальше должно идти выражение, если же идёт закрывающая скобка, то выводим ошибку
	type = sc->scan(lex);
	sc->SetUK(uk, ukLine, ukInLine);
	if (type == TYPE_BRACE_CLOSED)
	{ PrintError(ERROR_EXPECTED_EXPRESSION); }

again:
	TC->AddNeighbour(new Node(), tabs);
	TC->AddChild(new Node(), tabs);//открытие составного оператора
	bool isNewVar, isAssignToNewVar;
	//считываем выражение
	Tree * condition = parameter17(&isAssignToNewVar, &isNewVar);
	//если объявлена новая переменная, но не инициализирована, то это ошибка
	if (isNewVar && !isAssignToNewVar)
	{
		uk = ukLocal; ukLine = ukLineLocal; ukInLine = ukInLineLocal;
		sc->SetUK(uk, ukLine, ukInLine); PrintError(ERROR_VAR_IS_NOT_INIT);
	}

	//дальше должна идти закрывающая скобка
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_BRACE_CLOSED)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }
	//и наконец оператор в цикле
	if (condition->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
	{
		bool ifInWhile = false;
		switch (condition->getDataType())
		{
		case TypeData::TypeShortInt:
			if (condition->getDataValue().dataAsShortInt != 0)
			{	ifInWhile = true; }
			break;
		case TypeData::TypeFloat:
			if (abs(condition->getDataValue().dataAsFloat) > 4.296e-32)
			{	ifInWhile = true; }
			break;
		default:
			if (condition->getDataValue().dataAsShortInt != 0)
			{	ifInWhile = true; }
			break;
		}

		if (ifInWhile)
		{
			Tree* dataWhile = operator5(false, funcNode);
			TC->deleteUpToEmpty();
			if (dataWhile->getDataType() != TypeData::TypeUnreturned)
				return dataWhile;
			//восстанавливаем указатели
			//и переходим на момент считывания выражения
			//то есть идём на новый заход по циклу
			uk = ukLocal; ukLine = ukLineLocal; ukInLine = ukInLineLocal;
			sc->SetUK(uk, ukLine, ukInLine);
			//TC->setTemp(localTree);
			goto again;
		}
		TC->flagInterpret = false; //для считывания блока внутри while без интерпретации
		operator5(false, funcNode);
		TC->deleteUpToEmpty();
		TC->flagInterpret = true;
		Tree *funcData = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
		return funcData;
	}
	operator5(false, funcNode);
	TC->deleteUpToEmpty();
	TC->flagInterpret = localFlagInterpret;
	Tree *funcData = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
	return funcData;
}

Tree * TDiagram::operator5(bool makeChild, Tree *funcNode)
{
	Tree *funcData;
	int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	//если это точка с запятой, то ничего не делаем, а указатели переставляем ниже
	if (type == TYPE_SEMICOLON) { ; }
	else if (type == RETURN)
	{
		funcData = return_operator16(funcNode);
		if (TC->flagInterpret)
		{ return funcData; }
	}
	else if (type == WHILE)
	{
		funcData = while7(funcNode);
		if (funcData->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
		{ return funcData; }
	}
	else if (type == TYPE_CUR_BR_OPEN)
	{
		Tree *oldTemp;
		if (makeChild)
		{
			TC->AddNeighbour(new Node(), tabs);
			oldTemp = TC->getTemp();
			TC->AddChild(new Node(), tabs); //открытие составного оператора
			funcData = sostavnoi_operator15(funcNode);
			TC->deleteUpToEmpty();
			if (funcData->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
			{ return funcData; }
		}
		else
		{
			funcData = sostavnoi_operator15(funcNode);
			if (funcData->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
			{ return funcData; }
		}
	}
	//если дальше идет идентификатор, то это либо присвоение переменной значения,
	//либо вызов функции
	else if (type == TYPE_IDENT || type == MAIN)
	{
		LEXs name = *lex;
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		//если считали скобку - то это функция
		if (type == TYPE_BRACE_OPEN)
		{
			Tree *T = TC->Sem3_0(name);
			if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
			{ sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST); }
			//у функции может не быть параметров
			//но мы должны проверить, есть ли они
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);

			//если скобка не закрылась сразу, то дальше должны идти параметры
			if (type != TYPE_BRACE_CLOSED)
			{
				if (T->getParamsQuantity() == 0)
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS); }

				sc->SetUK(uk, ukLine, ukInLine);
				parameters6(T);

				//после параметров точно должна закрываться скобка
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_BRACE_CLOSED)
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }
			}
			else if (T->getParamsQuantity() != 0)
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS); }
			if(TC->flagInterpret)
				callFunc(T); // в функции callFunc копируем узел функции T
							 // слева от самого себя и выполняем функцию
		}
		//иначе это должно быть присвоение 
		else if (type == TYPE_ASSIGN)
		{
			Tree *expr = TC->Sem4_0(name);
			if (expr == NULL || expr->getObjectType() != TypeObject::ObjVar)
			{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST); }

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();

			Tree *param = expression9();
			if (TC->flagInterpret)
			{
				if (!expr->checkIfTypesOK(param))
				{ sc->SetUK(uk, ukLine, ukInLine); PrintError(ERROR_TYPES_DONT_MATCH); }
				expr->setDataValue(param->getDataValue(), param->getDataType());
				if (expr->getFlagInit())
				{ showInfo("ASSIGN VAR: \t\t", expr->getData()); }
				else
				{ expr->initVar();  showInfo("INIT VAR: \t\t", expr->getData()); }
			}
		}
		else
		{ 
			//sc->SetUK(ukTemp, ukLineTemp, ukInLineTemp);
			sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_EXPECTED_FUNCTION_OR_ASSIGNING); }
		//и заканчивается оно точкой с запятой
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		if (type != TYPE_SEMICOLON)
		{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }
	}
	else
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_OPERATOR); }
	
	funcData = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
	return funcData;
}

Tree * TDiagram::opers_desrps8(Tree *funcNode)
{
	Tree *funcData;

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type != TYPE_CUR_BR_CLOSED)
	{
		//если сначала стоит тип, то 
		//это должно быть объявление переменной/списка
		if (type == SHORT || type == FLOAT)
		{
			TypeData dt;
			if (type == SHORT)
			{
				dt = TypeData::TypeShortInt;
				type = sc->scan(lex);
			}
			else
				dt = TypeData::TypeFloat;
			if (type != INT && type != FLOAT)
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_TYPE); }
			int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);

			LEXs nameIdent = *lex;
			//если считали идентификатор - это список данных
			//другого не дано
			if (type != TYPE_IDENT)
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_IDENT); }

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			if (type == TYPE_COMMA || type == TYPE_ASSIGN)
			{
				sc->SetUK(ukT, ukTl, ukTil);
				//считываем список
				list3(dt);
				//список должен заканчиваться точкой с запятой
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);
				if (type != TYPE_SEMICOLON)
				{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }
			}
			//если считали точку с запятой - это объявление одной переменной, просто считываем дальше
			else if (type == TYPE_SEMICOLON)
			{
				int er = TC->Sem1(nameIdent, dt, false, tabs);
				if (er)
				{ sc->SetUK(ukT, ukTl, ukTil);	PrintError(er); }
			}
			//иначе примем, что это объявление одного идентификатора, после которого ожидается точка с запятой
			else
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }
		}
		//либо это оператор
		else if (type == TYPE_SEMICOLON || type == RETURN || type == WHILE || type == TYPE_CUR_BR_OPEN || type == TYPE_IDENT || type == MAIN)
		{
			sc->SetUK(uk, ukLine, ukInLine);
			funcData = operator5(true, funcNode);
			if (funcData->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
			{ return funcData; }
		}
		//либо ошибка
		else
		{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_OPER_OR_VAR); }
		//считываем следующую лексему
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	funcData = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
	return funcData;
}

Tree * TDiagram::expression9()
{
	Tree *expr1 = shift_res10();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_GT || type == TYPE_GE || type == TYPE_LT || type == TYPE_LE || type == TYPE_EQ || type == TYPE_NEQ)
	{
		int typeDump = type;
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = shift_res10();
		if (TC->flagInterpret)
		{
			switch (typeDump)
			{
			case TYPE_GT:
				expr1 = *expr1 > *expr2; break;
			case TYPE_GE:
				expr1 = *expr1 >= *expr2; break;
			case TYPE_LT:
				expr1 = *expr1 < *expr2; break;
			case TYPE_LE:
				expr1 = *expr1 <= *expr2; break;
			case TYPE_EQ:
				expr1 = *expr1 == *expr2; break;
			case TYPE_NEQ:
				expr1 = *expr1 != *expr2; break;
			}
		}

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::shift_res10()
{
	int ukN = uk, ukTN = ukLine, ukTiN = ukInLine;
	Tree *expr1 = addendum11();
	
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (TC->flagInterpret && (type == TYPE_LS || type == TYPE_GS))
	{
		if (expr1->getDataType() == TypeData::TypeFloat)
		{ sc->SetUK(ukN, ukTN, ukTiN); PrintError(ERROR_OPERAND_IS_FLOAT); }
	}
	while (type == TYPE_LS || type == TYPE_GS)
	{
		int typeDump = type;
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = addendum11();
		if (TC->flagInterpret)
		{
			if (expr2->getDataType() == TypeData::TypeFloat)
			{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_OPERAND_IS_FLOAT); }
			switch (typeDump)
			{
			case TYPE_LS:
				expr1 = *expr1 << *expr2; break;
			case TYPE_GS:
				expr1 = *expr1 >> *expr2; break;
			}
		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::addendum11()
{
	bool flagminus = false;
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type == TYPE_MINUS)
	{ flagminus = true; }
	if (type != TYPE_PLUS && type != TYPE_MINUS)
	{ sc->SetUK(uk, ukLine, ukInLine); }
	Tree *expr1 = multiplier12();
	if (TC->flagInterpret && flagminus)
	{ expr1 = -(*expr1); }
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_PLUS || type == TYPE_MINUS)
	{
		int typeDump = type;
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = multiplier12();
		if (TC->flagInterpret)
		{
			switch (typeDump)
			{
			case TYPE_PLUS:
				expr1 = *expr1 + *expr2; break;
			case TYPE_MINUS:
				expr1 = *expr1 - *expr2; break;
			}
		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::multiplier12()
{
	Tree *expr1 = simple_expression13();
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);

	while (type == TYPE_MUL || type == TYPE_DIV)
	{
		int typeDump = type;
		int ukT = sc->GetUK(), ukTl = sc->GetUKLine(), ukTil = sc->GetUKInLine();
		Tree *expr2 = simple_expression13();

		if (TC->flagInterpret)
		{
			switch (typeDump)
			{
			case TYPE_DIV:
				float secondData;
				if (expr2->getDataType() == TypeData::TypeFloat)	
					secondData = expr2->getDataValue().dataAsFloat;
				else	
					secondData = expr2->getDataValue().dataAsShortInt;
				if (abs(secondData) < 4.3e-32)
				{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_DIV_ZERO_EXCEPTION); }

				expr1 = *expr1 / *expr2; break;
			case TYPE_MUL:
				expr1 = *expr1 * *expr2; break;
			}
		}
		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);
	return expr1;
}

Tree * TDiagram::simple_expression13()
{
	Tree *simple_expr = NULL;
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	int ukT = uk, ukTl = ukLine, ukTil = ukInLine;
	type = sc->scan(lex);
	if (type != TYPE_CONST_10 && type != TYPE_CONST_CHAR)
	{
		if (type == TYPE_BRACE_OPEN)
		{
			simple_expr = expression9();
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			if (type != TYPE_BRACE_CLOSED)
			{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }
		}
		else if (type == TYPE_IDENT || type == MAIN)
		{
			LEXs name = *lex;
			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			type = sc->scan(lex);
			//если считали скобку - то это функция
			if (type == TYPE_BRACE_OPEN)
			{
				Tree *T = TC->Sem3_0(name);
				if (T == NULL || T->getObjectType() != TypeObject::ObjFunc)
				{   sc->SetUK(ukT, ukTl, ukTil);	PrintError(ERROR_FUNC_DOES_NOT_EXIST); }
				//у функции может не быть параметров
				//но мы должны проверить, есть ли они
				uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
				type = sc->scan(lex);

				//если скобка не закрылась сразу, то дальше должны идти параметры
				if (type != TYPE_BRACE_CLOSED)
				{
					if (T->getParamsQuantity() == 0)
					{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_LESS_PARAMS); }

					sc->SetUK(uk, ukLine, ukInLine);
					parameters6(T); //вот здесь мы записываем значения параметров в узел функции

					//после параметров точно должна закрываться скобка
					uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
					type = sc->scan(lex);
					if (type != TYPE_BRACE_CLOSED)
					{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_BRACE_CLOSED); }
				}
				else if (T->getParamsQuantity() != 0)
				{   sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_FUNC_HAS_MORE_PARAMS); }
				if (TC->flagInterpret)
					simple_expr = callFunc(T); // в функции callFunc копируем узел функции T
											   // слева от самого себя и выполняем функцию
				else
					simple_expr = T;
			}
			//иначе это просто переменная, возвращаем указатели перед переменной
			else
			{
				sc->SetUK(ukT, ukTl, ukTil);
				sc->scan(lex);
				//к этому моменту у нас указатели стоят после переменной
				//а в lex считан идентификатор переменной
				simple_expr = TC->Sem4_0(*lex);
				if (simple_expr == NULL || simple_expr->getObjectType() != TypeObject::ObjVar)
				{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST); }
				else if (!simple_expr->getFlagInit() && TC->flagInterpret)
				{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_IS_NOT_INIT); }
			}
		}
		else
		{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_EXPRESSION); }
	}
	else
	{
		if (type == TYPE_CONST_10)
		{
			simple_expr = new Tree(NULL, NULL, NULL, new Node(TypeData::TypeConstInt));
			simple_expr->initVar();
			TDataValue k; k.dataAsShortInt = atof(lex->c_str());
			simple_expr->setDataValue(k, TypeData::TypeConstInt);
		}
		else if (type == TYPE_CONST_CHAR)
		{
			simple_expr = new Tree(NULL, NULL, NULL, new Node(TypeData::TypeConstChar));
			simple_expr->initVar();
			TDataValue k;
			if (lex->c_str()[1] != '\\')
			{ k.dataAsShortInt = lex->c_str()[1]; }
			else
			{ k.dataAsShortInt = '\\' + lex->c_str()[2]; }

			simple_expr->setDataValue(k, TypeData::TypeConstChar);
		}
	}
	return simple_expr;
}

void TDiagram::listInFuncDef14(TreeControl *listInFuncTree, Node *funcNode)
{
	for (int i = 0; i < tabs; i++)
		cout << '\t';
	cout << "  ATTRIBUTES FOR FUNCTION (" << funcNode->getID() << ") :" << endl;
	tabs++;

	bool localFlagInterpret = TC->flagInterpret;
	TC->flagInterpret = true;
	//cout << "Считываем список параметров при объявлении функции" << endl;
	TreeControl *oldTree = TC;
	TC = listInFuncTree;

	TypeData dt = type2();
	bool isAssignToNewVar;
	Tree* var = variable4(dt, true, &isAssignToNewVar);
	if (var->getFlagInit())
	{ showInfo("	INIT ATTRIBUTE: \t", var->getData()); }
	funcNode->incParamsQuantity();

	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	while (type == TYPE_COMMA)
	{
		dt = type2();
		bool isAssignToNewVar;
		var = variable4(dt, true, &isAssignToNewVar);
		if (var->getFlagInit())
		{
			TDataValue k; k.dataAsFloat = 0; k.dataAsShortInt = 0;
			showInfo("	  INIT ATTRIBUTE: \t", var->getData());
		}
		funcNode->incParamsQuantity();

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
	}
	sc->SetUK(uk, ukLine, ukInLine);

	funcNode->setParams(TC);
	TC = oldTree;
	TC->flagInterpret = localFlagInterpret;
	/*for (int tabsi = 0; tabsi <= tabs; tabsi++)
		cout << '\t';
	cout << "Список параметров при объявлении функции считан" << endl << endl;*/
	tabs--;
}

Tree * TDiagram::sostavnoi_operator15(Tree *funcNode)
{
	//перед составным оператором уже считали фигурную скобочку
	//поэтому начинаем сразу с внутренностей оператора
	Tree* funcData = opers_desrps8(funcNode);
	if (funcData->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
	{ return funcData; }

	//и заканчивается он фигурной скобочкой
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_CUR_BR_CLOSED)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_CUR_BRACE_CLOSED); }

	funcData = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
	return funcData;
}

Tree * TDiagram::return_operator16(Tree *funcNode)
{
	Tree *param;
	//уже должны были считать "return", поэтому смотрим, что за ним
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	int ukT = uk, ukTl = ukLine, ukTil = ukInLine;
	type = sc->scan(lex);
	//за ним может быть либо переменная, либо элементарное выражение
	//если идентификатор, то это может быть либо присвоение, либо просто переменная, либо функция
	if (type == TYPE_IDENT || type == MAIN)
	{
		LEXs name = *lex;

		uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
		type = sc->scan(lex);
		//если считали =, то это присвоение
		if (type == TYPE_ASSIGN)
		{
			param = TC->Sem4_0(name);
			if (param == NULL || param->getObjectType() != TypeObject::ObjVar)
			{ sc->SetUK(ukT, ukTl, ukTil); PrintError(ERROR_VAR_DOES_NOT_EXIST); }

			uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
			Tree *expr = expression9();
			if (TC->flagInterpret)
			{
				if (!param->checkIfTypesOK(expr))
				{ sc->SetUK(uk, ukLine, ukInLine); PrintError(ERROR_TYPES_DONT_MATCH); }
				param->setDataValue(expr->getDataValue(), expr->getDataType());
				param->initVar();
			}
		}
		//иначе это выражение 
		else
		{
			sc->SetUK(ukT, ukTl, ukTil);
			param = expression9();
		}
	}
	//иначе это выражение
	else
	{
		sc->SetUK(ukT, ukTl, ukTil);
		param = expression9();
	}

	if (param->getDataType() != TypeData::TypeUnreturned && TC->flagInterpret)
	{
		if (!funcNode->checkIfTypesOK(param))
		{ sc->SetUK(ukT, ukTl, ukTil);  PrintError(ERROR_TYPES_DONT_MATCH_RET); }
		return TC->copyTree(param);
	}

	//после возвращаемого значения обязательно должно стоять ";"
	uk = sc->GetUK(); ukLine = sc->GetUKLine(); ukInLine = sc->GetUKInLine();
	type = sc->scan(lex);
	if (type != TYPE_SEMICOLON)
	{ sc->SetUK(uk, ukLine, ukInLine);	PrintError(ERROR_EXPECTED_SEMICOLON); }

	param = new Tree(nullptr, nullptr, nullptr, new Node(TypeData::TypeUnreturned));
	return param;
}

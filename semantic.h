#pragma once
#include "defs.h"
#include "scaner.h"

enum TypeObject {
	ObjVar = 1,		// простая переменная
	ObjFunc,		// функция

	ObjEmpty		//пустой узел
};

enum TypeData {
	TypeShortInt = 1,
	TypeFloat,

	TypeConstInt,
	TypeConstChar,

	TypeUnreturned
};
union TDataValue {
	short int	dataAsShortInt;
	float		dataAsFloat;
	TDataValue();
};

class Node;
class Tree;
class TreeControl;

class Node
{
private:
	// идентификатор объекта
	LEXs   id;
	// тип  объекта (функция/переменная/неименованная константа/пустой узел)
	TypeObject   ObjectType;
	// тип (short int/float/constInt/constChar)
	TypeData DataType;
	TDataValue Value;
	// количество параметров функции
	int ParamsQuantity;
	//поддерево параметров функции
	Tree *Params;
	// Флаг инициализации переменной
	bool FlagInit;
	int uk, ukLine, ukInLine;
public:
	//получить данные о типе объекта
	TypeObject getObjectType() const;
	void setDataObject(TypeObject objtype);
	void setID(LEXs id_);
	//получить тип данных
	TypeData getDataType() const;
	//получить значение переменной / возвращаемое значение функции
	TDataValue getDataValue() const;
	//установить значение переменной / возвращаемое значение функции
	void setDataValue(TDataValue val, TypeData dtype);
	void setDataType(TypeData dtype);
	//для функций и переменных получить идентификатор
	LEXs getID() const;
	//конструктор для переменной
	Node(LEXs id_, TypeData dt);
	//для переменных - инициализировать переменную
	void InitVar();
	//узнать, инициализирована ли переменная
	bool getFlagInit() const;

	//конструктор для функции
	Node(LEXs id_, TypeData dt, Tree *prms, int prmsQuantity);
	//получить ссылку на дерево параметров функции
	Tree *getParams();
	//назначить функции дерево параметров
	void setParams(TreeControl *t);
	void setParams(Tree *t);
	//получить количество параметров функции
	int getParamsQuantity() const;
	//увеличить показатель количества параметров функции
	void incParamsQuantity();

	//конструктор для неименованной константы
	Node(TypeData dt);
	//конструктор для пустого узла
	Node();

	~Node() {};
	void setUks(int uk_, int ukLine_, int ukInLine_);
	int getUk() const;
	int getUkLine() const;
	int getUkInLine() const;
};

class Tree {
private:
	// данные таблицы
	Node * data;          
	// родитель, левый и правый потомок
	Tree * Up, *Left, *Right;
public:
	//конструкторы
	Tree(Tree * l, Tree * r, Tree * u, Node * Data);
	Tree();
		
		TypeObject getObjectType() const;
		TypeData getDataType() const;
		TDataValue getDataValue() const;
		void setDataValue(TDataValue val, TypeData dtype);

		bool getFlagInit() const;
		void initVar();
		Tree *getParams();
		int getParamsQuantity() const;
		LEXs getID() const;
		
	//проверить приводимость типов
	bool checkIfTypesOK(Tree *checkWithThis) const;
	//добавить соседа
	void SetLeft(Node * data);
	//добавить потомка
	void SetRight(Node * data);

	void deleteLeft();
	void deleteRight();
	void deleteUp();

	//получить ссылку на соседа
	Tree *getLeft();
	//получить ссылку на потомка
	Tree *getRight();
	//получить ссылку на соседа сверху/родителя
	Tree *getUp();
	//найти в семантическом дереве узел, где был объявлен этот идентификатор
	Tree *FindUp(Tree * from, Node *id, bool &parentOrHigher);
		
	~Tree() {};

	Node * getData();

	void SetLeft(Tree* newTree);
	void SetRight(Tree* newTree);
	void SetUp(Tree* newTree);

	void setUks(int uk_, int ukLine_, int ukInLine_);
	int getUk() const;
	int getUkLine() const;
	int getUkInLine() const;

	void setID(LEXs id_);
};
//класс, в котором производится основная работа над семантическим деревом
//и где хранится это семантическое дерево
class TreeControl
{
private:
	//верхний узел дерева
	Tree *root;
	//активный узел дерева
	Tree *temp;

	map <TypeData, string> types;
	//печать параметров из узла функции
	void printFuncParams(Tree *temp);
	//печать дерева
	void printTree(Tree *temp);

public:
	bool flagInterpret;
	//конструктор
	TreeControl();
	//добавить соседа
	void AddNeighbour(Node *neighbour, int tabs);
	//добавить потомка
	void AddChild(Node *child, int tabs);
	//поиск узла выше - также показывает, нашли идентификатор в этом блоке или выше
	Tree *findUp(Node *id, bool &parentOrHigher);	
	//при возможности объявление переменной
	//при неуспехе возвращает ошибку
	int Sem1(LEXs id, TypeData dt, bool init, int tabs);
	//если функция уже объявлена, возвращает ошибку
	int Sem2_0(Node *newnode);
	//возвращает узел функции с этим именем (либо NULL, если такой нет)
	Tree *Sem3_0(LEXs id);
	Tree *Sem3_0(Tree* from, LEXs id);
	//возвращает узел переменной с этим именем (либо NULL, если такой нет)
	Tree *Sem4_0(LEXs id);

	//возвращает верхний узел семантического дерева
	Tree * getRoot();
	//возвращает ссылку на активный узел
	Tree *getTemp();

	//функция для назначения активного узла
	//(используется после окончания блока для возвращения)
	void setTemp(Tree *newtemp);

	//печать семантического дерева
	void printTC(int tabs);

	~TreeControl() {};

	void deleteUpToEmpty();

	Node* copyNode(Node* oldNode);
	// копировать сам узел и его параметры, если есть (не затрагивает соседей/детей)
	Tree* copyTree(Tree* oldTree); 
	void CleanAfterFuncCalls(Tree *func);
};
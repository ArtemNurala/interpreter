#include "operators.h"

Tree* operator<<(Tree firstOperand_, Tree secondOperand_)
{
	TDataValue res; res.dataAsShortInt = firstOperand_.getDataValue().dataAsShortInt << secondOperand_.getDataValue().dataAsShortInt;
	Node* result = new Node(TypeData::TypeShortInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeShortInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator>>(Tree firstOperand_, Tree secondOperand_)
{
	TDataValue res; res.dataAsShortInt = firstOperand_.getDataValue().dataAsShortInt >> secondOperand_.getDataValue().dataAsShortInt;
	Node* result = new Node(TypeData::TypeShortInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeShortInt);
	return new Tree(NULL, NULL, NULL, result);
}

Tree * retTreeFromFloatDataValue(TDataValue res)
{
	Node* result;
	if (res.dataAsFloat == (int)res.dataAsFloat)
	{
		res.dataAsShortInt = res.dataAsFloat;
		result = new Node(TypeData::TypeConstInt);
		result->InitVar();
		result->setDataValue(res, TypeData::TypeConstInt);
	}
	else
	{
		result = new Node(TypeData::TypeFloat);
		result->InitVar();
		result->setDataValue(res, TypeData::TypeFloat);
	}
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator /(Tree firstOperand_, Tree secondOperand_)
{
	float firstData;
	if (firstOperand_.getDataType() == TypeData::TypeFloat)
		firstData = firstOperand_.getDataValue().dataAsFloat;
	else
		firstData = firstOperand_.getDataValue().dataAsShortInt;
	float secondData;
	if (secondOperand_.getDataType() == TypeData::TypeFloat)
		secondData = secondOperand_.getDataValue().dataAsFloat;
	else
		secondData = secondOperand_.getDataValue().dataAsShortInt;

	TDataValue res; res.dataAsFloat = firstData / secondData;

	Node* result = new Node(TypeData::TypeFloat);
	return retTreeFromFloatDataValue(res);
}
Tree* operator *(Tree firstOperand_, Tree secondOperand_)
{
	float firstData;
	if (firstOperand_.getDataType() == TypeData::TypeFloat)
		firstData = firstOperand_.getDataValue().dataAsFloat;
	else
		firstData = firstOperand_.getDataValue().dataAsShortInt;
	float secondData;
	if (secondOperand_.getDataType() == TypeData::TypeFloat)
		secondData = secondOperand_.getDataValue().dataAsFloat;
	else
		secondData = secondOperand_.getDataValue().dataAsShortInt;

	TDataValue res; res.dataAsFloat = firstData * secondData;
	return retTreeFromFloatDataValue(res);
}
Tree* operator -(Tree firstOperand_, Tree secondOperand_)
{
	float firstData;
	if (firstOperand_.getDataType() == TypeData::TypeFloat)
		firstData = firstOperand_.getDataValue().dataAsFloat;
	else
		firstData = firstOperand_.getDataValue().dataAsShortInt;
	float secondData;
	if (secondOperand_.getDataType() == TypeData::TypeFloat)
		secondData = secondOperand_.getDataValue().dataAsFloat;
	else
		secondData = secondOperand_.getDataValue().dataAsShortInt;

	TDataValue res; res.dataAsFloat = firstData - secondData;
	return retTreeFromFloatDataValue(res);
}
Tree* operator -(Tree firstOperand)
{
	TDataValue k = firstOperand.getDataValue();
	if (firstOperand.getDataType() == TypeData::TypeFloat)
	{
		k.dataAsFloat = -k.dataAsFloat;
	}
	else
	{
		k.dataAsShortInt = -k.dataAsShortInt;
	}

	if (firstOperand.getDataType() == TypeData::TypeFloat)
	{
		return retTreeFromFloatDataValue(k);
	}
	Node* result = new Node(firstOperand.getDataType());
		result->InitVar();
		result->setDataValue(k, firstOperand.getDataType());
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator +(Tree firstOperand_, Tree secondOperand_)
{
	float firstData;
	if (firstOperand_.getDataType() == TypeData::TypeFloat)
		firstData = firstOperand_.getDataValue().dataAsFloat;
	else
		firstData = firstOperand_.getDataValue().dataAsShortInt;
	float secondData;
	if (secondOperand_.getDataType() == TypeData::TypeFloat)
		secondData = secondOperand_.getDataValue().dataAsFloat;
	else
		secondData = secondOperand_.getDataValue().dataAsShortInt;

	TDataValue res; res.dataAsFloat = firstData + secondData;
	return retTreeFromFloatDataValue(res);
}

Tree* operator ==(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat == secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt == secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat == secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt == secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt == secondOperand.getDataValue().dataAsShortInt;
	}

	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator !=(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat != secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt != secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat != secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt != secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt != secondOperand.getDataValue().dataAsShortInt;
	}
	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator >(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat > secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt > secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat > secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt > secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt > secondOperand.getDataValue().dataAsShortInt;
	}
	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator >=(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat >= secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt >= secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat >= secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt >= secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt >= secondOperand.getDataValue().dataAsShortInt;
	}
	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator <(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat < secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt < secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat < secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt < secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt < secondOperand.getDataValue().dataAsShortInt;
	}
	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}
Tree* operator <=(Tree firstOperand, Tree secondOperand)
{
	bool val;
	if (firstOperand.getDataType() == secondOperand.getDataType())
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat <= secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt <= secondOperand.getDataValue().dataAsShortInt;
	}
	else
	{
		if (firstOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsFloat <= secondOperand.getDataValue().dataAsShortInt;
		else if (secondOperand.getDataType() == TypeData::TypeFloat)
			val = firstOperand.getDataValue().dataAsShortInt <= secondOperand.getDataValue().dataAsFloat;
		else
			val = firstOperand.getDataValue().dataAsShortInt <= secondOperand.getDataValue().dataAsShortInt;
	}
	TDataValue res; res.dataAsShortInt = val;
	Node* result = new Node(TypeData::TypeConstInt);
	result->InitVar();
	result->setDataValue(res, TypeData::TypeConstInt);
	return new Tree(NULL, NULL, NULL, result);
}

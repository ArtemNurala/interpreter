#pragma once
#include "semantic.h"
Tree* operator<<(Tree firstOperand_, Tree secondOperand_);
Tree* operator>>(Tree firstOperand_, Tree secondOperand_);

Tree * retTreeFromFloatDataValue(TDataValue res);
Tree* operator /(Tree firstOperand_, Tree secondOperand_);
Tree* operator *(Tree firstOperand_, Tree secondOperand_);
Tree* operator -(Tree firstOperand_, Tree secondOperand_);
Tree* operator -(Tree firstOperand);
Tree* operator +(Tree firstOperand_, Tree secondOperand_);

Tree* operator ==(Tree firstOperand, Tree secondOperand);
Tree* operator !=(Tree firstOperand, Tree secondOperand);
Tree* operator >(Tree firstOperand, Tree secondOperand);
Tree* operator >=(Tree firstOperand, Tree secondOperand);
Tree* operator <(Tree firstOperand, Tree secondOperand);
Tree* operator <=(Tree firstOperand, Tree secondOperand);

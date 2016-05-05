
#include "Asset.h"

Asset::Asset()
{}

Asset::~Asset()
{
}

void Asset::setColor(int color)
{
	_color = color;
}

int Asset::getColor() const
{
	return _color;
}

int Asset::getColumn() const
{
	return _column;
}

int Asset::getRow() const
{
	return _row;
}

void Asset::setColumn(int column)
{
	_column = column;
}

void Asset::setRow(int row)
{
	_row = row;
}

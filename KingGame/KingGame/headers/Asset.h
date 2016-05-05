#ifndef ASSET_H
#define ASSET_H

#include "Object.h"

class Asset : public Object
{
	int _row;
	int _column;

	int _color;

public:


	Asset();

	~Asset();

	int getColor() const;
	void setColor(int color);

	int getColumn() const;
	void setColumn(int column);

	int getRow() const;
	void setRow(int row);
	
};

#endif

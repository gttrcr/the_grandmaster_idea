#pragma once

class position
{
private:
	unsigned char _col;
	unsigned int _row;

public:
	position()
	{
		_col = 'a';
		_row = 1;
	}

	position(unsigned char from_char)
	{
		_col = from_char % 8 + 'a';
		_row = (from_char / 8) + 1;
	}

	position(unsigned char col, unsigned int row)
	{
		_col = col;
		_row = row;
	}

	unsigned char get_col()
	{
		return _col;
	}

	unsigned int get_row()
	{
		return _row;
	}

	unsigned char to_uchar()
	{
		return (_row - 1) * 8 + (_col - 'a');
	}
};

inline bool operator==(position &p1, position &p2)
{
	return p1.get_col() == p2.get_col() && p1.get_row() == p2.get_row();
}
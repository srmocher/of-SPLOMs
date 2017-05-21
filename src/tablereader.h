#pragma once
#include <vector>
#include <string>
using namespace std;

class Column
{
private:
	string name;
	vector<float> values;
public:
	Column(string n);
	~Column();
	vector<float> get_values();
	string get_name();
	void add_value(float val);
};

class TableReader
{
private:
	vector<Column> columns;	
	int numColumns;
public:
	TableReader();
	~TableReader();
	void read_data(std::string& filename);
	std::vector<float> get_column(int which_column);
	std::string get_column_name(int which_column);
	int get_num_columns();
};




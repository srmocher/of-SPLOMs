#include "tablereader.h"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	std::stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;

}
TableReader::TableReader()
{}

TableReader::~TableReader() 
{
	
}
void TableReader::read_data(string& fileName)
{
	this->columns.clear();
	ifstream file(fileName);
	string str;
	
	//read column names
	getline(file, str);
	vector<string> column_names = split(str, '\t');
	for (int i = 0;i < column_names.size();i++)
	{
		Column column(column_names[i]);
		this->columns.push_back(column);
	}

	//read values
	while (getline(file, str))
	{
		vector<string> values = split(str, '\t');
		for (int i = 0;i < values.size();i++)
		{
			this->columns.at(i).add_value(stof(values[i]));
		}
	}
	
}



Column::Column(string n)
{
	this->name = n;
}

void Column::add_value(float val)
{
	this->values.push_back(val);
}

string Column::get_name() 
{
	return this->name;
}

Column::~Column()
{
	this->values.clear();
}
vector<float> Column::get_values()
{
	return this->values;
}

vector<float> TableReader::get_column(int columnIndex)
{
	Column column = this->columns.at(columnIndex);
	return column.get_values();
}

string TableReader::get_column_name(int columnIndex)
{
	Column column = columns.at(columnIndex);
	return column.get_name();
}

int TableReader::get_num_columns()
{
	int size =  columns.size();
	return size;
}
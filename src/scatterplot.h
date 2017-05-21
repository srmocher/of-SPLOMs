#pragma once

#include<vector>
#include "tablereader.h"
#include "ofEvents.h"
#include "ofColor.h"
using namespace std;

enum Axis{X,Y};

class DataPoint
{
private:
	float x;
	float y;
	
public:
	float x_pos, y_pos;
	DataPoint(float x, float y,float x_pos,float y_pos);
	void displayMessage();
	
};

class Data
{
private:
	vector<int> selectedIndices;
	ofColor color;
public:
	Data();
	Data(vector<int>,ofColor);
	vector<int> getIndices();

};
class Scatterplot
{
private:
	vector<float> x_data;
	vector<float> y_data;
	vector<float> original_x, original_y;
	int x_pos, y_pos;
	int canvasWidth, canvasHeight;
	string x_title, y_title;
	string dataSetName;
	int xOffset, yOffset;
	float x_max_val, y_max_val;
	DataPoint *point;
	float selectionStartX, selectionStartY,selectionEndX,selectionEndY;
	void drawGridLines();
	void drawAxes();
	void drawLabels();
	bool registeredEvents;	
	bool showPoint,selectionDraw,validSelection;
	bool onCanvas(float x,float y);
	bool displayAttributeName,drawXLabels,drawYLabels;
	Data selectedData;
	bool linkPoints;
public:
	Scatterplot();
	~Scatterplot();
	void setup(string dataSetName,vector<float> x, vector<float> y,int canvasWidth, int canvasHeight,int xOffset,int yOffset, int xPos, int yPos,string x_title,string y_title,float x_max,float y_max,bool drawXLabels,bool drawYLabels);
	void draw();	
	
	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs & args);
	void mouseScrolled(ofMouseEventArgs & args);
	void mouseEntered(ofMouseEventArgs & args);
	void mouseExited(ofMouseEventArgs & args);
	static ofEvent<Data> selectionCompleted;
	void setLinked(bool);
	void setLinkedData(Data);
};

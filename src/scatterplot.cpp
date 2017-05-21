#include "scatterplot.h"
#include "ofMain.h"
#include <iomanip>
#include <sstream>
#include "ofEvents.h"

ofEvent<Data> Scatterplot::selectionCompleted = ofEvent<Data>();
Scatterplot::Scatterplot()
{
	registeredEvents = false;
}

Scatterplot::~Scatterplot()
{

}
void Scatterplot::setup(string dataSetName,vector<float> x, vector<float> y,int canvasWidth, int canvasHeight,int xOffset,int yOffset,int xPos, int yPos,string x_title,string y_title,float x_max,float y_max,bool drawXLabels,bool drawYLabels)
{
	this->dataSetName = dataSetName;
	this->x_data = x;
	this->y_data = y;
	this->canvasHeight = canvasHeight;
	this->canvasWidth = canvasWidth;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	this->x_pos = xPos;
	this->y_pos = yPos;
	this->x_title = x_title;
	this->y_title = y_title;
	this->x_max_val = x_max;
	this->y_max_val = y_max;
	this->showPoint = false;
	this->original_x = x_data;
	this->original_y = y_data;
	if (x_title == y_title)
		this->displayAttributeName = true;
	else
		this->displayAttributeName = false;
	this->drawXLabels = drawXLabels;
	this->drawYLabels = drawYLabels;
	this->selectionDraw = false;
	this->linkPoints = false;
	if (!registeredEvents)
	{
		ofRegisterMouseEvents(this);
		
		registeredEvents = true;
	}
}

void Scatterplot::draw()
{
	if (selectionDraw)
	{
		ofSetColor(ofColor(150, 150, 150));
		ofFill();
		//ofNoFill();
		ofDrawRectangle(selectionStartX, selectionStartY, selectionEndX - selectionStartX, selectionEndY - selectionStartY);
		
		//ofFill();
	}
	ofPushMatrix();
	ofTranslate(x_pos, y_pos);
	ofTrueTypeFont font;
	drawGridLines();
	font.loadFont("verdana.ttf",9);
	ofSetColor(ofColor(0, 0, 0));
//	font.drawString(this->dataSetName, this->canvasWidth/2, 20);
	ofSetColor(ofColor(255,255,255,0.7));
	ofDrawRectangle(0, 0, canvasWidth, canvasHeight);
	//ofTranslate(xOffset, yOffset);
	ofSetColor(ofColor(31, 119, 180));
	if (!linkPoints)
	{
		for (auto i = 0;i < x_data.size();i++)
		{
			float x = x_data[i];
			float y = y_data[i];
			ofDrawCircle(x, canvasHeight - y, 3);
			//	cout << x << "," << y << endl;
		}
	}
	else
	{
		ofSetColor(ofColor(255, 0, 0));
		
		bool *points = new bool[x_data.size()];
		for (int i = 0;i < x_data.size();i++)
			points[i] = true;
		vector<int> indices = selectedData.getIndices();
       // cout<<indices.size()<<endl;
		for (int i = 0;i < indices.size();i++)
		{
			int index = indices[i];
			float x = x_data[index];
			float y = y_data[index];
			ofDrawCircle(x, canvasHeight - y, 3);
			points[index] = false;
		}
		ofSetColor(ofColor(238, 238, 238));
		for (int i = 0;i < x_data.size();i++)
		{
			float x = x_data[i];
			float y = y_data[i];
			if (points[i])
				ofDrawCircle(x, canvasHeight - y, 3);
		}
	}
	
	
	//drawLabels();
	ofPopMatrix();
	
	if (displayAttributeName)
	{
		ofSetColor(ofColor(0, 0, 0));
		ofPushMatrix();
		ofTranslate(x_pos,y_pos);
		font.drawString(x_title, 10, 15);
		ofPopMatrix();
	}
	drawAxes();
	

}

void Scatterplot::drawGridLines()
{
	ofTrueTypeFont font;
	font.loadFont("verdana.ttf", 5);
	auto mapped_x_max = std::max_element(x_data.begin(), x_data.end());
	auto mapped_y_max = std::max_element(y_data.begin(), y_data.end());
	int numLines = 4;
	float mapped_x_unit = *mapped_x_max /(float) numLines;
	float mapped_y_unit = *mapped_y_max / (float)numLines;
	float step = (float)(canvasWidth) / (float)numLines;
	float i = mapped_x_unit;
	//ofTranslate(x_pos, y_pos);
	for (auto j = step;j <= canvasWidth;j += step)
	{
		float originalX = ofMap(i, 0, *mapped_x_max, 0, x_max_val);
		ofSetColor(ofColor(238, 238, 238));
		ofDrawLine(j, canvasHeight+20, j, 0);
		ofSetColor(ofColor(0, 0, 0));
		std::ostringstream out;
		out << std::setprecision(3) << originalX;
		if(drawXLabels)
			font.drawString(out.str(), j, canvasHeight+10);
		i += mapped_x_unit;
	}

	i = mapped_y_unit;
	step = (float)(canvasHeight) / (float)numLines;
	for (auto j = step;j <= canvasHeight;j += step)
	{
		float originalY = ofMap(i, 0, *mapped_y_max, 0, y_max_val);
		ofSetColor(ofColor(238, 238, 238));
		
			ofDrawLine(0, canvasHeight - j, canvasWidth+60, canvasHeight-j);
			ofSetColor(ofColor(0, 0, 0));
			std::ostringstream out;
			out << std::setprecision(3) << originalY;
			if(drawYLabels)
				font.drawString(out.str(), -20, canvasHeight-j);
			i += mapped_y_unit;
	}
	//ofPopMatrix();
}

void Scatterplot::drawAxes()
{
	ofPushMatrix();
	ofTranslate(x_pos, y_pos + canvasHeight);
	ofSetColor(ofColor(0, 0, 0));
	ofDrawLine(0, 0, 0, -canvasHeight);
	ofDrawLine(0, 0, canvasWidth, 0);
	ofDrawLine(canvasWidth, 0, canvasWidth, -canvasHeight);
	ofDrawLine(0, -canvasHeight, canvasWidth, -canvasHeight);
	ofPopMatrix();
}

void Scatterplot::drawLabels()
{
	auto x_title = this->x_title;
	auto y_title = this->y_title;
	ofTrueTypeFont font;
	font.loadFont("verdana.ttf", 9);
	
	font.drawString(x_title, canvasWidth / 2, canvasHeight - yOffset + 60);

	auto rect = font.getStringBoundingBox(y_title, -70, canvasHeight / 2,true);
	//ofSetColor(0xcccccc);
	//ofDrawRectangle(rect.x,rect.y,rect.width,rect.height);
}

void Scatterplot::mouseDragged(ofMouseEventArgs &args) {
	float xpos = ofGetMouseX();
	float ypos = ofGetMouseY();
	bool pointOnCanvas = onCanvas(xpos ,ypos);
	if (pointOnCanvas && validSelection)
	{
		selectionDraw = true;
		selectionEndX = xpos;
		selectionEndY = ypos;
		if (selectionStartX < selectionEndX && selectionStartY < selectionEndY)
		{
			selectionDraw = true;
			/*float canvasStartX = selectionStartX - x_pos - xOffset;
			float canvasEndX = selectionEndX - x_pos - xOffset;
			float canvasStartY =  selectionStartY - y_pos - yOffset;
			float canvasEndY = selectionStartY - y_pos - yOffset;*/
			vector<int> selectedIndices;
			for (auto i = 0;i < x_data.size();i++)
			{
				float x = x_data[i];
				float y = y_data[i];
				y = canvasHeight - y + y_pos;
				x = x + x_pos;
				if (x >= selectionStartX && x <= selectionEndX && y >= selectionStartY && y <= selectionEndY)
				{
					selectedIndices.push_back(i);
				}
			}
			//original_x = x_data;
			//original_y = y_data;

			Data data(selectedIndices, ofColor(255, 0, 0, 0));
			ofNotifyEvent(selectionCompleted, data);
		}
		//ofDrawRectangle(selectionStartX, selectionStartY, selectionEndX-selectionStartX, selectionEndY-selectionStartY);
	}

	
}

void Scatterplot::mouseEntered(ofMouseEventArgs & args) {

}

void Scatterplot::mouseExited(ofMouseEventArgs & args) {
	
}

void Scatterplot::mouseMoved(ofMouseEventArgs &args) {
	//int x_position = ofGetMouseX();
	//int y_position = ofGetMouseY();
	//int wHeight = ofGetWindowHeight();
	////cout << x_position << "," << y_position << endl;
	////this->showPoint = false;
	//bool pointOnCanvas = onCanvas(x_position, y_position);
	//if (pointOnCanvas)
	//{
	//	
	//	
	//    
	//
	//}
	
}

void Scatterplot::mousePressed(ofMouseEventArgs & args) {
	
	bool rightPressed = ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT);
	if (rightPressed)
	{
		setLinked(false);
		validSelection = false;
		selectionDraw = false;
		selectionStartX = -1;
		selectionStartY = -1;
		selectionEndY = -1;
		selectionEndX = -1;
		return;
	}
	float posX = ofGetMouseX();
	float posY = ofGetMouseY();
	bool pointOnCanvas = onCanvas(posX,posY);
	if (pointOnCanvas)
	{
		validSelection = true;
		selectionStartX = posX;
		selectionStartY = posY;
	}
}

void Scatterplot::mouseReleased(ofMouseEventArgs & args) {
	bool rightPressed = ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT);
    float xpos = ofGetMouseX();
    float ypos = ofGetMouseY();
    bool pointOnCanvas = onCanvas(xpos ,ypos);
	if (rightPressed)
	{
		setLinked(false);
		validSelection = false;
		selectionDraw = false;
		selectionStartX = 0;
		selectionStartY = 0;
		selectionEndY = 0;
		selectionEndX = 0;
		return;
	}
   
	if (pointOnCanvas && (selectionStartX < selectionEndX) && (selectionStartY < selectionEndY))
	{
		selectionDraw = true;
        selectionEndX = xpos;
        selectionEndY = ypos;
		/*float canvasStartX = selectionStartX - x_pos - xOffset;
		float canvasEndX = selectionEndX - x_pos - xOffset;
		float canvasStartY =  selectionStartY - y_pos - yOffset;
		float canvasEndY = selectionStartY - y_pos - yOffset;*/
		vector<int> selectedIndices;
		for (auto i = 0;i < x_data.size();i++)
		{
            float x = x_data[i];
            float y = y_data[i];
            y = canvasHeight - y + y_pos;
            x = x + x_pos;
            if (x >= selectionStartX && x <= selectionEndX && y >= selectionStartY && y <= selectionEndY)
            {
                selectedIndices.push_back(i);
            }
		}
		//original_x = x_data;
		//original_y = y_data;
		
		Data data(selectedIndices,ofColor(255,0,0,0));
		ofNotifyEvent(selectionCompleted, data);
	}
}

void Scatterplot::mouseScrolled(ofMouseEventArgs & args) {

}

bool Scatterplot::onCanvas(float x, float y)
{
	float xStart = x_pos, xEnd = x_pos + canvasWidth;
	float yStart = y_pos, yEnd = y_pos + canvasHeight;
	if (x >= xStart && x <= xEnd && y >= yStart && y <= yEnd)
	{
		return true;
	}
	return false;
}

void DataPoint::displayMessage()
{
	string message = "(" + to_string(x) + "," + to_string(y) + ")";
	ofTrueTypeFont font;
	font.loadFont("verdana.ttf",10);
	font.drawString(message, x_pos, y_pos);
}

DataPoint::DataPoint(float x, float y, float x_pos, float y_pos)
{
	this->x = x;
	this->y = y;
	this->x_pos = x_pos;
	this->y_pos = y_pos;
}

Data::Data(vector<int> indices,ofColor color)
{
	this->selectedIndices = indices;
	this->color = color;
}



void Scatterplot::setLinked(bool link)
{
	this->linkPoints = link;
}

void Scatterplot::setLinkedData(Data data)
{
	this->selectedData = data;
}

vector<int> Data::getIndices()
{
	return this->selectedIndices;
}

Data::Data(){}

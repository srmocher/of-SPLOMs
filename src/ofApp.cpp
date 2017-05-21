#include "ofApp.h"
#include "tablereader.h"



//--------------------------------------------------------------
void ofApp::setup()
{
    canvasWidth = 800;
	canvasHeight = 600;
	
	readInput();	
	if (this->dataSetName.empty())
	{
        string path = ofToDataPath("iris.tsv");
		reader.read_data(path);
		this->dataSetName = "iris.tsv";
	}
	
	ofSetWindowTitle(this->dataSetName);
	ofColor color(0, 0, 0);
	ofSetBackgroundColor(ofColor(255, 255, 255,1.0));
	//ofToDataPath("bin/data");
	ofAddListener(Scatterplot::selectionCompleted,this, &ofApp::scatterPlotSelectionCompleted);
	
	//changeScatterPlot(0, 1);
	numColumns = reader.get_num_columns();
	plot = new Scatterplot[numColumns*numColumns];
	
	vector<Column> columns;
	for (int i = 0;i < numColumns;i++)
	{
		vector<float> data = reader.get_column(i);
		string column_name = reader.get_column_name(i);
		Column column(column_name);
		for (int i = 0;i < data.size();i++)
			column.add_value(data[i]);
		columns.push_back(column);
	}
	
	int indCanvasWidth = canvasWidth / numColumns;
	int indCanvasHeight = canvasHeight / numColumns;
	float initialXpos = 40;
	float initialYpos = 10;
	for (int i = 0;i <numColumns;i++)
	{
		
		Column y = columns[i];
		vector<float> yvals = y.get_values();
		bool drawYLabels=false, drawXLabels=false;
		auto y_max = std::max_element(yvals.begin(), yvals.end());
		vector<float> mappedYVals = mapData(yvals, Y);

		for (int j = numColumns-1;j >= 0;j--)
		{
			
			Column x = columns[j];
			vector<float> xvals = x.get_values();
			auto x_max = std::max_element(xvals.begin(), xvals.end()); 
			vector<float> mappedXVals = mapData(xvals,X);
			Scatterplot *splot = getPlot(i,j);
			if (j == numColumns - 1)
				drawYLabels = true;
			else
				drawYLabels = false;
			
			if (i==numColumns-1)
				drawXLabels = true;
			splot->setup("iris.tsv", mappedXVals, mappedYVals, indCanvasWidth, indCanvasHeight, 40, 40, initialXpos, initialYpos, x.get_name(), y.get_name(), *x_max, *y_max,drawXLabels,drawYLabels);
			initialXpos = initialXpos + indCanvasWidth +(1.0f/(float)numColumns)*140.0f;
		}
		initialXpos = 40.0f;
		initialYpos = initialYpos + indCanvasHeight+(1.0f/(float)numColumns)*80.0f;
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0;i < numColumns;i++)
	{
		for (int j = numColumns -1;j >= 0;j--)
		{
			Scatterplot *splot = getPlot(i, j);
			splot->draw();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'I' || key == 'i') {
		cout << "Iris data selected."<< endl;
		this->dataSetName = "iris.tsv";
        string path = ofToDataPath("iris.tsv");
		reader.read_data(path);
		setup();
	}
	else if (key == 'M' || key == 'm') {
		cout << "Autompg data selected." << endl;
		this->dataSetName = "auto-mpg.tsv";
        string path = ofToDataPath("auto-mpg.tsv");
		reader.read_data(path);
		setup();
	}
	else if (key == 'A' || key == 'a') {
		this->dataSetName = "abalone.tsv";
		cout << "Abalone data selected." << endl;
        string path = ofToDataPath("abalone.tsv");
		reader.read_data(path);
		setup();
	}
	else if (key == 'W' || key == 'w') {
		this->dataSetName = "winequality-white.tsv";
        string path = ofToDataPath("winequality-white.tsv");
		cout << "Wine quality data selected. Selecting first two columns as default" << endl;
		reader.read_data(path);
		setup();
	}
	/*else if (key == 'C' || key == 'c') {
		cout << "Change columns option selected" << endl;
		int numColumns = this->reader.get_num_columns();
		cout << "Select any 2 of the below columns (as X,Y) by entering the appropriate numbers" << endl;
		for (int i = 0;i < numColumns;i++)
			cout << (i + 1) << " - " << this->reader.get_column_name(i) << endl;
		int i, j;
		cin >> i >> j;
		if (i >= 1 && j >= 1 && i <= numColumns &&j <= numColumns)
		{
			changeScatterPlot(i - 1, j - 1);
		}
		else
		{
			cout << "Invalid input entered. Please retry." << endl;
		}
	}
*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::readInput() {
	cout << "Enter the key corresponding to dataset to view the SPLOM. By default, we show the SPLOM for Iris dataset" << endl;
	cout << "I - Iris" << endl;
	cout << "M - Auto-mpg" << endl;
	cout << "A - Abalone" << endl;
	cout << "W - Wine quality" << endl;
	
	//cout << "Enter C to select the columns/axes for the selected scatter plot" << endl;
	
	
}

void ofApp::changeScatterPlot(int x_colIndex, int y_colIndex) 
{
	vector<float> x_data = reader.get_column(x_colIndex);
	vector<float> y_data = reader.get_column(y_colIndex);
	string x_title = reader.get_column_name(x_colIndex);
	string y_title = reader.get_column_name(y_colIndex);
	auto x_max = std::max_element(x_data.begin(), x_data.end());
	auto y_max = std::max_element(y_data.begin(), y_data.end());
	vector<float> mappedX = mapData(x_data, X);
	vector<float> mappedY = mapData(y_data, Y);
	//plot.setup(this->dataSetName,mappedX, mappedY, canvasWidth, canvasHeight, 50,50, 60, 10, x_title, y_title,*x_max,*y_max);

}

vector<float> ofApp::mapData(vector<float> data,Axis axis)
{
	auto min = std::min_element(data.begin(), data.end());
	auto max = std::max_element(data.begin(), data.end());
	vector<float> output;
	int maxOutput;
	if (axis == X)
		maxOutput = (canvasWidth)/numColumns;
	else
		maxOutput = (canvasHeight)/numColumns;
	for (int i = 0;i < data.size();i++)
	{
		float mappedVal = ofMap(data[i], 0, *max, 0, maxOutput);
		output.push_back(mappedVal);
	}
	return output;
}

void ofApp::scatterPlotSelectionCompleted(Data &selectedData)
{
	
	for (int i = 0;i < numColumns;i++)
	{
		for (int j = 0;j < numColumns;j++)
		{
			Scatterplot *plot = getPlot(i, j);
			plot->setLinked(true);
			plot->setLinkedData(selectedData);
		}
	}
	
}

Scatterplot* ofApp::getPlot(int row, int col)
{
	return plot + (row*numColumns + col);
}

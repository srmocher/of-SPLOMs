#pragma once

#include "ofMain.h"
#include "scatterplot.h"
#include "ofEvents.h"
#include <string>
#include <vector>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void readInput();
		void changeScatterPlot(int x_colIndex,int y_colIndex);
		void scatterPlotSelectionCompleted(Data &selectedData);
		Scatterplot* getPlot(int, int);

	private:
		vector<Scatterplot> plots;
		Scatterplot *plot;
		TableReader reader;
		vector<float> mapData(vector<float>,Axis axis);		
		int canvasWidth, canvasHeight;
		string dataSetName;
		int numColumns;
};

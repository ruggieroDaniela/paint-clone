#pragma once
#include <iostream>

using namespace std;

class CShape{
	protected:
		float color[3];	

	public:
		int x0 = 0, y0 = 0, x1 = 0, y1 =0, clickX = 0, clickY = 0, x2 = 0, y2 = 0, x_ = 0, y_ = 1;
		int left = 0, right = 0, top = 0, bottom = 0, sizeP = 1, type = 0, cooCP = 0;
		bool mode = true, selected = true, stillDrawing = false, boundedB = true, poliLine = true;
		int aux_r[2] = {0, 0};
		bool isLine = false, isCircle = false, isEllipse = false, isTriangle = false, isRectangle = false, isBezier = false, radiusMode = false;
		int left_top[2] = { 0,0 };			// Corners 
		int left_bottom[2] = { 0,0 };
		int right_top[2] = { 0,0 };
		int right_bottom[2] = { 0,0 };
		vector<int> points;
		queue<GLfloat> previus;
		float bezierMax = 0.0, dist = 0.0 ; 
		vector<float> maxi; 
		int numPoints = 0; 
		float px = 0.0, py = 0.0; 

		// Color 
		bool fillColor = false;
		float fill_color[3] = { 0.0f, 0.0f, 0.0f };
		float border_color[3] = { 0.0f, 0.0f, 0.0f }; 

		CShape(float r, float g, float b){
			color[0] = r;
			color[1] = g;
			color[2] = b;
		}

		virtual ~CShape(){	
		
		}

		// Put pixel at (x,y)
		void putPixel(int x, int y, int sizePoint){
			glPointSize(sizePoint); 
			glBegin(GL_POINTS);
				glVertex2i(x, y);
			glEnd();
		}

		// Set color shape
		void setColor(float r, float g, float b){	
			glColor3f(r, g, b);
		}
		
		void horizontalLine(int xMin, int yMin, int xMax) {
			// Draw horizontal line 
			for (int i = xMin; i <= xMax; i++) {
				putPixel(i, yMin, 1);
			}
		}

		// Return true if the coordinates of the mouse's click (x,y) are inside the bounded box of the shape
		bool onBox(int x, int y) {
			int e = 3; // error margin 3px 

			if (x >= left - e && x <= right + e && y >= bottom - e && y <= top + e) {
				return true;
			}

			return false;
		}

		virtual void set(int x0, int y0, int x1, int y1) = 0;	// Set: Point 0 (x0, y0) && Point 1 (x1, y1)
		virtual void render() = 0;								// Draw shape 
		virtual void controlPoints() = 0;						// Draw control points 
		virtual bool onPoint(int x, int y) = 0;					// Check if a control point was clicked
		virtual bool onClick(int x, int y) = 0;					// Check if the shape was clicked on 
};

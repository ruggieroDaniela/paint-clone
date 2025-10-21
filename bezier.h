#pragma once
#include "shape.h"

class Cbezier : public CShape {
	public:
	float xMax = 0, yMax = 0, xMin = 0, yMin = 0;
	float max = 0.0, min = 0.0; 
	shared_ptr<CLine> guide = make_shared <CLine>(0, 0, 0);
	vector<float> maxVector; 

	Cbezier(float r, float g, float b) : CShape(r, g, b) {

	}

	~Cbezier() {
		
	}

	void set(int x0, int y0, int x1, int y1) {	
		int sizeV = points.size(); 

		xMax = points[0]; 
		xMin = points[0]; 
		yMax = points[1]; 
		yMin = points[1]; 

		for (int i = 0; i < sizeV; i+=2) {
			// Find maximun x point 
			if (points[i] > xMax) {
				xMax = points[i]; 
			}

			// Find minimun x point 
			if (points[i] < xMin) {
				xMin = points[i]; 
			}			
		}

		for (int j = 1; j < sizeV; j += 2) {
			// Find maximun y point
			if (points[j] > yMax) {
				yMax = points[j];
			}

			// Find minimun y point 
			if (points[j] < yMin) {
				yMin = points[j];
			}
		}

		this->x0 = xMin;
		this->y0 = yMin;
		this->x1 = xMax;
		this->y1 = yMax;

		left = xMin;
		right = xMax;
		top = yMax;
		bottom = yMin;
	}

	// Display control points 
	void controlPoints() {
		int size = points.size(); 
		setColor(0, 0, 0); 
		
		for (int i = 0; i < size; i+=2) {
			putPixel(points[i], points[i + 1], 6); 
		}
	}

	float tPoint(float t1, float t2, float u) {
		return t1 * (1 - u) + t2 * (u); 
	}

	void setGuideColor(shared_ptr<CLine>line) {
		for (int i = 0; i < 3; i++) {
			line->border_color[i] = border_color[i];
		}
	}

	// Draw beizer points 
	bool curve(int m, float u, bool dot, float x, float y ) {
		float tx = 0.0, ty = 0.0;
		int n = points.size();
		int j = 0;

		while (m > 0) {
			for (int i = m - 1; i > 0; i--) {
				tx = tPoint((float)points[j], (float)points[j + 2], u);
				ty = tPoint((float)points[j + 1], (float)points[j + 3], u);
				points.push_back(tx);
				points.push_back(ty); 
				j += 2;
			}
			
			j = n;
			n = points.size();
			m--;
		}
		if (dot) {			
			guide->poliLine = true;
			setColor(border_color[0], border_color[1], border_color[2]);
			
			if (!previus.empty()) {
				previus.push(tx);
				previus.push(ty);

				px = previus.front();
				previus.pop();
				py = previus.front();
				previus.pop();

				if (mode) {
					// Hardware Mode 
					glBegin(GL_LINE_STRIP);
						glVertex2i(px, py);
						glVertex2i(tx, ty);
					glEnd();
				}
				else {	
					
					// Software mode 
					guide->x0 = px; 
					guide->y0 = py; 
					guide->x1 = tx; 
					guide->y1 = ty; 
					guide->mode = false; 
					setGuideColor(guide); 					
					guide->render();		// Bresenham 						
				}

			}else {
				previus.push(tx);
				previus.push(ty);
			}
			
		}else {
			if((x == floor(tx) || x == ceil(tx)) &&  (y == floor(ty) || y == ceil(ty))){
				// On point  
				return true; 
			}
		}
		return false; 
	}

	void render() {		
		vector<int> auxP = points; 
		int m = (points.size() >> 1);	
		int x1_aux, y1_aux, x2_aux, y2_aux, i = 0;
		int x1_ = 0, x2_ = 0, y1_ = 0, y2_ = 0;
		int sizeV = points.size(); 		
		float dist, a, b; 

		if ((selected  && boundedB)|| stillDrawing) {
			setColor(0, 0, 0); 
			// Draw guide lines 			
			for (int i = 0; i < sizeV - 2; i += 2) {
				x1_aux = points[i];
				y1_aux = points[i + 1];
				x2_aux = points[i + 2];
				y2_aux = points[i + 3];				



				glBegin(GL_LINE_STRIP);
					glVertex2i(x1_aux, y1_aux);
					glVertex2i(x2_aux, y2_aux);
				glEnd();
			}
			
		}
		
		// Determine step
		for (int i = 0; i < points.size()-2; i+=2) {
			x1_ = points[i];
			y1_ = points[i + 1];
			x2_ = points[i + 2];
			y2_ = points[i + 3];

			a = (x2_ - x1_) * (x2_ - x1_); 
			b = (y2_ - y1_) * (y2_ - y1_); 
			dist = sqrt(a + b); 

			if (dist > bezierMax) {
				bezierMax = 1/dist; 
			}
		}
	
		if (points.size() >= 4) { 
			// Draw curve 
			setColor(border_color[0], border_color[1], border_color[2]);
			for (float u = 0.0; u <= 1; u += bezierMax) {
				curve(m, u, true, 0.0, 0.0);
				points = auxP;
			}

			points = auxP;
			previus.pop();
			previus.pop();
		}
			
	}

	// Check if a control point was clicked
	bool onPoint(int x, int y) {
		int size = points.size(); 	
		int xP = 0, yP = 0; 		

		for (int i = 0; i < size; i+=2) {
			xP = points[i]; 
			yP = points[i + 1]; 
			
			// Check if it is a control point 
			if ( ((x == xP) || (x >= xP - 3 && x <= xP + 3)) && ((y == yP) || (y >= yP - 3 && y <= yP + 3)) ) {
				cooCP = i;				
				return true;
			}			
		}

		return false; 
	}

	// Check if the curve was clicked on 
	bool onClick(int x, int y) {
		if (onBox(x, y)) {		// First we check if we are on the bounded box 
			int i = 0;
			vector<int> auxP = points;
			int m = points.size() / 2;

			// Check point  
			for (float u = 0.0; u <= 1; u += bezierMax) {
				if (curve(m, u, false, (float)x, (float)y)) {
					points = auxP;
					return true;
				}
				points = auxP;
			}

		}
		
		return false;
	}

};


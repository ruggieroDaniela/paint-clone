#pragma once
#include "shape.h"

class Crectangle : public CShape{
	public:
		bool dash = false; 

		Crectangle(float r, float g, float b) : CShape(r, g, b){

		}

		~Crectangle(){
			
		}

		void set(int x0, int y0, int x1, int y1){
			this->x0 = x0;	// Point 0 (x0, y0) --> left-top corner 
			this->y0 = y0;
			this->x1 = x1;	// Point 1 (x1, y1) --> right-bottom corner 
			this->y1 = y1;
			
			right = max(x0, x1);
			left = min(x0, x1);
			top = max(y0, y1);
			bottom = min(y0, y1);
			
			// Bounded Box corners  
			this->left_top[x_] = left; 
			this->left_top[y_] = top; 

			this->left_bottom[x_] = left;
			this->left_bottom[y_] = bottom; 

			this->right_bottom[x_] = right;
			this->right_bottom[y_] = bottom;

			this->right_top[x_] = right;
			this->right_top[y_] = top; 
		}

		void render(){			

			if (mode) {	
				// Hardware mode
				
				if (fillColor) {			// Fill rectangle 
					setColor(fill_color[0], fill_color[1], fill_color[2]);
					glBegin(GL_QUADS);
						glVertex2i(x0, y1);
						glVertex2i(x1, y1);
						glVertex2i(x1, y0);
						glVertex2i(x0, y0);
					glEnd();
				}	

				// Borders 
				setColor(border_color[0], border_color[1], border_color[2]);
				glBegin(GL_LINE_STRIP);
					glVertex2i(x0, y1);
					glVertex2i(x1, y1);
					glVertex2i(x1, y0);
					glVertex2i(x0, y0);
					glVertex2i(x0, y1);
				glEnd();
				
			}
			else {
				// Software mode	
				int auxB = bottom + 1;
				int auxT = top - 1;

				// Fill rectangle 
				if (fillColor) {
					setColor(fill_color[0], fill_color[1], fill_color[2]);
					horizontalLine(left, bottom, right);
					horizontalLine(left, top, right);					

					for (int i = auxB; i <= auxT; i++) {
						horizontalLine(left + 1, i, right - 1);
					}
				}

				setColor(border_color[0], border_color[1], border_color[2]);
				int start, stop;
				start = min(x0, x1); 
				stop = max(x0, x1); 

				// Horizontal lines
				for (int x = start; x < stop; x++) {
					putPixel(x, y0, sizeP);
					putPixel(x, y1, sizeP);

					if (dash) {
						x = x + 3;
					}
				}

				start = min(y0, y1); 
				stop = max(y0, y1); 

				// Vertical lines
				for (int y = start; y < stop; y++) {
					putPixel(x0, y, sizeP);
					putPixel(x1, y, sizeP);

					if (dash) {
						y = y + 3;
					}
				}						
			}
		}

		// Draw control points 
		void controlPoints() {
			setColor(0, 0, 0); 
			putPixel(right_bottom[x_], right_bottom[y_], 6);
			putPixel(right_top[x_], right_top[y_], 6); 
			putPixel(left_top[x_], left_top[y_], 6);
			putPixel(left_bottom[x_], left_bottom[y_], 6); 
		}

		//Check if a control point was clicked
		bool onPoint(int x, int y) {
			// Left bottom 
			if (x <= left_bottom[x_] + 3 && x >= left_bottom[x_] - 3 && y <= left_bottom[y_] + 3 && y >= left_bottom[y_] - 3) {
				cooCP = 1;
				return true;
			}

			// Left top 
			if (x <= left_top[x_] + 3 && x >= left_top[x_] - 3 && y <= left_top[y_] + 3 && y >= left_top[y_] - 3) {
				cooCP = 2;
				return true;
			}

			// Right top 
			if (x <= right_top[x_] + 3 && x >= right_top[x_] - 3 && y <= right_top[y_] + 3 && y >= right_top[y_] - 3) { 
				cooCP = 3;
				return true;
			}

			// Right bottom 
			if (x <= right_bottom[x_] + 3 && x >= right_bottom[x_] -3 && y <= right_bottom[y_]+ 3 && y >= right_bottom[y_] - 3) { 
				cooCP = 4; 
				return true; 
			}
			return false; 
		}

		bool onClick(int x, int y){
			//Check if a rectangle was clicked on
			if (x >= left && x <= right && y >= bottom && y <= top) {
				return true; 
			}

			return false;
		}	
};

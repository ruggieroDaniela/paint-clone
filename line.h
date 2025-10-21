#pragma once
#include "shape.h"

class CLine : public CShape{
	public:
		CLine(float r, float g, float b) : CShape(r, g, b){

		}

		~CLine(){
			
		}

		void set(int x0, int y0, int x1, int y1){
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1;

			right = max(x0, x1);
			left = min(x0, x1); 
			top = max(y0, y1); 
			bottom = min(y0, y1); 

			this->right_bottom[x_] = right;
			this->right_bottom[y_] = bottom;
		}

		void bresenham(int x0, int y0, int x1, int y1) {

			setColor(border_color[0], border_color[1], border_color[2]);
			int dx, dy, dx1, dy1, x, y, stop;
			int incE, incNE, d;
			
			dx = (x1 - x0);
			dy = (y1 - y0);

			dx1 = abs(dx);
			dy1 = abs(dy);

			// Case 1: 
			if (dy1 <= dx1) {
				d = (dy1 << 1) - dx1;
				incE = dy1 << 1;
				incNE = (dy1 - dx1) << 1;

				if (dx >= 0) {
					x = x0;
					stop = x1;
					y = y0;
				}
				else {
					x = x1;
					stop = x0;
					y = y1;
				}

				while (x < stop) {
					if (d < 0) {	// East
						d += incE;
					}
					else {			// Northeast 
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
							y++;
						}
						else {
							y--;
						}
						d += incNE;
					}
					x++;
					putPixel(x, y, sizeP);
				}

			}
			else {	// Case 2
				d = (dx1 << 1) - dy1;
				incE = dx1 << 1;
				incNE = (dx1 - dy1) << 1;

				if (dy >= 0) {
					y = y0;
					stop = y1;
					x = x0;
				}
				else {
					y = y1;
					stop = y0;
					x = x1;
				}

				while (y < stop) {
					if (d <= 0) {	// East
						d += incE;
					}
					else {			// Northeast
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
							x++;
						}
						else {
							x--;
						}
						d += incNE;
					}
					y++;
					putPixel(x, y, sizeP);
				}
			}
		}

		void render(){		
			
			if (mode){		// Hardware mode	
				setColor(border_color[0], border_color[1], border_color[2]);
				
				glBegin(GL_LINES);
					glVertex2i(x0, y0);
					glVertex2i(x1, y1);
				glEnd();
			
			}else{			// Software mode 
				bresenham(x0, y0, x1, y1);		
			}	
		}

		// Draw Control Points 
		void controlPoints() {
			setColor(0, 0, 0);
			putPixel(x0, y0, 6);
			putPixel(x1, y1, 6); 
		}

		// Check if a control point was clicked
		bool onPoint(int x, int y) {
			// On control Point (x0, y0)
			if (x >= x0 - 6 && x <= x0 + 6 && y >= y0 - 6 && y <= y0 + 6) {
				cooCP = 0; 
				return true; 
			}
			
			// On contorl Point (x1, y1)
			if (x >= x1 - 6 && x <= x1 + 6 && y >= y1 - 6 && y <= y1 + 6) {
				cooCP = 1; 
				return true;
			}
			
			return false; 
		}

		// Check if a line was clicked on
		bool onClick(int x, int y){			
			int ac, cb, ab, aux; 
			// AC + CB = AB  --> A = (x0, y0); B = (x1, y1); C = (x, y)

			if (onBox(x, y)) {	// First we check if we are on the bounded box 
				ac = (x - x0) * (x - x0) + (y - y0) * (y - y0);
				ac = (int)sqrt(ac);
				cb = (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y);
				cb = (int)sqrt(cb);
				ab = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
				ab = (int)sqrt(ab);
				aux = ac + cb;

				if (aux == ab || (aux <= (ab + 3) && aux >= (ab - 3))) {
					return true;
				}
				return false; 
			}					

			return false;
		}

};

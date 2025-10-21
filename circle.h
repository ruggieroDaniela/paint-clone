#pragma once
#include "shape.h"

class Ccircle : public CShape{
	public:
		Ccircle(float r, float g, float b) : CShape(r, g, b){

		}

		~Ccircle(){

		}

		void set(int x0, int y0, int x1, int y1){
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1;

			this->aux_r[0] = floor(sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));
			this->aux_r[1] = aux_r[0]; 

			left = -aux_r[0] + x0;
			right = aux_r[0] + x0;
			top = aux_r[0] + y0;
			bottom = -aux_r[0] + y0;
			
			// Control Points 
			this->right_bottom[x_] = right;
			this->right_bottom[y_] = y0;

			this->right_top[x_] = x0; 
			this->right_top[y_] = top; 

			this->left_top[x_] = left; 
			this->left_top[y_] = y0; 
			
			this->left_bottom[x_] = x0; 
			this->left_bottom[y_] = bottom; 		
		}


		void draw8points(int x, int y, int xc, int yc) {
			// Fill circle
			if (fillColor) {
				setColor(fill_color[0], fill_color[1], fill_color[2]);
				horizontalLine(-y + 1 + xc, x + yc, y - 1 + xc);
				horizontalLine(-y + 1 + xc, -x + yc, y - 1 + xc);
			}

			// Border 			
			setColor(border_color[0], border_color[1], border_color[2]);
			putPixel(x + xc, y + yc, sizeP);	// (x, y)
			putPixel(y + xc, x + yc, sizeP); 	// (y, x)
			putPixel(y + xc, yc - x, sizeP);	// (y, -x)
			putPixel(x + xc, yc - y, sizeP);	// (x, -y)
			putPixel(xc - x, yc - y, sizeP);	// (-x, -y)
			putPixel(xc - y, yc - x, sizeP);	// (-y, -x)
			putPixel(xc - y, x + yc, sizeP);	// (-y, x)
			putPixel(xc - x, y + yc, sizeP);	// (-x, y)					
		}

		void render(){

			setColor(border_color[0], border_color[1], border_color[2]); 
			int x, y, d; 

			x = 0; 
			y = aux_r[0]; // radius
			d = 1 - aux_r[0]; 

			draw8points(x , y , x0, y0); 

			while (y > x) {
				if (d < 0) {
					d += (x << 1) + 3;
				}else {
					d += ((x - y) << 1) + 5;					
					y--; 
					if (fillColor) {
						setColor(fill_color[0], fill_color[1], fill_color[2]);
						horizontalLine(-x + x0, y + y0, x + x0);
						horizontalLine(-x + x0, -y + y0, x + x0);
					}
				}
				x++; 
				draw8points(x , y , x0, y0);
			}			
		}

		// Draw Control Points 
		void controlPoints() {
			setColor(0, 0, 0);
			putPixel(right_bottom[x_], right_bottom[y_], 6);	
			putPixel(right_top[x_], right_top[y_], 6);
			putPixel(left_top[x_], left_top[y_], 6);
			putPixel(left_bottom[x_], left_bottom[y_], 6);		
		}

		// Check if a control point was clicked
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

			// Right-bottom 
			if (x <= right_bottom[x_] + 3 && x >= right_bottom[x_] - 3 && y <= right_bottom[y_] + 3 && y >= right_bottom[y_] - 3) {
				cooCP = 4;  
				return true;
			}					

			return false;
		}

		// Check if the circle was clicked on
		bool onClick(int x, int y){	
			if (onBox(x, y)) {		// First we check if we are on the bounded box 
				if (((x - this->x0) * (x - this->x0)) + ((y - this->y0) * (y - this->y0)) <= aux_r[0] * aux_r[0]) { 
					return true;
				}
			}			

			return false; 
		}

};

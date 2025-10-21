#pragma once
#include "shape.h"

class Cellipse : public CShape {
	public:
		Cellipse(float r, float g, float b) : CShape(r, g, b) {

		}

		~Cellipse() {
		
		}

		void set(int x0, int y0, int x1, int y1) {
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1;
			
			// Axis 
			this->aux_r[0] = abs(x1 - x0);	// a -> major radius , a^2 = (x1-x0)^2 
			this->aux_r[1] = abs(y1 - y0);	// b -> minor radius , b^2 = (y1-y0)^2

			left = -aux_r[0] + x0,
			right = aux_r[0] + x0, 
			top = aux_r[1] + y0, 
			bottom = -aux_r[1] + y0;
			
			// Bounded Box corners 
			this->left_top[x_] = left; 
			this->left_top[y_] = top;

			this->right_bottom[x_] = right;
			this->right_bottom[y_] = bottom;

			this->left_bottom[x_] = left;
			this->left_bottom[y_] = bottom;

			this->right_top[x_] = right;
			this->right_top[y_] = top;
		}

		void draw4points(int x, int y, int xc, int yc) {			
			// Symmetric points: (x, y), (-x, y), (x, -y), (-x, -y)
			setColor(border_color[0], border_color[1], border_color[2]);						
			putPixel(x + xc, y + yc, sizeP);
			putPixel(-x + xc, y + yc, sizeP);
			putPixel(x + xc, -y + yc, sizeP);
			putPixel(-x + xc, -y + yc, sizeP);			
		}

		void render() {
			setColor(border_color[0], border_color[1], border_color[2]);

			int x, y, d1, d2;
			int a = abs(x1 - x0);	// a -> major radius , a^2 = (x1-x0)^2 
			int b = abs(y1 - y0);	// b -> minor radius , b^2 = (y1-y0)^2

			// Initialization
			x = 0;
			y = b;

			// Auxiliars	
			int a_2 = a * a;
			int b_2 = b * b;
			int b_2x8 = b_2 << 3;					// 8 * b * b 
			int a_2x8 = a_2 << 3;					// 8 * a * a 
			int b_2x4 = b_2 << 2;					// 4 * b * b
			int a_2x4 = a_2 << 2;					// 4 * a * a 
			int dx = (b_2 * (x + 1)) << 3;			// 8 * b * b * x
			int dy = (a_2 * y - 1) << 3;			// 8 * a * a * y
		

			// Decision parameter 1: d = b*(4*b – 4*a*a) + a*a
			d1 = (b_2 << 2) - ((a_2 * b) << 2) + a_2;

			// Draw symmetric points
			draw4points(x, y, x0, y0);

			// Modality 1
			while (dx < dy) {
				if (d1 < 0) {     
					// d =  d + 4*(b*b*(2*x+3))
					dx = dx + b_2x8;
					d1 = d1 + dx + b_2x4;

				}else {			
					// d = d + 4*(b*b*(2*x+3)+a*a*(-2*y+2))
					dx = dx + b_2x8;
					dy = dy - a_2x8;
					d1 = d1 + dx - dy + b_2x4;			

					y--;
					// Fill ellipse 
					if (fillColor) {
						setColor(fill_color[0], fill_color[1], fill_color[2]);
						horizontalLine(-x + x0, y + y0, x + x0);
						horizontalLine(-x + x0, -y + y0, x + x0);
					}	
				}

				x++;
				draw4points(x, y, x0, y0);
			}

			// Modality 2		
			// Decision parameter of region 2		
			int x_4 = x << 2;						// 4x
			d2 = b_2 * (x_4 * x + x_4 + 1) + a_2 * (((y * y) << 2) - (y << 2) + 4) - ((a_2 * b_2) << 2);	// d = b*b*(4*x*x+4*x+1)+a*a*(4*y*y-8*y+4) - 4*a*a*b*b;

			while (y >= 0){			
				// Checking and updating parameter
				if (d2 < 0){
					//d = d + 4 * (b_2 * (2 * x + 2) + a_2* (-2 * y + 3));
					dx = dx + b_2x8;
					dy = dy - a_2x8;
					d2 = d2 + dx - dy + a_2x4;
					x++; 
				}else{
					//d = d + 4 * a_2* (-2 * y + 3);
					dy = dy - a_2x8;
					d2 = d2 + a_2x4 - dy;
				}

				y--; 
				draw4points(x, y, x0, y0);				// draw symmetric points

				if (fillColor) {
					// Fill ellipse 
					setColor(fill_color[0], fill_color[1], fill_color[2]);
					horizontalLine(-x + 1 + x0, y + y0, x - 1 + x0);
					horizontalLine(-x + 1 + x0, -y + y0, x - 1 + x0);
				}
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

			// Right bottom 
			if (x <= right_bottom[x_] + 3 && x >= right_bottom[x_] - 3 && y <= right_bottom[y_] + 3 && y >= right_bottom[y_] - 3) {
				cooCP = 4; 
				return true;
			}

			return false;
		}

		// Check if an ellipse was clicked on
		bool onClick(int x, int y) {	
			if (onBox(x, y)) {		// First we check if we are on the bounded box 
				long long a, b, major, minor, aux1, aux2, major_2, minor_2, mcm, left, right, result;
				long long longx, longy, longx0, longy0;

				longx = (long long)x;
				longy = (long long)y;
				longx0 = (long long)x0;
				longy0 = (long long)y0;

				a = abs((long long)x1 - longx0);	// a -> major radius , a^2 = (x1-x0)^2 
				b = abs((long long)y1 - longy0);	// b -> minor radius , b^2 = (y1-y0)^2

				if (a == 0 || b == 0) {
					return false;
				}

				// Determine major/minor axis 
				major = max(a, b);
				minor = min(a, b);

				aux1 = (longx - longx0) * (longx - longx0);			// (x-x0)^2
				aux2 = (longy - longy0) * (longy - longy0);			// (y-y0)^2

				major_2 = major * major;			// a^2
				minor_2 = minor * minor;			// b^2
				mcm = major_2 * minor_2;			// a^2 * b^2

				// Ellipse equation 
				left = minor_2 * aux1;				// b^2 * (x-x0)^2
				right = major_2 * aux2;				// a^2 * (y-y0)^2
				result = left + right;

				if (left + right <= mcm) {
					return true;
				}

				 
			}			

			return false; 			
		}
};

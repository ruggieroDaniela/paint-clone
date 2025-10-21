#pragma once
#include "shape.h"
#include <algorithm> 


class Ctriangle : public CShape{

	public:
		bool printThis = true;
		shared_ptr<CLine> line1 = make_shared <CLine>(border_color[0], border_color[1], border_color[2]);
		shared_ptr<CLine> line2 = make_shared <CLine>(border_color[0], border_color[1], border_color[2]);
		shared_ptr<CLine> line3 = make_shared <CLine>(border_color[0], border_color[1], border_color[2]);
				
		Ctriangle(float r, float g, float b) : CShape(r, g, b){

		}

		~Ctriangle(){
			//cout << "Se destruyo un triangulo" << endl;
		}
		

		void set(int x0, int y0, int x1, int y1){
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1; 		

			this->x2 = x2; 
			this->y2 = y2; 

			this->line1->set(x0, y0, x1, y1); 
			this->line2->set(x1, y1, x2, y2);
			this->line3->set(x2, y2, x0, y0);

			this->line1->mode = false; 
			this->line2->mode = false; 
			this->line3->mode = false;

			left = min(x0, min(x1, x2));
			right = max(x0, max(x1, x2)); 
			bottom = min(y0, min(y1, y2));
			top = max(y0, max(y1, y2));

			this->left_top[x_] = left;
			this->left_top[y_] = top;
			this->right_bottom[x_] = right;
			this->right_bottom[y_] = bottom;
		}
			

		float slope(float y_bottom, float x_bottom, float y_top, float x_top) {
			float m = 0.0;			

			if (y_top - y_bottom != 0) {				 
				m = (x_top -x_bottom) / (y_top - y_bottom);
			}
			
			return m;
		}

		void setLineColor(shared_ptr<CLine> line) {
			line->mode = false; // Software mode 

			for (int i = 0; i < 3; i++) {
				line->border_color[i] = border_color[i]; 
			}

		}

		void render(){			

			if (mode) {	
				// Hardware mode
				
				if (fillColor) {
					setColor(fill_color[0], fill_color[1], fill_color[2]);
					glBegin(GL_TRIANGLES);
					glVertex2i(x0, y0);
					glVertex2i(x1, y1);
					glVertex2i(x2, y2);
					glEnd();
				}
				
				// Borders 
				setColor(border_color[0], border_color[1], border_color[2]);				
				glBegin(GL_LINE_STRIP); 
					glVertex2i(x0, y0);
					glVertex2i(x1, y1);
					glVertex2i(x2, y2);
					glVertex2i(x0, y0);
				glEnd();
				
			}else {
				// Software mode 				

				if (fillColor) {
					setColor(fill_color[0], fill_color[1], fill_color[2]);

					int y_points[3] = { y0, y1, y2 };
					int x_points[3] = { x0, x1, x2 };
					bool aux[3] = { false, false, false };					
					int posMiddle;
					float x_02, x_01, x_12, m_02, m_01, m_12;
					float x_left, x_right;

					// Sort the points 
					int posBottom = min_element(y_points, y_points + 3) - y_points;
					int posTop = max_element(y_points, y_points + 3) - y_points;

					aux[posBottom] = true;
					aux[posTop] = true;

					for (int i = 0; i < 3; i++) {
						if (aux[i] == false) {
							posMiddle = i;
							break;
						}
					}

					// Slope (x0, y0) - (x2, y2)	
					m_02 = slope((float)y_points[posBottom], (float)x_points[posBottom], (float)y_points[posTop], (float)x_points[posTop]);

					// Slope (x0, y0) - (x1, y1)
					m_01 = slope((float)y_points[posBottom], (float)x_points[posBottom], (float)y_points[posMiddle], (float)x_points[posMiddle]);

					// Slope (x1, y1) - (x2, y2)
					m_12 = slope((float)y_points[posMiddle], (float)x_points[posMiddle], (float)y_points[posTop], (float)x_points[posTop]);

					// Display bottom triangle 
					for (int i = y_points[posBottom]; i < y_points[posMiddle]; i++) {
						x_02 = x_points[posBottom] + (m_02 * (i - y_points[posBottom]));
						x_01 = x_points[posBottom] + (m_01 * (i - y_points[posBottom]));

						x_left = min(x_02, x_01);
						x_right = max(x_02, x_01);

						x_left = ceil(x_left);
						x_right = floor(x_right); 

						for (int j = x_left; j <= x_right; j++) {
							putPixel(j, i, sizeP);
						}
					}

					// Display bottom triangle 
					for (int i = y_points[posMiddle]; i <= y_points[posTop]; i++) {
						x_02 = x_points[posBottom] + (m_02 * (i - y_points[posBottom]));
						x_12 = x_points[posMiddle] + (m_12 * (i - y_points[posMiddle]));

						x_left = min(x_02, x_12);
						x_right = max(x_02, x_12);
						
						x_left = ceil(x_left);
						x_right = floor(x_right);

						for (int j = x_left; j <= x_right; j++) {
							putPixel(j, i, sizeP);
						}

					}

				}	

				// Borders 
				setColor(border_color[0], border_color[1], border_color[2]);
				
				setLineColor(line1); 
				setLineColor(line2); 
				setLineColor(line3); 
				
				this->line1->render();		// Using Bresenham Algorithm
				this->line2->render();
				this->line3->render();
			}
		}

		// Draw control points 
		void controlPoints() {	
			setColor(0, 0, 0); 
			putPixel(x0, y0, 6); 
			putPixel(x1, y1, 6); 
			putPixel(x2, y2, 6); 
		}

		// Check if a control point was clicked
		bool onPoint(int x, int y) {
			if (x >= x0 - 6 && x <= x0 + 6 && y >= y0 - 6 && y <= y0 + 6) {
				cooCP = 0;
				return true;
			}

			if (x >= x1 - 6 && x <= x1 + 6 && y >= y1 - 6 && y <= y1 + 6) {
				cooCP = 1;
				return true;
			}

			if (x >= x2 - 6 && x <= x2 + 6 && y >= y2 - 6 && y <= y2 + 6) {
				cooCP = 2;
				return true;
			}

			return false;
		}

		int  area(int x1, int x2, int x3, int y1, int y2, int y3) {
			return (x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)); 
		}

		// Check if a triangle was clicked on 
		bool onClick(int x, int y){
			// (x0, y0) --> v1
			// (x1, y1) --> v2
			// (x2, y2) --> v3

			if (onBox(x, y)) {	// First we check if we are on the bounded box 
				int  abc = abs(area(x0, x1, x2, y0, y1, y2) << 1);
				int  area_1 = abs(area(x, x0, x1, y, y0, y1) << 1);
				int  area_2 = abs(area(x, x1, x2, y, y1, y2) << 1);
				int area_3 = abs(area(x, x0, x2, y, y0, y2) << 1);

				if (area_1 + area_2 + area_3 == abc) {
					return true;
				}
				
			}
			
			return false;
		}

};

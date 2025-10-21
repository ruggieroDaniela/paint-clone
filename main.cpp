#include <header.h>

//													Project #1
void changeSize(int w, int h) {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);

	if (h == 0)
		return;

	glMatrixMode(GL_PROJECTION);	// Use the Projection Matrix	
	glLoadIdentity();				// Reset Matrix
	glOrtho(0, w, 0, h, -1, 1);
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	
	width = w;
	height = h;
}

// Bring current shape to the front 
void bringToFront() {
	if (!shapes.empty()) {
		temp_shape = onClick_shape;
		shapes.remove(onClick_shape);
		shapes.push_back(temp_shape);
		onClick_shape = temp_shape;
		glutPostRedisplay();
	}
}

// Bring current shape one level to the front 
void levelPlus() {
	list<shared_ptr<CShape>>::iterator ptr2 = shapes.begin();
	shared_ptr<CShape> temp;

	for (auto const& s : shapes) {
		if (s == onClick_shape && ptr2!= prev(shapes.end(), 1)) {
			auto n = next(ptr2, 2);
			temp = onClick_shape;
			shapes.remove(onClick_shape);
			shapes.insert(n, temp);
			glutPostRedisplay();
			break;
		}
		ptr2++;
	}
}

// Send current shape to the back 
void sendToBack() {
	if (!shapes.empty()) {
		temp_shape = onClick_shape;
		shapes.remove(onClick_shape);
		shapes.push_front(temp_shape);
		onClick_shape = temp_shape;
		glutPostRedisplay();
	}
}

// Send current shape one level to the back 
void levelMinus() {
	list<shared_ptr<CShape>>::iterator ptr = shapes.begin();
	shared_ptr<CShape> temp;

	for (auto const& s : shapes) {
		if (s == onClick_shape && ptr != shapes.begin()) {
			auto p = prev(ptr, 1);
			temp = onClick_shape;
			shapes.remove(onClick_shape);
			shapes.insert(p, temp);
			glutPostRedisplay();
			break;
		}
		ptr++;
	}
}

// Bezier Curve Initialization 
void drawBezier() {
	shared_ptr<Cbezier> bezierC = make_shared <Cbezier>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
	bezierC->isBezier = true;
	shapes.push_back(bezierC);
	current_shape = bezierC;
	current_shape->type = 6;
	current_shape->mode = mode; 

	current_shape->border_color[0] = myBorderColor[0];
	current_shape->border_color[1] = myBorderColor[1];
	current_shape->border_color[2] = myBorderColor[2];
}

void drawShape() {
	switch (command) {

	// Draw a line
	case 1: {
		shared_ptr<CLine> line = make_shared <CLine>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
		line->isLine = true; 
		line->poliLine = false; 
		shapes.push_back(line);
		current_shape = line;
		break;
	}

	// Draw a circle
	case 2: {
		shared_ptr<Ccircle> circle = make_shared <Ccircle>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
		circle->radiusMode = true; 
		circle->isCircle = true; 
		shapes.push_back(circle);
		current_shape = circle; 
		break;
	}

	// Draw an ellipse 
	case 3: {
		shared_ptr<Cellipse> ellipse = make_shared <Cellipse>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
		ellipse->isEllipse = true;
		ellipse->radiusMode = true; 
		shapes.push_back(ellipse);
		current_shape = ellipse;
		break;
	}

	// Draw a rectangle
	case 4: {
		shared_ptr<Crectangle> rectA = make_shared <Crectangle>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
		rectA->isRectangle = true;
		shapes.push_back(rectA);
		current_shape = rectA;
		break;
	}

	// Draw a triangle 
	case 5: {
		shared_ptr<Ctriangle> triA = make_shared <Ctriangle>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
		triA->isTriangle = true;
		shapes.push_back(triA);
		current_shape = triA;
		makeTriangle = false; 
		click3 = false;
		waiting = false;
		break; 
	}	
	
	// Draw a Bezier Curve 
	case 6: {
		drawBezier(); 
		nextBezier = false;
		bezierGui = false;
		stop = false;
		current_shape->boundedB = false; 
		break; 
	}

	default:
		break;
	}

	if (command != 0) {
		// Set Border Color 
		current_shape->border_color[0] = myBorderColor[0];
		current_shape->border_color[1] = myBorderColor[1];
		current_shape->border_color[2] = myBorderColor[2]; 
		
		// Set Fill Color 
		current_shape->fillColor = fillColor;
		if (loading) {
			current_shape->fillColor = fillLoad;
		}
		current_shape->fill_color[0] = myFillColor[0];
		current_shape->fill_color[1] = myFillColor[1];
		current_shape->fill_color[2] = myFillColor[2];

		current_shape->mode = mode;
		current_shape->type = command;

		if (loading) {
			current_shape->mode = false; 
		}	 
	}
}

// Determine the primitive to be display 
int determineShape(string primitive) {
	int typeShape = 0;

	if (primitive == "LINE") {
		typeShape = 1;
	}

	if (primitive == "CIRCLE") {
		typeShape = 2;
	}

	if (primitive == "ELLIPSE") {
		typeShape = 3;
	}

	if (primitive == "RECTANGLE") {
		typeShape = 4;
	}

	if (primitive == "TRIANGLE") {
		typeShape = 5;
	}

	if (primitive == "FILLED_CIRCLE") {
		typeShape = 2;
	}

	if (primitive == "FILLED_ELLIPSE") {
		typeShape = 3;
	}

	if (primitive == "FILLED_RECTANGLE") {
		typeShape = 4;
	}

	if (primitive == "FILLED_TRIANGLE") { 
		typeShape = 5;
	}

	return typeShape;
}

// Check if the shape has fill color 
bool hasFill(shared_ptr<CShape> s) {
	if (s->isLine) {
		return false; 
	}

	if (s->isBezier) {
		return false; 
	}

	return true; 
}

// Get file name (Save)
void saveFileName() {
	FILE* lIn;
	char const* lFilterPatterns[2] = { "*.txt", "*.text" };
	doSave = true; 

	// File name
	lTheSaveFileName = tinyfd_saveFileDialog("Save File", "default.txt", 2, lFilterPatterns, NULL);

	if (!lTheSaveFileName){
		tinyfd_messageBox("Error", "Save file name is NULL", "ok", "error", 0);
		doSave = false; 
		return;
	}

#ifdef _WIN32
	if (tinyfd_winUtf8)
		lIn = _wfopen(tinyfd_utf8to16(lTheSaveFileName), L"w"); /* the UTF-8 filename is converted to UTF-16 to open the file*/
	else
#endif
		lIn = fopen(lTheSaveFileName, "w");

	// Error Message 
	if (!lIn){
		tinyfd_messageBox("Error", "Can not open this file in write mode", "ok", "error", 1);
		doSave = false; 
		return ;
	}

	fclose(lIn);
}

// Save scene
void save_file() {
	int shape_type, numPoints = 0;

	if(doSave){
		ofstream writeFile(lTheSaveFileName);

		writeFile << "BACKGROUND" << " " << background[0] << " " << background[1] << " " << background[2] << endl;

		for (auto const& x : shapes) {
			shape_type = x->type;

			if (x->isTriangle) {
				shape_type = 5;
			}

			// Saving Coordinates (x, y)
			if (x->isBezier) {
				numPoints = x->points.size();
				writeFile << primitives[shape_type] << (numPoints >> 1) << " ";
				for (int i = 0; i < numPoints - 1; i += 2) {
					writeFile << x->points[i] << " " << x->points[i + 1] << " ";
				}
			}
			else {	
				if (x->fillColor && hasFill(x)) {
					writeFile << primitives[shape_type + 5];
				}else {
					writeFile << primitives[shape_type];
				}

				writeFile << " " << x->x0 << " " << x->y0 << " " << x->x1 << " " << x->y1 << " ";
			}

			// Saving third vertice of the triangle
			if (shape_type == 5) {
				writeFile << x->x2 << " " << x->y2 << " ";
			}

			// Saving border color 
			writeFile << x->border_color[0] << " " << x->border_color[1] << " " << x->border_color[2];

			// Saving fill Color 
			if (x->fillColor && hasFill(x)) {
				writeFile << " " << x->fill_color[0] << " " << x->fill_color[1] << " " << x->fill_color[2] << "\n";
			}
			else {
				writeFile << endl;
			}
		}
		writeFile.close();
	}
}

// Get file name (load)
void loadFileName() {
	FILE* lIn;
	char const* lFilterPatterns[2] = { "*.txt", "*.text" };
	string line;  
	doLoad = true; 

	// File name
	lTheOpenFileName = tinyfd_openFileDialog("Load File", "", 2, lFilterPatterns, NULL, 0);

	// Error Message
	if (!lTheOpenFileName){
		tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 0);
		//dontLoad = true;
		doLoad = false; 
		return;
	}

#ifdef _WIN32
	if (tinyfd_winUtf8)
		lIn = _wfopen(tinyfd_utf8to16(lTheOpenFileName), L"r"); /* the UTF-8 filename is converted to UTF-16 */
	else
#endif
		lIn = fopen(lTheOpenFileName, "r");

	if (!lIn){
		tinyfd_messageBox("Error", "Can not open this file in read mode", "ok", "error", 1);
		doLoad = false; 
		return;
	}

	fclose(lIn);
}

// Split line 
vector<string> split(string s, string delimiter) {
	int posStart = 0, posEnd, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((posEnd = s.find(delimiter, posStart)) != string::npos) {
		token = s.substr(posStart, posEnd - posStart);
		posStart = posEnd + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(posStart));
	return res;
}

//Unselect box 
void unSelectBox() {
	if (!boxes.empty() && onClick_shape != NULL) {
		boxes.pop_front();
		createBox = false;
		edition = false;
		onClick_shape->selected = false;
		onControlPoint = false;
		draw = true;
	}	
}

// Validation
bool checkCoordinates(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] < 0) {	// Invalid negative coordinate 
			return false; 
		}
	}
	return true; 
}

// Load Scene 
void load_file() {
	string primitive, line;
	int coordinates[6] = { 0 }, typeShape = 0, stop = 4, num, cont = 0, val =0, borderVal = 0, fillVal = 0 ;
	float temp = 0.0, borderTemp = 0.0, fillTemp = 0.0, backgroundTemp[3] = { 0.0 };
	float backVal = 0; 
	vector<string> v;
	vector<int> bezierXY; 
	bool ignoreLine = false; 
	loading = true; 
	
	
	if(doLoad){
		ifstream readFile(lTheOpenFileName);

		while (getline(readFile, line)) {				// Reading line...
			ignoreLine = false;
			cont = 0;
			fillLoad = false;
			fillVal = 0.0;
			borderVal = 0.0;
			vector<string> v = split(line, " ");

			// Get primitive
			if (!v.empty()) {
				primitive = v.front();
				v.erase(v.begin());
			}

			// Processing lines 
			if (primitive == "BACKGROUND") {
				
				for (int i = 0; i < 3; i++) {
					if (!v.empty()) {
						backVal = stof(v.front()); 
						if (backVal >= 0.0 && backVal <= 1.0) {
							backgroundTemp[i] = backVal;
							v.erase(v.begin());
							cont++;
						}
					}
				}

				// Validation 
				if (cont == 3) {
					for (int i = 0; i < 3; i++) {
						background[i] = backgroundTemp[i];
					}
				}
			}
			else {
				typeShape = determineShape(primitive);

				//Bezier 
				num = primitive.back() - '0';
				primitive.pop_back();
				if (primitive == "BEZIER") {
					typeShape = 6;
					stop = num * 2;
				}
				else {
					// Triangle 
					if (typeShape == 5) {
						stop = 6;
					}
					else {
						stop = 4;
					}
				}

				if (typeShape != 0) {					// Valid primitive  		
					// Cordinates (x, y)
					for (int i = 0; i < stop; i++) {
						if (!v.empty()) {
							val = stoi(v.front());
							if (val >= 0.0) {			// Valid coordenate 
								if (typeShape == 6) {
									bezierXY.push_back(val);
								}
								else {
									coordinates[i] = val;
								}
							}
							else {
								ignoreLine = true;
							}
							v.erase(v.begin());
						}
					}

					if (!ignoreLine) {
						// Border color 
						if (!v.empty()) {
							for (int i = 0; i < 3; i++) {
								if (!v.empty()) {
									borderTemp = stof(v.front());
									if (borderTemp >= 0.0 && borderTemp <= 1.0) {
										myBorderColor[i] = borderTemp;
										v.erase(v.begin());
										borderVal++;
									}
								}
							}
						}

						// Fill Color 
						if (!v.empty()) {
							for (int i = 0; i < 3; i++) {
								if (!v.empty()) {
									fillTemp = stof(v.front());
									if (fillTemp >= 0.0 && fillTemp <= 1.0) {
										myFillColor[i] = fillTemp;
										v.erase(v.begin());
										fillVal++;
									}
								}
							}
							fillLoad = true;
						}

						// Display shapes 
						command = typeShape;

						if ((borderVal == 3 && !fillLoad) || (borderVal == 3 && fillVal == 3 && fillLoad)) {

							drawShape();

							// Triangle coordinates
							if (typeShape == 5) {
								current_shape->x2 = coordinates[4];
								current_shape->y2 = coordinates[5];
							}

							if (typeShape == 6) {
								current_shape->points = bezierXY;
								current_shape->numPoints = bezierXY.size();
								current_shape->set(0, 0, 0, 0);
								bezierXY.clear();
							}else {
								current_shape->set(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
							}
						}

					}
				}				
			}
		}

		// Clear
		loading = false;		 
		command = 0; 
		lineGui = false;
		circleGui = false;
		ellipseGui = false;
		rectGui = false;
		bezierGui = false;
		unSelectBox(); 
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW); 
		readFile.close();
	}	
}

void menu() {
	{	// Menu 
		ImGui::SetNextWindowSize({ 400.0f, 220.0f });
		ImGui::Begin("Menu");
		ImGui::Text("Select the shape you want to draw: ");
		ImGui::Spacing; 
		if (ImGui::CollapsingHeader("Shapes")) {
			
			if (ImGui::BeginTable("split", 3)) {
				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Line", &lineGui)) {		// Draw a Line 
					command = 1;
					circleGui = false;
					ellipseGui = false;
					rectGui = false;
					click3 = false;
					bezierGui = false;
					waiting = false;
					stop = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Circle", &circleGui)) {	// Draw a Circle 
					command = 2;
					lineGui = false;
					ellipseGui = false;
					rectGui = false;
					click3 = false;
					bezierGui = false;
					waiting = false;
					stop = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Ellipse", &ellipseGui)) {	// Draw an Ellipse 
					command = 3;
					circleGui = false;
					lineGui = false;
					rectGui = false;
					click3 = false;
					bezierGui = false;
					waiting = false;
					stop = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Rectangle", &rectGui)) {	// Draw a rectangle 
					command = 4;
					circleGui = false;
					ellipseGui = false;
					lineGui = false;
					click3 = false;
					bezierGui = false;
					waiting = false;
					stop = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Triangle", &click3)) {		// Draw a triangle 
					command = 5;
					instructions_triangles = true;
					waiting = true;

					circleGui = false;
					ellipseGui = false;
					rectGui = false;
					lineGui = false;
					bezierGui = false;
					stop = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Bezier Curve", &bezierGui)) {	// Draw a Bezier curve 
					command = 6;
					stop = true;
					nextBezier = true; 
					instructions_bezier = true;
					waiting = false;
					point1 = true; 

					circleGui = false;
					ellipseGui = false;
					rectGui = false;
					click3 = false;
					lineGui = false;
				}

				ImGui::EndTable();
			}
		}

		if (ImGui::CollapsingHeader("Display Mode")) {
			
			if (ImGui::BeginTable("split", 3)) {

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Hardware", &hardwareGui)) {	// Display Mode: Hardware
					mode = true;
					softwareGui = false;
				}

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Software", &softwareGui)) {	// Display Mode: Software 
					mode = false;
					hardwareGui = false;
				}

				ImGui::EndTable();
			}
		}

		if (ImGui::CollapsingHeader("Colors")) {
			
			if (ImGui::BeginTable("split", 3)) {

				ImGui::TableNextColumn();
				if (ImGui::Checkbox("Fill Shape", &flag1)) {			// Fill shape 
					fillColor = flag1; 
					if (!shapes.empty() && onClick_shape->selected) {
						onClick_shape->fillColor = flag1; 
						onClick_shape->fill_color[0] = myFillColor[0];
						onClick_shape->fill_color[1] = myFillColor[1];
						onClick_shape->fill_color[2] = myFillColor[2];
					}
				}

				ImGui::TableNextColumn();
				if (ImGui::ColorEdit3("Fill Color", myFillColor, ImGuiColorEditFlags_NoInputs)) {
					// Change fill color 
					if (!shapes.empty() && onClick_shape->selected) {
						onClick_shape->fill_color[0] = myFillColor[0];
						onClick_shape->fill_color[1] = myFillColor[1];
						onClick_shape->fill_color[2] = myFillColor[2];
					}					
				}
				
				ImGui::TableNextColumn();

				ImGui::TableNextColumn();
				ImGui::ColorEdit3("Background", background, ImGuiColorEditFlags_NoInputs);

				ImGui::TableNextColumn();
				if (ImGui::ColorEdit3("Border Color", myBorderColor, ImGuiColorEditFlags_NoInputs)) {
					// Change border color 
					if (!shapes.empty() && onClick_shape->selected) {
						onClick_shape->border_color[0] = myBorderColor[0];
						onClick_shape->border_color[1] = myBorderColor[1];
						onClick_shape->border_color[2] = myBorderColor[2];
					}		
				}
				
				ImGui::TableNextColumn();		
				ImGui::EndTable();
			}
		}


		if (ImGui::CollapsingHeader("Scene")) {
			
			if (ImGui::BeginTable("split", 3)) {
				ImGui::Indent();
				ImGui::TableNextColumn();
				if (ImGui::Selectable("Bring Forward")) {		// Bring current shape to the front 
					bringToFront();
					bringFront = false;
				}
				ImGui::TableNextColumn();
				if (ImGui::Button("+", ImVec2(20, 20))) {		// Bring current shape one level to the front 
					levelPlus();
					level_plus = true;
				}
				ImGui::TableNextColumn();

				ImGui::TableNextColumn();
				if (ImGui::Selectable("Send Backward")) {		// Send current shape to the back 
					sendToBack();
					sendBack = false;
				}
				ImGui::TableNextColumn();
				if (ImGui::Button("-", ImVec2(20, 20))) {		// Send current shape one level to the back
					levelMinus();
					level_minus = true;
				}
				ImGui::TableNextColumn();
				ImGui::EndTable();
			}
		}

		if (ImGui::CollapsingHeader("File")) {					// Save scene 
			ImGui::Indent();
			if (ImGui::Selectable("SAVE", &saveGui)) {
				saveFileName();
				save_file();				
				loadGui = false;
			}

			if (ImGui::Selectable("LOAD", &loadGui)) {			// Load scene 
				loadFileName();
				load_file(); 
				saveGui = false;
			}
		}

		ImGui::Text(" ");
		if (ImGui::BeginTable("split", 3)) {

			ImGui::TableNextColumn();
			ImGui::Indent(); 
			if (ImGui::Button("ESCAPE", ImVec2(70, 25))) {		// Exit program 
				exit(0);
			}

			ImGui::TableNextColumn();
			if (ImGui::Button("DELETE", ImVec2(70, 25))) {		// Delete current shape 
				if (onClick_shape != NULL && onClick_shape->selected && !boxes.empty()) {
					shapes.remove(onClick_shape);
					boxes.pop_front();
					createBox = false;
					edition = false;
					onControlPoint = false;
					onClick_shape->selected = false;
					draw = true; 
					glutPostRedisplay();
				}

			}

			ImGui::TableNextColumn();
			if (ImGui::Button("CLEAR ALL", ImVec2(70, 25))) {	// Clear screen 
				unSelectBox();
				shapes.clear();
				boxes.clear();	
				background[0] = 1.0; 
				background[1] = 1.0; 
				background[2] = 1.0; 
				
				glutPostRedisplay();
			}

			ImGui::EndTable();
		}			
		
		ImGui::End();
	}


	// Show instructions to draw a triangle
	if (instructions_triangles && click3) {
		ImGui::Begin("Instructions", &instructions_triangles);   
		ImGui::Text("Please, enter 3 clicks");
		ImGui::End();
		
	}

	// Show instructions to draw a Bezier Curve 
	if (instructions_bezier && bezierGui) {
		ImGui::Begin("Instructions", &instructions_bezier);   
		ImGui::Text("Please, enter the points (left-click). To finish, right click with the mouse");
		ImGui::End();
	}
}

void renderScene() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	menu();	// display menu 

	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(background[0], background[1], background[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// Display shapes 
	for (auto const& x : shapes) {		
		x->render();
		if (createBox && onClick_shape->selected) {
			onClick_shape->controlPoints();
		}
	}

	// Display bounded box
	for (auto const& b : boxes)
		b->render();

	glutSwapBuffers();
	glutPostRedisplay();
}

// Handle event: key press 
void processKeys(unsigned char key, int x, int y) {

	if (key >= 0 && key <= 255) {
		// EXIT when the user presses the ESCAPE key.
		if (key == 27)
			exit(0);

		// The user wants to draw a line (PRESSED '1')
		if (key == 49) {
			command = 1;
			lineGui = true; 
			circleGui = false;
			ellipseGui = false;
			rectGui = false;
			click3 = false;
			bezierGui = false;
			waiting = false;
			stop = false;
		}

		// The user wants to draw a circle (PRESSED '2')
		if (key == 50) {
			command = 2;
			circleGui = true; 
			lineGui = false;
			ellipseGui = false;
			rectGui = false;
			click3 = false;
			bezierGui = false;
			waiting = false;
			stop = false;
		}

		// The user wants to draw an ellipse (PRESSED '3')
		if (key == 51) {
			command = 3;
			ellipseGui = true; 
			circleGui = false;
			lineGui = false;
			rectGui = false;
			click3 = false;
			bezierGui = false;
			waiting = false;
			stop = false;
		}

		// The user wants to draw a rectangle (PRESSED '4')
		if (key == 52) {
			command = 4;
			rectGui = true; 
			circleGui = false;
			ellipseGui = false;
			lineGui = false;
			click3 = false;
			bezierGui = false;
			waiting = false;
			stop = false;
		}

		// The user wants to draw a triangle (PRESSED '5')
		if (key == 53) {
			waiting = true;
			command = 5;
			instructions_triangles = true;
			click3 = true; 
			circleGui = false;
			ellipseGui = false;
			rectGui = false;
			lineGui = false;
			bezierGui = false;
			stop = false;
		}

		// The user wants to draw a Bezier Curve (PRESSED '6')
		if (key == 54) {
			stop = true;
			nextBezier = true;
			instructions_bezier = true;
			point1 = true;
			command = 6;

			bezierGui = true; 
			circleGui = false;
			ellipseGui = false;
			rectGui = false;
			click3 = false;
			lineGui = false;
			waiting = false;
		}		

		// Enable/Disable Hardware/Software mode (PRESSED 'h')
		if (key == 104) {
			mode = !mode;
			if (mode) {
				hardwareGui = true; 
				softwareGui = false;
			}
			else {
				softwareGui = true; 
				hardwareGui = false;
			}
		}

		// Enviar al fondo (PRESSED 'b')
		if (key == 98) {
			sendBack = !sendBack;
			sendToBack();
		}

		// Enviar al frente (PRESSED 'f')
		if (key == 102) {
			bringFront = !bringFront;
			bringToFront();
		}

		// Level + (PRESSED '+')
		if (key == 43) {
			levelPlus();
		}			

		// Level - (PRESSED '-')
		if (key == 45) {
			levelMinus(); 
		}			

		// Deselect (PRESSED 'u')
		if (key == 117) {
			unSelectBox(); 
			selected = false; 
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}

		// Delete (PRESSED 'delete')
		if (key == 127) {
			if (onClick_shape != NULL && onClick_shape->selected && !boxes.empty()) {				
				shapes.remove(onClick_shape);
				boxes.pop_front();
				createBox = false;
				edition = false;
				onControlPoint = false;
				draw = true;					
				onClick_shape->selected = false;
				
				glutPostRedisplay();
				glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			}			
		}

		// Save (PRESSED 's')
		if (key == 115) {
			saveFileName();
			save_file();
		}			

		// Load (PRESSED 'L')
		if (key == 76) {
			loadFileName();
			load_file();
		}			

		// Clear (PRESSED 'x')
		if (key == 120) {
			unSelectBox();			
			shapes.clear();
			boxes.clear();
			background[0] = 1.0;
			background[1] = 1.0;
			background[2] = 1.0;
			glutPostRedisplay();
		}
	}
	else {
		ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	}
}

void boundedBox(shared_ptr<CShape> thisShape) {
	int radius[2];

	// Draw Bounded Box 
	if (thisShape->radiusMode) {
		radius[0] = thisShape->aux_r[0];
		radius[1] = thisShape->aux_r[1];
		box->set(-radius[0] + thisShape->x0, radius[1] + thisShape->y0, radius[0] + thisShape->x0, -radius[1] + thisShape->y0);
	}else {
		if (thisShape->isTriangle) {
			box->set(thisShape->left_top[x_], thisShape->left_top[y_], thisShape->right_bottom[x_], thisShape->right_bottom[y_]);
		}else {	
			if (thisShape->isBezier) {
				thisShape->set(0, 0, 0, 0); 
				box->set(thisShape->x0, thisShape->y0, thisShape->x1, thisShape->y1);
			}else {
				box->set(thisShape->x0, thisShape->y0, thisShape->x1, thisShape->y1);
			}
		}
	}
}

// Select shape that was click on 
void selectShape(shared_ptr<CShape> s, int x, int y) {
	onClick_shape = s;
	onClick_shape->clickX = x;				// Click's coordinates 
	onClick_shape->clickY = height - y;
	onClick_shape->selected = true;
	onClick_shape->boundedB = true;
	flag1 = onClick_shape->fillColor; 
}

// When there is a shape select and the user selects another one 
void clickOnAnotherShape() {
	if (!boxes.empty()) {
		boxes.pop_front();
		glutPostRedisplay();		 
		box->mode = false;	// Hardware Mode 			
		box->dash = true;
		boxes.push_back(box);

		boundedBox(onClick_shape);
		onClick_shape->boundedB = true; 
		glutPostRedisplay();
	}
}

// Resize onClick shape 
void reSize(int moveXX, int moveYY) {
	int aux_x0 = onClick_shape->x0;
	int aux_y0 = onClick_shape->y0;
	int aux_x1 = onClick_shape->x1;
	int aux_y1 = onClick_shape->y1;

	// Line
	if (onClick_shape->isLine) {
		if (onClick_shape->cooCP == 1) {	// Left point
			onClick_shape->set(aux_x0, aux_y0, aux_x1 + moveXX, aux_y1 + moveYY);
		}
		else {								// Right point 
			onClick_shape->set(aux_x0 + moveXX, aux_y0 + moveYY, aux_x1, aux_y1);
		}
	}

	// Ellipse
	if (onClick_shape->isEllipse) {
		if (onClick_shape->cooCP == 1) {	// Left-bottom
			onClick_shape->set(aux_x0, aux_y0, onClick_shape->left_bottom[x_] + moveXX, onClick_shape->left_bottom[y_] + moveYY);
		}
		if (onClick_shape->cooCP == 4) {	// Left-top
			onClick_shape->set(aux_x0, aux_y0, onClick_shape->right_bottom[x_] + moveXX, onClick_shape->right_bottom[y_] + moveYY);
		}
		
		if (onClick_shape->cooCP == 3) {	// Right-top
			onClick_shape->set(aux_x0, aux_y0, onClick_shape->right_top[x_] + moveXX, onClick_shape->right_top[y_] + moveYY);
		}

		if (onClick_shape->cooCP == 2) {	// Right-bottom
			onClick_shape->set(aux_x0, aux_y0, onClick_shape->left_top[x_] + moveXX, onClick_shape->left_top[y_] + moveYY);
		}		
	}

	// Circle
	if (onClick_shape->isCircle) {
		if (onClick_shape->cooCP == 1) {	// Left-bottom 
			onClick_shape->set(aux_x0, aux_y0, aux_x0, aux_y0 - onClick_shape->aux_r[0] + moveYY);
		}

		if (onClick_shape->cooCP == 2) {	// Left-top
			onClick_shape->set(aux_x0, aux_y0, aux_x0 - onClick_shape->aux_r[0] + moveXX, aux_y0);
		}

		if (onClick_shape->cooCP == 3) {	// Right-top
			onClick_shape->set(aux_x0, aux_y0, aux_x0, aux_y0 + onClick_shape->aux_r[0] + moveYY);
		}
		if (onClick_shape->cooCP == 4) {	// Right-bottom
			onClick_shape->set(aux_x0, aux_y0, aux_x0 + onClick_shape->aux_r[0] + moveXX, aux_y0);
		}
	}

	// Rectangle 
	if (onClick_shape->isRectangle) {
		if (onClick_shape->cooCP == 1) {	// Left bottom corner 
			onClick_shape->set(onClick_shape->left_top[x_] + moveXX, onClick_shape->left_top[y_], onClick_shape->right_bottom[x_], onClick_shape->right_bottom[y_] + moveYY);
		}

		if (onClick_shape->cooCP == 2 ) {	// Left top corner 
			onClick_shape->set(onClick_shape->left_top[x_] + moveXX, onClick_shape->left_top[y_] + moveYY, onClick_shape->right_bottom[x_], onClick_shape->right_bottom[y_]);
		}

		if (onClick_shape->cooCP == 3) {	// Right top corner 
			onClick_shape->set(onClick_shape->left_top[x_], onClick_shape->left_top[y_] + moveYY, onClick_shape->right_bottom[x_] + moveXX, onClick_shape->right_bottom[y_]);
		}

		if (onClick_shape->cooCP == 4) {	// Right bottom corner
			onClick_shape->set(onClick_shape->left_top[x_], onClick_shape->left_top[y_], onClick_shape->right_bottom[x_] + moveXX, onClick_shape->right_bottom[y_] + moveYY);
		}		
	}

	// Triangle
	if (onClick_shape->isTriangle) {
		if (onClick_shape->cooCP == 0) {	// Vertex 0
			onClick_shape->set(aux_x0 + moveXX, aux_y0 + moveYY, aux_x1, aux_y1);
		}

		if (onClick_shape->cooCP == 1) {	// Vertex 1
			onClick_shape->set(aux_x0, aux_y0, aux_x1 + moveXX, aux_y1 + moveYY);
		}

		if (onClick_shape->cooCP == 2) {	// Vertex 2
			onClick_shape->x2 = onClick_shape->x2 + moveXX;
			onClick_shape->y2 = onClick_shape->y2 + moveYY;

			onClick_shape->left = min(aux_x0, min(aux_x1, onClick_shape->x2));
			onClick_shape->right = max(aux_x0, max(aux_x1, onClick_shape->x2));
			onClick_shape->bottom = min(aux_y0, min(aux_y1, onClick_shape->y2));
			onClick_shape->top = max(aux_y0, max(aux_y1, onClick_shape->y2));

			onClick_shape->left_top[x_] = onClick_shape->left;
			onClick_shape->left_top[y_] = onClick_shape->top;
			onClick_shape->right_bottom[x_] = onClick_shape->right;
			onClick_shape->right_bottom[y_] = onClick_shape->bottom;

			onClick_shape->set(aux_x0, aux_y0, aux_x1, aux_y1);
		}
	}

	// Bezier 
	if (onClick_shape->isBezier) {
		int movePoint = onClick_shape->cooCP; 
		 
		onClick_shape->points[movePoint] = onClick_shape->points[movePoint] + moveXX; 
		onClick_shape->points[movePoint + 1] = onClick_shape->points[movePoint + 1] + moveYY;	
		
	}
}

// Handle event: mouse click
void processMouse(int button, int state, int x, int y) {
	int secondX = 0, secondY = 0;
	bool isSelected = false;

	// When the left button is pressed 
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		firstClick = true;
		firstX = x;
		firstY = height - y;

		// Waiting for three clicks to draw a triangle 
		if (command == 5 && draw && click3) {
			if (triAngle.size() == 4) {
				triAngle.push_back(x);
				triAngle.push_back(height - y);
				makeTriangle = true;

				// Draw a triangle 
				if (draw && !selected && command == 5 && makeTriangle) {
					shared_ptr<Ctriangle> triA = make_shared <Ctriangle>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
					triA->mode = mode;
					triA->x2 = triAngle[4];
					triA->y2 = triAngle[5];
					triA->set(triAngle[0], triAngle[1], triAngle[2], triAngle[3]);
					triA->isTriangle = true;
					shapes.push_back(triA);
					current_shape = triA;

					current_shape->fillColor = fillColor;
					current_shape->fill_color[0] = myFillColor[0];
					current_shape->fill_color[1] = myFillColor[1];
					current_shape->fill_color[2] = myFillColor[2];

					//current_shape->borderColor = borderColor;
					current_shape->border_color[0] = myBorderColor[0];
					current_shape->border_color[1] = myBorderColor[1];
					current_shape->border_color[2] = myBorderColor[2];

					triAngle.clear();
					makeTriangle = false;
					command = 0;			// Disable drawing triangle mode 
					click3 = false; 
					waiting = false;
				}
			}else {
				// Save (x, y)
				triAngle.push_back(x);
				triAngle.push_back(height - y);
			}
		}

		// Bezier Curves 
		if (command == 6 && draw) {

			bPoints.push_back(x);
			bPoints.push_back(height - y);
			
			if (nextBezier) {				// Next point 
				if (point1) {
					drawBezier();
					current_shape->points = bPoints;
					current_shape->numPoints = bPoints.size();
					 
					point1 = false;					 
				}
				else {
					current_shape->points = bPoints;
					current_shape->numPoints = bPoints.size();
				}

				current_shape->stillDrawing = true; 
			}
			else {
				bPoints.clear(); 
			}		
			
		}
		reverseShapes = shapes; 
		reverse(reverseShapes.begin(), reverseShapes.end());

		// Searching for onClick shape 
		for (auto const& s : reverseShapes) {
		
			isSelected = s->onClick(x, height - y);

			// When a shape is selected 
			if (isSelected && !createBox) {
				if (onClick_shape != NULL) {
					onClick_shape->boundedB = false;
				}
				
				selectShape(s, x, y);
				clickOnAnotherShape();
				
				draw = false;		 //Disable 
				selected = true;	// Enable dragging 				
				break;
			}
			else {	// When the box is selected 
				if (createBox && s->onBox(x, height - y)) {	
					if (onClick_shape != NULL) {
						onClick_shape->boundedB = false;
					}
					
					selectShape(s, x, y);
					clickOnAnotherShape();

					draw = false;			//Disable 
					selected = true;		// Enable dragging 			
					onBoundedBox = true;
					onControlPoint = false;
					break;
				}
			}

		}
		
		// Draw bounded box of the current shape
		if (selected && !createBox) {
			boundedBox(onClick_shape);
			box->mode = false;	// Modo Hardware			
			box->dash = true;
			boxes.push_back(box);
			createBox = true;
		}

		// Unselect box
		if (!selected && !boxes.empty() && !onBoundedBox) {
			unSelectBox();
			bringFront = false;
		}

		// Unselect box 
		if (!selected && !boxes.empty() && onBoundedBox) {
			unSelectBox();
			onBoundedBox = false;
			bringFront = false;
		}

		// Edition mode ON 
		if (createBox) {
			edition = true;
		}

		if (edition) {		
			// Bring current shape to the front 
			if (bringFront) {
				bringToFront();
			}

			// Send current shape to the back 
			if (sendBack) {
				sendToBack();
			}

			// Control point was selected 
			if (onClick_shape->onPoint(x, height - y)) {
				onControlPoint = true;

				if (onClick_shape->isLine || onClick_shape->isTriangle) {
					// Click on point (x0, y0)
					if (onClick_shape->cooCP == 0) {
						auxMouseX = onClick_shape->x0;
						auxMouseY = onClick_shape->y0;
					}

					// Click on point (x1, y1)
					if (onClick_shape->cooCP == 1) {
						auxMouseX = onClick_shape->x1;
						auxMouseY = onClick_shape->y1;
					}

					// Click on point (x2, y2)
					if (onClick_shape->isTriangle && onClick_shape->cooCP == 2) {
						auxMouseX = onClick_shape->x2;
						auxMouseY = onClick_shape->y2;
					}

				}else {					
					auxMouseX = x;
					auxMouseY = height - y;
				}

				draw = false;
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);

		// Autoselect shape after drawing 
		if (justDraw) {			 
			box->mode = false;	// Modo Software			
			box->dash = true;
			boxes.push_back(box);
			boundedBox(current_shape);
			onClick_shape = current_shape;
			glutPostRedisplay();
			createBox = true;
			justDraw = false;
		}
		
		// Unselected current shape 
		if (selected && !draw) {
			selected = false;
			draw = true;
		}		
		
		draw = true; // Enable drawing 
	}

	// Draw Bezier Curve 
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && command==6 && draw) {
		nextBezier = false; 
		bezierGui = false;
		stop = false; 
		
		box->mode = false;	// Software Mode 			
		box->dash = true;
		boxes.push_back(box);
		boundedBox(current_shape);
		onClick_shape = current_shape;
		onClick_shape->selected = true; 
		createBox = true;
		onClick_shape->stillDrawing = false;		
		point1 = true; 
		bPoints.clear(); 
	}
}

// Handle event: mouse motion 
void processMotion(int x, int y) {
	int secondX = x;
	int secondY = height - y;
	int moveX, moveY;
 
	// Create shape 
	if (draw && !selected && firstClick && !waiting && command !=6) {
		drawShape();
	}

	// Draw the current shape
	if (command != 0 && draw && !selected && !onControlPoint && !waiting && command != 6) {
		firstClick = false;
		current_shape->set(firstX, firstY, secondX, secondY);
		justDraw = true;
		glutPostRedisplay();
	}

	// Dragg the current shape 
	if (selected && onClick_shape->selected && !draw && !onControlPoint && !waiting && !stop) {		
		// Movement 
		moveX = secondX - (onClick_shape->clickX);
		moveY = secondY - (onClick_shape->clickY);

		// Dragging shape 
		if (onClick_shape->isTriangle) {
			onClick_shape->x2 = onClick_shape->x2 + moveX;
			onClick_shape->y2 = onClick_shape->y2 + moveY;
		}

		if (onClick_shape->isBezier) {
			
			for (int i = 0; i < onClick_shape->points.size(); i+=2) {
				onClick_shape->points[i] = onClick_shape->points[i] + moveX; 
				onClick_shape->points[i + 1] = onClick_shape->points[i + 1] + moveY; 
			}

		}else {
			onClick_shape->set(onClick_shape->x0 + moveX, onClick_shape->y0 + moveY, onClick_shape->x1 + moveX, onClick_shape->y1 + moveY);
		}
		
		// Dragging bounded box 
		if (createBox && !boxes.empty()) {
			boundedBox(onClick_shape);
		}

		glutPostRedisplay();

		// Update mouse coordinates 
		onClick_shape->clickX = secondX;
		onClick_shape->clickY = secondY;
	}

	// On a control point 
	if (onControlPoint && !draw && !waiting && !stop) {
		int moveXX = secondX - auxMouseX;
		int moveYY = secondY - auxMouseY;

		reSize(moveXX, moveYY);
		boundedBox(onClick_shape);
		glutPostRedisplay();

		auxMouseX = secondX;
		auxMouseY = secondY;
	}
}

// Redirect mouse click
void onClick(int button, int state, int x, int y) {
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse) {
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	}
	else {
		processMouse(button, state, x, y);
	}
}

// Redirect mouse motion 
void onMotion(int x1, int y1) {
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse) {
		ImGui_ImplGLUT_MotionFunc(x1, y1);
	}
	else {
		processMotion(x1, y1);
	}
}

void onPassiveMotion(int x1, int y1) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x1, (float)y1);

	// Change mouse cursor 
	if (!shapes.empty()  && onClick_shape!= NULL){
		
		if (onClick_shape->selected && onClick_shape->onBox(x1, height - y1)) {
			 // Drag shape 
			glutSetCursor(GLUT_CURSOR_CYCLE);
			if (onClick_shape->onPoint(x1, height - y1)) {
				glutSetCursor(GLUT_CURSOR_DESTROY);			// Change size 
			}
		}
		else {
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}
	}	
}

int main(int argc, char** argv) {

	// Init GLUT and create Window
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(100, 100);
	width = glutGet(GLUT_SCREEN_WIDTH);
	height = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(width, height);
	glutCreateWindow("Project 1 - Daniela Ruggiero");

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	// Display function
	glutDisplayFunc(renderScene);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();
	
	glutReshapeFunc(changeSize);				// Process resize window 
	glutKeyboardFunc(processKeys);				// Process keybord
	glutMouseFunc(onClick);						// Process Mouse's click
	glutMotionFunc(onMotion);					// Process Mouse's motion 
	glutPassiveMotionFunc(onPassiveMotion);		// Process Mouse's passive motion 
	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

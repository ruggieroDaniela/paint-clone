
#define FREEGLUT_STATIC
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#include "GL/freeglut.h"

#include <tinyfiledialogs.h>
#include <list>
#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <line.h>
#include <triangle.h>
#include <rectangle.h>
#include <circle.h>
#include <ellipse.h>
#include <bezier.h>

#ifdef _MSC_VER
#pragma warning(disable:4996) /* silences warnings about strcpy strcat fopen*/
#endif

//															Proyecto #01

// Global variables 
bool draw = true, selected = false, deleteShape = false, createBox = false, edition = false;
bool justDraw = false, onControlPoint = false, onBoundedBox = false, firstClick = false, waiting = false;
bool fillColor = false, fillLoad = false, loading = false;
bool bringFront = false, sendBack = false, save = false, load = false, level_plus = false, level_minus = false;
int firstX = 0, firstY = 0, auxMouseX = 0, auxMouseY = 0, contClicks = 0, x_ = 0, y_ = 1;
int width = 1300, height = 750, command = 0;
float myBorderColor[3] = { 0.0f, 0.0f, 0.0f };					// Default border color 
float myFillColor[3] = { 1.0f, 1.0f, 1.0f };					// Default fill color 
float background[3] = { 1.0f, 1.0f, 1.0f };						// Default Background color 
bool mode = true;												// Hardware = true; Software = false
char const* lTheSaveFileName;									// Save File Name 
char const* lTheOpenFileName;									// Open File Name  
bool doLoad = true, doSave = true; 

// Data Structures 
list <shared_ptr<CShape>> shapes, boxes, reverseShapes;
shared_ptr<CShape> current_shape = NULL, onClick_shape = NULL, current_box = NULL, temp_shape = NULL;
string primitives[11] = { "null", "LINE", "CIRCLE", "ELLIPSE", "RECTANGLE", "TRIANGLE", "BEZIER", "FILLED_CIRCLE", "FILLED_ELLIPSE", "FILLED_RECTANGLE", "FILLED_TRIANGLE" };
ofstream writeFile, readFile;

// Interface 
static bool show_demo_window = true;
static bool instructions_triangles = false, instructions_bezier = false;
static bool flag1 = false, flag2 = false, click3 = false;
static bool lineGui = false, circleGui = false, ellipseGui = false, rectGui = false, bezierGui = false;
static bool hardwareGui = true, softwareGui = false, saveGui = false, loadGui = false;

// Bezier 
bool nextBezier = true, point1 = false;
bool stop = false;
vector<int> bPoints;

// Triangle 
shared_ptr<Crectangle> box = make_shared <Crectangle>(myBorderColor[0], myBorderColor[1], myBorderColor[2]);
bool makeTriangle = false;
vector<int> triAngle;
Universidad Central de Venezuela 
Introduction to Computer Graphics
Daniela Ruggiero
Project #1

---

## 2D Drawing Application (Paint Clone)

2D drawing application developed in **C++**, a Paint clone project that allows users to create and manipulate basic shapes like **lines, circles, rectangles, triangles, and curves**. It includes functions to **change colors**, **move and resize figures**, and manage their **layer order** (bring to front or send to back).

The program features two rendering modes: 
* **Software** (drawing pixel by pixel using the **Mid-Point Algorithm** to display primitives).
* **Hardware** (accelerated with **OpenGL**).

Scenes can be **saved and loaded** from text files. The user interface is integrated using the **ImGui** library for a modern and responsive control panel with **TinyDialogFile** integrated to handle native OS dialogs for seamless file operations.

! [Demo](/portafolio-paint-project.gif)

---

## Execution Instructions

**Solution:** `Project1.snl`

1.  Open with **Microsoft Visual Studio 2019**.
2.  Compile and run using **Debug x86** / **Release x86**.
3.  **Executable:** `Project1.exe`

---

## SHORTCUTS 

| Key | Function |
| :---- | :------ |
| **1** | Draw a line |
| **2** | Draw a circle |
| **3** | Draw an ellipse |
| **4** | Draw a rectangle |
| **5** | Draw a triangle |
| **6** | Draw a bezier curve |
| **h** | Enable/Disable Hardware/Software Mode (Default mode: Hardware) |
| **b** | Send current shape to the back |
| **f** | Bring current shape to the front |
| **+** | Bring the current shape one level to the front |
| **-** | Send the current shape one level to the back |
| **u** | Unselect current shape |
| **delete** | Delete the current shape |
| **s** | Save file |
| **L** | Load file |
| **x** | Clear screen |
| **esc** | Exit (ESCAPE) |

*In order to change the background color, fill color or border color, please pick a color from the **color picker (interface)**.*

---

## Interface Details

* **To draw lines, circles, rectangles and ellipses** please click and drag the mouse.
* **To draw a triangle** please introduce **3 clicks** (vertices of the triangle), after that the drawing option will be disabled. If you want to draw another triangle, please click on the option again.
* **To draw a bezier curve** please introduce the points (**left click**). **Right click** to finish drawing, after that the drawing option will be disabled. If you want to draw another curve, please click on the option again.
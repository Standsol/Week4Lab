#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "math.h"

const int width = 1024;
const int height = 768;

float* pixels = new float[width*height * 3];
void drawSquare(const int& edge, const int& i0, const int& j0, const float& red, const float& green, const float& blue);
void drawCircle(const int& x_center, const int& y_center, const int& r, const int& red, const int& green, const int& blue);

class Box // NO PARENT
{
public:
	int start_x, start_y;
	int edge;// some variables

	void draw()
	{
		drawSquare(200,100,100,1.0f, 0.0f, 0.0f);// draw box here
	}
};

class Circle // NO PARENT
{
public:
	int x_center, y_center;
	int r;// some variables

	void draw()
	{
		drawCircle(500, 500, 100, 1.0f,0.0f, 0.0f);// draw circle here
	}
};

class GeometricObjectInterface
{
public:
	virtual void draw()
	{
	}
};

// And implement an templatized GeometricObject class.
template<class T_OPERATION>
class GeometricObject : public GeometricObjectInterface
{
public:
	void draw()
	{
		T_OPERATION draw_object;
		draw_object.draw();
	}
};


void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	if (i0 == i1)//그림을 그리는 평면을, 즉 width와 height 을 x좌표와 y좌표로 봤을 때 x좌표가 같을경우
	{
		if (j0 < j1)//아래점에서 위의점으로 올라가는 선을 그릴 때
		{
			for (int i = i0, j = j0; j <= j1; j++)
			{
				drawPixel(i, j, red, green, blue);
			}
		}
		else//위에서 아래로 내려가는 선을 그릴 때
		{
			for (int i = i0, j = j0; j >= j1; j--)
			{
				drawPixel(i, j, red, green, blue);
			}
		}
	}
	else//x좌표가 다를 경우
	{
		if (i0 < i1)//왼쪽에서 오른쪽으로 가는 선을 그릴 때
		{
			for (int i = i0; i <= i1; i++)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				drawPixel(i, j, red, green, blue);
			}
		}
		else//오른쪽에서 왼쪽으로 가는 선을 그릴 때
		{
			for (int i = i0; i >= i1; i--)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				drawPixel(i, j, red, green, blue);
			}
		}
	}
}
//void drawTriangle(const int& i0, const int& j0, const int& i1, const int& j1, const int& i2, const int& j2, const float& red, const float& green, const float& blue)
//{
//	drawLine(i0, j0, i1, j1, red, green, blue);
//	drawLine(i0, j0, i2, j2, red, green, blue);
//	drawLine(i2, j2, i1, j1, red, green, blue);
//}
void drawSquare(const int& edge, const int& i0, const int& j0, const float& red, const float& green, const float& blue)
{
	drawLine(i0, j0, i0 + edge, j0, red, green, blue);
	drawLine(i0 + edge, j0, i0 + edge, j0 + edge, red, green, blue);
	drawLine(i0 + edge, j0 + edge, i0, j0 + edge, red, green, blue);
	drawLine(i0, j0 + edge, i0, j0, red, green, blue);

}
//void drawPentagon(const int& i0, const int& j0, const int& i1, const int& j1, const int& i2, const int& j2, const int& i3, const int& j3, const int& i4, const int&j4, const float& red, const float& green, const float& blue)
//{
//	drawLine(i0, j0, i1, j1, red, green, blue);
//	drawLine(i1, j1, i2, j2, red, green, blue);
//	drawLine(i2, j2, i3, j3, red, green, blue);
//	drawLine(i3, j3, i4, j4, red, green, blue);
//	drawLine(i4, j4, i0, j0, red, green, blue);
//}
//void drawFullSquare(const int& i0, const int& j0, const int& i1, const int& j1, const int& i2, const int& j2, const int& i3, const int& j3, const float& red, const float& green, const float& blue)
//{
//	for (int i = i0; i <= i1; i++)//i0,y0좌표에서부터 i1,j3좌표까지 사각형 안을 계속해서 선을 그어 색을 채우는 식
//	{
//		for (int j = j0; j <= j3; j++)
//		{
//			drawLine(i0, j0, i, j, red, green, blue);
//		}
//	}
//}
void drawCircle(const int& x_center, const int& y_center, const int& r, const int& red, const int& green, const int& blue)
{
	for (int i = 0; i <= (x_center + r); i++)//원의 중심에서 사각형 면까지의 거리는 중심좌표에서 반지름을 더해줘야 함
	{
		for (int j = 0; j <= (y_center + r); j++)
		{
			const double f = (i - x_center) * (i - x_center) + (j - y_center) * (j - y_center) - (r*r);//몬테카를로 때의 원의 방정식을 이용 
			const double f1 = (i - x_center) * (i - x_center) + (j - y_center) * (j - y_center) - ((r - 3)*(r - 3));//만들려고하는 원보다 아주 조금 작은 원을 하나 더 만듬

			if (f <= 0 && f1 > 0)//큰 원의 안쪽과 작은 원의 바깥쪽의 교집합
			{
				drawPixel(i, j, red, green, blue);
			}

		}
	}
}
//void drawThickLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
//{
//	if (i0 == i1)//x좌표가 같을 경우
//	{
//		for (int i = j0; i < j1; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				drawPixel(i1 + j, i, red, green, blue);//처음선의 바로옆으로 계속 옮겨가며 선을 그려 두껍게 표시
//			}
//		}
//	}
//	else if (j0 == j1)//y좌표가 같은경우
//	{
//		for (int i = i0; i < i1; i++)
//		{
//			const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;
//			for (int k = 0; k < 3; k++)//그리려는 선과 그 선의 픽셀 주변을 계속 채워나가 선을 두껍게 표시
//			{
//				drawPixel(i, j, red, green, blue);
//				drawPixel(i, j - k, red, green, blue);
//				drawPixel(i, j + k, red, green, blue);
//			}
//		}
//	}
//	else if (j0 != j1)//y좌표가 다른경우
//	{
//		for (int i = i0; i < i1; i++)
//		{
//			const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;
//			for (int k = 0; k < 3; k++)
//			{
//				drawPixel(i, j, red, green, blue);
//				drawPixel(i, j - k, red, green, blue);
//				drawPixel(i, j + k, red, green, blue);
//			}
//		}
//	}
//}
/*
void drawOnPixelBuffer()
{
//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
std::fill_n(pixels, width*height * 3, 1.0f);	// white background

//for (int i = 0; i<width*height; i++) {
//	pixels[i * 3 + 0] = 1.0f; // red
//	pixels[i * 3 + 1] = 1.0f; // green
//	pixels[i * 3 + 2] = 1.0f; // blue
//}

const int i = rand() % width, j = rand() % height;
drawPixel(i, j, 0.0f, 0.0f, 0.0f);

// drawing a line
//TODO: anti-aliasing
const int i0 = 100, i1 = 200;
const int j0 = 50, j1 = 80;
drawLine(i0, j0, i1, j1, 1.0f, 0.0f, 0.0f);

//TODO: try moving object
}
*/
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		std::fill_n(pixels, width*height * 3, 1.0f);	// white background
		
		std::vector<GeometricObjectInterface*> obj_list;
		obj_list.push_back(new GeometricObject<Circle>);
		obj_list.push_back(new GeometricObject<Box>);
		for (auto itr : obj_list)
			itr->draw();
		

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}



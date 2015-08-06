#include "drawproc.h"

// Reference: http://processingjs.org/learning/basic/arctangent/
//
struct Eye
{
	int ex, ey;
	int size;
	float angle = 0.0;

	Eye() :ex(0), ey(0), size(0), angle(0.0f){}
	Eye(int x, int y, int s) {
		ex = x;
		ey = y;
		size = s;
	}



	void update(int mx, int my) {
		angle = atan2((float)(my - ey), (float)(mx - ex));
	}



	void display() {
		//pushMatrix();
		//translate(ex, ey);
		fill(255);
		//ellipse(0, 0, size, size);
		ellipse(ex, ey, size, size);
		//rotate(angle);
		fill(153);
		//ellipse(size / 4, 0, size / 2, size / 2);
		ellipse(ex+ (size / 4), ey + 0, size / 2, size / 2);
		//popMatrix();
	}
};

Eye e1, e2, e3, e4, e5;



void setup()
{
	size(200, 200);
	smooth();
	noStroke();

	e1 = Eye(50, 16, 80);
	e2 = Eye(64, 85, 40);
	e3 = Eye(90, 200, 120);
	e4 = Eye(150, 44, 40);
	e5 = Eye(175, 120, 80);
}



void draw() 
{
	background(102);

	e1.update(mouseX, mouseY);
	e2.update(mouseX, mouseY);
	e3.update(mouseX, mouseY);
	e4.update(mouseX, mouseY);
	e5.update(mouseX, mouseY);

	e1.display();
	e2.display();
	e3.display();
	e4.display();
	e5.display();
}





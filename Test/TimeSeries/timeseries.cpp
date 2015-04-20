#include "milk-tea-coffee.h"
#include "drawproc.h"
#include <cfloat>

int rowCount = sizeof(data) / sizeof(data[0]);

static const int COLUMN_YEAR = 0;
static const int COLUMN_MILK = 1;
static const int COLUMN_TEA = 2;
static const int COLUMN_COFFEE = 3;

bool isValid(const int row, const int col);
void drawDataPoints(const int col);
LRESULT CALLBACK keyPressed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Data ranges
float dataMin;
float dataMax;

// year ranges
int yearMin;
int yearMax;

// plot area
float plotX1, plotY1;
float plotX2, plotY2;

int currentColumn = COLUMN_MILK;
int columnCount = 3;

const uint8_t * plotFont;


bool isValid(const int row, const int col)
{
	return true;
}

float getColumnMax(int col)
{
	float m = FLT_MIN;
	float value;

	for (int row = 0; row < rowCount; row++) {
		// only consider valid data items
		if (isValid(row, col)) {
			switch (col) {
			case COLUMN_YEAR:
				value = data[row].year;
				break;
			case COLUMN_MILK:
				value = data[row].milk;
				break;
			case COLUMN_TEA:
				value = data[row].tea;
				break;
			case COLUMN_COFFEE:
				value = data[row].coffee;
				break;
			}
			if (value > m) {
				m = value;
			}
		}
	}
	return m;
}

float getDataMax()
{
	float m = FLT_MIN;
	m = max(m, getColumnMax(COLUMN_MILK));
	m = max(m, getColumnMax(COLUMN_TEA));
	m = max(m, getColumnMax(COLUMN_COFFEE));

	return m;
}

float getFloat(const int row, const int col)
{
	float value;

	switch (col) {
	case COLUMN_YEAR:
		value = data[row].year;
		break;
	case COLUMN_MILK:
		value = data[row].milk;
		break;
	case COLUMN_TEA:
		value = data[row].tea;
		break;
	case COLUMN_COFFEE:
		value = data[row].coffee;
		break;
	}
	return value;
}

const char * getColumnName(const int col)
{
	switch (col) {
	case COLUMN_MILK:
		return "Milk";
	case COLUMN_TEA:
		return "Tea";
	case COLUMN_COFFEE:
		return "Coffee";
	}
	return "UNKNOWN";
}

void setup()
{
	setOnKeyTypedHandler(keyPressed);

	size(720, 405);

	//noLoop();
	dataMin = 0;
	dataMax = getDataMax();
	columnCount = 3;

	yearMin = data[0].year;
	yearMax = data[rowCount - 1].year;

	plotX1 = 50;
	plotX2 = width - plotX1;
	plotY1 = 60;
	plotY2 = height - plotY1;

	setFont(verdana18_bold);

	//smooth();
}

void draw()
{
	background(pLightGray);

	// plot area as white box
	fill((uint8_t)255);
	rectMode(CORNERS);
	noStroke();
	rect(plotX1, plotY1, plotX2, plotY2);

	// draw title of current plot
	fill((uint8_t)0);
	//textSize(20);
	const char * title = getColumnName(currentColumn);
	text(title, plotX1, plotY1 - 20);

	// plot the actual columnar data
	stroke(RGBA(0x56, 0x79, 0xc1, 255));
	strokeWeight(5);
	drawDataPoints(currentColumn);
}



void drawDataPoints(const int col)
{
	for (int row = 0; row < rowCount; row++){
		if (isValid(row, col)) {
			float value = getFloat(row, col);
			float x = MAP(data[row].year, yearMin, yearMax, plotX1, plotX2);
			float y = MAP(value, dataMin, dataMax, plotY2, plotY1);
			point(x, y);
		}
	}
}

LRESULT CALLBACK keyPressed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (key)
	{
	case ']':
		currentColumn++;
		if (currentColumn > COLUMN_COFFEE) {
			currentColumn = COLUMN_MILK;
		}
		break;

	case '[':
		currentColumn--;
		if (currentColumn < COLUMN_MILK) {
			currentColumn = COLUMN_COFFEE;
		}
		break;
	}

	return 0;
}

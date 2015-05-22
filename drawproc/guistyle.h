enum FrameStyle {
	Sunken = 0x01,
	Raised = 0x02
};

struct GUIStyle {
	unsigned int baseColor;
	unsigned int highlightColor;
	unsigned int shadowColor;
	unsigned int backgroundColor;
	unsigned int textBackgroundColor;
	unsigned int borderWidth;
	unsigned int padding;

	GUIStyle() :
		borderWidth(2), 
		padding(2){}



	void SetBaseColor(const int color);
	void DrawFrame(int x, int y, int w, int h, int style);
	void DrawSunkenRect(const int x, const int y, const int w, const int h);
	void DrawRaisedRect(const int x, const int y, const int w, const int h);

};

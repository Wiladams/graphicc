
//local Colors = require "Colors"
//local ColorUtils = require "ColorUtils"


enum FrameStyle = {
	Sunken = 0x01,
	Raised = 0x02
};


struct GUIStyle {
	GUIStyle();

	void SetBaseColor(const int color);

	int baseColor;
	int highlightColor;
	int shadowColor;
	int backgroundColor;
	int textBackgroundColor;
};


GUIStyle::GUIStyle()
{

}

GUIStyle::SetBaseColor(const int acolor)	 
{
	self.BaseColor = acolor;
	self.HighlightColor = ColorUtils.brighter(self.BaseColor);
	self.ShadowColor = ColorUtils.darker(self.BaseColor);
	self.Background = ColorUtils.brighter(self.HighlightColor);
	self.TextBackground = self.BaseColor;
}

void GUIStyle : _init()
		self.BorderWidth = 2;
	self.Padding = 2;
}

/*
self:SetBaseColor(Colors.LtGray)

self.Foreground = Colors.LtGray;

self.BottomShadow = ColorUtils.darker(self.Foreground);
self.BottomShadowTopLiner = ColorUtils.brighter(self.BottomShadow);

self.TopShadow = ColorUtils.brighter(self.Foreground);

	 --Calculated
		 self.SunkenColor = self.Foreground;
	 self.RaisedColor = self.Foreground;
}
*/

void GUIStyle::DrawFrame(aPort, x, y, w, h, style)
		 local n = 0;

	 if style == FrameStyle.Sunken then
	 for n = 0, self.BorderWidth - 1 do
 aPort:SetStrokeColor(self.HighlightColor)
 aPort : DrawLine(x + n, y + h - n, x + w - n, y + h - n);    --bottom shadow
	 aPort : DrawLine(x + w - n, y + n, x + w - n, y + h);	    --right shadow
			 end

	   for n = 0, self.BorderWidth - 1 do
   aPort:SetStrokeColor(self.ShadowColor)
   aPort : DrawLine(x + n, y + n, x + w - n, y + n);	    --top edge
	   aPort : DrawLine(x + n, y + n, x + n, y + h - n);	    --left edge
			   end
			   elseif style == FrameStyle.Raised then
		 for n = 0, self.BorderWidth - 1 do
	 aPort:SetStrokeColor(self.ShadowColor)
	 aPort : DrawLine(x + n, y + h - n, x + w - n, y + h - n);      --bottom shadow
		 aPort : DrawLine(x + w - n, y + n, x + w - n, y + h);	    --right shadow
				 end

		   for n = 0, self.BorderWidth - 1 do
	   aPort:SetStrokeColor(self.HighlightColor)
	   aPort : DrawLine(x + n, y + n, x + w - n, y + n);	    --top edge
		   aPort : DrawLine(x + n, y + n, x + n, y + h - n);	    --left edge
		}
	}
}

void GUIStyle::DrawSunkenRect(self, aPort, x, y, w, h)
{			   
	aPort:SetStrokeColor(Colors.Transparent)
	aPort : SetFillColor(self.BaseColor)
	aPort : DrawRect(x, y, w, h);

	self:DrawFrame(aPort, x, y, w, h, Sunken);
}

void GUIStyle::DrawRaisedRect(aPort, const int x, const int y, const int w, const int h)
{
	aPort:SetStrokeColor(Colors.Transparent)
	aPort : SetFillColor(self.BaseColor)
	aPort : DrawRect(x, y, w, h);

	self:DrawFrame(aPort, x, y, w, h, Raised);
}



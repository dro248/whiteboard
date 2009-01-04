
#include "cursor.h"
#include "timer.h"

#include <list>


#define MAXPOINTS 5

namespace Filter
{
	std::list<Point> cache;
	
	void init()
	{
		cache.clear();
	}
	
	
	Point average()
	{
		Point ret;
		
		for (std::list<Point>::const_iterator i = cache.begin(); 
			i != cache.end(); ++i) 
		{
			ret.x += i->x;
			ret.y += i->y;
		}
		
		ret.x /= cache.size();
		ret.y /= cache.size();
		
		return ret;
	}
	
	
	Point process(const Point &p)
	{
		cache.push_back(p);
		if (cache.size() > MAXPOINTS)
			cache.pop_front();
		
		return average();
	}
}




Click::Click(but_t b)
{
	initialTime = Timer::getTicks();
	but = b;
	button(true);
	Filter::init();
}

bool Click::refresh(bool evt)
{
	int t = Timer::getTicks();
	
	if (but == DOUBLE)
		return false;
	
	if (evt)
	{
		initialTime = t;
		return true;
	}
	else
	{
		if ((t - initialTime) > 50)
		{
			button(false);
			return false;
		}
	}
	return true;
}

void Click::button(bool press)
{
	Display* display = XOpenDisplay(0);
	
	switch(but)
	{
		case LEFT:
			XTestFakeButtonEvent(display, 1, (int) press, 0);
			break;
			
		case RIGHT:
			XTestFakeButtonEvent(display, 3, (int) press, 0);
			break;
			
		case DOUBLE:
			XTestFakeButtonEvent(display, 1, 1, 0);
			XTestFakeButtonEvent(display, 1, 0, 40);
			XTestFakeButtonEvent(display, 1, 1, 80);
			XTestFakeButtonEvent(display, 1, 0, 120);
			break;
		
		case NOCLICK:
			break;
	}
	
	XCloseDisplay(display);
}



// FAKECURSOR ///////////////////////////////////////////////////////////////////



FakeCursor::FakeCursor()
{
	state = INACTIVE;
	wii = 0;
	click = 0;
	setClickType(Click::LEFT);
}

void FakeCursor::attachWiimote(Wiimote *wiim)
{
	wii = wiim;
}

Wiimote *FakeCursor::getWii()
{
	return wii;
}

void FakeCursor::activate()
{
	state = ACTIVE;
}

void FakeCursor::deactivate()
{
	state = INACTIVE;
}

void FakeCursor::setClickType(Click::but_t c)
{
	clickType = c;
}

bool FakeCursor::checkLimits(Point p)
{
	if ((p.x < 0) && (p.y < 0))
	{
		setClickType(Click::NOCLICK);
		return false;
	}
	if (p.x < 0)
	{
		setClickType(Click::RIGHT);
		return false;	
	}
	if (p.y < 0)
	{
		setClickType(Click::DOUBLE);
		return false;
	}
	return true;
}

void FakeCursor::update()
{		
	if (!wii) return;
	if (state != ACTIVE) return;
	
	if (wii->dataReady())
	{
		Point p = Filter::process(wii->getPos());
		
		if ((!click) && (checkLimits(p) == false))
			return;
		
		move(p);
		
		if (!click)
			click = new Click(clickType);
		else
			click->refresh(true);
	}
	else
	{
		if ((click) && (!click->refresh(false)))
		{
			clickType = Click::LEFT;
			delete click;
			click = 0;
			Filter::init();
		}
	}
	
}


void FakeCursor::move(Point p)
{
	display = XOpenDisplay(0);
	XTestFakeMotionEvent(display,-1,p.x,p.y,0);
	XCloseDisplay(display);
}





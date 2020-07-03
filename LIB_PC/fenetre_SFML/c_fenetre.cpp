#include "c_fenetre.h"
#if SFLM

c_fenetre fenetre;

c_fenetre::c_fenetre() :
	window(sf::VideoMode(XFENETRE, YFENETRE), TITREFENETRE,/* sf::Style::Titlebar,*/ /*sf::Style::Fullscreen*/ sf::Style::Close),
	XS(XMAX/2),
	YS(YMAX/2),
	REDU(3.1415 +1)//Perfection
	
{
	window.setFramerateLimit(FREQ);//24Tour /sec
}

void c_fenetre::pool_event(void) {

	sf::Event event;
	while (fenetre.get_fenetre().pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			fenetre.get_fenetre().close();
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::BackSpace)
			{
				//RAS
			}
		}
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0)
				MyZoom({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, 1 / ZOOM_SPEED);

			else if (event.mouseWheelScroll.delta < 0)
				MyZoom({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, 1 * ZOOM_SPEED);
		}
	}
}

void c_fenetre::draw_pt(int x, int y, COULEUR c)
{
	static bool init = 1;
	static sf::RectangleShape rect;
	if (init) {
		rect.setSize(sf::Vector2f(2, 2));
		init = 0;
	}
	//c = (rand() % 0xFF)<<16 | (rand() % 0xFF)<<8 | rand() % 0xFF;
	rect.setFillColor(sf::Color(c << 8 | 0xFF));
	rect.setPosition(xys(x, y));
	window.draw(rect);
}

sf::RenderWindow & c_fenetre::get_fenetre()
{
	return window;
}


sf::Vector2f c_fenetre::xys(int x, int y) {
	return sf::Vector2f(X2S(x), YFENETRE - Y2S(y));
}


void c_fenetre::MyZoom(sf::Vector2i pixel, float zoom_value)
{

	//std::cout << "x=" << pixel.x << " y=" << pixel.y << std::endl;

	int X0 = RX2S(pixel.x);
	int Y0 = RY2S(YFENETRE - pixel.y);

	REDU *= zoom_value;

	int X1 = RX2S(pixel.x);
	int Y1 = RY2S(YFENETRE - pixel.y);


	XS += -(X1 - X0);
	YS += -(Y1 - Y0);

	//std::cout << "xS=" << XS << " yS=" << YS << std::endl;
}

void c_fenetre::ForceCamera(sf::Vector2f pos) {
	XS = pos.x;
	YS = pos.y;
}


void c_fenetre::draw_rectangle(sf::Vector2f p1, sf::Vector2f p2, COULEUR c) {
	static sf::RectangleShape myrect;
	myrect.setFillColor(sf::Color(0));
	myrect.setOutlineColor(sf::Color(c << 8 | 0xFF));
	myrect.setOutlineThickness(1);
	myrect.setSize(sf::Vector2f(abs(p1.x - p2.x), abs(p1.y - p2.y)));
	size_t px = (signed int)std::min((signed int)p1.x, (signed int)p2.x) < 0 ? 0 : std::min((signed int)p1.x, (signed int)p2.x);
	size_t py = (signed int)std::min((signed int)p1.y, (signed int)p2.y) < 0 ? 0 : std::min((signed int)p1.y, (signed int)p2.y);
	myrect.setPosition((signed int)std::min((signed int)p1.x, (signed int)p2.x), (signed int)std::min((signed int)p1.y, (signed int)p2.y));
	window.draw(myrect);
}

void c_fenetre::draw_rectanglewREDU(sf::Vector2f p1, sf::Vector2f p2, COULEUR c) {

	sf::Vector2f p11 = xys(p1.x, p1.y);
	sf::Vector2f p22 = xys(p2.x, p2.y);

	static sf::RectangleShape myrect;
	myrect.setFillColor(sf::Color(0));
	myrect.setOutlineColor(sf::Color(c << 8 | 0xFF));
	myrect.setOutlineThickness(1);
	myrect.setSize(sf::Vector2f(abs(p11.x - p22.x), abs(p11.y - p22.y)));
	myrect.setPosition(std::min(p11.x, p22.x), std::min(p11.y, p22.y));
	window.draw(myrect);
	
	//draw_rectangle(xys(p1.x,p1.y), xys(p2.x, p2.y), c);
}

void c_fenetre::draw_circle(sf::Vector2f p, int R, COULEUR c) {
	sf::CircleShape myrect(R);
	myrect.setFillColor(sf::Color(0));
	myrect.setOutlineColor(sf::Color(c << 8 | 0xFF));
	myrect.setOutlineThickness(1);
	myrect.setPosition(p.x - R, p.y - R);
	window.draw(myrect);
}


void c_fenetre::draw_circleREDU(sf::Vector2f p,int R, COULEUR c) {
	p = xys(p.x, p.y);
	R = R / REDU;

	sf::CircleShape myrect(R);
	myrect.setFillColor(sf::Color(0));
	myrect.setOutlineColor(sf::Color(c << 8 | 0xFF));
	myrect.setOutlineThickness(1);
	myrect.setPosition(p.x-R,p.y-R);
	window.draw(myrect);
}

void c_fenetre::draw_terrain(void) {
	///Bordure ext
	draw_rectangle(
		xys(		XMIN,		YMIN),
		xys(		XMAX,		YMAX), 
		blanc);
	///accelerateur
	draw_rectangle(
		xys(		XMAX - 58,	500),
		xys(		XMAX,		2500),
		blanc);
	///Rampes
	draw_rectangle(
		xys(		0,			450),
		xys(		457,		2550),
		blanc);
	///Balances
	draw_rectangle(
		xys(		457 + 200,	YMAX / 2 + 20),
		xys(		0,			YMAX / 2 - 20),
		blanc);
	///Pilone A
	draw_rectangle(
		xys(		AX,			AY),
		xys(		AX + AXC,	AY + AYC),
		blanc);
	//Pylone B
	draw_rectangle(
		xys(		BX,			BY),
		xys(		BX - BXC,	BY + BYC),
		blanc);
}

void c_fenetre::actualiser()
{
	window.display();
	window.clear();

	static sf::Clock Clock;
	sf::Time Time = Clock.getElapsedTime();
	Clock.restart();
	std::cout << "FPS : " << 1000000.f/(float)Time.asMicroseconds() << std::endl;
	
}


c_fenetre::~c_fenetre()
{
}



void c_fenetre::draw_sniper(int x, int y, COULEUR c) {
	draw_rectanglewREDU(sf::Vector2f(x-500, y), sf::Vector2f(x+500, y), c);
	draw_rectanglewREDU(sf::Vector2f(x, y-500), sf::Vector2f(x, y+500), c);
	//draw_circleREDU(sf::Vector2f(x, y), 8, c);
}


COULEUR couleur_style(STYLE style, int x, int xmax)
{
	COULEUR p;
	unsigned char r = 0, g = 0, b = 0;
	switch (style)
	{
	case no:
		r = 0;
		g = 0;
		b = 0;
		break;
	case lunary://Lunary
		r = 255 - (255)* x / xmax;
		g = (255) * x / xmax;
		b = -10;
		break;
	case solary://Solary
		r = -10;
		g = 255 - (255) * x / xmax;
		b = (255)* x / xmax;
		break;
	default:
		break;
	}
	p = r << 16 | g << 8 | b;
	return p;
}

#endif
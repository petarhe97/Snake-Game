/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <sstream>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS = 60;
int inputSpeed = 5;
bool gameStart = false;
bool gamePause = false;
bool gameRestart = false;
const int width = 800;
const int height = 600;
const int BLOCK = 15;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	Pixmap	pixmap;
	Pixmap  bitmap;
	Pixmap snakeBody;
	Pixmap fruitPic;
	Pixmap background;
	int		width;		// size of window
	int		height;
};

class Coord {
	int x;
	int y;
	int direction;
	public:
		Coord(int x, int y, int direction) : x(x) , y(y), direction(direction){}
		int getX(){return x;}
		int getY(){return y;}
		int getDirection(){return direction;}
		void setX(int newX){x = newX;}
		void setY(int newY){y = newY;}
		void setD(int newD){direction = newD;}
};

/*
	1
4		2   <- directions
	3
*/


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};       

class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
				for(int i = 0; i < length; i ++){
					XCopyPlane(xinfo.display, xinfo.snakeBody, xinfo.pixmap, xinfo.gc[1],
					0, 0,
					blockSize, blockSize,
					body[i]->getX(), body[i]->getY(),
					1);
				}
		}
		
		void move(XInfo &xinfo) {
			for(int i = 0; i < length; i++){
				int d = body[i]->getDirection();
				int x = body[i]->getX();
				int y = body[i]->getY();
				if(d == 1){
					body[i]->setY(y - speed*blockSize);
				}else if(d == 2){
					body[i]->setX(x + speed*blockSize);
				}else if(d == 3){
					body[i]->setY(y + speed*blockSize);
				}else{
					body[i]->setX(x - speed*blockSize);
				}
			}
			for(int i = length-1; i > 0; i--){
				body[i]->setD(body[i-1]->getDirection());
			}
			

            // ** ADD YOUR LOGIC **
            // Here, you will be performing collision detection between the snake, 
            // the fruit, and the obstacles depending on what the snake lands on.
		}

		int getXdirection(){
			int d = body[0]->getDirection();
			if(d == 2){
				return 1;
			}else if(d == 4){
				return -1;
			}else{
				return 0;
			}
		}

		int getYdirection(){
			int d = body[0]->getDirection();
			if(d == 3){
				return 1;
			}else if(d == 1){
				return -1;
			}else{
				return 0;
			}
		}

		int getHeadX(){
			return body[0]->getX();
		}

		int getHeadY(){
			return body[0]->getY();
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */ 
        void didEatFruit() {
			int endX = body[length-1]->getX();
			int endY = body[length-1]->getY();
			int endD = body[length-1]->getDirection();
			Coord *newPart = nullptr;
			if(endD == 1){
				newPart = new Coord(endX,endY+blockSize,endD);
			}else if(endD == 2){
				newPart = new Coord(endX-blockSize,endY,endD);
			}else if(endD == 3){
				newPart = new Coord(endX,endY-blockSize,endD);
			}else{
				newPart = new Coord(endX+blockSize,endY,endD);
			}
			body.push_back(newPart);
			++length;
        }

        bool checkHitObstacle() {
			if(getHeadX() < BLOCK*3 || (getHeadX() + blockSize) > (800 - BLOCK*3) || getHeadY() < BLOCK*3 || (getHeadY() + blockSize) > (600 - BLOCK*3)){
				return true;
			}

			for(int i = 1; i < length; i++){
				if(getHeadX() == body[i]->getX() && getHeadY() == body[i]->getY()){
					return true;
				}
			}
			return false;
        }

		bool checkOverlap(int x, int y){
			for(int i = 0; i < length; i++){
				if(x == body[i]->getX() && y == body[i]->getY()){
					return true;
				}
			}
			return false;
		}

		int getScore(){
			return length - SIZE;
		}

        void turnLeft() {
			int curdirection = body[0]->getDirection();
			if(curdirection == 1){
				body[0]->setD(4);
				turn = true;
			}else if(curdirection == 2){
				body[0]->setD(1);
				turn = true;
			}else if(curdirection == 3){
				body[0]->setD(2);
				turn = true;
			}else if(curdirection == 4){
				body[0]->setD(3);
				turn = true;
			}
        }

        void turnRight() {
			int curdirection = body[0]->getDirection();
			if(curdirection == 1){
				body[0]->setD(2);
				turn = true;
			}else if(curdirection == 2){
				body[0]->setD(3);
				turn = true;
			}else if(curdirection == 3){
				body[0]->setD(4);
				turn = true;
			}else if(curdirection == 4){
				body[0]->setD(1);
				turn = true;
			}
        }

		Snake(int x, int y){
			length = SIZE;
			blockSize = BLOCK;
			x = x + (length - 1)*blockSize;
			Coord *newBlock = nullptr;
			for(int i = 0 ;  i < length; i++){
				newBlock = new Coord(x - i*blockSize, y, 2);
				body.push_back(newBlock);
			}
			speed = 1;
			turn = false;
		}

		void resetSnake(int x, int y){
			for(int i = 0; i < length; i++){
				delete body[i];
			}
			body.clear();
			length = SIZE;
			blockSize = BLOCK;
			x = x + (length - 1)*blockSize;
			Coord *newBlock = nullptr;
			for(int i = 0 ;  i < length; i++){
				newBlock = new Coord(x - i*blockSize, y, 2);
				body.push_back(newBlock);
			}
			speed = 1;
			turn = false;
		}

		~Snake(){
			for(int i = 0; i < length; i++){
				delete body[i];
			}
		}
		
	
	private:
		vector<Coord*> body;
		bool turn;
		int blockSize;
		int length;
		int speed;

		const int SIZE = 5;
};

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XCopyPlane(xinfo.display, xinfo.fruitPic, xinfo.pixmap, xinfo.gc[2],
					0, 0,
					BLOCK, BLOCK,
					x, y,
					1);
        }

        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit 
            x = 90;
            y = 90;
        }

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */

		 void changeLocation(){
			 srand(time(0));
			 x = rand() % ((800-BLOCK*8)/BLOCK) + 4;
			 y = rand() % ((600-BLOCK*8)/BLOCK) + 4;
			 x = x * BLOCK;
			 y = y * BLOCK;
		 }

		 void resetFruit(){
			 x = 90;
			 y = 90;
		 }

		 int getX(){
			 return x;
		 }

		 int getY(){
			 return y;
		 }

    private:
        int x;
        int y;
};


list<Displayable *> dList;           // list of Displayables
Snake snake(150, 450);
Fruit fruit;


/*
 * Initialize X and create a window
 */

void initX(int argc, char *argv[], XInfo &xinfo) {
	XSizeHints hints;
	unsigned long white, black;

	if(argc > 1){
		string inputFPS = string(argv[1]);
		string givenSpeed = string(argv[2]);
		FPS = stoi(inputFPS);
		inputSpeed = stoi(givenSpeed);
	}

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xinfo.display = XOpenDisplay( "" );
	if ( !xinfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xinfo.screen = DefaultScreen( xinfo.display );

	white = XWhitePixel( xinfo.display, xinfo.screen );
	black = XBlackPixel( xinfo.display, xinfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xinfo.window = XCreateSimpleWindow( 
		xinfo.display,				// display where window appears
		DefaultRootWindow( xinfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xinfo.display,		// display containing the window
		xinfo.window,		// window whose properties are set
		"x3_doublebuffer",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 2;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], 0x00FF00);
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, hints.width, hints.height, depth);
	xinfo.width = hints.width;
	xinfo.height = hints.height;

		unsigned int bitmap_width = 800;
	unsigned int bitmap_height = 600;
	int rc = XReadBitmapFile(xinfo.display, xinfo.window,
                         "splashScreen.xbm",
                         &bitmap_width, &bitmap_height,
                         &xinfo.bitmap,
                         0, 0);
	int bp = XReadBitmapFile(xinfo.display, xinfo.window, "background.xbm", &bitmap_width, &bitmap_height, &xinfo.background, 0, 0);
	bitmap_width = 15;
	bitmap_height = 15;
	int sb = XReadBitmapFile(xinfo.display, xinfo.window, "snakeBody.xbm", &bitmap_width, &bitmap_height, &xinfo.snakeBody, 0, 0);
	int fp = XReadBitmapFile(xinfo.display, xinfo.window, "fruit.xbm", &bitmap_width, &bitmap_height, &xinfo.fruitPic, 0, 0);
/* check for failure or success. */
switch (rc) {
    case BitmapOpenFailed:
        fprintf(stderr, "XReadBitmapFile - could not open file 'icon.bmp'.\n");
        break;
    case BitmapFileInvalid:
        fprintf(stderr,
                "XReadBitmapFile - file '%s' doesn't contain a valid bitmap.\n",
                "icon.bmp");
        break;
    case BitmapNoMemory:
        fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
        break;
    case BitmapSuccess:
        /* bitmap loaded successfully - do something with it... */
      
        break;
}

	XFontStruct * font;
	font = XLoadQueryFont (xinfo.display, "12x24");
	XSetFont (xinfo.display, xinfo.gc[0], font->fid);
	XSetFont (xinfo.display, xinfo.gc[1], font->fid);
	
	XSelectInput(xinfo.display, xinfo.window, 
		ButtonPressMask | KeyPressMask | PointerMotionMask 
		| EnterWindowMask | LeaveWindowMask
		| StructureNotifyMask   // for resize events
		);

	/*
	 * Don't paint the background -- reduce flickering
	 */
	XSetWindowBackgroundPixmap(xinfo.display, xinfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
	
	XFlush(xinfo.display);
}

/*
 * Function to repaint a display list
 */


void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	// draw into the buffer
	// note that a window and a pixmap are “drawables”
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 
		0, 0, xinfo.width, xinfo.height); 
	if(gameStart){
		XCopyPlane(xinfo.display, xinfo.background, xinfo.pixmap, xinfo.gc[2],
					45, 45,
					710, 510,
					45, 45,
					1);
		//XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 45, 45, 710, 510);
		stringstream ss;
		ss << "Score: ";
		ss << snake.getScore();
		string text = ss.str();
		XDrawImageString( xinfo.display, xinfo.pixmap, xinfo.gc[0],
					10, 30, text.c_str(), text.length());
		while( begin != end ) {
			Displayable *d = *begin;
			d->paint(xinfo); // the displayables know about the pixmap
			begin++;
		}
		// copy buffer to window
	
	}else{
		XCopyPlane(xinfo.display, xinfo.bitmap, xinfo.pixmap, xinfo.gc[1],
          0, 0,
          xinfo.width, xinfo.height,
          0, 0,
          1);
	}
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window 


	XFlush( xinfo.display );

}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1 && gameStart && !gamePause) {
		//printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			cout << "Your final score was " << snake.getScore() << endl;
			error("Terminating normally.");
		}else if(text[0] == 'w'){
			if(snake.getYdirection() == 0){
				if(snake.getXdirection() == 1){
					snake.turnLeft();
				}else{
					snake.turnRight();
				}
			}
		}else if(text[0] == 'a'){
			if(snake.getXdirection() == 0){
				if(snake.getYdirection() == 1){
					snake.turnRight();
				}else{
					snake.turnLeft();
				}
			}
		}else if(text[0] == 's'){
			if(snake.getYdirection() == 0){
				if(snake.getXdirection() == 1){
					snake.turnRight();
				}else{
					snake.turnLeft();
				}
			}
		}else if(text[0] == 'd'){
			if(snake.getXdirection() == 0){
				if(snake.getYdirection() == 1){
					snake.turnLeft();
				}else{
					snake.turnRight();
				}
			}
		}else if(text[0] == 'p'){
			gamePause = true;
		}else if(text[0] == 'r'){
			gameRestart = true;
		}
	}else if(i == 1 && !gamePause){
		//printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}else if(text[0] == 's'){
			gameStart = true;
		} 
	}else if(i == 1 && gamePause){
		if (text[0] == 'q') {
			cout << "Your final score was " << snake.getScore() << endl;
			error("Terminating normally.");
		}else if(text[0] == 'p'){
			gamePause = false;
		}else if(text[0] == 'r'){
			gameRestart = true;
		}
	}
}

void handleAnimation(XInfo &xinfo, int inside) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */ 
	 if(snake.checkHitObstacle()){
		 cout << "Your final score is " << snake.getScore() << endl;
		 error("Game Over");
	 }
	 if(snake.getHeadX() == fruit.getX() && snake.getHeadY() == fruit.getY()){
		 fruit.changeLocation();
		 while(snake.checkOverlap(fruit.getX(), fruit.getY())){
			 fruit.changeLocation();
		 }
		 snake.didEatFruit();
	 }
	snake.move(xinfo);
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&snake);
    dList.push_front(&fruit);
	
	XEvent event;
	unsigned long rightNow = now();
	unsigned long lastRepaint = now();
	int inside = 0;
	long moving = 0;
	while( !gameRestart ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */

	    rightNow = now();
		unsigned long elapsed = rightNow - lastRepaint;
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			//cout << "event.type=" << event.type << "\n";
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		}

		if(gameStart){
			if(moving == 0 || elapsed >= 1000000/FPS){
					repaint(xinfo);
					if(!gamePause){
						moving += 1;
						if(moving % (int)ceil((double)FPS/(inputSpeed + 15)) == 0){  //Makes sure that the snake moves a set number of times 
							handleAnimation(xinfo, inside);								// no matter what fps it is. (A bit off between 60fps and 25fps
						}																//since it is integer calculation)
					}
				lastRepaint = rightNow;
			}
		}else{
			if(elapsed >= 1000000/FPS){
				repaint(xinfo);
				lastRepaint = rightNow;
			}
		}
	} 

	
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	XInfo xInfo;

	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	while(gameRestart == true){
		gameRestart = false;
		snake.resetSnake(150,450);
		fruit.resetFruit();
		eventLoop(xInfo);
	}
	XCloseDisplay(xInfo.display);
}

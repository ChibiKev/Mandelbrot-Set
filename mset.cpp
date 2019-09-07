#include <QtGui>
#include "mset.h"

// for our timer function:
#include <time.h>
#include <inttypes.h>

// for debugging:
#include <iostream>
using std::cout;
using std::endl;


// hack: keep a variable to let us know if the application
// has been initialized so that we can draw the set on startup
bool drawnYet = false;


// In case you want to time your routines, the following function
// should give you a microsecond timer.
uint64_t clockGetTime_mu()
{
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000LL + (uint64_t)ts.tv_nsec / 1000LL;
}

mandelbrotArea::mandelbrotArea(QWidget *parent) : QWidget(parent) //Professor Wrote
{
	// set our inital view to hold the entire mandelbrot set.
	// a 3x3 square with -2,-1.5 as the lower left should work.
	llCoord.real = -2;
	llCoord.imag = -1.5;
	windowWidth = 3;
	this->maxIterations = 200; // a decent default
	setAttribute(Qt::WA_StaticContents);
}

bool mandelbrotArea::saveImage(const QString &fileName, const char *fileFormat) //Professor Wrote
{
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());
	visibleImage.save(fileName, fileFormat);
	return true;
}

void mandelbrotArea::refreshImage() //Written
{
	// this is just an example.  by making this function a "slot" (see the
	// header file) we can easily connect it to events from other objects
	// just as an illustration, we'll draw the gradient the other way.
	#if 0 //Professor Wrote=1 Written=0
	QPainter qp(&image);
	QColor qc;
	QPen qpen;
	// get the dimensions of our image in terms of pixels:
	unsigned long iwidth = image.width();
	unsigned long iheight = image.height();
	double unit = 1.0 / iwidth; // on a scale of 0-1, how wide is a pixel?
	for (unsigned long i = 0; i < iwidth; i++) {
		qc.setRgbF(i*unit,sqrt(i*unit),i*unit); // set the color we want to draw.
		qpen.setColor(qc); // apply the color to the pen
		qp.setPen(qpen);   // set the painter to use that pen
		qp.drawLine(iwidth-i-1,0,iwidth-i-1,iheight); // draw a line of the specified color.
	}
	update(); // repaint screen contents
	return;
#else
	this->render(); //Just calls render again to refresh, since it's like the same code
	return;
#endif
}

void mandelbrotArea::mouseDoubleClickEvent(QMouseEvent *event) //Written
{
	// TODO: write the code for the zoom operation.  You can get
	// the mouse button from event->button() and check for
	// event->button() == Qt::LeftButton, etc. to figure out what
	// button is being pressed.
	unsigned long iwidth = image.width();
	unsigned long iheight = image.height();

	//First declare the new x and y so it's able to generate
	double newX=  (windowWidth/iwidth) * event->x() + llCoord.real;
	double newY= -(windowWidth/iheight) * event->y() - llCoord.imag;

	// Left Click= Zoom in
	if (event->button() == Qt::LeftButton){
		windowWidth=windowWidth/2; //Dividing the window width since it's zooming in.
		llCoord.real= newX - windowWidth/2;
		llCoord.imag= newY - windowWidth/2;
		//Applying it to the newest window
		maxIterations+=100;
		this->render();
		//Render this and return
		return;
	}
	// Right Click= Zoom out
	if (event->button() == Qt::RightButton){
		windowWidth=windowWidth*2; //Diluting the window width since it's zooming out.
		llCoord.real= newX - windowWidth/2;
		llCoord.imag= newY - windowWidth/2;
		maxIterations-=100;
		this->render();
		return;
		//Same process as left click.
	}
}

void mandelbrotArea::paintEvent(QPaintEvent *event) //Professor Wrote
{
	// check: if blank, render first.
	if (!drawnYet) {
		drawnYet = true;
		this->render();
	}
	// otherwise, just repaint the portion of the screen that needs it.
	QPainter painter(this);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);
}

void mandelbrotArea::resizeEvent(QResizeEvent *event) //Professor Wrote
{
	resizeImage(&image, QSize(width(), height()));
	update();
	QWidget::resizeEvent(event);
	return;
}

void mandelbrotArea::render() //Written
{
	// TODO: write this function.
	// draw the portion of the mandelbrot set that lies in our window.
	// idea: loop through all the pixels, iterate the function figure out how
	// quickly the point is diverging, and color it based on the result
	// to give you some idea of what you need to do, I've drawn a nice green
	// gradient on the screen.  You will want to set the individual pixels
	// using the qp.drawPoint(x,y) function, instead of drawing a line.
	QPainter qp(&image);
	QColor qc;
	QPen qpen;
	// get the dimensions of our image in terms of pixels:
	unsigned long iwidth = image.width();
	unsigned long iheight = image.height();
  #if 0 //Professor Wrote=0 Written=1
	double unit = 1.0 / iwidth; // on a scale of 0-1, how wide is a pixel?
	for (unsigned long i = 0; i < iwidth; i++) {
		qc.setRgbF(i*unit,sqrt(i*unit),i*unit); // set the color we want to draw.
		qpen.setColor(qc); // apply the color to the pen
		qp.setPen(qpen);   // set the painter to use that pen
		qp.drawLine(i,0,i,iheight); // draw a line of the specified color.
	}
  #else
	//Implement a view x and y function
  for (size_t x=0; x<iwidth; x++){ //Setting x as the width to be displayed
			for (size_t y=0; y<iheight; y++){ //Setting y as the height to be displayed
					double real = (windowWidth/iwidth) * x + llCoord.real;
					double imag = -(windowWidth/iwidth) *  y - llCoord.imag;
					complex A  = complex(real, imag);
					complex B = complex(); //Used for messing around with
					complex C; //Just as storage
					bool explodeSet = false; //Declaration of explosion, used for color, original is set at false
					unsigned long numI = 1;
					for(numI = 1; numI < maxIterations ; numI++) {
						C  = B * B + A;
						if(C.norm() > 2){
							explodeSet = true; //complex number that messes up based off B^2 + A
							break;
						}
						B = C;
					}
					if (explodeSet) {
						qc.setRgbF(0,1.0-sqrt(1.0/numI),0); //The color of green based off RBG
						qpen.setColor(qc);  // apply the color to the pen
						qp.setPen(qpen); // set the painter to use that pen
						qp.drawPoint(x,y); // draw a point of the specified color.
					}
					else {
						qc.setRgbF(0,0,0); //The color of black based off RGB
						qpen.setColor(qc); // apply the color to the pen
						qp.setPen(qpen); // set the painter to use that pen
						qp.drawPoint(x,y); // draw a point of the specified color.
					}
				}
			}
      #endif
	update(); // Refresh the page with updated stuff
	return;
}

void mandelbrotArea::resizeImage(QImage *image, const QSize &newSize) //Professor Wrote
{
	// your custom resize code would go here...
	if (image->size() == newSize)
		return;

	// for now, we will just expand our image with blank (white) space.
	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}

unsigned long mandelbrotArea::getIterations() //Professor Wrote
{
	/* NOTE: this is used by MainWindow for the initial value to place
	 * in the iteration control dialog. */
	return this->maxIterations;
}

void mandelbrotArea::setIterations(unsigned long newMax) //Professor Wrote
{
	/* NOTE: this is called by MainWindow once the iterations
	 * dialog has been processed. */
	this->maxIterations = newMax;
}

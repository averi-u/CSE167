#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Your assignment consists of filling in parts that say /* YOUR CODE HERE */
// There are many helpful functions that can be found in Curve.cpp! In particular,
// take a look at moveActivePoint, drawLine, connectTheDots, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
	/* YOUR CODE HERE */
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	//make sure scene gets redrawn
	if (theOnlyCurve != NULL) 
	{
        float dx = (x - oldx) / (float)width;

        float dy = (height-y-oldy) / (float)height;

        theOnlyCurve->moveActivePoint(dx, dy);
		oldx = x; oldy = y;
        glutPostRedisplay();
    }
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
	float posx = x / (float)width;
    float posY = (height - y) / (float)height;

	if (theOnlyCurve && state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			/* YOUR CODE HERE */
			theOnlyCurve->addPoint(posx, posY);
		}
		if (button == GLUT_RIGHT_BUTTON) {	
			/* YOUR CODE HERE */
			theOnlyCurve->updateActivePoint(posx,posY);

      		theOnlyCurve->deleteActivePoint();
		}
	}

	/* YOUR CODE HERE */
	//update oldx, and oldy
	//make sure scene gets redrawn
	oldx = x; oldy = y;
	glutPostRedisplay();

}



#include "Bezier.h"

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {

	connectTheDots();
	int i,j,k;
	Pvector::iterator p;

	/* YOUR CODE HERE */
	Pvector bezier;
	for (float u = 0; u <= 1; u += 1.0f/levelOfDetail) {
		Pvector pts = points;

		while (pts.size() > 1) {
		Pvector newPts;
		for (int i = 0; i < pts.size() - 1; i++) 
		{
			float x = (1-u)*pts[i].x + u*pts[i+1].x;

			float y = (1-u)*pts[i].y + u*pts[i+1].y;

			newPts.push_back(Point(x, y));
			
		}
		pts = newPts;
		}

		bezier.push_back(pts[0]);
	}
	
	for (int i = 1; i < bezier.size(); i++) 
	{

		drawLine(bezier[i].x, bezier[i].y, bezier[i-1].x, bezier[i-1].y);
	}


}



#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {


	connectTheDots();
	/* YOUR CODE HERE */
	Pvector::iterator p1 = points.begin();
	Pvector::iterator p2 = p1 + 1;

	Pvector::iterator p3 = p2 + 1;
	Pvector::iterator p4 = p3 + 1;

	while (p4 != points.end()) {
		drawSegment(p1, p2, p3, p4, levelOfDetail);

		p1++; p2++; 
		p3++; p4++;
	}
}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, 
		Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {

	float x, y;
	/* YOUR CODE HERE */

	//draw segment
	float prevX = (1/6.0f)*((float)((p1->x) + 4*(p2->x) + (p3->x)));

	float prevY = (1/6.0f)*((float)((p1->y) + 4*(p2->y) + (p3->y)));

	Point(prevX, prevY).draw();
	for (int j = 1; j <= levelOfDetail; j++) {
		float u = j/((float)levelOfDetail);
		
		float x =  u*u*u*((1/6.0f)*((float)( -(p1->x) + 3*(p2->x) - 3*(p3->x) + (p4->x))))
				+  u*u*((1/6.0f)*((float)(3*(p1->x) - 6*(p2->x) + 3*(p3->x))))
				+    u*((1/6.0f)*((float)(-3*(p1->x) + 3*(p3->x))))

				+      ((1/6.0f)*((float)(  (p1->x) + 4*(p2->x) +   (p3->x))));

		float y =  u*u*u*((1/6.0f)*((float)( -(p1->y) + 3*(p2->y) - 3*(p3->y) + (p4->y))))
				+  u*u*((1/6.0f)*((float)(3*(p1->y) - 6*(p2->y) + 3*(p3->y))))

				+    u*((1/6.0f)*((float)(-3*(p1->y) + 3*(p3->y))))

				+      ((1/6.0f)*((float)(  (p1->y) + 4*(p2->y) +   (p3->y))));
		
		drawLine(prevX, prevY, x, y);
		prevX = x; prevY = y;
	}

	//then create a Point to be drawn where the knot should be

	Point p(x, y);
	p.draw();
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
		/* YOUR CODE HERE  (only one to three lines)*/
		float newX = ((1/2.0f)*((it->x) + ((it+1)->x)));
		float newY = ((1/2.0f)*((it->y) + ((it+1)->y)));
		*it = Point(newX, newY);
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {

				/* YOUR CODE HERE */
				drawLine((it-> x),(it-> y),
							((it+1)-> x),((it+1) -> y));
			}
		}
	} else {
		Pvector* accum = new Pvector();
		Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
		accumulateNextLevel(accum, points);
		Pvector::iterator it = (*accum).begin();
		
		int size = (*accum).size();
		int half = size / 2;

		for (int i = 1; i <= size; i++, it++) {
			if ((i <= half) || (((size % 2) == 1) && (i == (half + 1)))) 
			{
				left.points.push_back(*it);

			}
			if (i > half) 
			{
				right.points.push_back(*it);
			}
		}

		left.draw(1-levelOfDetail);
		right.draw(1-levelOfDetail);
		delete accum;
	}
}

/*************************************************************
 * 1. Name:
 *      Tyler Aston, Caleb Barzee
 * 2. Assignment Name:
 *      Lab 02: Apollo 11
 * 3. Assignment Description:
 *      Simulate the Apollo 11 landing
 * 4. What was the hardest part? Be as specific as possible.
 *      It seemed pretty straight forward, the most difficult was probably changing some of the output and input statements.
 *      *revised*
 *      Not repeating code by writing more modular functions proved challenging for me. I had to learn more about pass by reference
 *      in order for the code to remain concise.
 * 5. How long did it take for you to complete the assignment?
 *      We worked together for approximately 5 hours.
 *****************************************************************/

#include <iostream> // for CIN and COUT
using namespace std;

// For PI, square root, sin, cos
#define _USE_MATH_DEFINES
#include <math.h>

#define WEIGHT 15103.000  // Weight in KG (Maybe consider renaming to LANDER_MASS)
#define GRAVITY -1.625    // Vertical acceleration due to gravity, in m/s^2
#define THRUST 45000.000  // Thrust of main engine, in Newtons (kg m/s^2)
#define INITIAL_X_POS 0.0 // Assuming initial horizontal position to be 0
#define TIME_INTERVAL 1.0 // Calculations are done in 1 second interval
#define RUN_TEST          // if define run test cases or not
/***************************************************
 * COMPUTE DISTANCE
 * Apply inertia to compute a new position using the distance equation.
 * The equation is:
 *     s = s + v t + 1/2 a t^2
 * INPUT
 *     s : original position, in meters
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     s : new position, in meters
 **************************************************/
double computeDis(double s, double v, double t, double a)
{
   // I added parenthesis to help visualize order of operations
   return s + (v * t) + (0.5 * a) * (t * t);
}
/**************************************************
 * COMPUTE ACCELERATION
 * Find the acceleration given a thrust and mass.
 * This will be done using Newton's second law of motion:
 *     f = m * a
 * INPUT
 *     f : force, in Newtons (kg * m / s^2)
 *     m : mass, in kilograms
 * OUTPUT
 *     a : acceleration, in meters/second^2
 ***************************************************/
double computeAccel(double f, double m)
{
   return f / m;
}
/***********************************************
 * COMPUTE VELOCITY
 * Starting with a given velocity, find the new
 * velocity once acceleration is applied. This is
 * called the Kinematics equation. The
 * equation is:
 *     v = v + a t
 * INPUT
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     v : new velocity, in meters/second
 ***********************************************/
double computeVel(double v, double a, double t)
{
   // Returns new velocity
   return v + a * t;
}

/***********************************************
 * COMPUTE VERTICAL COMPONENT
 * Find the vertical component of a velocity or acceleration.
 * The equation is:
 *     cos(a) = y / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     y : the vertical component of the total
 ***********************************************/
double computeY(double a, double total)
{
   // Return Y
   return cos(a) * total;
}
/***********************************************
 * COMPUTE HORIZONTAL COMPONENT
 * Find the horizontal component of a velocity or acceleration.
 * The equation is:
 *     sin(a) = x / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     x : the vertical component of the total
 ***********************************************/
double computeX(double a, double total)
{
   // Return x
   return sin(a) * total;
}
/************************************************
 * COMPUTE TOTAL COMPONENT
 * Given the horizontal and vertical components of
 * something (velocity or acceleration), determine
 * the total component. To do this, use the Pythagorean Theorem:
 *    x^2 + y^2 = t^2
 * where:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    | /
 *    |/
 * INPUT
 *    x : horizontal component
 *    y : vertical component
 * OUTPUT
 *    total : total component
 ***********************************************/
double computeTotal(double x, double y)
{
   // a^2 + b^2 = c^2
   return sqrt((x * x) + (y * y));
}

/*************************************************
 * RADIANS FROM DEGEES
 * Convert degrees to radians:
 *     radians / 2pi = degrees / 360
 * INPUT
 *     d : degrees from 0 to 360
 * OUTPUT
 *     r : radians from 0 to 2pi
 **************************************************/
double toRadians(double degrees)
{
   // I did some simplification
   return degrees * M_PI / 180.0;
}
/**************************************************
 * PROMPT
 * A generic function to prompt the user for a double
 * INPUT
 *      message : the message to display to the user
 * OUTPUT
 *      response : the user's response
 ***************************************************/
double prompt(string message)
{
   double response;
   // Prompt message
   cout << message;
   cin >> response;

   return response;
}
/**************************************************
 * Rotate LM
 * Computes the vector components of acceleration based on angle.
 * INPUT
 *      accelerationThrust: Total acceleration of ML
 *      aDegrees: Angle of ML
 * OUTPUT
 *      returns an array of 3 doubles [ddx, ddy, aRadians]
 ***************************************************/
pair<double, double> rotateLM(double accelerationThrust, double aDegrees)
{
   double aRadians = toRadians(aDegrees);
   // Horizontal acceleration due to thrust
   double ddx = computeX(aRadians, accelerationThrust);
   // Vertical acceleration due to thrust
   double ddy = computeY(aRadians, accelerationThrust) + GRAVITY;
   return make_pair(ddx, ddy);
}
/**************************************************
 * MOVE LM
 * Updates LM position and velocity, calls displayStats to display updated position and velocity
 * INPUT
 *      x: horizontal position
 *      y: altitude
 *      dx: horizontal velocity
 *      dy: vertical velocity
 *      ddx: horizontal acceleration
 *      ddy: vertical acceleration
 * OUTPUT
 *      NA
 ***************************************************/
void moveLM(double &x, double &y, double &dx, double &dy, double ddx, double ddy)
{
   // Total horizontal velocity
   dx = computeVel(dx, ddx, TIME_INTERVAL);
   // Total vertical velocity
   dy = computeVel(dy, ddy, TIME_INTERVAL);
   // Compute distance traveled for each interval by x and y
   x = computeDis(x, dx, TIME_INTERVAL, ddx);
   y = computeDis(y, dy, TIME_INTERVAL, ddy);
}
/**************************************************
 * DISPLAY STATS
 * Displays the LM position and veloctity statistics
 * INPUT
 *      x: horizontal postion
 *      y: vertical positon
 *      dx: horizontal velocity
 *      dy: vertical velocity
 *      v: total velocity magnitude
 *      degrees: angle of LM
 *      seconds: time elapsed
 * OUTPUT
 *      NA
 ***************************************************/
void displayStats(double x, double y, double dx, double dy, double aDegrees, double secondsElapsed)
{
   // Output
   cout.setf(ios::fixed | ios::showpoint);
   cout.precision(2);
   cout << secondsElapsed << "s - ";
   cout << "x, y:(" << x << ", " << y << ")m ";
   cout << "dx, dy:(" << dx << ", " << dy << ")m/s ";
   cout << "speed:" << computeTotal(dx, dy) << "m/s ";
   cout << "angle:" << aDegrees << "deg \n";
}
/****************************************************************
 * RUN TEST
 * Runs previously initialized tests of LM simulation
 * INPUT
 *      testName: string of test name to be run
 *      testArray: array of test inputs ({y, dx, dy, degrees1, degrees2})

 ****************************************************************/
void runTest(string testName, double testArray[])
{
   // variable intializations
   double x = INITIAL_X_POS;
   double accelerationThrust = computeAccel(THRUST, WEIGHT);
   double secondsElapsed = 0;
   double y = testArray[0];
   double dx = testArray[1];
   double dy = testArray[2];
   double aDegrees = testArray[3];
   pair<double, double> accelVector = rotateLM(accelerationThrust, aDegrees);
   cout << "\nRunning test: " << testName << "\n\n";
   for (int i = 0; i < 10; i++)
   {
      if (i == 5)
      {
         aDegrees = testArray[4];
         accelVector = rotateLM(accelerationThrust, aDegrees);
      }
      moveLM(x, y, dx, dy, accelVector.first, accelVector.second);
      secondsElapsed += TIME_INTERVAL;
      displayStats(x, y, dx, dy, aDegrees, secondsElapsed);
   }
}
/****************************************************************
 * RUN USER INIT
 * Runs user initialized simulation of LM
 ****************************************************************/
void runUserInit()
{
   // initialize variables
   double x = INITIAL_X_POS;
   double accelerationThrust = computeAccel(THRUST, WEIGHT);
   double secondsElapsed = 0;
   pair<double, double> accelVector = make_pair(0.0, 0.0);
   // Prompt for input and variables to be computed
   double dx = prompt("What is your horizontal velocity (m/s)? ");
   double dy = prompt("What is your vertical velocity (m/s)? ");
   double y = prompt("What is your altitude (m)? ");
   double aDegrees = prompt("What is the angle of the LM where 0 is up (degrees)? ");
   cout << endl;
   cout << "For the next 5 seconds with the main engine on, the position of the lander is:\n\n";
   for (int i = 0; i < 10; i++)
   {
      if (i == 0)
         accelVector = rotateLM(accelerationThrust, aDegrees);
      if (i == 5)
      {
         aDegrees = prompt("\nWhat is the angle of the LM where 0 is up (degrees)? ");
         accelVector = rotateLM(accelerationThrust, aDegrees);
         cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is:\n\n";
      }
      moveLM(x, y, dx, dy, accelVector.first, accelVector.second);
      secondsElapsed += TIME_INTERVAL;
      displayStats(x, y, dx, dy, aDegrees, secondsElapsed);
   }
}
/****************************************************************
 * MAIN
 * Prompt for input, compute new position, and display output
 ****************************************************************/
int main()
{
#ifdef RUN_TEST
   // {y, dx, dy, degrees1, degrees2}
   double example[] = {100, 0, -10, 60, -60};
   double hardLanding[] = {100.0, 10.53, -13.959, -45.0, 0.0};
   double crash[] = {207.77, -35.00, -15.00, 90.0, 45.0};
   double armstrong[] = {56.11, 10.00, -10.00, -42.185, 0.0};

   //   string test0 = "Example";
   //   runTest(test0, example);
   string test1 = "Hard Landing";
   runTest(test1, hardLanding);
   string test2 = "Crash";
   runTest(test2, crash);
   string test3 = "Armstrong is awesome!";
   runTest(test3, armstrong);
   return 0;
#endif
   runUserInit();
   return 0;
}

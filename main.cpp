#pragma comment(lib,"cg.lib")  
#pragma comment(lib,"cgGL.lib")  

#include <windows.h>  
#include <GL/glut.h>  
#include <CG/cg.h>  
#include <CG/cgGL.h>  
#include <iostream>  

using namespace std;

//some global variables  
static CGcontext context;
static CGprofile myCgVertexProfile, myCgFragmentProfile;
static CGprogram myCgVertexProgram, myCgFragmentProgram;
static const char *myProgramName = "CgSample4_varying_parameter",
*myVertexProgramFileName = "vertex.cg",
*myVertexProgramName = "main_v",
*myFragmentProgramFileName = "fragment.cg",
*myFragmentProgramName = "main_f";
bool CgUsed = true;

static void checkForCgError(const char *situation)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);
	if (error != CG_NO_ERROR)
	{
		cout << myProgramName << " "
			<< situation << " "
			<< string << endl;
		if (CG_COMPILER_ERROR == error)
			cout << cgGetLastListing(context) << endl;
		exit(1);
	}
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);

	//init Cg   
	context = cgCreateContext();

	myCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	checkForCgError("selecting vertex profile");

	myCgVertexProgram = cgCreateProgramFromFile(
		context,
		CG_SOURCE,
		myVertexProgramFileName,
		myCgVertexProfile,
		myVertexProgramName,
		NULL);
	checkForCgError("Creating vertex Cg program from file");

	cgGLLoadProgram(myCgVertexProgram);
	checkForCgError("loading vertex program");

	myCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	checkForCgError("selecting fragment profile");
	myCgFragmentProgram = cgCreateProgramFromFile(
		context,
		CG_SOURCE,
		myFragmentProgramFileName,
		myCgFragmentProfile,
		myFragmentProgramName,
		NULL);
	checkForCgError("creating fragment program from file");

	cgGLLoadProgram(myCgFragmentProgram);
	checkForCgError("loading fragment program");
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		CgUsed = !CgUsed;
		glutPostRedisplay();
		break;
	case 27:
		cgDestroyProgram(myCgVertexProgram);
		cgDestroyProgram(myCgFragmentProgram);
		cgDestroyContext(context);
		exit(0);
		break;
	}
}

//display FPS on the title of the window  
void displayFPS() {
	static float framesPerSecond = 0.0f;       // This will store our fps  
	static float lastTime = 0.0f;       // This will hold the time from the last frame  
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		framesPerSecond /= currentTime - lastTime;
		char strFrameRate[256];
		lastTime = currentTime;
		sprintf_s(strFrameRate, "Current Frames Per Second: %f", framesPerSecond);
		glutSetWindowTitle(strFrameRate);
		framesPerSecond = 0;
	}
}

void display()
{
	displayFPS();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (CgUsed)//enable Cg   
	{
		cout << "Cg is been used !" << endl;
		cgGLBindProgram(myCgVertexProgram);
		checkForCgError("binding vertex program");

		cgGLEnableProfile(myCgVertexProfile);
		checkForCgError("enabling vertex profile");

		cgGLBindProgram(myCgFragmentProgram);
		checkForCgError("binding fragment program");

		cgGLEnableProfile(myCgFragmentProfile);
		checkForCgError("enabling fragment profile");
	}
	else
		cout << "Cg is not used !" << endl;

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);  /* Red */
	glVertex2f(-0.8, 0.8);

	glColor3f(0, 1, 0);  /* Green */
	glVertex2f(0.8, 0.8);

	glColor3f(0, 0, 1);  /* Blue */
	glVertex2f(0.0, -0.8);
	glEnd();

	cgGLDisableProfile(myCgVertexProfile);
	checkForCgError("disabling vertex profile");

	cgGLDisableProfile(myCgFragmentProfile);
	checkForCgError("disabling fragment profile");

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Cg Test 2");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
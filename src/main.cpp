/*
 * Allows the user to explore a procedurally generated landscape consisting of
 * lakes, trees, and cabins. This source code is based on Assignment 3.
 *
 * See README.pdf for keyboard controls and further details.
 */
#include <cstdio>
#include <ctime>
#include <queue>

#include "OpenGL.h"

#include "CoordinateAxes.h"
#include "ConeLight.h"
#include "ICamera.h"
#include "FrameBuffer.h"
#include "FreeCamera.h"
#include "FreeController.h"
#include "FreeFrame.h"
#include "Lights.h"
#include "Primitives.h"
#include "Scene.h"
#include "SchreuderUtil.h"
#include "Skybox.h"
#include "SphericalCamera.h"
#include "SphericalController.h"
#include "SphericalFrame.h"
#include "TextureManager.h"
#include "ToggleController.h"

// Scene to render
Scene* scene = NULL;
Skybox* skyBox;

// ICamera to view the world from.
SphericalFrame* cameraSphericalFrame = NULL;
IController* cameraSphericalController = NULL;

FreeFrame* cameraFreeFrame = NULL;
IController* cameraFreeController = NULL;

ToggleController* cameraController = NULL;

ICamera* camera = NULL;

// Light to illuminate the world.
SphericalFrame* lightFrame = NULL;
ConeLight* coneLight = NULL;
IController* lightController = NULL;

// Swap between different controls
int activeController = 0;
int numControllers = 2;
IController** controllers = NULL;

// Handle associated with the window so that it can be closed correctly.
int globalWindowHandle = -1;

// For FPS calculation
std::queue<clock_t> times;

bool doReflections = true;

void deinitialize() {
	delete skyBox;
	delete scene;
	delete camera;
	delete coneLight;

	delete[] controllers;
	delete lightController;
	delete cameraController;
	delete cameraFreeController;
	delete cameraSphericalController;

	delete lightFrame;
	delete cameraFreeFrame;
	delete cameraSphericalFrame;

	TextureManager::Get()->UnloadAll();
}

/*
* This function is called by GLUT when the window is resized
*/
void reshape(int width, int height)
{
	//  Calculate width to height ratio
	double w2h = (height > 0) ? (double)width / height : 1;

	//  Set viewport as entire window
	glViewport(0, 0, width, height);

	//  Select projection matrix
	glMatrixMode(GL_PROJECTION);

	//  Set projection to identity
	glLoadIdentity();

	// Setup the projection based on the camera.
	camera->SetupProjection(w2h);

	//  Select model view matrix
	glMatrixMode(GL_MODELVIEW);

	//  Set model view to identity
	glLoadIdentity();
}

/*
* Handles regular keyboard input.
*/
void keyboard(unsigned char key, int x, int y) {
	// Exit the program. (char 27 = ESC)
	if (key == 27) {
		deinitialize();
		glutDestroyWindow(globalWindowHandle);
		globalWindowHandle = -1;
		return;
	}

	// Change what is being controlled (camera / light)
	int modifier = glutGetModifiers();
	if (key == '\t') {
		if (modifier == GLUT_ACTIVE_CTRL) {
			// backward
			if (activeController == 0)
				activeController = numControllers - 1;
			else
				activeController--;
		}
		else {
			// forward
			if (activeController == numControllers - 1)
				activeController = 0;
			else
				activeController++;
		}
	}

	// Scene abilities
	if (key == 'n') {
		delete scene;
		scene = new Scene();
	}
	else if (key == '/') {
		doReflections = !doReflections;
	}
	else if (key == '.') {
		scene->ToggleSmoke();
	}

	// Swap the camera / navigation controls
	if (key == 'o' || key == 'p' || key == '9' || key == '0') {
		if (key == 'o') {
			camera->SetViewMode(VM_ORTHOGONAL);
		}
		else if (key == 'p') {
			camera->SetViewMode(VM_PERSPECTIVE);
		}
		else if (key == '9') {
			delete camera;
			camera = new SphericalCamera(cameraSphericalFrame, VM_ORTHOGONAL);
			cameraController->SetUntoggled();
		}
		else if (key == '0') {
			delete camera;
			camera = new FreeCamera(cameraFreeFrame);
			cameraController->SetToggled();
		}

		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	// Assume regular navigation control.
	else {
		controllers[activeController]->HandleKey(key);
	}

	glutPostRedisplay();
}

/*
* This function is called by GLUT when special keys are pressed
*/
void special(int key, int x, int y) {
	controllers[activeController]->HandleSpecialKey(key);

	glutPostRedisplay();
}

/*
* Allocates memory for all of the non-OpenGL assets.
*/
void initialize() {
	cameraSphericalFrame = new SphericalFrame(0.4, 0, -45, 45);
	cameraFreeFrame = new FreeFrame(4, 4, 4, 0, 40, 227);
	lightFrame = new SphericalFrame(5, 0, -45, 45);

	cameraSphericalController = new SphericalController(cameraSphericalFrame);
	cameraFreeController = new FreeController(cameraFreeFrame);
	cameraController = new ToggleController(cameraSphericalController, cameraFreeController);
	lightController = new SphericalController(lightFrame);

	controllers = new IController*[2];
	controllers[0] = cameraController;
	controllers[1] = lightController;

	coneLight = new ConeLight(lightFrame, GL_LIGHT0, &LIT_NATURAL, 45, 4);
	camera = new SphericalCamera(cameraSphericalFrame, VM_ORTHOGONAL);
	scene = new Scene();
	skyBox = new Skybox();
}

/*
* Instructs the scene to update its internal state.
*/
void idle(int notUsed) {
	// If the window has closed, no point in doing anything.
	if (globalWindowHandle < 0)
		return;

	scene->DoIdle();
	glutPostRedisplay();
	glutTimerFunc(42, idle, 0);
}

/*
* Renders the scene based on the type of pass (refraction, reflection, etc).
*/
void doPass(RenderPass renderPass) {
	// Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Render from below the surface of the water.
	if (renderPass == RP_REFLECTION) {
		cameraSphericalFrame->orientation.AddPitch(-2 * cameraSphericalFrame->orientation.Pitch());

		cameraFreeFrame->orientation.AddPitch(-2 * cameraFreeFrame->orientation.Pitch());
		cameraFreeFrame->position.z += -2 * cameraFreeFrame->position.z;
	}

	camera->Render();

	// Render the light
	coneLight->Render();

	if (renderPass != RP_FINAL) {
		glEnable(GL_CLIP_PLANE0);
		GLdouble planeEq[] = { 0, 0, +1, 0 };
		if (renderPass == RP_REFRACTION)
			// +1 above water, -1 below water
			planeEq[2] = -1;

		glClipPlane(GL_CLIP_PLANE0, planeEq);
	}

	glPushMatrix();
	glTranslatef(-.55, -.55, -.25);
	glScalef(1.1, 1.1, 1.1);
	skyBox->Render();
	glPopMatrix();

	glTranslatef(-0.5, -0.5, 0);

	scene->SetPass(renderPass);
	scene->Render();

	if (renderPass != RP_FINAL) {
		glDisable(GL_CLIP_PLANE0);
	}

	// Put the camera back where it belongs.
	if (renderPass == RP_REFLECTION) {
		cameraSphericalFrame->orientation.AddPitch(-2 * cameraSphericalFrame->orientation.Pitch());

		cameraFreeFrame->orientation.AddPitch(-2 * cameraFreeFrame->orientation.Pitch());
		cameraFreeFrame->position.z += -2 * cameraFreeFrame->position.z;
	}
}

/*
 * This function is called by GLUT to display the scene
 */
void display() {
	clock_t now = clock();
	clock_t cutOff = now - CLOCKS_PER_SEC;
	while (times.size() > 0 && times.front() < cutOff)
		times.pop();

	if (doReflections) {
		int width = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);

		FrameBuffer reflectionBuffer = FrameBuffer(width, height, GL_COLOR_ATTACHMENT0, GL_NONE);
		reflectionBuffer.Begin();
		doPass(RP_REFLECTION);
		reflectionBuffer.End();

		FrameBuffer refractionBuffer = FrameBuffer(width, height, GL_COLOR_ATTACHMENT1, GL_DEPTH_ATTACHMENT);
		refractionBuffer.Begin();
		doPass(RP_REFRACTION);
		refractionBuffer.End();

		scene->GetWater()->SetBuffers(&reflectionBuffer, &refractionBuffer);
		doPass(RP_FINAL);
	}
	else {
		scene->GetWater()->SetBuffers(NULL, NULL);
		doPass(RP_FINAL);
	}

#if _DEBUG
	CoordinateAxes ca = CoordinateAxes();
	ca.Render();
	Print("FPS: %d", times.size());
#endif

	// Sanity check
	ErrCheck("display");

	//  Flush and swap buffer
	glFlush();
	glutSwapBuffers();

	times.push(now);
}

/*
 * GLUT based application
 */
int main(int argc, char* argv[]) {
	//  Initialize GLUT
	glutInit(&argc, argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	//  Create window
	glutInitWindowSize(500, 500);
	globalWindowHandle = glutCreateWindow("Project: Garrett Lewellen");

	//  Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

#ifdef USEGLEW
	// Prepare glew so that shaders can be used later in the code.
	GLenum initResult = glewInit();
	if (initResult != GLEW_OK) {
		fprintf(stderr, "Failed to initialize glew. result = %d\n", initResult);
		exit(1);
	}
#endif

#ifdef DEBUG
	// 2015-06-6 GEL Added the following to verify my dedicated video card was 
	// being used. (GeForce GT 525M/PCI/SSE2 vs. Intel(R) HD Graphics Family)
	//
	// Note to self:
	// Desktop > NVIDIA Control Panel > 3D Settings > Manage 3D Settings 
	//         > Global Settings 
	//         > Preferred graphics processor: High-performance NVIDIA processor
	fprintf(stdout, "Vendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "Version: %s\n", glGetString(GL_VERSION));
#endif

	// Set up alpha channel support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 2015-06-15 GEL Found that this fixes the overlapping transparent texture 
	// problem.
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	// Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);		// Make sure scaling doesn't interfer with light calculation.
	glShadeModel(GL_SMOOTH);	// Make lighting look less boxy (smoother looking spheres/curved surfaces)


	initialize();

	idle(0);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;
}
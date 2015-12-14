#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cLaser.h"
#include "tardisWarsGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;

	// Sphere
	cSphere Sun(2, 40, 40);
	cSphere Mercury(0.08, 40, 40);
	cSphere Venus(0.2, 40, 40);
	cSphere Earth(0.3, 30, 30);
	cSphere Moon(0.02, 20, 20);
	cSphere Mars(0.2, 20, 20);
	cSphere Jupiter(0.8, 20, 20);
	cSphere Saturn(0.5, 20, 20);
	cSphere Uranus(0.4, 20, 20);
	cSphere Neptune(0.4, 20, 20);



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
	
	//The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	// Create Texture map
	cTexture tardisTexture;
	tardisTexture.createTexture("Models/tardis.png");
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceShip/sh3.jpg");
	cTexture laserTexture;
	laserTexture.createTexture("Models/laser.tga");
	cTexture starTexture;
	starTexture.createTexture("Images/star.png");
	cTexture sunTexture;
	sunTexture.createTexture("Images/Sun.png");
	cTexture mercuryTexture;
	mercuryTexture.createTexture("Images/Mercury.jpg");
	cTexture venusTexture;
	venusTexture.createTexture("Images/Venus.jpg");
	cTexture earthTexture;
	earthTexture.createTexture("Images/Earth.png");
	cTexture moonTexture;
	moonTexture.createTexture("Images/Moon.png");
	cTexture marsTexture;
	marsTexture.createTexture("Images/Mars.jpg");
	cTexture jupiterTexture;
	jupiterTexture.createTexture("Images/Jupiter.jpg");
	cTexture saturnTexture;
	saturnTexture.createTexture("Images/Saturn.jpg");
	cTexture uranusTexture;
	uranusTexture.createTexture("Images/Uranus.jpg");
	cTexture neptuneTexture;
	neptuneTexture.createTexture("Images/Neptune.jpg");

	Sun.initialise(sunTexture.getTexture(), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	Mercury.initialise(mercuryTexture.getTexture(), glm::vec3(-3, 0, -3), glm::vec3(0, 0, 0));
	float mercuryRotSpeed = 0.0f;
	GLfloat mercuryOrbit = 0.0f;

	Venus.initialise(venusTexture.getTexture(), glm::vec3(4, 0, 4), glm::vec3(0, 0, 0));
	float venusRotSpeed = 0.0f;
	GLfloat venusOrbit = 0.0f;

	Earth.initialise(earthTexture.getTexture(), glm::vec3(0, 0, 6), glm::vec3(0, 0, 0));
	float earthRotSpeed = 5.0f;
	GLfloat earthOrbit = 0.0f;

	Moon.initialise(moonTexture.getTexture(), glm::vec3(0, 0.5, 6.6), glm::vec3(0, 0, 0));
	float moonRotSpeed = 0.0f;
	GLfloat moonOrbit = 0.0f;

	Mars.initialise(marsTexture.getTexture(), glm::vec3(8, 0, -8), glm::vec3(0, 0, 0));
	float marsRotSpeed = 0.0f;
	GLfloat marsOrbit = 0.0f;

	Jupiter.initialise(jupiterTexture.getTexture(), glm::vec3(-10, 0, 10), glm::vec3(0, 0, 0));
	float jupiterRotSpeed = 0.0f;
	GLfloat jupiterOrbit = 0.0f;

	Saturn.initialise(saturnTexture.getTexture(), glm::vec3(12, 0, 12), glm::vec3(0, 0, 0));
	float saturnRotSpeed = 0.0f;
	GLfloat saturnOrbit = 0.0f;

	Uranus.initialise(uranusTexture.getTexture(), glm::vec3(14, 0, 14), glm::vec3(0, 0, 0));
	float uranusRotSpeed = 0.0f;
	GLfloat uranusOrbit = 0.0f;

	Neptune.initialise(neptuneTexture.getTexture(), glm::vec3(-16, 0, 16), glm::vec3(0, 0, 0));
	float neptuneRotSpeed = 0.0f;
	GLfloat neptuneOrbit = 0.0f;

	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);
	cMaterial mercuryMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial venusMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial earthMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial moonMaterial(lightColour4(0.1f, 0.1f, 0.1f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(0, 0, 0, 1.0f), 10.0f);
	cMaterial marsMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial jupiterMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial saturnMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial uranusMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial neptuneMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial tardisMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 4.2, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -4.2, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-4.2, 0, 0, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(4.2, 0, 0, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight topLight(GL_LIGHT4, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 4.2, 0, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight botLight(GL_LIGHT5, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, -.2, 0, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);



	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// Create a camera
	cCamera theCamera;
	theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);

	cCamera staticCamera;
	GLfloat cameraRotRadius = 20.0f;
	GLfloat cameraRotationAngle = 10.0f;

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Model
	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/tardis1314.obj", tardisTexture); // Player

	cModelLoader theLaser;
	theLaser.loadModel("Models/laser.obj", laserTexture);


	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, 0), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);

	float tCount = 0.0f;
	string outputMsg;

	theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);

	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		// Lab code goes here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth, windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (camera1){
			glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());
			glm::vec3 currentCameraPos = theCamera.getTheCameraPos();
			GLfloat posX = (glm::sin(glm::radians(cameraRotationAngle)) * cameraRotRadius); // *elapsedTime;
			GLfloat posZ = (glm::cos(glm::radians(cameraRotationAngle)) * cameraRotRadius); // *elapsedTime;
			theCamera.setTheCameraPos(glm::vec3(posX, 0.0f, posZ));
			theCamera.update();
			cameraRotationAngle -= (5.0f * elapsedTime);
		}
		else if (camera2){
			staticCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 20.0f));
			staticCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
			staticCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
			staticCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
			staticCamera.setTheProjectionMatrix(45.0f, staticCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
			glLoadMatrixf((GLfloat*)&staticCamera.getTheViewMatrix());
			staticCamera.update();
		}
	

		tardisMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		thePlayer.update(elapsedTime);

		for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
		{
			if ((*laserIterartor)->isActive())
			{
				theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
				(*laserIterartor)->update(elapsedTime);
			}
		}

		theStarField.render(0.0f);
		sunMaterial.useMaterial();
		//Sun
		Sun.prepare(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();
		topLight.lightOn();
		botLight.lightOn();
		Sun.render(Sun.getRotAngle());
		//Mercury
		Mercury.prepare(Mercury.getRotAngle());
		glRotatef(mercuryOrbit, 0.0f, 1.0f, 0.0f);
		mercuryMaterial.useMaterial();
		Mercury.render(Mercury.getRotAngle());
		//Venus
		Venus.prepare(Venus.getRotAngle());
		glRotatef(venusOrbit, 0.0f, 1.0f, 0.0f);
		venusMaterial.useMaterial();
		Venus.render(Venus.getRotAngle());
		// Earth
		Earth.prepare(Earth.getRotAngle()); //Do any pre-rendering logic
		glRotatef(earthOrbit, 0.0f, 1.0f, 0.0f);
		earthMaterial.useMaterial();				// Set the material for use
		Earth.render(Earth.getRotAngle()); //Render the scene
		// Moon
		Moon.prepare(Moon.getRotAngle());
		moonMaterial.useMaterial();
		Moon.render(Moon.getRotAngle());
		// Mars
		Mars.prepare(Mars.getRotAngle());
		glRotatef(marsOrbit, 0.0f, 1.0f, 0.0f);
		marsMaterial.useMaterial();
		Mars.render(Mars.getRotAngle());
		// Jupiter
		Jupiter.prepare(Jupiter.getRotAngle());
		glRotatef(jupiterOrbit, 0.0f, 1.0f, 0.0f);
		jupiterMaterial.useMaterial();
		Jupiter.render(Jupiter.getRotAngle());
		// Saturn
		Saturn.prepare(Saturn.getRotAngle());
		glRotatef(saturnOrbit, 0.0f, 1.0f, 0.0f);
		saturnMaterial.useMaterial();
		Saturn.render(Saturn.getRotAngle());
		// Uranus
		Uranus.prepare(Uranus.getRotAngle());
		glRotatef(uranusOrbit, 0.0f, 1.0f, 0.0f);
		uranusMaterial.useMaterial();
		Uranus.render(Uranus.getRotAngle());
		// Neptune
		Neptune.prepare(Neptune.getRotAngle());
		glRotatef(neptuneOrbit, 0.0f, 1.0f, 0.0f);
		neptuneMaterial.useMaterial();
		Neptune.render(Neptune.getRotAngle());

		neptuneOrbit -= (0.1f * elapsedTime);
		uranusOrbit += (0.2f * elapsedTime);
		saturnOrbit += (0.3f * elapsedTime);
		jupiterOrbit += (0.4f * elapsedTime);
		marsOrbit += (0.5f * elapsedTime);
		earthOrbit += (0.6f * elapsedTime);
		venusOrbit += (0.7f * elapsedTime);
		mercuryOrbit += (0.8f * elapsedTime);

		pgmWNDMgr->swapBuffers();

		tCount += elapsedTime;

		thePlayer.update(elapsedTime);

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}

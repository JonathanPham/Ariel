// Ariel: FLIP Fluid Simulator
// Written by Yining Karl Li
//
// File: viewer.hpp
// GL viewer based on standard Takua viewer framework

#ifndef VIEWER_HPP
#define VIEWER_HPP

#define GLEW_STATIC

#include <tbb/tbb.h>
#include <tbb/mutex.h>
#include <thread> 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "../sim/flip.hpp"

namespace viewerCore {

struct VboData{
	GLuint 			m_vboID;
	GLuint 			m_cboID;
	int 			m_size;
	GLenum 			m_type;
	std::string 	m_key;
};

//Used just for tracking OpenGL viewport camera position/keeping in sync with rendercam
struct GLCamera{
	glm::vec2 		m_mouseOld;
	glm::vec3 		m_rotate;
	glm::vec3 		m_translate;
	glm::vec2 		m_fov;
	float 			m_lookat;
	int 			m_currentKey;
	int 			m_currentMouseClick;
	float 			m_rotateSpeed;
	float 			m_zoomSpeed;
	float 			m_panSpeed;

	//Initializer
	GLCamera(): m_mouseOld(glm::vec2(0.0f,0.0f)), 
				m_rotate(glm::vec3(0.0f,0.0f,0.0f)), 
				m_translate(glm::vec3(0.0f,0.0f,0.0f)),
				m_lookat(0.0f),
				m_fov(45.0f),
				m_currentKey(0),
				m_currentMouseClick(0),
				m_rotateSpeed(0.2f),
				m_zoomSpeed(0.1f),
				m_panSpeed(0.1f){};
};

//====================================
// Class Declarations
//====================================

class Viewer{
	public:
		Viewer();
		~Viewer();

		bool Launch();
		void Load(fluidCore::flipsim* sim, bool retina);
		void Load(fluidCore::flipsim* sim, bool retina, glm::vec2 resolution, glm::vec3 camrotate, 
				  glm::vec3 camtranslate, glm::vec2 camfov, float camlookat);
	private:
		//Initialize stuff
		bool Init();

		//Sim thread stuff
		void SimLoopThread();

		//Main draw functions
		void MainLoop();
		void UpdateInputs();

		//VBO stuff
		VboData CreateVBO(VboData data, float* vertices, unsigned int vertexcount, float* colors,
                          unsigned int colorcount, GLenum type, std::string key);
		VboData CreateVBOFromObj(objCore::Obj* o, glm::vec4 color, std::string key);

		void SaveFrame();

		//Interface callbacks
		static void ErrorCallback(int error, const char* description);		
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		//Cached data
		bool 								m_loaded;
		bool 								m_runrender;
		glm::vec2 							m_resolution;
		GLFWwindow* 						m_window;
		std::vector<VboData> 				m_vbos;
		std::map<std::string, int> 			m_vbokeys;
		std::map<std::string, glm::vec2> 	m_frameranges;
		GLCamera 							m_cam;
		std::vector<fluidCore::particle*>* 	m_particles;

    	fluidCore::flipsim* 				m_sim;
    	bool 								m_siminitialized;
    	bool 								m_drawobjects;
    	bool 								m_drawInvalid;
    	
    	unsigned char* 						m_bitmapData;
    	bool 								m_dumpFramebuffer;
    	bool 								m_dumpReady;
    	bool 								m_pause;
    	tbb::mutex 							m_framebufferWriteLock;
    	int 								m_framebufferScale;

    	bool 								m_dumpVDB;
    	bool 								m_dumpOBJ;
    	bool 								m_dumpPARTIO;
};
}

#endif

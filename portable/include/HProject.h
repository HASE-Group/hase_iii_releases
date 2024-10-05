#pragma once
#include <sstream>
#include <fstream>
#include <map>
/**
* Members of this class have to be used to get the environment attributes of a HASE project. This environment has
* been previously created by the HBuilder through interaction with the user. This class manages those parts of a
* project that are somehow common to both HEngine DES and HBuilder.
*
* author  Juan Carlos Diaz y Carballo. July 2005.
*/

//using namespace std;

class HProject
{
	std::string      animationFile;
	bool        isAnimated;
	std::ofstream    animation;
	float		timeStamp;                       // To allow printing the time for the 1st entry

	std::map<float, std::string> eventTimes;               // Holds the time and the data of a delayed event

	std::string  getProperty(std::string, int);


 public:

	HProject() {
		isAnimated = false;
		timeStamp = 0.0f;
	};

	bool	open( const char*);
	bool    isAnimationOn();
	void    addEvent(float);
	bool    createAnimation();
	bool    createAnimation(std::string, int, int, int, int, int);
	void    writeAnimationEntry(const char *, float);
	void    createAnimationMetaData();

};

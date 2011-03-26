#include "stereomatching.hpp"


StereoMatching::StereoMatching(char * theName): name(theName){
	everyFrameGet = 1; // it means - every frame
	propertiesChanged = false;
}

StereoMatching::~StereoMatching(){
	Property * element;

	while(properties.size() > 0){
		element = properties.back();
		properties.pop_back();
		delete element;
	}
}

// ustawienie flagi, ze zmienily sie parametry
// przy kolejnym wywolaniu exec zostana one wziete pod uwage
void StereoMatching::setPropertiesWereChanged(){
	propertiesChanged = true;
}

void StereoMatching::changeProperties(){
	// change values in all properties
	for(int i = 0; i < properties.size(); ++i)
		properties[i]->changeValue();
}

	// setters
void StereoMatching::setEveryFrameGet(int value){
	everyFrameGet = value;
}
	
	// getters
int	StereoMatching::getEveryFrameGet(){
	return everyFrameGet;
}

char * StereoMatching::getName(){
	return name;
}
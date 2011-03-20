#include "stereomatching.hpp"


StereoMatching::StereoMatching(std::string theName): name(theName){
	everyFrameGet = 1; // it means - every frame
}

StereoMatching::~StereoMatching(){
	Property * element;

	while(properties.size() > 0){
		element = properties.back();
		properties.pop_back();
		delete element;
	}
}

	
	// list of properties
boolean	StereoMatching::propetriesWereChanged(){
	// check all properties if they were changed

	bool changed = false;
	for(int i = 0; i < properties.size(); ++i)
		changed = changed || properties[i]->wasChanged();

	return false;
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

std::string StereoMatching::getName(){
	return name;
}
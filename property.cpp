#include "property.hpp"

#include <stdio.h>

Property::Property(char * theName, int theValue, Property::TYPE theType):
name(theName), 
value(theValue),
defaultValue(theValue),
type(theType){
	
	tableValues = NULL;
	valueToChange = value;
	minValue = -1;
	maxValue = -1;
	step = 1;
}


Property::~Property(){
	if(tableValues)
		delete tableValues;
}


// getters
char * Property::getName(){
	return name;
}

int	Property::getValue(){
	return value;
}

int	Property::getDefaultValue(){
	return defaultValue;
}

Property::TYPE Property::getType(){
	return type;
}


void Property::changeValue(){
	value = valueToChange;
}

	// setters

void Property::setValueToChange(int newVal){
	valueToChange = newVal;
}

void Property::setValueToChangeUp(){
	if(type != BOOL){
		++valueToChange;
	}
}

void Property::setValueToChangeDown(){
	if(type != BOOL){
		--valueToChange;
	}
}

void Property::setValueToDefault(){
	valueToChange = defaultValue;
}


void Property::setTableValues(int * values){
	tableValues = values;
}
#include "property.hpp"


Property::Property(std::string theName, int theValue, Property::TYPE theType):
name(theName), 
value(theValue),
defaultValue(theValue),
type(theType){
	
	tableValues = NULL;
	valueToChange = value;
	minValue = -1;
	maxValue = -1;
	changed = false;
}


Property::~Property(){
	if(tableValues)
		delete tableValues;
}


	// getters
std::string Property::getName(){
	return name;
}

int	Property::getValue(){
	if(type == DISCRETE && tableValues)
		return tableValues[value];
	return value;
}

int	Property::getMinValue(){
	return minValue;
}

int	Property::getMaxValue(){
	return maxValue;
}

int	Property::getDefaultValue(){
	return defaultValue;
}

Property::TYPE Property::getType(){
	return type;
}

bool Property::wasChanged(){
	return changed;
}

void Property::changeValue(){
	value = valueToChange;
	changed = false;
}

	// setters

void Property::setValueToChange(int newVal){
	valueToChange = newVal;
	changed = true;
}

void Property::setValueToChangeUp(){
	++valueToChange;
	changed = true;
}

void Property::setValueToChangeDown(){
	--valueToChange;
	changed = true;
}

void Property::setValueToDefault(){
	valueToChange = defaultValue;
	changed = true;
}

void Property::setMinValue(int newMin){
	minValue = newMin;
}

void Property::setMaxValue(int newMax){
	maxValue = maxValue;
}

void Property::setTableValues(int * values){
	tableValues = values;
}
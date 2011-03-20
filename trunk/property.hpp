#pragma once

#include <string>

class Property
{
public:
	enum TYPE { INT, BOOL, DISCRETE};

	Property(std::string theName, int theValue, TYPE theType);
	~Property();


	// getters
	std::string getName();
	int			getValue();
	TYPE		getType();
	bool		wasChanged();
	void		changeValue();
	int			getDefaultValue();


	// setters
	void		setValueToChange(int newVal);
	void		setValueToChangeUp();
	void		setValueToChangeDown();
	void		setValueToDefault();

	void		setTableValues(int * values);
	

	int minValue;
	int maxValue;
////// for discrete - table with possible values
	int * tableValues;

protected:

	std::string name;
	
	TYPE type;

////// for all TYPE
////// if one of these is -1 then is not specified

	// if type == DISCRETE then value is the index in a table with values
	int value;

	// if type == DISCRETE then it's the min and max value of the index
	int valueToChange;
	int defaultValue;


	bool changed;
};


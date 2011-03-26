#pragma once

#include <QObject>

class Property: public QObject
{
	Q_OBJECT

public:
	enum TYPE { INT, BOOL, DISCRETE};

	Property(char * theName, int theValue, TYPE theType);
	~Property();


	// getters
	char *		getName();
	int			getValue();
	TYPE		getType();
	void		changeValue();
	int			getDefaultValue();
	
	//setters
	void		setTableValues(int * values);

	int minValue;
	int maxValue;
	int step;
////// for discrete - table with possible values
	int * tableValues;

public slots:

	// setters
	void		setValueToChange(int newVal);

	// not for BOOOL
	void		setValueToChangeUp();
	void		setValueToChangeDown();

	void		setValueToDefault();



protected:

	char * name;
	TYPE type;

////// for all TYPE
////// if one of these is -1 then is not specified

	// if type == DISCRETE then value is the index in a table with values
	int value;

	// if type == DISCRETE then it's the min and max value of the index
	int valueToChange;
	int defaultValue;
};


#pragma once

#include <string>
#include <cv.h>
#include <vector>

#include "property.hpp"

class StereoMatching
{
public:
	StereoMatching(char * theName);
	~StereoMatching();

	// virtual functions
	virtual void exec(cv::Mat & image1, cv::Mat & image2, cv::Mat & retImage) = 0;
	
	// list of properties
	void		setPropertiesWereChanged();
	void		changeProperties();
	
	std::vector<Property *> properties;
	int			id;

	// setters
	void		setEveryFrameGet(int value);
	
	// getters
	int			getEveryFrameGet();
	char *		getName();

protected:
	virtual void init() = 0;
	virtual void applyAllParametres() = 0;

	bool		propertiesChanged;

private:
	char *		name;
	int			everyFrameGet;

};


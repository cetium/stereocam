#pragma once

#include <string>
#include <cv.h>
#include <vector>

#include "property.hpp"

class StereoMatching
{
public:
	StereoMatching(std::string theName);
	~StereoMatching();

	// virtual functions
	virtual void init() = 0;
	virtual void exec(cv::Mat & image1, cv::Mat & image2, cv::Mat & retImage) = 0;
	virtual void applyAllParametres() = 0;
	
	// list of properties
	boolean		propetriesWereChanged();
	void		changeProperties();
	
	std::vector<Property *> properties;


	// setters
	void		setEveryFrameGet(int value);
	
	// getters
	int			getEveryFrameGet();
	std::string getName();

protected:
	std::string name;
	int			everyFrameGet;
};


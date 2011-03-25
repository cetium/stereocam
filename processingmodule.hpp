#ifndef PROCESSINGMODULE_HPP
#define PROCESSINGMODULE_HPP

#include <cv.h>
#include <vector>

#include "settings.hpp"
#include "processing.hpp"


class ProcessingModule{

public:
	ProcessingModule();
	~ProcessingModule();

	void init(Settings * sets);
	IplImage * process(IplImage * toProcess);

	IplImage * preMatchingProcessing(IplImage * toProcess);
	IplImage * postMatchingProcessing(IplImage * toProcess);

private:
	void initWindow();

	std::vector<Processing *> preMatchingProcess;
	std::vector<Processing *> postMatchingProcess;

	Settings * settings;
};


#endif
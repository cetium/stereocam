#pragma once


class Simple3DScene
{
public:
	Simple3DScene();
	~Simple3DScene();

	void init(int w, int h);
	void draw();
	void resize(int w, int h);

	void moveFlat(int direction);
	void rotate(int xdiff, int ydiff);

private:

};


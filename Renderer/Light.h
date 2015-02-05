#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	Light();
	~Light();

	bool Init();
	bool Release();

private:
	double pIntensity;
	double pRadius;
	double pAngle;
	double pDirection;
};

#endif
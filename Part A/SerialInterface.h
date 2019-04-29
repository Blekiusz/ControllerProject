#pragma once

#include <serial\serial.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class SerialInterface
{
public:
	SerialInterface();
	~SerialInterface();

	bool connect = false;

	void send(std::string msg);
	float getPositionX();
	float getPositionY();
	bool getTrigger();
	bool getIR();

	void close();



private:
	serial::Serial* mySerial;

	float xposition = 0;
	float yposition = 0;
	bool trigger = false;
	bool reload = true;

	int button1;
	int button2;
};
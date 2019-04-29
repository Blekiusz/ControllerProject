#include "stdafx.h"
#include "SerialInterface.h"
#include <iostream>
#include <Windows.h>
#include <string>

SerialInterface::SerialInterface()
{
	std::vector <serial::PortInfo> devicesFound = serial::list_ports();

	std::vector <serial::PortInfo>::iterator iter = devicesFound.begin();

	while (iter != devicesFound.end())
	{
		serial::PortInfo device = *iter++;
		std::string port = "COM3";

		try {
			mySerial = new serial::Serial(port, 9600, serial::Timeout::simpleTimeout(250));

			if (mySerial->isOpen())
			{
				std::cout << "Connection success: " << port << "\n";
				connect = true;

				break;
			}
		}
		catch (std::exception &e) {

		}
	}
}
SerialInterface::~SerialInterface()
{

}

void SerialInterface::send(std::string msg)
{
	if (connect)
	{
		mySerial->write(msg);
	}
}

float SerialInterface::getPositionX()
{
	if (connect)
	{
		mySerial->write("X");
		std::string result = mySerial->readline();
		float x = std::stof(result);
		return -x;
	}
}

float SerialInterface::getPositionY()
{
	if (connect)
	{
		mySerial->write("Y");
		std::string result = mySerial->readline();
		float y = std::stof(result);
		return y;
	}
}

bool SerialInterface::getTrigger() {
	if (connect)
	{
		mySerial->write("B");
		std::string result = mySerial->readline();
		std::cout << result << std::endl;
		if (result.find('1')) return true;
		else return false;
	}
}

bool SerialInterface::getIR() {
	if (connect)
	{
		mySerial->write("R");
		std::string result = mySerial->readline();
		if (result.find('1')) return true;
		else return false;
	}
}

void SerialInterface::close()
{
	mySerial->flush();
	mySerial->close();
}
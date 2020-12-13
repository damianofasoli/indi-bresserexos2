/*
 * Main.cpp
 * 
 * Copyright 2020 Kevin Krüger <kkevin@gmx.net>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <iostream>
#include "Config.hpp"
#include "SerialDeviceControl/SerialCommand.hpp"

int main(int argc, char **argv)
{
	//DriverTest
	std::cout << argv[0] << " Version " << BresserExosIIGoToDriverForIndi_VERSION_MAJOR << "." << BresserExosIIGoToDriverForIndi_VERSION_MINOR << std::endl;
	
	if(!SerialDeviceControl::SerialCommand::GetStopMotionCommandMessage(nullptr))
	{
		std::cout << "Get Stop Motion Command fail -> OK!" << std::endl;
	}
	
	if(!SerialDeviceControl::SerialCommand::GetParkCommandMessage(nullptr))
	{
		std::cout << "Get Park Command fail -> OK!" << std::endl;
	}

	if(!SerialDeviceControl::SerialCommand::GetGotoCommandMessage(nullptr, 0.0,0.0))
	{
		std::cout << "Get GoTo Command fail -> OK!" << std::endl;
	}
	
	if(!SerialDeviceControl::SerialCommand::GetSetSiteLocationCommandMessage(nullptr, 0.0,0.0))
	{
		std::cout << "Get Set Site Location Command fail -> OK!" << std::endl;
	}	
	
	if(!SerialDeviceControl::SerialCommand::GetSetDateTimeCommandMessage(nullptr,2020,12,12,12,12,00))
	{
		std::cout << "Get Set Date Time Command fail -> OK!" << std::endl;
	}
	
	return 0;
}


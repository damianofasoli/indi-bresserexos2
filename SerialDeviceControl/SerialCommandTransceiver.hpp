/*
 * SerialCommandTransceiver.hpp
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
 
#ifndef _SERIALCOMMANDTRANSCEIVER_H_INCLUDED_
#define _SERIALCOMMANDTRANSCEIVER_H_INCLUDED_

#include <cstdint>
#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <thread>
#include <chrono>
#include <algorithm>
#include "Config.hpp"

#include "INotifyPointingCoordinatesReceived.hpp"
#include "ISerialInterface.hpp"
#include "CriticalData.hpp"
#include "SerialCommand.hpp"

namespace SerialDeviceControl
{
	//Simple data structure for a coordinate pair.
	struct EquatorialCoordinates
	{
		//The time stamp when this coordinates where received.
		std::chrono::time_point<std::chrono::system_clock> TimeStamp;
		
		//decimal value of the right ascension.
		float RightAscension;
		
		//decimal value of the declination.
		float Declination;
	};
	
	template<class InterfaceType, class CallbackType>
	class SerialCommandTransceiver
	{
		public:
			
			SerialCommandTransceiver(InterfaceType& interfaceImplementation, CallbackType& dataReceivedCallback) :
			mDataReceivedCallback(dataReceivedCallback),
			mInterfaceImplementation(interfaceImplementation),
			mThreadRunning(false),
			mSerialReaderThread()
			{
				SerialCommand::PushHeader(mMessageHeader);
			}
			
			virtual ~SerialCommandTransceiver()
			{
				bool threadRunning = mThreadRunning.Get();
				
				if(threadRunning)
				{
					Stop();
				}
			}
			
			void Start()
			{
				mSerialReaderThread = std::thread(&SerialCommandTransceiver::SerialReaderThreadFunction,this);
			}
			
			void SendMessageBuffer(uint8_t* buffer, size_t offset, size_t length)
			{
			
			}
			
			void Stop()
			{
				mThreadRunning.Set(false);
				
				mSerialReaderThread.join();
			}
			
		private:
		
			InterfaceType& mInterfaceImplementation;
			
			CallbackType& mDataReceivedCallback;
			
			CriticalData<bool> mThreadRunning;
			
			//std::vector<EquatorialCoordinates> mReceivedMessages;
			
			std::vector<uint8_t> mSerialReceiverBuffer;
			std::vector<uint8_t> mMessageHeader;
			std::thread mSerialReaderThread; 
			
			void TryParseMessagesFromBuffer()
			{
				if(mSerialReceiverBuffer.size()>0)
				{
					std::vector<uint8_t>::iterator startPosition = std::search(mSerialReceiverBuffer.begin(),mSerialReceiverBuffer.end(),mMessageHeader.begin(),mMessageHeader.end());
					
					std::vector<uint8_t>::iterator endPosition = startPosition + 13;
					
					if(startPosition != mSerialReceiverBuffer.end() && endPosition != mSerialReceiverBuffer.end())
					{
						std::cout << "found sequence!" << std::endl;
					}
				}
			}
			
			void SerialReaderThreadFunction()
			{
				std::cout << "thread started!" << std::endl;
				bool running = mThreadRunning.Get();
				
				mInterfaceImplementation.Open();
				
				if(running==false)
				{
					mThreadRunning.Set(true);
					
					do
					{
						//controller sends status messages about every second so wait a bit and 
						std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
						
						size_t bufferContent = mInterfaceImplementation.BytesToRead();
						int16_t data = -1;
			
						while((data = mInterfaceImplementation.ReadByte())>-1)
						{
							mSerialReceiverBuffer.push_back((uint8_t)data);
						}
						
						TryParseMessagesFromBuffer();
						
						std::cout << "Serial buffer has " << std::dec << mSerialReceiverBuffer.size() << " bytes available" << std::endl;
						
						//Do serial business
						
						running = mThreadRunning.Get();
					}
					while(running == true);
					
					std::cout << "thread stopped!" << std::endl;
				}
				mInterfaceImplementation.Flush();
				mInterfaceImplementation.Close();
			}
	};
}
#endif

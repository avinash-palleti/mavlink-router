// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file vehicle_command_Publisher.cpp
 * This file contains the implementation of the publisher functions.
 *
 * This file was generated by the tool fastcdrgen.
 */


#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>

#include <fastrtps/Domain.h>

#include <fastrtps/utils/eClock.h>

#include "vehicle_command_Publisher.h"


using namespace std;

vehicle_command_Publisher::vehicle_command_Publisher() : mp_participant(nullptr), mp_publisher(nullptr) {}

vehicle_command_Publisher::~vehicle_command_Publisher() {	Domain::removeParticipant(mp_participant);}

bool vehicle_command_Publisher::init()
{
	// Create RTPSParticipant
	
	ParticipantAttributes PParam;
	PParam.rtps.builtin.domainId = 0;
	PParam.rtps.builtin.leaseDuration = c_TimeInfinite;
	PParam.rtps.setName("Participant_publisher");  //You can put here the name you want
	mp_participant = Domain::createParticipant(PParam);
	if(mp_participant == nullptr)
		return false;
	
	//Register the type
	
	Domain::registerType(mp_participant,(TopicDataType*) &myType);
	
	// Create Publisher
	
	PublisherAttributes Wparam;
	Wparam.topic.topicKind = NO_KEY;
	Wparam.topic.topicDataType = myType.getName();  //This type MUST be registered
	Wparam.topic.topicName = "vehicle_command_PubSubTopic";
	mp_publisher = Domain::createPublisher(mp_participant,Wparam,(PublisherListener*)&m_listener);
	if(mp_publisher == nullptr)
		return false;
	std::cout << "Publisher created, waiting for Subscribers." << std::endl;
	return true;
}

void vehicle_command_Publisher::PubListener::onPublicationMatched(Publisher* pub,MatchingInfo& info)
{
	if (info.status == MATCHED_MATCHING)
	{
		n_matched++;
		std::cout << "Publisher matched" << std::endl;
	}
	else
	{
		n_matched--;
		std::cout << "Publisher unmatched" << std::endl;
	}
}

void vehicle_command_Publisher::run()
{
	while(m_listener.n_matched == 0)
	{
		eClock::my_sleep(250); // Sleep 250 ms
	}
	
	
	/* Initialize your structure here */

    int msgsent = 0;
    char ch = 'y';
    bool run = true;
    cout << "Publish one of the following commands: " << endl
         << "r      reset" << endl
         << "b      reset to bootloader" << endl
         << "a      arm" << endl
         << "d      disarm" << endl
         << "q      quit publisher" << endl;
    while(run && std::cin >> ch) {
        vehicle_command_ st { };
        switch (ch) {
        case 'r':
            cout << "sending reset" << endl;
            st.command() = 246;
            st.param1() = 1;
            mp_publisher->write(&st);  ++msgsent;
            break;
        case 'b':
            cout << "sending reset to bootloader" << endl;
            st.command() = 246;
            st.param1() = 3;
            mp_publisher->write(&st);  ++msgsent;
            break;
        case 'a':
            cout << "sending arm " << endl;
            st.command() = 400;
            st.param1() = 1;
            mp_publisher->write(&st);  ++msgsent;
            break;
        case 'd':
            cout << "sending disarm " << endl;
            st.command() = 400;
            st.param1() = 0;
            mp_publisher->write(&st);  ++msgsent;
            break;
        case 'q':
            cout << "Stopping execution " << endl;
            run = false;
            break;
        default:
            cout << "Command " << ch << " not recognized, knows only: [rbadq]:" << endl;
        }
    }
}
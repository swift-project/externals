/**
	@file

	VATSIM C++ Shim Library

	@author Gary Tekulsky 1134195
	@version 0.4.18

	<!-- Copyright © 2012-2014 Gary Tekulsky 1134195. All rights reserved. -->
	@copyright Copyright &copy; 2012-2014 Gary Tekulsky 1134195. All rights reserved.
*/

/*
	-----[LICENSE]-----

	Project: VATSIM C++ Shim Library
	Version: 0.4.18
	Author: Gary Tekulsky 1134195
	Contact: http://forums.VATSIM.net/ucp.php?i=pm&mode=compose&u=1134195
	
	Copyright © 2012-2014 Gary Tekulsky 1134195. All rights reserved.
	The below text embodies the ideas of the permissions that the final legal text should allow.
	
	---[The following is a general idea of what the final copyright should generally embody, it is not in effect at this time]---
	This project is available for free to be used by any legitimate project attempting to connect to the Virtual Air Traffic Simulation Network (VATSIM).
	The underlying code of this project is protected by the VATSIM NDA. This header file is not protected by the VATSIM NDA. It is requested that this
	shim library only be distributed in the unlinked form by the author or a VATSIM representative unless permission is given. When used by a valid
	project for the VATSIM network, a link to an approved download location should be used to refer other developers to it. The accompanying shared library
	(DLL, SO, etc) may be distributed directly with the compiled application software (Pilot Client, ATC Client, etc) without permission so long as it is used
	in a non-malicious way and the application software has been approved for use on the VATSIM network. This library is not to be used commercially.
	This so-called "License" should remain intact in this header file as written but can be excluded from the application software.



	-----[WARNING]-----

	Generally speaking the VATSIM libraries are not thread-safe, nor are they re-entrant.  The program must assure that only one thread is running library code at a time.

	It is legal to call network sending routines from a network callback.  It is not legal to connect or disconnect or create or dispose of network objects from a network
	callback.

	It is not legal to invoke any voice routines from a voice callback.

	It is recommended that one of the following two methods is used to handle calls to the VATSIM library to deal with the threading limitations:
	   1. Always call library methods from the main thread. The libraries functions do not block, so the program simply needs to call them with adequate frequency
	   2. Always call library methods from a single worker thread and use some kind of synchronizing structure, such as a message queue, to send commands to the library worker
	       thread. Additional message queues may be required for callbacks.

	It is the clients responsibility to call destruction/deallocation routines on all objects/handles.

	This library does not provide any sort of copy/clone functionality.

	-----[WARNING]-----
	Failure to destroy Cvatlib_Network or Cvatlib_Voice_Simple objects may cause resources to not be released. Ensure that all exit paths properly destroy these objects
	using obj->Destroy() so that resources are freed up.

	-----[PROGRAMMING NOTES]-----
	Objects are created using the x::Create() static methods. Objects are destroyed using the obj->Destroy() methods.

	-----[BUILD NOTES]-----
	The VATSIM C++ Shim Library requires a C++11 compatible compiler.

	Tested compilers:
	->Microsoft Windows: Microsoft Visual Studio 2010
	->Apple OS X: Not tested yet
	->GNU/Linux: Not tested yet
*/

/// @cond

#ifndef VATLIB_HEADER_H
#define VATLIB_HEADER_H

/****************************************
 * OPERATING SYSTEMS
 ***************************************/

#if defined(_WIN32) || defined(__WIN32__)
#   define _OS_WIN32
#   define _OS_WIN
#endif
#if defined(_WIN64) || defined(__WIN64__)
#   define _OS_WIN64
#   define _OS_WIN
#endif
#if defined(__APPLE__) || defined(__MACOSX__)
#   define _OS_MACOSX
#endif
#if defined(unix) || defined(__unix) || defined(__unix__)
#   define _OS_UNIX
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
#   define _OS_LINUX
#endif

/****************************************
 * COMPILERS
 ***************************************/

#if defined(__GNUC__)
#   define _CC_GNU
#endif

#if defined(__MINGW32__)
#   define _CC_MINGW
#endif

#if defined(_MSC_VER)
#   define _CC_MSVC
#   if _MSC_VER >= 1700
#       define _CC_MSVC11
#   elif _MSC_VER >= 1600
#       define _CC_MSVC10
#   elif _MSC_VER >= 1500
#       define _CC_MSVC9
#   elif _MSC_VER >= 1400
#       define _CC_MSVC8
#   endif
#endif


#if defined(_OS_WIN)
#   ifdef VATLIB_EXPORTS
#      define VATLIB_API __declspec(dllexport)
#   elif VATLIB_STATIC
#      define VATLIB_API
#   else
#      define VATLIB_API __declspec(dllimport)
#   endif
#else
#   define VATLIB_API
#endif

#include <deque>
#include <exception>
#include <stdint.h>

using namespace std;

#define EXC extern "C"

#define VM virtual

#define PVM  = 0

/// @endcond

/**
	The library name as a string
*/
#define VATLIB_NAME "VATSIM C++ Shim Library"

/**
	The library copyright text as a string (UTF-8)
*/
#define VATLIB_COPYRIGHT_UTF8 "Copyright © 2012-2014 Gary Tekulsky 1134195. All rights reserved."

/**
	The library copyright text as a string (ASCII)
*/
#define VATLIB_COPYRIGHT_ASCII "Copyright (C) 2012-2014 Gary Tekulsky 1134195. All rights reserved."

/**
	Major component of the library version
*/
#define VATLIB_VERSION_MAJOR 0

/**
	Minor component of the library version
*/
#define VATLIB_VERSION_MINOR 4

/**
	Micro component of the library version
*/
#define VATLIB_VERSION_MICRO 18

/**
	The library version as a string
*/
#define VATLIB_VERSION "0.4.18"

/**
	Guaranteed 64-bit int definition
*/
#define INT64 int64_t

/**
	Guaranteed 64-bit unsigned int definition
*/
#define UINT64 uint64_t

/**
	Guaranteed 32-bit int definition
*/
#define INT int32_t

/**
	Guaranteed 32-bit unsigned int definition
*/
#define UINT uint32_t

/**
	Guaranteed 16-bit int (short) definition
*/
#define SHORT int16_t

/**
	Guaranteed 16-bit unsigned int (unsigned short) definition
*/
#define USHORT uint16_t

/// @cond

#ifdef _OS_WIN
#   define CALL __stdcall
#else
#   define CALL 
#endif

#undef NULL
#define NULL nullptr 

/// @endcond

/**
	@class Cvatlib_Network

	@brief Contains methods and variables for the network library
*/
class VATLIB_API Cvatlib_Network {
public:
	/* Enums */

	/**
		@name Error Codes
	*/
	///@{
	/**
		Error Codes
	*/
	enum error_enum {
		/**
			No error
		*/
		error_Ok = 0,
		/**
			The selected callsign is already being used by a connected user (Fatal)
		*/
		error_CallsignTaken = 1,
		/**
			The selected callsign is invalid (Fatal)
		*/
		error_CallsignInvalid = 2,
		/**
			Already registered (Fatal)

			@todo find the real meaning
		*/
		error_Registered = 3,
		/**
			Syntax error in packet
		*/
		error_Syntax = 4,
		/**
			Invalid source (sender) callsign in packet
		*/
		error_SourceInvalid = 5,
		/**
			Invalid CID or Password (Fatal)
		*/
		error_CIDPasswdInvalid = 6,
		/**
			No such callsign on the network
		*/
		error_CallsignNotExists = 7,
		/**
			No flightplan available for that callsign
		*/
		error_NoFP = 8,
		/**
			The requested weather profile does not exist
		*/
		error_NoWeather = 9,
		/**
			Invalid protocol version (Fatal)
		*/
		error_ProtoVersion = 10,
		/**
			Requested level too high (eg. login as S2 when user is a S1) (Fatal)
		*/
		error_LevelTooHigh = 11,
		/**
			The server is full (Fatal)
		*/
		error_ServerFull = 12,
		/**
			The CID is suspended (Fatal)
		*/
		error_CIDSuspended = 13,
		/**
			Invalid control

			@todo find the real meaning
		*/
		error_InvalidControl = 14,
		/**
			Invalid position for rating (Fatal)
		*/
		error_InvalidPosition = 15,
		/**
			This software is not authorized (Fatal)
		*/
		error_SoftwareNotAuthorized = 16
	};
	/**
		Represents an @ref error_enum value
	*/
	typedef INT error;
	///@}

	/**
		@name Connection Types
		
		The type of client the user is connecting as
	*/
	///@{
	/**
		Connection Types

		The type of client the user is connecting as
	*/
	enum connectionType_enum {
		/**
			Unknown type. Should not occur
		*/
		connectionType_Unknown = 0,
		/**
			Pilot client
		*/
		connectionType_Pilot = 1,
		/**
			ATC client
		*/
		connectionType_ATC = 2,
		/**
			AVC

			@deprecated This is a legacy value for the Advanced Voice Client and should not be used
		*/
		connectionType_AVC = 3
	};
	/**
		Represents a @ref connectionType_enum value
	*/
	typedef INT connectionType;
	///@}

	/**
		@name Connection Statuses
	*/
	///@{
	/**
		Connection Statuses
	*/
	enum connStatus_enum {
		/**
			Not connected
		*/
		connStatus_Idle = 1,
		/**
			Attempting to connect
		*/
		connStatus_Connecting = 2,
		/**
			Connected. Login may be performed
		*/
		connStatus_Connected = 3,
		/**
			Disconnected from server
		*/
		connStatus_Disconnected = 4,
		/**
			An error was thrown
		*/
		connStatus_Error = 5,
		/**
			The connection to the remote server failed
		*/
		connStatus_ConnectionFailed = 6,
		/**
			The connection to the remote server was lost
		*/
		connStatus_ConnectionLost = 7
	};
	/**
		Represents a @ref connStatus_enum value
	*/
	typedef INT connStatus;
	///@}

	/**
		@name Transponder States

		@note The xpndrMode type is meant to hold the character code number of the chars in the enum
	*/
	///@{
	
	/**
		Transponder States

		@note The xpndrMode type is meant to hold the character code number of the chars in the enum
	*/
	enum xpndrMode_enum {
		/**
			Off/Standby mode
		*/
		xpndrMode_Standby = 'S',
		/**
			Normal/Charlie
		*/
		xpndrMode_Normal = 'N',
		/**
			Normal/Charlie and ident
		*/
		xpndrMode_Ident = 'Y'
	};
	/**
		Represents a @ref xpndrMode_enum value
	*/
	typedef INT xpndrMode;
	///@}

	/**
		@name ATC Ratings
	*/
	///@{
	/**
		ATC Ratings
	*/
	enum atcRating_enum {
		/**
			Observer
		*/
		atcRating_Obs = 1,
		/**
			Student 1
		*/
		atcRating_S1 = 2,
		/**
			Student 2
		*/
		atcRating_S2 = 3,
		/**
			Student 3
		*/
		atcRating_S3 = 4,
		/**
			Controller 1
		*/
		atcRating_C1 = 5,
		/**
			Controller 2
		*/
		atcRating_C2 = 6,
		/**
			Controller 3
		*/
		atcRating_C3 = 7,
		/**
			Instructor 1
		*/
		atcRating_I1 = 8,
		/**
			Instructor 2
		*/
		atcRating_I2 = 9,
		/**
			Instructor 3
		*/
		atcRating_I3 = 10,
		/**
			Supervisor
		*/
		atcRating_Sup = 11,
		/**
			Administrator
		*/
		atcRating_Adm = 12
	};
	/**
		Represents an @ref atcRating_enum value
	*/
	typedef INT atcRating;
	///@}

	/**
		@name Pilot Ratings
	*/
	///@{
	/**
		Pilot Ratings
	*/
	enum pilotRating_enum {
		/**
			Unknown rating
		*/
		pilotRating_Unknown = 0,
		/**
			Student
		*/
		pilotRating_Student = 1,
		/**
			VFR rating
		*/
		pilotRating_VFR = 2,
		/**
			IFR rating
		*/
		pilotRating_IFR = 3,
		/**
			Pilot rating instructor
		*/
		pilotRating_Instructor = 4,
		/**
			Supervisor
		*/
		pilotRating_Sup = 5
	};
	/**
		Represents a @ref pilotRating_enum value
	*/
	typedef INT pilotRating;
	///@}

	/**
		@name ATC Facility Types
	*/
	///@{
	/**
		ATC Facility Types
	*/
	enum facilityType_enum {
		/**
			Unknown facility type
		*/
		facilityType_Unknown = 0,
		/**
			Flight service station
		*/
		facilityType_FSS = 1,
		/**
			Delivery
		*/
		facilityType_Del = 2,
		/**
			Ground
		*/
		facilityType_Gnd = 3,
		/**
			Tower or ATIS
		*/
		facilityType_TwrAtis = 4,
		/**
			Departure/Approach
		*/
		facilityType_DepApp = 5,
		/**
			Center
		*/
		facilityType_Ctr = 6
	};
	/**
		Represents a @ref facilityType_enum value
	*/
	typedef INT facilityType;
	///@}

	/**
		@name Flight Simulator Types
	*/
	///@{
	/**
		Flight Simulator Types
	*/
	enum simType_enum {
		/**
			Unknown simulator
		*/
		simType_Unknown = 0,
		/**
			MS Flight Simulator 95
		*/
		simType_MSFS95 = 1,
		/**
			MS Flight Simulator 98. Also used currently for FS9/FSX
		*/
		simType_MSFS98 = 2,
		/**
			MS Combat Flight Sim
		*/
		simType_MSCFS = 3,
		/**
			Airline Simulator 2
		*/
		simType_AS2 = 14,
		/**
			Precision Simulator 1
		*/
		simType_PS1 = 15,
		/**
			XPlane
		*/
		simType_XPlane = 11,
	};
	/**
		Represents a @ref simType_enum value
	*/
	typedef INT simType;
	///@}

	/**
		@name Flight Plan Rule Types
	*/
	///@{
	/**
		Flight Plan Rule Types
	*/
	enum fpRuleType_enum {
		/**
			Instrument flight rules
		*/
		fpRuleType_IFR = 'I',
		/**
			Visual flight rules
		*/
		fpRuleType_VFR = 'V',
		/**
			Defense VFR. Not used
		*/
		fpRuleType_DVFR = 'D',
		/**
			Special VFR. Do not file this
		*/
		fpRuleType_SVFR = 'S'
	};
	/**
		Represents a @ref fpRuleType_enum value
	*/
	typedef INT fpRuleType;
	///@}

	/**
		@name Info Query Types
		
		Parameters the user can query the server or other users for
	*/
	///@{
	/**
		Info Query Types

		Parameters the user can query the server or other users for
	*/
	enum infoQuery_enum {
		/**
			Request a pilots flightplan. Pilot replies with flightplan (Only send to pilots)

			Replies are received in @ref onFlightPlanReceivedEvent

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_FP = 0,
		/**
			Request a pilots com1 frequency (Only send to pilots)

			Reply parameters are frequency (eg. 122.8), NULL
		*/
		infoQuery_Freq = 1,
		/**
			Gets user info (Supervisors only)

			Replies are received as a text message

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_UserInfo = 2,
		/**
			Request a controllers text ATIS (Only send to controllers)

			Replies are received as multiple @ref onAtisReplyReceivedEvent
		*/
		infoQuery_ATIS = 3,
		/**
			What server is this client on?

			Reply parameters are HostnameOrIP, NULL

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_Server = 4,
		/**
			What is this clients real name?

			Reply parameters are Name, SectorFile (empty string for pilots)
		*/
		infoQuery_Name = 5,
		/**
			Does this callsign have ATC privileges? (Only send to controllers)

			This info query actually gets sent to the server. If a blank callsign is sent, server assumes client is asking about itself

			Reply parameters are YorN, CallsignChecked (If you sent blank callsign, will be NULL)

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_ATC = 6,
		/**
			What is this clients capabilities?

			Replies are received in @ref onCapabilitiesReplyReceivedEvent

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_Capabilities = 7,
		/**
			What is my public IP? (Only send to server)

			Reply parameters are IP, NULL

			@note The internal library will automatically handle the sending of a reply to this info query
		*/
		infoQuery_IP = 8
	};
	/**
		Represents an @ref infoQuery_enum value
	*/
	typedef INT infoQuery;
	///@}

	/**
		@name ATIS Line Identifiers

		@note The atisLineType type is meant to hold the character code number of the chars in the enum
	*/
	///@{
	/**
		@name ATIS Line Identifiers

		@note The atisLineType type is meant to hold the character code number of the chars in the enum
	*/
	enum atisLineType_enum {
		/**
			Denotes that the ATIS message contains the voice room address
		*/
		atisLineType_VoiceRoom = 'V',
		/**
			Denotes that the ATIS message contains regular text
		*/
		atisLineType_TextMessage = 'T',
		/**
			Denotes that the ATIS message contains the logoff time
		*/
		atisLineType_ZuluLogoff = 'Z',
		/**
			Sent as the last line of an ATIS message and denotes the number of lines in the ATIS message, including this one
		*/
		atisLineType_LineCount = 'E'
	};
	/**
		Represents an @ref atisLineType_enum value
	*/
	typedef INT atisLineType;
	///@}

	/**
		@name Aircraft Engine Types
	*/
	///@{
	/**
		Aircraft Engine Types
	*/
	enum acEngineType_enum {
		/**
			Piston powered airplane
		*/
		acEngineType_Piston = 0,
		/**
			Jet powered airplane
		*/
		acEngineType_Jet = 1,
		/**
			Unknown engine type
		*/
		acEngineType_None = 2,
		/**
			Helicopter
		*/
		acEngineType_Helo = 3
	};
	/**
		Represents an @ref acEngineType_enum value
	*/
	typedef INT acEngineType;
	///@}

	/**
		@name Shared Pilot Properties
		
		Pilot properties shared between ATC clients
	*/
	///@{
	/**
		Shared Pilot Properties

		Pilot properties shared between ATC clients
	*/
	enum ccpPropertyType_enum {
		/**
			Scratchpad in data block
		*/
		ccpPropertyType_Scratchpad = 0,
		/**
			Voice capabilities (voice, text, receive only)
		*/
		ccpPropertyType_VoiceType,
		/**
			Assigned squawk code
		*/
		ccpPropertyType_SquawkCode,
		/**
			Final altitude (obsolete)
		*/
		ccpPropertyType_FinalAltitude,
		/**
			Assigned temporary altitude
		*/
		ccpPropertyType_TempAltitude
	};
	/**
		Represents a @ref ccpPropertyType_enum value
	*/
	typedef INT ccpPropertyType;
	///@}

	/**
		@name Landline Commands
	*/
	///@{
	/**
		Landline Commands
	*/
	enum landlineCommand_enum {
		/**
			Request landline with target client
		*/
		landlineCommand_Request = 0,
		/**
			Approve a landline request
		*/
		landlineCommand_Approve,
		/**
			Reject a landline request
		*/
		landlineCommand_Reject,
		/**
			Terminate the landline
		*/
		landlineCommand_End
	};
	/**
		Represents a @ref landlineCommand_enum value
	*/
	typedef INT landlineCommand;
	///@}

	/**
		@name Landline Types
	*/
	///@{
	/**
		Landline Types
	*/
	enum landlineType_enum {
		/**
			Intercom, key mic to talk
		*/
		landlineType_Intercom = 0,
		/**
			Override, receiver of call has open mic
		*/
		landlineType_Override,
		/**
			Listen to receivers frequency
		*/
		landlineType_Monitor
	};
	/**
		Represents a @ref landlineType_enum value
	*/
	typedef INT landlineType;
	///@}
	
	/**
		@name Pilot Track Commands
	*/
	///@{
	/**
		Pilot Track Commands
	*/
	enum trackCommand_enum {
		/**
			Start a track on the aircraft
		*/
		trackCommand_StartTrack = 0,
		/**
			Drop a track on an aircraft
		*/
		trackCommand_DropTrack,
		/**
			Indicate that the local client own the track on the aircraft
		*/
		trackCommand_IHave,
		/**
			Request for which client owns the track on an aircraft
		*/
		trackCommand_WhoHas,
		/**
			A pointout of an aircraft to another controller
		*/
		trackCommand_Pointout,
		/**
			Make an aircraft appear in another controllers departure list
		*/
		trackCommand_DepartureList
	};
	/**
		Represents a @ref trackCommand_enum value
	*/
	typedef INT trackCommand;
	///@}

	/* Constants */
	// These values are set by the values in the underlying VATSIM library upon running the constructor

	/**
		@name Predefined Frequencies
	*/
	///@{
	/**
		VATSIM universal unicom frequency
	*/
	double freqUnicom;
	/**
		VATSIM universal emergency channel
	*/
	double freqGuard;
	/**
		VATSIM universal ATC chat frequency
	*/
	double freqATCP;
	/**
		VATSIM universal unprimed ATC frequency
	*/
	double freqATCU;
	///@}

	/**
		@name Capabilities

		Capabilities that can be used when running CreateNetworkSession or that can be sent as a reply to an infoQuery_Capabilities
		@n To use these values, append an = and a 1 to them to mark the capability as present. eg. sprintf(result, "%s=1", capability_AtcInfo);
		@n Capabilities should be separated by a colon (:) eg. sprintf(result, "%s=1:%s=1", capability_AtcInfo, capability_ModelDesc);
	*/
	///@{
	/**
		Can accept ATIS responses via client request packets. All clients should set this
	*/
	char* capability_AtcInfo;
	/**
		Can send/receive secondary visibility centerpoints. ATC and server only
	*/
	char* capability_SecPos;
	/**
		Can send/receive modern model description data. All clients should set this
	*/
	char* capability_ModelDesc;
	/**
		Can send/receive inter-facility coordination data. Used by Euroscope
	*/
	char* capability_OnGoingCoord;
	/**
		Can send/receive high-speed position updates. Used by Squawkbox
	*/
	char* capability_InterimPos;
	/**
		Position reports are not forwarded to other clients, only to servers. Essentially, the client does not appear to anyone but supervisors and servers
	*/
	char* capability_Stealth;
	///@}

	/**
		@name Aircraft Info

		Aircraft info for SendPlaneInfo
		@n To use these values, add them to the array of strings followed by their values. eg. keysValues = { acinfo_Equipment, "B739", acinfo_Airline, "AAL", NULL };
		@n This array should be NULL terminated
	*/
	///@{
	/**
		The aircraft ICAO model code
	*/
	char* acinfo_Equipment;
	/**
		The airline ICAO code. Optional
	*/
	char* acinfo_Airline;
	/**
		The aircraft livery. Optional
	*/
	char* acinfo_Livery;
	///@}

	/* Structs */
	/**
		Information for a pilot login
	*/
	typedef	struct {
		/**
			The desired callsign
		*/
		const char* callsign;
		/**
			The users real name and home airport
		*/
		const char* name;
		/**
			The simulator being used
		*/
		simType sim;
		/**
			The users pilot rating
		*/
		pilotRating rating;
	} PilotConnectionInfo;

	/**
		Information for an ATC login
	*/
	typedef	 struct {
		/**
			The desired callsign
		*/
		const char* callsign;
		/**
			The users real name
		*/
		const char* name;
		/**
			The users ATC rating
		*/
		atcRating rating;
	} ATCConnectionInfo;

	/**
		Pilot position update information
	*/
	typedef struct {
		/**
			Latitude in decimal degrees
		*/
		double lat;
		/**
			Longitude in decimal degrees
		*/
		double lon;
		/**
			Altitude in feet MSL
		*/
		INT altTrue;
		/**
			Number to add to altTrue to get pressure altitude in feet MSL
		*/
		INT altAdj;
		/**
			Ground speed in knots
		*/
		INT groundSpeed;
		/**
			Heading clockwise from true north in degrees 0-359
		*/
		double heading;
		/**
			Bank in degrees. Positive for right roll
		*/
		double bank;
		/**
			Pitch in degrees. Positive for pitch up
		*/
		double pitch;
		/**
			Transponder code in decimal 0000-7777, no illegal digits please
		*/
		SHORT xpdrCode;
		/**
			Current transponder mode. This is the character code of the character in the enum
		*/
		xpndrMode xpdrMode;
		/**
			The rating of this aircrafts pilot
		*/
		pilotRating rating;
	} PilotPosUpdate;

	/**
		ATC position update information
	*/
	typedef struct {
		/**
			Primary frequency in kilohertz
		*/
		INT frequency;
		/**
			Current facility type the controller is connected as
		*/
		facilityType facility;
		/**
			Visibility range of the controllers radar in nm
		*/
		INT visibleRange;
		/**
			The controllers VATSIM rating
		*/
		atcRating rating;
		/**
			Latitude of the controllers radar center in decimal degrees
		*/
		double lat;
		/**
			Longitude of the controllers radar center in decimal degrees
		*/
		double lon;
		/**
			Elevation in feet MSL. Not used, use 0
		*/
		INT elevation;
	} ATCPosUpdate;

	/**
		Flightplan information
	*/
	typedef struct {
		/**
			Flight rules for this flight plan
		*/
		fpRuleType fpRules;
		/**
			The full aircraft type code in ICAO form. eg. T/B734/F
		*/
		const char* acType;
		/**
			True air speed in cruise in knots
		*/
		INT trueCruiseSpeed;
		/**
			Departure airport ICAO code
		*/
		const char* depApt;
		/**
			Planned departure time in Zulu (UTC) time in 24 hour format. eg. 1300 for 1:00 PM UTC
		*/
		INT depTimePlanned;
		/**
			Actual departure time in Zulu (UTC) time in 24 hour format. eg. 1300 for 1:00 PM UTC
		*/
		INT depTimeActual;
		/**
			Cruise altitude in FL or feet form. eg. FL250 or 15000
		*/
		const char* cruiseAlt;
		/**
			Destination airport ICAO code
		*/
		const char* destApt;
		/**
			Hours component of expected enroute time
		*/
		INT enrouteHrs;
		/**
			Minutes component of expected enroute time
		*/
		INT enrouteMins;
		/**
			Hours component of available flight time with fuel on board
		*/
		INT fuelHrs;
		/**
			Minutes component of available flight time with fuel on board
		*/
		INT fuelMins;
		/**
			Alternate destination airport ICAO code
		*/
		const char* altApt;
		/**
			Flight plan remarks. No colons, max 100 chars
		*/
		const char* remarks;
		/**
			Capitalized, dot seperated flight plan route. No colons, max 100 chars
		*/
		const char* route;
	} FlightPlan;

	/**
		A temperature layer for a weather profile
	*/
	typedef struct {
		/**
			Top of the layer in feet MSL
		*/
		INT ceiling;
		/**
			Temperature in degrees celsius
		*/
		INT temp;
	} TempLayer;

	/**
		A wind layer for a weather profile
	*/
	typedef struct {
		/**
			Top of the wind layer in feet MSL
		*/
		INT ceiling;
		/**
			Bottom of the wind layer in feet MSL
		*/
		INT floor;
		/**
			Direction of the wind in degrees from true north 0-359
		*/
		INT direction;
		/**
			Wind speed in knots
		*/
		INT speed;
		/**
			If 1, wind is gusting. If 0, wind is not gusting
		*/
		INT gusting;
		/**
			Turbulence measure on  scale of 0-255. 0 is no turbulence while 255 is extreme turbulence
		*/
		INT turbulence;
	} WindLayer;

	/**
		A cloud layer for a weather profile
	*/
	typedef struct {
		/**
			Top of the layer in feet MSL
		*/
		INT ceiling;
		/**
			Bottom of the layer in feet MSL
		*/
		INT floor;
		/**
			Cloud cover from 0-8. 8 is overcast
		*/
		INT coverage;
		/**
			If 1, icing is present
		*/
		INT icing;
		/**
			Turbulence measure on  scale of 0-255. 0 is no turbulence while 255 is extreme turbulence
		*/
		INT turbulence;
	} CloudLayer;

	/**
		A storm layer for a weather profile
	*/
	typedef struct {
		/**
			Top of the layer in feet MSL
		*/
		INT ceiling;
		/**
			Bottom of the layer in feet MSL
		*/
		INT floor;
		/**
			Cloud cover from 0-8. 8 is overcast
		*/
		INT coverage;
		/**
			If 1, icing is present
		*/
		INT icing;
		/**
			Turbulence measure on  scale of 0-255. 0 is no turbulence while 255 is extreme turbulence
		*/
		INT turbulence;
	} StormLayer;

	/* Callback Definitions */
	/**
		@name Setup and Handling Callbacks
	*/
	///@{
	/**
		A callback for when the connection status of the network changes

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] oldStatus Passes in the previous connection status
		@param[in] newStatus Passes in the new connection status
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onConnectionStatusChangedEvent) (Cvatlib_Network* obj, connStatus oldStatus, connStatus newStatus, void* cbVar);

	/**
		A callback for when the server returns an error

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] type Passes in the error type from the error struct
		@param[in] message Passes in the error message returned by the server
		@param[in] errorData Passes in extra data related to certain errors. eg. On a flight plan not found error, the pilot whose flight plan wasn't found
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onErrorReceivedEvent) (Cvatlib_Network* obj, error type, const char* message, const char* errorData, void* cbVar);
	///@}

	/**
		@name Basic Communications Callbacks
	*/
	///@{
	/**
		A callback for when a private text message is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] from Passes in the callsign which sent the message
		@param[in] to Passes in the callsign the message is addressed to
		@param[in] message Passes in the content of the text message
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onTextMessageReceivedEvent) (Cvatlib_Network* obj, const char* from, const char* to, const char* message, void* cbVar);

	/**
		A callback for when a text message over a radio frequency is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] from Passes in the callsign which sent the message
		@param[in] numFreq Passes in the number of frequencies the message was sent on
		@param[in] freqList Passes in an array of integers containing the frequencies the message was sent on. Frequencies are passed in as the full frequency without decimal.
			eg. frequency 122.800 would be passed in as 122800
		@param[in] message Passes in the content of the text message
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onRadioMessageReceivedEvent) (Cvatlib_Network* obj, const char* from, INT numFreq, INT* freqList, const char* message, void* cbVar);

	/**
		A callback for when an infoQuery request is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the client requesting the data
		@param[in] type Passes in the infoQuery type of the request
		@param[in] data Passes in any data required for the request
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note Certain requests are handled automatically by the library in the background. Those requests will not trigger this callback
	*/
	typedef void (*onInfoQueryRequestReceivedEvent) (Cvatlib_Network* obj, const char* callsign, infoQuery type, const char* data, void* cbVar);

	/**
		A callback for when an infoQuery reply is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the client that sent the reply
		@param[in] type Passes in the infoQuery type of the reply
		@param[in] data Passes in data from the reply
		@param[in] data2 Passes in secondary data from the reply. Used for infoQuery_ATC & infoQuery_Name requests only
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note ATIS and Capabilities replies have their own callbacks
	*/
	typedef void (*onInfoQueryReplyReceivedEvent) (Cvatlib_Network* obj, const char* callsign, infoQuery type, const char* data, const char* data2, void* cbVar);

	/**
		A callback for when a capabilities reply is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the client that sent the reply
		@param[in] keysValues Passes in the capabilities of the remote client as an array in the format { "capability", "1or0", ..., NULL }
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note keysValues is NULL terminated
	*/
	typedef void (*onCapabilitiesReplyReceivedEvent) (Cvatlib_Network* obj, const char* callsign, const char** keysValues, void* cbVar);

	/**
		A callback for when an ATIS reply is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the client that sent the reply
		@param[in] type Passes in the atisLineType for this reply
		@param[in] data Passes in data from the reply
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note This is called once for each ATIS line
	*/
	typedef void (*onAtisReplyReceivedEvent) (Cvatlib_Network* obj, const char* callsign, atisLineType type, const char* data, void* cbVar);

	/**
		A callback for when the user receive a flight plan

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the pilot whose flightplan has been updated
		@param[in] fp Passes in a FlightPlan struct with the new/updated flight plan information
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onFlightPlanReceivedEvent) (Cvatlib_Network* obj, const char* callsign, FlightPlan fp, void* cbVar);

	/**
		A callback for when pilot info is requested

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the requestor
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onPilotInfoRequestReceivedEvent) (Cvatlib_Network* obj, const char* callsign, void* cbVar);

	/**
		A callback for when pilot info is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the pilot whose info has been sent
		@param[in] keysValues Passes in the information of the remote pilot as an array in the format { acinfo_Equipment, "B739", ..., NULL }
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note keysValues is NULL terminated
	*/
	typedef void (*onPilotInfoReceivedEvent) (Cvatlib_Network* obj, const char* callsign, const char** keysValues, void* cbVar);

	/**
		A callback for when a custom pilot packet is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the sender
		@param[in] packetId Passes in an alpha-numeric string used by the client program to identify the packets purpose
		@param[in] data Passes in an array of data to be used by the client
		@param[in] dataSize Passes in the number of elements in the array
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onCustomPilotPacketReceivedEvent) (Cvatlib_Network* obj, const char* callsign, const char* packetId, const char** data, INT dataSize, void* cbVar);
	///@}

	/**
		@name ATC Callbacks
	*/
	///@{
	/**
		A callback for when a controller disconnects from the network

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who disconnected
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onControllerDisconnectedEvent) (Cvatlib_Network* obj, const char* callsign, void* cbVar);

	/**
		A callback for when an ATC position update comes in

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller whose position has been updated
		@param[in] pos Passes in an ATCPosUpdate struct with the new data
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onAtcPositionUpdateEvent) (Cvatlib_Network* obj, const char* callsign, ATCPosUpdate pos, void* cbVar);

	/**
		A callback for when the user receive a request to handoff an aircraft to the user

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcCallsign Passes in the callsign of the controller who wishes to handoff the aircraft to the user
		@param[in] acCallsign Passes in the callsign of the aircraft the controller wishes to handoff to the user
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onHandoffRequestReceivedEvent) (Cvatlib_Network* obj, const char* atcCallsign, const char* acCallsign, void* cbVar);

	/**
		A callback for when another controller has accepted a handoff, not neccessarily from the user

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcFromCallsign Passes in the callsign of the controller who handed off the aircraft
		@param[in] atcToCallsign Passes in the callsign of the controller who has accepted handoff of the aircraft
		@param[in] acCallsign Passes in the callsign of the aircraft that was handed off
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onHandoffAcceptedEvent) (Cvatlib_Network* obj, const char* atcFromCallsign, const char* atcToCallsign, const char* acCallsign, void* cbVar);

	/**
		A callback for when a handoff request from/to the user was canceled/rejected

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcCallsign Passes in the callsign of the other controller involved in the handoff
		@param[in] acCallsign Passes in the callsign of the aircraft that is no longer being handed off
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onHandoffCancelledRejectedEvent) (Cvatlib_Network* obj, const char* atcCallsign, const char* acCallsign, void* cbVar);

	/**
		A callback for when a landline command is received from another ATC

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who is sending the info
		@param[in] cmd Passes in the landlineCommand that has been sent
		@param[in] type Passes in the landlineType associated with the command
		@param[in] ip Passes in the external IP of the remote client
		@param[in] port Passes in the voice port of the remote client. Usually 3290
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onLandlineCommandReceivedEvent) (Cvatlib_Network* obj, const char* callsign, landlineCommand cmd, landlineType type, const char* ip, USHORT port,
		void* cbVar);

	/**
		A callback for when a tracking command is received from another ATC

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcCallsign Passes in the callsign of the controller who is sending the tracking command
		@param[in] acCallsign Passes in the callsign of the pilot whose is the subject of the tracking command
		@param[in] cmd Passes in the trackCommand that was sent
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onTrackingCommandReceivedEvent) (Cvatlib_Network* obj, const char* atcCallsign, const char* acCallsign, trackCommand cmd, void* cbVar);

	/**
		A callback for when another ATC requests or cancels a request for a break

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who is sending the break command
		@param[in] wantsBreak Passes in if the controller is requesting a break or cancelling the request
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onBreakCommandReceivedEvent) (Cvatlib_Network* obj, const char* callsign, bool wantsBreak, void* cbVar);

	/**
		A callback for when a shared state is received from another ATC

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcCallsign Passes in the callsign of the controller who is sending the info
		@param[in] acCallsign Passes in the callsign of the pilot whose info has been sent
		@param[in] prop Passes in the ccpPropertyType of the info sent
		@param[in] data Passes in the new value of the property
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onSharedStateReceivedEvent) (Cvatlib_Network* obj, const char* atcCallsign, const char* acCallsign, ccpPropertyType prop, const char* data, void* cbVar);

	/**
		A callback for when another ATC client queries the local client to see if it is a modern client

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who is sending the query
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note A reply is automatically sent by the underlying library stating that the local client is a modern client, this callback exists only as a notification
			that the local client was queried
	*/
	typedef void (*onSharedStateIDRequestReceivedEvent) (Cvatlib_Network* obj, const char* callsign, void* cbVar);

	/**
		A callback for when another ATC client replies saying that they are a modern client

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who is replying the query
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note There is no callback for if a client failed to send this reply, which would signal that they are not a modern client
	*/
	typedef void (*onSharedStateIDReplyReceivedEvent) (Cvatlib_Network* obj, const char* callsign, void* cbVar);

	/**
		A callback for when another ATC client pushes a strip to the local client

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] atcCallsign Passes in the callsign of the controller who is pushing the strip
		@param[in] acCallsign Passes in the callsign of the aircraft whose strip is being pushed
		@param[in] type Passes in the flight strip format identifier for the annotations
		@param[in] annotations Passes in the array of annotations on the strip
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note annotations is NULL terminated
	*/
	typedef void (*onPushStripReceivedEvent) (Cvatlib_Network* obj, const char* atcCallsign, const char* acCallsign, INT type, const char** annotations, void* cbVar);

	/**
		A callback for when another controller requests or cancels a request for help

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the controller who is requesting help
		@param[in] wantsHelp Passes in if the controller wants help, false means a previous help request is being canceled
		@param[in] message Passes in the message stating what the controller needs help with, if wantsHelp is true
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onHelpRequestReceivedEvent) (Cvatlib_Network* obj, const char* callsign, bool wantsHelp, const char* message, void* cbVar);
	///@}

	/**
		@name Pilot Callbacks
	*/
	///@{
	/**
		A callback for when a pilot disconnects from the network

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the pilot who disconnected
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onPilotDisconnectedEvent) (Cvatlib_Network* obj, const char* callsign, void* cbVar);

	/**
		A callback for when a pilot position update comes in

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the pilot whose position has been updated
		@param[in] pos Passes in a PilotPosUpdate struct with the new data
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onPilotPositionUpdateEvent) (Cvatlib_Network* obj, const char* callsign, PilotPosUpdate pos, void* cbVar);

	/**
		A callback for when an interim (high speed) pilot position update comes in

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the pilot whose position has been updated
		@param[in] pos Passes in a PilotPosUpdate struct with the new data
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note The PilotPosUpdate will only contain valid values for latitude, longitude, true altitude, pitch, bank, and heading
		@note This callback will also be triggered by Squawkbox high speed updates
	*/
	typedef void (*onInterimPilotPositionUpdateEvent) (Cvatlib_Network* obj, const char* callsign, PilotPosUpdate pos, void* cbVar);
	///@}

	/**
		@name Administrative Callbacks
	*/
	///@{
	/**
		A callback for when the user are kicked from the network

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] reason Passes in the reason provided by the supervisor for kicking the user
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note When this event is triggered, shimlib automatically disconnects the client
	*/
	typedef void (*onKickedEvent) (Cvatlib_Network* obj, const char* reason, void* cbVar);

	/**
		A callback for when the user receive a reply to a ping

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] callsign Passes in the callsign of the client who has responded to a ping
		@param[in] elapsedTime Passes in the amount of time, in seconds, that it took to receive the reply
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onPongEvent) (Cvatlib_Network* obj, const char* callsign, INT elapsedTime, void* cbVar);
	///@}

	/**
		@name Weather Callbacks
	*/
	///@{
	/**
		A callback for when METAR data is received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] data Passes in the content of the METAR
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onMetarReceivedEvent) (Cvatlib_Network* obj, const char* data, void* cbVar);

	/**
		A callback for when the temperature layers are received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] layers Passes in the temperature layers
		@param[in] pressure Passes in the barometric pressure in hundredths of inches of mercury. eg. 2992
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onTemperatureDataReceivedEvent) (Cvatlib_Network* obj, TempLayer layers[4], INT pressure, void* cbVar);

	/**
		A callback for when the wind layers are received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] layers Passes in the wind layers
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onWindDataReceivedEvent) (Cvatlib_Network* obj, WindLayer layers[4], void* cbVar);

	/**
		A callback for when the cloud layers are received

		@param[in] obj Passes in a reference to the Cvatlib_Network object to which it is registered
		@param[in] cLayers Passes in the cloud layers
		@param[in] sLayer Passes in the storm layer
		@param[in] visibility Passes in the visibility in statute miles
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onCloudDataReceivedEvent) (Cvatlib_Network* obj, CloudLayer cLayers[2], StormLayer sLayer, float visibility, void* cbVar);
	///@}

	/* Methods */

	/**
		Contructor
	*/
	static Cvatlib_Network* CALL Create();

	/**
		Destructor
	*/
	VM void CALL Destroy()PVM;

	/**
		Default Destructor

		@deprecated Use the @ref Cvatlib_Network::Destroy method to delete a Cvatlib_Network object
	*/
	VM ~Cvatlib_Network()PVM;

	/*
		******************************
		* SETUP AND HANDLING METHODS *
		******************************
	*/
	/**
		@name Setup and Handling Methods
	*/
	///@{

	/**
		Creates a new network session

		@param[in] versionStr The name and version string sent to other clients who query the clients name eg. MyClient 1.0
		@param[in] versionMaj The major version of the client eg. the 1 in 1.0
		@param[in] versionMin The minor version of the client eg. the 0 in 1.0
		@param[in] hostAppName The name of the host application eg. ???MSFS
		@param[in] publicClientID The public client ID number issued by VATSIM
		@param[in] privateClientKey The private authentication key issued by VATSIM
		@param[in] clientCaps A string describing the clients capabilities separated by colons. eg. ATCINFO=1:SECPOS=1

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws NetworkSessionExistsException Thrown if a network session already exists
	*/
	VM void CALL CreateNetworkSession(const char* versionStr, INT versionMaj, INT versionMin, const char* hostAppName, SHORT publicClientID, const char* privateClientKey,
		const char* clientCaps)PVM;

	/**
		Destroys the network session. Only do this if the client is disconnected from the network and the session is no longer needed

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created

		@note Once this is called, the Cvatlib_Network object is no longer valid and should be destroyed
	*/
	VM void CALL DestroyNetworkSession()PVM;

	/**
		Prepares the required information that will be used to login to the server as a pilot. This does NOT actually do the login  or connect and should be called
		before connecting

		@param[in] server The IP or hostname of the VATSIM server to connect to
		@param[in] port The port to connect to the server on. Use 6809
		@param[in] vatsimID The VATSIM CID of the user
		@param[in] password The password of the user
		@param[in] pConnInfo A filled in PilotConnectionInfo struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL SetPilotLoginInfo(const char* server, USHORT port, const char* vatsimID, const char* password, PilotConnectionInfo pConnInfo)PVM;

	/**
		Prepares the required information that will be used to login to the server as a controller. This does NOT actually do the login  or connect and should be called
		before connecting

		@param[in] server The IP or hostname of the VATSIM server to connect to
		@param[in] port The port to connect to the server on. Use 6809
		@param[in] vatsimID The VATSIM CID of the user
		@param[in] password The password of the user
		@param[in] aConnInfo A filled in ATCConnectionInfo struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL SetATCLoginInfo(const char* server, USHORT port, const char* vatsimID, const char* password, ATCConnectionInfo aConnInfo)PVM;

	/**
		Attempts to connect to the server and start the authentication/login process. This method should not be invoked unless login information has been specified by
		using obj->SetPilotLoginInfo or obj->SetATCLoginInfo

		@return true if the basic socket connection was established, otherwise false. False could mean the socket is already connected/connecting or that a DNS lookup on the
		server failed

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM bool CALL ConnectAndLogon()PVM;

	/**
		Attempts to logoff from the network

		@param[in] timeoutSeconds Number of seconds to wait before forcefully dropping the connection. Use 0 for no timeout

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection

		@note This method does not block
	*/
	VM void CALL LogoffAndDisconnect(INT timeoutSeconds)PVM;

	/**
		Calling this method makes the library do all of the work. If this method is not called nothing will happen. The program should call this at least every 100ms

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created

		@note This should be called a few times after disconnecting to allow cleanup to occur
	*/
	VM void CALL DoProcessing()PVM;

	/**
		Gets the error code from the network socket as a string
		
		@return A c string containing either EOK, a standard Linux errno code for a specific set of common network errors, or EUNKWN \<int errorCode>

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created

		@note EUNKWN is used for less common and more fatal error codes. An output might look like "EUNKWN 421". A developer could then look up "421" in error code headers
		@note The supported error strings are ECONNABORTED, ECONNREFUSED, ECONNRESET, EHOSTDOWN, EHOSTUNREACH, ENETDOWN, ENETRESET, ENETUNREACH, ENOTCONN, and ETIMEDOUT
		@note It is recommended that this be called/logged when onConnectionStatusChangedEvent gives a connStatus_Error, connStatus_ConnectionFailed, or connStatus_ConnectionLost
	*/
	VM const char* CALL GetNetworkErrorCode()PVM;

	/**
		Indicates if this Cvatlib_Network object is still valid

		@return true if this Cvatlib_Network object is still valid to use
	*/
	VM bool CALL IsValid()PVM;

	/**
		Indicates if a network session has been created with CreateNetworkSession

		@return true if the network session has been created and is valid
	*/
	VM bool CALL IsSessionExists()PVM;

	/**
		Indicates if this Cvatlib_Network object is successfully connected to the network

		@return true if fully connected to the network
	*/
	VM bool CALL IsNetworkConnected()PVM;
	
	///@}
	/*
		********************************
		* BASIC COMMUNICATIONS METHODS *
		********************************
	*/
	/**
		@name Basic Communications Methods
	*/
	///@{

	/**
		Send a private text message to another callsign

		@param[in] callsign The callsign to send the message to
		@param[in] message The message to send

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendPrivateTextMessage(const char* callsign, const char* message)PVM;

	/**
		Sends a text message over one or more radio frequencies

		@param[in] numFreq The number of frequencies this message will be sent over
		@param[in] frequencies An array of frequencies as integers in kilohertz
		@param[in] message The message to send

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendRadioTextMessage(INT numFreq, INT* frequencies, const char* message)PVM;

	/**
		Sends an info query to the specified client

		@param[in] queryType An info query type to request from the infoQuery enum
		@param[in] callsign The callsign to send the query to

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendInfoQuery(infoQuery queryType, const char* callsign)PVM;

	/**
		Sends a reply to an info query

		@param[in] queryType The info query type that is being replied to from the infoQuery enum
		@param[in] callsign The callsign to send the reply to
		@param[in] data The reply data

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL ReplyToInfoQuery(infoQuery queryType, const char* callsign, const char* data)PVM;

	/**
		Requests aircraft type and model info

		@param[in] callsign The callsign to send the request to

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL RequestPlaneInfo(const char* callsign)PVM;

	///@}
	/*
		***************
		* ATC METHODS *
		***************
	*/
	/**
		@name ATC Methods
	*/
	///@{

	/**
		Sends an ATC position update

		@param[in] info A filled in ATCPosUpdate struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendATCUpdate(ATCPosUpdate info)PVM;

	/**
		Requests to handoff an aircraft to another controller

		@param[in] atcCallsign The controller to request to take the aircraft
		@param[in] acCallsign The aircraft to request the handoff for

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL RequestHandoff(const char* atcCallsign, const char* acCallsign)PVM;

	/**
		Approves a handoff request

		@param[in] atcCallsign The controller to receive the aircraft from
		@param[in] acCallsign The aircraft to approve the handoff for

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL ApproveHandoff(const char* atcCallsign, const char* acCallsign)PVM;

	/**
		Cancels/Rejects a handoff request

		@param[in] atcCallsign The controller to reject the request from
		@param[in] acCallsign The aircraft to reject the handoff for

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL CancelHandoff(const char* atcCallsign, const char* acCallsign)PVM;

	/**
		Amends a flightplan

		@param[in] callsign The callsign whose flightplan is to be updated
		@param[in] fp A filled in FlightPlan struct with all of the information of the updated flightplan, including data which hasn't changed

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL AmendFlightPlan(const char* callsign, FlightPlan fp)PVM;

	/**
		Sends changed shared state data about pilot clients

		@param[in] atcCallsign The ATC callsign to send the state change to. Pass NULL to broadcast to all in range
		@param[in] acCallsign The aircraft whose state is being updated
		@param[in] prop The ccpPropertyType of the value that is being updated
		@param[in] value The new value

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendSharedState(const char* atcCallsign, const char* acCallsign, ccpPropertyType prop, const char* value)PVM;

	/**
		Sends a special message to the target to see if they are a modern client

		@param[in] atcCallsign The ATC callsign to check

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendSharedStateIDRequest(const char* atcCallsign)PVM;

	/**
		Sends a landline command to another client

		@param[in] callsign The callsign to send the command to
		@param[in] cmd The landlineCommand to send
		@param[in] type The landlineType of the command
		@param[in] ip The local clients external IP
		@param[in] port The local clients voice port. Usually 3290

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendLandlineCommand(const char* callsign, landlineCommand cmd, landlineType type, const char* ip, USHORT port)PVM;

	/**
		Sends a tracking command to the server or another controller

		@param[in] atcCallsign The controller to send the command to. Pass NULL if not needed
		@param[in] acCallsign The aircraft to perform the command on
		@param[in] cmd The trackCommand to execute

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendTrackingCommand(const char* atcCallsign, const char* acCallsign, trackCommand cmd)PVM;

	/**
		Broadcasts to all controllers in range that the user want/no longer want a break or are logging off soon

		@param[in] wantBreak True to mark the user as wanting a break, false to mark the user as no longer wanting a break

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SetWantBreak(bool wantBreak)PVM;

	/**
		Sends a line of an ATIS. To send a full text ATIS this must be called multiple times with the appropriate atisLineType. The program must send an atisLineType_LineCount
		message with the number of lines in the ATIS, including the atisLineType_LineCount message after the rest of the ATIS has been sent to finish the ATIS transmission

		@param[in] callsign The callsign to send the ATIS line to
		@param[in] type The atisLineType of the message
		@param[in] data The content of the line

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendAtisLine(const char* callsign, atisLineType type, const char* data)PVM;

	/**
		Updates one of a controllers secondary visibility centers. Only send this if the secondary visibility centers are being used by the controller. They should be sent
		once every 60 seconds. This information will not be rebroadcast to other clients

		@param[in] index The secondary visibility center number to update 0-2. This index number does not include the primary visibility center
		@param[in] lat The latitude of the visibility center in decimal degrees
		@param[in] lon The longitude of the visibility center in decimal degrees

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendSecondaryATCUpdate(INT index, double lat, double lon)PVM;

	/**
		Pushes a flight strip to another controller

		@param[in] atcCallsign The controller to push the flight strip to
		@param[in] acCallsign The aircraft to push to the controller

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL PushFlightStrip(const char* atcCallsign, const char* acCallsign)PVM;

	/**
		Pushes a flight strip to another controller with type identifier and annotations

		@param[in] atcCallsign The controller to push the flight strip to
		@param[in] acCallsign The aircraft to push to the controller
		@param[in] type Strip format identifier
		@param[in] annotations An array of up to 9 strings containing the annotations on the flight strip

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection

		@note annotations must be NULL terminated
	*/
	VM void CALL PushFlightStripEx(const char* atcCallsign, const char* acCallsign, INT type, const char** annotations)PVM;

	/**
		Requests help from other controllers

		@param[in] needHelp True if the user need help from other controllers, false to indicate that the user no longer need help
		@param[in] msg If needHelp is true, the message containing the controllers help query. eg. How do i assign a squawk code?

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL RequestHelp(bool needHelp, const char* msg)PVM;

	/**
		Notifies all clients in range that the client has updated their ATIS or controller info

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL NotifyNewATCInfo()PVM;

	///@}
	/*
		*****************
		* PILOT METHODS *
		*****************
	*/
	/**
		@name Pilot Methods
	*/
	///@{

	/**
		Sends a pilot position update

		@param[in] info A filled in PilotPosUpdate struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendPilotUpdate(PilotPosUpdate info)PVM;

	/**
		Sends an interim (high speed) pilot position update to a specified client

		@param[in] callsign The callsign of the client to send the high speed update to
		@param[in] info A filled in PilotPosUpdate struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection

		@note Only the latitude, longitude, true altitude, pitch, bank, and heading values are used in high speed updates
		@note Only send this packet to clients with the capability_InterimPos flag set. (see SendInfoQuery, infoQuery_Capabilities, and onCapabilitiesReplyReceivedEvent)
		@note Some clients will ignore this packet even if the capability_InterimPos flag is set because some clients may not use the Squawkbox high speed update format
		@note This method should be used in-between regular position updates only. eg. When doing a 1 Hz update rate the program should
			send 1 regular update followed by 4 high speed updates and then repeat
	*/
	VM void CALL SendInterimPilotUpdate(const char* callsign, PilotPosUpdate info)PVM;

	/**
		Sends a flight plan to the server

		@param[in] fp A filled in FlightPlan struct

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL SendFlightPlan(FlightPlan fp)PVM;

	/**
		Sends aircraft info to another client

		@param[in] callsign The client to send the info to
		@param[in] keysValues An array of strings containing the aircrafts information. eg. keysValues = { acinfo_Equipment, "B739", acinfo_Airline, "AAL", NULL };

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
		
		@note keysValues must be NULL terminated
	*/
	VM void CALL SendPlaneInfo(const char* callsign, const char** keysValues)PVM;

	/**
		Sends a custom pilot packet to another client

		@param[in] callsign The client to send the packet to
		@param[in] packetId An alpha-numeric string used by the client program to identify the packets purpose
		@param[in] data An array of data to be used by the client
		@param[in] dataSize The number of elements in the array

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection

		@note Do not use colons (:) in packetId or data, otherwise, it will tokenize incorrectly
	*/
	VM void CALL SendCustomPilotPacket(const char* callsign, const char* packetId, const char** data, INT dataSize)PVM;

	///@}
	/*
		**************************
		* ADMINISTRATIVE METHODS *
		**************************
	*/
	/**
		@name Administrative Methods
	*/
	///@{

	/**
		Kicks/kills a user from the server

		@param[in] callsign The callsign of the user to kick
		@param[in] message Reason message to show the user

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL KickUser(const char* callsign, const char* message)PVM;

	/**
		Pings a user or usergroup. All clients may use this method but only admins or supervisors may use the all ATC/everyone callsigns

		@param[in] callsign The callsign to ping. Can also be *A to ping all ATC or * to ping everyone (Adm/Sup only)

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL PingUser(const char* callsign)PVM;

	/**
		Sends a pong in response to a ping. All clients may use this method to respond to a ping

		@param[in] callsign The callsign to send the response to
		@param[in] time The timestamp that was in the received ping packet

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL PongUser(const char* callsign, const char* time)PVM;

	///@}
	/*
		*******************
		* WEATHER METHODS *
		*******************
	*/
	/**
		@name Weather Methods
	*/
	///@{

	/**
		Requests a text METAR for an airport

		@param[in] airport The ICAO code of the airport to request the METAR for

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL RequestMetar(const char* airport)PVM;

	/**
		Requests a full weather profile for an airport. Clients should send this every 5 minutes, but no more regularly, with the name of the latest
		METAR reporting airport to get recent, local weather to update the environment

		@param[in] airport The ICAO code of the airport to request the weather data for

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
		@throws NetworkNotConnectedException Thrown if there is no network connection
	*/
	VM void CALL RequestWeatherData(const char* airport)PVM;

	///@}
	/*
		*************************
		* CALLBACK REGISTRATION *
		*************************
	*/
	/**
		@name Callback Registration Methods
	*/
	///@{

	/**
		Installs the onConnectionStatusChangedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] connStatusEvent A function with the onConnectionStatusChangedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnConnectionStatusChangedEvent(onConnectionStatusChangedEvent connStatusEvent, void* cbVar)PVM;

	/**
		Installs the onTextMessageReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] txtRecEvent A function with the onTextMessageReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnTextMessageReceivedEvent(onTextMessageReceivedEvent txtRecEvent, void* cbVar)PVM;

	/**
		Installs the onRadioMessageReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] radioRecEvent A function with the onRadioMessageReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnRadioMessageReceivedEvent(onRadioMessageReceivedEvent radioRecEvent, void* cbVar)PVM;

	/**
		Installs the onPilotDisconnectedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pilotDcEvent A function with the onPilotDisconnectedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPilotDisconnectedEvent(onPilotDisconnectedEvent pilotDcEvent, void* cbVar)PVM;

	/**
		Installs the onControllerDisconnectedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] controllerDcEvent A function with the onControllerDisconnectedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnControllerDisconnectedEvent(onControllerDisconnectedEvent controllerDcEvent, void* cbVar)PVM;

	/**
		Installs the onPilotPositionUpdateEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pilotPosUpdEvent A function with the onPilotPositionUpdateEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPilotPositionUpdateEvent(onPilotPositionUpdateEvent pilotPosUpdEvent, void* cbVar)PVM;

	/**
		Installs the onInterimPilotPositionUpdateEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] interimPilotPosUpdEvent A function with the onInterimPilotPositionUpdateEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnInterimPilotPositionUpdateEvent(onInterimPilotPositionUpdateEvent interimPilotPosUpdEvent, void* cbVar)PVM;

	/**
		Installs the onAtcPositionUpdateEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] atcPosUpdEvent A function with the onAtcPositionUpdateEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnAtcPositionUpdateEvent(onAtcPositionUpdateEvent atcPosUpdEvent, void* cbVar)PVM;

	/**
		Installs the onKickedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] kickedEvent A function with the onKickedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnKickedEvent(onKickedEvent kickedEvent, void* cbVar)PVM;

	/**
		Installs the onPongEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pongEvent A function with the onPongEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPongEvent(onPongEvent pongEvent, void* cbVar)PVM;

	/**
		Installs the onFlightPlanReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] fpEvent A function with the onFlightPlanReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnFlightPlanReceivedEvent(onFlightPlanReceivedEvent fpEvent, void* cbVar)PVM;

	/**
		Installs the onHandoffRequestReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] hoReqEvent A function with the onHandoffRequestReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnHandoffRequestReceivedEvent(onHandoffRequestReceivedEvent hoReqEvent, void* cbVar)PVM;

	/**
		Installs the onHandoffAcceptedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] hoAccEvent A function with the onHandoffAcceptedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnHandoffAcceptedEvent(onHandoffAcceptedEvent hoAccEvent, void* cbVar)PVM;

	/**
		Installs the onHandoffCancelledRejectedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] hoRejEvent A function with the onHandoffCancelledRejectedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnHandoffCancelledRejectedEvent(onHandoffCancelledRejectedEvent hoRejEvent, void* cbVar)PVM;

	/**
		Installs the onMetarReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] metarEvent A function with the onMetarReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnMetarReceivedEvent(onMetarReceivedEvent metarEvent, void* cbVar)PVM;

	/**
		Installs the onInfoQueryRequestReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] infoQReqEvent A function with the onInfoQueryRequestReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnInfoQueryRequestReceivedEvent(onInfoQueryRequestReceivedEvent infoQReqEvent, void* cbVar)PVM;

	/**
		Installs the onInfoQueryReplyReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] infoQReplyEvent A function with the onInfoQueryReplyReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnInfoQueryReplyReceivedEvent(onInfoQueryReplyReceivedEvent infoQReplyEvent, void* cbVar)PVM;

	/**
		Installs the onCapabilitiesReplyReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] capsReplyEvent A function with the onCapabilitiesReplyReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnCapabilitiesReplyReceivedEvent(onCapabilitiesReplyReceivedEvent capsReplyEvent, void* cbVar)PVM;

	/**
		Installs the onAtisReplyReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] atisReplyEvent A function with the onAtisReplyReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnAtisReplyReceivedEvent(onAtisReplyReceivedEvent atisReplyEvent, void* cbVar)PVM;

	/**
		Installs the onTemperatureDataReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] tempDataEvent A function with the onTemperatureDataReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnTemperatureDataReceivedEvent(onTemperatureDataReceivedEvent tempDataEvent, void* cbVar)PVM;

	/**
		Installs the onErrorReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] errorEvent A function with the onErrorReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnErrorReceivedEvent(onErrorReceivedEvent errorEvent, void* cbVar)PVM;

	/**
		Installs the onWindDataReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] windDataEvent A function with the onWindDataReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnWindDataReceivedEvent(onWindDataReceivedEvent windDataEvent, void* cbVar)PVM;

	/**
		Installs the onCloudDataReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] cloudDataEvent A function with the onCloudDataReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnCloudDataReceivedEvent(onCloudDataReceivedEvent cloudDataEvent, void* cbVar)PVM;

	/**
		Installs the onPilotInfoRequestReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pilotInfoReqEvent A function with the onPilotInfoRequestReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPilotInfoRequestReceivedEvent(onPilotInfoRequestReceivedEvent pilotInfoReqEvent, void* cbVar)PVM;

	/**
		Installs the onPilotInfoReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pilotInfoEvent A function with the onPilotInfoReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPilotInfoReceivedEvent(onPilotInfoReceivedEvent pilotInfoEvent, void* cbVar)PVM;

	/**
		Installs the onCustomPilotPacketReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] customPilotPacketEvent A function with the onCustomPilotPacketReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnCustomPilotPacketReceivedEvent(onCustomPilotPacketReceivedEvent customPilotPacketEvent, void* cbVar)PVM;

	/**
		Installs the onSharedStateReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] sharedStateEvent A function with the onSharedStateReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnSharedStateReceivedEvent(onSharedStateReceivedEvent sharedStateEvent, void* cbVar)PVM;

	/**
		Installs the onLandlineCommandReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] landlineCmdEvent A function with the onLandlineCommandReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnLandlineCommandReceivedEvent(onLandlineCommandReceivedEvent landlineCmdEvent, void* cbVar)PVM;

	/**
		Installs the onTrackingCommandReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] trackCmdEvent A function with the onTrackingCommandReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnTrackingCommandReceivedEvent(onTrackingCommandReceivedEvent trackCmdEvent, void* cbVar)PVM;

	/**
		Installs the onBreakCommandReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] breakCmdEvent A function with the onBreakCommandReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnBreakCommandReceivedEvent(onBreakCommandReceivedEvent breakCmdEvent, void* cbVar)PVM;

	/**
		Installs the onSharedStateIDRequestReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] shStIdReqEvent A function with the onSharedStateIDRequestReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnSharedStateIDRequestReceivedEvent(onSharedStateIDRequestReceivedEvent shStIdReqEvent, void* cbVar)PVM;

	/**
		Installs the onSharedStateIDReplyReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] shStIdRepEvent A function with the onSharedStateIDReplyReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnSharedStateIDReplyReceivedEvent(onSharedStateIDReplyReceivedEvent shStIdRepEvent, void* cbVar)PVM;

	/**
		Installs the onPushStripReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] pushStripEvent A function with the onPushStripReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnPushStripReceivedEvent(onPushStripReceivedEvent pushStripEvent, void* cbVar)PVM;

	/**
		Installs the onHelpRequestReceivedEvent callback. The program can only install 1 of these. Calling this method again will replace the existing function

		@param[in] helpReqEvent A function with the onHelpRequestReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws InvalidNetworkSessionException Thrown if a network session has not been created
	*/
	VM void CALL InstallOnHelpRequestReceivedEvent(onHelpRequestReceivedEvent helpReqEvent, void* cbVar)PVM;

	///@}
	/*
		***************************
		* VATSIM STATUS RETRIEVER *
		***************************
	*/
	/**
		@name VATSIM Status Retriever Methods
	*/
	///@{

	/**
		Gets a list of urls that can be queried to get the latest VATSIM data file

		@return A @ref NULL terminated const array of c strings containing the VATSIM data file urls

		@note If querying the urls for data, the program is required to pick them at random each time
		@note The structure for the data files can be found by opening them in a web browser

		@warning The memory of the return value must be freed using Cvatlib_Network::GetVatsimStatusUrls_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimStatusUrls()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Network::GetVatsimStatusUrls

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimStatusUrls_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM FSD server names

		@return A @ref NULL terminated const array of c strings containing the VATSIM FSD server names

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Network::GetVatsimFSDServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Network::GetVatsimFSDServerNames_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimFSDServerNames()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Network::GetVatsimFSDServerNames

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimFSDServerNames_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM FSD server IP addresses

		@return A @ref NULL terminated const array of c strings containing the VATSIM FSD server IP addresses

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Network::GetVatsimFSDServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Network::GetVatsimFSDServerIps_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimFSDServerIps()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Network::GetVatsimFSDServerIps

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimFSDServerIps_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM FSD server locations

		@return A @ref NULL terminated const array of c strings containing the VATSIM FSD server locations

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Network::GetVatsimFSDServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Network::GetVatsimFSDServerLocations_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimFSDServerLocations()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Network::GetVatsimFSDServerLocations

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimFSDServerLocations_Free(const char* const* memory)PVM;

	/**
		Gets a list denoting whether each known FSD server is currently accepting connections

		@return A @ref NULL terminated const array of bools denoting whether each FSD server is accepting connections

		@warning The memory of the return value must be freed using Cvatlib_Network::GetVatsimFSDServerAcceptingConnections_Free before calling this method again and when the program exits
	*/
	VM const bool* CALL GetVatsimFSDServerAcceptingConnections()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Network::GetVatsimFSDServerAcceptingConnections

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimFSDServerAcceptingConnections_Free(const bool* memory)PVM;

	///@}
};

/**
	@class VatlibException

	@brief This is the base class for all exceptions thrown by the shim library
*/
class VATLIB_API VatlibException : public std::exception {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	virtual const char* what() const throw()
	{
		return "Unknown Vatlib exception";
	}
};

/**
	@class NetworkNotConnectedException

	@brief An exception that is thrown if a method requiring a network connection is called before a connection attempt is made

	@note This exception is thrown only by @ref Cvatlib_Network
*/
class VATLIB_API NetworkNotConnectedException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method that requires a network connection before connecting";
	}
};

/**
	@class InvalidNetworkSessionException

	@brief An exception that is thrown if a method requiring a network session is called before a network session is created

	@note This exception is thrown only by @ref Cvatlib_Network
*/
class VATLIB_API InvalidNetworkSessionException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method that requires a network session before it is created";
	}
};

/**
	@class NetworkSessionExistsException

	@brief An exception that is thrown if an attempt to create a network session is made when one already exists

	@note This exception is thrown only by @ref Cvatlib_Network
*/
class VATLIB_API NetworkSessionExistsException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to create a network session when one already exists";
	}
};

/**
	@class ObjectNotSetupException

	@brief An exception that is thrown if a method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method

	@note This exception is thrown only by @ref Cvatlib_Voice_Simple
*/
class VATLIB_API ObjectNotSetupException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method before executing Setup method";
	}
};

/**
	@class InvalidRoomException

	@brief An exception that is thrown if a method requiring a room is called when it isn't valid

	@note This exception is thrown only by @ref Cvatlib_Voice_Simple
*/
class VATLIB_API InvalidRoomException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method that requires a valid room";
	}
};

/**
	@class RoomNotConnectedException

	@brief An exception that is thrown if a method requiring a room is called when it isn't connected

	@note This exception is thrown only by @ref Cvatlib_Voice_Simple
*/
class VATLIB_API RoomNotConnectedException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method that requires a connected room";
	}
};

/**
	@class InvalidObjectException

	@brief An exception that is thrown if a method is called after the object becomes invalid
*/
class VATLIB_API InvalidObjectException : public VatlibException {
public:
	/**
		Gets the description of the exception

		@return The description of the exception
	*/
	const char* what() const throw() {
		return "Attempted to call a method on an invalid object";
	}
};

/**
	Converts a FS9 PBH into double precision pitch(-180 to 180), bank(-180 to 180), and heading(0 to 360) values
	
	@param[in] pbh The pbh value to convert
	@param[out] outPitch A double variable to receive the final pitch value
	@param[out] outBank A double variable to receive the final bank value
	@param[out] outHeading A double variable to receive the final heading value
*/
EXC VATLIB_API void CALL ConvertPBHToDoubles(UINT pbh, double& outPitch, double& outBank, double& outHeading);

/**
	Converts double precision pitch(-180 to 180), bank(-180 to 180), and heading(0 to 360) values to a FS9 PBH value

	@param[in] pitch The pitch value
	@param[in] bank The bank value
	@param[in] heading The heading value
	@param[out] outPbh An unsigned int variable to receive the final pbh value
*/
EXC VATLIB_API void CALL ConvertDoublesToPBH(double pitch, double bank, double heading, UINT& outPbh);

/**
	@class Cvatlib_Voice_Simple
	
	@brief Contains methods and variables for the simple implementation of the voice library. This is most suitable for a pilot client

	@note Due to the internal nature of this class, it is only valid to have one(1) instance of it at any given time
*/
class VATLIB_API Cvatlib_Voice_Simple {
public:
	/* Enums */
	/**
		@name Room States
		
		Connection states for voice rooms
	*/
	///@{
	/**
		Room State
		
		The connection state of a room
	*/
	enum roomState_enum {
		/**
			Voice room is not connected
		*/
		roomState_NotConnected,
		/**
			Voice room is in the process of connecting
		*/
		roomState_Connecting,
		/**
			Voice room is connected
		*/
		roomState_Connected,
		/**
			Voice room is in the process of disconnecting
		*/
		roomState_Disconnecting
	};
	/**
		Represents a @ref roomState_enum value
	*/
	typedef INT roomState;
	///@}

	/**
		@name Room Status Updates
		
		Status updates on state of the room
	*/
	///@{
	/**
		Room Status Updates
		
		Status updates on state of the room
	*/
	enum roomStatusUpdate_enum {
		/**
			Successfully joined the voice room
		*/
		roomStatusUpdate_JoinSuccess = 0,
		/**
			Failed to join the voice room
		*/
		roomStatusUpdate_JoinFail = 1,
		/**
			Disconnected unexpectedly from the voice 
		*/
		roomStatusUpdate_UnexpectedDisconnectOrKicked = 2,
		/**
			Voice room leave command 
		*/
		roomStatusUpdate_LeaveComplete = 3,
		/**
			Another user joined or left the voice 
		*/
		roomStatusUpdate_UserJoinsLeaves = 4,
		/**
			Audio has started from the voice room
		*/
		roomStatusUpdate_RoomAudioStarted = 5,
		/**
			Audio has stopped from the voice room
		*/
		roomStatusUpdate_RoomAudioStopped = 6,
		/**
			Global audio has started

			@note This state is global, the room number will be negative
		*/
		roomStatusUpdate_AudioStarted = 7,
		/**
			Global audio has stopped

			@note This state is global, the room number will be negative
		*/
		roomStatusUpdate_AudioStopped = 8
	};
	/**
		Represents a @ref roomStatusUpdate_enum value
	*/
	typedef INT roomStatusUpdate;
	///@}

	/**
		@name Automatic Gain Control Responses

		Results from the overall mic tests after doing a squelch test followed by a mic test
	
		@note These are bit fields so more than one error can be given at a time
	*/
	///@{
	/**
		Automatic Gain Control Responses

		Results from the overall mic tests after doing a squelch test followed by a mic test
	
		@note These are bit fields so more than one error can be given at a time
	*/
	enum agc_enum {
		/**
			The test went ok
		*/
		agc_Ok = 0,
		/**
			The overall background noise is very loud and may be a nuisance to others
		*/
		agc_BkgndNoiseLoud = 1,
		/**
			The overall background noise is loud enough that others probably wont be able to distinguish speech from it
		*/
		agc_TalkDrownedOut = 2,
		/**
			The overall mic volume is too hot, the user should lower the volume in the windows volume control panel
		*/
		agc_TalkMicHot = 4,
		/**
			The overall mic volume is too cold, the user should raise the volume in the windows control panel and enable mic boost if needed
		*/
		agc_TalkMicCold = 8
	};
	/**
		Represents an @ref agc_enum value
	*/
	typedef INT agc;
	///@}

	/* Callback Definitions */

	/**
		A callback for when a room status update occurs

		@param[in] obj Passes in a reference to the Cvatlib_Voice_Simple object to which it is registered
		@param[in] upd Passes in the roomStatusUpdate enum value that represents the type of update that occurred
		@param[in] roomIndex Passes in the index of the room whose status has been updated
		@param[in] cbVar Passes in a variable defined when the callback is installed
	*/
	typedef void (*onRoomStatusUpdateEvent) (Cvatlib_Voice_Simple* obj, roomStatusUpdate upd, INT roomIndex, void* cbVar);

	/**
		A callback for when one of the users in a room is sent to the client. This is called once per user, including the local client, when a rooms user list is requested 

		@param[in] obj Passes in a reference to the Cvatlib_Voice_Simple object to which it is registered
		@param[in] name Passes in the name of the user
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note The room index is not provided for this, the client should only have one user of this callback running at a time or use cbVar to distinguish the calls
		@note This callback does not indicate when the last user has been received
	*/
	typedef void (*onRoomUserReceivedEvent) (Cvatlib_Voice_Simple* obj, const char* name, void* cbVar);

	/**
		A callback for when an audio device is received

		@param[in] obj Passes in a reference to the Cvatlib_Voice_Simple object to which it is registered
		@param[in] name Passes in the name of the device
		@param[in] cbVar Passes in a variable defined when the callback is installed

		@note When operating on audio devices the 0-based index is passed in. This callback does not pass in the index, so in order to get the index the client must somehow save
			the exact order in which each instance of this callback is received
		@note This callback will not indicate if it it passing in an input or output device, the program should only have one user of this callback running at a time,
			separate callback functions, or use cbVar to distinguish the calls
		@note This callback does not indicate when the last device has been received
		@note If saving devices for user settings it is highly recommended to save by device name and not index since indexes can change but device names usually do not
	*/
	typedef void (*onHardwareDeviceReceivedEvent) (Cvatlib_Voice_Simple* obj, const char* name, void* cbVar);

	/* Methods */

	/**
		Contructor
	*/
	static Cvatlib_Voice_Simple* CALL Create();

	/**
		Destructor
	*/
	VM void CALL Destroy()PVM;

	/**
		Default Destructor

		@deprecated Use the @ref Cvatlib_Voice_Simple::Destroy method to delete a Cvatlib_Voice_Simple object
	*/
	VM ~Cvatlib_Voice_Simple()PVM;

	/*
		*****************************
		* ROOM AND HANDLING METHODS *
		*****************************
	*/
	/**
		@name Room and Handling Methods
	*/
	///@{

	/**
		Sets up the simple voice library

		@param[in] initNetwork Set to true if the networking library needs to be inited, false if it is already done it already. If a Cvatlib_Network object has been created,
			the network is inited and false should be passed in
		@param[in] udpPort The UDP port to send and receive voice on. The current standard is 3290
		@param[in] roomCount A positive number stating how many potential voice room connections should want to be made available
		@param[in] outputCount A positive number stating how many potential output devices should be made available. This is the number the client intends to use, not the number
			of devices on the system
		@param[in] roomStatusUpdEvent A function with the onRoomStatusUpdateEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@return true on success

		@throws InvalidObjectException Thrown if this method is called on an invalid object
	*/
	VM bool CALL Setup(bool initNetwork, SHORT udpPort, INT roomCount, INT outputCount, onRoomStatusUpdateEvent roomStatusUpdEvent, void* cbVar)PVM;

	/**
		Attempts to join a room

		@param[in] roomIndex Which room index to associate the room with. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method
		@param[in] callsign The callsign the local client will be known as in the room
		@param[in] roomSpec A room specification string in the form \<server>[:port][/room]

		@return false if the DNS lookup fails or an invalid room index is entered, otherwise true

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index

		@note LeaveRoom should be executed before rejoining a room. If this is not done this method will perform an abortive and disorderly disconnect on the old room
		@note The roomStatusUpdate_JoinFail message is not sent if this method returns false
		@note When this method returns true, it only means that the neccessary requirements to ATTEMPT the room join were met, if a failure occurs during the actual joining
			process, onRoomStatusUpdateEvent will be triggered with a roomStatusUpdate_JoinFail
	*/
	VM bool CALL JoinRoom(INT roomIndex, const char* callsign, const char* roomSpec)PVM;

	/**
		Leaves a room

		@param[in] roomIndex Which room index to leave. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected

		@note This can also be used to abort a JoinRoom that has become hung
	*/
	VM void CALL LeaveRoom(INT roomIndex)PVM;

	/**
		Sets the volume of the output on a per-room basis

		@param[in] roomIndex The 0-based index of the room to change
		@param[in] volume The volume to set on the room. Valid values 0-100 where 0 is mute and 100 is no change in volume

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called

		@note This volume setting is implemented as a modifier to the output volume set by @ref Cvatlib_Voice_Simple::SetOutputVolume "SetOutputVolume"
	*/
	VM void CALL SetRoomVolume(INT roomIndex, INT volume)PVM;

	/**
		Sets the state of the microphone, controlling if its sent to the designated room

		@param[in] roomIndex Which room index to set the state on. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method
		@param[in] micIsOpen Set to true to send captured input audio to the designated room, false to stop sending

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected
	*/
	VM void CALL SetMicState(INT roomIndex, bool micIsOpen)PVM;

	/**
		Checks if audio currently is, or very recently was, playing in the designated room

		@param[in] roomIndex Which room index to check. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup method

		@return true if audio is currently, or was very recently, playing in the designated room

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected
	*/
	VM bool CALL IsAudioPlaying(INT roomIndex)PVM;

	/**
		Calling this method makes the library do all of the work. If this method is not called, nothing will happen. This should be called at least every 100ms
		
		@note This should be called a few times after disconnecting to allow cleanup to occur

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL DoProcessing()PVM;

	/**
		Indicates if this Cvatlib_Voice_Simple object is still valid

		@return true if this Cvatlib_Voice_Simple object is still valid to use
	*/
	VM bool CALL IsValid()PVM;

	/**
		Indicates if this Cvatlib_Voice_Simple object has been setup using Setup

		@return true if this Cvatlib_Voice_Simple object has been setup
	*/
	VM bool CALL IsSetup()PVM;

	/**
		Indicates if the specified voice room is a valid room index (not out of bounds)

		@return true if the specified room index is valid
	*/
	VM bool CALL IsRoomValid(INT roomIndex)PVM;

	/**
		Indicates if the specified voice room is connected

		@return true if the specified room index is valid and connected
	*/
	VM bool CALL IsRoomConnected(INT roomIndex)PVM;

	/**
		Gets a list of every member in a room, including the local client

		@param[in] roomIndex Which room index to check. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method
		@param[in] roomUsrRecdEvent A function with the onRoomUserReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected

		@note The callback function does not provide a way to distinguish which room the result is being returned for, this method should only be running one at a time or
			cbVar should be used to distinguish the calls
		@note The onRoomUserReceivedEvent is called separately for each user in the room and does not indicate when all users have been received
	*/
	VM void CALL GetRoomUserList(INT roomIndex, onRoomUserReceivedEvent roomUsrRecdEvent, void* cbVar)PVM;

	/**
		Gets the roomState of the designated room

		@param[in] roomIndex Which room index to check. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method

		@return The roomState of the designated room

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected
	*/
	VM roomState CALL GetRoomState(INT roomIndex)PVM;

	/**
		Makes/removes the connection between each mixer and hardware codec. By default none of these are attached so there will be no audio

		@param[in] roomIndex Which room index to set. Valid values are between 0 (inclusive) and the value of roomCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method
		@param[in] outputIndex Which output index to set. Valid values are between 0 (inclusive) and the value of outputCount (exclusive) from the @ref Cvatlib_Voice_Simple::Setup "Setup" method
		@param[in] isOn Set to true to connect the designated output so that audio from the designated room is played through it, false to disconnect the output from the room

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
		@throws InvalidRoomException Thrown if this method is called with an invalid room index
		@throws RoomNotConnectedException Thrown if this method is called with a room that isn't connected
	*/
	VM void CALL SetOutputState(INT roomIndex, INT outputIndex, bool isOn)PVM;

	///@}
	/*
		*****************************
		* AUDIO HARDWARE MANAGEMENT *
		*****************************
	*/
	/**
		@name Audio Hardware Management Methods
	*/
	///@{

	/**
		Gets a list of all hardware input/capture devices, such as microphones, and sends the names to the provided callback in index order

		@param[in] hdwrDevRecdEvent A function with the onHardwareDeviceReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called

		@note The callback function does not provide a way to distinguish if it is returning for inputs or outputs, there should only be one of these methods running at a time
			unless cbVar or a different callback function is used distinguish the calls
		@note The onHardwareDeviceReceivedEvent is called separately for each device and does not indicate when all devices have been received
		@note The onHardwareDeviceReceivedEvent does not provide the numerical index required for manipulating the device settings. Devices will be sent through the callback
			in numerical index order and it is the programs job to create and save the 0-based numerical index
	*/
	VM void CALL GetInputDevices(onHardwareDeviceReceivedEvent hdwrDevRecdEvent, void* cbVar)PVM;

	/**
		Gets a list of all hardware output/playback devices, such as speakers, and sends the names to the provided callback in index order

		@param[in] hdwrDevRecdEvent A function with the onHardwareDeviceReceivedEvent signature
		@param[in] cbVar An optional variable to be passed into the callback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called

		@note The callback function does not provide a way to distinguish if it is returning for inputs or outputs, there should only be one of these methods running at a time
			unless cbVar or a different callback function is used distinguish the calls
		@note  The onHardwareDeviceReceivedEvent is called separately for each device and does not indicate when all devices have been received
		@note The onHardwareDeviceReceivedEvent does not provide the numerical index required for manipulating the device settings. Devices will be sent through the callback
			in numerical index order and it is the programs job to create and save the 0-based numerical index
	*/
	VM void CALL GetOutputDevices(onHardwareDeviceReceivedEvent hdwrDevRecdEvent, void* cbVar)PVM;

	/**
		Sets the input/capture device that will be used by all rooms

		@param[in] devIndex The 0-based index of the device to use, based on the order in which GetInputDevices called the onHardwareDeviceReceivedEvent

		@return true if the operation succeeded

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM bool CALL SetInputDevice(SHORT devIndex)PVM;

	/**
		Sets the output/playback device that will be used by any rooms that select the designated output index
		
		@param[in] outputIndex The 0-based index of the output codec to assign the output device to
		@param[in] devIndex The 0-based index of the device to use, based on the order in which GetOutputDevices called the onHardwareDeviceReceivedEvent

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetOutputDevice(INT outputIndex, SHORT devIndex)PVM;

	/**
		Checks if the input/capture device has hit a fatal error. Its a good idea to call this after using SetInputDevice

		@return false if the input device hit a fatal error

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM bool CALL IsInputDeviceAlive()PVM;

	/**
		Checks if the output/playback device has hit a fatal error. Its a good idea to call this after using SetOutputDevice

		@param[in] outputIndex The 0-based index of the output codec to check

		@return false if the output device hit a fatal error

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM bool CALL IsOutputDeviceAlive(INT outputIndex)PVM;

	/**
		Sets the volume of the output/playback device

		@param[in] outputIndex The 0-based index of the output codec to change
		@param[in] volume The volume to set on the codec. Valid values 0-100 where 0 is mute and 100 is no change in volume

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called

		@note This volume setting is independent of the operating system volume control and operates by modifying the incoming data before it gets passed to the operating system
	*/
	VM void CALL SetOutputVolume(INT outputIndex, INT volume)PVM;

	/**
		Sets the volume of the sidetone, the beep that plays when the transmit button is pressed

		@param[in] volume The volume to set on the codec. Valid values 0-100 where 0 is mute and 100 is no change in volume

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetSideToneVolume(INT volume)PVM;

	/**
		Enables or disables a band pass filter with edge frequencies of 300Hz and 3kHz. This simulates the effect of "distorted" aviation transmissions by removing upper
		and lower frequencies that don't exist in ATC/pilot transmissions

		@param[in] outputIndex The 0-based index of the output codec to change
		@param[in] isEnabled Set to true to enable the filter

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetFilterEnabled(INT outputIndex, bool isEnabled)PVM;

	/**
		Enables or disables passing audio from the input device directly to the output device of the specified output codec

		@param[in] outputIndex The 0-based index of the output codec to play the audio on
		@param[in] isOn Set to true to enable the loopback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetAudioLoopback(INT outputIndex, bool isOn)PVM;

	///@}
	/*
		*******************
		* MIC TEST WIZARD *
		*******************
	*/
	/**
		@name Mic Test Wizard Methods
	*/
	///@{

	/**
		This starts the routine for determining the best squelch (microphone cutout) level. During this test the user should be as quiet as possible.
		Running this test for 5 seconds should be sufficient

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL BeginFindSquelch()PVM;

	/**
		This ends the squelch test and makes the suggested squelch level available for retrieval

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL EndFindSquelch()PVM;

	/**
		This starts the routine to determine if speech is coming in at an acceptable volume. The recording test can be run at the same time as this test.
		The squelch test should be run before this test. During this test the user should speak normally. Running this test for 5 seconds should be sufficient

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL BeginMicTest()PVM;

	/**
		This ends the mic test and returns an agc bit field with the results of the mic test. It is common to get 1|2|4

		@return Any number of agc enum values as a bit field. If this is non-zero, each of the flags defines a detected problem

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM agc CALL EndMicTest()PVM;

	/**
		Records captured audio. The main use is to allow the user to hear how they might sound to controllers and other pilots as well as to test their playback device

		@param[in] fileName The file to save the recording to

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL StartRecordTest(const char* fileName)PVM;

	/**
		Ends recording of audio
	*/
	VM void CALL StopRecordTest()PVM;

	/**
		Plays the audio recorded during the record test. This routine automatically uses the last recorded stream. This method does not monitor when playback is completed so
		the program must figure out when it is done and call StopPlaybackTest

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL StartPlaybackTest()PVM;

	/**
		Stops the audio playback

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL StopPlaybackTest()PVM;

	/**
		Tells if the program is playing audio

		@return true if StartPlaybackTest was run but StopPlaybackTest has not

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called

		@note This does not tell if there actually is audio playing or not, it only indicates if the program has run StartPlaybackTest but not StopPlaybackTest
	*/
	VM bool CALL IsPlaying()PVM;

	/**
		Gets a vaguely accurate time elapsed in seconds. Useful for measuring relative time intervals for running the tests

		@return The time value

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM double CALL GetTime()PVM;

	///@}
	/*
		***************************************
		* PROGRAMMATIC ACCESS TO MIC SETTINGS *
		***************************************
	*/
	/**
		@name Mic Settings Methods
	*/
	///@{

	/**
		Use this to get the suggested microphone squelch level from the squelch test. It is suggested that this be saved so that it can be restored each time the client is run

		@return The agc suggested mic squelch from the squelch test

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM float CALL GetInputSquelch()PVM;

	/**
		Use this to get the output squelch used for clients who have not implemented the squelch tests yet. This is mainly used for older RW users. Since levels vary by user
		there really is no good choice but the default one should work well. If the client allows changing this it is suggested that it be saved so that the client can restore
		it each time it is run

		@return The agc applied squelch for users who have not already applied it to their mic

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM float CALL GetOutputSquelch()PVM;

	/**
		Use this method to set the microphone squelch. This can be used to change it manually or to restore a saved setting. The value is a binary representation of peak
		amplitude with full scale being 2^16-1. This value should be just a level louder than the background noise to keep the agc from going crazy when the user is not talking

		@param[in] level The squelch level to set for the microphone

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetInputSquelch(float level)PVM;

	/**
		Use this method to set the output squelch. This can be used to change it manually or to restore a saved setting. The value is a binary representation of peak
		amplitude with full scale being 2^16-1. This mainly serves to clean up some older RW users

		@param[in] level The squelch level to set for incoming audio

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL SetOutputSquelch(float level)PVM;

	/**
		This enables the sidetone, the beep that plays when the user begins transmitting

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL EnableSideTone()PVM;

	/**
		This disables the sidetone, the beep that plays when the user begins transmitting

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM void CALL DisableSideTone()PVM;

	/**
		Tells if the sidetone is currently enabled. The sidetone is enabled by default on each startup

		@return true if the sidetone is enabled

		@throws InvalidObjectException Thrown if this method is called on an invalid object
		@throws ObjectNotSetupException Thrown if this method is called before the @ref Cvatlib_Voice_Simple::Setup "Setup" method is called
	*/
	VM bool CALL IsSideToneEnabled()PVM;

	///@}
	/*
		***************************
		* VATSIM STATUS RETRIEVER *
		***************************
	*/
	/**
		@name VATSIM Status Retriever Methods
	*/
	///@{

	/**
		Gets a list of urls that can be queried to get the latest VATSIM data file

		@return A @ref NULL terminated const array of c strings containing the VATSIM data file urls

		@note If querying the urls for data, the program is required to pick them at random each time
		@note The structure for the data files can be found by opening them in a web browser

		@warning The memory of the return value must be freed using Cvatlib_Voice_Simple::GetVatsimStatusUrls_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimStatusUrls()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Voice_Simple::GetVatsimStatusUrls

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimStatusUrls_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM voice server names

		@return A @ref NULL terminated const array of c strings containing the VATSIM voice server names

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Voice_Simple::GetVatsimVoiceServerNames_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimVoiceServerNames()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerNames

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimVoiceServerNames_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM voice server IP addresses

		@return A @ref NULL terminated const array of c strings containing the VATSIM voice server IP addresses

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Voice_Simple::GetVatsimVoiceServerIps_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimVoiceServerIps()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerIps

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimVoiceServerIps_Free(const char* const* memory)PVM;

	/**
		Gets a list of known VATSIM voice server locations

		@return A @ref NULL terminated const array of c strings containing the VATSIM voice server locations

		@note It is possible for a listed server to not be currently accepting connections, see @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerAcceptingConnections

		@warning The memory of the return value must be freed using Cvatlib_Voice_Simple::GetVatsimVoiceServerLocations_Free before calling this method again and when the program exits
	*/
	VM const char* const* CALL GetVatsimVoiceServerLocations()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerLocations

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimVoiceServerLocations_Free(const char* const* memory)PVM;

	/**
		Gets a list denoting whether each known voice server is currently accepting connections

		@return A @ref NULL terminated const array of bools denoting whether each voice server is accepting connections

		@warning The memory of the return value must be freed using Cvatlib_Voice_Simple::GetVatsimVoiceServerAcceptingConnections_Free before calling this method again and when the program exits
	*/
	VM const bool* CALL GetVatsimVoiceServerAcceptingConnections()PVM;

	/**
		Frees the memory used by the return value of @ref Cvatlib_Voice_Simple::GetVatsimVoiceServerAcceptingConnections

		@param[in] memory The memory to free
	*/
	VM void CALL GetVatsimVoiceServerAcceptingConnections_Free(const bool* memory)PVM;

	///@}
};

#endif

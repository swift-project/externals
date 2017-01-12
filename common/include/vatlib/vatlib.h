#ifndef VATLIB_h_
#define VATLIB_h_

/**
    \file vatlib.h
    \author Roland Winklmeier
    \date 2014
    \brief VATSIM library

    Vatlib is the library required to interface with typical
    VATSIM servers. It provides methods to communicate via
    network and voice protocols.
 */

#if defined(_WIN32) && !defined(VATLIB_STATIC)
/* You should define VATLIB_EXPORTS *only* when building the DLL. */

#  ifdef VATLIB_EXPORTS
#    define VATLIB_API __declspec(dllexport)
#  else
#    define VATLIB_API __declspec(dllimport)
#  endif
#else
#  define VATLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>
#include <stdbool.h>


/***************************************************************************
    General Section
 ***************************************************************************/

/** Versions 0.1.0 through 1.0.0 are in the range 0 to 100.
 From version 1.0.1 on it's xxyyzz, where x=major, y=minor, z=release
 Reason:
 Leading zeros changes the number to octal.
*/
#define VAT_LIBVATLIB_VERSION 906

/**
 * Retrieve the release number of the currently running Vatlib build,
 eg 904.
*/
VATLIB_API int Vat_GetVersion();

/** Retrieve a textual description of the current Vatlib build,
 eg "Vatlib V0.9.1 (built (built Oct 20 2014 11:10:00).
*/
VATLIB_API const char *Vat_GetVersionText();

/** Log severity levels. They are used within the log handlers to specify
 the severity of the log message.

 @see VatlibLogHandler_t, Vat_SetVoiceLogHandler, Vat_SetVoiceLogSeverityLevel,
 Vat_SetNetworkLogHandler, Vat_SetNetworkLogSeverityLevel

*/
typedef enum
{
    SeverityNone,       /**< Severity None. Nothing is logged. */
    SeverityError,      /**< Severity Error. Only errors are logged. */
    SeverityWarning,    /**< Severity Warning. Warning and higher is logged. */
    SeverityInfo,       /**< Severity Info. Info and higher is is logged. */
    SeverityDebug       /**< Severity Debug. Debug and higher is logged. */
}
SeverityLevel;

/** Log handler callback. A function of this signature is called by vatlib when a message is logged.

 @param level The log message severity.
 @param message The messages logged by vatlib.
 @see SeverityLevel
 */
typedef void (* VatlibLogHandler_t)(
    SeverityLevel level,
    const char *message);

/***************************************************************************
    Network Section
 ***************************************************************************/

/**
 VatSessionID is an opaque reference to a session. A session is a single user's connection
 to the protocol. A session may be connected multiple times, but its login information
 can only be specified when disconnected. Each session has callbacks for incoming data
 associated with it; these are good for the life of a session but must be set up
 separately for each session. For this reason, you probably want to keep your session
 aroud a long time.
*/
#ifdef __cplusplus
typedef class FSDClient PCSBClient;
typedef PCSBClient *VatSessionID;
#else
typedef void *VatSessionID;
#endif

/** A test method to pass valid FSD message lines as if they have been received from the
 FSD servers. The message is fully processed and all callbacks are called.

 @param session
 @param message The FSD message line
*/
VATLIB_API void Vat_SendFsdMessage(
    VatSessionID session,
    const char *message);

/** Server type
*/
typedef enum
{
    vatServerLegacyFsd,   /**< Legacy FSD. */
    vatServerVatsim       /**< VATSIM server. */
}
VatServerType;

/** Connection status
*/
typedef enum
{
    vatStatusIdle,          /**< Not yet connected. Deprecated and not used anymore! */
    vatStatusConnecting,    /**< Connecting to server */
    vatStatusConnected,     /**< Connection successful (log-in may be in process). */
    vatStatusDisconnecting, /**< Disconnecting from server. */
    vatStatusDisconnected,  /**< Disconnected from server. */
    vatStatusError          /**< Connection error. Deprecated and not used anymore! */
}
VatConnectionStatus;

/** Client capability flags
*/
typedef enum
{
    /** None */
    vatCapsNone             = (1 << 0),

    /**
     * Can accept ATIS responses
     */
    vatCapsAtcInfo          = (1 << 1),

    /**
     * Can send/receive secondary visibility center points (ATC/Server only)
     */
    vatCapsSecondaryPos     = (1 << 2),

    /**
     * Can send/receive modern model packets.
     *
     * This should be the standard for any new pilot client. Also all older VATSIM clients
     * starting from SB3 do support this capability.
     * Aircraft info contains
     * \li Aircraft ICAO identifier
     * \li Airline ICAO identifier (optional)
     * \li Airline livery (optional)
     */
    vatCapsAircraftInfo     = (1 << 3),

    /**
     * Can send/receive inter-facility coordination packets (ATC only)
     */
    vatCapsOngoingCoord     = (1 << 4),

    /**
     * Can send/receive Interim position updates (pilot only)
     * \deprecated Used only by Squawkbox with high precision errors. Use
     * vatCapsFastPos instead.
     */
    vatCapsInterminPos      = (1 << 5),

    /**
     * Can send/receive fast position updates (pilot only)
     */
    vatCapsFastPos          = (1 << 6),

    /**
     * Stealth mode
     */
    vatCapsStealth          = (1 << 7),

    /**
     * Aircraft Config
     */
    vatCapsAircraftConfig   = (1 << 8)
}
VatCapabilities;


/***************************************************************************
    PROTOCOL CONSTANTS
 ***************************************************************************/

/** VATSIM-standard unicom */
#define VAT_FREQUENCY_UNICOM 122.8

/** Emergency channel */
#define VAT_FREQUENCY_GUARD 121.5

/** ATC party-line */
#define VAT_FREQUENCY_ATC 149.999


/** Server error codes
*/
typedef enum
{
    vatServerErrorNone,             /**< No error */
    vatServerErrorCsInUs,           /**< Callsign in use */
    vatServerErrorCallsignInvalid,  /**< Callsign invalid */
    vatServerErrorRegistered,       /**< Already registered */
    vatServerErrorSyntax,           /**< Syntax error */
    vatServerErrorSrcInvalid,       /**< Invalid source in packet */
    vatServerErrorCidInvalid,       /**< Invalid CID/password */
    vatServerErrorNoSuchCs,         /**< No such callsign */
    vatServerErrorNoFP,             /**< No flightplan */
    vatServerErrorNoWeather,        /**< No such weather profile */
    vatServerErrorRevision,         /**< Invalid protocol revision */
    vatServerErrorLevel,            /**< Requested level too high */
    vatServerErrorServFull,         /**< No more clients */
    vatServerErrorCsSuspended,      /**< CID/PID was suspended */
    vatServerErrorInvalidCtrl,      /**< Not valid control */
    vatServerErrorInvPos,           /**< Invalid position for rating */
    vatServerErrorUnAuth,           /**< Not authorized software */
    vatServerWrongType,             /**< Wrong server type */
    vatErrorUnknown                 /**< Unknown error */
}
VatServerError;

/** Client type
*/
typedef enum
{
    vatUnknownClient,   /**< Unknown type */
    vatPilot,           /**< Pilot client type */
    vatAtc              /**< ATC client type */
}
VatClientType;

/** Transponder modes
*/
typedef enum
{
    vatTransponderModeStandby,  /**< Transponder is off, or in standby. */
    vatTransponderModeCharlie,  /**< Transponder is on mode C, not identing. */
    vatTransponderModeIdent     /**< Transponder is on mode C and identing. */
}
VatTransponderMode;

/** ATC ratings
*/
typedef enum
{
    vatAtcRatingUnknown,        /**< Unknown */
    vatAtcRatingObserver,       /**< OBS */
    vatAtcRatingStudent,        /**< S1 */
    vatAtcRatingStudent2,       /**< S2 */
    vatAtcRatingStudent3,       /**< S3 */
    vatAtcRatingController1,    /**< C1 */
    vatAtcRatingController2,    /**< C2 */
    vatAtcRatingController3,    /**< C3 */
    vatAtcRatingInstructor1,    /**< I1 */
    vatAtcRatingInstructor2,    /**< I2 */
    vatAtcRatingInstructor3,    /**< I3 */
    vatAtcRatingSupervisor,     /**< SUP */
    vatAtcRatingAdministrator   /**< ADM */
}
VatAtcRating;

/** Pilot ratings.
*/
typedef enum
{
    vatPilotRatingUnknown,      /**< Unknown rating */
    vatPilotRatingStudent,      /**< P1 */
    vatPilotRatingVFR,          /**< P2 */
    vatPilotRatingIFR,          /**< P3 */
    vatPilotRatingInstructor,   /**< Instructor */
    vatPilotRatingSupervisor    /**< SUP */
}
VatPilotRating;

/** ATC facility type
*/
typedef enum
{
    vatFacilityTypeUnknown, /**< Unknown facility type */
    vatFacilityTypeFSS,     /**< FSS */
    vatFacilityTypeDEL,     /**< Delivery */
    vatFacilityTypeGND,     /**< Ground */
    vatFacilityTypeTWR,     /**< Tower */
    vatFacilityTypeAPP,     /**< Approach */
    vatFacilityTypeCTR      /**< Center */
}
VatFacilityType;

/** Flight simulator type
*/
typedef enum
{
    vatSimTypeUnknown,  /**< Unknown simulator type */
    vatSimTypeMSFS95,   /**< MS Flight Simulator 95 */
    vatSimTypeMSFS98,   /**< MS Flight Simulator 98 */
    vatSimTypeMSCFS,    /**< MSCFS (?) */
    vatSimTypeXPLANE,   /**< X-Plane */
    vatSimTypeAS2,      /**< AS2 */
    vatSimTypePS1       /**< PS1 */
}
VatSimType;

/** Flight rules
*/
typedef enum
{
    vatFlightTypeIFR,   /**< IFR flight rules. */
    vatFlightTypeVFR,   /**< Visual flight rules. */
    vatFlightTypeSVFR,  /**< Special visual flight rules */
    vatFlightTypeDVFR   /**< Defense visual Flight Rules */
}
VatFlightType;

/** Client query types
*/
typedef enum
{
    vatClientQueryFP,     /**< Flight plan (pilots only, reply is a flight plan mesasge */
    vatClientQueryFreq,   /**< Frequency (pilots only) */
    vatClientQueryInfo,   /**< User Info (must be supervisor) */
    vatClientQueryAtis,   /**< ATIS (controllers only, reply is a text message */
    vatClientQueryServer, /**< What server is this client on */
    vatClientQueryName,   /**< Real name */
    vatClientQueryAtc,    /**< Is this client working ATC or just an observer (ATC only) */
    vatClientQueryCaps,   /**< What capabilities does this client have */
    vatClientQueryIP,     /**< What IP address am I sending from? */
}
VatClientQueryType;

/** Aircraft engine type
*/
typedef enum
{
    vatEngineTypePiston,    /**< Piston */
    vatEngineTypeJet,       /**< Jet */
    vatEngineTypeNone,      /**< None */
    vatEngineTypeHelo       /**< Helo */
}
VatEngineType;

/** Operations for a land line
*/
typedef enum
{
    vatLandlineCmdRequest,  /**< Request landline with other client. */
    vatLandlineCmdApprove,  /**< Approve a request for a landline connection. */
    vatLandlineCmdReject,   /**< Reject a request for a landline. */
    vatLandlineCmdEnd       /**< Terminate an in-progress landline. */
}
VatLandlineCmd;

/** Types of landlines
*/
typedef enum
{
    vatLandlineTypeIntercom,    /**< Intercom, a basic two-way telephone call. */
    vatLandlineTypeOverride,    /**< Override. Receiver doesn't have to key mic. */
    vatLandlineTypeMonitor      /**< Monitor - one way send back to the initiator. */
}
VatLandlineType;

/** Tracking commands
*/
typedef enum
{
    vatTrackingCmdStartTrack,   /**< Start tracking */
    vatTrackingCmdDropTrack,    /**< Drop tracking */
    vatTrackingCmdIHave,        /**< I'm tracking */
    vatTrackingCmdWhoHas,       /**< Who is tracking */
    vatTrackingCmdPointout,     /**< Point out a target on the radar screen */
    vatTrackingCmdDepartureList /**< Add target to departure list */
}
VatTrackingCmd;

/** Pilot position structure
*/
typedef struct
{
    double latitude;        /**< Latitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    double longitude;       /**< Longitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    int altitudeTrue;       /**< True altitude in feet above MSL. */
    int altitudePressure;   /**< Pressure altitude in feet above MSL. */
    int groundSpeed;        /**< Ground speed in knots. */
    double heading;         /**< Heading in degrees, clockwise from true north, 0-359. */
    double bank;            /**< Bank in degrees, positive = roll right. */
    double pitch;           /**< Pitch in degrees, positive = pitch up. */
    int transponderCode;    /**< Transponder code. Valid values are between 0000-7777. */
    VatTransponderMode transponderMode; /**< Current transponder mode. Either standby, charlie or ident. */
    VatPilotRating rating;  /**< Pilot rating. This is not yet used in VATSIM. So VATSIM expects every pilot client to send vatPilotRatingStudent only. */
}
VatPilotPosition;

/** Interim pilot position structure
*/
typedef struct
{
    double latitude;    /**< Latitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    double longitude;   /**< Longitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    int altitudeTrue;   /**< True altitude in feet above MSL. */
    int groundSpeed;    /**< Ground speed in knots. */
    double heading;     /**< Heading in degrees, clockwise from true north, 0-359. */
    double bank;        /**< Bank in degrees, positive = roll right. */
    double pitch;       /**< Pitch in degrees, positive = pitch up. */
}
VatInterimPilotPosition;


/** ATC position structure
*/
typedef struct
{
    int frequency;              /**< ATC frequency in khz. */
    VatFacilityType facility;   /**< Facility type */
    int visibleRange;           /**< Visible range in nm */
    VatAtcRating rating;        /**< ATC rating */
    double latitude;            /**< Latitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    double longitude;           /**< Longitude in decimal degrees. Precision shall be minimum 5 fractional digits. */
    int elevation;              /**< Elevation AGL in feet. */
}
VatAtcPosition;


/** Pilot Connection information
*/
typedef struct
{
    const char *callsign;       /**< Pilots callsign. */
    const char *name;           /**< Pilots real name. */
    VatSimType simType;         /**< Simulator type. */
    VatPilotRating rating;      /**< Pilot rating. This is not yet used in VATSIM. So VATSIM expects every pilot
                                     client to send vatPilotRatingStudent only. This must also be consistent with what is
                                     sent in @see VatPilotPosition.
                                 */
}
VatPilotConnection;

/** ATC Connection information
*/
typedef struct
{
    const char *callsign;   /** Controllers callsign */
    const char *name;       /** Controllers real name */
    VatAtcRating rating;    /** Controller rating. This must also be consistent with what is
                                sent in @see VatAtcPosition.
                            */
}
VatAtcConnection;

/** A single temperature layer
*/
typedef struct
{
    int ceiling;    /**< Top of temperature layer in feet MSL */
    int temp;       /**< Temperature in degrees celsius */
}
VatTempLayer;

/** A single wind layer
*/
typedef struct
{
    int ceiling;    /**< Wind ceiling in ft MSL. */
    int floor;      /**< Wind floor in ft MSL. */
    int direction;  /**< Wind direction in degrees clockwise from true north. */
    int speed;      /**< Wind speed in knots. */
    int gusting;    /**< 1 = true, 0 = false. */
    int turbulence; /**< 0 - 255, 0 = none, 255 = extreme. */
}
VatWindLayer;

/** A single cloud layer.
*/
typedef struct
{
    int ceiling;    /**< Cloud ceiling in ft MSL */
    int floor;      /**< Cloud floor in ft MSL. */
    int coverage;   /**< Cloud coverage. 0 - 8 octets, 0 = clear, 8 = overcast. */
    int icing;      /**< Icing 1 = true, 0 = false. */
    int turbulence; /**< Turbulence 0 - 255, 0 = none, 255 = extreme. */
}
VatCloudLayer;

/** A thunderstorm layer.
*/
typedef struct
{
    int ceiling;    /**< Thunderstorm ceiling in ft MSL */
    int floor;      /**< Thunderstorm floor in ft MSL. */
    int coverage;   /**< Thunderstorm coverage. 0 - 8 octets, 0 = clear, 8 = overcast. */
    int deviation;  /**< Icing 1 = true, 0 = false. */
    int turbulence; /**< Turbulence 0 - 255, 0 = none, 255 = extreme. */
}
VatThunderStormLayer;


/** A flight plan structure sent or received from the server.
 @note All pointers must be valid and cannot be NULL. Pass empty strings for empty input.
*/
typedef struct
{
    VatFlightType flightType;       /**< Flight type, IFR, VFR, etc. */
    const char *aircraftType;       /**< Aircraft ICAO type e.g. T/B734/F */
    int trueCruisingSpeed;          /**< True cruising speed in knkots */
    const char *departAirport;      /**< ICAO code of departure airport */
    int departTime;                 /**< Departure time in UTC, e.g. 2030 = 8:30 pm GMT */
    int departTimeActual;           /**< Actual departure time in UTC, e.g. 2030 = 8:30 pm GMT */
    const char *cruiseAltitude;     /**< Planned cruising altitude. FL230 or 15000 */
    const char *destAirport;        /**< ICAO code of destination airport */
    int enrouteHrs;                 /**< Enroute time - hours */
    int enrouteMins;                /**< Enroute time - minutes */
    int fuelHrs;                    /**< Available fuel - hours */
    int fuelMins;                   /**< Available fuel - minutes */
    const char *alternateAirport;   /**< ICAO code of alternate airport */
    const char *remarks;            /**< Flight plan remarks */
    const char *route;              /**< Flight route in capital letters. Separate with dots. */
}
VatFlightPlan;

/** Controller Atis structure
 @note All pointers must be valid and cannot be NULL. Pass empty strings for empty input.
*/
typedef struct
{
    const char *voiceRoom;  /**< Controller voice room in the form <url>/room */
    const char **textLines; /**< Controller messages lines. Maximum 4 */
    int textLineCount;      /**< Number of message lines */
    const char *zuluLogoff; /**< Controllers planned logoff time, e.g 20:00 UTC */
}
VatControllerAtis;

/** Aircraft information structure
 @note All pointers must be valid and cannot be NULL. Pass empty strings for empty input.
*/
typedef struct
{

    const char *aircraftType;   /**< Aircraft ICAO identifier. */
    const char *airline;        /**< Airline callsign. */
    const char *livery;         /**< Livery identifier. */
}
VatAircraftInfo;

/**
 Vatlib uses installed callback functions as parameters for some function calls,
 e.g. to set up notification handlers. The typedefs defined in this group describe the function parameters used to set
 up or clear the callback functions and should also be referenced to define the
 callback function to handle such events in the user's code.

 Each callback function has always two mandatory parameters. VatSessionID is the pointer to the session
 which called the callback. This can be used to identify the session object.
 The second mandatory parameter ref lets you any user defined data. As in C++ all callbacks have to be static
 functions, user defined data is typically set to the C++ object pointer. This allows you to forward the call
 from the static method to the listening C++ class.

@defgroup callback_functions Callback function typedefs
 \{
*/

/***************************************************************************
    Callbacks for all clients
 ***************************************************************************/


/***************************************************************************
    Callbacks for pilot clients
 ***************************************************************************/



/***************************************************************************
    Callbacks for ATC clients
 ***************************************************************************/

/** State change callback.
 @param session Session handler
 @param oldStatus Old connection status
 @param newStatus New connection status
 @param ref User defined data
*/
typedef void (* VatStateChangeHandler_f)(
    VatSessionID session,
    VatConnectionStatus oldStatus,
    VatConnectionStatus newStatus,
    void *ref);

/**
 Functions of type VatTextMessageHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when a private text message has been received.
 @param session Session pointer
 @param sender Callsign of the message sender
 @param receiver Callsign of the message receiver
 @param ref User defined data
*/
typedef void (* VatTextMessageHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *receiver,
    const char *message,
    void *ref);

/**
 Functions of type VatRadioMessageHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when a radio text message has been received. Vatlib does not
 know which frequency the user is listening to therefore this callback is called for every
 radio message. It is the responsibility of the client to filter relevant messages.
 Each radio message can be transmitted on more then one frequency.
 @param session Session pointer
 @param sender Callsign of the message sender
 @param freqCount The number of frequencies on which the text message was transmitted
 @param freqList Array of frequencies on which the text message was transmitted
 @param receiver Callsign of the message receiver
 @param ref User defined data
*/
typedef void (* VatRadioMessageHandler_f)(
    VatSessionID session,
    const char *sender,
    int freqCount,
    int *freqList,
    const char *message,
    void *ref);

/**
 Functions of type VatDeletePilotHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when a pilot has left the network.
 @param session Session pointer.
 @param sender Callsign of the pilot leaving the network.
 @param ref User defined data.
*/
typedef void (* VatDeletePilotHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
 Functions of type VatDeleteAtcHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when an ATC station has left the network.
 @param session Session pointer.
 @param sender Callsign of the ATC station leaving the network.
 @param ref User defined data.
*/
typedef void (* VatDeleteAtcHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
 Functions of type VatPilotPositionHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when a pilot's position has been reported.
 @param session Session pointer.
 @param sender Callsign of the pilot reporting its position.
 @param position Pilot position struct
 @param ref User defined data.
*/
typedef void (* VatPilotPositionHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatPilotPosition *position,
    void *ref);

/**
 Functions of type VatPilotPositionHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when a pilot's position has been reported.
 @param session Session pointer.
 @param sender Callsign of the pilot reporting its position.
 @param position Interim pilot position struct
 @param ref User defined data.
 @see VatInterimPilotPosition
*/
typedef void (* VatInterimPilotPositionHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatInterimPilotPosition *position,
    void *ref);

/**
 Functions of type VatAtcPositionHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when an ATC position has been reported.
 @param session Session pointer.
 @param sender Callsign of the ATC station reporting its position.
 @param position ATC position struct
 @param ref User defined data.
*/
typedef void (* VatAtcPositionHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatAtcPosition *position,
    void *ref);

/**
 Functions of type VatKillHandler_f are implemented by vatlib clients.
 Vatlib calls this callback to inform you have been kicked from the network. There
 is no further action requred. Both - vatlib and server - will close the connection
 immediatly.
 @param session Session pointer.
 @param reason Message from the supervisor, why you have been kicked. This can also be empty.
 @param ref User defined data.
*/
typedef void (* VatKillHandler_f)(
    VatSessionID session,
    const char *reason,
    void *ref);

/**
 Functions of type VatPongHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when your ping has been answered with a pong.
 @param session Session pointer.
 @param elapsedTime Elapsed time, after ping had been sent in milliseconds.
 @param ref User defined data.
*/
typedef void (* VatPongHandler_f)(
    VatSessionID session,
    const char *sender,
    double elapsedTime,
    void *ref);

/**
 Functions of type VatPongHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when your ping has been answered with a pong.
 @param session Session pointer.
 @param elapsedTime Elapsed time, after ping had been sent in milliseconds.
 @param ref User defined data.
*/
typedef void (* VatFlightPlanHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatFlightPlan *flightPlan,
    void *ref);

/**
 Functions of type VatHandoffRequestHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when another ATC client is requesting to handoff a target
 to you. You can either accept the handover or reject it.
 @param session Session pointer.
 @param target Targets callsign
 @param ref User defined data.
 @see Vat_AcceptHandoff, Vat_CancelHandoff
*/
typedef void (* VatHandoffRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatHandoffAcceptedHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when another ATC client has accepted the handoff of target
 from you.
 @param session Session pointer.
 @param target Targets callsign
 @param ref User defined data.
*/
typedef void (* VatHandoffAcceptedHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatHandoffCancelledHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when another ATC client has cancelled the handoff of target
 from you. You are still tracking it.
 @param session Session pointer.
 @param target Targets callsign
 @param ref User defined data.
*/
typedef void (* VatHandoffCancelledHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatMetarRequestHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when metar is requested from you for a ICAO station.
 This is never going to happen with VATSIM servers, since you always request METAR
 from the servers and not the other way round. But just in case someone needs it.
 @param session Session pointer.
 @param station Targets callsign
 @param ref User defined data.
 @see Vat_RequestMetar
*/
typedef void (* VatMetarRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *station,
    void *ref);

/**
 Functions of type VatMetarResponseHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when metar data has been received from the server as previously
 requested. It does not tell you which ICAO station the metar belongs to, so you better don't
 send several requests in parallel, but better wait until one request is answered.
 @param session Session pointer.
 @param metar METAR string
 @param ref User defined data.
 @see Vat_RequestMetar
*/
typedef void (* VatMetarResponseHandler_f)(
    VatSessionID session,
    const char *metar,
    void *ref);

/**
 Functions of type VatClientQueryHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when another client is requesting some information from you.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param type Type of information the remote client is asking for.
 @param ref User defined data.
*/
typedef void (* VatClientQueryHandler_f)(
    VatSessionID session,
    const char *sender,
    VatClientQueryType type,
    const char *data,
    void *ref);

/**
 Functions of type VatClientQueryResponseHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when the info request has been answered by another client to you.
 Function signature when an info reponse is received. For most info responses, only data is passed
 back, but for an ATC query about another ATC, data2 is also used.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param type Type of information the remote client is asking for.
 @param data Reponse data. The content depends heavily on the request type.
 @param data2 Some responses use a second data field.
 @todo Complete the descriptions for the different sub types
 @param ref User defined data.
*/
typedef void (* VatClientQueryResponseHandler_f)(
    VatSessionID session,
    const char *sender,
    VatClientQueryType type,
    const char *data,
    const char *data2,
    void *ref);

/**
 Functions of type VatInfoCAPSReplyHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when another client has sent his client capabilities.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param capabilityFlags A flag containing each supported capability.
 @param ref User defined data.
 @see VatCapabilities
*/
typedef void (* VatInfoCAPSReplyHandler_f)(
    VatSessionID session,
    const char *sender,
    int capabilityFlags,
    void *ref);

/**
 Functions of type VatVoiceRoomHandler_f are implemented by vatlib clients.
 Vatlib calls this callback when an ATC client has sent his voice room.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param voiceRoom Voiceroom string in the form of "<host>/room", e.g. "voice.somewhere.com/zzzz_twr".
 @param ref User defined data.
*/
typedef void (* VatVoiceRoomHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *voiceRoom,
    void *ref);

/**
 Functions of type VatControllerAtisHandler_f are implemented by vatlib ATC clients.
 Vatlib calls this callback when an ATC client has sent the controller ATIS. Be aware this
 is not the ATIS of an airport, but some detailed information about the controller itself
 (e.g. until when he is going to be online, voiceroom) and some information he wants to pass
 onto pilots (e.g. links to charts or initial calls advices).
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param atis The ATC controllers atis.
 @param ref User defined data.
 @see VatControllerAtis
*/
typedef void (* VatControllerAtisHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatControllerAtis *atis,
    void *ref);

/**
 Functions of type VatServerErrorHandler_f are implemented by all vatlib clients.
 Vatlib calls this callback when the server reported an error. This can be due to
 wrong login details, a malformed packet etc.
 @param session Session pointer.
 @param errorCode Exact code identifying the error.
 @param parameter String identifying the exact parameter which caused the error.
 @param description Some additional and helpful description what went wrong.
 @param ref User defined data.
 @see VatServerError
*/
typedef void (* VatServerErrorHandler_f)(
    VatSessionID session,
    VatServerError errorCode,
    const char *parameter,
    const char *description,
    void *ref);

/**
 Functions of type VatTemperatureDataHandler_f are implemented by vatlib pilot clients.
 Vatlib calls this callback when temperature layers for an ICAO station have been received
 as previously requested. Be aware that this callback does not say which ICAO station the
 data belongs to. So you better do not run multiple requests in parallel.
 @param session Session pointer.
 @param tempLayers Array of 4 temperature layers
 @param pressure The barometric pressure in the form 2992.
 @param ref User defined data.
 @see VatTempLayer
*/
typedef void (* VatTemperatureDataHandler_f)(
    VatSessionID session,
    const VatTempLayer tempLayers[4],
    int pressure,
    void *ref);

/**
 Functions of type VatWindDataHandler_f are implemented by vatlib pilot clients.
 Vatlib calls this callback when wind layers for an ICAO station have been received
 as previously requested. Be aware that this callback does not say which ICAO station the
 data belongs to. So you better do not run multiple requests for different stations in parallel.
 @param session Session pointer.
 @param windLayers Array of 4 wind layers
 @param pressure The barometric pressure in the form 2992.
 @param ref User defined data.
 @see VatWindLayer
*/
typedef void (* VatWindDataHandler_f)(
    VatSessionID session,
    const VatWindLayer windLayers[4],
    void *ref);

/**
 Functions of type VatCloudDataHandler_f are implemented by vatlib pilot clients.
 Vatlib calls this callback when cloud layers for an ICAO station have been received
 as previously requested. Be aware that this callback does not say which ICAO station the
 data belongs to. So you better do not run multiple requests for different stations in parallel.
 @param session Session pointer.
 @param cloudLayers Array of 2 cloud layers
 @param thunderStormLayer One extra layer in case of thunderstorms. If no thunderstorm is around,
 this layer will have invalid values.

 @param visibility Visibility in statute miles.
 @param ref User defined data.
 @see VatCloudLayer, VatThunderStormLayer
*/
typedef void (* VatCloudDataHandler_f)(
    VatSessionID session,
    const VatCloudLayer cloudLayers[2],
    VatThunderStormLayer thunderStormLayer,
    float visibility,
    void *ref);

/**
 Functions of type VatAircraftConfigHandler_f are implemented by vatlib pilot clients.
 Vatlib calls this callback when aircraft configuration has been received from another pilot
 client. Aircraft configuration is a JSON encoded object and does contain among other things:
 @li aircraft lights (on/off)
 @li aircraft engines (on/off)
 @li gear down/up
 @li aircraft on ground (true/false)
 @li flaps between 0 - 100 %.
 @li ...

 @param session Session pointer.
 @param sender Callsign of the sending client
 @param aircraftConfig JSON encoded aircraft config object.
 @param ref User defined data.
*/
typedef void (* VatAircraftConfigHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *aircraftConfig,
    void *ref);

/**
 Functions of type VatCustomAtcPacketHandler_f are implemented by vatlib ATC clients.
 Vatlib calls this callback when a custom ATC client communication packet
 has been received, which was unknown to vatlib. It therefore forwards the packet type
 plus all tokens to the client. This allows clients to implement custom packages and
 handle them themselves.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param subType String identifying the type of custom packet.
 @param data Array of const char pointers. Each pointer points to a packet token string.
 @param dataSize The size of the const char* array.
 @param ref User defined data.
*/
typedef void (* VatCustomAtcPacketHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *subType,
    const char **data,
    int dataSize,
    void *ref);

/**
 Functions of type VatCustomPilotPacketHandler_f are implemented by pilot clients.
 Vatlib calls this callback when a custom pilot client communication packet
 has been received, which was unknown to vatlib. It therefore forwards the packet type
 plus all tokens to the client. This allows clients to implement custom packages and
 handle them themselves.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param subType String identifying the type of custom packet.
 @param data Array of const char pointers. Each pointer points to a packet token string.
 @param dataSize The size of the const char* array.
 @param ref User defined data.
*/
typedef void (* VatCustomPilotPacketHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *subType,
    const char **data,
    int dataSize,
    void *ref);

/**
 Functions of type VatAircraftInfoRequestHandler_f are implemented by pilot clients.
 Vatlib calls this callback when a aircraft info request is received. A aircraft
 info request needs to be responded by sending the modern plane info.
 @param session Session pointer.
 @param sender Callsign of the sending client
 @param ref User defined data.
 @see Vat_SendAircraftInfo
*/
typedef void (* VatAircraftInfoRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
 Functions of type VatAircraftInfoResponseHandler_f are implemented by pilot clients.
 Vatlib calls this callback when a aircraft info response is received. A aircraft
 info reponse is the answer to an aircraft info request. It provides you in the best case
 information about the other clients aircraft type, airline and livery. Other clients
 might have disabled to send this information, so it could be that you receive only
 some.
 This is the modern version. Older clients used to send a different packet as reponse.
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param aircraftInfo Aircraft info struct.
 @param ref User defined data.
 @see VatAircraftInfo, VatLegacyAircraftInfoHandler_f
*/
typedef void (* VatAircraftInfoResponseHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatAircraftInfo *aircraftInfo,
    void *ref);

/**
 Functions of type VatLegacyAircraftInfoHandler_f are implemented by pilot clients.
 Vatlib calls this callback when a aircraft info response is received. A aircraft
 info reponse is the answer to an aircraft info request. It provides you information
 about the engine type and the used CSL model.
 This is the legacy version of the reponse. Modern clients use to send a different packet as reponse.
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param aircraftInfo Aircraft info struct.
 @param ref User defined data.
 @see VatAircraftInfoResponseHandler_f
*/
typedef void (* VatLegacyAircraftInfoHandler_f)(
    VatSessionID session,
    const char *sender,
    VatEngineType engineType,
    const char *cslModel,
    void *ref);

/**
 Functions of type VatIPCHandler_f are implemented by ??? clients.
 @todo complete the description
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param ipcCommand
 @param ipcSlot
 @param ipcValue
 @param ref User defined data.
*/
typedef void (* VatIPCHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *ipcCommand,
    int ipcSlot,
    int ipcValue,
    void *ref);

/**
 Functions of type VatTargetsScratchpadHandler_f are implemented by ATC clients.
 Vatlib calls this callback when the targets scratchpad has been changed by another ATC client.
 Use this callback to update your local scratchpad.
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param target Callsign of the target.
 @param scratchpad New content of the scratchpad
 @param ref User defined data.
 @see VatAircraftInfoResponseHandler_f
*/
typedef void (* VatTargetsScratchpadHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    const char *scratchpad,
    void *ref);

/**
 Functions of type VatHandleLandlineCommandHandler_f are implemented by ??? clients.
 @todo complete the description
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param command
 @param type
 @param ip
 @param port
 @param ref User defined data.
*/
typedef void (* VatLandlineCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    VatLandlineCmd command,
    VatLandlineType type,
    const char *ip,
    int port,
    void *ref);

/**
 Functions of type VatControllerBreakCommandHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller requested a break or stopped the break.
 If break mode is active, the ATC client shall display the ATC controller different
 and no handover shall be initiated.
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param wantsBreak True if the controller requested a break and false if the break is over
 @param ref User defined data.
*/
typedef void (* VatControllerBreakCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    bool wantsBreak,
    void *ref);

/**
    Function signature when a nex-gen client is ID'ing us
 */
typedef void (* VatSharedStateIDHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
    Function signature when a nex-gen client is replying to our ID
 */
typedef void (* VatSharedStateDIHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
 Functions of type VatPushToDepartureListHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller wants to push a target into our
 depature list. The receiving client shall add the target into the departure list,
 if it is not already there.
 @todo Callback Setter and send method
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param target Callsign of the target.
 @param ref User defined data.
*/
typedef void (* VatPushToDepartureListHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatPointOutHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller wants to point out a target onto our
 radar screen. The receiving client shall highlight target on the radar screen.
 @todo Callback Setter and send method
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param target Callsign of the target.
 @param ref User defined data.
*/
typedef void (* VatPointOutHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatIHaveTargetHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller answered that he is currently
 tracking the target. This is typically the reponse to the request who is tracking
 the target.
 @todo Callback Setter and send method
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param target Callsign of the target.
 @param ref User defined data.
*/
typedef void (* VatIHaveTargetHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
 Functions of type VatPushFlightStripHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller wants to push a flight strip
 into our flight strip bay. Receiving ATC clients shall add the flight strip as
 requested and with all the information received from the packet.
 @todo Callback Setter and send method
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param target Callsign of the target.
 @param type Type of the flight strip
 @param annotations Array of strings with flight strip annotations
 @param ref User defined data.
*/
typedef void (* VatPushFlightStripHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    int type,
    const char **annotations,
    void *ref);

/**
 Functions of type VatHelpCommandHandler_f are implemented by ATC clients.
 Vatlib calls this callback when an ATC controller requested help or stopped it.
 If help mode is active, the ATC client shall display the ATC controller in a different
 way.
 @param session Session pointer.
 @param sender Callsign of the sending client.
 @param wantsHelp True if the controller requested help and false if the request is over.
 @param message Additional details about the help request.
 @param ref User defined data.
*/
typedef void (* VatHelpCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    bool wantsHelp,
    const char *message,
    void *ref);

/**
 Functions of type VatFsdMessageHandler_f can be implemented by any client.
 @param session Session pointer.
 @param message Raw FSD message
 @param ref User defined data.
*/
typedef void (* VatFsdMessageHandler_f)(
    VatSessionID session,
    const char *message,
    void *ref);

/** \} */ /* group callback_functions */


/***************************************************************************
    CALLBACK INSTALLERS
 ***************************************************************************/
/**
 These functions allow you to associate a callback with a session. Install
 a NULL handler to delete a callback. You pass in a void "reference ptr"
 that is returned to you in your callback (in the ref field). This lets
 you find local variables attached to this particular instance of a session.
*/

/**
 Installs the NetworkLogHandler callback. Whenever vatlib is logging something
 from the network side, this callback will be called.
 It does not have any user defined field yet.
 @param maximumSeverityLevel The maximal severity you want to receive.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatlibLogHandler_t
*/
VATLIB_API void Vat_SetNetworkLogHandler(
        SeverityLevel maximumSeverityLevel,
        VatlibLogHandler_t handler);

/**
 Sets the maximal severity to a new value.
*/
VATLIB_API void Vat_SetNetworkLogSeverityLevel(
        SeverityLevel maximumSeverityLevel);

/**
 Installs the StateChangeHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatStateChangeHandler_f
*/
VATLIB_API void Vat_SetStateChangeHandler(
    VatSessionID session,
    VatStateChangeHandler_f handler,
    void *ref);

/**
 Installs the TextMessageHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatTextMessageHandler_f
*/
VATLIB_API void Vat_SetTextMessageHandler(
    VatSessionID session,
    VatTextMessageHandler_f handler,
    void *ref);

/**
 Installs the RadioMessageHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatRadioMessageHandler_f
*/
VATLIB_API void Vat_SetRadioMessageHandler(
    VatSessionID session,
    VatRadioMessageHandler_f handler,
    void *ref);

/**
 Installs the PilotPositionHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatPilotPositionHandler_f
*/
VATLIB_API void Vat_SetPilotPositionHandler(
    VatSessionID session,
    VatPilotPositionHandler_f handler,
    void *ref);

/**
 Installs the SetInterimPilotPositionHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatInterimPilotPositionHandler_f
*/
VATLIB_API void Vat_SetInterimPilotPositionHandler(
    VatSessionID session,
    VatInterimPilotPositionHandler_f handler,
    void *ref);

/**
 Installs the AtcPositionHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatAtcPositionHandler_f
*/
VATLIB_API void Vat_SetAtcPositionHandler(
    VatSessionID session,
    VatAtcPositionHandler_f handler,
    void *ref);

/**
 Installs the DeletePilotHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatDeletePilotHandler_f
*/
VATLIB_API void Vat_SetDeletePilotHandler(
    VatSessionID session,
    VatDeletePilotHandler_f handler,
    void *ref);

/**
 Installs the DeleteAtcHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatDeleteAtcHandler_f
*/
VATLIB_API void Vat_SetDeleteAtcHandler(
    VatSessionID session,
    VatDeleteAtcHandler_f handler,
    void *ref);

/**
 Installs the KillHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatKillHandler_f
*/
VATLIB_API void Vat_SetKillHandler(
    VatSessionID session,
    VatKillHandler_f handler,
    void *ref);

/**
 Installs the PongHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatPongHandler_f
*/
VATLIB_API void Vat_SetPongHandler(
    VatSessionID session,
    VatPongHandler_f handler,
    void *ref);

/**
 Installs the FlightPlanHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatFlightPlanHandler_f
*/
VATLIB_API void Vat_SetFlightPlanHandler(
    VatSessionID session,
    VatFlightPlanHandler_f handler,
    void *ref);

/**
 Installs the HandoffRequestHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatHandoffRequestHandler_f
*/
VATLIB_API void Vat_SetHandoffRequestHandler(
    VatSessionID session,
    VatHandoffRequestHandler_f handler,
    void *ref);

/**
 Installs the HandoffAcceptedHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatHandoffAcceptedHandler_f
*/
VATLIB_API void Vat_SetHandoffAcceptedHandler(
    VatSessionID session,
    VatHandoffAcceptedHandler_f handler,
    void *ref);

/**
 Installs the HandoffCancelledHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatHandoffCancelledHandler_f
*/
VATLIB_API void Vat_SetHandoffCancelledHandler(
    VatSessionID session,
    VatHandoffCancelledHandler_f handler,
    void *ref);

/**
 Installs the MetarRequestHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatMetarRequestHandler_f
*/
VATLIB_API void Vat_SetMetarRequestHandler(
    VatSessionID session,
    VatMetarRequestHandler_f handler,
    void *ref);

/**
 Installs the MetarResponseHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatMetarResponseHandler_f
*/
VATLIB_API void Vat_SetMetarResponseHandler(
    VatSessionID session,
    VatMetarResponseHandler_f handler,
    void *ref);

/**
 Installs the InfoRequestHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatInfoRequestHandler_f
*/
VATLIB_API void Vat_SetClientQueryHandler(
    VatSessionID session,
    VatClientQueryHandler_f handler,
    void *ref);

/**
 Installs the ClientQueryResponseHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatInfoResponseHandler_f
*/
VATLIB_API void Vat_SetClientQueryResponseHandler(
    VatSessionID session,
    VatClientQueryResponseHandler_f handler,
    void *ref);

/**
 Installs the InfoCAPSReplyHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatInfoCAPSReplyHandler_f
*/
VATLIB_API void Vat_SetInfoCAPSReplyHandler(
    VatSessionID session,
    VatInfoCAPSReplyHandler_f handler,
    void *ref);

/**
 Installs the VoiceRoomHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatVoiceRoomHandler_f
*/
VATLIB_API void Vat_SetVoiceRoomHandler(
    VatSessionID session,
    VatVoiceRoomHandler_f handler,
    void *ref);

/**
 Installs the ControllerAtisHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatControllerAtisHandler_f
*/
VATLIB_API void Vat_SetControllerAtisHandler(
    VatSessionID session,
    VatControllerAtisHandler_f handler,
    void *ref);

/**
 Installs the TemperatureDataHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatTemperatureDataHandler_f
*/
VATLIB_API void Vat_SetTemperatureDataHandler(
    VatSessionID session,
    VatTemperatureDataHandler_f handler,
    void *ref);

/**
 Installs the WindDataHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatWindDataHandler_f
*/
VATLIB_API void Vat_SetWindDataHandler(
    VatSessionID session,
    VatWindDataHandler_f handler,
    void *ref);

/**
 Installs the CloudDataHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatCloudDataHandler_f
*/
VATLIB_API void Vat_SetCloudDataHandler(
    VatSessionID session,
    VatCloudDataHandler_f handler,
    void *ref);

/**
 Installs the AircraftConfigHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatAircraftConfigHandler_f
*/
VATLIB_API void Vat_SetAircraftConfigHandler(
    VatSessionID session,
    VatAircraftConfigHandler_f handler,
    void *ref);

/**
 Installs the CustomAtcPacketHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatCustomAtcPacketHandler_f
*/
VATLIB_API void Vat_SetCustomAtcPacketHandler(
    VatSessionID session,
    VatCustomAtcPacketHandler_f handler,
    void *ref);

/**
 Installs the CustomPilotPacketHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatCustomPilotPacketHandler_f
*/
VATLIB_API void Vat_SetCustomPilotPacketHandler(
    VatSessionID session,
    VatCustomPilotPacketHandler_f handler,
    void *ref);

/**
 Installs the AircraftInfoRequestHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatAircraftInfoRequestHandler_f
*/
VATLIB_API void Vat_SetAircraftInfoRequestHandler(
    VatSessionID session,
    VatAircraftInfoRequestHandler_f handler,
    void *ref);

/**
 Installs the AircraftInfoHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatAircraftInfoResponseHandler_f
*/
VATLIB_API void Vat_SetAircraftInfoHandler(
    VatSessionID session,
    VatAircraftInfoResponseHandler_f handler,
    void *ref);

/**
 Installs the LegacyAircraftInfoHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatLegacyAircraftInfoHandler_f
*/
VATLIB_API void Vat_SetLegacyAircraftInfoHandler(
    VatSessionID session,
    VatLegacyAircraftInfoHandler_f handler,
    void *ref);

/**
 Installs the IPCHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatIPCHandler_f
*/
VATLIB_API void Vat_SetIPCHandler(
    VatSessionID session,
    VatIPCHandler_f handler,
    void *ref);

/**
 Installs the TargetScratchpadHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatTargetsScratchpadHandler_f
*/
VATLIB_API void Vat_SetTargetScratchpadHandler(
    VatSessionID session,
    VatTargetsScratchpadHandler_f handler,
    void *ref);

/**
 Installs the LandlineCommandHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatStateChangeHandler_f
*/
VATLIB_API void Vat_SetLandlineCommandHandler(
    VatSessionID session,
    VatLandlineCommandHandler_f handler,
    void *ref);

/**
 Installs the ControllerBreakCommandHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatControllerBreakCommandHandler_f
*/
VATLIB_API void Vat_SetControllerBreakCommandHandler(
    VatSessionID session,
    VatControllerBreakCommandHandler_f handler,
    void *ref);

/**
 Installs the SharedStateIDHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatSharedStateIDHandler_f
*/
VATLIB_API void Vat_SetSharedStateIDHandler(
    VatSessionID session,
    VatSharedStateIDHandler_f handler,
    void *ref);

/**
 Installs the SharedStateDIHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatSharedStateDIHandler_f
*/
VATLIB_API void Vat_SetSharedStateDIHandler(
    VatSessionID session,
    VatSharedStateDIHandler_f handler,
    void *ref);

/**
 Installs the PushFlightStripHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatPushFlightStripHandler_f
*/
VATLIB_API void Vat_SetPushFlightStripHandler(
    VatSessionID session,
    VatPushFlightStripHandler_f handler,
    void *ref);

/**
 Installs the HelpCommandHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatStateChangeHandler_f
*/
VATLIB_API void Vat_SetHelpCommandHandler(
    VatSessionID session,
    VatHelpCommandHandler_f handler,
    void *ref);

/**
 Installs the ServerErrorHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatServerErrorHandler_f
*/
VATLIB_API void Vat_SetServerErrorHandler(
    VatSessionID session,
    VatServerErrorHandler_f handler,
    void *ref);

/**
 Installs the FsdMessageHandler callback.
 @param session The session the callback should be installed to.
 @param handler Pass a function pointer to install or nullptr to delete it.
 @param ref Pointer to user defined data, which will be passed to the callback
 @warning Your callback must be thread safe, because it is not guaranteed that
 the callback is always called from the main thread.
 @see VatFsdMessageHandler_f
*/
VATLIB_API void Vat_SetFsdMessageHandler(
    VatSessionID session,
    VatFsdMessageHandler_f handler,
    void *ref);

/**
 Dump all raw FSD messages to file.
 @param session The session the callback should be installed to.
 @param FilePath can be a filename, relative or absolute path. If filePath is nullptr, dumping is stopped.
*/
VATLIB_API void Vat_DumpFsdMessageToFile(
        VatSessionID session,
        const char *filePath);

/** Execute all pending network tasks synchronous. Call this method
 regularly to keep vatlib busy. This method will actually trigger most of the callbacks.
*/
VATLIB_API int Vat_ExecuteNetworkTasks(
    VatSessionID session);

/** Execute all pending network tasks asynchronous. Call this method
 only once and vatlib will run in the background. Most callbacks will be called from a different
 thread in this case. In order to stop, call @see Vat_StopAsyncNetworkExecution.
 @param session
*/
VATLIB_API void Vat_ExecuteNetworkTasksAsync(
    VatSessionID session);

/** Stops asynchronous execution
 @param session
*/
VATLIB_API void Vat_StopAsyncNetworkExecution(
    VatSessionID session);

/**
 Vat_CreateNetworkSession creates a new networking session. Most applications will have only one session however
 someday this may change.

 @param clientVersionString  A string name for your application including version number. Eg. "XTower 1.0". This information
 will be sent to other clients who may query your version.

 @param clientMajorVersion Integer major version number eg. the "1" in 1.3
 @param clientMinorVersion Integer minor version number eg. the "3" in 1.3
 @param hostVersion A string name for the host application. If you're a plugin for Flight Sim, you'd put the MSFS name here.
 @param publicClientID Your public client ID as issued by the VATSIM administrator. Leave NULL for other networks.
 @param privateKey Your private key as issued by the VATSIM administrator. Leave NULL for other networks.
 @param clientCAPS Integer flags which capabilities this clients has.

 @todo clientCAPS should be a bitflag instead of a string
 @return A session ID which is essentialy a pointer to your session instance.
 @see Vat_DestroyNetworkSession
*/
VATLIB_API VatSessionID Vat_CreateNetworkSession(
    VatServerType serverType,
    const char *clientVersionString,
    int clientMajorVersion,
    int clientMinorVersion,
    const char *hostApplication,
    int publicClientID,
    const char *privateKey,
    int clientCapabilities);

/**
 Vat_DestroyNetworkSession deallocates an existing session. This should be done when a session is
 disconnected and will not be used anymore.

 @param session The session ID of the session that you want to terminate.
 @return None
 @see Vat_CreateNetworkSession
*/
VATLIB_API void Vat_DestroyNetworkSession(VatSessionID session);

/**
 Vat_SpecifyPilotLogon is used to declare the information for a pilot connection to the network.
 This does NOT connect you to the network by itself.

 @param session The session ID to be used for this operation.
 @param host A string containing the host address. It can be a string IP address or hostname.
 @param port The port to connect to the server on.
 @param id A string containing the CID of the pilot.
 @param password A string containing the password of the pilot.
 @param info A struct containing the pilot specific information necessary for the connection to the server.
 @return None
 @see VatPilotConnection_t
*/
VATLIB_API void Vat_SpecifyPilotLogon(
    VatSessionID session,
    const char *host,
    int port,
    const char *id,
    const char *password,
    const VatPilotConnection *info);

/**
 Vat_SpecifyATCLogon is used to declare the information for an ATC connection to the network.
 This does NOT connect you to the network by itself.

 @param session The session ID to be used for this operation.
 @param host A string containing the server address. It can be a string IP address or hostname.
 @param port A port > 0 to connect to the server on.
 @param id A string containing the CID of the controller.
 @param password A string containing the password of the controller.
 @param info A struct containing the controller specific information necessary for the connection to the server.
 @return None
 @see VatATCConnection_t
*/
VATLIB_API void Vat_SpecifyATCLogon(
    VatSessionID session,
    const char *host,
    int port,
    const char *id,
    const char *password,
    const VatAtcConnection *info);

/**
 Vat_Logon attempts to actually logon to the server or reconnect if disconnected.
 You must have already specified connection information using Vat_SpecifyPilotLogon or Vat_SpecifyATCLogon.

 @param session The session ID to be used for this operation.
 @see Vat_Logoff
 @see Vat_SpecifyPilotLogon
 @see Vat_SpecifyATCLogon
*/
VATLIB_API void Vat_Logon(VatSessionID session);

/**
 Vat_Logoff attempts to logoff from the network.

 @param session The session ID to be used for this operation.
 @return None
 @see Vat_Logon
*/
VATLIB_API void Vat_Logoff(VatSessionID session);

/**
 Vat_GetStatus reads the status of your connection. Note that you should avoid using this function because
 it is better to install a notifier to detect when a status has changed. This routine does NOT read new
 status changes so no work is done and no callbacks will get called.

 @param session  The session ID to be used for this operation.
 @return Current connection status
*/
VATLIB_API VatConnectionStatus Vat_GetStatus(VatSessionID session);


/***************************************************************************
    BASIC COMMUNICATIONS FUNCTIONS
 ***************************************************************************/

/**
 Send a private 1-to-1 chat message.

 @param session  The session ID to be used for this operation.
 @param receiver A string containing the receiver callsign
 @param message  A string containing the text message
 @return None
*/
VATLIB_API void Vat_SendTextMessage(
    VatSessionID session,
    const char *receiver,
    const char *message);

/**
 Send a radio message on one or more frequencies, specified as an array of ints.

 @param session The session ID to be used for this operation.
 @param frequencies Pointer to array of frequencies as integer values, e.g. 12345 for 123.45 MHz
 @param freqCount Size of frequency array
 @param message A string containing the text message
 @return None
*/
VATLIB_API void Vat_SendRadioMessage(
    VatSessionID session,
    int *frequencies,
    int freqCount,
    const char *message);

/**
 Send a message to all clients with rating SUP or ADM

 @param session The session ID to be used for this operation.
 @param message A string containing the text message
 @return None
*/
VATLIB_API void Vat_SendWallop(
    VatSessionID session,
    const char *message);

/**
 Used to request information from a client or server. Passing null as
 callsign will probe the server for information.

 @param session The session ID to be used for this operation.
 @param type Type of information you want to query
 @param callsign Client callsign, you want to probe
 @return None
*/
VATLIB_API void Vat_SendClientQuery(
    VatSessionID session,
    VatClientQueryType type,
    const char *callsign);

/**
 Reply to a client query.

 @param session The session ID to be used for this operation.
 @param type Type of information you want to request
 @param callsign Client callsign, you want to probe

 @param payload Array of data depending on the type of query response.
 The client is only allowed to send two types of client query responses:

 @li vatClientQueryName
 @li vatClientQueryFreq.

 The rest is handled internally in vatlib.

 vatClientQueryFreq:
 payload should have ONE string with the frequency in decimal, e.g. "119.90".
 The unit is per default MHz and does not need to be added.

 vatClientQueryInfo:
 Don't use it! Vatlib will send it itself. If you try anyway, it
 will be ignored.

 vatClientQueryAtis:
 Don't use it. Use Vat_SendControllerAtis instead.

 vatClientQueryServer:
 Don't use it! Vatlib will send it itself. If you try anyway, it
 will be ignored.

 vatClientQueryName:
 payload should contain two strings. The first one is the users name and is
 mandatory. The second one should contain any additional usefull information
 about the user himself. This is for example used by Euroscope to send the
 used AIRAC information. Leave it blank if you don't want to send anything.

 vatClientQueryAtc:
 Don't use it! Vatlib will send it itself. If you try anyway, it
 will be ignored.

 vatClientQueryCaps:
 Don't use it! Vatlib will send it itself. If you try anyway, it
 will be ignored.

 vatClientQueryIP:
 Don't use it! Vatlib will send it itself. If you try anyway, it
 will be ignored.

 @return None
 @see VatClientQueryType, Vat_SendControllerAtis
*/
VATLIB_API void Vat_SendClientQueryResponse(VatSessionID session,
    VatClientQueryType type,
    const char *callsign,
    const char **payload,
    int payloadSize);

/**
 Request CSL model info from someone else.

 @param session The session ID to be used for this operation.
 @param callsign Client callsign, you want to request from
 @return None
*/
VATLIB_API void Vat_RequestAircraftInfo(
    VatSessionID session,
    const char *callsign);

/***************************************************************************
    FUNCTIONS FOR ATC
 ***************************************************************************/

/**
 Send an ATC position update packet.

 @param session The session ID to be used for this operation.
 @param info Struct containing ATC position details
 @return None
 @see VatAtcPosition
*/
VATLIB_API void Vat_SendATCUpdate(
    VatSessionID session,
    const VatAtcPosition *info);

/**
 Requests an ATC Handoff.

 @param session The session ID to be used for this operation.
 @param receiver Controller callsign, from which you are requesting
 @param target Aircraft callsign, you would to hand off
 @return None
 @see VatAtcPosition
*/
VATLIB_API void Vat_RequestHandoff(VatSessionID session,
    const char *receiver,
    const char *target);

/**
 Accept an ATC Handoff

 @param session The session ID to be used for this operation.
 @param receiver Controller callsign, who requested the handoff
 @param target Aircraft callsign, which is handed off
 @return None
*/
VATLIB_API void Vat_AcceptHandoff(VatSessionID session,
    const char *receiver,
    const char *target);

/**
 Cancel a handoff with another ATC.

 @param session The session ID to be used for this operation.
 @param receiver Controller callsign, who requested the handoff
 @param target Aircraft callsign, which is handed off
 @return None
*/
VATLIB_API void Vat_CancelHandoff(
    VatSessionID session,
    const char *receiver,
    const char *target);

/**
 Amend a flight plan. The one stored on the server will be overwritten
 with the one you are sending now.

 @param session The session ID to be used for this operation.
 @param callsign Aircraft callsign, which flightplan needs to be amended.
 @param flightplan New full flightplan including all changed you have made.
 @return None
*/
VATLIB_API void Vat_AmendFlightPlan(
    VatSessionID session,
    const char *callsign,
    const VatFlightPlan *flightplan);

VATLIB_API void Vat_SendTargetsScratchpad(
    VatSessionID session,
    const char *receiver,
    const char *target,
    const char *scratchpad);

/**
 Send an ASRC-type ID to check if someone is a next-gen client.

 @param session The session ID to be used for this operation.
 @param atc
 @return None
*/
VATLIB_API void Vat_SendSharedStateID(
    VatSessionID session,
    const char *atc);

/**
 Send a land line command to other aircraft.

 @param session The session ID to be used for this operation.
 @param callsign Aircraft callsign
 @param command Landline command
 @param type landline command type
 @param ip
 @param port
 @return None
*/
VATLIB_API void Vat_SendLandlineCommand(
    VatSessionID session,
    const char *callsign,
    VatLandlineCmd command,
    VatLandlineType type,
    const char *ip,
    int port);

/**
 Send a tracking command.

 @param session The session ID to be used for this operation.
 @param atc ATC callsign, may be null if not needed.
 @param aircraft Aircraft callsign
 @param command Tracking command
 @return None
*/
VATLIB_API void Vat_SendTrackingCommand(
    VatSessionID session,
    const char *atc,
    const char *aircraft,
    VatTrackingCmd command);

/**
 Turns the break on and off

 @param session The session ID to be used for this operation.
 @param wantsBreak Pass true to turn the break on and false to turn it off.
 @return None
*/
VATLIB_API void Vat_SendBreakCommand(
    VatSessionID session,
    bool wantsBreak);

/**
 Send controller atis

 @param session The session ID to be used for this operation.
 @param receiver Receiver callsign
 @param atis Controller atis struct
 @return None
*/
VATLIB_API void Vat_SendControllerAtis(
    VatSessionID session,
    const char *receiver,
    const VatControllerAtis *atis);

/**
 Sends a secondary ATC position update

 @param session The session ID to be used for this operation.
 @param index ???
 @param latitude
 @param longitude
 @return None
*/
VATLIB_API void Vat_SendSecondaryATCUpdate(
    VatSessionID session,
    int index,
    double latitude,
    double longitude);

/**
 Pushes a pilots flight strip with a type identifier and array of annotations - Added by RAC on 02/28/2007

 @param session The session ID to be used for this operation.
 @param receiver Controller callsign the flight strip is pushed to.
 @param target Aircraft callsign, the flight strip belongs to.
 @param type ???
 @param annotations Array of strings with annotations
 @return None
*/
VATLIB_API void Vat_PushFlightStripEx(
    VatSessionID session,
    const char *receiver,
    const char *target,
    int type,
    const char **annotations);

/**
 Turns the request for help on and off

 @param session The session ID to be used for this operation.
 @param needHelp Pass true to turn the help request on and false to turn it off.
 @param message Pass in a optional message. May be null if not needed.
 @return None
*/
VATLIB_API void Vat_SendHelpCommand(
    VatSessionID session,
    bool needHelp,
    const char *message);

/**
 Notifies all in-range clients that our controller info or ATIS has changed.
 This will automatically increase the ATIS identifier for other ATC clients
 in range.

 @param session The session ID to be used for this operation.
 @return None
*/
VATLIB_API void Vat_NotifyNewInfo(
    VatSessionID session);

/***************************************************************************
    FUNCTIONS FOR PILOTS
 ***************************************************************************/

/**
 Send a pilot position update packet.

 @param session The session ID to be used for this operation.
 @param info Struct with pilot position details
 @return None
 @see VatPilotPosition
*/
VATLIB_API void Vat_SendPilotUpdate(
    VatSessionID session,
    const VatPilotPosition *info);

/**
 Send an interim pilot position update packet. This is sent to a specific
 callsign only instead of broadcasted.
 It depends on the receiving clients software if they accept intermin pilot
 position updates.

 @param session The session ID to be used for this operation.
 @param callsign Receiving callsign
 @param pilotPosition Struct with pilot position details
 @return None
 @see VatPilotPosition
*/
VATLIB_API void Vat_SendInterimPilotUpdate(
    VatSessionID session,
    const char *receiver,
    const VatInterimPilotPosition *pilotPosition);

/**
 Sends the server a flight plan.

 @param session The session ID to be used for this operation.
 @param flightPlan Your flightplan
 @return None
*/
VATLIB_API void Vat_SendFlightPlan(
    VatSessionID session,
    const VatFlightPlan *flightPlan);

/**
 Tell another client about your plane's info (CSL model, etc.)

 @param session The session ID to be used for this operation.
 @param receiver Receiver callsign
 @param engineType Engine Type
 @param CSLModel A string containing your CSL model
 @return None
 @deprecated Do not use this method anymore. This legacy is just
 here for SB2.3 clients. Most should use the modern one.
 @see Vat_SendAircraftInfo
*/
VATLIB_API void Vat_SendLegacyPlaneInfo(
    VatSessionID session,
    const char *receiver,
    VatEngineType engineType,
    const char *CSLModel);

/**
 Tell another client about your plane's info (CSL model, etc.).
 This is the modern send plane info that SB3, FSInn and XSB will use.

 @param session The session ID to be used for this operation.
 @param receiver Receiver callsign
 @param info Struct with plane information
 @return None
*/
VATLIB_API void Vat_SendAircraftInfo(
    VatSessionID session,
    const char *receiver,
    const VatAircraftInfo *info);

/**
 Send JSON encoded aircraft config to target callsign.

 @param session The session ID to be used for this operation.
 @param receiver Receiver callsign
 @param aircraftConfig JSON string
 @return None
*/
VATLIB_API void Vat_SendAircraftConfig(
    VatSessionID session,
    const char *receiver,
    const char *aircraftConfig);

/**
 Broadcast JSON encoded aircraft config to everyone in range.

 @param session The session ID to be used for this operation.
 @param aircraftConfig JSON string
 @return None
*/
VATLIB_API void Vat_SendAircraftConfigBroadcast(
    VatSessionID session,
    const char *aircraftConfig);

/**
 Send a custom pilot packet to target. This can be any subtype, as long
 as it does not clash with the existing subtypes which are known to vatlib.

 @param session The session ID to be used for this operation.
 @param receiver Receiver callsign
 @param subType Packet subtype
 @param data Array of strings containing the packet tokens
 @param dataSize Size of tokens
 @return None
*/
VATLIB_API void Vat_SendCustomPilotPacket(
    VatSessionID session,
    const char *receiver,
    const char *subType,
    const char **data,
    int dataSize);


/***************************************************************************
    ADMIN FUNCTIONS
 ***************************************************************************/

/**
 Kills a user off of the server with a message

 @param session The session ID to be used for this operation.
 @param callsign A string containing the callsign to be killed from the network
 @param message A string containing a message to the user
 @return None
*/
VATLIB_API void Vat_KillUser(
    VatSessionID session,
    const char *receiver,
    const char *message);

/**
 Sends a ping packet to the server.
 Pass in a callsign to ping a user, *A to ping all ATC or ?? to ping everyone.

 @param session The session ID to be used for this operation.
 @param receiver A string containing the receiver callsign
 @return None
*/
VATLIB_API void Vat_SendPing(
    VatSessionID session,
    const char *receiver);

/**
 Sends a pong back after receiving a ping request.

 @param session The session ID to be used for this operation.
 @param receiver A string containing the receiver callsign
 @param time Format???
 @deprecated This method is kept for backward compatibility, but doesn't not send
 anymore anything. A ping is answered by vatlib automatically. So no need to worry.
 @return None
*/
VATLIB_API void Vat_SendPong(
    VatSessionID session,
    const char *receiver,
    const char *time);

/***************************************************************************
    WEATHER FUNCTIONS
 ***************************************************************************/

/**
 Sends a Metar request.

 @param session The session ID to be used for this operation.
 @param station A string with ICAO identifier of the station
 @return None
*/
VATLIB_API void Vat_RequestMetar(
    VatSessionID session,
    const char *station);

/**
 Sends Metar data.

 @param session The session ID to be used for this operation.
 @param receiver A string containing the receiver callsign
 @param data Metar
 @return None
*/
VATLIB_API void Vat_SendMetar(
    VatSessionID session,
    const char *receiver,
    const char *metar);

/**
 Sends a request to the server for weather. The response from
 the server are detailed layers of wind, clouds and temperature.
 They are calculated from the local METAR and some additional
 woodoo. So their usage is questionable. Consider using
 @see Vat_RequestMetar instead.

 @param session The session ID to be used for this operation.
 @param station The station whether is requested for
 @return None
*/
VATLIB_API void Vat_RequestWeather(
    VatSessionID session,
    const char *station);

/****************************************************************
    GENERAL VVL NOTES
 ****************************************************************

 Your application is responsable for memory management of all
 Voice objects - each one has a Create and Delete routine.

 Like Network, most callbacks take a void parameter. You can
 use this to pass data from your call out of Voice to your
 callback.
*/

#if defined(__cplusplus) && defined(VVL_BUILDING_LIB)

class CAudioProducerConsumer;
class CAudioProducer;
class CAudioConsumer;
class UDPAudioPort;
class VoiceChannel;
class CDirectVoiceLink;
class CFileBasedRecorder;
class CLocalInputCodec;
class CLocalOutputCodec;
class CAudioMixer;
class CConcatenatedFilePlayer;
class CAudioService;
typedef CAudioProducerConsumer             *VatAudioProducerConsumer;
typedef CAudioProducer                     *VatAudioProducer;
typedef CAudioConsumer                     *VatAudioConsumer;
typedef UDPAudioPort                       *VatUDPAudioPort;
typedef VoiceChannel                       *VatVoiceChannel;
typedef CLocalInputCodec                   *VatLocalInputCodec;
typedef CLocalOutputCodec                  *VatLocalOutputCodec;
typedef CAudioMixer                        *VatAudioMixer;
typedef CAudioService                      *VatAudioService;

#else

struct VatUDPAudioPort_tag;
struct VatAudioService_tag;
struct VatProducer_tag;
struct VatConsumer_tag;
struct VatProducerConsumer_tag;
typedef struct VatProducerConsumer_tag         *VatAudioProducerConsumer;
typedef struct VatProducer_tag                 *VatAudioProducer;
typedef struct VatConsumer_tag                 *VatAudioConsumer;
typedef struct VatAudioService_tag             *VatAudioService;
typedef struct VatUDPAudioPort_tag             *VatUDPAudioPort;
typedef struct VatProducerConsumer_tag         *VatVoiceChannel;
typedef struct VatProducer_tag                 *VatLocalInputCodec;
typedef struct VatConsumer_tag                 *VatLocalOutputCodec;
typedef struct VatProducerConsumer_tag         *VatAudioMixer;

#endif

// Voice

enum
{
    vatVoiceStarted,
    vatVoiceStopped
};
typedef int VatVoiceTransmissionStatus;

/**
    VoiceLocalCodecAudioHandler_f

    This function is called when audio is playing from the mic, and can be used
    for primitive metering of incoming audio.

    OPEN ISSUE: it's unclear of exactly when this callback is made.
 */
typedef void (* VatLocalCodecAudioHandler_f)(
    int isAudioPlaying,
    void *ref);

/*
    VVLLocalCodecHardwareIterator_f
 *
    This callback is called once for each hardare device when you attempt
    to "enumerate" hardware.
 *
 */
typedef void (* VatLocalCodecHardwareIterator_f)(
    int deviceIndex,
    const char *hardwareName,
    void *ref);

/* The state of your voice room connection has changed. */
typedef void (* VatConnectionChangedHandler_f)(
    VatVoiceChannel channel,
    VatConnectionStatus oldStatus,
    VatConnectionStatus newStatus,
    void *ref);

/* A client has joined the voice room */
typedef void (* VatClientJoinedHandler_f)(
    VatVoiceChannel channel,
    int id,
    const char *callsign,
    void *ref);

/* A client has left the voice room */
typedef void (* VatClientLeftHandler_f)(
    VatVoiceChannel channel,
    int id,
    const char *callsign,
    void *ref);

/* The state of room voice has changed (started/stopped) */
typedef void (* VatVoiceTransmissionChangedHandler_f)(
    VatVoiceChannel channel,
    VatVoiceTransmissionStatus status,
    void *ref);

/*
    VVLVoiceChannelRoomIterator_f
 *
    This callback receipes info about each member of the room.
 *
 */
typedef void (* VatVoiceChannelRoomIterator_f)(
    VatVoiceChannel channel,
    int id,
    const char *name,
    void *ref);

VATLIB_API void Vat_SetVoiceLogHandler(SeverityLevel severityLevel, VatlibLogHandler_t handler);
VATLIB_API void Vat_SetVoiceLogSeverityLevel(SeverityLevel severityLevel);

/****************************************************************
    Audio Service Routines
 ****************************************************************/

VATLIB_API VatAudioService Vat_CreateAudioService();

VATLIB_API void Vat_DestroyAudioService(VatAudioService audioService);

/**
  \brief Execute all pending audio tasks synchronously.

  This routine does all processing necessary to run the voice
  subsystem.  Call this routine often! It will return the
  number of seconds until it needs to be called again.  You
  may receive a call back from this routine if you have notifiers
  functions installed on some of your objects.

  (Note: currently the time returned by Vat_ExecuteTasks may not
  be reliable. I suggest simply calling it on a fixed timer for
  now, perhaps 50 ms. This will be addressed in future lib
  updates.)

  During normal operations, this is the only vatlib routine you need
  to call - it takes care of all audio processing.

  \sa Vat_AsyncExecuteTasks
 */
VATLIB_API int Vat_ExecuteTasks(VatAudioService audioService);

/**
  \brief Start async audio processing

  This routine does all processing necessary to run the voice
  subsystem asynchronous. It will return the immediatly und run
  as until you stop it again. You may receive a call back from
  this routine if you have notifiers functions installed on
  some of your objects.

  \sa Vat_StopAsyncExecution
  \sa Vat_ExecuteTasks
 */
VATLIB_API void Vat_AsyncExecuteTasks(VatAudioService audioService);


/**
  \brief Stop async audio processing

  This routine will stop any asynchronous running audio processing.
  You won't hear any audio any more and recorded audio will not
  sent to the voice channel any more.

  \sa Vat_StopAsyncExecution
  \sa Vat_ExecuteTasks

 */
VATLIB_API void Vat_StopAsyncExecution(VatAudioService audioService);


/****************************************************************
    GENERAL Voice ROUTINES, CAudioProducerConsumer
 ****************************************************************/

/**
    \class VatAudioProducerConsumer

    The base type for all Voice audio objects is the
    VatAudioProducerConsumer.  This object represents an audio
    device that can both send and receive audio.

    Each producer and consumer has a number of ports, specified by
    64 bit integers.  You can receive audio on any port number, and
    does port numbering have to be contiguous. The idea is to allow
    an audio consumer to identify audio from different sources.

    Most audio devices can receive and send on only one port; port
    0 is used by convention.

    Input and output audio ports are separate and distinct; you
    can connect a device to one device for input on port 0 and
    another for output on port 0. To wire both input port 0 and
    output port 0 you must make two connections.

*/

/**
  \brief Vat_VoiceConnect

  This routine links an audio source and an audio destination.
  Specify the source and destination objects and the port numbers
  for each one. If the objects were previously connected, any
  old connections are severed.  You can connect a producer to a
  nullptr consumer to disconnect it.
  Important: (The producer cannot be nullptr.)
 */
VATLIB_API void Vat_ConnectProducerToConsumer(
    VatAudioProducer producer,
    int producerPort,
    VatAudioConsumer consumer,
    int consumerPort);

VATLIB_API void Vat_ConnectProducerConsumerToConsumer(
    VatAudioProducerConsumer producer,
    int producerPort,
    VatAudioConsumer consumer,
    int consumerPort);

VATLIB_API void Vat_ConnectProducerToProducerConsumer(
    VatAudioProducer producer,
    int producerPort,
    VatAudioProducerConsumer consumer,
    int consumerPort);

VATLIB_API void Vat_ConnectProducerConsumerToProducerConsumer(
    VatAudioProducerConsumer producer,
    int producerPort,
    VatAudioProducerConsumer consumer,
    int consumerPort);


/**
    \class VatUDPAudioPort

    This object is an actual UDP port that voice data is sent through.
    You should not have to use it directly; simply instantiate an object
    and pass it to any direct voice links and channels as you make them.

    A single UDP audio port is adequate for all voice configurations.
    The UDP audio port is capable of sorting out multiple audio streams
    all coming to the single UDP port.

*/

/**
    Vat_CreateUDPAudioPort creates a new UDP audio port. You should only
    need one of these in your client.

    Pass an existing @param audioService which will take care of task execution.

    @param port is the UDP port number you want to use. If you pass 0, an ephemeral port will
    be taken and is the recommended usage. You can also set a fixed port and vatlib will
    try to bind to it.

    @return Upon success Vat_CreateUDPAudioPort() returns a valid pointer to a
    VatUDPAudioPort object. If something went wrong (e.g. the fixed port is already taken)
    a NULL pointer will be returned and must be handeled by the calling application.
 */
VATLIB_API VatUDPAudioPort Vat_CreateUDPAudioPort(VatAudioService audioService, int port);

/**
    Vat_DestroyUDPAudioPort
 *
    Deallocates a VVLUDPAudioPort object. Delete your port on cleanup.

    @param audioPort A valid VatUDPAudioPort object.
 */
VATLIB_API void Vat_DestroyUDPAudioPort(VatUDPAudioPort audioPort);

/**
    Vat_GetPortNumber

    @param audioPort A valid VatUDPAudioPort object.

    @return This method returns the used port number. If a fixed port
    was given in Vat_CreateUDPAudioPort() and the allocation was
    successful, it will return the same port. If a ephemeral port
    was taken, this method returns the actual UDP port.

    @see Vat_CreateUDPAudioPort
 */
VATLIB_API int Vat_GetPortNumber(VatUDPAudioPort udpAudioPort);


/**
    \class VatLocalCodec

    The local codec object sends audio to and from a sound card,
    such as a microphone or speakers. It also contains the codec
    that encodes and decodes audio.

    A local codec can be built to have an input and output audio
    device or just an output device. Use port 0 for both input
    and output.

    The local codec also contains code to do automatic gain
    correction (AGC), Acoustic Echo Cancellation
    and hardware selection and has APIs to manage
    these functions.

    These are the result codes we get from the AGC after doing a full squelch
    test followed by a full mic test:.  Note that these are bit fields, more
    than one of these can go wrong at a time.  For example, it is likely that
    if the users talk is drowned out, that either the bkgnd noise will be too
    loud or the mic will be too cold.

    A good resolution procedure is probably:

    \li If we have a hot or cold mic (bits 2 or 3 set):
        Address hot and cold mics first before worrying about anything else.
        If any other bits are set (0 or 1) make this strongly suggested, otherwise
        it's more of an FYI.
    \li If we have a good mic level and we get bkgnd noise loud and a drown
        out ask user to turn down fans, flight sim noise, whatever.
    \li If we have a good mic and get drowned out without loud bkgnd noise
       ask user to talk louder or put mic closer to mouth.
    \li If we have a good mic level and we get bkgnd noise but no drown out
        warn the user that background noise may be an issue and to try
        to provide cleaner environment.  (Not as strict of a warning as (2)
 */

/**
    Vat_CreateLocalCodec
 *
    Create a new local codec object.  You can pass an audio handler if you
    wish to find out when audio is playing.  You can use noMic to create
    an output-only local codec, for example, "speakers".
 *
 */

/** Audio codec
*/
typedef enum
{
    vatCodecLegacy  = (1 << 0), /**< Legacy codec */
    vatCodecOpus    = (1 << 1), /**< Opus codec */
}
VatAudioCodec;


VATLIB_API VatLocalInputCodec Vat_CreateLocalInputCodec(VatAudioService audioService, VatAudioCodec codec);

VATLIB_API VatLocalOutputCodec Vat_CreateLocalOutputCodec(VatAudioService audioService, VatAudioCodec codec);

/**
    Vat_DestroyLocalCodec
 *
    This destroys the codec object - call this when you are done.
 *
 */
VATLIB_API void Vat_DestroyLocalInputCodec(VatLocalInputCodec inputCodec);

VATLIB_API void Vat_DestroyLocalOutputCodec(VatLocalOutputCodec localCodec);


/**
    Vat_GetInputDeviceInfo
 *
    This routine iterates all hardware devices. Your callbacks will be called
    once for each hardware device. Hardware is numbered starting from 0. It
    also returns the current inputs if the pointers are not NULL.
 *
 */
VATLIB_API void Vat_GetInputDeviceInfo(
    VatLocalInputCodec localCodec,
    VatLocalCodecHardwareIterator_f inputIterator, // Can be NULL
    void *inputRef,
    int *currentInput);      // Can be NULL

VATLIB_API void Vat_GetOutputDeviceInfo(
    VatLocalOutputCodec localCodec,
    VatLocalCodecHardwareIterator_f outputIterator, // Can be NULL
    void *outputRef,
    int *currentOutput);      // Can be NULL

/**
    Vat_SetAudioInputDevice
 *
    This routine changes the input device - pass a zero-based index.
 *
 */
VATLIB_API int Vat_SetAudioInputDevice(
    VatLocalInputCodec localCodec,
    int inputIndex);

/**
    Vat_SetAudioOutputDevice
 *
    This routine changes the output device - pass a zero-based index.
 *
 */
VATLIB_API void Vat_SetAudioOutputDevice(
    VatLocalOutputCodec localCodec,
    int outputIndex);

/**
    Vat_SetOutputVolume
 *
    This routine changes the total volume of the given local codec.
    Volume is accepted as percent from 0 (muted) - 300 ( 3x gain ).
 *
 */
VATLIB_API void Vat_SetOutputVolume(
    VatLocalOutputCodec localCodec,
    int volume);


/**
    Vat_GetOutputVolume
 *
    This routine returns total volume of the given local codec.
 *
 */
VATLIB_API int Vat_GetOutputVolume(
    VatLocalOutputCodec localCodec);

/**
    Vat_IsRecordingAlive

    Returns true if the input hardware device is functioning, false if it is not,
    for example due to the hardware not being able to support a given bitrate.
    Use this to inform the user if a hardware selection fails.  (For example,
    picking the internal modem as an input device usually fails. :-)

 */
VATLIB_API bool Vat_IsRecordingAlive(
    VatLocalInputCodec localCodec);

/** Returns the current input RMS level in dB */
VATLIB_API double Vat_GetInputRmsLevel(
    VatLocalInputCodec localCodec);

/** Returns the current input Peak level in dB */
VATLIB_API double Vat_GetInputPeakLevel(
    VatLocalInputCodec localCodec);

/** Returns the current output RMS level in dB. Range: -180 to 0 dB */
VATLIB_API double Vat_GetOutputRmsLevel(
    VatLocalOutputCodec localCodec);

/** Returns the current output Peak level in dB. Range: -180 to 0 dB */
VATLIB_API double Vat_GetOutputPeakLevel(
    VatLocalOutputCodec localCodec);

/**
    Vat_IsPlaybackAlive
 *
    Returns whether the hardware device is functioning for output - same idea
    as above.
 *
 */
VATLIB_API bool Vat_IsPlaybackAlive(VatLocalOutputCodec localCodec);


/**
    Vat_EnableFilter

    The local codec comes with an optional band-pass filter that makes
    transmissions sound more like they might in an airplane.  Pass 0
    or 1 to enable or disable this - defaults to off.

 */
VATLIB_API void Vat_EnableFilter(VatLocalOutputCodec localCodec, int status);

/**
    Vat_PlaySideTone

    This is used to indicate if the PTT button is pressed, for the purpsoe of
    generating "side tone". If your application supports side-tone, call this
    when the PTT key is toggled.

    \todo this function really controls side tone, NOT audio out of the
    hardware device. The audio codec always sends audio - use the mixer to
    disable transmission to radios, for example.
 *
 */
VATLIB_API void Vat_PlaySideTone(
    VatLocalOutputCodec localCodec);

/**
    Vat_SetSideToneVolume
 *
    This routine sets the level of side tone. By default it is muted.
    Volume is a percent integer from 0 to 100. 0 means muted and 100
    maximum volume.
 *
 */
VATLIB_API void Vat_SetSideToneVolume(
    VatLocalOutputCodec localCodec,
    int volume);

VATLIB_API void Vat_SetLocalCodecAudioHandler(
    VatLocalOutputCodec localCodec,
    VatLocalCodecAudioHandler_f handler,
    void *ref);

/**

    \class VatVoiceChannel

    VatVoiceChannel represents a connection to a voice room.  All audio from any
    member of the room comes out output port 0. Any audio sent into input port
    0 is sent to all members of the room. A VVLVoiceChannel must be connected and
    can be connected to many different rooms, but only one room at a time.

    You can attach a handler to the room to find out the status of the room in a
    number of ways.

    Because voice channels send audio over UDP, you need a single VatUDPAudioPort
    to make one or more VatVoiceChannels.
*/

/**
    \brief create voice channel
 *
    This creates a new voice room object. When the room is created,
    it starts in a disconnected state.
 *
    \param ip - the IP address of the server.
    \param port - the port of the server, usually 3782.
    \param channel - the string name of the voice room to join.
    \param callsign - your callsign as it will appear in the room.  By convention
       this is your VATSIM callsign followed by your CID.
    \param audioPort - you UDP audio port object.
 */
VATLIB_API VatVoiceChannel Vat_CreateVoiceChannel(
    VatAudioService audioService,
    const char *ip,
    int port,
    const char *channel,
    const char *callsign,
    VatUDPAudioPort udpAudioPort);

/**
    \brief Destroy voice Channel
 *
    This will destroy an existing voice channel.
 *
    \param channel Existing voice channel
 */
VATLIB_API void Vat_DestroyVoiceChannel(
    VatVoiceChannel channel);

/**
    \brief Returns room info
 *
    Returns useful room info, including the room name, your callsign, your
    assigned ID, and a vector of all clients in the room (including yourself!)
 *
    \param channel Channel object
    \param iterator Information callback
    \param roomName String containing the room name after routine has returned
    \param roomID Integer containing your room id after routine has returned
    \param ref User defined data for RoomIterator callback
 */
VATLIB_API void Vat_GetRoomInfo(VatVoiceChannel channel,
                                VatVoiceChannelRoomIterator_f iterator,
                                char *roomName,
                                int16_t *roomID,
                                char *callsign,
                                void *ref);

/**
    Vat_SetRoomInfo
 *
    Use this function to change the room you want to join. Only use when
    you are NOT connected!  You can use this right before connecting to pick
    a new room.
 *
 */
VATLIB_API void Vat_SetRoomInfo(
    VatVoiceChannel channel,
    const char *ip,
    int port,
    const char *channelName,
    const char *callsign);

/**
    Vat_JoinRoom

    Begin connecting to the room.  you will receive status messages for either a
    success or failure to join.  (Failure will be indicated by a disconnected message.)
 */
VATLIB_API void Vat_JoinRoom(VatVoiceChannel channel);

/**
    Vat_DisconnectFromRoom

    Disconnect from the room.   Disconnect message will be sent later when the disconnect
    completes.
 */
VATLIB_API void Vat_DisconnectFromRoom(
    VatVoiceChannel channel);

VATLIB_API void Vat_SetConnectionChangedHandler(
    VatVoiceChannel channel,
    VatConnectionChangedHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetClientJoinedHandler(
    VatVoiceChannel channel,
    VatClientJoinedHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetClientLeftHandler(
    VatVoiceChannel channel,
    VatClientJoinedHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetVoiceTransmissionChangedHandler(
    VatVoiceChannel channel,
    VatVoiceTransmissionChangedHandler_f handler,
    void *ref);

/**

    \class VatAudioMixer

    The audio mixer is an arbitrary audio router. Its two main capabilities
    are merging incoming audio from multiple producers into one consumer and
    splitting/replicating audio from one producer to multiple consumers.

    (Because audio devices can only be connected one input to one output,
    the mixer is the only way to split or merge audio.)

    The mixer can be thuoght of as an NxM matrix, with each square
    representing whether audio will make it from the input to the output.

    For example:
    <pre>

                    Output Port
                    0   1   2   3
                0       X
    Input Port  1   X   X
                2               X

    </pre>
    (Here we have a mixer connections from 1->0, 1->1, 0->1, and 2->3.

    In this example, input on port 0 will be sent to output port 1.
    Ipnput on port 1 will be sent to output ports 0 and 1, and
    input on port 2 will be sent to output port 3.  Note that output
    port 1 is fed by two input ports and inpnut port 1 feeds two
    output ports.

    An example of why you might do this is to share one codec with
    multiple voice rooms. For example:
    <pre>

                                Output Port
                    Speaker     V1  V2  V3
                Mic             X       X
    Input Port  V1      X
                V2      X
                V3
    </pre>

    In this example the Mic is routed to voice rooms 1 and 3 but not
    2; it's as if we're listening only in voice room 2.  Further more
    Voice rooms 1 and 2 but not 3 are routed to the speaker, so we'll
    hear voice rooms 1 and 2 but 3 is 'muted'.

    You can also do whacky thngs like this:

    <pre>
                                Output Port
                    Speaker     V1  V2  V3
                Mic             X       X
    Input Port  V1      X
                V2      X               X
                V3
    </pre>

    In this example voice room 2 is routed to voice  room 3.  So
    everything said in voice room 2 will be echoed to voice room 3
    but not vice versa!  Note that connections are either input or
    output, so any full-duplex source like a voice room must be routed
    in two places, its input and its output.

    As a final strange example of what you can do with the mixer,
    you can in fact route the mixer into itself!  Why would you ever
    want to do this?!?  The answer is to provide an easy way to mute
    whole sets of audio.  For example, consider this very complicated
    topology (just the outputs are shown for convenience):

            Speaker V1  V2  V3  V4  L1  L2  L3
    Mic             X   X   X   X

    In this case we have four voice rooms and 3 land line clals.  To
    mute every voice call we have to hit he mixer in four cases.  But
    you can also do this:
                    Speaker Voice Send V1 V2 V3 V4 Land Send L1 L2 L3
    Mic                         1                       2
    Voice Return                5       3 3  3  3
    Land Return                                              4  4  4

    I have numbered the mixer points for clarity:
    1. This connects the Mic to the 'voice send' bus.  Deactivating this
    point mutes the Mic for all voice rooms at once.
    2. This connects the Mic to the 'land line send' bus.  Deactivating
    this point mutes the Mic for all land lines.
    3. These points conect the voice room feed to individual voice rooms.
    Any one can be deactivated to mute a specific voice room.
    4. These points connect the land line feed to individual land lines.
    Any one of these points can be deactivated to kill an individual
    land line.
    5. This point would form a feedback loop sending the voice return
    back into the voice send.  This is a VERY BAD IDEA!  Do not EVER
    route the mixer into itself.

    The point of the above example is that since you can have about 4
    billion inputs and outputs on the mixer, it can be useful to group
    related sets.  In this case to implement the functionality of muting
    all voice rooms when a land line call is in effect, the land line
    code must simply tap point 1 on the board, not all of points 3.
*/

/**
 *  Vat_CreateAudioMixer
 *
 *  This routine creates a new audio mixer.  Most apps only require
 *  one audio mixer.
 */
VATLIB_API VatAudioMixer Vat_CreateAudioMixer();

/**
 *  Vat_DestroyAudioMixer
 *
 *  Destroys the audio mixer object.
 */
VATLIB_API void Vat_DestroyAudioMixer(
    VatAudioMixer mixer);

/**
    Vat_AudioMixerMakeMixerConnection

    This routine creates or removes a mixer connection.  Audio
    received on inPort will or will not be sent to outPort
    based on 'connect'.  There is no limit to how many inputs
    one output can take a feed from or how many outputs one input
    can feed.

 */
VATLIB_API void Vat_MakeMixerConnection(VatAudioMixer mixer,
                                        int port,
                                        int outPort,
                                        bool connect);

/**
    Vat_HasMixerConnection

    This routine returns whether a mixer connection is on
    or not.  Use this to figure out what is muted.

 */
VATLIB_API bool Vat_HasMixerConnection(
    VatAudioMixer mixer,
    int port,
    int outPort);

/**
    Vat_GetLastInputTime, Vat_GetLastOutputTime

    Monitoring. These routines tell whether an input port has
    sent data or an output port has received data. The result
    is a time in seconds since last send. -1 indicates no data
    has ever been sent or received.

 */
VATLIB_API float Vat_GetLastInputTime(
    VatAudioMixer mixer,
    int port);

VATLIB_API float Vat_GetLastOutputTime(
    VatAudioMixer mixer,
    int outPort);

#ifdef __cplusplus
}
#endif

#endif

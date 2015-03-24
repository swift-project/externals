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

/* Versions 0.1.0 through 1.0.0 are in the range 0 to 100
 * From version 1.0.1 on it's xxyyzz, where x=major, y=minor, z=release
 * Reason:
 * Leading zeros changes the number to octal.
 */
#define VAT_LIBVATLIB_VERION 903 /* 0.9.3 */

/** Retrieve the release number of the currently running Vatlib build,
 eg 903.
*/
VATLIB_API int Vat_GetVersion();

/** Retrieve a textual description of the current Vatlib build,
 eg "Vatlib V0.9.1 (built (built Oct 20 2014 11:10:00).
*/
VATLIB_API const char *Vat_GetVersionText();

typedef enum
{
    SeverityNone,
    SeverityError,
    SeverityWarning,
    SeverityInfo,
    SeverityDebug
}
SeverityLevel;

/**
 * \brief Log handler callback.
 * \param message Log message.
 *
 * A function of this type is called by vatlib when a message is logged.
 */
typedef void (* VatlibLogHandler_t)(
    SeverityLevel level,
    const char *message);

/***************************************************************************
    Network Section
 ***************************************************************************/

/**
 * \brief VatSessionID
 *
 * This is an opaque reference to a session.  A session is a single user's connection
 * to the protocol.  A session may be connected multiple times, but its login information
 * can only be specified when disconnected.  Each session has callbacks for incoming data
 * associated with it; these are good for the life of a session but must be set up
 * separately for each session.  For this reason, you probably want to keep your session
 * aroud a long time.
 */
#ifdef __cplusplus
typedef class FSDClient PCSBClient;
typedef PCSBClient *VatSessionID;
#else
typedef void *VatSessionID;
#endif

VATLIB_API void Vat_sendFsdMessage(
    VatSessionID session,
    const char *message);

/** Server type */
typedef enum
{
    vatServerLegacyFsd,   /**< Legacy FSD. */
    vatServerVatsim       /**< VATSIM server. */
}
VatServerType;

/** Connection status */
typedef enum
{
    /** Not yet connected */
    vatStatusIdle,
    /** Connecting to server */
    vatStatusConnecting,
    /**<  Connection successful (log-in may be in process). */
    vatStatusConnected,
    /**<  Disconnecting from server. */
    vatStatusDisconnecting,
    /**<  Disconnected from server. */
    vatStatusDisconnected,
    /**<  Some kind of error. */
    vatStatusError
}
VatConnectionStatus;

/** Client capability flags */
typedef enum
{
    /** None */
    vatCapsNone             = (1 << 0),
    /** Can accept ATIS responses */
    vatCapsAtcInfo          = (1 << 1),
    /** Can send/receive secondary visibility center points (ATC/Server only) */
    vatCapsSecondaryPos     = (1 << 2),
    /** Can send/receive modern model packets */
    vatCapsModelDesc        = (1 << 3),
    /** Can send/receive inter-facility coordination packets (ATC only) */
    vatCapsOngoingCoord     = (1 << 4),
    /** Can send/receive high-speed position updates (pilot only) */
    vatCapsInterminPos      = (1 << 5),
    /** Stealth mode */
    vatCapsStealth          = (1 << 6),
    /** Aircraft Config */
    vatCapsAircraftConfig   = (1 << 7)
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


/** Server error codes */
typedef enum
{
    /** No error */
    vatServerErrorNone,
    /** Callsign in use */
    vatServerErrorCsInUs,
    /** Callsign invalid */
    vatServerErrorCallsignInvalid,
    /** Already registered */
    vatServerErrorRegistered,
    /** Syntax error */
    vatServerErrorSyntax,
    /** Invalid source in packet */
    vatServerErrorSrcInvalid,
    /** Invalid CID/password */
    vatServerErrorCidInvalid,
    /** No such callsign */
    vatServerErrorNoSuchCs,
    /** No flightplan */
    vatServerErrorNoFP,
    /** No such weather profile */
    vatServerErrorNoWeather,
    /** Invalid protocol revision */
    vatServerErrorRevision,
    /** Requested level too high */
    vatServerErrorLevel,
    /** No more clients */
    vatServerErrorServFull,
    /** CID/PID was suspended */
    vatServerErrorCsSuspended,
    /** Not valid control */
    vatServerErrorInvalidCtrl,
    /** Invalid position for rating */
    vatServerErrorInvPos,
    /** Not authorized software */
    vatServerErrorUnAuth,
    /** Wrong server type */
    vatServerWrongType,
    /** Unknown error */
    vatErrorUnknown
}
VatServerError;

/** Client type */
typedef enum
{
    /** Unknown type */
    vatUnknownClient = 0,
    /** Pilot client type */
    vatPilot = 1,
    /** ATC client type */
    vatAtc
}
VatClientType;

/** Transponder modes */
typedef enum
{
    /** Transponder is off, or in standby. */
    vatTransponderModeStandby,
    /** Transponder is on mode C, not identing. */
    vatTransponderModeCharlie,
    /** Transponder is on mode C and identing. */
    vatTransponderModeIdent
}
VatTransponderMode;

/** ATC ratings */
typedef enum
{
    /** Unknown */
    vatAtcRatingUnknown = 0,
    /** OBS */
    vatAtcRatingObserver,
    /** S1 */
    vatAtcRatingStudent,
    /** S2 */
    vatAtcRatingStudent2,
    /** S3 */
    vatAtcRatingStudent3,
    /** C1 */
    vatAtcRatingController1,
    /** C2 */
    vatAtcRatingController2,
    /** C3 */
    vatAtcRatingController3,
    /** I1 */
    vatAtcRatingInstructor1,
    /** I2 */
    vatAtcRatingInstructor2,
    /** I3 */
    vatAtcRatingInstructor3,
    /** SUP */
    vatAtcRatingSupervisor,
    /** ADM */
    vatAtcRatingAdministrator
}
VatAtcRating;

/** Pilot ratings */
typedef enum
{
    /** Unknown rating */
    vatPilotRatingUnknown = 0,
    /** P1 */
    vatPilotRatingStudent,
    /** P2 */
    vatPilotRatingVFR,
    /** P3 */
    vatPilotRatingIFR,
    /** Instructor */
    vatPilotRatingInstructor,
    /** SUP */
    vatPilotRatingSupervisor
}
VatPilotRating;

/** ATC facility type */
typedef enum
{
    /** Unknown facility type */
    vatFacilityTypeUnknown = 0,
    /** FSS */
    vatFacilityTypeFSS,
    /** Delivery */
    vatFacilityTypeDEL,
    /** Ground */
    vatFacilityTypeGND,
    /** Tower */
    vatFacilityTypeTWR,
    /** Approach */
    vatFacilityTypeAPP,
    /** Center */
    vatFacilityTypeCTR
}
VatFacilityType;

/** Flight simulator type */
typedef enum
{
    /** Unknown simulator type */
    vatSimTypeUnknown = 0,
    /** MS Flight Simulator 95 */
    vatSimTypeMSFS95 = 1,
    /** MS Flight Simulator 98 */
    vatSimTypeMSFS98 = 2,
    /** MSCFS (?) */
    vatSimTypeMSCFS = 3,
    /** X-Plane */
    vatSimTypeXPLANE = 11,
    /** AS2 */
    vatSimTypeAS2 = 14,
    /** PS1 */
    vatSimTypePS1 = 15
}
VatSimType;

/** Flight rules */
typedef enum
{
    /**< IFR flight rules. */
    vatFlightTypeIFR,
    /**< Visual flight rules. */
    vatFlightTypeVFR,
    /**< Special visual flight rules */
    vatFlightTypeSVFR,
    /**< Defense visual Flight Rules */
    vatFlightTypeDVFR
} VatFlightType;

/** Client query types */
typedef enum
{
    /** Flight plan (pilots only, reply is a flight plan mesasge */
    vatInfoQueryTypeFP = 0,
    /**< Frequency (pilots only) */
    vatInfoQueryTypeFreq = 1,
    /**< User Info (must be supervisor) */
    vatInfoQueryTypeInfo = 2,
    /**< ATIS (controllers only, reply is a text message */
    vatInfoQueryTypeAtis = 3,
    /**< What server is this client on */
    vatInfoQueryTypeServer = 4,
    /**< Real name */
    vatInfoQueryTypeName = 5,
    /**< Is this client working ATC or just an observer (ATC only) */
    vatInfoQueryTypeAtc = 6,
    /**< What capabilities does this client have */
    vatInfoQueryTypeCaps = 7,
    /**< What IP address am I sending from? */
    vatInfoQueryTypeIP = 8,
}
VatInfoQueryType;

/** Aircraft engine type */
typedef enum
{
    /**< Piston */
    vatEngineTypePiston = 0,
    /**< Jet */
    vatEngineTypeJet    = 1,
    /**< None */
    vatEngineTypeNone   = 2,
    /**< Helo */
    vatEngineTypeHelo   = 3
} VatEngineType;

/** Operations for a land line */
typedef enum
{
    /**< Request landline with other client. */
    vatLandlineCmdRequest = 0,
    /**< Approve a request for a landline connection. */
    vatLandlineCmdApprove,
    /**< Reject a request for a landline. */
    vatLandlineCmdReject,
    /**< Terminate an in-progress landline. */
    vatLandlineCmdEnd
} VatLandlineCmd;

/**  Types of landlines */
typedef enum
{
    /**< Intercom, a basic two-way telephone call. */
    vatLandlineTypeIntercom = 0,
    /**< Override.  Receiver doesn't have to key mic. */
    vatLandlineTypeOverride,
    /**< Monitor - one way send back to the initiator. */
    vatLandlineTypeMonitor
} VatLandlineType;

/**  Tracking commands */
typedef enum
{
    /**< Start tracking */
    vatTrackingCmdStartTrack = 0,
    /**< Drop tracking */
    vatTrackingCmdDropTrack,
    /**< I'm tracking */
    vatTrackingCmdIHave,
    /**< Who is tracking */
    vatTrackingCmdWhoHas,
    /**< ??? */
    vatTrackingCmdPointout,
    /**< Departure list */
    vatTrackingCmdDepartureList
} VatTrackingCmd;

/** Pilot position structure */
typedef struct
{
    /** Latitude in decimal degrees */
    double latitude;
    /**< Longitude in decimal degrees */
    double longitude;
    /**< True altitude in feet above MSL */
    int altitudeTrue;
    /**< Pressure altitude in feet above MSL */
    int altitudePressure;
    /**< Ground speed in knots */
    int groundSpeed;
    /**< Heading in degrees, clockwise from true north, 0-359 */
    double heading;
    /**< Bank in degrees, positive = roll right */
    double bank;
    /**< Pitch in degrees, positive = pitch up */
    double pitch;
    /**< 0-7777 */
    int transponderCode;
    /**< standby, charlie or ident */
    VatTransponderMode transponderMode;
    /**< rating */
    VatPilotRating rating;
}
VatPilotPosition;


/** ATC position structure */
typedef struct
{
    /**< ATC frequency in khz. */
    int frequency;
    /**< Facility type */
    VatFacilityType facility;
    /**< Visible range in nm */
    int visibleRange;
    /**< Rating */
    VatAtcRating rating;
    /**< Latitude in decimal degrees. */
    double latitude;
    /**< Longitude in decimal degrees. */
    double longitude;
    /**< Elevation AGL in feet (not used.) */
    int elevation;
} VatAtcPosition;


/** Pilot Connection information */
typedef struct
{
    /** Pilots callsign */
    const char *callsign;
    /** Pilots real name */
    const char *name;
    /** Simulator type */
    VatSimType simType;
    /** Pilot rating */
    VatPilotRating rating;
}
VatPilotConnection;

/** ATC Connection information  */
typedef struct
{
    /** Controllers callsign */
    const char *callsign;
    /** Controllers real name */
    const char *name;
    /** Controller rating */
    VatAtcRating rating;
}
VatAtcConnection;

/** A single temperature layer */
typedef struct
{
    /** Top of temperature layer in feet MSL */
    int ceiling;
    /** Temperature in degrees celsius */
    int temp;
} VatTempLayer;

/** A single wind layer */
typedef struct
{
    /** in ft MSL */
    int ceiling;
    /** in ft MSL */
    int floor;
    /** In degrees clockwise from true north */
    int direction;
    /** Wind speed in knots. */
    int speed;
    /** 1 = true, 0 = false */
    int gusting;
    /** 0 - 255, 0 = none, 255 = extreme */
    int turbulence;
}
VatWindLayer;

/** A single cloud layer. */
typedef struct
{
    /** Ft. MSL */
    int ceiling;
    /** Ft. MSL */
    int floor;
    /** 0 - 8 octets, 0 = clear, 8 = overcast. */
    int coverage;
    /** 1 = true, 0 = false */
    int icing;
    /** 0 - 255 */
    int turbulence;
}
VatCloudLayer;

/** A thunderstorm layer. NOTE: unclear why this is different from a regular cloud layer! */
typedef struct
{
    /** In ft. */
    int ceiling;
    /** In ft. */
    int floor;
    /** 0 - 8 octets */
    int coverage;
    /** ?? */
    int deviation;
    /** 0 - 255 */
    int turbulence;
}
VatThunderStormLayer;


/**
 * \brief A flight plan structure
 * \note Pass empty strings for empty input, never null.
 */
typedef struct
{
    VatFlightType flightType;
    /** Aircraft ICAO type e.g. T/B734/F */
    const char *aircraftType;
    /** Cruising speed in knkots */
    int trueCruisingSpeed;
    /** ICAO code of departure airport */
    const char *departAirport;
    /** Departure time in UTC, e.g. 2030 = 8:30 pm GMT */
    int departTime;
    /** Actual departure time in UTC, e.g. 2030 = 8:30 pm GMT */
    int departTimeActual;
    /** Planned cruising altitude. FL230 or 15000 */
    const char *cruiseAltitude;
    /** ICAO code of destination airport */
    const char *destAirport;
    /** Enroute time - hours */
    int enrouteHrs;
    /** Enroute time - minutes */
    int enrouteMins;
    /** Available fuel - hours */
    int fuelHrs;
    /** Available fuel - minutes */
    int fuelMins;
    /** ICAO code of alternate airport */
    const char *alternateAirport;
    /** Flight plan remarks */
    const char *remarks;
    /** Flight route in capital letters. Separate with dots. */
    const char *route;
}
VatFlightPlan;

/** Controller Atis structure */
typedef struct
{
    /** Controller voice room in the form <url>/room */
    const char *voiceRoom;
    /** Controller messages lines. Maximum 4 */
    const char **textLines;
    /** Number of message lines */
    int textLineCount;
    /** Controllers planned logoff time, e.g 20:00 UTC */
    const char *zuluLogoff;
} VatControllerAtis;

/** Aircraft information structure */
typedef struct
{
    /** Aircraft ICAO identifier */
    const char *aircraftType;
    /** Airline callsign */
    const char *airline;
    /** Livery identifier */
    const char *livery;
} VatAircraftInfo;

/**
 *  \defgroup callback_functions Callback function typedefs
 *  \brief Typedefs defined in vatlib.h for callback or handler functions passed as function parameters.
 *
 *  Vatlib uses installed callback functions as parameters for some function calls,
 *  e.g. to set up notification handlers.
 *
 *  The typedefs defined in this group describe the function parameters used to set
 *  up or clear the callback functions and should also be referenced to define the
 *  callback function to handle such events in the user's code.
 *  \{
 */

/**
 * \brief Connection change callback
 * \param session Session handler
 * \param oldStatus Old connection status
 * \param newStatus New connection status
 * \param ref User defined data
 */
typedef void (* VatStateChangeHandler_f)(
    VatSessionID session,
    VatConnectionStatus oldStatus,
    VatConnectionStatus newStatus,
    void *ref);

/**
 * \brief Private message callback
 * \param session Session handler
 * \param from Sender callsign
 * \param to Receiver callsign
 * \param ref User defined data
 */
typedef void (* VatTextMessageHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *receiver,
    const char *message,
    void *ref);

/** Function signature when a radio transmission has been received. An array of frequencies in khz
    is passed in. */
typedef void (* VatRadioMessageHandler_f)(
    VatSessionID session,
    const char *sender,
    int freqCount,
    int *freqList,
    const char *message,
    void *ref);

/** Function signature when a pilot has left the network. */
typedef void (* VatDeletePilotHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
    Function signature when a controller has left the network.
 */
typedef void (* VatDeleteAtcHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
    Function signature when a pilot's position has been reported.
 */
typedef void (* VatPilotPositionHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatPilotPosition *position,
    void *ref);

/**
    Function signature when a controller's position has been reported.
*/
typedef void (* VatAtcPositionHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatAtcPosition *position,
    void *ref);

/**
    Function signature when you are being killed from the network.
*/
typedef void (* VatKillHandler_f)(
    VatSessionID session,
    const char *reason,
    void *ref);

/**
    Function signature when your ping has been answered with a pong.
    \param elapsedTime Elapsed time, after ping had been sent in milliseconds.
*/
typedef void (* VatPongHandler_f)(
    VatSessionID session,
    const char *sender,
    double elapsedTime,
    void *ref);

/**
    Function signature for incoming flight plan.
*/
typedef void (* VatFlightPlanHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatFlightPlan *flightPlan,
    void *ref);

/**
    Function signature for a handoff request from another ATC to you.
*/
typedef void (* VatHandoffRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
    Function signature when another ATC has accepted a handoff.
*/
typedef void (* VatHandoffAcceptedHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
    Function signature when a the other ATC is canceling or refusing the hanodff.
*/
typedef void (* VatHandoffCancelHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    void *ref);

/**
    Function signature for an incoming metar in text format.
    OPEN ISSUE: shouldn't this be labeled as ACARS METAR?
*/
typedef void (* VatACARSDataHandler_f)(
    VatSessionID session,
    const char *data,
    void *ref);

/**
    Function signature when someone is requesting ACARS data of you.
    OPEN ISSUE: isn't this server only?!?
*/
typedef void (* VatACARSRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *data,
    void *ref);

typedef void (* VatMetarRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *station,
    void *ref);

typedef void (* VatMetarResponseHandler_f)(
    VatSessionID session,
    const char *metar,
    void *ref);

/** Function signature when someone is requesting info from you. This will only be:
    real name, frequency, ATIS. Info and server are provided automatically by the
    library.
 */
typedef void (* VatInfoRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    VatInfoQueryType type,
    const char *data,
    void *ref);

/** Function signature when an info reponse is received.
    For most info responses, only data is passed back, but
    for an ATC query about another ATC, data2 is also used.
 */
typedef void (* VatInfoResponseHandler_f)(
    VatSessionID session,
    const char *sender,
    VatInfoQueryType type,
    const char *data,
    const char *data2,
    void *ref);

/** Function signature when a CAPS reply has been received.
    It will contain a reply for each Capability it knows
    about as well as whether or not it supports it. This will come
    all in one packet.
    \see VatCapabilities
 */
typedef void (* VatInfoCAPSReplyHandler_f)(
    VatSessionID session,
    const char *sender,
    int capabilityFlags,
    void *ref);

/** A controller voiceroom has been received. */
typedef void (* VatVoiceRoomHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *voiceRoom,
    void *ref);

/** A controller ATIS has been received. */
typedef void (* VatControllerAtisHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatControllerAtis *atis,
    void *ref);

/** Function signature for network errors
 */
typedef void (* VatServerErrorHandler_f)(
    VatSessionID session,
    VatServerError errorCode,
    const char *parameter,
    const char *description,
    void *ref);

/** Function signature for an incoming temperature and pressure data.
    The barometric pressure is in the form 2992 (e.g. hundredths of inches.)
 */
typedef void (* VatTemperatureDataHandler_f)(
    VatSessionID session,
    const VatTempLayer layer[4],
    int pressure,
    void *ref);

/** Function signature for incoming wind data.
 */
typedef void (* VatWindDataHandler_f)(
    VatSessionID session,
    const VatWindLayer layer[4],
    void *ref);

/** Function signature for incoming cloud and thunderstorm data.
    Visibility is in statute miles.
 */
typedef void (* VatCloudDataHandler_f)(
    VatSessionID session,
    const VatCloudLayer cloudLayer[2],
    VatThunderStormLayer thunderStormLayer,
    float visibility,
    void *ref);

/**
 * Function signature for incoming aircraft config packets.
 */
typedef void (* VatAircraftConfigHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *aircraftConfig,
    void *ref);

/**
    Function signature for some kind of incoming misc.
    ATC message ("ProController" packet.)
    OPEN ISSUE: should we have generic handlers for unknown variants?
 */
typedef void (* VatATCClientComHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *data,
    void *ref);

/**
    Function signature for some kind of incoming misc.
    pilot message ("SquawkBox" packet.)
 */
typedef void (* VatPilotClientComHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *subType,
    void *ref);

/**
    Function signature for some kind of incoming misc.
    pilot message ("SquawkBox" packet.)
 */
typedef void (* VatCustomPilotPacketHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *subType,
    const char **data,
    int dataSize,
    void *ref);

/**
    Function signature when another client is requesting info
    about our info as a pilot (e.g. what kind of plane are we?
 */
typedef void (* VatAircraftInfoRequestHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
    Function signature when a pilot is telling us what kind of plane they are.
 */
typedef void (* VatAircraftInfoHandler_f)(
    VatSessionID session,
    const char *sender,
    const VatAircraftInfo *aircraftInfo,
    void *ref);

/**
    Function signature when a pilot is telling us what kind of plane they are.
 */
typedef void (* VatLegacyAircraftInfoHandler_f)(
    VatSessionID session,
    const char *sender,
    VatEngineType engineType,
    const char *cslModel,
    void *ref);

/**
    Function signature when someone is attempting to write an FSUIPC command to us.
    (Most sims can't support this.)
 */
typedef void (* VatHandleIPCHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *ipcCommand,
    int ipcSlot,
    int ipcValue,
    void *ref);

typedef void (* VatTargetsScratchpadHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    const char *scratchpad,
    void *ref);

/**
    Function signature when some kind of land line command is being sent
    from another client.
 */
typedef void (* VatHandleLandlineCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    VatLandlineCmd command,
    VatLandlineType type,
    const char *ip,
    int port,
    void *ref);

/**
    Function signature for a tracking command from another ATC.
 */
typedef void (* VatHandleTrackingCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *target,
    VatTrackingCmd command,
    void *ref);

/**
    Function signature when a controller wants a break.
 */
typedef void (* VatControllerBreakCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    bool wantsBreak,
    void *ref);

/**
    Function signature when a nex-gen client is ID'ing us
 */
typedef void (* VatHandleSharedStateIDHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);

/**
    Function signature when a nex-gen client is replying to our ID
 */
typedef void (* VatHandleSharedStateDIHandler_f)(
    VatSessionID session,
    const char *sender,
    void *ref);
/**
    Function signature when a flight strip is being pushed to us.
    New way which supports strip type and annotations - added by RAC 02/28/2007.
 */
typedef void (* VatHandlePushStripCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    const char *aCCallsign,
    int type,
    const char **annotations,
    void *ref);

/**
    Function signature for a start/end request for help
 */
typedef void (* VatHandleHelpCommandHandler_f)(
    VatSessionID session,
    const char *sender,
    bool wantsHelp,
    const char *message,
    void *ref);

/** \} */ /* group callback_functions */


/***************************************************************************
    CALLBACK INSTALLERS
 ***************************************************************************
    These functions allow you to associate a callback with a session.  Install
    a NULL handler to ignore a callback.  You pass in a void    "reference ptr"
    that is returned to you in your callback (in the ref field).  This lets
    you find local variables attached to this particular instance of a session.
 *
 */

VATLIB_API void Vat_SetNetworkLogHandler(SeverityLevel severityLevel, VatlibLogHandler_t handler);
VATLIB_API void Vat_SetNetworkLogSeverityLevel(SeverityLevel severityLevel);

/**
    \brief ReceiveStateChange callback installer

    Installs a notification callback when the network connection stautus
    has changed.
    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetStateChangeHandler(
    VatSessionID session,
    VatStateChangeHandler_f handler,
    void *ref);

/**
    \brief Message callback installer

    Installs a notification callback when a text message has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetTextMessageHandler(
    VatSessionID session,
    VatTextMessageHandler_f handler,
    void *ref);

/**
    \brief Radio callback installer

    Installs a notification callback when a radio text message has been d.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetRadioMessageHandler(
    VatSessionID session,
    VatRadioMessageHandler_f handler,
    void *ref);

/**
    \brief PilotPosition callback installer

    Installs a notification callback when a pilot position has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetPilotPositionHandler(
    VatSessionID session,
    VatPilotPositionHandler_f handler,
    void *ref);

/**
    \brief InterimPilotPosition callback installer

    Installs a notification callback when a interim pilot position has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetInterimPilotPositionHandler(
    VatSessionID session,
    VatPilotPositionHandler_f handler,
    void *ref);

/**
    \brief ATCPosition callback installer

    Installs a notification callback when a ATC position has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetAtcPositionHandler(
    VatSessionID session,
    VatAtcPositionHandler_f handler,
    void *ref);
/**
    \brief DeletePilot callback installer

    Installs a callback notifying when a pilot has left the network.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetDeletePilotHandler(
    VatSessionID session,
    VatDeletePilotHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifying when a controller has left the network.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetDeleteAtcHandler(
    VatSessionID session,
    VatDeleteAtcHandler_f handler,
    void *ref);

/**
    \brief Kill callback installer

    Install a function callback to be notified when you have been kicked from the network.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetKillHandler(
    VatSessionID session,
    VatKillHandler_f handler,
    void *ref);

/**
    \brief Pong callback installer

    Install a function callback to be notified about a pong echo from a previous ping.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetPongHandler(
    VatSessionID session,
    VatPongHandler_f handler,
    void *ref);

/**
    \brief FlightPlan callback installer

    Installs a callback notifiying about an incoming flightplan.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetFlightPlanHandler(
    VatSessionID session,
    VatFlightPlanHandler_f handler,
    void *ref);

/**
    \brief HandoffRequest callback installer

    Installs a callback notifiying about a handoff request. A handoff
    request will be sent to another controller to take over tracking of
    an aircraft.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
    \see Vat_SetHandoffAcceptanceHandler
    \see Vat_SetHandoffCancelHandler
  */
VATLIB_API void Vat_SetHandoffRequestHandler(
    VatSessionID session,
    VatHandoffRequestHandler_f handler,
    void *ref);

/**
    \brief HandoffAccepted callback installer

    Installs a callback notifiying when another controller has
    accepted your previous handoff request and is now tracking the
    aircraft.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
    \see Vat_SetHandoffRequestHandler
  */
VATLIB_API void Vat_SetHandoffAcceptedHandler(
    VatSessionID session,
    VatHandoffAcceptedHandler_f handler,
    void *ref);

/**
    \brief HandoffCancel callback installer

    Installs a callback notifiying when another controller has
    canceled your previous handoff request and is not tracking the
    aircraft.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
    \see Vat_SetHandoffRequestHandler
  */
VATLIB_API void Vat_SetHandoffCancelHandler(
    VatSessionID session,
    VatHandoffCancelHandler_f handler,
    void *ref);

/**
    \brief ACARSData callback installer

    Installs a callback notifiying when ACARS data has been received.
    Typical ACARS data is METAR.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetACARSResponseHandler(
    VatSessionID session,
    VatACARSDataHandler_f handler,
    void *ref);

/**
    \brief ACARSRequest callback installer

    IInstalls a callback notifiying when ACARS data is requested from you.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback
    \param ref Pointer to user defined data, which will be passed to the callback
    \todo Clarify the exact usage and data
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetACARSRequestHandler(
    VatSessionID session,
    VatACARSRequestHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetMetarRequestHandler(
    VatSessionID session,
    VatMetarRequestHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetMetarResponseHandler(
    VatSessionID session,
    VatMetarResponseHandler_f handler,
    void *ref);

/**
    \brief InfoRequest callback installer

    Installs a callback notifiying when another client is requesting information
    from you. This request is for ATC clients only.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
    \see Vat_SetInfoReplyHandler
  */
VATLIB_API void Vat_SetInfoRequestHandler(
    VatSessionID session,
    VatInfoRequestHandler_f handler,
    void *ref);

/**
    \brief InfoResponse callback installer

    Installs a callback notifiying when a client has replied to your info
    request.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
    \see Vat_SetInfoRequestHandler
  */
VATLIB_API void Vat_SetInfoResponseHandler(
    VatSessionID session,
    VatInfoResponseHandler_f handler,
    void *ref);

/**
    \brief InfoCAPSReply callback installer

    Installs a callback notifiying when another client has replied to your
    CAPS info request.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \see Vat_SetInfoRequestHandler
    \todo CAPS should be a bit mask instead of strings
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.

  */
VATLIB_API void Vat_SetInfoCAPSReplyHandler(
    VatSessionID session,
    VatInfoCAPSReplyHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetVoiceRoomHandler(
    VatSessionID session,
    VatVoiceRoomHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetControllerAtisHandler(
    VatSessionID session,
    VatControllerAtisHandler_f handler,
    void *ref);

/**
    \brief TemperatureData callback installer

    Installs a callback notifiying when temperature data has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetTemperatureDataHandler(
    VatSessionID session,
    VatTemperatureDataHandler_f handler,
    void *ref);

/**
    \brief WindData callback installer

    Installs a callback notifiying when wind data has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetWindDataHandler(
    VatSessionID session,
    VatWindDataHandler_f handler,
    void *ref);

/**
    \brief CloudData callback installer

    Installs a callback notifiying when cloud data has been received.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetCloudDataHandler(
    VatSessionID session,
    VatCloudDataHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetAircraftConfigHandler(
    VatSessionID session,
    VatAircraftConfigHandler_f handler,
    void *ref);

/**
    \brief ATCClientCom callback installer

    Installs a callback notifiying about a custom ATC client packet. This will
    report only the subtype of the packet, not the data. Extensions to the protocol
    should always be added to vatlib itself and end up in its own callback.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetATCClientComHandler(
    VatSessionID session,
    VatATCClientComHandler_f handler,
    void *ref);

/**
    \brief PilotClientCom callback installer

    Installs a callback notifiying about a custom pilot client packet. This will
    report only the subtype of the packet, not the data. Extensions to the protocol
    should always be added to vatlib itself and end up in its own callback.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \see Vat_SetCustomPilotPacketHandler
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetPilotClientComHandler(
    VatSessionID session,
    VatPilotClientComHandler_f handler,
    void *ref);

/**
    \brief CustomPilotPacket callback installer

    Installs a callback notifiying about a custom pilot client packet. This callback
    will make subtype and data available. Use this, if you want to send custom
    packets which are not going to be incorporated into the VATSIM protocol.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetCustomPilotPacketHandler(
    VatSessionID session,
    VatCustomPilotPacketHandler_f handler,
    void *ref);

/**
    \brief PilotInfoRequest callback installer

    Installs a callback notifiying about an info request from another client.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \todo Pilot clients only
    \see Vat_SetPilotInfoHandler
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetAircraftInfoRequestHandler(
    VatSessionID session,
    VatAircraftInfoRequestHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when a pilot client has answered your info
    request.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \see Vat_SetPilotInfoRequestHandler
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetAircraftInfoHandler(
    VatSessionID session,
    VatAircraftInfoHandler_f handler,
    void *ref);

VATLIB_API void Vat_SetLegacyAircraftInfoHandler(
    VatSessionID session,
    VatLegacyAircraftInfoHandler_f handler,
    void *ref);

/**
    \brief HandleIPC callback installer

    Installs a callback notifiying when another client is trying to send an
    FSUIPC commant to us.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleIPCHandler(
    VatSessionID session,
    VatHandleIPCHandler_f handler,
    void *ref);

/**
    \brief HandleSharedState callback installer

    Installs a callback notifiying when another client is sending its shared
    state to us, because it probably changed.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetTargetScratchpadHandler(
    VatSessionID session,
    VatTargetsScratchpadHandler_f handler,
    void *ref);

/**
    \brief HandleLandlineCommand callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleLandlineCommandHandler(
    VatSessionID session,
    VatHandleLandlineCommandHandler_f handler,
    void *ref);
/**
    \brief StateChange callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleTrackingCommandHandler(
    VatSessionID session,
    VatHandleTrackingCommandHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer
    Installs a notification callback when the network connection stautus
    has changed.
    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleControllerBreakCommandHandler(
    VatSessionID session,
    VatControllerBreakCommandHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleSharedStateIDHandler(
    VatSessionID session,
    VatHandleSharedStateIDHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleSharedStateDIHandler(
    VatSessionID session,
    VatHandleSharedStateDIHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandlePushStripCommandHandler(
    VatSessionID session,
    VatHandlePushStripCommandHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when ...

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetHandleHelpCommandHandler(
    VatSessionID session,
    VatHandleHelpCommandHandler_f handler,
    void *ref);

/**
    \brief StateChange callback installer

    Installs a callback notifiying when an error occured.

    \param session The session ID to be used for this operation.
    \param handler Pass a function pointer to install or
       nullptr to deinstall the callback.
    \param ref Pointer to user defined data, which will be passed to the callback
    \warning Your callback must be thread safe, because it is not garuanteed that
       the callback is always called from the main thread.
  */
VATLIB_API void Vat_SetServerErrorHandler(
    VatSessionID session,
    VatServerErrorHandler_f handler,
    void *ref);

VATLIB_API int Vat_ExecuteNetworkTasks(
    VatSessionID session);

VATLIB_API void Vat_ExecuteNetworkTasksAsync(
    VatSessionID session);

VATLIB_API void Vat_StopAsyncNetworkExecution(
    VatSessionID session);

/**
    Vat_CreateNetworkSession creates a new networking session. Most applications will have only one session however
    someday this may change.

    \param clientVersionString  A string name for your application including version number. Eg. "XTower 1.0". This information
        will be sent to other clients who may query your version.
    \param clientMajorVersion   Integer major version number eg. the "1" in 1.3
    \param clientMinorVersion   Integer minor version number eg. the "3" in 1.3
    \param hostVersion          A string name for the host application. If you're a plugin for Flight Sim, you'd put the MSFS name here.
    \param publicClientID       Your public client ID as issued by the VATSIM administrator. Leave NULL for other networks.
    \param privateKey           Your private key as issued by the VATSIM administrator. Leave NULL for other networks.
    \param clientCAPS           Integer flags which capabilities this clients has.

    \todo clientCAPS should be a bitflag instead of a string
    \return A session ID which is essentialy a pointer to your session instance.
    \see Vat_DestroyNetworkSession
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
    Vat_DestroyNetworkSession deallocates an existing session. This should be done when a session is disconnected and will not be used anymore.

    \param session The session ID of the session that you want to terminate.
    \return None
    \see Vat_CreateNetworkSession
*/
VATLIB_API void Vat_DestroyNetworkSession(VatSessionID session);

/**
    Vat_SpecifyPilotLogon is used to declare the information for a pilot connection to the network. This does NOT connect you to the network by itself.

    \param session  The session ID to be used for this operation.
    \param server   A string containing the server address. It can be a string IP address or hostname.
    \param port     The port to connect to the server on.
    \param id       A string containing the CID of the pilot.
    \param password A string containing the password of the pilot.
    \param info     A struct containing the pilot specific information necessary for the connection to the server.
    \return None
    \see VatPilotConnection_t
*/
VATLIB_API void Vat_SpecifyPilotLogon(
    VatSessionID session,
    const char *server,
    int port,
    const char *id,
    const char *password,
    const VatPilotConnection *info);

/**
    Vat_SpecifyATCLogon is used to declare the information for an ATC connection to the network. This does NOT connect you to the network by itself.

    \param session  The session ID to be used for this operation.
    \param server   A string containing the server address. It can be a string IP address or hostname.
    \param port     A port > 0 to connect to the server on.
    \param id       A string containing the CID of the controller.
    \param password A string containing the password of the controller.
    \param info     A struct containing the controller specific information necessary for the connection to the server.
    \return None
    \see VatATCConnection_t
*/
VATLIB_API void Vat_SpecifyATCLogon(
    VatSessionID session,
    const char *server,
    int port,
    const char *id,
    const char *password,
    const VatAtcConnection *info);

/**
    Vat_Logon attempts to actually logon to the server or reconnect if disconnected. You must have already specified connection information
    using Vat_SpecifyPilotLogon or Vat_SpecifyATCLogon.

    \param session The session ID to be used for this operation.
    \see Vat_Logoff
    \see Vat_SpecifyPilotLogon
    \see Vat_SpecifyATCLogon
*/
VATLIB_API void Vat_Logon(VatSessionID session);

/**
    Vat_Logoff attempts to logoff the network.

    \param session The session ID to be used for this operation.
    \return None
    \see Vat_Logon
*/
VATLIB_API void Vat_Logoff(VatSessionID session);

/**
    Vat_GetStatus reads the status of your connection. Note that you should avoid using this function because it is better to install a notifier to detect
    when a status has changed. This routine does NOT read new status changes so no work is done and no callbacks will get called.

    \param session  The session ID to be used for this operation.
    \return None
*/
VATLIB_API VatConnectionStatus Vat_GetStatus(VatSessionID session);


/***************************************************************************
    BASIC COMMUNICATIONS FUNCTIONS
 ***************************************************************************/

/**
    Send a private 1-to-1 chat message.

    \param session  The session ID to be used for this operation.
    \param receiver A string containing the receiver callsign
    \param message  A string containing the text message
    \return None
*/
VATLIB_API void Vat_SendTextMessage(
    VatSessionID session,
    const char *receiver,
    const char *message);

/**
    Send a radio message on one or more frequencies, specified as an array of doubles.

    \param session      The session ID to be used for this operation.
    \param frequencies  Pointer to array of frequencies as integer values, e.g. 12345 for 123.45 MHz
    \param freqCount    Size of frequency array
    \param message      A string containing the text message
    \return None
*/
VATLIB_API void Vat_SendRadioMessage(VatSessionID session,
                                     int *frequencies,
                                     int freqCount,
                                     const char *message);

/**
    Used to request information from a client or server. Passing null into a
    callsign will probe the server for information.

    \param session The session ID to be used for this operation.
    \param type Type of information you want to request
    \param callsign Client callsign, you want to probe
    \return None
*/
VATLIB_API void Vat_RequestInformation(
    VatSessionID session,
    VatInfoQueryType type,
    const char *callsign);

/**
    Sends back information to the requester

    \param session The session ID to be used for this operation.
    \param type Type of information you want to request
    \param callsign Client callsign, you want to probe
    \param string containing requested data
    \return None
*/
VATLIB_API void Vat_SendInformation(
    VatSessionID session,
    VatInfoQueryType type,
    const char *callsign,
    const char *data);

/**
    Request CSL model info from someone else.

    \param session The session ID to be used for this operation.
    \param callsign Client callsign, you want to request from
    \return None
*/
VATLIB_API void Vat_RequestAircraftInfo(
    VatSessionID session,
    const char *callsign);


/***************************************************************************
    FUNCTIONS FOR ATC
 ***************************************************************************/

/**
    Send an ATC position update packet.

    \param session The session ID to be used for this operation.
    \param info Struct containing ATC position details
    \return None
    \see VatAtcPosition
*/
VATLIB_API void Vat_SendATCUpdate(
    VatSessionID session,
    const VatAtcPosition *info);

/**
    Requests an ATC Handoff.

    \param session The session ID to be used for this operation.
    \param receiver Controller callsign, from which you are requesting
    \param target Aircraft callsign, you would to hand off
    \return None
    \see VatAtcPosition
*/
VATLIB_API void Vat_RequestHandoff(VatSessionID session,
    const char *receiver,
    const char *target);

/**
    Accept an ATC Handoff

    \param session The session ID to be used for this operation.
    \param receiver Controller callsign, who requested the handoff
    \param target Aircraft callsign, which is handed off
    \return None
*/
VATLIB_API void Vat_AcceptHandoff(VatSessionID session,
    const char *receiver,
    const char *target);

/**
    Cancel a handoff with another ATC.

    \param session The session ID to be used for this operation.
    \param receiver Controller callsign, who requested the handoff
    \param target Aircraft callsign, which is handed off
    \return None
*/
VATLIB_API void Vat_CancelHandoff(
    VatSessionID session,
    const char *receiver,
    const char *target);

/**
    Amend a flight plan. The one stored on the server will be overwritten
    with the one you are sending now.

    \param session The session ID to be used for this operation.
    \param callsign Aircraft callsign, which flightplan needs to be amended.
    \param flightplan New full flightplan including all changed you have made.
    \return None
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

    \param session The session ID to be used for this operation.
    \param atc
    \return None
*/
VATLIB_API void Vat_SendSharedStateID(
    VatSessionID session,
    const char *atc);

/**
    Send a land line command to other aircraft.

    \param session The session ID to be used for this operation.
    \param callsign Aircraft callsign
    \param command Landline command
    \param type landline command type
    \param ip
    \param port
    \return None
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

    \param session The session ID to be used for this operation.
    \param atc ATC callsign, may be null if not needed.
    \param aircraft Aircraft callsign
    \param command Tracking command
    \return None
*/
VATLIB_API void Vat_SendTrackingCommand(
    VatSessionID session,
    const char *atc,
    const char *aircraft,
    VatTrackingCmd command);

/**
    Turns the break on and off

    \param session The session ID to be used for this operation.
    \param wantsBreak Pass true to turn the break on and false to turn it off.
    \return None
*/
VATLIB_API void Vat_SendBreakCommand(
    VatSessionID session,
    bool wantsBreak);

VATLIB_API void Vat_SendControllerAtis(VatSessionID session,
                                       const char *callsign,
                                       const VatControllerAtis *atis);

/**
    Sends a secondary ATC position update

    \param session The session ID to be used for this operation.
    \param index ???
    \param latitude
    \param longitude
    \return None
*/
VATLIB_API void Vat_SendSecondaryATCUpdate(
    VatSessionID session,
    int index,
    double latitude,
    double longitude);

/**
    Pushes a pilots flight strip into the bay of a controller

    \param session The session ID to be used for this operation.
    \param atcCallsign Controller callsign the flight strip is pushed to.
    \param aircraftCallsign Aircraft callsign, the flight strip belongs to.
    \return None
*/
VATLIB_API void Vat_PushFlightStrip(
    VatSessionID session,
    const char *atcCallsign,
    const char *aircraftCallsign);

/**
    Pushes a pilots flight strip with a type identifier and array of annotations - Added by RAC on 02/28/2007

    \param session The session ID to be used for this operation.
    \param atcCallsign Controller callsign the flight strip is pushed to.
    \param aircraftCallsign Aircraft callsign, the flight strip belongs to.
    \param type ???
    \param annotations Array of strings with annotations
    \return None
*/
VATLIB_API void Vat_PushFlightStripEx(
    VatSessionID session,
    const char *atcCallsign,
    const char *aircraftCallsign,
    int type,
    const char **annotations);

/**
    Turns the request for help on and off


    \param session The session ID to be used for this operation.
    \param needHelp Pass true to turn the help request on and false to turn it off.
    \param message Pass in a optional message. May be null if not needed.
    \return None
*/
VATLIB_API void Vat_SendHelpCommand(
    VatSessionID session,
    bool needHelp,
    const char *message);

/**
    Notifies all in-range clients that our controller info or ATIS has changed.
    This will automatically increase the ATIS identifier for other ATC clients
    in range.

    \param session The session ID to be used for this operation.
    \return None
*/
VATLIB_API void Vat_NotifyNewInfo(
    VatSessionID session);



/***************************************************************************
    FUNCTIONS FOR PILOTS
 ***************************************************************************/

/**
    Send a pilot position update packet.

    \param session The session ID to be used for this operation.
    \param info Struct with pilot position details
    \return None
    \see VatPilotPosition
*/
VATLIB_API void Vat_SendPilotUpdate(
    VatSessionID session,
    const VatPilotPosition *info);

/**
    Send an interim pilot position update packet. This is sent to a specific
    callsign only instead of broadcasted.
    It depends on the receiving clients software if they accept intermin pilot
    position updates.

    \param session The session ID to be used for this operation.
    \param callsign Receiving callsign
    \param info Struct with pilot position details
    \return None
    \see VatPilotPosition
*/
VATLIB_API void Vat_SendInterimPilotUpdate(
    VatSessionID session,
    const char *callsign,
    const VatPilotPosition *info);

/**
    Sends the server a flight plan.

    \param session The session ID to be used for this operation.
    \param flightPlan Your flightplan
    \return None
*/
VATLIB_API void Vat_SendFlightPlan(
    VatSessionID session,
    const VatFlightPlan *flightPlan);

/**
    Tell another client about your plane's info (CSL model, etc.)

    \param session The session ID to be used for this operation.
    \param callsign Receiver callsign
    \param engineType Engine Type
    \param CSLModel A string containing your CSL model
    \return None
    \deprecated Do not use this method anymore. This legacy is just
    here for SB2.3 clients. Most should use the modern one.
    \see Vat_SendModernPlaneInfo
*/
VATLIB_API void Vat_SendLegacyPlaneInfo(
    VatSessionID session,
    const char *callsign,
    VatEngineType engineType,
    const char *CSLModel);

/**
    Tell another client about your plane's info (CSL model, etc.).
    This is the modern send plane info that SB3, FSInn and XSB will use.

    \param session The session ID to be used for this operation.
    \param callsign Receiver callsign
    \param info Struct with plane information
    \return None
*/
VATLIB_API void Vat_SendModernPlaneInfo(
    VatSessionID session,
    const char *callsign,
    const VatAircraftInfo *info);

VATLIB_API void Vat_SendAircraftConfig(
    VatSessionID session,
    const char *receiver,
    const char *aircraftConfig);

VATLIB_API void Vat_SendAircraftConfigBroadcast(
    VatSessionID session,
    const char *aircraftConfig);

VATLIB_API void Vat_SendCustomPilotPacket(
    VatSessionID session,
    const char *receiver,
    const char *packetSubType,
    const char **data,
    int dataCount);


/***************************************************************************
    ADMIN FUNCTIONS
 ***************************************************************************/

/**
    Kills a user off of the server with a message

    \param session The session ID to be used for this operation.
    \param callsign A string containing the callsign to be killed from the network
    \param message A string containing a message to the user
    \return None
*/
VATLIB_API void Vat_KillUser(
    VatSessionID session,
    const char *callsign,
    const char *message);

/**
    Sends a ping packet to the server.
    Pass in a callsign to ping a user, *A to ping all ATC or ?? to ping everyone.

    \param session The session ID to be used for this operation.
    \param callsign A string containing the receiver callsign
    \return None
*/
VATLIB_API void Vat_SendPing(
    VatSessionID session,
    const char *callsign);

/**
    Sends a pong back after receiving a ping request.

    \param session The session ID to be used for this operation.
    \param callsign A string containing the receiver callsign
    \param time Format???
    \return None
*/

VATLIB_API void Vat_SendPong(
    VatSessionID session,
    const char *callsign,
    const char *time);

/***************************************************************************
    WEATHER FUNCTIONS
 ***************************************************************************/

/* Used to request ACARS information. */
VATLIB_API void Vat_RequestMetar(
    VatSessionID session,
    const char *station);

/* Sends ACARS information to receiver. */
VATLIB_API void Vat_SendMetar(
    VatSessionID session,
    const char *receiver,
    const char *data);

/**
    Sends a request to the server for weather

    \param session The session ID to be used for this operation.
    \param station The station whether is requested for
    \return None
*/
VATLIB_API void Vat_RequestWeather(
    VatSessionID session,
    const char *station);

/****************************************************************
    GENERAL VVL NOTES
 ****************************************************************

    Your application is responsable for memory management of all
    VVL "objects" - each one has a Create and Delete routine.

    Like PCSB, most callbacks take a void    parameter.  You can
    use this to pass data from your call out of VVL to your
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
    Vat_CreateUDPAudioPort
 *
    Creates a new VVL UDP audio port. Pass the UDP port number that your
    client should use.  This is usually port 3290.  You should only need
    one of these in your client.
 *
 */
VATLIB_API VatUDPAudioPort Vat_CreateUDPAudioPort(VatAudioService audioService, int port);

/**
    Vat_DestroyUDPAudioPort
 *
    Deallocates a VVLUDPAudioPort object. Delete your port on cleanup.
 *
 */
VATLIB_API void Vat_DestroyUDPAudioPort(VatUDPAudioPort audioPort);

/**
    Vat_GetPortNumber
 *
    This accessor returns the port number of the port object.
 *
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
VATLIB_API VatLocalInputCodec Vat_CreateLocalInputCodec(VatAudioService audioService);

VATLIB_API VatLocalOutputCodec Vat_CreateLocalOutputCodec(VatAudioService audioService);

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

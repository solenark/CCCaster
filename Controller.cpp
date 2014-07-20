#include "Controller.h"
#include "Logger.h"

#include <SDL.h>

#include <cstdlib>

using namespace std;


#define DEFAULT_DEADZONE    25000

#define EVENT_KEYBOARD      0
#define EVENT_JOY_AXIS      1
#define EVENT_JOY_HAT       2
#define EVENT_JOY_BUTTON    3

#define AXIS_POSITIVE       1
#define AXIS_NEGATIVE       2


void Controller::joystickEvent ( const SDL_JoyAxisEvent& event )
{
    uint8_t value = 0;
    if ( abs ( event.value ) > deadzones[event.axis] )
        value = ( event.value > 0 ? AXIS_POSITIVE : AXIS_NEGATIVE );

    if ( keyToMap != 0 )
    {
        uint32_t *activeAxes = activeMappings[EVENT_JOY_AXIS][event.axis];
        bool isActive = ( activeAxes[AXIS_POSITIVE] || activeAxes[AXIS_NEGATIVE] );

        if ( value == 0 && isActive )
        {
            // Done mapping if the axis returned to 0
            mappings[EVENT_JOY_AXIS][event.axis][activeAxes[AXIS_POSITIVE] ? AXIS_POSITIVE : AXIS_NEGATIVE] = keyToMap;

            mappings[EVENT_JOY_AXIS][event.axis][0] =
                ( mappings[EVENT_JOY_AXIS][event.axis][AXIS_POSITIVE]
                  | mappings[EVENT_JOY_AXIS][event.axis][AXIS_NEGATIVE] );

            LOG_CONTROLLER ( this, "Mapped axis%d %s to %08x",
                             event.axis, ( activeAxes[AXIS_POSITIVE] ? "+" : "-" ), keyToMap );

            if ( owner != 0 )
                owner->doneMapping ( this, keyToMap );
            cancelMapping();
        }

        // Otherwise ignore already active mappings
        if ( isActive )
            return;

        activeAxes[value] = keyToMap;
        return;
    }

    uint32_t key = mappings[EVENT_JOY_AXIS][event.axis][value];

    if ( key != 0 )
    {
        if ( value == 0 )
            state &= ~key;
        else
            state |= key;
    }

    LOG_CONTROLLER ( this, "axis=%d; value=%s; EVENT_JOY_AXIS",
                     event.axis, ( value == 0 ? "0" : ( value == AXIS_POSITIVE ? "+" : "-" ) ) );
}

static int ConvertHatToNumPad ( int hat )
{
    int dir = 5;

    if ( hat != SDL_HAT_CENTERED )
    {
        if ( hat & SDL_HAT_UP )
            dir = 8;
        else if ( hat & SDL_HAT_DOWN )
            dir = 2;

        if ( hat & SDL_HAT_LEFT )
            --dir;
        else if ( hat & SDL_HAT_RIGHT )
            ++dir;
    }

    return dir;
}

void Controller::joystickEvent ( const SDL_JoyHatEvent& event )
{
    uint8_t vert = event.value & ( SDL_HAT_LEFT | SDL_HAT_RIGHT );
    uint8_t hori = event.value & ( SDL_HAT_UP | SDL_HAT_DOWN );

    // if ( keyToMap != 0 )
    // {
    //     uint32_t *activeValues = activeMappings[EVENT_JOY_HAT][event.hat];
    //     uint8_t activeValue = 0;
    //     for ( int i = 0; i < 16; ++i )
    //     {
    //         if ( activeValues[i] )
    //         {
    //             activeValue = i;
    //             break;
    //         }
    //     }

    //     if ( event.value == SDL_HAT_CENTERED && activeValue )
    //     {
    //         // Done mapping if the hat is centered
    //         mappings[EVENT_JOY_HAT][event.hat][activeValue] = activeValues[activeValue];

    //         mappings[EVENT_JOY_HAT][event.hat][SDL_HAT_CENTERED] = 0;
    //         for ( int i = 0; i < 16; ++i )
    //             mappings[EVENT_JOY_HAT][event.hat][SDL_HAT_CENTERED] |= mappings[EVENT_JOY_HAT][event.hat][i];

    //         LOG_CONTROLLER ( this, "Mapped hat%d %d to %08x",
    //                          event.hat, ConvertHatToNumPad ( activeValue ), keyToMap );

    //         if ( owner != 0 )
    //             owner->doneMapping ( this, keyToMap );
    //         cancelMapping();
    //     }

    //     // Otherwise ignore already active mappings
    //     if ( activeValue )
    //         return;

    //     activeValues[event.value] = keyToMap;
    //     return;
    // }

    uint32_t key1 = mappings[EVENT_JOY_HAT][event.hat][vert];

    if ( key1 != 0 )
    {
        if ( vert == SDL_HAT_CENTERED )
            state &= ~key1;
        else
            state |= key1;
    }

    uint32_t key2 = mappings[EVENT_JOY_HAT][event.hat][hori];

    if ( key2 != 0 )
    {
        if ( hori == SDL_HAT_CENTERED )
            state &= ~key2;
        else
            state |= key2;
    }

    LOG_CONTROLLER ( this, "hat=%d; value=%d; EVENT_JOY_HAT", event.hat, ConvertHatToNumPad ( event.value ) );
}

void Controller::joystickEvent ( const SDL_JoyButtonEvent& event )
{
    uint32_t key = mappings[EVENT_JOY_BUTTON][event.button][event.state];

    if ( keyToMap != 0 )
    {
        uint32_t *activeStates = activeMappings[EVENT_JOY_BUTTON][event.button];
        bool isActive = ( activeStates[SDL_PRESSED] );

        if ( event.state == SDL_RELEASED && isActive )
        {
            // Done mapping if the button was tapped
            mappings[EVENT_JOY_BUTTON][event.button][SDL_PRESSED]
                = mappings[EVENT_JOY_BUTTON][event.button][SDL_RELEASED] = keyToMap;

            LOG_CONTROLLER ( this, "Mapped button%d to %08x", event.button, keyToMap );

            if ( owner != 0 )
                owner->doneMapping ( this, keyToMap );
            cancelMapping();
        }

        // Otherwise ignore already active mappings
        if ( isActive )
            return;

        activeStates[event.state] = keyToMap;
        return;
    }

    if ( key == 0 )
        return;

    if ( event.state == SDL_RELEASED )
        state &= ~key;
    else if ( event.state == SDL_PRESSED )
        state |= key;

    LOG_CONTROLLER ( this, "button=%d; value=%d; EVENT_JOY_BUTTON", event.button, ( event.state == SDL_PRESSED ) );
}

Controller::Controller ( KeyboardEnum ) : joystick ( 0 ), state ( 0 ), keyToMap ( 0 )
{
    memset ( &guid, 0, sizeof ( guid ) );

    clearMapping();
}

Controller::Controller ( SDL_Joystick *joystick ) : joystick ( joystick ), state ( 0 ), keyToMap ( 0 )
{
    SDL_JoystickGUID guid = SDL_JoystickGetGUID ( joystick );
    memcpy ( &this->guid.guid, guid.data, sizeof ( guid.data ) );

    auto it = guidBitset.find ( this->guid.guid );

    if ( it == guidBitset.end() )
    {
        this->guid.index = 0;
        guidBitset[this->guid.guid] = 1u;
    }
    else
    {
        for ( uint32_t i = 0; i < 32; ++i )
        {
            if ( ( it->second & ( 1u << i ) ) == 0u )
            {
                guidBitset[this->guid.guid] |= ( 1u << i );
                this->guid.index = i;
                return;
            }
        }

        Exception err = toString ( "Too many duplicate guids for: '%s'", this->guid.guid );
        LOG_AND_THROW ( err, "" );
    }

    clearMapping();

    // Default axis mappings
    mappings[EVENT_JOY_AXIS][0][0] = MASK_X_AXIS;
    mappings[EVENT_JOY_AXIS][0][AXIS_POSITIVE] = BIT_RIGHT;
    mappings[EVENT_JOY_AXIS][0][AXIS_NEGATIVE] = BIT_LEFT;
    mappings[EVENT_JOY_AXIS][1][0] = MASK_Y_AXIS;
    mappings[EVENT_JOY_AXIS][1][AXIS_POSITIVE] = BIT_DOWN; // SDL joystick Y-axis is inverted
    mappings[EVENT_JOY_AXIS][1][AXIS_NEGATIVE] = BIT_UP;
    mappings[EVENT_JOY_AXIS][2][0] = MASK_X_AXIS;
    mappings[EVENT_JOY_AXIS][2][AXIS_POSITIVE] = BIT_RIGHT;
    mappings[EVENT_JOY_AXIS][2][AXIS_NEGATIVE] = BIT_LEFT;
    mappings[EVENT_JOY_AXIS][3][0] = MASK_Y_AXIS;
    mappings[EVENT_JOY_AXIS][3][AXIS_POSITIVE] = BIT_DOWN; // SDL joystick Y-axis is inverted
    mappings[EVENT_JOY_AXIS][3][AXIS_NEGATIVE] = BIT_UP;

    // Default hat mappings
    mappings[EVENT_JOY_HAT][0][SDL_HAT_CENTERED] = ( MASK_X_AXIS | MASK_Y_AXIS );
    mappings[EVENT_JOY_HAT][0][SDL_HAT_UP]       = BIT_UP;
    mappings[EVENT_JOY_HAT][0][SDL_HAT_RIGHT]    = BIT_RIGHT;
    mappings[EVENT_JOY_HAT][0][SDL_HAT_DOWN]     = BIT_DOWN;
    mappings[EVENT_JOY_HAT][0][SDL_HAT_LEFT]     = BIT_LEFT;
}

Controller::~Controller()
{
    auto it = guidBitset.find ( guid.guid );

    if ( it == guidBitset.end() )
        return;

    if ( guid.index >= 32 )
        return;

    guidBitset[guid.guid] &= ~ ( 1u << guid.index );
}

void Controller::startMapping ( Owner *owner, uint32_t key )
{
    cancelMapping();

    LOG ( "Starting mapping %08x", key );

    this->owner = owner;
    keyToMap = key;
}

void Controller::cancelMapping()
{
    owner = 0;
    keyToMap = 0;

    for ( auto& a : activeMappings )
        for ( auto& b : a )
            for ( auto& c : b )
                c = 0;
}

void Controller::clearMapping()
{
    for ( auto& a : mappings )
        for ( auto& b : a )
            for ( auto& c : b )
                c = 0;

    for ( auto& v : deadzones )
        v = DEFAULT_DEADZONE;
}

unordered_map<Guid, uint32_t> Controller::guidBitset;

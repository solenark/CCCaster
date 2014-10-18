#pragma once

#include <cstdint>
#include <iostream>


// Number of frames of inputs to send per message
#define NUM_INPUTS                  30


// Game constants and addresses are prefixed CC
#define CC_VERSION                  "1.4.0"
#define CC_TITLE                    "MELTY BLOOD Actress Again Current Code Ver.1.07 Rev." CC_VERSION
#define CC_STARTUP_TITLE_EN         CC_TITLE " Startup Menu"
#define CC_STARTUP_TITLE_JP         CC_TITLE " ‹N“®ƒƒjƒ…["
#define CC_STARTUP_BUTTON           "OK"
#define CC_NETWORK_CONFIG_FILE      "System\\NetConnect.dat"
#define CC_NETWORK_USERNAME_KEY     "UserName"

#define CC_LOOP_START_ADDR          ( ( char * )     0x40D330 ) // Start of the main event loop
#define CC_SCREEN_WIDTH_ADDR        ( ( uint32_t * ) 0x54D048 ) // The width of the main viewport
#define CC_DAMAGE_LEVEL_ADDR        ( ( uint32_t * ) 0x553FCC ) // Default 2
#define CC_WIN_COUNT_VS_ADDR        ( ( uint32_t * ) 0x553FDC ) // Default 2
#define CC_TIMER_SPEED_ADDR         ( ( uint32_t * ) 0x553FD0 ) // Default 2
#define CC_AUTO_REPLAY_SAVE_ADDR    ( ( uint32_t * ) 0x553FE8 ) // 0 to disable, 1 to enable
#define CC_WORLD_TIMER_ADDR         ( ( uint32_t * ) 0x55D1D4 ) // Frame step timer, always counting up
#define CC_SKIP_FRAMES_ADDR         ( ( uint32_t * ) 0x55D25C ) // Set to N to disable FPS limit for N frames
#define CC_ROUND_TIMER_ADDR         ( ( uint32_t * ) 0x562A3C ) // Counts down from 4752, may stop
#define CC_REAL_TIMER_ADDR          ( ( uint32_t * ) 0x562A40 ) // Counts up from 0 after round start
#define CC_STAGE_SELECTOR_ADDR      ( ( uint32_t * ) 0x74FD98 ) // Currently selected stage, can be written to
#define CC_FPS_COUNTER_ADDR         ( ( uint32_t * ) 0x774A70 ) // Value of the displayed FPS counter
#define CC_PERF_FREQ_ADDR           ( ( uint64_t * ) 0x774A80 ) // Value of QueryPerformanceFrequency for game FPS

#define CC_PTR_TO_WRITE_INPUT_ADDR  ( ( char * ) 0x76E6AC )     // Pointer to the location to write game input
#define CC_P1_OFFSET_DIRECTION      ( 0x18 )                    // Offset to write P1 direction input
#define CC_P1_OFFSET_BUTTONS        ( 0x24 )                    // Offset to write P1 buttons input
#define CC_P2_OFFSET_DIRECTION      ( 0x2C )                    // Offset to write P2 direction input
#define CC_P2_OFFSET_BUTTONS        ( 0x38 )                    // Offset to write P2 buttons input

// Directions are just written in numpad format, EXCEPT neutral is 0
#define CC_BUTTON_A                 ( 0x0010 )
#define CC_BUTTON_B                 ( 0x0020 )
#define CC_BUTTON_C                 ( 0x0008 )
#define CC_BUTTON_D                 ( 0x0004 )
#define CC_BUTTON_E                 ( 0x0080 )
#define CC_BUTTON_AB                ( 0x0040 )
#define CC_BUTTON_START             ( 0x0001 )
#define CC_BUTTON_FN1               ( 0x0100 )
#define CC_BUTTON_FN2               ( 0x0200 )
#define CC_BUTTON_SELECT            ( 0x0400 )
#define CC_BUTTON_CANCEL            ( 0x0800 )

#define CC_GAME_MODE_ADDR           ( ( uint32_t * ) 0x54EEE8 ) // Current game mode, constants below

// List of game modes relevant to netplay
#define CC_GAME_MODE_STARTUP        ( 65535 )
#define CC_GAME_MODE_OPENING        ( 3 )
#define CC_GAME_MODE_TITLE          ( 2 )
#define CC_GAME_MODE_LOADING_DEMO   ( 13 )
#define CC_GAME_MODE_HIGH_SCORES    ( 11 )
#define CC_GAME_MODE_MAIN           ( 25 )
#define CC_GAME_MODE_CHARA_SELECT   ( 20 )
#define CC_GAME_MODE_LOADING        ( 8 )
#define CC_GAME_MODE_INGAME         ( 1 )
#define CC_GAME_MODE_RETRY          ( 5 )

// Character select data, can be assigned directly to update the game state
#define CC_P1_CHARACTER_ADDR        ( ( uint8_t * )  0x74D8F8 )
#define CC_P1_MOON_ADDR             ( ( uint8_t * )  0x74D900 )
#define CC_P1_COLOR_ADDR            ( ( uint8_t * )  0x74D904 )
#define CC_P2_CHARACTER_ADDR        ( ( uint8_t * )  0x74D91C )
#define CC_P2_MOON_ADDR             ( ( uint8_t * )  0x74D924 )
#define CC_P2_COLOR_ADDR            ( ( uint8_t * )  0x74D928 )

// Complete RNG state
#define CC_RNGSTATE0_ADDR           ( ( uint32_t * ) 0x563778 )
#define CC_RNGSTATE1_ADDR           ( ( uint32_t * ) 0x56377C )
#define CC_RNGSTATE2_ADDR           ( ( uint32_t * ) 0x564068 )
#define CC_RNGSTATE3_ADDR           ( ( char * )     0x56406C )
#define CC_RNGSTATE3_SIZE           ( 224 )

// Character select selection mode
#define CC_CHARA_SELECT_CHARA       ( 0 )
#define CC_CHARA_SELECT_MOON        ( 1 )
#define CC_CHARA_SELECT_COLOUR      ( 2 )


// Asm hacks are prefixed MM (for modified memory), they should be written to safe locations
#define MM_HOOK_CALL1_ADDR          ( ( char * ) 0x40D032 )
#define MM_HOOK_CALL2_ADDR          ( ( char * ) 0x40D411 )


union IndexedFrame
{
    struct { uint32_t index, frame; } parts;
    uint64_t value;
};

inline std::ostream& operator<< ( std::ostream& os, const IndexedFrame& indexedFrame )
{
    return ( os << indexedFrame.parts.index << ':' << indexedFrame.parts.frame );
}


inline const char *gameModeStr ( uint32_t gameMode )
{
    switch ( gameMode )
    {
        case CC_GAME_MODE_STARTUP:
            return "Startup";

        case CC_GAME_MODE_OPENING:
            return "Opening";

        case CC_GAME_MODE_TITLE:
            return "Title";

        case CC_GAME_MODE_LOADING_DEMO:
            return "Loading-demo";

        case CC_GAME_MODE_HIGH_SCORES:
            return "High-scores";

        case CC_GAME_MODE_MAIN:
            return "Main";

        case CC_GAME_MODE_CHARA_SELECT:
            return "Character-select";

        case CC_GAME_MODE_LOADING:
            return "Loading";

        case CC_GAME_MODE_INGAME:
            return "In-game";

        case CC_GAME_MODE_RETRY:
            return "Retry";

        default:
            break;
    }

    return "Unknown game mode!";
}

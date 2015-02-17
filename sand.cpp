/* Sand is a lightweight and simple time framework written in C++11. BOOST licensed.
 * Sand supports Unix stamps, hires timers, calendars and locales.
 * Copyright (c) 2010-2014 Mario 'rlyeh' Rodriguez

 * Simple fps framerate locker. based on code by /u/concavator (ref: http://goo.gl/Ry50A4)

 * issues:
 * - if year < 1900, sand::rtc() behavior is undefined

 * to do:
 * - grain -> struct grain { std::uint64_t seconds, fract; };
 * - looper/rtc -> serialize factor as well (and held?)
 * - move factor/shift and pause/resume to dt
 * - kiloseconds, ks | ref: http://bavardage.github.com/Kiloseconds/
 * - something like http://momentjs.com/ for pretty printing
 * - also, https://code.google.com/p/datejs/

 * - rlyeh ~~ listening to The Mission / Butterfly on a wheel
 */

#include <cassert>
#include <cmath>
#include <ctime>

#include <chrono>
#include <deque>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "sand.hpp"

#if defined(_WIN32)
#   include <Windows.h>
#   define kSandTimerHandle                      LARGE_INTEGER
#   define kSandTimerFreq( handle )              { kSandTimerHandle fhandle; DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0); ::QueryPerformanceFrequency( &fhandle ); ::SetThreadAffinityMask(::GetCurrentThread(), oldmask); frequency = 1000000.0 / double(fhandle.QuadPart); }
#   define kSandTimerUpdate( handle )            {                           DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0); ::QueryPerformanceCounter  ( &handle ); ::SetThreadAffinityMask(::GetCurrentThread(), oldmask); }
#   define kSandTimerSetCounter( handle, value ) handle.QuadPart = value
#   define kSandTimerDiffCounter( handle1, handle2 ) ( ( handle2.QuadPart - handle1.QuadPart ) * frequency )
#   define kSandTimerSleep( seconds_f )          Sleep( (int)(seconds_f * 1000) )
#   define kSandTimerWink( units_t )             Sleep( units_t )
#else
#   include <sys/time.h>
#   include <unistd.h>
//  hmmm... check clock_getres() as seen in http://tdistler.com/2010/06/27/high-performance-timing-on-linux-windows#more-350
//  frequency int clock_getres(clockid_t clock_id, struct timespec *res);
//  clock     int clock_gettime(clockid_t clock_id, struct timespec *tp);
//  nanosleep() instead?
#   define kSandTimerHandle                      timeval
#   define kSandTimerFreq( handle )
#   define kSandTimerUpdate( handle )            gettimeofday( &handle, NULL )
#   define kSandTimerSetCounter( handle, value ) do { handle.tv_sec = 0; handle.tv_usec = value; } while (0)
#   define kSandTimerDiffCounter( handle1, handle2 ) ( (handle2.tv_sec * 1000000.0) + handle2.tv_usec ) - ( (handle1.tv_sec * 1000000.0) + handle1.tv_usec )
#   define kSandTimerSleep( seconds_f )          usleep( seconds_f * 1000000.f )
#   define kSandTimerWink( units_t )             usleep( units_t )
//  do { float fractpart, intpart; fractpart = std::modf( seconds_f, &intpart); \
//    ::sleep( int(intpart) ); usleep( int(fractpart * 1000000) ); } while( 0 )
#endif

#ifdef SAND_USE_OMP_TIMERS
// todo: test this
#include <omp.h>
#undef  kSandTimerHandle
#define kSandTimerHandle                            double
#undef  kSandTimerFreq
#define kSandTimerFreq( handle )
#undef  kSandTimerUpdate
#define kSandTimerUpdate( handle )                  { handle = omp_get_wtime(); }
#undef  kSandTimerSetCounter
#define kSandTimerSetCounter( handle, value )       ( handle = value / 1000000.0 )
#undef  kSandTimerDiffCounter
#define kSandTimerDiffCounter( handle1, handle2 )   ( handle2 - handle1 )
#endif

#ifdef  _MSC_VER
#define $msc   $yes
#define $melse $no
#else
#define $msc   $no
#define $melse $yes
#endif

#define $yes(...) __VA_ARGS__
#define $no(...)

#if defined(__GNUC__) && (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ <= 40902 )
    namespace std
    {
        static std::string put_time( const std::tm* tmb, const char* fmt ) {
            std::string s( 128, '\0' );
            while( !strftime( &s[0], s.size(), fmt, tmb ) )
                s.resize( s.size() + 128 );
            return s;
        }
    }
#endif

/////////////////////////////////////////////////////////////////////////////

namespace sand
{
namespace
{
    int floor( double f ) {
        return int( ::floor(f) );
    }
    int ceil( double f ) {
        return int( ::ceil(f) );
    }
    class custom : public std::string
    {
        public:

        custom() : std::string()
        {}

        template<typename T>
        custom( const T &t ) : std::string() {
            std::stringstream ss;
            if( ss << t ) this->assign( ss.str() );
        }

        std::deque< custom > tokenize( const std::string &delimiters ) const { // taken from wire::string::tokenize
            std::string map( 256, '\0' );
            for( const unsigned char &ch : delimiters )
                map[ ch ] = '\1';
            std::deque< custom > tokens(1);
            for( const unsigned char &ch : *this ) {
                /**/ if( !map.at(ch)          ) tokens.back().push_back( char(ch) );
                else if( tokens.back().size() ) tokens.push_back( custom() );
            }
            while( tokens.size() && !tokens.back().size() ) tokens.pop_back();
            return tokens;
        }

        template <typename T>
        T as() const {
            T t;
            std::stringstream ss;
            ss << *this;
            return ss >> t ? t : T();
        }
    };

    namespace legacy
    {
        auto epoch = [](){
            return std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        };

        class dt
        {
            public:

            dt()
            {
                reset();
            }

            double s() //const
            {
                return us() / 1000000.0;
            }

            double ms() //const
            {
                return us() / 1000.0;
            }

            double us() //const
            {
                kSandTimerUpdate( endCount );

                return kSandTimerDiffCounter( startCount, endCount );
            }

            double ns() //const
            {
                return us() * 1000.0;
            }

            void reset()
            {
                kSandTimerFreq( frequency ); //to dt2() ctor ?

                kSandTimerSetCounter( startCount, 0 );
                kSandTimerSetCounter( endCount, 0 );

                kSandTimerUpdate( startCount );
            }

            protected:

            kSandTimerHandle startCount, endCount;
            double frequency;
        };
    }
}
}

namespace sand
{
    namespace
    {
        double offset = 0;
        legacy::dt local;
    }

    double now() {
        static const double app_epoch = double( std::time(NULL) );
        return offset + local.s() + app_epoch;
    }

    double uptime() {
        return offset + local.s();
    }

    void lapse( double t ) {
        offset += t;
    }

    std::string locale( double timestamp_secs, const std::string &locale_, const std::string &format ) { // taken from sole::printftime
        std::string timef;
        try {
            std::time_t t = uint64_t( timestamp_secs );
            std::tm tm;
            $msc(
                localtime_s( &tm, &t );
            )
            $melse(
                localtime_r( &t, &tm );
            )

            std::stringstream ss;

            std::locale lc( locale_.c_str() );
            ss.imbue( lc );
            ss << std::put_time( &tm, format.empty() ? "%c" : format.c_str() );

            timef = ss.str();
        }
        catch(...) {
            timef = "";
        }
        return timef;
    }

    std::string format( double t, const std::string &format_, const std::string &locale_ )
    {
#if 0
        char pBuffer[80];

        struct tm * timeinfo;
        time_t stored = (time_t)( t );
        timeinfo = localtime ( &stored );
        strftime(pBuffer, 80, format_.c_str(), timeinfo);

        return pBuffer;
#else
        return locale( t, locale_, format_ );
#endif
    }


    double str( const std::string &timestamp_YMDhms ) {
        std::deque< custom > token = custom( timestamp_YMDhms ).tokenize(":-/ "); //:)

        if( token.size() < 6 )
            return 0;

        struct tm timeinfo;

        //months are in [0..] range where days, hours, mins and secs use [1..] (doh!)
        timeinfo.tm_year  = token[0].as<int>() - 1900;
        timeinfo.tm_mon   = token[1].as<int>() - 1;
        timeinfo.tm_mday  = token[2].as<int>();
        timeinfo.tm_hour  = token[3].as<int>();
        timeinfo.tm_min   = token[4].as<int>();
        timeinfo.tm_sec   = token[5].as<int>();
        //-1 = do not adjust daylight savings
        timeinfo.tm_isdst = -1;

        return double( mktime( &timeinfo ) );
    }

    std::string str( double t ) {
        return format( t, "%Y-%m-%d %H:%M:%S" );
    }

    void wink() {
        kSandTimerWink( 1 );
    }

    void sleep( double seconds ) {
        kSandTimerSleep( seconds );
        return;

        /*
        sand::dt dt;
        while( dt.s() < secs ) {
            sand::wink();
        }
        */

        std::chrono::microseconds duration( (int)(seconds * 1000000) );
        std::this_thread::sleep_for( duration );
    }

    double nanoseconds( double t ) {
        return t / 1000000000.0;
    }
    double microseconds( double t ) {
        return t / 1000000.0;
    }
    double milliseconds( double t ) {
        return t / 1000.0;
    }
    double seconds( double t ) {
        return t;
    }
    double minutes( double t ) {
        return t * seconds(60);
    }
    double hours( double t ) {
        return t * minutes(60);
    }
    double days( double t ) {
        return t * hours(24);
    }
    double weeks( double t ) {
        return t * days(7);
    }
    double months( double t ) {
        return t * ( weeks(t) / 4 );
    }
    double years( double t ) {
        return t * days(365.242190402); // + days(n/4);
    }

    double calendar( const std::string &YMDhms ) {
        return str( YMDhms );
    }

    double to_nanoseconds( double t ) {
        return t * 1000000000.0;
    }
    double to_microseconds( double t ) {
        return t * 1000000.0;
    }
    double to_milliseconds( double t ) {
        return t * 1000.0;
    }
    double to_seconds( double t ) {
        return t;
    }
    double to_minutes( double t ) {
        return t / seconds(60);
    }
    double to_hours( double t ) {
        return t / minutes(60);
    }
    double to_days( double t ) {
        return t / hours(24);
    }
    double to_weeks( double t ) {
        return t / days(7);
    }
    double to_months( double t ) {
        return t / ( to_weeks(t) / 4 );
    }
    double to_years( double t ) {
        return t / days(365.242190402); // + days(n/4);
    }

    int year( double t ) {
        return custom( format(t, "%Y") ).as<int>();
    }
    int month( double t ) {
        return custom( format(t, "%m") ).as<int>();
    }
    int day( double t ) {
        return custom( format(t, "%d") ).as<int>();
    }

    int hour( double t ) {
        return custom( format(t, "%H") ).as<int>();
    }
    int minute( double t ) {
        return custom( format(t, "%M") ).as<int>();
    }
    int second( double t ) {
        return custom( format(t, "%S") ).as<int>();
    }
}


namespace sand
{
    std::string ago( double diff_seconds ) {
        // based on code by John Resig (jquery.com)
        int abs_diff = int(std::abs(diff_seconds));
        int day_diff = sand::floor(abs_diff / 86400);

        if( day_diff == 0 ) {
            if( abs_diff <   60 ) return "just now";
            if( abs_diff <  120 ) return "a minute ago";
            if( abs_diff < 3600 ) return std::to_string(sand::floor(abs_diff/60)) + " minutes ago";
            if( abs_diff < 7200 ) return "an hour ago";
            return std::to_string( sand::floor(abs_diff/3600) ) + " hours ago";
        }
        if( day_diff ==  1 ) return "yesterday";
        if( day_diff <= 13 ) return std::to_string(day_diff) + " days ago";
        if( day_diff  < 31 ) return std::to_string(sand::ceil(day_diff/7)) + " weeks ago";
        if( day_diff  < 62 ) return "a month ago";
        if( day_diff < 365 ) return std::to_string(sand::ceil(day_diff/31)) + " months ago";
        if( day_diff < 730 ) return "a year ago";
        return std::to_string(sand::ceil(day_diff/365)) + " years ago";
    }

    std::string in( double diff_seconds ) {
        // based on code by John Resig (jquery.com)
        int abs_diff = int(std::abs(diff_seconds));
        int day_diff = sand::floor(abs_diff / 86400);

        if( day_diff == 0 ) {
            if( abs_diff <   60 ) return "right now";
            if( abs_diff <  120 ) return "in a minute";
            if( abs_diff < 3600 ) return std::string("in ") + std::to_string(sand::floor(abs_diff/60)) + " minutes";
            if( abs_diff < 7200 ) return "in an hour";
            return std::string("in ") + std::to_string( sand::floor(abs_diff/3600) ) + " hours ago";
        }
        if( day_diff ==  1 ) return "tomorrow";
        if( day_diff <= 13 ) return std::string("in ") + std::to_string(day_diff) + " days";
        if( day_diff  < 31 ) return std::string("in ") + std::to_string(sand::ceil(day_diff/7)) + " weeks";
        if( day_diff  < 62 ) return "in a month";
        if( day_diff < 365 ) return std::string("in ") + std::to_string(sand::ceil(day_diff/31)) + " months";
        if( day_diff < 730 ) return "in a year";
        return std::string("in ") + std::to_string(sand::ceil(day_diff/365)) + " years";
    }

    std::string pretty( double diff_seconds ) {
        return diff_seconds < 0 ? ago(diff_seconds) : in(diff_seconds);
    }

    std::string diff( double since, double then ) {
        double diff_seconds = abs(then) - abs(since);
        return pretty( diff_seconds );
    }

    std::string diff( double then ) {
        return diff( then, now() );
    }
}

#undef kSandTimerWink
#undef kSandTimerHandle
#undef kSandTimerFreq
#undef kSandTimerUpdate
#undef kSandTimerDiffCounter
#undef kSandTimerSetCounter
#undef kSandTimerGetCounter
#undef kSandTimerSleep
#undef kSandTimerWink

#undef $yes
#undef $no
#undef $melse
#undef $msc




// simple fps framerate locker. based on code by /u/concavator (ref: http://goo.gl/Ry50A4)
// - rlyeh. BOOST licensed

#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

namespace {
    // function that locks your logic to desired framerate (in HZ).
    // if optional FPS arg is provided, function fills it up with number of frames per second.
    // returns true if you should render your game after logic update; else update logic only.
    bool lock_impl( unsigned HZ, unsigned *FPS ) {
        // rw vars
        static volatile unsigned hz = 60, isGameRunning = 1, maxframeskip = 10;
        // ro vars
        static volatile unsigned fps = 0;
        // private vars
        static volatile unsigned timer_counter = 0, loop_counter = 0;
        // private threaded timer
        static struct install {
            install() {
                std::thread([&]{
                    std::chrono::microseconds acc( 0 ), third( 300000 );
                    while( isGameRunning ) {
                        // update timer
                        timer_counter++;
                        std::chrono::microseconds duration( int(1000000/hz) );
                        std::this_thread::sleep_for( duration );
                        // update fps 3 times per second
                        acc += duration;
                        if( acc >= third ) {
                            acc -= acc;
                            static int before = loop_counter;
                            fps = int( round( (loop_counter - before) * 3.3333333 ) );
                            before = loop_counter;
                        }
                    }
                    isGameRunning = 1;
                }).detach();
            }
            ~install() {
                for( hz = 10000, isGameRunning = 0; !isGameRunning ; );
            }
        } timer;

        hz = HZ > 0 ? HZ : hz;
        if(FPS) *FPS = fps;

        // we got too far ahead, cpu idle wait
        while( loop_counter > timer_counter && isGameRunning ) {
            std::this_thread::yield();
        }

        // max auto frameskip is 10, ie, even if speed is low paint at least one frame every 10
        if( timer_counter > loop_counter + 10 ) {
            timer_counter = loop_counter;
        }

        loop_counter++;

        // only draw if we are fast enough, otherwise skip the frame
        return( loop_counter >= timer_counter );
    }
}

namespace sand {
    unsigned fps_ = 0;
    unsigned get_fps() {
        return fps_;
    }
    bool lock( unsigned HZ ) {
        return lock_impl( HZ, &fps_ );
    }
}

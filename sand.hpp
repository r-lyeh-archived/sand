/* Sand is a lightweight and simple time framework written in C++11. zlib/libpng licensed.
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


#pragma once

#include <cstdint>
#include <ctime>

#include <deque>
#include <string>

#define SAND_VERSION "0.1.0" /* (2015/09/19) Moved framerate locker to a library apart
#define SAND_VERSION "0.0.0" // (2010/xx/xx) Initial version */

namespace sand
{
    // sleep for shortest lapse
    void wink();
    // sleep for given seconds at least
    void sleep( double seconds );

    // seconds since unix epoch (hires)
    double now();
    // seconds since start of program (hires)
    double uptime();
    // advance all clocks specified time (for QA and testing purposes)
    void lapse( double t );
    // void pause( bool on = false ) {}
    // void resume() { pause(false); }

    // conversion
    double nanoseconds( double t );
    double microseconds( double t );
    double milliseconds( double t );
    double seconds( double t );
    double minutes( double t );
    double hours( double t );
    double days( double t );
    double weeks( double t );
    double months( double t );
    double years( double t );
    double to_nanoseconds( double t );
    double to_microseconds( double t );
    double to_milliseconds( double t );
    double to_seconds( double t );
    double to_minutes( double t );
    double to_hours( double t );
    double to_days( double t );
    double to_weeks( double t );
    double to_months( double t );
    double to_years( double t );

    double calendar( const std::string &YMDhms );
    // double date( const std::string &YMD );
    // double time( const std::string &hms );

    // extraction
    int hour( double timestamp );
    int minute( double timestamp );
    int second( double timestamp );
    int day( double timestamp );
    int month( double timestamp );
    int year( double timestamp );

    // pretty printing
    std::string diff( double since );
    std::string diff( double since, double then );
    std::string pretty( double t );
    std::string ago( double t );
    std::string in( double t );

    // format
    // locale = "es-ES", "Chinese_China.936", "en_US.UTF8", etc...
    std::string format( double timestamp, const std::string &format, const std::string &locale = std::string() );
    std::string locale( double timestamp, const std::string &locale, const std::string &format = std::string() );

    // serialization
    std::string str( double timestamp );
    double str( const std::string &timestamp );

    // usage:
    // sand::dt dt;
    // [do something]
    // double seconds_taken = dt.s();
    class dt
    {
        double start;

        public:

        dt() : start( sand::now() )
        {}

        void reset() {
            start = sand::now();
        }

        double s() {
            return to_seconds( sand::now() - start );
        }
        double ms() {
            return to_milliseconds( sand::now() - start );
        }
        double us() {
            return to_microseconds( sand::now() - start );
        }
        double ns() {
            return to_nanoseconds( sand::now() - start );
        }
    };

    // once(); !!
    // usage:
    // sand::chrono ch(3.5);
    // ch.s() -> [0..1]
    class chrono
    {
        sand::dt dt;
        double top;

        public:

        explicit
        chrono( double _top = 1 ) : top(_top)
        {}

        double s() {
            if( top > 0 ) {
                double now = dt.s() / top;
                return now >= 1.0 ? 1.0 : now;
            }
            return 1.0;
        }

        void reset() {
            dt.reset();
        }

        void reset( double _top ) {
            top = _top;
            dt.reset();
        }
    };

    // every(); !!
    // usage:
    // sand::looper l(3.5); // loop every 3.5seconds
    // l.s() -> [0..1][...]
    class looper
    {
        sand::dt dt;
        float factor;

        public:

        explicit
        looper( const float seconds = 1.f ) : factor(1.f/seconds)
        {}

        void reset( float seconds = 1.f ) {
            factor = 1.f/seconds;
            dt.reset();
        }

        double s() {
            double now = dt.s() * factor;
            if( now < 1.0 ) return now;
            dt = sand::dt();
            return 1.0;
        }
    };
}

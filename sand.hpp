// Sand, a functional time controller (C++11). ZLIB/LibPNG licensed.
// - rlyeh ~~ listening to The Mission / Butterfly on a wheel

// @todo, max 64-bit
// 18446744073709551615
// 1TTTTYYMMDDhhmmssxxx TTTT = timezone, xxx = milliseconds
// 18446744073709551615
// 00YYMMDDhhmmssxxxxxx xxx xxx = microseconds

#pragma once
#include <stdint.h>
#include <ctime>
#include <deque>
#include <string>

#define SAND_VERSION "v2.0.0" /* (2015/09/26) Upgraded version - more portable, less error prone
#define SAND_VERSION "v1.0.0" // (2013/04/12) Initial version */

namespace sand
{
    using point = uint64_t; // absolute time in milliseconds since Unix Epoch (1970/01/01 00:00:00.000 +00:00)
    using lapse =  int64_t; // relative time in milliseconds between two timepoints; can be negative.
    using uint = unsigned int;

    // sleep thread for specified milliseconds (at least)
    void sleep( int64_t stamp );

    // UTC absolute time, GMT timezone, local time and uptime since program epoch (in milliseconds)
    int64_t utc();
    int64_t gmt();
    int64_t now();
    int64_t uptime();

    // advance/rewind all clocks specified milliseconds (useful for QA and testing purposes)
    void shift( int64_t lapse );

    // conversion to milliseconds
    int64_t nanoseconds( int64_t lapse );
    int64_t microseconds( int64_t lapse );
    int64_t milliseconds( int64_t lapse );
    int64_t seconds( int64_t lapse );
    int64_t minutes( int64_t lapse );
    int64_t hours( int64_t lapse );
    int64_t days( int64_t lapse );
    int64_t weeks( int64_t lapse );

    // conversion from milliseconds
    int64_t as_nanoseconds( int64_t lapse );
    int64_t as_microseconds( int64_t lapse );
    int64_t as_milliseconds( int64_t lapse );
    int64_t as_seconds( int64_t lapse );
    int64_t as_minutes( int64_t lapse );
    int64_t as_hours( int64_t lapse );
    int64_t as_days( int64_t lapse );
    int64_t as_weeks( int64_t lapse );

    // calendar
    int64_t date( int year, int month, int day );
    int64_t time( int hour, int minute, int second, int millis = 0 );
    int64_t datetime( int year, int month, int day, int hour, int minute, int second, int millis = 0 );

    // extraction
    int year( int64_t stamp );
    int month( int64_t stamp );
    int day( int64_t stamp );
    int hour( int64_t stamp );
    int minute( int64_t stamp );
    int second( int64_t stamp );
    int millisecond( int64_t stamp );

    // print
    std::string format( int64_t stamp, const std::string &format = "yyyy-mm-dd HH:MM:SS.MS" );
    std::string pretty( int64_t lapse );

    // serialization
    std::string str( int64_t stamp );
    int64_t str( const std::string &ymdhmsmtz );

    // usage:
    // sand::timer dt;
    // [do something]
    // int64_t nanoseconds_taken = dt.ns(); (relative time)
    class timer
    {
        int64_t start;

        public:

        timer() : start( sand::now() )
        {}

        void reset() {
            start = sand::now();
        }

        int64_t s() {
            return as_seconds( sand::now() - start );
        }
        int64_t ms() {
            return as_milliseconds( sand::now() - start );
        }
        int64_t us() {
            return as_microseconds( sand::now() - start );
        }
        int64_t ns() {
            return as_nanoseconds( sand::now() - start );
        }
    };

    // usage:
    // sand::chrono ch(3.5); // in seconds
    // ch.t() -> [0..1] (normalized floating time)
    class chrono
    {
        sand::timer dt;
        double top;

        public:

        explicit
        chrono( double _top = 1 ) : top(_top * 1000000000.0)
        {}

        double t() {
            if( top > 0 ) {
                double now = dt.ns() / top;
                return now >= 1.0 ? 1.0 : now;
            }
            return 1.0;
        }

        void reset() {
            dt.reset();
        }

        void reset( int64_t _top ) {
            top = _top;
            dt.reset();
        }
    };

    // usage:
    // sand::looper l(3.5); // in seconds (similar to sand::chrono but will loop over and over)
    // l.t() -> [0..1][...] (normalized floating time)
    class looper
    {
        sand::timer dt;
        double factor;

        public:

        explicit
        looper( const double seconds = 1.0 ) : factor(1.0/seconds)
        {}

        double t() {
            double now = (dt.ns() / 1000000000.0) * factor;
            if( now < 1.0 ) return now;
            dt = sand::timer();
            return 1.0;
        }

        void reset( double seconds = 1.0 ) {
            factor = 1.0/seconds;
            dt.reset();
        }
    };

    // @todo
    // every(s); // if( every(5.0) ) {}
    // once();   // if( once() ) {}
}



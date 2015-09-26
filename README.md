sand <a href="https://travis-ci.org/r-lyeh/sand"><img src="https://api.travis-ci.org/r-lyeh/sand.svg?branch=master" align="right" /></a>
====

Sand is a lightweight timer controller (C++11).

Features:
- [x] Wrapper around Unix stamps, hires timers and calendars.
- [x] Portable. Does not rely on standard C library or locales.
- [x] Cross-platform, stand-alone, no external dependencies only.
- [x] Tiny. One header and one source file.
- [x] ZLIB/LibPNG licensed.

### API - time format (64-bit base10)
```
18446744073709551615
00YYMMDDhhmmssxxxxxx xxx xxx = microseconds

proposal
18446744073709551615
1TTTTYYMMDDhhmmssxxx TTTT = timezone, xxx = milliseconds
```

### API
```c++
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

    class sand::timer dt;
    // usage:
    // - int64_t nanoseconds_taken = dt.ns();  // (relative time)
    // - int64_t seconds_taken = dt.s();       // (relative time)
    // - int64_t milliseconds_taken = dt.ms(); // (relative time)
    // - see also .reset();

    class sand::chrono chr(3.5); // in seconds
    // usage:
    // - chr.t() -> [0..1] (normalized floating time)
    // - see also .reset();

    class sand::looper lp(3.5); // in seconds (similar to sand::chrono but will loop over and over)
    // usage:
    // - lp.t() -> [0..1][...] (normalized floating time)
    // - see also .reset();
}
```

### Special notes
- g++ users: both `-std=c++11` and `-lrt` may be required when compiling `sand.cpp`

### Changelog
- v2.0.0 (2015/09/26)
  - A step forward towards portability and determinism.
- v0.1.0 (2015/09/19)
  - Moved framerate locker to a library apart
- v0.0.0 (2010/xx/xx)
  - Initial version

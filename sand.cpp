// Sand, a functional time controller (C++11). ZLIB/LibPNG licensed.
// - rlyeh ~~ listening to The Mission / Butterfly on a wheel

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

#include <thread>
#include <chrono>
#if !defined(SAND_USE_OMP) && ( defined(USE_OMP) || defined(_MSC_VER) /*|| defined(__ANDROID_API__)*/ )
#   define SAND_USE_OMP
#   include <omp.h>
#endif

#pragma warning(push)
#pragma warning(disable: 4996) // gmtime, localtime

/////////////////////////////////////////////////////////////////////////////

namespace sand
{
namespace
{
    int64_t clock() {
#   ifdef SAND_USE_OMP
        static auto const epoch = omp_get_wtime(); 
        return (int64_t)(( omp_get_wtime() - epoch ) * 1000);
#   else
        static auto const epoch = std::chrono::steady_clock::now(); // milli ms > micro us > nano ns
        return (int64_t)std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::steady_clock::now() - epoch ).count();
#   endif
    }

    std::string floor( double f ) {
        return std::to_string( int( ::floor(f) ) );
    }
    std::string ceil( double f ) {
        return std::to_string( int( ::ceil(f) ) );
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

    int64_t offset = 0;
}

    int64_t gmt() {
        time_t t = std::time(0);
        struct tm *gtm = std::gmtime(&t);
        int hour1 = gtm->tm_hour;
        struct tm *ltm = std::localtime(&t);
        int hour2 = ltm->tm_hour;
        return hours(hour2 - hour1);
    }

    int64_t utc() {
        static const int64_t app_epoch = seconds(std::time(NULL));
        return offset + app_epoch + sand::clock();
    }

    int64_t now() {
        static const int64_t gmt_epoch = gmt();
        return utc() + gmt_epoch;
    }

    int64_t uptime() {
        return offset + sand::clock();
    }

    void shift( int64_t t ) {
        offset += seconds(t);
    }

    struct timestamp_t {
        int millis; // 0-999
        int second; // 0-59
        int minute; // 0-59
        int hour;   // 0-23
        int day;    // 0-30
        int month;  // 0-11
        int year;   // 0-xx (representing 1900-2xxx)
    };
    enum : int {
        RTC_EPOCH_JULIAN_DAY = 2440588, // January 1st, 1970
    };

    timestamp_t epoch_to_timestamp( int64_t epoch ) {
        int64_t year, month, day, l, n;

        timestamp_t dt;
        dt.millis = epoch % 1000;
        epoch /= 1000;

        // Reference: Fliegel, H. F. and van Flandern, T. C. (1968).
        // Communications of the ACM, Vol. 11, No. 10 (October, 1968).
        l = epoch / 86400 + 68569 + RTC_EPOCH_JULIAN_DAY;
        n = 4 * l / 146097;
        l = l - (146097 * n + 3) / 4;
        year = 4000 * (l + 1) / 1461001;
        l = l - 1461 * year / 4 + 31;
        month = 80 * l / 2447;
        day = l - 2447 * month / 80;
        l = month / 11;
        month = month + 2 - 12 * l;
        year = 100 * (n - 49) + year + l;

        dt.day = (int)day - 1;
        dt.month = (int)month - 1;
        dt.year = (int)year - 1900;

        epoch = epoch % (24 * 3600);
        dt.hour = (int)epoch / 3600;

        epoch = epoch % 3600;
        dt.minute = (int)epoch / 60;
        dt.second = (int)epoch % 60;
        return dt;
    }

    int64_t date( int year, int month, int day ) {
        // Reference: Fliegel, H. F. and van Flandern, T. C. (1968).
        // Communications of the ACM, Vol. 11, No. 10 (October, 1968).
        return days( day - 32075 - RTC_EPOCH_JULIAN_DAY
            + 1461 * (year + 4800 + (month - 14) / 12) / 4
            + 367 * (month - 2 - 12 * ((month - 14) / 12)) / 12
            - 3 * ((year + 4900 + (month - 14) / 12) / 100) / 4 );
    }

    int64_t time( int hour, int minute, int second, int millis ) {
        return hours(hour) + minutes(minute) + seconds(second) + milliseconds(millis);
    }

    int64_t datetime( int year, int month, int day, int hour, int minute, int second, int millis ) {
        return date( year, month, day ) + time( hour, minute, second, millis );
    }

    namespace {
        // this function may look lame, but iostream is a hell and sprintf has portability issues
        // also, we only cover what we really need.
        std::string itoa(int x, int zerodigits = 0) {
            if( zerodigits == 4 ) {
            if( x >= 1000 ) return         std::to_string(x);
            if( x >=  100 ) return   "0" + std::to_string(x);
            if( x >=   10 ) return  "00" + std::to_string(x);
                            return "000" + std::to_string(x);
            }
            if( zerodigits == 3 ) {
            if( x >= 100 ) return        std::to_string(x);
            if( x >=  10 ) return  "0" + std::to_string(x);
                           return "00" + std::to_string(x);
            }
            if( zerodigits == 2 ) {
            if( x >=  10 ) return        std::to_string(x);
                           return  "0" + std::to_string(x);
            }
            return std::to_string(x);
        }
        std::string replace( std::string self, const std::string &target, const std::string &replacement ) {
            size_t found = 0;
            while( ( found = self.find( target, found ) ) != std::string::npos ) {
                self.replace( found, target.length(), replacement );
                found += replacement.length();
            }
            return self;
        }
    }

    std::string format( int64_t timestamp, const std::string &format ) {
        timestamp_t dt = epoch_to_timestamp( timestamp );

        const char *mo[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
        "Aug", "Sep", "Oct", "Nov", "Dec" };
        const char *MO[] = { "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December" };

        std::string timef = format;
        timef = replace(timef, "MS", itoa((int)dt.millis, 3));
        timef = replace(timef, "SS", itoa((int)dt.second, 2));
        timef = replace(timef, "MM", itoa((int)dt.minute, 2));
        timef = replace(timef, "HH", itoa((int)dt.hour, 2));
        timef = replace(timef, "yyyy", itoa((int)dt.year + 1900, 4));
        timef = replace(timef, "yy", itoa((int)dt.year / 100, 2));
        timef = replace(timef, "mmmm", "\1");
        timef = replace(timef, "mmm", "\2");
        timef = replace(timef, "mm", "\3");
        timef = replace(timef, "m", "\4");
        timef = replace(timef, "dd", "\5");
        timef = replace(timef, "d", "\6");
        timef = replace(timef, "\1", MO[dt.month]);
        timef = replace(timef, "\2", mo[dt.month]);
        timef = replace(timef, "\3", itoa((int)dt.month + 1, 2));
        timef = replace(timef, "\4", itoa((int)dt.month + 1, 1));
        timef = replace(timef, "\5", itoa((int)dt.day + 1, 2));
        timef = replace(timef, "\6", itoa((int)dt.day + 1, 1));

        /*
        %c - full datetime
        %x - full date
        %X - full time

        %Y - year YYYY (range 1900 to xxxx)
        %y - year YY (range 00 to 99)
        %C - The century number (year/100) as a 2-digit integer. (SU)

        %B - full month name 
        %b - short month name 
        %m - month MM (range 01 to 12)

        %A - full weekday name
        %a - short weekday name
        %d - day of the month DD (range 01 to 31).
        %e - day of the month D no leading zero. (range 1 to 31)
        %j - day of the year DDD (range 001 to 366).

        %V - The ISO 8601 week number (see NOTES) of the current year as a decimal number, range 01 to 53, where week 1 is the first week that has at least 4 days in the new year. See also %U and %W. (SU)

        %F - The date in %Y-%m-%d (the ISO 8601 date format). (C99)
        %r - The time in a.m. or p.m. notation %I:%M:%S %p. (SU)
        %R - The time in 24-hour notation (%H:%M). (SU)
        %T - The time in 24-hour notation (%H:%M:%S). (SU)

        %H - hour as a decimal number using a 24-hour clock (range 00 to 23).
        %H - hour as a decimal number using a 24-hour clock (range 0 to 23); no leading zero (TZ)
        %I - hour as a decimal number using a 12-hour clock (range 01 to 12).
        %I - hour as a decimal number using a 12-hour clock (range 1 to 12); no leading zero (TZ)

        %M - minute as a decimal number (range 00 to 59).

        %S - second as a decimal number (range 00 to 60). (The range is up to 60 to allow for occasional leap seconds.)
        %s - number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC). (TZ)

        %P - Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale. Noon is treated as "PM" and midnight as "AM".
        %p - Either "am" or "pm" according to the given time value, or the corresponding strings for the current locale. Noon is treated as "pm" and midnight as "am".

        %z - The +hhmm or -hhmm numeric timezone (that is, the hour and minute offset from UTC). (SU)
        %Z - The timezone or name or abbreviation.
        */

        return timef;
    }

    // serialization

    std::string str( int64_t t ) {
        return format( t, "yyyy-mm-dd HH:MM:SS.MS" );
    }

    int64_t str( const std::string &ymdhmsm ) {
        std::deque< custom > token = custom( ymdhmsm ).tokenize(" :/.TZ+-");

        if( token.size() < 6 )
            return 0;

        int year   = token[0].as<int>();
        int month  = token[1].as<int>();
        int day    = token[2].as<int>();
        int hour   = token[3].as<int>();
        int minute = token[4].as<int>();
        int second = token[5].as<int>();
        int millis = token.size() <= 6 ? 0 : token[6].as<int>();
        // timezone

        return date( year, month, day ) + time( hour, minute, second, millis );
    }

    void sleep( int64_t millisecs ) {
        std::chrono::milliseconds duration( (int)(millisecs) );
        std::this_thread::sleep_for( duration );
    }

    int64_t nanoseconds( int64_t t ) {
        return t / 1000000;
    }
    int64_t microseconds( int64_t t ) {
        return t / 1000;
    }
    int64_t milliseconds( int64_t t ) {
        return t;
    }
    int64_t seconds( int64_t t ) {
        return t * 1000;
    }
    int64_t minutes( int64_t t ) {
        return t * seconds(60);
    }
    int64_t hours( int64_t t ) {
        return t * minutes(60);
    }
    int64_t days( int64_t t ) {
        return t * hours(24);
    }
    int64_t weeks( int64_t t ) {
        return t * days(7);
    }

    int64_t as_nanoseconds( int64_t t ) {
        return t * 1000000;
    }
    int64_t as_microseconds( int64_t t ) {
        return t * 1000;
    }
    int64_t as_milliseconds( int64_t t ) {
        return t;
    }
    int64_t as_seconds( int64_t t ) {
        return t / 1000;
    }
    int64_t as_minutes( int64_t t ) {
        return t / seconds(60);
    }
    int64_t as_hours( int64_t t ) {
        return t / minutes(60);
    }
    int64_t as_days( int64_t t ) {
        return t / hours(24);
    }
    int64_t as_weeks( int64_t t ) {
        return t / days(7);
    }

    int year( int64_t t ) {
        return custom( format( t, "yyyy" ) ).as<int>();
    }
    int month( int64_t t ) {
        return custom( format( t, "m" ) ).as<int>();
    }
    int day( int64_t t ) {
        return custom( format( t, "d" ) ).as<int>();
    }
    int hour( int64_t t ) {
        return custom( format( t, "HH" ) ).as<int>();
    }
    int minute( int64_t t ) {
        return custom( format( t, "MM" ) ).as<int>();
    }
    int second( int64_t t ) {
        return custom( format( t, "SS" ) ).as<int>();
    }
    int millisecond( int64_t t ) {
        return custom( format( t, "MS" ) ).as<int>();
    }

    // pretty (deictic) human time
    std::string pretty( int64_t reltime_ms ) {
        // based on code by John Resig (jquery.com)
        int abs_diff = (int)std::abs(reltime_ms / 1000);
        int day_diff = (int)std::floor(abs_diff / 86400);

        if( reltime_ms < 0 ) {
            if( day_diff == 0 ) {
                if( abs_diff <=   0 ) return "right now";
                if( abs_diff <=   1 ) return "a second ago";
                if( abs_diff <   60 ) return sand::floor(abs_diff) + " seconds ago";
                if( abs_diff <  120 ) return "a minute ago";
                if( abs_diff < 3600 ) return sand::floor(abs_diff/60) + " minutes ago";
                if( abs_diff < 7200 ) return "an hour ago";
                return sand::floor(abs_diff/3600) + " hours ago";
            }
            if( day_diff ==  1 ) return "yesterday";
            if( day_diff <= 13 ) return std::to_string(day_diff) + " days ago";
            if( day_diff  < 31 ) return sand::ceil(day_diff/7) + " weeks ago";
            if( day_diff  < 62 ) return "a month ago";
            if( day_diff < 365 ) return sand::ceil(day_diff/31) + " months ago";
            if( day_diff < 730 ) return "a year ago";
            return sand::ceil(day_diff/365) + " years ago";
        } else {
            if( day_diff == 0 ) {
                if( abs_diff <=   0 ) return "right now";
                if( abs_diff <=   1 ) return "a second from now";
                if( abs_diff <   60 ) return sand::floor(abs_diff) + " seconds from now";
                if( abs_diff <  120 ) return "a minute from now";
                if( abs_diff < 3600 ) return sand::floor(abs_diff/60) + " minutes from now";
                if( abs_diff < 7200 ) return "an hour from now";
                return sand::floor(abs_diff/3600) + " hours from now";
            }
            if( day_diff ==  1 ) return "tomorrow";
            if( day_diff <= 13 ) return std::to_string(day_diff) + " days from now";
            if( day_diff  < 31 ) return sand::ceil(day_diff/7) + " weeks from now";
            if( day_diff  < 62 ) return "a month from now";
            if( day_diff < 365 ) return sand::ceil(day_diff/31) + " months from now";
            if( day_diff < 730 ) return "a year from now";
            return sand::ceil(day_diff/365) + " years from now";
        }
    }
}

#pragma warning(pop)


#pragma once

#include <cstdint>
#include <ctime>

#include <deque>
#include <string>

namespace sand
{
    // sleep for shortest lapse
    void wink();
    // sleep for given seconds at least
    void sleep( double seconds );

    // seconds since unix epoch (hires)
    double now();
    // seconds since start of program (hires)
    double runtime();
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
    std::string pretty( double t );
    std::string ago( double t );
    std::string in( double t );

    // format
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
            return ( sand::now() - start ) * 1.0;
        }
        double ms() {
            return ( sand::now() - start ) * 1000.0;
        }
        double us() {
            return ( sand::now() - start ) * 1000000.0;
        }
        double ns() {
            return ( sand::now() - start ) * 1000000000.0;
        }
    };

    //
    // double tick(__FILE__, __LINE__);
    // bool wait( double hz = 60.0 );

    class fps
    {
        public:

        fps();

        bool tick();
        void wait( double frames_per_second = 60.0 );

        std::deque< float > get_history() const;

        std::string str() const;
        size_t get() const;

        protected:

        sand::dt dt, frame_limiter, frame_timer;
        size_t frames, frames_per_second;
        std::string format;

        std::deque< float > history;
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

    // tweening
    #if 0

    // @todo
    float easein(t,tween) { return tween(t); }
    float easeout(t,tween) { return 1-tween(1-t); }
    float easeinout(t,tween) { return t < 0.5 ? tween(t*2) * 0.5 : easeout(t*2-1) * 0.5 + 0.5; }

    // @todo, from lQuery
    local function linear(t, b, c, d)
      return c * t / d + b
    end
    local function swing(t, b, c, d)
        return ((-cos(pi * t / d) / 2) + 0.5) * c + b
    end

    template <class T>
    void hyperbolicSmoothJoin(T &x, T xTarget, T speed)
    {
        x += (xTarget - x)*speed;
    }

    template <class T>
    void reverseHyperbolicSmoothJoin(T &x, T xTarget, T speed)
    {
    if(xTarget==x) return;
    if(xTarget>=x)
    {
       x += xTarget*xTarget*speed/(xTarget - x)/3.0f;
    if (x>xTarget) x=xTarget;
    }
    else
    {
       x += xTarget*xTarget*speed/(xTarget - x)/3.0f;
    if (x<xTarget) x=xTarget;
    }
    }

    template <class T>
    void linearSmoothJoin(T &x, T xTarget, T speed)
    {
    if(xTarget>x)
    {
    x += speed;
    if (x>xTarget) x=xTarget;
    }
    else
    {
    x -= speed;
    if (x<xTarget) x=xTarget;
    }
    }

    #endif

    // basics
    float ping( float dt01 );
    float pong( float dt01 );
    float pingpong( float dt01 );
    float sinus( float dt01 );

    // basics f(t) = t
    float linear( float dt01 );

    // penner's f(t) = t^2 ; memoized
    float quadin( float dt01 );
    float quadout( float dt01 );
    float quadinout( float dt01 );

    // penner's f(t) = t^3 ; memoized
    float cubicin( float dt01 );
    float cubicout( float dt01 );
    float cubicinout( float dt01 );

    // penner's f(t) = t^4 ; memoized
    float quartin( float dt01 );
    float quartout( float dt01 );
    float quartinout( float dt01 );

    // penner's f(t) = t^5 ; memoized
    float quintin( float dt01 );
    float quintout( float dt01 );
    float quintinout( float dt01 );

    // penner's f(t) = sin(t) ; memoized
    float sinein( float dt01 );
    float sineout( float dt01 );
    float sineinout( float dt01 );

    // penner's f(t) = 2^t ; memoized
    float expoin( float dt01 );
    float expoout( float dt01 );
    float expoinout( float dt01 );

    // penner's f(t) = sqrt(1-t^2) ; memoized
    float circin( float dt01 );
    float circout( float dt01 );
    float circinout( float dt01 );

    // penner's exponentially decaying sine wave ; memoized
    float elasticin( float dt01 );
    float elasticout( float dt01 );
    float elasticinout( float dt01 );

    // penner's overshooting cubic easing f(t) = (s+1)*t^3 - s*t^2 ; memoized
    float backin( float dt01 );
    float backout( float dt01 );
    float backinout( float dt01 );

    // penner's exponentially decaying parabolic bounce ; memoized
    float bouncein( float dt01 );
    float bounceout( float dt01 );
    float bounceinout( float dt01 );

    // gapjumper's ; memoized
    float sinesquare( float dt01 );
    float exponential( float dt01 );

    // terry schubring's ; memoized
    float terrys1( float dt01 );
    float terrys2( float dt01 );
    float terrys3( float dt01 );

    // tomas cepeda's ; memoized
    float acelbreak( float dt01 );
    float sinpi2( float dt01 );

    // generics ; always fresh, never memoized
    float tween( int type, float dt01 );
    const char *type( int type );

    enum TYPE
    {
        LINEAR_01,

        QUADIN_01,          // t^2
        QUADOUT_01,
        QUADINOUT_01,
        CUBICIN_01,         // t^3
        CUBICOUT_01,
        CUBICINOUT_01,
        QUARTIN_01,         // t^4
        QUARTOUT_01,
        QUARTINOUT_01,
        QUINTIN_01,         // t^5
        QUINTOUT_01,
        QUINTINOUT_01,
        SINEIN_01,          // sin(t)
        SINEOUT_01,
        SINEINOUT_01,
        EXPOIN_01,          // 2^t
        EXPOOUT_01,
        EXPOINOUT_01,
        CIRCIN_01,          // sqrt(1-t^2)
        CIRCOUT_01,
        CIRCINOUT_01,
        ELASTICIN_01,       // exponentially decaying sine wave
        ELASTICOUT_01,
        ELASTICINOUT_01,
        BACKIN_01,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
        BACKOUT_01,
        BACKINOUT_01,
        BOUNCEIN_01,        // exponentially decaying parabolic bounce
        BOUNCEOUT_01,
        BOUNCEINOUT_01,

        SINESQUARE,         // gapjumper's
        EXPONENTIAL,        // gapjumper's
        SCHUBRING1,         // terry schubring's formula 1
        SCHUBRING2,         // terry schubring's formula 2
        SCHUBRING3,         // terry schubring's formula 3

        SINPI2_01,          // tomas cepeda's
        ACELBREAK_01,

        TOTAL,
        UNDEFINED
    };
}

sand
====

- Sand is a lightweight and simple time framework written in C++11.
- Sand supports Unix stamps, hires timers, calendars, locales and tweening.
- Tiny. One header and one source file.
- Cross-platform. Builds on Windows/Linux/MacosX. Compiles on g++/clang/msvc.
- OS dependencies only. No third party dependencies.
- MIT licensed.

sync api
--------
- `wink()`
- `sleep(time)`


- `now()`
- `runtime()`
- `lapse(time)`

conversion api
--------------
- `nanoseconds(time)`
- `microseconds(time)`
- `milliseconds(time)`
- `seconds(time)`
- `minutes(time)`
- `hours(time)`
- `days(time)`
- `weeks(time)`
- `years(time)`
- `to_nanoseconds(time)`
- `to_microseconds(time)`
- `to_milliseconds(time)`
- `to_seconds(time)`
- `to_minutes(time)`
- `to_hours(time)`
- `to_days(time)`
- `to_weeks(time)`
- `to_years(time)`

extraction api
--------------
- `seconds(time)`
- `minutes(time)`
- `hours(time)`
- `days(time)`
- `weeks(time)`
- `years(time)`

pretty printing
---------------
- `ago(time)`
- `in(time)`

classes
-------
- `dt()`
- `rtc()`
- `fps()`
- `chrono()`
- `looper()`

tweening basics
---------------
- `ping(delta)`
- `pong(delta)`
- `pingpong(delta)`
- `sinus(delta)`
- `tween(type,delta)`

tweening extra
--------------
- `linear(delta)`
- `quadin(delta)`
- `quadout(delta)`
- `quadinout(delta)`
- `cubicin(delta)`
- `cubicout(delta)`
- `cubicinout(delta)`
- `quartin(delta)`
- `quartout(delta)`
- `quartinout(delta)`
- `quintin(delta)`
- `quintout(delta)`
- `quintinout(delta)`
- `sinein(delta)`
- `sineout(delta)`
- `sineinout(delta)`
- `expoin(delta)`
- `expoout(delta)`
- `expoinout(delta)`
- `circin(delta)`
- `circout(delta)`
- `circinout(delta)`
- `elasticin(delta)`
- `elasticout(delta)`
- `elasticinout(delta)`
- `backin(delta)`
- `backout(delta)`
- `backinout(delta)`
- `bouncein(delta)`
- `bounceout(delta)`
- `bounceinout(delta)`
- `sinesquare(delta)`
- `exponential(delta)`
- `terrys1(delta)`
- `terrys2(delta)`
- `terrys3(delta)`
- `acelbreak(delta)`
- `sinpi2(delta)`

sample
------
```
```

possible output
---------------
```
```

special notes
-------------
- g++ users: both `-std=c++11` and `-lrt` may be required when compiling `sole.cpp`

sand
====

- Sand is a lightweight and simple time library written in C++11.
- Sand is a wrapper around Unix stamps, hires timers, calendars, locales and tweening.
- Sand is tiny. One header and one source file.
- Sand is cross-platform. Builds on Windows/Linux/MacosX. Compiles on g++/clang/msvc.
- Sand is stand-alone. OS dependencies only. No third party dependencies.
- Sand is MIT licensed.

sync api
--------
- `wink()`: @todoc
- `sleep(time)`: @todoc
- `now()`: @todoc
- `runtime()`: @todoc
- `lapse(time)`: @todoc
- `lock(HZ)`: @todoc
- `get_fps()`: @todoc

conversion api
--------------
- `nanoseconds(time)`: @todoc
- `microseconds(time)`: @todoc
- `milliseconds(time)`: @todoc
- `seconds(time)`: @todoc
- `minutes(time)`: @todoc
- `hours(time)`: @todoc
- `days(time)`: @todoc
- `weeks(time)`: @todoc
- `years(time)`: @todoc

- `to_nanoseconds(time)`: @todoc
- `to_microseconds(time)`: @todoc
- `to_milliseconds(time)`: @todoc
- `to_seconds(time)`: @todoc
- `to_minutes(time)`: @todoc
- `to_hours(time)`: @todoc
- `to_days(time)`: @todoc
- `to_weeks(time)`: @todoc
- `to_years(time)`: @todoc

- `calendar(time)`: @todoc

extraction api
--------------
- `seconds(time)`: @todoc
- `minutes(time)`: @todoc
- `hours(time)`: @todoc
- `days(time)`: @todoc
- `weeks(time)`: @todoc
- `years(time)`: @todoc

pretty printing
---------------
- `pretty(time)`: @todoc
- `ago(time)`: @todoc
- `in(time)`: @todoc

format
------
- `format(time,string format[,string locale])`: @todoc
- `locale(time,string locale[,string format])`: @todoc

serialization
-------------
- `str(string)`: @todoc
- `str(time)`: @todoc

classes
-------
- `dt()`: @todoc
- `chrono()`: @todoc
- `looper()`: @todoc

tweening basics
---------------
- `ping(delta)`: @todoc
- `pong(delta)`: @todoc
- `pingpong(delta)`: @todoc
- `sinus(delta)`: @todoc
- `tween(type,delta)`: @todoc
- `type(type)`: @todoc

tweening extra
--------------
- `linear(delta)`: @todoc
- `quadin(delta)`: @todoc
- `quadout(delta)`: @todoc
- `quadinout(delta)`: @todoc
- `cubicin(delta)`: @todoc
- `cubicout(delta)`: @todoc
- `cubicinout(delta)`: @todoc
- `quartin(delta)`: @todoc
- `quartout(delta)`: @todoc
- `quartinout(delta)`: @todoc
- `quintin(delta)`: @todoc
- `quintout(delta)`: @todoc
- `quintinout(delta)`: @todoc
- `sinein(delta)`: @todoc
- `sineout(delta)`: @todoc
- `sineinout(delta)`: @todoc
- `expoin(delta)`: @todoc
- `expoout(delta)`: @todoc
- `expoinout(delta)`: @todoc
- `circin(delta)`: @todoc
- `circout(delta)`: @todoc
- `circinout(delta)`: @todoc
- `elasticin(delta)`: @todoc
- `elasticout(delta)`: @todoc
- `elasticinout(delta)`: @todoc
- `backin(delta)`: @todoc
- `backout(delta)`: @todoc
- `backinout(delta)`: @todoc
- `bouncein(delta)`: @todoc
- `bounceout(delta)`: @todoc
- `bounceinout(delta)`: @todoc
- `sinesquare(delta)`: @todoc
- `exponential(delta)`: @todoc
- `terrys1(delta)`: @todoc
- `terrys2(delta)`: @todoc
- `terrys3(delta)`: @todoc
- `acelbreak(delta)`: @todoc
- `sinpi2(delta)`: @todoc

sample
------
```c++
```

possible output
---------------
```
```

special notes
-------------
- g++ users: both `-std=c++11` and `-lrt` may be required when compiling `sand.cpp`

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
- `wink()`: @todocument
- `sleep(time)`: @todocument
- `now()`: @todocument
- `runtime()`: @todocument
- `lapse(time)`: @todocument

conversion api
--------------
- `nanoseconds(time)`: @todocument
- `microseconds(time)`: @todocument
- `milliseconds(time)`: @todocument
- `seconds(time)`: @todocument
- `minutes(time)`: @todocument
- `hours(time)`: @todocument
- `days(time)`: @todocument
- `weeks(time)`: @todocument
- `years(time)`: @todocument

- `to_nanoseconds(time)`: @todocument
- `to_microseconds(time)`: @todocument
- `to_milliseconds(time)`: @todocument
- `to_seconds(time)`: @todocument
- `to_minutes(time)`: @todocument
- `to_hours(time)`: @todocument
- `to_days(time)`: @todocument
- `to_weeks(time)`: @todocument
- `to_years(time)`: @todocument

- `calendar(time)`: @todocument

extraction api
--------------
- `seconds(time)`: @todocument
- `minutes(time)`: @todocument
- `hours(time)`: @todocument
- `days(time)`: @todocument
- `weeks(time)`: @todocument
- `years(time)`: @todocument

pretty printing
---------------
- `pretty(time)`: @todocument
- `ago(time)`: @todocument
- `in(time)`: @todocument

format
------
- `format(time,string format[,string locale])`: @todocument
- `locale(time,string locale[,string format])`: @todocument

serialization
-------------
- `str(string)`: @todocument
- `str(time)`: @todocument

classes
-------
- `dt()`: @todocument
- `fps()`: @todocument
- `chrono()`: @todocument
- `looper()`: @todocument

tweening basics
---------------
- `ping(delta)`: @todocument
- `pong(delta)`: @todocument
- `pingpong(delta)`: @todocument
- `sinus(delta)`: @todocument
- `tween(type,delta)`: @todocument
- `type(type)`: @todocument

tweening extra
--------------
- `linear(delta)`: @todocument
- `quadin(delta)`: @todocument
- `quadout(delta)`: @todocument
- `quadinout(delta)`: @todocument
- `cubicin(delta)`: @todocument
- `cubicout(delta)`: @todocument
- `cubicinout(delta)`: @todocument
- `quartin(delta)`: @todocument
- `quartout(delta)`: @todocument
- `quartinout(delta)`: @todocument
- `quintin(delta)`: @todocument
- `quintout(delta)`: @todocument
- `quintinout(delta)`: @todocument
- `sinein(delta)`: @todocument
- `sineout(delta)`: @todocument
- `sineinout(delta)`: @todocument
- `expoin(delta)`: @todocument
- `expoout(delta)`: @todocument
- `expoinout(delta)`: @todocument
- `circin(delta)`: @todocument
- `circout(delta)`: @todocument
- `circinout(delta)`: @todocument
- `elasticin(delta)`: @todocument
- `elasticout(delta)`: @todocument
- `elasticinout(delta)`: @todocument
- `backin(delta)`: @todocument
- `backout(delta)`: @todocument
- `backinout(delta)`: @todocument
- `bouncein(delta)`: @todocument
- `bounceout(delta)`: @todocument
- `bounceinout(delta)`: @todocument
- `sinesquare(delta)`: @todocument
- `exponential(delta)`: @todocument
- `terrys1(delta)`: @todocument
- `terrys2(delta)`: @todocument
- `terrys3(delta)`: @todocument
- `acelbreak(delta)`: @todocument
- `sinpi2(delta)`: @todocument

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
- g++ users: both `-std=c++11` and `-lrt` may be required when compiling `sand.cpp`

#pragma once
#include <cstdint>
#include <functional>
#include <chrono>
#include <string>
#include <cassert>
#include <thread>

using namespace std::chrono_literals; //using chrono literial by default
using hires_clock = std::chrono::high_resolution_clock;
using sys_clock = std::chrono::system_clock;

class noncopyable
{
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

//TODO: nothrow?
class ScopeGuard : public noncopyable {
public:
    using callback_type = std::function<void()>;
    ScopeGuard(const callback_type& callback) : callback_(std::move(callback))
    {}

    ScopeGuard(callback_type&& callback) : callback_(std::move(callback))
    {}

    ~ScopeGuard() {
        if (callback_)
            callback_();
    }
private:
    callback_type callback_;
};

class StopWatch :public noncopyable {
public:

    StopWatch()
    {
        reset();
    }

    void start() {
        reset();
    }

    void reset() {
        begin_ = hires_clock::now();
    }

    template<typename time_unit>
    time_unit elapsed() {
        auto now = hires_clock::now();
        return std::chrono::duration_cast<time_unit>(now - begin_);
    }

    std::chrono::seconds::rep elapsed_seconds()
    {
        return elapsed<std::chrono::seconds>().count();
    }

    std::chrono::milliseconds::rep elapsed_milliseconds()
    {
        return elapsed<std::chrono::milliseconds>().count();
    }

    std::chrono::nanoseconds::rep elapsed_nanoseconds()
    {
        return elapsed<std::chrono::nanoseconds>().count();
    }
private:
    hires_clock::time_point begin_;
};

class FrameRate :public noncopyable {
public:

    FrameRate() : frame_rate_(.0f), frame_counter_(0), frame_count_(0), time_stamp_(0ns)
    {

    }

    void reset() {
        frame_rate_ = .0f;
        frame_count_ = 0;
        frame_counter_ = 0;

        time_stamp_ = hires_clock::now();
    }

    void update() {
        auto now = hires_clock::now();
        auto frame_time = now - time_stamp_;

        if (frame_time > std::chrono::seconds{ 1 })//refresh rate -> 1s
        {
            frame_count_ = frame_counter_;
            frame_rate_ = 1.0f / frame_count_;
            frame_counter_ = 0;
            time_stamp_ = hires_clock::now();
        }

        ++frame_counter_;
    }

    int get_tick_count()
    {
        return frame_count_;
    }

    float get() {
        return frame_rate_;
    }
private:
    float frame_rate_;
    int frame_counter_;
    int frame_count_;
    hires_clock::time_point time_stamp_;
};

template<size_t FPS>
class FrameTimeLimit {
    using fps_unit_t = std::chrono::duration<hires_clock::rep, std::ratio<1, FPS>>;

public:
    FrameTimeLimit() : time_stamp_()
        , desired_frame_rate_{ std::chrono::duration_cast<hires_clock::duration>(fps_unit_t{ 1 }) }
    {
    }

    void update() {
        auto now = hires_clock::now();
        auto frame_time = now - time_stamp_;

        while (frame_time < desired_frame_rate_)
        {
            //auto diff = desired_frame_rate_ - frame_time;

            now = hires_clock::now();
            frame_time = now - time_stamp_;

            std::this_thread::sleep_for(1ns);
        }

        time_stamp_ = now;
    }

private:
    hires_clock::time_point time_stamp_;
    const hires_clock::duration desired_frame_rate_; //eg. target fps: 60 

};



template<typename... Ts>
void unpack(Ts&&...) {}//utility function to unpack variadic template arguments

//it could be simpler...
template<size_t N, size_t... M>
struct sum_of
{
    static constexpr size_t value = N +  sum_of<M...>::value;
};

template<size_t N>
struct sum_of<N>
{
    static constexpr size_t value = N;
};

//special function
//calculate pre-n elements of tuple
//eg: partial_sum<3>::value(make_tuple(1,2,3,4,5)) -> 0 + 1 + 2
template<size_t Take>
struct partial_sum
{
    //static constexpr size_t value = N + sum_of<M...>::value;
    template<typename Tuple>
    static constexpr size_t value(Tuple &&tup)
    {
        return std::get<Take>(tup) + partial_sum<Take - 1>::value(std::forward<Tuple&&>(tup));
    }
};

template<>
struct partial_sum<0>
{
    template<typename Tuple>
    static constexpr size_t value(Tuple&&)
    {
        return 0;
    }
};

template<typename T>
constexpr bool is_power_of_two(T x)
{
    return ((x != 0) && !(x & (x - 1)));
}


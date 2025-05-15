#pragma once

#include <functional>
#include <ranges>

namespace game
{

template <class T, T Invalid = {}>
class AutoRelease
{
  public:
    AutoRelease()
        : AutoRelease(Invalid, nullptr)
    {
    }

    AutoRelease(T obj, std::function<void(T)> deleter)
        : obj_(obj)
        , deleter_(deleter)
    {
    }

    ~AutoRelease()
    {
        if (obj_ != Invalid)
        {
            deleter_(obj_);
        }
    }

    AutoRelease(const AutoRelease&) = delete;
    AutoRelease& operator=(const AutoRelease&) = delete;

    AutoRelease(AutoRelease&& other)
        : AutoRelease()
    {
        swap(other);
    }

    AutoRelease& operator=(AutoRelease&& other)
    {
        AutoRelease new_obj(std::move(other));
        swap(new_obj);

        return *this;
    }

    void swap(AutoRelease& other) noexcept
    {
        std::ranges::swap(obj_, other.obj_);
        std::ranges::swap(deleter_, other.deleter_);
    }

    T get() const
    {
        return obj_;
    }
    operator T() const
    {
        return obj_;
    }

    explicit operator bool() const
    {
        return obj_ != Invalid;
    }

  private:
    T obj_;
    std::function<void(T)> deleter_;
};

}

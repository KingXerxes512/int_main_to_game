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
        : m_Obj(obj)
        , m_Deleter(deleter)
    {
    }

    ~AutoRelease()
    {
        if (m_Obj != Invalid)
        {
            m_Deleter(m_Obj);
        }
    }

    AutoRelease(const AutoRelease&) = delete;
    AutoRelease& operator=(const AutoRelease&) = delete;

    AutoRelease(AutoRelease&& other) noexcept
        : AutoRelease()
    {
        swap(other);
    }

    AutoRelease& operator=(AutoRelease&& other) noexcept
    {
        AutoRelease new_obj(std::move(other));
        swap(new_obj);

        return *this;
    }

    void swap(AutoRelease& other) noexcept
    {
        std::ranges::swap(m_Obj, other.m_Obj);
        std::ranges::swap(m_Deleter, other.m_Deleter);
    }

    T get() const
    {
        return m_Obj;
    }
    operator T() const
    {
        return m_Obj;
    }

    explicit operator bool() const
    {
        return m_Obj != Invalid;
    }

    T* operator&() noexcept
    {
        return std::addressof(m_Obj);
    }

  private:
    T m_Obj;
    std::function<void(T)> m_Deleter;
};

}

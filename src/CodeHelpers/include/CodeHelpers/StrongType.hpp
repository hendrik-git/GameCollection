#pragma once
#include <type_traits>

namespace CodeHelper
{
	template<class Tag, typename T>
	class Alias
	{
	  public:
		Alias() : value_() {}

		explicit Alias(const T& value) : value_(value) {}

		explicit Alias(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
			: value_(std::move(value))
		{
		}

		operator T&() noexcept
		{
			return value_;
		}

		operator const T&() const noexcept
		{
			return value_;
		}

		friend void swap(Alias& a, Alias& b) noexcept
		{
			using std::swap;
			swap(static_cast<T&>(a), static_cast<T&>(b));
		}

	  private:
		T value_;
	};


	template<class Tag, typename T>
	class StrongType
	{
	  public:
		StrongType() : value_() {}

		explicit StrongType(const T& value) : value_(value) {}

		explicit StrongType(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
			: value_(std::move(value))
		{
		}

		explicit operator T&() noexcept
		{
			return value_;
		}

		explicit operator const T&() const noexcept
		{
			return value_;
		}

		friend void swap(StrongType& a, StrongType& b) noexcept
		{
			using std::swap;
			swap(static_cast<T&>(a), static_cast<T&>(b));
		}

	  private:
		T value_;
	};

}  // namespace CodeHelper
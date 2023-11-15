/// @file Score.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2023.11.14
/// @author Hendrik Poettker

#pragma once

namespace GameSystems
{
	inline namespace Events
	{
		struct score_changed
		{
			int value = 0;
		};

	}  // namespace Events


	class Score
	{
	  public:
		void on_score_changed(const score_changed& data) noexcept
		{
			score_ += data.value;
		}

		void reset()
		{
			score_ = 0;
		};

		auto get() const noexcept -> int
		{
			return score_;
		}

	  private:
		int score_ = 0;
	};

}  // namespace GameSystems
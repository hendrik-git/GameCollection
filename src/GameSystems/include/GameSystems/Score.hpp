/// @file Score.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2023.11.14
/// @author Hendrik Poettker

#pragma once
#include <filesystem>
#include <string>

namespace GameSystems
{
	namespace fs = std::filesystem;

	class Score
	{
	  public:
		Score(const std::string& id, const fs::path file = "scores.toml");

		~Score();

		void on_score_changed(const int value) noexcept;

		void reset()
		{
			score_	  = 0;
			new_high_ = false;
		};

		auto get() const noexcept -> int
		{
			return score_;
		}

	  private:
		std::string id_;
		fs::path	score_file_;
		int			highscore_ = 0;
		int			score_	   = 0;
		bool		new_high_  = false;
	};

}  // namespace GameSystems
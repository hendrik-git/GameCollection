#include <CodeHelpers/Logger.hpp>
#include <GameSystems/Score.hpp>
#include <algorithm>
#include <fstream>
#include <tomlplusplus/toml.hpp>

namespace GameSystems
{
	Score::Score(const std::string& id, const fs::path file) : id_(id), score_file_(file)
	{
		using namespace CodeHelper;
		try
		{
			if(fs::exists(file))
			{
				LOG_INFO(dl, "Reading highscore for '{}' from '{}'", id_, score_file_.string());
				highscore_ = toml::parse_file(file.c_str())[id_].value_or(0);
				LOG_INFO(dl, "Highscore for '{}' is '{}'", id_, highscore_);
			}
			else
			{
				LOG_WARNING(
					dl, "Creating new highscore file '{}' for '{}'", score_file_.string(), id_);
				auto		  tbl = toml::table{{id, highscore_}};
				std::ofstream ofile(file.string(), std::ios::binary);
				ofile << "# Highscores for each game in the collection\n\n" << tbl;
			}
		}
		catch(const std::exception& e)
		{
			LOG_ERROR(dl, "{}", e.what());
		}
	}

	Score::~Score()
	{
		try
		{
			if(!new_high_)
			{
				return;
			}
			LOG_INFO(CodeHelper::dl, "Saving new highscore '{}' for '{}'", highscore_, id_);
			auto score = toml::parse_file(score_file_.c_str());
			score.insert_or_assign(id_, highscore_);
			std::ofstream ofile(score_file_.string(), std::ios::binary);
			ofile << "# Highscores for each game in the collection\n\n" << score;
		}
		catch(const std::exception& e)
		{
			LOG_ERROR(CodeHelper::dl, "{}", e.what());
		}
	}

	void Score::on_score_changed(const int value) noexcept
	{
		score_ += value;
		if(score_ > highscore_)
		{
			highscore_ = score_;
			new_high_  = true;
		}
	}
}  // namespace GameSystems
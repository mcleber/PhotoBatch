#include "Mode.h"
#include "Utils.h"

#include "ModeFactory.h"

#include <iostream>
#include <filesystem>
#include <chrono>

Mode::Mode(const std::string& filter, const std::string& folder)
	: m_Filter{ filter }
    , m_Folder{ folder }
{
}

const std::string& Mode::GetFilter() const
{
	return m_Filter;
}

const std::string& Mode::GetFolder() const
{
	return m_Folder;
}

void Mode::Run()
{
	using ClockT = std::chrono::high_resolution_clock;
	
	ClockT::time_point starTime = ClockT::now();
	RunImpl();
	ClockT::time_point endTime = ClockT::now();

	ClockT::duration elapsedTime = endTime - starTime;
	std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

	std::cout << GetModeName() << "Operação finalizda em " << elapsedTimeMs.count() << "ms" << std::endl;

}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const
{
	std::vector<std::filesystem::path> files;
	int numSkippedFiles = { 0 };

	// Coletar todos os arquivos que correspondem ao filtro especificado.
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder()))
	{
		const bool bIsFile = std::filesystem::is_regular_file(entry.path());
		const bool bMatchFilter = GetFilter().empty() || (entry.path().string().find(GetFilter()) != std::string::npos);
		const bool bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter && bMatchExtension)
		{
			files.push_back(entry.path());
		}
		else
		{
			numSkippedFiles++;
		}
	}

	std::cout << GetModeName() << "Número de arquivos encontrados: " << files.size() << std::endl;
	std::cout << GetModeName() << "Número de arquivos ignorados  : " << numSkippedFiles << std::endl;

	return files;
}

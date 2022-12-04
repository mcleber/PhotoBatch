#include "ModeFactory.h"
#include "ArgumentParser.h"
#include "Utils.h"

#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"

#include <iostream>
#include <array>

std::unique_ptr<Mode>CreateModeFactory(const ArgumentParser& argParser)
{
	// Ler as flags que o ArgumentParser identificou
	const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
	const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
	const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);

	// Validar os Modos
	const std::array<bool, 4> modes = { bRenameMode, bConvertMode, bResizeMode, bScaleMode };
	const std::ptrdiff_t numActiveModes = std::count(std::begin(modes), std::end(modes), true);

	// Verifica se somente um modo do PhotoBatch está ativo.
	if (numActiveModes != 1)
	{
		// Se houver mais de um modo ativo, lança uma excessão
		throw std::invalid_argument("Apenas um modo pode estar ativo.");
	}

	// Validar o diretório passado como a opção Folder
	const std::string folder = argParser.GetOptionAs<std::string>(Args::Opts::Folder);
	if (folder.empty())
	{
		throw std::invalid_argument("O diretório não pode ficar em branco.");
	}

	if (!std::filesystem::exists(folder))
	{
		throw std::invalid_argument("O diretório especificado não existe.");
	}

	// Validar se o Filtro é uma string válida
	const std::string filter = argParser.GetOptionAs<std::string>(Args::Opts::Filter);
	if (!filter.empty() && Utils::HasInvalidChars(filter))
	{
		throw std::invalid_argument("O Filter não pode conter " + Utils::GetInvalidChars());
	}

	// Validar o modo Resize
	if (bResizeMode)
	{
		int width{ 0 };
		int height{ 0 };

		try
		{
			width = argParser.GetOptionAs<int>(Args::Opts::Width);
			height = argParser.GetOptionAs<int>(Args::Opts::Height);
		}
		catch (const std::invalid_argument& exception)
		{
			throw std::invalid_argument("O valor inserido para Width ou Height não são números válidos.");
		}

		// No modo resize o comprimento e a altura devem ser maiores que 0
		if (width <= 0 || height <= 0)
		{
			throw std::invalid_argument("Width e Height devem ser maiores que 0.");
		}

		if (filter.empty())
		{
			throw std::invalid_argument("O Filter não pode ficar em branco no modo Resize.");
		}

		return std::make_unique<ResizeMode>(filter, folder, width, height);
	}

	// Validar o modo Scale
	if (bScaleMode)
	{
		float amount{ 0.0f };

		try
		{
			amount = argParser.GetOptionAs<float>(Args::Opts::Amount);
		}
		catch (const std::invalid_argument& exception)
		{
			throw std::invalid_argument("O valor inserido para Amount não é um número válido.");
		}

		// No ScaleMode o amount deve ser maior que 0
		if (amount <= 0.0f)
		{
			throw std::invalid_argument("Amount deve ser maior que zero.");
		}

		if (filter.empty())
		{
			throw std::invalid_argument("O Filter não pode ficar em branco no modo Escala.");
		}

		return std::make_unique<ScaleMode>(filter, folder, amount);
	}

	// Validar o modo Rename
	if (bRenameMode)
	{
		int startNumber = { -1 };

		try
		{
			startNumber = argParser.GetOptionAs<int>(Args::Opts::StartNumber);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor inserido para StartNumber não é um número válido.");
		}

		std::string prefix = argParser.GetOptionAs<std::string>(Args::Opts::Prefix);

		if (startNumber < 0)
		{
			throw std::invalid_argument("StartNumber deve ser maior ou igual a zero.");
		}

		if (prefix.empty() || Utils::HasInvalidChars(prefix))
		{
			throw std::invalid_argument("O Prefixo não pode ficar em branco e não pode conter os caracteres. " + Utils::GetInvalidChars());
		}

		return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
	}

	// Validar o modo Convert
	if (bConvertMode)
	{
		const std::string from = argParser.GetOptionAs<std::string>(Args::Opts::From);
		const std::string to = argParser.GetOptionAs<std::string>(Args::Opts::To);
		const std::array<std::string, 3> convertOptions = { "jpg", "png", "tga" };

		const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
		const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);

		if (!bIsFromValid || !bIsToValid)
		{
			throw std::invalid_argument("From e To devem ser png, jpg ou tga.");
		}

		if (from == to)
		{
			throw std::invalid_argument("From e To devem ser diferentes.");
		}

		const std::map<std::string, ConvertMode::Format> convertOptionsMap = {
			{"jpg", ConvertMode::Format::JPG},
			{"png", ConvertMode::Format::PNG},
			{"tga", ConvertMode::Format::TGA}

		};

		return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
	}

	return nullptr;
}

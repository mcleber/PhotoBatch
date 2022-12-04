#include "ArgumentParser.h"
#include "Utils.h"

#include <iostream>

void ArgumentParser::RegisterFlag(const std::string& flag)
{
	if (!flag.empty() && !Utils::HasWhiteSpaces(flag))
	{
		m_Flags[flag] = false;
	}
}

void ArgumentParser::RegisterOption(const std::string& option)
{
	if (!option.empty() && !Utils::HasWhiteSpaces(option))
	{
		m_Options[option] = "";
	}
}


bool ArgumentParser::GetFlag(const std::string& flag) const
{
	if (!flag.empty())
	{
		auto flagIt = m_Flags.find(flag); //flag iterator
		if (flagIt != std::end(m_Flags))
		{
			return flagIt->second;
		}
	}

	return false;
}

const std::string& ArgumentParser::GetOption(const std::string& option) const
{
	if (!option.empty())
	{
		auto optionIt = m_Options.find(option);
		if (optionIt != std::end(m_Options))
		{
			return optionIt->second;
		}
	}

	static std::string EmptyOption = "";
	return EmptyOption;
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const
{
	const std::string& optionValue = GetOption(option);

	if (!optionValue.empty())
	{
		return std::stof(optionValue); //string to float
	}

	return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const
{
	const std::string& optionValue = GetOption(option);

	if (!optionValue.empty())
	{
		return std::stoi(optionValue); //string to int
	}

	return -1;
}

void ArgumentParser::SetHelpMessage(const std::string& helpMessage)
{
	m_HelpMessage = helpMessage;
}

const std::string& ArgumentParser::GetHelpMessage() const
{
	return m_HelpMessage;
}


void ArgumentParser::Parse(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			// Converte os arqumentos para string
			std::string arg = Utils::ToLower(argv[i]);

			if (arg.length() >= 3)
			{
				// Argumentos devem ter pelo menos 3 caracteres
				if (arg[0] == '-' && arg[1] == '-')
				{
					// Remover os "--" iniciais pois já foram validados
					arg = arg.substr(2);

					// Depois do arg.substr: "rename"
					if (arg.find_first_of('=') != std::string::npos)
					{
						const size_t equalSignPos = arg.find('=');
						if (equalSignPos != std::string::npos)
						{
							// Dividir a opção em chave e valor
							std::string optionName = arg.substr(0, equalSignPos);
							std::string optionValue = arg.substr(equalSignPos + 1);

							auto optionIt = m_Options.find(optionName);
							if (optionIt != std::end(m_Options))
							{
								// Opção registrada
								optionIt->second = optionValue;
							}
						}
					}
					else
					{
						// Verificar se a flag existe no map
						auto flagIt = m_Flags.find(arg);
						if (flagIt != std::end(m_Flags))
						{
							// Flag registrada
							flagIt->second = true;
						}
					}
				}
			}
		}
	}
}

#pragma once

#include <string>
#include <map>

class ArgumentParser
{
public:
	// Registra uma flag usada durante o Parse
	void RegisterFlag(const std::string& flag);

	// Registra uma opção usada durante o Parse
	void RegisterOption(const std::string& option);

	// GoogleTest: Retorna se a flag está ou não registrada
	//bool IsFlagRegistered(const std::string& flag) const;
	
	// GoogleTest: Retorna se a opção está ou não registrada
	//bool IsOptionRegistered(const std::string& option) const;

	// Obtem o valor de uma flag obtida durante o Parse
	bool GetFlag(const std::string& flag) const;

	template<typename T>
	T GetOptionAs(const std::string& option) const;

	template<>
	std::string GetOptionAs(const std::string& option) const { return GetOption(option); }

	template<>
	float GetOptionAs(const std::string& option) const { return GetOptionAsFloat(option); }

	template<>
	int GetOptionAs(const std::string& option) const { return GetOptionAsInt(option); }

	// Faz o Parse da lista de argumentos em argv
	void Parse(int argc, char* argv[]);

	void SetHelpMessage(const std::string& helpMessage);

	const std::string& GetHelpMessage() const;
	

private:
	// Obtem o valor de uma opção lida durante o Parse
	const std::string& GetOption(const std::string& option) const;

	// Obtem o valor de uma opção convertida para float
	float GetOptionAsFloat(const std::string& option) const;

	// Obtem o valor de uma opção convertida para int
	int GetOptionAsInt(const std::string& option) const;

	// map mapeia chaves em valores.
	std::map<std::string, bool> m_Flags;
	std::map<std::string, std::string> m_Options;
	std::string m_HelpMessage;

};

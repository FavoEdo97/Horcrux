#pragma once
#include "stdafx.h"
#include "Horcrux.h"



CHorcrux::CHorcrux() {
	m_strPathInput = "";
	m_strPathOutput = "";
	m_strFilename = "";
	m_strKeyBase64 = "";
	m_iNumHorcruxs = 0;
	m_bInitialized = false;
	m_bDec = false;
}


void CHorcrux::Init(std::string strPathInput, std::string strPathOutput, const int& iHorcruxs, const int& keyLength)
{
	if (iHorcruxs <= 0)
	{
		std::cout << "Numero di horcrux deve essere >0";
		return;
	}
	if (keyLength < 16)
	{
		std::cout << "La len della chiave deve essere >=16";
		return;
	}
	m_strPathInput = strPathInput;
	m_iNumHorcruxs = iHorcruxs;
	SetOutputPath(strPathOutput);
	if (!std::filesystem::exists(strPathInput) || m_strPathOutput.empty())
	{
		std::cout << "Path di input o output non valido";
		return;
	}
		
	
	std::uintmax_t uiFileSize = std::filesystem::file_size(strPathInput);
	if (uiFileSize <= 0)
	{
		std::cout << "File size <=0" << std::endl;
		return;
	}
	if (uiFileSize <= iHorcruxs)
	{
		std::cout << "Il numero di horcruxs che vuoi creare e' troppo grande rispetto alla dimensione del file in input, dimensioone file:" << uiFileSize << " n horcruxs: " << iHorcruxs << std::endl;
		return;
	}
	m_acbuffInput.resize(uiFileSize);
	m_acKey.resize(keyLength);

	RAND_bytes(&m_acKey.front(),keyLength);
	m_strKeyBase64 = Utils::base64_encode(&m_acKey.front(), keyLength);
	//stampo la chiave per l'utente
	std::cout<<"Key : "<< m_strKeyBase64.c_str()<<std::endl;

	m_bInitialized = initCypher();
	return;
	
}
void CHorcrux::Init(std::string strPathOutput, std::vector<std::string> acPathsHorcruxs, std::string strKeyDec)
{
	if (acPathsHorcruxs.size() <= 0)
	{
		std::cout << "inserire almeno un horcrux path" << std::endl;
		return;
	}
	if (strKeyDec.size() < 44)
	{
		std::cout << "len della chiave troppo corta" << std::endl;
		return;
	}

	m_bDec = true;
	if (SetOutputPath(strPathOutput))
		m_strPathOutput = strPathOutput;
	else
	{
		std::cout << "Path di output non valido: " << strPathOutput << std::endl;
		return;

	}
	std::uintmax_t uiFileSize = 0;
	for (auto el : acPathsHorcruxs)
	{
		uiFileSize += std::filesystem::file_size(el);
	}
	if (uiFileSize <= 0)
		return;
	//ridimensiono i buffer considerando (ricordare che nel file cifrato c'è il padding)
	m_acbuffInput.resize(uiFileSize);
	m_acbuffOutput.resize(uiFileSize);
	m_acPathsHorcruxs = std::move(acPathsHorcruxs);
	std::string strKey = Utils::base64_decode(strKeyDec);

	if (strKey.size() <= 0)
		return;
	m_acKey.resize(strKey.size());

	memcpy(&m_acKey.front(), &strKey[0], strKey.size());
	m_bInitialized = initCypher();
	return;
	
}

bool CHorcrux::Save() {
	if (!IsInitialized() || !IsEncryptModeEnabled())
	{
		std::cout << "L'oggetto non è inizializzato oppure non è in modalità di cifratura" << std::endl;
		return false;

	}
		
	EncryptFileBase();
	if (CreateHorcruxs())
		SaveHorcruxs();
	else
	{
		std::cout << "Non sono riuscito a creare gli horcruxs" << std::endl;
		return false;
	}
	
	return true;
}

void CHorcrux::Load() {
	if (!IsInitialized() || IsEncryptModeEnabled())
	{
		std::cout << "L'oggetto non è inizializzato oppure non è in modalità di decifratura" << std::endl;
		return;

	}
	ReadHorcruxs();
	Decrypt();
	SavePlainText();
}

void CHorcrux::EncryptFileBase()
{
	Utils::ReadFromFile(m_strPathInput, &m_acbuffInput.front());
	Encrypt();
}

void CHorcrux::ReadHorcruxs()
{
	size_t bytesLetti =0 ;
	for (auto el : m_acPathsHorcruxs)
	{
		Utils::ReadFromFile(el, &m_acbuffInput[bytesLetti]);
		bytesLetti+=std::filesystem::file_size(el);
	}

}

bool CHorcrux::SetOutputPath(const std::string& strPath)
{
	if (std::filesystem::is_directory(strPath))
	{
		std::cout << "il path di output e' una directory, inserire anche il nome del file da creare" << std::endl;
		return false;
	}
		
	size_t iLogPath = 0;
	std::string strTmp;
	if (ExistDirPath(strPath, iLogPath, strTmp)) {
		m_strFilename = strPath.substr(iLogPath + 1, strPath.size());
		if (m_bDec && !m_strFilename.empty())
			return true;
		m_strPathOutput = strTmp;
		std::string::size_type pos = m_strFilename.find('.');
		if (m_strFilename.npos != pos)
			m_strFilename = m_strFilename.substr(0, pos);
		if (m_strFilename.empty())
		{

			m_strFilename = "Horcruxs";
		}

	}
	else
		return false;
	return true;
}

bool CHorcrux::ExistDirPath(const std::string& strPath, size_t & iLogPath, std::string & strTmp)
{
	iLogPath = strPath.find_last_of("\\");
	if (iLogPath == 0)
		iLogPath = strPath.find_last_of("/");//verificare per linux
	if (iLogPath == 0)
		iLogPath = strPath.find_last_of("//");//verificare per linux

	if (iLogPath == 0)
		return false;
	strTmp = strPath.substr(0, iLogPath + 1);

	if (!std::filesystem::exists(strTmp)) {
		return false;
	}
	return true;
}
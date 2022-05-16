#include "../HorcruxLib/stdafx.h"
#include <ctime>
#include<gtest/gtest.h>

class HorcruxLibTest : public::testing::Test {
public:
	static void GetPathTemp(std::string& InputOutputDir, std::string& strPlainInput)
	{
		InputOutputDir = std::filesystem::temp_directory_path().u8string();
		EXPECT_TRUE(!InputOutputDir.empty());
		strPlainInput = InputOutputDir + "PlainText.txt";
		return;

	}


	static std::string gen_random(const int len) {
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		std::string tmp_s;
		tmp_s.reserve(len);

		for (int i = 0; i < len; ++i) {
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
			if (i % 40 == 0 && i != 0) {
				tmp_s += "\n";
			}

		}

		return tmp_s;
	}

protected:

	void SetUp() override {
		//NEL METODO DI SETUP CREO UN FILE CASUALE 
		GetPathTemp(m_strInputOutputDir, m_strPlainInput);

		srand((unsigned)time(NULL) * getpid());
		int max = 1000000;
		int min = 500000;
		int randNum = rand() % (max - min + 1) + min;
		std::string strBuffer= gen_random(randNum);
		std::cout << "Scrivo un file random nel path:" << m_strPlainInput << " lungo " << randNum << " bytes";

		Utils::WriteOnFile(m_strPlainInput, &strBuffer.front(), randNum);


		max = 10;
		min = 1;
		m_iHorcrux= rand() % (max - min + 1) + min;;
		

	}

	void TearDown() override {
		m_strInputOutputDir.clear();
		m_strPlainInput.clear();
		m_iHorcrux = 0;
	}



	bool InitEncryptMode(bool bForceHorcruxNum=false)
	{
		//se mi arriva quel booleano vuol dire che sto eseguendo il test per far fallire la decifratura, quindi non posso cifrare un solo horcrux, ce ne devono essere almeno 2
		if (bForceHorcruxNum && m_iHorcrux==1)
			m_iHorcrux = 2;
		std::string strPathOutput = m_strInputOutputDir + "horcruxAes.txt";
		HorcruxAesEncrypt.Init(m_strPlainInput, strPathOutput, m_iHorcrux);
		return HorcruxAesEncrypt.IsInitialized();
	}

	bool InitDecryptMode()
	{
		EXPECT_TRUE(SaveEncryptFiles());

		//creo il nome del file di output 
		std::string strOutputPathDec = HorcruxAesEncrypt.GetPathOutput() + HorcruxAesEncrypt.GetFileName() + "PlainText";
		//metto in un vettore i file creati
		std::vector<std::string> acOutputPath = {};
		for (int i = 0;i < m_iHorcrux;i++) {
			acOutputPath.push_back(HorcruxAesEncrypt.GetPathHorcruxs(i));

		}
		//ottengo la chiave in base 64
		std::string strKeyBase64 = HorcruxAesEncrypt.GetKeyBase64();

		HorcruxAesDecrypt.Init(strOutputPathDec, acOutputPath, strKeyBase64);
		return HorcruxAesDecrypt.IsInitialized();
	}

	bool SaveEncryptFiles(bool bForceHorcruxNum=false)
	{
		EXPECT_TRUE(InitEncryptMode(bForceHorcruxNum));
		EXPECT_TRUE(HorcruxAesEncrypt.Save());
		//verifichiamo che la somma della dimensione degli horcruxs corrisponda alla dimensione del file iniziale + padding
		std::uintmax_t plainInputSize = std::filesystem::file_size(m_strPlainInput);
		std::uintmax_t uiHorcruxsSize = 0;
		for (int i = 0;i < m_iHorcrux;i++) {
			std::string strPathHorcrux = HorcruxAesEncrypt.GetPathHorcruxs(i);
			uiHorcruxsSize += std::filesystem::file_size(strPathHorcrux);
			std::cout << "size horcrux " << uiHorcruxsSize << std::endl;
		}
		EXPECT_EQ(uiHorcruxsSize, plainInputSize + HorcruxAesEncrypt.GetPaddingSize());

		std::cout << "la somma della dimensione dei file cifrati " << uiHorcruxsSize << " corrisponde a quella del file in chiaro + padding" << std::endl;
		if(uiHorcruxsSize== (plainInputSize + HorcruxAesEncrypt.GetPaddingSize()))
			return true;
		return false;
	}


	CHorcruxAes HorcruxAesDecrypt;
	CHorcruxAes HorcruxAesEncrypt;
	std::string m_strPlainInput;
	std::string m_strInputOutputDir;
	int m_iHorcrux;
};

//tests di fail della init in encrypt mode
TEST(InitCypherFail, InitEncryptFailNumHorcruxBiggerThanBuff)
{
	CHorcruxAes HorcruxAesEncrypt;
	std::string strInputOutpuDir = "";
	std::string strPlainFileInput = "";
	HorcruxLibTest::GetPathTemp(strInputOutpuDir, strPlainFileInput);
	//creo un file con dei dati inferiori al numero di horcrux
	std::string strBuffer = "AA";
	Utils::WriteOnFile(strPlainFileInput, &strBuffer.front(), strBuffer.size());
	HorcruxAesEncrypt.Init(strPlainFileInput, strInputOutpuDir + "horcruxAes.txt", 3);
	EXPECT_FALSE(HorcruxAesEncrypt.IsInitialized());
}

TEST(InitCypherFail, InitEncryptFailNumHorcrux0orLess)
{
	CHorcruxAes HorcruxAesEncrypt;

	std::string strInputOutpuDir = "";
	std::string strPlainFileInput = "";
	HorcruxLibTest::GetPathTemp(strInputOutpuDir, strPlainFileInput);
	std::string strBuffer = "AA";
	Utils::WriteOnFile(strPlainFileInput, &strBuffer.front(), strBuffer.size());
	//num horcrux 0
	HorcruxAesEncrypt.Init(strPlainFileInput, strInputOutpuDir + "horcruxAes.txt", 0);
	EXPECT_FALSE(HorcruxAesEncrypt.IsInitialized());
	HorcruxAesEncrypt.Init(strPlainFileInput, strInputOutpuDir + "horcruxAes.txt", -1);
	EXPECT_FALSE(HorcruxAesEncrypt.IsInitialized());
}
TEST(InitCypherFail, InitEncryptFailPathInput)
{
	CHorcruxAes HorcruxAesEncrypt;
	std::string path=std::filesystem::temp_directory_path().u8string();
	HorcruxAesEncrypt.Init("", path+"horcruxAes.txt", 1);
	EXPECT_FALSE(HorcruxAesEncrypt.IsInitialized());

}
TEST(InitCypherFail, InitEncryptFailPathOutput)
{
	CHorcruxAes HorcruxAesEncrypt;
	//genero un file sulla temp per dare un file di input valido e poi passo il path di output non valorizzato
	std::string strInputOutpuDir = "";
	std::string strPlainFileInput = "";
	HorcruxLibTest::GetPathTemp(strInputOutpuDir, strPlainFileInput);
	std::string strBuffer = "AA";
	Utils::WriteOnFile(strPlainFileInput, &strBuffer.front(), strBuffer.size());
	HorcruxAesEncrypt.Init(strPlainFileInput, "", 1);
	EXPECT_FALSE(HorcruxAesEncrypt.IsInitialized());

}


TEST(InitCypherFail, InitDecryptFailOutputPath)
{
	//deve fallire perchè passo un path di output vuoto
	CHorcruxAes HorcruxAesDecrypt;
	std::string path = std::filesystem::temp_directory_path().u8string();
	std::vector<std::string> acHorcrux = { path+"provaAes0" };
	HorcruxAesDecrypt.Init("", acHorcrux, "zkXtA2TDbwZ7CeoFHAGao+6erNp1fHgEGyxkSGeb06g=");
	EXPECT_FALSE(HorcruxAesDecrypt.IsInitialized());

}

TEST(InitCypherFail, InitDecryptFailOutputNumHorcrux)
{
	//deve fallire perchè passo il path degli horcruxs vuoto
	CHorcruxAes HorcruxAesDecrypt;
	std::string outputDir = "";
	std::string strPlainInput = "";
	std::vector<std::string> acHorcrux = { };
	
	HorcruxLibTest::GetPathTemp(outputDir, strPlainInput);
	HorcruxAesDecrypt.Init(outputDir, acHorcrux, "zkXtA2TDbwZ7CeoFHAGao+6erNp1fHgEGyxkSGeb06g=");
	EXPECT_FALSE(HorcruxAesDecrypt.IsInitialized());

}

TEST(InitCypherFail, InitDecryptFailKeyLen)
{
	//deve fallire perchè passo una chiave piu corta
	CHorcruxAes HorcruxAesDecrypt;
	std::string outputDir = "";
	std::string strPlainInput = "";
	std::string path = std::filesystem::temp_directory_path().u8string();
	std::vector<std::string> acHorcrux = { path+"provaAes0" };
	
	HorcruxLibTest::GetPathTemp(outputDir, strPlainInput);
	HorcruxAesDecrypt.Init(outputDir, acHorcrux, "zkXtA2TDbwZ7CeoFHAGao+6erNp1fHgEGyxkSGeb06g");
	EXPECT_FALSE(HorcruxAesDecrypt.IsInitialized());

}


//TEST CLASS :


//test init valida
TEST_F(HorcruxLibTest, TestInitEncryptMode)
{
	EXPECT_TRUE(InitEncryptMode());

}

//test cifratura
TEST_F(HorcruxLibTest, TestEncryptFile)
{
	EXPECT_TRUE(SaveEncryptFiles());
}



//test Decifratura
TEST_F(HorcruxLibTest, TestDecryptInit)
{
	EXPECT_TRUE(InitDecryptMode());

}


TEST_F(HorcruxLibTest, DecryptMode)
{
	EXPECT_TRUE(InitDecryptMode());
	HorcruxAesDecrypt.Load();
	int buffFile1Size = std::filesystem::file_size(HorcruxAesEncrypt.GetPathInput());
	int buffFile2Size = std::filesystem::file_size(HorcruxAesDecrypt.GetPathOutput());
	//verifico che le dimensioni dei due file siano le stesse (file in chiaro passato nella fase di cifratura e file decifrato)
	EXPECT_EQ(buffFile1Size, buffFile2Size);
	//verifico che i buffer letti siano uguali
	std::vector<unsigned char> BuffFile1(buffFile1Size);
	std::vector<unsigned char> BuffFile2(buffFile2Size);
	Utils::ReadFromFile(HorcruxAesEncrypt.GetPathInput(), &BuffFile1.front());
	Utils::ReadFromFile(HorcruxAesDecrypt.GetPathOutput(), &BuffFile2.front());
	bool bAreEqual=(memcmp(&BuffFile1.front(), &BuffFile2.front(), buffFile2Size) == 0);
	EXPECT_TRUE(bAreEqual);

}



TEST_F(HorcruxLibTest, DecryptModeFail)
{
	//carico i file cifrati correttamente
	EXPECT_TRUE(SaveEncryptFiles(true));

	//creo il nome del file di output 
	std::string strOutputPathDec = HorcruxAesEncrypt.GetPathOutput() + HorcruxAesEncrypt.GetFileName() + "PlainText";
	//metto in un vettore i file creati
	std::vector<std::string> acOutputPath = {};
	for (int i = 0;i < m_iHorcrux;i++) {
		acOutputPath.push_back(HorcruxAesEncrypt.GetPathHorcruxs(i));

	}

	//elimino un path di horcrux dal vettore per farlo fallire
	acOutputPath.pop_back();
	//ottengo la chiave in base 64
	std::string strKeyBase64 = HorcruxAesEncrypt.GetKeyBase64();

	HorcruxAesDecrypt.Init(strOutputPathDec, acOutputPath, strKeyBase64);
	EXPECT_TRUE(HorcruxAesDecrypt.IsInitialized());
	HorcruxAesDecrypt.Load();

	int buffFile1Size = std::filesystem::file_size(HorcruxAesEncrypt.GetPathInput());
	int buffFile2Size = std::filesystem::file_size(HorcruxAesDecrypt.GetPathOutput());
	//verifico che le dimensioni dei due file siano le stesse (file in chiaro passato nella fase di cifratura e file decifrato)
	EXPECT_FALSE(buffFile1Size == buffFile2Size);
	//verifico che i buffer letti siano uguali
	std::vector<unsigned char> BuffFile1(buffFile1Size);
	std::vector<unsigned char> BuffFile2(buffFile2Size);
	Utils::ReadFromFile(HorcruxAesEncrypt.GetPathInput(), &BuffFile1.front());
	Utils::ReadFromFile(HorcruxAesDecrypt.GetPathOutput(), &BuffFile2.front());
	bool bAreEqual = (memcmp(&BuffFile1.front(), &BuffFile2.front(), std::max(buffFile2Size,buffFile1Size)) == 0);
	EXPECT_FALSE(bAreEqual);

}
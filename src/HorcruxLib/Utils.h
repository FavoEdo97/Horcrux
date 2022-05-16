#pragma once

namespace Utils
{

	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	static bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}


	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
	{
		std::string ret;
		int i = 0, j = 0;
		unsigned char char_array_3[3], char_array_4[4];

		while (in_len--)
		{
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';

		}

		return ret;

	}



	static std::string base64_decode(std::string const& encoded_string)
	{
		int in_len = encoded_string.size();
		int i = 0, j = 0, in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
		{
			char_array_4[i++] = encoded_string[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] = base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret += char_array_3[i];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++)
				ret += char_array_3[j];
		}

		return ret;
	}



	static void ReadFromFile(const std::string& strPathInput, unsigned char* buffOutput)
	{
		boost::iostreams::mapped_file mmap(strPathInput, boost::iostreams::mapped_file::readonly);
		size_t t = mmap.size();
		memcpy(buffOutput, mmap.const_data(), mmap.size());
		mmap.close();
	}

	static void WriteOnFile(const std::string& strFilePath, const char* buffInput, size_t lenBuff)
	{
		namespace bio = boost::iostreams;
		bio::mapped_file_params params;
		params.path = strFilePath;
		params.new_file_size = lenBuff;
		params.flags = bio::mapped_file::mapmode::readwrite;
		bio::stream<bio::mapped_file_sink> out(params);
		out.write(buffInput, lenBuff);
		out.close();
	}


	//.\horcrux.exe load -k <decryption key> <inputfiles> <output_file>
	static bool ParseCmdLoad(int argc, char* argv[], std::vector<std::string>& acPathsHorcruxs, std::string& strOutput_file, std::string& Key)
	{

		
		namespace po = boost::program_options;
		po::options_description desc("Allowed options");
		desc.add_options()
			("load", "decryption phase")
			("-k", po::value<std::vector<std::string>>(), "<decryption key>")
			;

		po::positional_options_description p;
		p.add("load", 1);
		p.add("-k", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
		po::notify(vm);

		if (vm.count("-k"))
		{
			std::vector <std::string> vArg = vm["-k"].as<std::vector<std::string>>();
			//ci devono essere almeno 3 campi se no manca qualcosa
			if (vArg.size() < 3)
			{
				std::cout << "Error load , you should use: horcrux load -k <decryption key> <inputfiles> <output_file>" << std::endl;
				return false;
			}
			else
			{
				//il primo parametro è la chiave
				Key = vArg[0];
				//l'ultimo è il file di output
				strOutput_file = vArg[vArg.size() - 1];
				vArg.pop_back();
				//tutti quelli in mezzo sono i path degli horcruxs
				for (int i = 1;i < vArg.size();i++)
				{
					acPathsHorcruxs.push_back(vArg[i]);

				}
			}
			
		}
		else
		{
			std::cout << "Error load , you should use: horcrux load -k <decryption key> <inputfiles> <output_file>" << std::endl;
			return false;
		}
		return true;

	}


	//per numeri non negativi
	static bool isNumber(const std::string& s)
	{

		return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
		/*return std::all_of(s.begin(), s.end(),
			[](char c) { return std::isdigit(c) != 0; });*/
	}



	//horcrux create -n <horcrux count> <inputpath> <output path>
	static bool ParseCmdCreate(int argc, char* argv[], std::string& strInputPath, std::string& strOutputPath, int& iHorcruxsCnt)
	{
		if (argc != 6)
		{
			std::cout << "Error create , you should use: horcrux create -k <decryption key> <inputfiles> <output_file>" << std::endl;
			return false;
		}
		namespace po = boost::program_options;
		po::options_description desc("Allowed options");
		desc.add_options()
			("create", "decryption phase")
			("-n", po::value<std::vector<std::string>>(), "<horcrux count>")
			;

		po::positional_options_description p;
		p.add("create", 1);
		p.add("-n", -1);


		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
		po::notify(vm);

		if (vm.count("-n"))
		{
			std::vector <std::string> vArg = vm["-n"].as<std::vector<std::string>>();
			//ci devono essere esattamente 3 campi se no manca qualcosa o c'è qualcosa di troppo
			if (vArg.size() != 3)
			{
				std::cout << "Error create , you should use: horcrux create -k <decryption key> <inputfiles> <output_file>" << std::endl;
				return false;
			}
			else
			{
				//primo parametro è il numero di horcruxs
				if (isNumber(vArg[0]))
				{
					iHorcruxsCnt = std::stoi(vArg[0]);
				}
				else
				{
					std::cout << "Error create, -n should be non-negative , you should use: horcrux create -k <decryption key> <inputfiles> <output_file>" << std::endl;
					return false;
				}

				if (iHorcruxsCnt <= 0)
				{
					std::cout << "Numero di horcruxs <=0 " << std::endl;
					return false;
				}
			
				//Poi c'è il file di input
				strInputPath = vArg[1];
				strOutputPath = vArg[2];
			}

		}
		else
		{
			std::cout << "Error create , you should use: horcrux create -k <decryption key> <inputfiles> <output_file>" << std::endl;
			return false;
		}

		return true;

	}



}
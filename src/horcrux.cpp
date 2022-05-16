// horcruxs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include "HorcruxLib/stdafx.h"

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cout << "You should use  : horcrux load -k <decryption key> <inputfiles> <output_file> " << std::endl;
        std::cout << " Or : horcrux create -n <horcrux count> <inputpath> <output path> " << std::endl;
        return -1;
    }  
    
    if (strcmp(argv[1], "load") == 0) {
        if (argc < 6)
        {
            std::cout << "You should use  : horcrux load -k <decryption key> <inputfiles> <output_file> " << std::endl;
            return -2;
        }
        std::vector<std::string> acPathsHorcruxs{};
        std::string strOutput_file = "";
        std::string Key = "";
        if (Utils::ParseCmdLoad(argc, argv, acPathsHorcruxs, strOutput_file, Key))
        {
            CHorcruxAes DeCypher;
            DeCypher.Init(strOutput_file, acPathsHorcruxs, Key);
            if (DeCypher.IsInitialized())
                DeCypher.Load();

        }
        else
        {
            std::cout << "Errore nel parsing dei parametri" << std::endl;
            return -3;
        }

    }
    else if (strcmp(argv[1], "create") == 0)
    {
        if (argc < 6)
        {
            std::cout << "You should use: horcrux create -n <horcrux count> <inputpath> <output path> " << std::endl;
            return -4;
        }
        int iHorcruxsCnt = 0;
        std::string strInputPath = "";
        std::string strOutputPath = "";
        if (Utils::ParseCmdCreate(argc, argv, strInputPath, strOutputPath, iHorcruxsCnt))
        {

            CHorcruxAes Cypher;
            Cypher.Init(strInputPath, strOutputPath, iHorcruxsCnt);
            if (Cypher.IsInitialized())
                if (!Cypher.Save())
                    return -5;

        }
        else
        {
            std::cout << "Errore nel parsing dei parametri" << std::endl;
            return -6;
        }

    }
    else
    {
        std::cout << "You should use  : horcrux load -k <decryption key> <inputfiles> <output_file> " << std::endl;
        std::cout << " Or : horcrux create -n <horcrux count> <inputpath> <output path> " << std::endl;
        return -7;
    }

    return 0;

}







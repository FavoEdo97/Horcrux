#pragma once
#include "stdafx.h"
#include "HorcruxAes.h"



void CHorcruxAes::Decrypt() {
    //decifro
    AES_set_decrypt_key(&m_acKey.front(), 256, &m_AesKey);
    for (int i = 0;i < m_acbuffInput.size();i += AES_BLOCK_SIZE)
    {
        AES_decrypt(&m_acbuffInput[i], &m_acbuffOutput[i],&m_AesKey);
    }
    //rimuovo il padding
    int iBytesPadding = m_acbuffOutput[m_acbuffOutput.size()-1];
    for (int i = 0;i < iBytesPadding;i++)
    {
        m_acbuffOutput.pop_back();
    }
    //svuoto i vector che non servono piu
    m_acKey.clear();
    m_acbuffInput.clear();
    return;
}


void CHorcruxAes::Encrypt() {

   AES_set_encrypt_key(&m_acKey.front(), 256, &m_AesKey);
   //aggiungo il padding ANSI X9.23
                        
   m_iRequiredPadding = (AES_BLOCK_SIZE - (m_acbuffInput.size() % AES_BLOCK_SIZE));
   for (int i = 0; i < m_iRequiredPadding -1; i++) {
       m_acbuffInput.push_back(0);
   }
   //l'ultimo byte mi indica quanto padding ho
   m_acbuffInput.push_back(m_iRequiredPadding);

   //cifro il buffer
   m_acbuffOutput.resize(m_acbuffInput.size());

   for (int i = 0;i < m_acbuffInput.size();i += AES_BLOCK_SIZE)
   {

       AES_encrypt(&m_acbuffInput[i], &m_acbuffOutput[i],&m_AesKey);
   }
   m_acbuffInput.clear();
   return;
}

bool CHorcruxAes::CreateHorcruxs()
{
    if (!IsInitialized() || m_acbuffOutput.size() <=0)
        return false;
    //creo gli horcruxs considerando dividendo la dimensione del buffer per il numero di file
    // l'ultimo horcruxs sarà più pesante perchè conterrà anche il resto
    int iSizeToAdd = m_acbuffOutput.size() % m_iNumHorcruxs;
    size_t ullDimHorcruxs = m_acbuffOutput.size() / m_iNumHorcruxs;
    //creao una lista di horcruxs
    m_acHorcruxs.resize(m_iNumHorcruxs);
    int i = 0;
    for (size_t bytes=0;i < m_iNumHorcruxs - 1; bytes += ullDimHorcruxs,i++)
    {
        m_acHorcruxs[i].resize(ullDimHorcruxs);
        memcpy(&m_acHorcruxs[i][0], &m_acbuffOutput[i* ullDimHorcruxs], ullDimHorcruxs);
    }
    //creo l'ultimo horcruxs
    m_acHorcruxs[i].resize(ullDimHorcruxs+ iSizeToAdd);
    memcpy(&m_acHorcruxs[i][0], &m_acbuffOutput[i*ullDimHorcruxs], ullDimHorcruxs+ iSizeToAdd);
    m_acbuffOutput.clear();
    return true;

}
void CHorcruxAes::SaveHorcruxs()
{
    //cicla sugli horcruxs passati in input e li salva
    namespace bio = boost::iostreams;
    for (int i = 0;i < m_iNumHorcruxs;i++)
    {
       m_acPathsHorcruxs.push_back(GetPathOutput() + GetFileName() + boost::lexical_cast<std::string>(i));
       Utils::WriteOnFile(m_acPathsHorcruxs[i], (const char*)&m_acHorcruxs[i].front(), m_acHorcruxs[i].size());
       std::cout << "Creato horcruxs: " << m_acPathsHorcruxs[i] << std::endl;
    }
    for (auto el : m_acHorcruxs)
        el.clear();
}

void CHorcruxAes::SavePlainText()
{
    Utils::WriteOnFile(GetPathOutput(), (const char*)&m_acbuffOutput.front(), m_acbuffOutput.size());
    std::cout << "Salvato Plain Text: " << GetPathOutput() << std::endl;
    //svuoto buffer di output
    m_acbuffOutput.clear();
}
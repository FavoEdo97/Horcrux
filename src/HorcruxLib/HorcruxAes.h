#pragma once


class CHorcruxAes :
	public CHorcrux
{
public:
	CHorcruxAes() :CHorcrux() { m_iRequiredPadding = 0; };
	~CHorcruxAes() {};
	inline int GetPaddingSize() { return m_iRequiredPadding; };
protected:
	virtual void Encrypt() override;
	virtual bool CreateHorcruxs() override;
	virtual void SaveHorcruxs() override;
	virtual void Decrypt() override;
	virtual void SavePlainText() override;
	AES_KEY m_AesKey;
	std::vector<std::vector<unsigned char>> m_acHorcruxs;///< Vettore con i path degli horcruxs passati dall'utente
	int m_iRequiredPadding;

};


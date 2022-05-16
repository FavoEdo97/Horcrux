#pragma once

class CHorcrux
{


public:
	CHorcrux();
	~CHorcrux(){};
	
	/**
	* Metodo di Init per la fase di cifratura.
	* In questo metodo vengono inizializzati le variabili membro e viene generata una chiave random la cui lunghezza è passata in input 
	* Se viene passato un path non valido la init fallisce.
	* I chunks verranno nominati come il nome inserito dall'utente senza l'estensione del file, se l'utente inserisce solo il path della cartella allora li nominerà Horcruxs(contatore) 
	* @param strPathInput Path del file in input da cifrare.
	* @param strPathOutput path nel quale verranno inseriti i vari chunk .
	* @param iHorcruxs Numero di chunk da creare.
	* @param keyLength Lunghezza della chiave da utilizzare (assert>=16).
	*/
	void Init(std::string strPathInput, std::string strPathOutput, const int& iHorcruxs,const int & keyLength=32);


	/**
	* Metodo di Init per la fase di Decifratura.
	* In questo metodo vengono inizializzati le variabili membro utili per la decifratura dei chunks
	* Se viene passato un path non valido la init fallisce.
	* @param strPathOutput Path del file di output decifrato.
	* @param acPathsHorcruxs Vettore che l'utente deve passare in modo ordinato per decifrare i chunks.
	* @param strKeyDec Chiave di decifratura.
	* @return Return Nothing.
	*/

	void Init(std::string strPathOutput, std::vector<std::string> acPathsHorcruxs, std::string strKeyDec);


	/**
	* Metodo che viene richiamato quando l'utente vuole creare i chunks cifrati
	* Richiama i metodi virtuali che i figli devono estendere per la cifratura /salvataggio dei file
	* @return Return Nothing.
	*/

	bool Save();
	/**
	* Metodo che viene richiamato quando l'utente vuole ottenere il file in chiaro dai file cifrati
	* Richiama i metodi virtuali che i figli devono estendere per la decifratura /salvataggio dei file 
	* @return Return true or false.
	*/
	void Load();
	

	/**
	* Metodo per verificare se l'oggetto è inizializzato correttamente
	* @return true se è inizializzato correttamente altrimenti false.
	*/
	inline bool IsInitialized() { return m_bInitialized; };


	/**
	* Metodo per ottenere solo il nome del file senza il path, utile solo per la fase di salvataggio degli horcruxs
	* @return Stringa con il nome del file
	*/

	inline std::string GetFileName() { return m_strFilename; };
	/**
	* Metodo per ottenere il path del file, se siamo nella fase di cifratura restituisce solo il path della cartella altrimenti l'intero path inserito dall'utente
	* @return Stringa con path di output
	*/
	inline std::string GetPathOutput() { return m_strPathOutput; };


	/**
	* Metodo per ottenere il path del file in input(quello in chiaro)
	* @return Stringa con path di input
	*/
	inline std::string GetPathInput() { return m_strPathInput; };



	/**
	* Metodo per ottenere la key in formato base64
	* @return Stringa con la key base64
	*/
	inline std::string GetKeyBase64() { return m_strKeyBase64; };


	/**
	* Metodo per ottenere il path di un horcruxs a uno specifico indice,utile solo nella fase di decifratura
	* @return Stringa con il path dell'horcruxs
	*/
	inline std::string GetPathHorcruxs(const int& iIndex) { if (m_acPathsHorcruxs.size() - 1 >= iIndex) return m_acPathsHorcruxs[iIndex]; else return ""; };

	/**
	* Metodo per sapere se siamo in modalità di ciratura oppure no, il valore della variabile viene valorizzato dopo la init
	* @return true or false
	*/
	inline bool IsEncryptModeEnabled() { return !m_bDec; };

protected:
	/**
	* Metodo virtuale per fare delle init aggiuntive
	* @return True di default 
	*/
	virtual bool initCypher() { return true; };
	/**
	* Metodo virtuale per fare la cifratura del buffer
	* @return Nothing
	*/
	virtual void Encrypt() = 0;
	/**
	* Metodo virtuale per fare la creazione dei chunks
	* @return Nothing
	*/
	virtual bool CreateHorcruxs()=0;
	/**
	* Metodo virtuale per salvare i chunks
	* @return Nothing
	*/
	virtual void SaveHorcruxs()=0;
	/**
	* Metodo virtuale per decifrare
	* @return Nothing
	*/
	virtual void Decrypt()=0;

	/**
	* Metodo virtuale per salvare il file decifrato
	* @return Nothing
	*/
	virtual void SavePlainText()=0;
	/**
	* Metodo virtuale per leggere gli horcruxs (nel caso in cui non fossero salvati nel file system i figli possono estendere il metodo)
	* @return Nothing
	*/
	virtual void ReadHorcruxs();
private:

	/**
	* Metodo per cifrare il file dato in input, esso utilizza la funzione readFromFile per popolare il buffer.
	* Infine richiama il metodo virtuale per la cifratura 
	* @return Nothing
	*/
	void EncryptFileBase();

	/**
	* Metodo per settare il file di output (e m_strFilename)
	* @return True se i path inseriti esistono false altrimenti
	*/
	bool SetOutputPath(const std::string& strPath);
	/**
	* Metodo per verificare se i path esistono (da verificare se funziona per linux)
	* @return True se i path inseriti esistono false altrimenti
	*/
	bool ExistDirPath(const std::string& strPath, size_t& iLogPath, std::string& strTmp);

protected:
	int m_iNumHorcruxs;///< numero di horcruxs da creare
	std::vector<unsigned char> m_acbuffInput;///< Buffer di input (viene valorizzato con il testo in chiaro in fase di cifratura e con il testo cifrato in fase di decifratura)
	std::vector<unsigned char> m_acbuffOutput;///< Buffer di output (viene valorizzato con il testo in chiaro in fase di decifratura e con il testo cifrato in fase di cifratura)
	std::vector<unsigned char > m_acKey;///< Buffer nel quale viene inserita la chiave da utilizzare
	std::vector<std::string> m_acPathsHorcruxs;///<vettore con i path degli horcruxs
private:
	std::string	m_strPathOutput;///<Path di output
	std::string	m_strFilename;///< Nome del file di output senza il percorso
	std::string	m_strPathInput;///<Path di input
	bool m_bDec;///< indica se siamo in modalità di decifratura oppure no
	bool m_bInitialized;///<indica se l'oggetto è stato inizializzato correttamente
	std::string m_strKeyBase64;///<stringa con la chiave codificata in base64

};


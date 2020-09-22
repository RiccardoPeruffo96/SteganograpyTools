#include <string>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include "iofile.h"
#include "aes128.h"
#include "aes256.h"
#include "steganography.h"
#include "DiffieHellman.h"

/**
 * @author Peruffo Riccardo
 *
 * @brief 
 * ENG DOC: show a work's option and ask an answer
 * ITA DOC: mostra le opzioni disponibili ed aspetta risposta
 *
 * @return int answer / l'opzione selezionata
 */
int menu();

/**
 * @author Peruffo Riccardo
 *
 * @brief 
 * ENG DOC: from a file's name, return the file's name without extension, or empty string if there isn't any extension 
 * ITA DOC: Data una stringa restituisce la stringa senza estensione, se non c'e' estensione, restituisce una stringa vuota
 *
 * @param s - file's name / il nome del file
 * @return  string without extension or empty string / stringa senza estensione o stringa vuota
 */
std::string namefileWithoutExtension(std::string s);

int main(int argv, char** argc)
{
	//test aes128
	//test_features();
	//debug aes256
	//new_features();
	//std::cout << "Cript controlled" << std::endl;
	//test aes128 ^-1
	//test_features_inv();
	//debug aes256 ^-1
	//new_features_inv();
	//exit(0);

	//menu
	switch (menu())
	{
		case 1:
		{
			//vogliamo nascondere dati in un'immagine:
			
			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["hide"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			Json::Value img_input = root["hide"]["img_input"]; //contiene i valori dell'attributo 'img_input' dentro il json
			if (img_input["name"].asString().compare("") != 0) //se va tutto a buon fine
			{
				steganography steg = steganography(); //dichiarazione oggetto

				/*
				std::string a = img_input["name"].asString();
				std::string b = inputfile.getFileName();
				//data_file;
				int n = inputfile.getSizeFile();
				std::string c = root["hide"]["img_output"]["name"].asString();
				*/
				steg.createMergeFile(img_input["name"].asString(), inputfile.getFileName(), data_file, inputfile.getSizeFile(), root["hide"]["img_output"]["name"].asString()); //creazione file di output
				steg.~steganography();
			}
			else
				std::cout << "Il nome dell'immagine che hai inserito nel file config.json e' senza estensione." << std::endl;
			
			//puliamo
			delete[] data_file;
			data_file = NULL;
			inputfile.~iofile();

			break;
		}

		case 2:
		{
			//vogliamo estrarre dati nascosti in un'immagine

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			std::string img_nome_file = root["seek"]["img_input"]["name"].asString(); //nome del file 
			std::string name_hide_file = ""; //contenitore del nome del file nascosto
			int data_file_length; //lunghezza file

			steganography steg = steganography();
			char* data_file = steg.DeMergeFile(img_nome_file, name_hide_file, data_file_length);
			steg.~steganography();

			std::string file_name_output = "";

			if (name_hide_file.compare(root["seek"]["file_output"]["name"].asString()) != 0)
			{
				std::string scelta = "";
				std::cout << "Il file trovato si chiama: " << name_hide_file;
				std::cout << "\nIl nome impostato su config.json e': " << root["seek"]["file_output"]["name"].asString();
				std::cout << "\nPer tenere il nome originale del file premere 1 e poi invio, altrimenti premi un tasto a scelta e poi invio e verra' impostato il nome configurato:\n> ";
				std::cin >> scelta;
				if(scelta.compare("1") == 0)
					file_name_output.append(name_hide_file);
				else 
					file_name_output.append(root["seek"]["file_output"]["name"].asString());
			}
			else
				file_name_output.append(name_hide_file);

			iofile outputfile = iofile(file_name_output, data_file_length);

			outputfile.writeFile(data_file);

			delete[] data_file;
			data_file = NULL;
			outputfile.~iofile();

			break;
		}

		case 3:
		{
			//criptare un file

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["encrypt"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			std::string key = ""; //contiene la stringa dei caratteri esadecimali della password

			//lettura password
			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 32 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input);

			std::cout << std::endl;

			//criptiamo il file in input
			aes128 aes = aes128(data_file, inputfile.getSizeFile(), key, true);
			//ed otteniamo i dati criptati
			char* encrypt_data_file = aes.getEncryptData();

			//creiamo l'oggetto per creare il nuovo file
			iofile outputfile = iofile(root["encrypt"]["file_output"]["name"].asString(), aes.getNewLength());
			aes.~aes128();

			//creiamo il nuovo file
			outputfile.writeFile(encrypt_data_file);

			//puliamo
			delete[] data_file;
			delete[] encrypt_data_file;
			data_file = NULL;
			encrypt_data_file = NULL;
			inputfile.~iofile();
			outputfile.~iofile();
			
			break;
		}

		case 4:
		{
			//decriptare un file

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["decrypt"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 32 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input); //qua ho ottenuto la chiave

			std::cout << std::endl;

			//decriptiamo il file
			aes128 aes = aes128(data_file, inputfile.getSizeFile(), key, false);
			char* decrypt_data_file = aes.getDecryptData();

			iofile outputfile = iofile(root["decrypt"]["file_output"]["name"].asString(), aes.getNewLength());
			aes.~aes128();

			//creiamo il file scaricando i char salvati
			outputfile.writeFile(decrypt_data_file);

			//puliamo
			delete[] data_file;
			delete[] decrypt_data_file;
			data_file = NULL;
			decrypt_data_file = NULL;
			inputfile.~iofile();
			outputfile.~iofile();

			break;
		}

		case 5:
		{
			//criptare un file e nasconderlo dentro un'immagine

			//fase 1
			//otteniamo le informazioni sul file
			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["hide"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			//fase 2
			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 32 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input);

			std::cout << std::endl;

			//fase 3
			//criptiamo il file

			aes128 aes = aes128(data_file, inputfile.getSizeFile(), key, true);
			char* encrypt_data_file = aes.getEncryptData();

			//fase 4
			//nascondiamo i dati all'interno dell'immagine
			Json::Value img_input = root["hide"]["img_input"]; //contiene i valori dell'attributo 'img_input' dentro il json
			if (img_input["name"].asString().compare("") != 0) //se va tutto a buon fine
			{
				steganography steg = steganography(); //dichiarazione oggetto

				/*
				std::string a = img_input["name"].asString();
				std::string b = inputfile.getFileName();
				//data_file;
				int n = inputfile.getSizeFile();
				std::string c = root["hide"]["img_output"]["name"].asString();
				*/
				steg.createMergeFile(img_input["name"].asString(), inputfile.getFileName(), encrypt_data_file, aes.getNewLength(), root["hide"]["img_output"]["name"].asString()); //creazione file di output
				steg.~steganography();
			}
			else
				std::cout << "Il nome dell'immagine che hai inserito nel file config.json e' senza estensione." << std::endl;

			//puliamo
			delete[] data_file;
			delete[] encrypt_data_file;
			data_file = NULL;
			encrypt_data_file = NULL;
			inputfile.~iofile();
			aes.~aes128();

			break;
		}

		case 6:
		{
			//estrarre dati da un'immagine e decriptarli

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			std::string img_nome_file = root["seek"]["img_input"]["name"].asString(); //nome del file 
			std::string name_hide_file = ""; //contenitore del nome del file nascosto
			int data_file_length; //lunghezza file

			//otteniamo i dati del file
			steganography steg = steganography();
			char* data_file = steg.DeMergeFile(img_nome_file, name_hide_file, data_file_length);
			steg.~steganography();

			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 32 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input); //qua ho ottenuto la chiave

			std::cout << std::endl;

			//decriptiamo il file
			aes128 aes = aes128(data_file, data_file_length, key, false);
			char* decrypt_data_file = aes.getDecryptData();

			//iter per scegliere il nome del file in output
			std::string file_name_output = "";

			if (name_hide_file.compare(root["seek"]["file_output"]["name"].asString()) != 0)
			{
				std::string scelta = "";
				std::cout << "Il file trovato si chiama: " << name_hide_file;
				std::cout << "\nIl nome impostato su config.json e': " << root["seek"]["file_output"]["name"].asString();
				std::cout << "\nPer tenere il nome originale del file premere 1 e poi invio, altrimenti premi un tasto a scelta e poi invio e verra' impostato il nome configurato:\n> ";
				std::cin >> scelta;
				if (scelta.compare("1") == 0)
					file_name_output.append(name_hide_file);
				else
					file_name_output.append(root["seek"]["file_output"]["name"].asString());
			}
			else
				file_name_output.append(name_hide_file);

			//scrittura dei dati
			iofile outputfile = iofile(file_name_output, aes.getNewLength());

			outputfile.writeFile(decrypt_data_file);

			delete[] data_file;
			delete[] decrypt_data_file;
			data_file = NULL;
			decrypt_data_file = NULL;
			outputfile.~iofile();
			aes.~aes128();

			break;
		}

		case 7:
		{
			//generare una nuova semi chiave per codificare/decodificare
			DfHl::DiffieHellman DH = DfHl::DiffieHellman();
			DH.HalfKey_Generator_128bits();
			//DH.HalfKey_Generator_PAR();
			std::cout << "\nSalva e nascondi questo numero esadecimale di 32 caratteri per poter generare la chiave derivante dalla mezza chiave ricevuta dal tuo confidente:\n> " << DH.getX() << std::endl;
			std::cout << "Invia questo numero esadecimale di 32 caratteri al tuo confidente per poter generare una chiave:\n> " << DH.getY() << std::endl;
			DH.~DiffieHellman();
			break;
		}

		case 8:
		{
			std::cout << std::endl;
			//generare una nuova chiave da una semichiave per codificare/decodificare
			std::string X;
			std::string Y;

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "Inserisci la semichiave di 32 caratteri che il tuo amico ti ha inviato:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa semichiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della semichiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa semichiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			Y.append(input);

			std::cout << std::endl;

			inputCheck = 0;
			//check input strings
			do
			{
				std::cout << "Inserisci la semichiave segreta di 32 caratteri che hai mantenuto:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 32)
				{
					//every char it's a hex
					for (int i = 0; i < 32; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa semichiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della semichiave dev'essere di 32 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa semichiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			X.append(input);

			std::cout << std::endl;

			DfHl::DiffieHellman DH = DfHl::DiffieHellman(X, Y);
			DH.Key_Generator_from_HalfKey_128bits();
			std::cout << "La chiave segreta usata per criptare e decriptare e':\n> " << DH.getK() << std::endl;
			DH.~DiffieHellman();
			std::cout << "MANTIENILA SEGRETA!!!" << std::endl;

			break;
		}

		case 9:
		{
			//vogliamo sovrascrivere un file e poi cancellarlo
			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["remove_file"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString(), true); //cancello il file

			break;
		}

		case 10:
		{
			//criptare un file

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["encrypt"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			std::string key = ""; //contiene la stringa dei caratteri esadecimali della password

			//lettura password
			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 64 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 64
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input);

			std::cout << std::endl;

			//criptiamo il file in input
			aes256 aes = aes256(data_file, inputfile.getSizeFile(), key, true);
			//ed otteniamo i dati criptati
			char* encrypt_data_file = aes.getEncryptData();

			//creiamo l'oggetto per creare il nuovo file
			iofile outputfile = iofile(root["encrypt"]["file_output"]["name"].asString(), aes.getNewLength());
			aes.~aes256();

			//creiamo il nuovo file
			outputfile.writeFile(encrypt_data_file);

			//puliamo
			delete[] data_file;
			delete[] encrypt_data_file;
			data_file = NULL;
			encrypt_data_file = NULL;
			inputfile.~iofile();
			outputfile.~iofile();

			break;
		}

		case 11:
		{
			//decriptare un file

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["decrypt"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 64 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 64
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input); //qua ho ottenuto la chiave

			std::cout << std::endl;

			//decriptiamo il file
			aes256 aes = aes256(data_file, inputfile.getSizeFile(), key, false);
			char* decrypt_data_file = aes.getDecryptData();

			iofile outputfile = iofile(root["decrypt"]["file_output"]["name"].asString(), aes.getNewLength());
			aes.~aes256();

			//creiamo il file scaricando i char salvati
			outputfile.writeFile(decrypt_data_file);

			//puliamo
			delete[] data_file;
			delete[] decrypt_data_file;
			data_file = NULL;
			decrypt_data_file = NULL;
			inputfile.~iofile();
			outputfile.~iofile();

			break;
		}

		case 12:
		{
			//criptare un file e nasconderlo dentro un'immagine

			//fase 1
			//otteniamo le informazioni sul file
			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			//leggiamo l'input
			Json::Value file = root["hide"]["file_input"]; //contiene i valori dell'attributo 'file' dentro il json

			iofile inputfile = iofile(file["name"].asString()); //ottengo il valore relativo al campo 'name' nel json

			char* data_file = inputfile.getData(); //leggo i dati del file

			//fase 2
			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 64 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input);

			std::cout << std::endl;

			//fase 3
			//criptiamo il file

			aes256 aes = aes256(data_file, inputfile.getSizeFile(), key, true);
			char* encrypt_data_file = aes.getEncryptData();

			//fase 4
			//nascondiamo i dati all'interno dell'immagine
			Json::Value img_input = root["hide"]["img_input"]; //contiene i valori dell'attributo 'img_input' dentro il json
			if (img_input["name"].asString().compare("") != 0) //se va tutto a buon fine
			{
				steganography steg = steganography(); //dichiarazione oggetto

				/*
				std::string a = img_input["name"].asString();
				std::string b = inputfile.getFileName();
				//data_file;
				int n = inputfile.getSizeFile();
				std::string c = root["hide"]["img_output"]["name"].asString();
				*/
				steg.createMergeFile(img_input["name"].asString(), inputfile.getFileName(), encrypt_data_file, aes.getNewLength(), root["hide"]["img_output"]["name"].asString()); //creazione file di output
				steg.~steganography();
			}
			else
				std::cout << "Il nome dell'immagine che hai inserito nel file config.json e' senza estensione." << std::endl;

			//puliamo
			delete[] data_file;
			delete[] encrypt_data_file;
			data_file = NULL;
			encrypt_data_file = NULL;
			inputfile.~iofile();
			aes.~aes256();

			break;
		}

		case 13:
		{
			//estrarre dati da un'immagine e decriptarli

			//leggiamo nome del file, immagine di input ed output
			std::ifstream config_file("config.json");
			Json::Value root;
			config_file >> root;
			config_file.close();

			std::string img_nome_file = root["seek"]["img_input"]["name"].asString(); //nome del file 
			std::string name_hide_file = ""; //contenitore del nome del file nascosto
			int data_file_length; //lunghezza file

			//otteniamo i dati del file
			steganography steg = steganography();
			char* data_file = steg.DeMergeFile(img_nome_file, name_hide_file, data_file_length);
			steg.~steganography();

			//otteniamo la password
			std::string key = "";

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "\nInserisci la chiave di 64 caratteri generata con l'algoritmo DiffieHellman:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 32
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa chiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della chiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa chiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			key.append(input); //qua ho ottenuto la chiave

			std::cout << std::endl;

			//decriptiamo il file
			aes128 aes = aes128(data_file, data_file_length, key, false);
			char* decrypt_data_file = aes.getDecryptData();

			//iter per scegliere il nome del file in output
			std::string file_name_output = "";

			if (name_hide_file.compare(root["seek"]["file_output"]["name"].asString()) != 0)
			{
				std::string scelta = "";
				std::cout << "Il file trovato si chiama: " << name_hide_file;
				std::cout << "\nIl nome impostato su config.json e': " << root["seek"]["file_output"]["name"].asString();
				std::cout << "\nPer tenere il nome originale del file premere 1 e poi invio, altrimenti premi un tasto a scelta e poi invio e verra' impostato il nome configurato:\n> ";
				std::cin >> scelta;
				if (scelta.compare("1") == 0)
					file_name_output.append(name_hide_file);
				else
					file_name_output.append(root["seek"]["file_output"]["name"].asString());
			}
			else
				file_name_output.append(name_hide_file);

			//scrittura dei dati
			iofile outputfile = iofile(file_name_output, aes.getNewLength());

			outputfile.writeFile(decrypt_data_file);

			delete[] data_file;
			delete[] decrypt_data_file;
			data_file = NULL;
			decrypt_data_file = NULL;
			outputfile.~iofile();
			aes.~aes128();

			break;
		}

		case 14:
		{
			//generare una nuova semi chiave per codificare/decodificare
			DfHl::DiffieHellman DH = DfHl::DiffieHellman();
			DH.HalfKey_Generator_256bits();
			//DH.HalfKey_Generator_PAR();
			std::cout << "\nSalva e nascondi questo numero esadecimale di 64 caratteri per poter generare la chiave derivante dalla mezza chiave ricevuta dal tuo confidente:\n> " << DH.getX() << std::endl;
			std::cout << "Invia questo numero esadecimale di 64 caratteri al tuo confidente per poter generare una chiave:\n> " << DH.getY() << std::endl;
			DH.~DiffieHellman();
			break;
		}

		case 15:
		{
			std::cout << std::endl;
			//generare una nuova chiave da una semichiave per codificare/decodificare
			std::string X;
			std::string Y;

			std::string input;
			std::string conferma;
			int inputCheck = 0;
			int otherChar;

			//check input strings
			do
			{
				std::cout << "Inserisci la semichiave di 64 caratteri che il tuo amico ti ha inviato:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 64
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa semichiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della semichiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa semichiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			Y.append(input);

			std::cout << std::endl;

			inputCheck = 0;
			//check input strings
			do
			{
				std::cout << "Inserisci la semichiave segreta di 64 caratteri che hai mantenuto:\n> ";
				otherChar = 0;
				std::cin >> input;

				//input.length must be 64
				if (input.length() == 64)
				{
					//every char it's a hex
					for (int i = 0; i < 64; i++)
					{
						//se c'e' un solo char non hex
						if (!isxdigit(input[i]))
						{
							otherChar++; //viene marchiato
						}
					}

					//se non e' marchiato
					if (otherChar == 0)
					{
						inputCheck++; //svincolo il ciclo
					}
					else
					{
						//altrimenti riprova finche' non si sveglia
						std::cout << "\nLa semichiave inserita contiene caratteri non validi (non esadecimali).\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
					}
				}
				else
				{
					std::cout << "\nLa lunghezza della semichiave dev'essere di 64 caratteri.\nL'input che hai inserito non puo' essere corretto: Riprova\n" << std::endl;
				}

				if (inputCheck != 0) //ultimo baluardo di conferma
				{
					std::cout << "\nLa semichiave da te inserita e':\n> " << input << "\n\nVuoi confermarla? (s/n)\n> ";
					std::cin >> conferma;

					if (conferma[0] != 's' && conferma[0] != 'S')
						inputCheck = 0;
				}

			} while (inputCheck == 0);

			X.append(input);

			std::cout << std::endl;

			DfHl::DiffieHellman DH = DfHl::DiffieHellman(X, Y);
			DH.Key_Generator_from_HalfKey_256bits();
			std::cout << "La chiave segreta usata per criptare e decriptare e':\n> " << DH.getK() << std::endl;
			DH.~DiffieHellman();
			std::cout << "MANTIENILA SEGRETA!!!" << std::endl;

			break;
		}

		default:
		{
			//opzione non prevista
			std::cout << "Unexpected way" << std::endl;
			break;
		}
	}

	std::cout << "\nDone:\tGoodbye!\n";

	return 0;

}

int menu()
{
	std::string intext;
	int inputMenu = 0;
	do
	{
		std::cout <<
			"Benvenuto!\n\n"
			"Seleziona cosa vuoi fare:\n\t"
			"1) nascondere dati in un'immagine\n\t"
			"2) estrarre dati da un'immagine\n\t"
			"3) criptare un file usando l'algoritmo AES-128\n\t"
			"4) decriptare un file usando l'algoritmo AES-128\n\t"
			"5) criptare un file e nasconderlo dentro un'immagine usando l'algoritmo AES-128\n\t"
			"6) estrarre dati da un'immagine e decriptarli usando l'algoritmo AES-128\n\t"
			"7) generare una nuova semi chiave per generare una chiave usando l'algoritmo DiffieHellman in modalita' compatibile con AES-128\n\t"
			"8) generare una nuova chiave da una semichiave per codificare e decodificare usando l'algoritmo DiffieHellman in modalita' compatibile con AES-128\n\t"
			"9) cancellare un file in modo che sia irrecuperabile dal disco fisso\n\t"
			"10) criptare un file usando l'algoritmo AES-256\n\t"
			"11) decriptare un file usando l'algoritmo AES-256\n\t"
			"12) criptare un file e nasconderlo dentro un'immagine usando l'algoritmo AES-256\n\t"
			"13) estrarre dati da un'immagine e decriptarli usando l'algoritmo AES-256\n\t"
			"14) generare una nuova semi chiave per generare una chiave usando l'algoritmo DiffieHellman in modalita' compatibile con AES-256\n\t"
			"15) generare una nuova chiave da una semichiave per codificare e decodificare usando l'algoritmo DiffieHellman in modalita' compatibile con AES-256\n\n"
			"Inserisci un valore tra 1 e 15 e premi invio:\n\n> ";

		std::cin >> intext;

		inputMenu = atoi(intext.c_str());

		if (inputMenu < 1 || inputMenu > 15)
			std::cout << "Input non accettato: Riprova\n";

	} while (inputMenu < 1 || inputMenu > 15);

	return inputMenu;
}

std::string namefileWithoutExtension(std::string s)
{
	/*
	//"lol.txt"
	0 l
	1 o
	2 l
	3 .
	4 t
	5 x
	6 t
	
	l = 7
	i = 6
	s[6] = t nope
	s[5] = x nope
	s[4] = t nope
	s[3] = . yes
	*/
	std::string res = "";
	int l = s.length();

	for (int i = l - 1; i >= 0; i--)
	{
		if (s[i] == '.')
			res.append(s.substr(0, i));
	}

	return res;
}
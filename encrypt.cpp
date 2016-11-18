#include <cstring>
#include "EncryptedArray.h"
#include "FHE.h"
#include <fstream>
#include "myUtils.h"
#include <NTL/ZZX.h>
#include <iostream>

int main(int argc, char **argv)
{
	// Read the public key from disk
	Timer tReadKeys;
	tReadKeys.start();
	fstream pubKeyFile("pubkey.txt", fstream::in);
	assert(pubKeyFile.is_open());
	unsigned long m, p, r;
	vector<long> gens, ords;

	readContextBase(pubKeyFile, m, p, r, gens, ords);
	FHEcontext context(m, p, r, gens, ords);
	pubKeyFile >> context;

	FHEPubKey publicKey(context);
	pubKeyFile >> publicKey;	
	
	pubKeyFile.close();

	tReadKeys.stop();
	std::cout << "Time for reading keys from disk: " << tReadKeys.elapsed_time() << "s" << std::endl;	
	
	EncryptedArray ea(context);	
	uint nslots = ea.size();

	// Read the message to be encrypted
	// This is specific to this example and will not work for messages that are, for example, sentences.
	// In this case, it should be done a mapping between strings and a vector of ints (or bits), like Bloom filters or other techniques.	
	std::fstream messageFile("message.txt", fstream::in);
	assert(messageFile.is_open());

	uint count = 0;
	std::vector<long int> plaintext(nslots, 0);        
	for (;;) {
		std::string token;
		if(!(messageFile>>token)) break;
		plaintext[count] = std::stoi(token);
		count++;
	}
	messageFile.close();
	
	std::cout << "Message to be encrypted: " << std::endl << plaintext << std::endl;
	
    // Encryption
	Timer tEncryption;
	tEncryption.start();

	Ctxt ctxt(publicKey); 
	ea.encrypt(ctxt, publicKey, plaintext);

	tEncryption.stop();
	std::cout << "Time for encryption: " << tEncryption.elapsed_time() << "s" << std::endl;

	// Output ciphertext to file
	std::fstream ciphertext("ciphertext.txt", fstream::out|fstream::trunc);
	assert(ciphertextFile.is_open());
	ciphertext << ctxt;
	ciphertext.close();

	return 0;
}

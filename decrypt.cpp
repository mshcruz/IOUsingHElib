#include <cstring>
#include "EncryptedArray.h"
#include "FHE.h"
#include <fstream>
#include "myUtils.h"
#include <NTL/ZZX.h>
#include <iostream>

int main(int argc, char **argv)
{
	// Read secret key from file
	Timer tReadPrivateKey;
	tReadPrivateKey.start();
	
	fstream secKeyFile("seckey.txt", fstream::in);
	unsigned long m, p, r;
	vector<long> gens, ords;
	readContextBase(secKeyFile, m, p, r, gens, ords);
	FHEcontext context(m, p, r, gens, ords);
	secKeyFile >> context;

	FHESecKey secretKey(context);
	const FHEPubKey& publicKey = secretKey;

	secKeyFile >> secretKey;

	tReadPrivateKey.stop();
	std::cout << "Time for reading private key from disk: " << tReadPrivateKey.elapsed_time() << "s" << std::endl;	

	// Read ciphertext from file
	Timer tReadCiphertext;
	tReadCiphertext.start();
	fstream ciphertextFile("ciphertext.txt", fstream::in);	
	Ctxt ctxt(publicKey);
	ciphertextFile >> ctxt;

	tReadCiphertext.stop();
	std::cout << "Time for reading ciphertext from disk: " << tReadCiphertext.elapsed_time() << "s" << std::endl;	

	// Decrypt ciphertext
	Timer tDecryption;
	tDecryption.start();
	
	EncryptedArray ea(context);
	long nslots = ea.size();
	
	std::vector<long int> decrypted(nslots,0);
	ea.decrypt(ctxt, secretKey, decrypted);	

	tDecryption.stop();
	std::cout << "Time for decryption: " << tDecryption.elapsed_time() << "s" << std::endl;	
	
	std::cout << "Decrypted message: " << std::endl <<  decrypted << std::endl;

	return 0;
}

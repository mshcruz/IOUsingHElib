COMPILER = $(CPP)
CPPFLAGS = -Wall -std=c++11 -DNDEBUG -g2 -I$(HELIB) -pthread -DFHE_THREADS -DFHE_DCRT_THREADS -DFHE_BOOTS_THREADS
LIBS = $(HELIB)/fhe.a -lntl -lgmp -lm -lrt -lcryptopp
TARGET = keyGenerator encrypt decrypt

.PHONY: all clean run

all: $(TARGET)

keyGenerator: keyGenerator.cpp
	$(COMPILER) $(CPPFLAGS) -o $@ $< $(LIBS)

encrypt: encrypt.cpp
	$(COMPILER) $(CPPFLAGS) -o $@ $< $(LIBS)

decrypt: decrypt.cpp
	$(COMPILER) $(CPPFLAGS) -o $@ $< $(LIBS)

run: $(TARGET)
	./keyGenerator && \
	./encrypt && \
	./decrypt

clean:
	rm -f $(TARGET) ciphertext.txt seckey.txt pubkey.txt 

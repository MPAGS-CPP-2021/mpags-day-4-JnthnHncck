//! Unit Tests for MPAGSCipher CaesarCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Encryption Functions Correctly", "[playfair]")
{
    PlayfairCipher pc{"Pla,yfa&ir Ex&am,ple"};
    REQUIRE(pc.applyCipher("HELLOWORLD", CipherMode::Encrypt) == "DMYRANVQCRHV");
}

TEST_CASE("Decryption Functions Correctly", "[playfair]")
{
    PlayfairCipher pc{"Pla,yfa&ir Ex&am,ple"};
    REQUIRE(pc.applyCipher("DMYRANVQCRHV", CipherMode::Decrypt) == "HELXLOWORLDZ");
}
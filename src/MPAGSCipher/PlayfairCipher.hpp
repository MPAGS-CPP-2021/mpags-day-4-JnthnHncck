#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

class PlayfairCipher {

    public: 
        /**
        * \brief Create a new PlayfairCipher with the given key
        *
        * \param key the key to use in the cipher
        */
        explicit PlayfairCipher(const std::string& key);

        /**
        * \brief Sets the key of the cipher to the given value
        *
        * \param key the new key to use in the cipher
        */
        void setKey(const std::string& key);

        /**
        * \brief Applies the cipher to a given input string
        *
        * \param inputText A string to be encrypted or decrypted
        * \param cipherMode Indicates whether the string should be encrypted or decrypted
        * \return The result of applying the cipher to inputText
        */
        std::string applyCipher(const std::string& inputText, const CipherMode cipherMode) const;

    private:
        std::string key_{""};
        const std::string alphabet_{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        std::map< char, std::pair< int, int > > L2C_;
        std::map< std::pair< int, int >, char > C2L_;
};

#endif //MPAGSCIPHER_PLAYFAIRCIPHER_HPP
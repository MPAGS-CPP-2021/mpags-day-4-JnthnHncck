#include "PlayfairCipher.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <math.h>

PlayfairCipher::PlayfairCipher(const std::string& key) {
    PlayfairCipher::setKey(key);
}

void PlayfairCipher::setKey(const std::string& key) {
    // store the original key
    key_ = key;
    
    // Append the alphabet
    key_ = key_ + alphabet_;
    
    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    // Remove non-alpha characters
    auto to_delete = [] (unsigned char ch1) {return !(std::isalpha(ch1));};

    auto iter = std::remove_if(key_.begin(), key_.end(), to_delete);
    key_.erase(iter, key_.end());

    // Change J -> I
    auto change_J = [&] (char ch2) {
        if (ch2 == 'J') {
            return 'I';
        }
        else {
            return ch2;
        }
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), change_J);
    
    // Remove duplicated letters
    std::string prev_chars;
    auto check_dup = [&] (unsigned char new_char) {
        if (prev_chars.find(new_char) == std::string::npos) {
            prev_chars += new_char;
            return false;
        }
        else {
            return true;
        }
    };
    auto iter2 = std::remove_if(key_.begin(), key_.end(), check_dup);
    key_.erase(iter2, key_.end());

    // Store the coords of each letter
    // Store the playfair cipher key map
    for (int i{0}; i<25; i++) {
        int col = i % 5;
        int row = i / 5;
        std::pair< int, int > coords{ row, col };
        std::pair< char, std::pair < int, int > > L2CPair{ key_[i], coords };
        std::pair< std::pair < int, int >, char > C2LPair{ coords, key_[i] };
        L2C_.insert(L2CPair);
        C2L_.insert(C2LPair);
    }
}

std::string PlayfairCipher::applyCipher( \
    const std::string& inputText, \
    const CipherMode cipherMode ) const
{   
    // Change J → I
    std::string inputTextJ2I{inputText};
    for (auto& inputChar : inputTextJ2I) {
        if (inputChar == 'J') {
            inputChar = 'I';
        }
    }
    
    // If repeated chars in a digraph add an X or Q if XX
    const size_t inputSize = inputTextJ2I.size();
    char prevChar{'?'};
    std::string newText{""};
    for (size_t i{0}; i < inputSize; i++) {
        if (inputTextJ2I[i] != prevChar) {
            newText += inputTextJ2I[i];
        }
        else if (prevChar != 'X') {
            newText += 'X';
            newText += inputTextJ2I[i];
        }
        else {
            newText += 'Q';
            newText += inputTextJ2I[i];
        }
        prevChar = inputTextJ2I[i];
    }
    
    // if the size of input is odd, add a trailing Z
    size_t newSize = newText.size();
    if (newSize % 2 == 1) {
        newText += 'Z';
        newSize++;
    }
    
    // Loop over the input in Digraphs
    std::string finalText{""};
    for (size_t i{0}; i < newSize; i+=2) {
        //   - Find the coords in the grid for each digraph
        char di1{newText[i]};
        char di2{newText[i+1]};
        auto cFinder1 = L2C_.find(di1);
        auto cFinder2 = L2C_.find(di2);
        std::pair< int, int > coords1{(*cFinder1).second};
        std::pair< int, int > coords2{(*cFinder2).second};

        //   - Apply the rules to these coords to get 'new' coords
        int rowDiff = abs(coords1.first - coords2.first);
        int colDiff = abs(coords1.second - coords2.second);

        //We seem to need to initialise these pairs outside of the switch logic
        std::pair< int, int > newCoords1{ 0, 0 };
        std::pair< int, int > newCoords2{ 0, 0 };

        switch(cipherMode) {
            case CipherMode::Encrypt:
                if (rowDiff == 0) {
                    //Same row: the column is increased by one
                    newCoords1.first = coords1.first;
                    newCoords1.second = (coords1.second + 1) % 5;
                    newCoords2.first = coords2.first;
                    newCoords2.second = (coords2.second + 1) % 5;
                }
                else if (colDiff == 0) {
                    //Same column: the row is increased by one
                    newCoords1.first = (coords1.first + 1) % 5;
                    newCoords1.second = coords1.second;
                    newCoords2.first = (coords2.first + 1) % 5;
                    newCoords2.second = coords2.second;
                }
                else {
                    //Rectangle: the row stays constant, the column is changed to that of the other letter
                    newCoords1.first = coords1.first;
                    newCoords1.second = coords2.second;
                    newCoords2.first = coords2.first;
                    newCoords2.second = coords1.second;
                }
                break;

            case CipherMode::Decrypt:
                if (rowDiff == 0) {
                    //Same row: the column is decreased by one
                    newCoords1.first = coords1.first;
                    newCoords1.second = (coords1.second + 4) % 5;
                    newCoords2.first = coords2.first;
                    newCoords2.second = (coords2.second + 4) % 5;
                }
                else if (colDiff == 0) {
                    //Same column: the row is decreased by one
                    newCoords1.first = (coords1.first + 4) % 5;
                    newCoords1.second = coords1.second;
                    newCoords2.first = (coords2.first + 4) % 5;
                    newCoords2.second = coords2.second;
                }
                else {
                    //Rectangle: the row stays constant, the column is changed to that of the other letter
                    newCoords1.first = coords1.first;
                    newCoords1.second = coords2.second;
                    newCoords2.first = coords2.first;
                    newCoords2.second = coords1.second;
                }
                break;
        }

        //   - Find the letter associated with the new coords
        auto lFinder1 = C2L_.find(newCoords1);
        auto lFinder2 = C2L_.find(newCoords2);
        finalText += (*lFinder1).second;
        finalText += (*lFinder2).second;
    }

    // return the text
    return finalText;
}
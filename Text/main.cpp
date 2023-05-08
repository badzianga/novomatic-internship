#include <algorithm>
#include <iostream>
#include <string>

const std::string WHITESPACE = " \n\r\t\f\v";
const std::string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
const std::string NUMBER = "0123456789";
const std::string ALPHANUMERIC = UPPERCASE + LOWERCASE + NUMBER;


// Pauses program until user presses any key.
void pause() {
#if _WIN32
    system("pause");
#else
    system("read");
#endif
}


// Pops first word from text and returns it.
std::string getWord(std::string &predicate) {
    std::string nextWord;
    int nextSpace = predicate.find_first_of(WHITESPACE);
    if (nextSpace != std::string::npos) {
        nextWord = predicate.substr(0, nextSpace);
        predicate.erase(0, nextSpace + 1);
    }
    else {
        nextWord = predicate;
        predicate = "";
    }
    return nextWord;
}


// Prints text about invalid predicate and exits program.
void throwError() {
    std::cout << "Niewlasciwy predykat!\n";
    std::cout << "Zastosowanie: [is/-] [not/isn't/-] [whitespace/uppercase/lowercase/number/alphanumeric]\n";
    std::cout << "lub: " << "[is/-] [not/isn't/-] [letter/digit] <znak>\n";
    std::cout << "Przyklady:\n"; 
    std::cout << "is whitespace\n";
    std::cout << "not letter a\n";
    std::cout << "isn't digit 9\n";
    pause();
    exit(1);
}


int main() {
    setlocale (LC_CTYPE, "Polish");

    std::string text;
    std::string predicate;

    std::cout << "tekst: ";
    std::getline(std::cin, text);

    std::cout << "predykat: ";
    std::getline(std::cin, predicate);
    std::for_each(predicate.begin(), predicate.end(), [](char& c){ c = tolower(c); });

    std::string word;
    std::string selectedPredicate = "";
    bool negated = false;
    bool letterNext = false;
    bool isBefore = false;
    size_t start;
    size_t end;

    while (true) {
        word = getWord(predicate);

        if (word == "whitespace") {
            selectedPredicate = WHITESPACE;
        }
        else if (word == "lowercase") {
            selectedPredicate = LOWERCASE;
        }
        else if (word == "uppercase") {
            selectedPredicate = UPPERCASE;
        }
        else if (word == "number") {
            selectedPredicate = NUMBER;
        }
        else if (word == "alphanumeric") {
            selectedPredicate = ALPHANUMERIC;
        }
        else if (word == "letter" || word == "digit") {
            letterNext = true;
        }
        else if (letterNext) {
            if (word.length() == 1) {
                letterNext = false;
                selectedPredicate = word;
            }
            else {
                throwError();
            }
        }
        else if (word == "is") {
            if (isBefore) {
                throwError();
            }
            else {
                isBefore = true;
                continue;
            }
        }
        else if (word == "not" || word == "isn't") {
            negated = !negated;
        }
        else {
            throwError();
        }

        if (selectedPredicate != "") {
            if (negated) {
                start = text.find_first_of(selectedPredicate);
                end = text.find_last_of(selectedPredicate);
                negated = false;
            }
            else {
                start = text.find_first_not_of(selectedPredicate);
                end = text.find_last_not_of(selectedPredicate);
            }

            selectedPredicate = "";

            if (end != std::string::npos) {
                text = text.substr(0, end + 1);
            }

            if (start != std::string::npos) {
                text = text.substr(start);
            }

            break;
        }
    }

    std::cout << text << '\n';

    pause();

    return 0;
}

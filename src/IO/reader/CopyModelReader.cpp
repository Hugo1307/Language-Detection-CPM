#include <iostream>
#include "CopyModelReader.h"
#include "../../utils/utils.h"

CopyModelReader::CopyModelReader(std::string filePath, int windowSize) : Reader(std::move(filePath)) {
    this->windowSize = windowSize;
    this->currentPosition = 0;
    this->pastPosition = 0;
}

bool CopyModelReader::readWindow() {

    if (!Reader::getFileInputStream()->is_open())
        return false;

    // This means that we have expanded the window, but, we have reached the threshold, and we want to stop
    // expanding the window.
    if (this->currentWindow.size() > this->windowSize) {

        // We will calculate the offset, i.e., how many characters we have expanded.
        // We need to shift the window "offset" times.

        // Example: In the word "cortina", "corti" is the current window (currentWindowSize 5), and the windowSize is 3.
        // We need to shift the window 3 times (5-3+1), so we can have "ti" as the current window.
        // After, we will expand the window again, and we will have "tin" as the current window.

        // Shifting the window, in practice, eliminates the first character of the window and moves the other ones one
        // position to the left.
        int offsetBetweenWindows = (int) this->currentWindow.size() - this->windowSize + 1;

        for (int i = 0; i < offsetBetweenWindows; i++) {
            shiftWindow();
        }

        return this->expand();

    } else { // The window is not expanded yet.

        // It is the first window.
        if (this->currentWindow.empty()) {
            for (int i = 0; i < this->windowSize; i++)
                if (!this->expand())
                    return false;
            return true;
        } else {
            // We need to shift the window and read a new character.
            shiftWindow();
            return this->expand();
        }

    }

}

bool CopyModelReader::expand() {

    if (!Reader::getFileInputStream()->is_open()) {
        std::cerr << "[CopyModelReader::expand] File is not open." << std::endl;
        return false;
    }

    if (Reader::getFileInputStream()->eof()) {
        std::cerr << "[CopyModelReader::expand] File is at the end." << std::endl;
        return false;
    }

    if (Reader::getFileInputStream()->fail()) {
        std::cerr << "[CopyModelReader::expand] File failed to read." << std::endl;
        return false;
    }

    std::string nextCharacter = Reader::readCharacter();
    this->currentWindow.push_back(nextCharacter);

    this->currentPosition++;

    return true;

}

void CopyModelReader::shiftWindow() {

    for (int i = 0; i < this->currentWindow.size()-1; i++)
        this->currentWindow[i] = this->currentWindow[i + 1];
    this->currentWindow.pop_back();

}

int CopyModelReader::getWindowSize() const {
    return this->windowSize;
}

std::vector<std::string> CopyModelReader::getCurrentWindow() {
    return this->currentWindow;
}

int CopyModelReader::getCurrentPosition() const {
    return this->currentPosition;
}

int CopyModelReader::getPastPosition() const {
    return this->pastPosition;
}


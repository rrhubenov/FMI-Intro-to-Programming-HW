#include <iostream>

const char GOOD_SYMBOL = 'o';
const char EMPTY_SYMBOL = '_';
const char DUPLICATE_SYMBOL = '+';
const char CONFLICT_SYMBOL = 'x';

const unsigned short MAX_PROJECTORS_COUNT = 64;

char channels[512] = { [0 ... 511] = EMPTY_SYMBOL };

//Stores all the projectors as a 2D array. [0] - serial number, [1] - start channel, [2] - end channel
unsigned short projectors[MAX_PROJECTORS_COUNT][3];

unsigned short number_of_projectors = 0;

bool isSerialNumberValid(unsigned short serial_number) {
    return true;
}

void updateChannelIntervalWithSymbol(unsigned short start_channel, unsigned short end_channel, char symbol) {
    for(int i = start_channel - 1; i < end_channel - 1; i++) {
        channels[i] = symbol;
    }
}

//Checks if a channel is between two other channels
bool isChannelInner(unsigned short checked_channel, unsigned short start_channel, unsigned short end_channel) {
    return start_channel < checked_channel && checked_channel < end_channel;
}

void addProjectorChannels(unsigned short start_channel, unsigned short end_channel) {
    updateChannelIntervalWithSymbol(start_channel, end_channel, GOOD_SYMBOL);

    //Check if duplication exists and update channel symbols
    for(int i = 0; i < number_of_projectors; i++) {
        if(projectors[i][1] == start_channel && projectors[i][2] == end_channel) {
            updateChannelIntervalWithSymbol(start_channel, end_channel, DUPLICATE_SYMBOL);
            break;
        }
    }

    //Check if a conflict exists with any of the other projectors and update channel symbols
    for(int i = 0; i < number_of_projectors; i++) {
        //TODO: Validate the math
        if(isChannelInner(start_channel, projectors[i][1], projectors[i][2])) {
            if(projectors[i][2] < end_channel) { 
                updateChannelIntervalWithSymbol(
                    start_channel,
                    projectors[i][2],
                    CONFLICT_SYMBOL
                );
            }
            else updateChannelIntervalWithSymbol(
                    start_channel,
                    end_channel,
                    CONFLICT_SYMBOL
                 );
        } else if (start_channel < projectors[i][1] && isChannelInner(end_channel, projectors[i][1], projectors[i][2])) {
            updateChannelIntervalWithSymbol(projectors[i][1], end_channel, CONFLICT_SYMBOL); 
        }
    }
}

void addProjector(unsigned short serial_number, unsigned short start_channel, unsigned short number_of_channels) {
    unsigned short end_channel = start_channel + number_of_channels;

    addProjectorChannels(start_channel, end_channel);

    projectors[number_of_projectors][0] = serial_number;
    projectors[number_of_projectors][1] = start_channel;
    projectors[number_of_projectors][2] = end_channel;

    number_of_projectors++;
}

void removeProjector(unsigned short serial_number) {
    
}

void nextCommand(char &command) {
    std::cout << "Enter new command: ";
    std::cin >> command;
}

bool projectorExists(unsigned short serial_number) {
    for(int i = 0; i < number_of_projectors; i++) {
        if(projectors[i][0] == serial_number) return true;
    }
    return false;
}

int main() {
    char command;
    nextCommand(command);

    while(command != 'e') {

        unsigned short serial_number;
        unsigned short number_of_channels;
        unsigned short start_channel;

        switch(command) {
            case 'a':
                 if(number_of_projectors >= 64) {
                    std::cout << "ERROR: Maximum amount of projectors reached." << std::endl;
                    break;
                 }

                std::cin >> serial_number >> number_of_channels >> start_channel;

                if(start_channel + number_of_channels < 1 ||
                    start_channel + number_of_channels > 512) {
                    std::cout << "INPUT ERROR: Channels outside of allowed interval [1;512]." << std::endl;
                    break;
                }

                if(!isSerialNumberValid(serial_number)) {
                    std::cout << "INPUT ERROR: Serial number does not comply." << std::endl;
                    break;
                }

                addProjector(serial_number, start_channel, number_of_channels);

                break;

            case 'r':
                std::cin >> serial_number;

                if(projectorExists(serial_number)) {
                    removeProjector(serial_number);
                } else {
                    std::cout << "ERROR: No projector matching the serial number was found." << std::endl;
                    break;
                }

            case 'p':
                for(int i = 0; i < 512; i++) {
                    std::cout << channels[i];
                }
                std::cout << std::endl;
                break;
       }

        nextCommand(command);
    }
    
    return 0;
}

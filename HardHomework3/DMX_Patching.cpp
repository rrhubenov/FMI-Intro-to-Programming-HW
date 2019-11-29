#include <iostream>

const char GOOD_SYMBOL = 'o';
const char EMPTY_SYMBOL = '_';
const char DUPLICATE_SYMBOL = '+';
const char CONFLICT_SYMBOL = 'x';

const unsigned short MAX_PROJECTORS_COUNT = 64;

const unsigned short CHANNEL_COUNT = 512;

//Stores all the projectors as a 2D array. [0] - serial number, [1] - start channel, [2] - end channel
unsigned short projectors[1024][3];

unsigned short number_of_projectors = 0;

//Print error functions.
void printIntervalError();
void printNoMatchingSerialNumberError();
void printSerialNumberDoesNotComplyError();
void printMaximumAmountError();
void printExistingProjectorError();

//Projector actions.
void addProjector(unsigned short serial_number, unsigned short start_channel, unsigned short number_of_channels);
void removeProjector(unsigned short serial_number);
void moveProjector(unsigned short serial_number, unsigned short new_start_channel);
void printChannels();

//Projector helper functions.
short getProjectorIndex(unsigned short serial_number);
short getProjectorChannelLength(unsigned short serial_number);
bool projectorExists(unsigned short serial_number);

//Projector serial number validation.
unsigned short countSetBits(unsigned short serial_number);
bool isSerialNumberValid(unsigned short serial_number);

//Channel actions and helper functions.
void updateChannelIntervalWithSymbol(char (&channels_array)[CHANNEL_COUNT], unsigned short start_channel, unsigned short end_channel, char symbol);
bool isChannelInner(unsigned short checked_channel, unsigned short start_channel, unsigned short end_channel);
bool isChannelsInInterval(unsigned short start_channel, unsigned short end_channel);

//Misc.
void nextCommand(char &command);

int main() {
    char command;
    nextCommand(command);

    while(command != 'e') {

        unsigned short serial_number;
        unsigned short number_of_channels;
        unsigned short start_channel;

        unsigned short end_channel;

        switch(command) {
            case 'a':
                 if(number_of_projectors >= MAX_PROJECTORS_COUNT) {
                    printMaximumAmountError();
                    break;
                 }

                std::cin >> serial_number >> number_of_channels >> start_channel;

                if(projectorExists(serial_number)) {
                    printExistingProjectorError();
                    break;
                }

                end_channel = start_channel + number_of_projectors;

                if(!isChannelsInInterval(start_channel, end_channel)) {
                    printIntervalError();
                    break;
                }

                if(!isSerialNumberValid(serial_number)) {
                    printSerialNumberDoesNotComplyError();
                    break;
                }

                addProjector(serial_number, start_channel, number_of_channels);

                break;

            case 'm':
                std::cin >> serial_number >> start_channel;

                if(!projectorExists(serial_number)) {
                    printNoMatchingSerialNumberError();
                    break;
                }

                end_channel = getProjectorChannelLength(serial_number) + start_channel;

                if(!isChannelsInInterval(start_channel, end_channel)) {
                    printIntervalError();
                    break;
                }

                moveProjector(serial_number, start_channel);

                break;

            case 'r':
                std::cin >> serial_number;

                if(projectorExists(serial_number)) {
                    removeProjector(serial_number);
                } else {
                    printNoMatchingSerialNumberError();
                }

                break;

            case 'p':
                printChannels();
                break;
       }

        nextCommand(command);
    }
    
    return 0;
}

void printSerialNumberDoesNotComplyError() {
    std::cout << "INPUT ERROR: Serial number does not comply." << std::endl;
}

void printMaximumAmountError() {
     std::cout << "ERROR: Maximum amount of projectors reached." << std::endl;
}

void printIntervalError() {
    std::cout << "INPUT ERROR: Channels outside of allowed interval [1;" << CHANNEL_COUNT << "]." << std::endl;
}

void printNoMatchingSerialNumberError() {
    std::cout << "ERROR: No projector matching the serial number was found."  << std::endl;
}

void printExistingProjectorError() {
    std::cout << "ERROR: A projector with this serial number already exists." << std::endl;
}

unsigned short countSetBits(unsigned short serial_number) 
{ 
    unsigned short count = 0; 
    while (serial_number) { 
        count += serial_number & 1; 
        serial_number >>= 1; 
    } 
    return count; 
} 

bool isSerialNumberValid(unsigned short serial_number) {
    if(serial_number % countSetBits(serial_number) == 0) return true;
    else return false;
}

short getProjectorIndex(unsigned short serial_number) {
    for(int i = 0; i < number_of_projectors; i++) {
        if(projectors[i][0] == serial_number) return i;
    }
    return -1;
}

short getProjectorChannelLength(unsigned short serial_number) {
    for(int i = 0; i < number_of_projectors; i++) {
        if(projectors[i][0] == serial_number) return projectors[i][2] - projectors[i][1];
    }
    return -1;
}

void updateChannelIntervalWithSymbol(char (&channels_array)[CHANNEL_COUNT], unsigned short start_channel, unsigned short end_channel, char symbol) {
    for(int i = start_channel - 1; i < end_channel - 1; i++) {
        channels_array[i] = symbol;
    }
}

//Checks if a channel is between two other channels
bool isChannelInner(unsigned short checked_channel, unsigned short start_channel, unsigned short end_channel) {
    return start_channel < checked_channel && checked_channel < end_channel;
}

bool isChannelsInInterval(unsigned short start_channel, unsigned short end_channel) {
    if(start_channel >= 1 && start_channel <= CHANNEL_COUNT
        && end_channel >= 1 && end_channel <= CHANNEL_COUNT) return true;
    else return false;
}

void addProjector(unsigned short serial_number, unsigned short start_channel, unsigned short number_of_channels) {
    unsigned short end_channel = start_channel + number_of_channels;

    projectors[number_of_projectors][0] = serial_number;
    projectors[number_of_projectors][1] = start_channel;
    projectors[number_of_projectors][2] = end_channel;

    number_of_projectors++;
}

//Shifts every projector element to the right of the desired one. Ex. {1, 2, 3, 4, 5} remove on index 2 => {1, 2, 4, 5, 5}
void removeProjector(unsigned short serial_number) {
    short index = getProjectorIndex(serial_number);
    for(int i = index; i < number_of_projectors; ++i) {
        projectors[i][0] = projectors[i+1][0];
        projectors[i][1] = projectors[i+1][1];
        projectors[i][2] = projectors[i+1][2];
    }
    number_of_projectors--;
}
 

void moveProjector(unsigned short serial_number, unsigned short new_start_channel) {
    for(int i = 0; i < number_of_projectors; i++) {
        if(projectors[i][0] == serial_number) {
            projectors[i][2] = new_start_channel + projectors[i][2] - projectors[i][1];
            projectors[i][1] = new_start_channel;
            break;
        }
    }
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

void printChannels() {
    char channels[CHANNEL_COUNT] = { [0 ... CHANNEL_COUNT - 1] = EMPTY_SYMBOL };


    //Iterate trough all the projectors
    for(int i = 0; i < number_of_projectors; i++) {
        updateChannelIntervalWithSymbol(channels, projectors[i][1], projectors[i][2], GOOD_SYMBOL);

        //Check if duplication exists and update channel symbols
        for(int j = 0; j < number_of_projectors; j++) {
            if(j == i) continue;
            if(projectors[i][1] == projectors[j][1] && projectors[i][2] == projectors[j][2]) {
                updateChannelIntervalWithSymbol(channels, projectors[i][1], projectors[i][2], DUPLICATE_SYMBOL);
                break;
            }
        }

        //Check if a conflict exists with any of the other projectors and update channel symbols
        for(int j = 0; j < number_of_projectors; j++) {
            if(j == i) continue;
            //Check if the start channel is between the start and end channel of the other projector
            if(isChannelInner(projectors[i][1], projectors[j][1], projectors[j][2])) {
                //Check if the end channel is outside of the start and end channel of the other projector
                if(projectors[j][2] < projectors[i][2]) { 
                    updateChannelIntervalWithSymbol(
                        channels,
                        projectors[i][1],
                        projectors[j][2],
                        CONFLICT_SYMBOL
                    );
                }
                //Else the whole projector is inside the start and end channel of the other projector
                else updateChannelIntervalWithSymbol(
                        channels,
                        projectors[i][1],
                        projectors[i][2],
                        CONFLICT_SYMBOL
                    );
            //Check if the end channel is between the start and end channel of the other projector  
            } else if (projectors[i][1] < projectors[j][1] && isChannelInner(projectors[i][2], projectors[j][1], projectors[j][2])) {
                updateChannelIntervalWithSymbol(channels, projectors[j][1], projectors[i][2], CONFLICT_SYMBOL); 
            }
        }
    }

    for(int i = 0; i < CHANNEL_COUNT; i++) {
        std::cout << channels[i];
    }
    std::cout << std::endl;
}

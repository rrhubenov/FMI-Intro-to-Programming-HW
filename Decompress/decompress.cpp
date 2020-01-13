#include <iostream>
#include <cstring>

void decompress(char* data) {
	if(strlen(data) == 0) {
		return;
	}
	else if(data[0] < 48 || data[0] > 57) {
		std::cout << data[0];
		decompress(++data);
	}
	else {
		int digit_place = 1;
		int repeat_count = 0;
		while(data[0] >= 48 && data[0] <= 57) {
            if(data[0] == '0') repeat_count *= 10;
            else repeat_count += (data[0] - '0')*digit_place;

			digit_place *= 10;
			data++;
		}
		if(data[0] == '(') {
			int left_bracket_count = 1;
			char* bracket_iterator = data;
            int dummy;
            
			while(strchr(bracket_iterator+1, '(') != NULL || strchr(bracket_iterator+1, '(') < strchr(bracket_iterator+1, ')')) {
				left_bracket_count++;
			    bracket_iterator = strchr(bracket_iterator+1, '(');
			}

			for(int i = 0; i < left_bracket_count; i++) {
				bracket_iterator = strchr(bracket_iterator+1, ')');
			}
			
            char* repeat_string;
            strncpy(repeat_string, data + 1, bracket_iterator - data - 1);
            repeat_string[bracket_iterator - data - 1] = '\0';

			for(int i = 0; i < repeat_count; i++) {
				decompress(repeat_string); 
			}
            decompress(bracket_iterator+1);
		}
	}
}

int main() {
	char* data;

	std::cin >> data;
		
    data[strlen(data)] = '\0';

    decompress(data);

    //if(strchr(data, '(') == NULL) std::cout << "ARE BE" << std::endl;

	return 0;
}

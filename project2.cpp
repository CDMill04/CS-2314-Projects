#include <string>
#include <iostream>
using namespace std;

class Chip {
private:
char chipType; // Type of the chip (A: Addition, S: Subtraction, etc.)
std::string id; // Unique identifier for the chip
Chip* input1; // Pointer to the first input chip
Chip* input2; // Pointer to the second input chip (can be NULL)
Chip* output; // Ptr to the output chip (is NULL for output chips)
double inputValue; //for the input chip
bool visted; //This variable is to check if our compute() function is repeating itself

public:
// Constructor
Chip(char type, const string& id);
// Method prototypes
void setInput1(Chip* inputChip); // Sets the first input chip
void setInput2(Chip* inputChip); // second input chip (can be NULL)
void setOutput(Chip* outputChip); // Sets the output chip (can be NULL)
double compute(); // Performs the operation based on the chip type
void display() const; // Displays the chip's information
//example: I6, Output = S600 --- for the input Chip
//example: O50, Input 1 = S600 --- for the output Chip
//example: A100, Input 1 = I1, Input 2 = I2, Output = M300
string getId() const; // Returns the chip ID
//****** OTHER METHODS AS NECESSARY ************//
Chip() : visted(false) {}
//Put getters and setters here
char getChipType() const;
Chip* getInput1() const;
Chip* getInput2() const;
Chip* getOutput() const;
double getInputValue() const;

void setChipType(char x);
void setID(string x);
void setInputValue(double x);
};

char Chip::getChipType() const { //Most of the following methods are standard getters and setters
    return chipType;
}

string Chip::getId() const {
    return id;
}

Chip* Chip::getInput1() const {
    return input1;
}

Chip* Chip::getInput2() const {
    return input2;
}

Chip* Chip::getOutput() const {
    return output;
}

double Chip::getInputValue() const {
    return inputValue;
}

void Chip::setChipType(char x) {
    chipType = x;
}

void Chip::setID(string x) {
    id = x;
}

void Chip::setInput1(Chip* inputChip) {
        input1 = inputChip;  
}

void Chip::setInput2(Chip* inputChip) {
    input2 = inputChip;
}

void Chip::setOutput(Chip* outputChip) {
    output = outputChip;
}

void Chip::setInputValue(double x) {
    inputValue = x;
}

Chip::Chip(char type, const string& id) { //Constructor for Chip
    chipType = type;
    setID(id);
    inputValue = 0;
    input1 = 0;
    input2 = nullptr;
    output = nullptr;
}

double Chip::compute() { //This function needs to recursively read in the inputs and find out what they output
if (visted == true) {
    return inputValue;
}
visted = true;
if (getChipType() == 'I') { //This is the base case
    return inputValue;
}

double inputVal1 = 0;
double inputVal2 = 0;

if (getInput1() != nullptr) { //Grab the input or inputs, if there are two
    inputVal1 = getInput1() -> compute();
}

if (getInput2() != nullptr) {
    inputVal2 = getInput2() -> compute();
}

//Now this is where chip type really comes into play
//These look at the type of chip and perform an operation accordingly
if (getChipType() == 'A') {
    inputValue = inputVal1 + inputVal2;

} else if (getChipType() == 'M') {
    inputValue = inputVal1 * inputVal2;

} else if (getChipType() == 'S') {
    inputValue = inputVal1 - inputVal2;

} else if (getChipType() == 'D') {
    inputValue = inputVal1 / inputVal2;

} else if (getChipType() == 'N') {
    inputValue = -inputVal1;

} else if (getChipType() == 'O') {
    inputValue = inputVal1;

}
return inputValue; //Returns the values back to the function and continues until we reach base case
}

void Chip::display() const { //This function needs to show the operations
if (getChipType() == 'O') {
    std::cout << getId() << ", Input 1 = " << getInput1() -> getId() << endl; //This will make sure that O50 displays properly

} else if (getInput1() == nullptr) { //This loop mostly handles I's
        std::cout << getId() << ", Output = " << (getOutput() -> getId()) << endl;

    } else if (getOutput() != nullptr) { //This loop must be able to put none if there is nothing in input2
        std::cout << getId() << ", Input 1 = " << getInput1() -> getId() << ", Input 2 = "
                  << (getInput2() ? getInput2() -> getId() : "None")
                  << ", Output = " << (getOutput() -> getId()) << endl;

    } else if (getInput2() == nullptr) { //Same here
        std::cout << getId() << ", Input 1 = " << getInput1() -> getId() << ", Input 2 = None, Output = "
                  << (getOutput() -> getId()) << endl;

    }
}

double convertString(const std::string& s) { //Had to build this to convert stings to doubles
    double result = 0.0;
    double decimalFactor = 1.0;
    bool isFraction = false;

    size_t i = 0; //Wish we could update headers

     
    for (; i < s.length(); ++i) { //This iterates through the string
        char c = s[i];

        if (c == '.') { //Checks to see if it is a whole number or not
            isFraction = true; 
            continue;
        }
        
        if (c >= '0' && c <= '9') { //If it is a whole number, it converts it here
            if (!isFraction) {
                result = result * 10 + (c - '0');
            } else { //If not, it does so here
                decimalFactor /= 10;
                result += (c - '0') * decimalFactor;
            }
        }
    }
    return result;
}

int main () {
//**** ALL THE VARIABLES YOU NEED FOR THIS MAIN FUNCTION *****//
//Had to add quite a few new variables.
int numChips;
int chipNumber = 0;
int chipIDNumLoops = 0;
int opIDNumLoops = 0;
double outputNumber = 0.0;
string oldChipID = "";
string oldOperatorID = "";
double result = 0;
bool hasOp = false; //To see if the operator is present in the main's large for-loop
Chip** allChips;
int numCommands;
std::cin >> numChips;
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Had to include this to move to next line.


allChips = new Chip*[numChips];

for (int i=0; i < numChips+1; i++) { //This for loop will read in all of the chips and create their places in allChips
    std::string line;
    

    if(std::getline(std::cin, line)) { //This must iterate through the first lines
        
       if (i < numChips) { //Ensures that it will not exceed the limit of chips

        char c = line[0];

            if (chipNumber < numChips) { //Creates chips and keeps track of the number
              allChips[chipNumber] = new Chip(c, line);
              chipNumber++;
            } else {
                break;
            }
            //std::cout << line << endl;
        } else { //I had to move reading the numCommands up here. It was having trouble
            numCommands = convertString(line);
            break;
        }
    }
}

//std::cout << numCommands << endl; I nearly forgot to comment this debugging code out
/*std::cout << numChips << endl;
for (int i = 0; i < numChips; i++) {
    std::cout << (allChips[i] -> getId()) << endl;
} */

for (int i=0; i < numCommands; i++) { //This is the for loop that deals with operations
    std::string line;

    while (std::getline(std::cin, line)) {
        std::string identifier, chipID, operation; //This code breaks apart the lines fed
        std::double_t inputNumber;
        size_t spacePosition = 0;

        spacePosition = line.find(' '); //This gets the idendifying letter
        identifier = line.substr(0, spacePosition); 
        line.erase(0, spacePosition + 1);

        spacePosition = line.find(' '); //This part grabs the ChipID
        if (!line.empty() && spacePosition != std::string::npos) {
            chipID = line.substr(0, spacePosition);
            line.erase(0, spacePosition + 1);
        } else { //If there is nothing else after this line, it will just make the chipID the line
            chipID = line; 
        }

        if (!line.empty()) { //Only executes if there is line left
            if (identifier != "I") {
                operation = line;
                hasOp = true;
            } else {
                inputNumber = convertString(line); //This converts the value to a double for the I chips
            }
        }

        for (int i = 0; i < numChips; i++) {
            if (chipID == allChips[i]->getId()) {

                if (identifier == "I") { //This part sets the value for I chips
                    allChips[i]->setInputValue(inputNumber);

                } else if (hasOp) { //If it is not an I chip, it adds inputs and outputs
                    for (int j = 0; j < numChips; j++) {
                        if (allChips[j] -> getId() == operation) {

                            if (allChips[j]->getInput1() == nullptr) {
                                allChips[j]->setInput1(allChips[i]);  //It will check if the first is empty before it adds to input2

                            } else {
                                allChips[j]->setInput2(allChips[i]); 
                            }

                            allChips[i]->setOutput(allChips[j]); //Finally, we track the output

                            if (identifier == "O") {
                                result = allChips[i] -> compute();
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}
std::cout << "Computation Starts" << endl;

//Now, we grab the inputValue from O50 that we stored when we computed the value, and we output the connections
std::cout << "The output value from this circuit is " << result << endl; 
std::cout << "***** Showing the connections that were established" << endl;

//Creates output using display. Display itself will give output
for (int i = 0; i < numChips; i++) {
    if (allChips[i] -> getChipType() != 'O') {
        allChips[i] -> display();
    }
}

//This just makes sure that the O50 line appears last
for (int i = 0; i < numChips; i++) {
    if (allChips[i] -> getChipType() == 'O') {
        allChips[i] -> display();
    }
}

for (int i = 0; i < chipNumber; i++) { //Deletes each Chip inside allChips, then allChips itself
    delete allChips[i];
}
delete[] allChips;

return 0;
}

/* DOCUMENTATION

    LLM Usage: Prompts and Rationale

How can Iterate line by line through a redirected input given that it is a mix of numbers and letters in C++?
Rationale: In Java, I would usually use other libraries and methods to iterate through a line to get the parts I wanted. 
I had no idea how to do this in C++. This will help me greatly in future projects.

I can only have #include iostream and string
Rationale: The ideas it gave me all had different headers, and I had to make sure that I could only use these.

is getLine predefined?
Rationale: This was throwing a strange error. I had to check to make sure I did not have to implement it. Let this be a lesson 
of the occasional questionability of LLM.

How can I name my objects accordingly using loops like the ones discussed before, given that I want to name objects after each line?
Rationale: I needed ideas on how to dynamically name objects, since I am still new to C++. The method I ended up using 
in this project is a more general method, so I will be able to use it for future preference.

How can I compare a string to another string in C++?
Rationale: I have not worked with string in C++ before, and I have to use .equals() in Java. Its a simple question, but one I 
needed answered nonetheless.

So why does Java use .equals() instead?
Rationale: I was curious. I wanted to know the difference in data structures and references between Java and C++.

How can I convert String to doubles in C++
Rationale: I needed some way to get decimals from string inputs, and as previously mentioned, I am used to Java, not C++. This
was enlightening for future reference.

How can I be sure that, when reading in lines, I pass to the next line after I run out of characters?
Rationale: I was having trouble with the cin >> variable lines not passing the newline properly, so I needed a method to force 
it past it. I'll be sure to use this in the future.

for cin >> variable; how can I make sure that it goes to the next line? 
Rationale: A more specific question than the one asked previously.

Given a class that can hold inputs and outputs, and given the methods of reading and breaking up a string you have showed me, why does mine skip the second part of the sentence?
Rationale: I was having more trouble reading lines in, and at this point I was debugging a little. The problem was a 
mistake in hwo I was breaking up lines. I have to pay more attention to that.

Why is this throwing an error?
if (getChipType() == "O") {

}
Rationale: I was using "", when I should have been using ''. I did not remember the difference. I never used this method and 
ended up deleting it.

If I have an array of objects, how can I call a .compute() method on an individual object in that array?
Rationale: I needed an idea on how to start the compute() method. I never ended up using this idea anyway, and the method I 
have now is an evolution of an original recursive design I came up with.

How can I call the size of an array outside of the main method?
Rationale: I was throwing several ideas at the wall, and one involved the size of allChips, but outside the main method, so 
I needed to know how to call it if I could.

So how could I write recursive code calling my compute() over and over again
Rationale: I just needed a jumpstart on ideas on how to actually implement my idea. I was proud of how it turned out. I can 
code recursively a little better I suppose.

it says it cannot assign double from void on my compute() method
Rationale: I had an error I could not place. I ended up changing the return type from void to double.

My compute method might work, but right now it is just looping endlessly. It doesnt seem to ever break
Rationale: Again, I just needed some ideas for a bug fix. 

Why do I get this warning?
 warning: default member initializer for non-static data member is a C++11 extension [-Wc++11-extensions]
bool visted = false; //This variable is to check if our compute() function is repeating itself
            ^
1 warning generated.
Rationale: At this point I was deep into debugging. This warning confused me, and I got to learn about the difference between 
C++11 and older versions.

How can I make sure that an input with a unique identifier is the last line of output?
Rationale: This was the last bug I had to fix. I was so annoyed by this bug in that only appeared in the third input, and I had 
to make sure it could never recure.

    Incremental Develpment:

The ways I used AI to help me in constructing my project and the lessons I learned are mostly documented in the Rationale, but I
will give an overview. In all, whenever I needed ideas for methods or a bug fix to something that was eluding me, I would ask AI
for advice and assitance in how to solve the issue. I also used it for when I plain did not know how to implement something. When
I was debugging and my plans failed, I would see what AI had to say about it. All in all, AI helped me in increasing my knowledge
of the inner workings of C++, steps on how to debug certain issues, and in giving me ideas when I was fresh out.

    Debugging and Testing Plan:

Tests: The tests I used in debugging my code were usually along the lines of attempting to see where the code entered and where
it didnt by used of cout << something << endl;. I had these structured throughout different loops and followed the output to see
what I had to fix. A few fixes involved new variables to try and track the values of certain objects, or new ways of performing
a function. The latter usually did not help much, and I think I only ended up adding one new variable that actually helped.

Issues and Resolutions: Aside from the standard, occasional syntax and property usage errors, I had a few frustrating bugs. I
ended up solving an issue where my inputs for the number of chips and commands were not inputting correctly by rewriting aspects
of the loops and adding numCommands to part of a loop. I also had to fix some loops that prevented my inputs and outputs from
inputting correctly. They would loop multiple times and not record. It was very frustrating. Another issue I had was with the
display method. There were actually many issues with this method, but it was mostly with how I read in different chips, and 
revisiting and updating some of my code, as well as tracking the inputs solved this one in the end. The last memorable bug(s)
concerned my compute method, but beyond a few bugs with endless looping that were fixed by adding a variable that tracked and 
did not allow the method to loop several times per chip, it was actually much easier to fix than I thought. There was also a
problem with the negation chip because I was stupid and did not apply the operation to the proper input.

Verification: I did use output statements and different variables to verify that my classes and methods were working properly,
but the most consistant test was just running the program and seeing if it gave me the output I wanted. That was the extent of
the verification I used. It was certainly lighter that other sections.




*/



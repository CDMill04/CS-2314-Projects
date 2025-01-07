#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

int main () { 

const int MAX_TOKENS = 10000; //Create an array some size that will not be exceeded
string originalOrder[MAX_TOKENS]; //This array will hold things as they are read in
int originalCount = 0; //Time to count tokens

string token;
unordered_map<string, int> myTokens; //This keeps track of token frequency
unordered_map<string, int> tokenPositions; //This keeps trap of token positions
map<pair<int, string>, string> sortedTokens; //This keeps track of sorted tokens

while (cin >> token) { //Grab frequency
        //Record if the token has been seen before. If it was, token count goes up
    if (myTokens.find(token) == myTokens.end() && originalCount < MAX_TOKENS) {
        originalOrder[originalCount++] = token; 
    }
    myTokens[token]++;
}

//Sort by frequency
for (unordered_map<string, int>::iterator it = myTokens.begin(); it != myTokens.end(); ++it) {
    sortedTokens[pair<int, string>(-it -> second, it -> first)] = it -> first;
}

//Assign sorted positions to tokens
int position = 1;
for (map<pair<int, string>, string>::iterator it = sortedTokens.begin(); it != sortedTokens.end(); ++it) {
    tokenPositions[it -> second] = position++;
    cout << it -> second << " ";
}
cout << "\n**********" << endl; //Needed a newline character to separate the output

//Get original order of tokens and output their locations
for (int i = 0; i < originalCount; ++i) {

    unordered_map<string, int>::iterator posIt = tokenPositions.find(originalOrder[i]); //Iterate though and output
    if (posIt != tokenPositions.end()) {
        cout << posIt -> second << " ";
    }
}

cout << endl; //To avoid a % sign in the output
return 0;
}

/* DOCUMENTATION

    LLM Usage: Prompts and Rationale

How can I sort an unorder_map by a value in decreasing order?
Rationale: I needed a refresher on how to use maps and unordered_maps. I had used them a little in Java, but not here.
I can see the value in using a map, and I'll be sure to implement it more often moving foward.

How can I fix the way that a map sorts its keys
Rationale: I wanted to understand the map function a little better. This actually led me to use a multimap and a method
to sort the tokens the ways that I wanted to.

I just want map to store the values by their frequency, which is determined by myTokens[token]++. Will map do this 
automatically, or will I need to step in?
Rationale: This was another part of the need to sort the tokens properly. This led to my next question and a few
valuable lessons.

How can I write a custom comparator for a multimap that sorts tokens in decreasing order
Rationale: This is more of an extension of the previous question. I learned how to write a custome comparator, and I used
it for a little while. I will definitely keep that in mind for future projects.

What does importing the algorithm library do for me? What are its most important functions?
Rationale: This question is more self explanatory. I wanted to know what the algorithm library could do. It is chock full
of helpful methods, and it is certainly one to keep in mind.

If I define the multimap to use my custom sort, do I need to redefine my insert method?
Rationale: While the answer was no, it did lead to me finding out how what areas to change. Valuable things to know.

How can I make it so that any file I use can be opened and closed no matter what file I use?
Rationale: This one was unfortunate, because there is no way to actually do so with redirected input. You have to change
the filename manually. 

how do I get rid of the % at the end of the output?
Rationale: I wanted to make sure that the output was clean. Those pesky extra characters annoy me.

How can I begin to edit a file that takes ifstream myInputFile(filename) and turn in into cin >> tokens
Rationale: I needed somewhere to start with this conversion, as it is a little different than what I am used to.
Turns out I was overthinking it, and I should really keep that in mind for the future. Either way it helped me rebuild
the input.

Is there a way to grab both an unordered_map's frequency of tokens and the index positions of them in one go?
Rationale: I really did not know how to go about doing this. This is where I got the max array idea, and it quickly
turned into a solution.

    Incremental Development:

I used my tools like AI and notes less this time, as this was a smaller project. Still, they helped to explain some 
more complicated or unknown aspects of the project to me, such as the map, unordered_map, and algorithm libraries. AI 
also helped me to understand when and how to write a custom comparator for a multimap and map, as well as how to
integrate it. The problems I faced this time were mostly about the indexes of where the tokens were stored. They were
showing, but not being stored correctly. It was also a little difficult to switch from inputting a filename to reading
one in, but again, some AI and some reference to old projects helped. AI helped with that and with the synatx/general 
understanding of the code. Trial and error once again got me through in the end.

    Debugging and Testing plan:

I actually did not have a lot of bugs in this program. I had some syntax errors, but it was nothing copilot couldn't
get me past. I did not even use the tried and try method of printing statements for different parts of the code that much.
I printed a little to make sure that the file was opening and closing correctly, but that was about it. The problems I
encountered were more of my output being incorrect, and so my testing plan was trying out different cobinations of my
custom comparator, the default ones, map, unordered_map, and multimap to see what I could get to work. This project was
a nice change of pace from the others, and it was a bit of a relief to have few bugs.

Issues and Resolutions:
The issues I encountered regarding the indexes of tokens were somewhat annoying, because my program ran, and it was
outputting what I wanted, it just was not storing the indexes correctly. I had to go back and change the way that I was
storing the tokens, and as I have said, that mostly just involved trying different combinations until I arrived at a 
solution that worked. Of course, then was the matter of switching how I inputted the info, but that gave me few issues.
 A few syntax errors were present here and there, and I got a segmentation error once, but other than these few issues, 
 I had a somewhat smooth time with this project.

Verification:
I built the program in the main project5.cpp file, then ran it and tried a few smaller fixes before I decided to 
larger fixes were needed and to use a separate file to implement them there for testing. This was were I tried all of
my different combinations of maps and comparators. Once I had that figured out, I moved the working code back to the
program and ran it with all the different inputs to make sure I was getting the outputs I needed. Altogether, I had a
very simple yet effetive method of verification for this project, and I believe that it was a good way to go about it.
*/
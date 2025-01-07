#include <iostream>
#include <vector>
using namespace std;

class duplicateInsertion : public exception { //These are my exception classes
    public:
    const char* what() const throw() { //Both have simple what() functions
        return "Duplicate Insertion";
    }
};

class NotFoundException : public exception { //These are not very complicated, just useful for debugging
    public:
    const char* what() const throw() {
        return "Not Found";
    }
};
/***** Write your exception class here ******* SEE main function *****/

template <class DT>
class MTree {
protected:
int M; // Maximum number of children per node (M+1 way split)
vector<DT> values; // Values stored in the node (M-1 values)
vector<MTree*> children; // Pointers to child MTrees (M+1 children)
MTree* parent; // Pointer to the parent
public:
MTree(int M);
~MTree();
bool is_leaf() const; // Check if the current node is a leaf
void insert(const DT& value); // Insert a value into the MTree
void split_node(); // Split the node if it exceeds capacity (i.e >=M)
MTree* find_child(const DT& value); // Find the correct child to follow
bool search(const DT& value); // Search for a value in the MTree
void remove(const DT& value); // Delete a value from the MTree
void buildTree(const vector<DT>& input_values); // Build the tree
vector<DT> collect_values(); // Collect values from all leaf nodes
bool find (DT& value);
//My functions
int binarySearch(const std::vector<DT>& tree, int value); //Binary search is always super useful
void sortIt(std::vector<DT>& sortTime); //This is a helper function for collect_values
void display(); //This one was intially used for debugging, but I repurposed it for the final list
//void displayHelper(int level, const std::string& nodeLabel) const; //A remenant of my debugging
void clear(); //Custom clear function to clear nodes and the tree
void insert_into_parent(const DT& value, MTree* leftChild, MTree* rightChild); //Helpful for insert and for splitting
};

template <class DT>
MTree<DT>::MTree(int M) { //This is the parameterized constructor
    this -> M = M;
    parent = nullptr;
}

template <class DT>
MTree<DT>::~MTree() { //A simple destructor
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

template <class DT> //Another simple method, but vital. Checks if the node is a leaf
bool MTree<DT>::is_leaf() const {
    return children.size() == 0;
}

template <class DT>
void MTree<DT>::insert(const DT& value) { //Here we go. Getting into the more complicated functions now
    int index = binarySearch(values, value);
    if (index != -1) { //Check to see if the value is already in the tree
        throw duplicateInsertion();
    }

    if (is_leaf()) { //If it's a leaf, insert the value
        if (values.size() < M - 1) {
            values.push_back(value);
            std::sort(values.begin(), values.end());

        } else { //Split the node if necessary and insert the value
            split_node();
            find_child(value) -> insert(value);
        }

    } else { //Otherwise, find the correct child and insert the value
        MTree* child = find_child(value);
        child -> insert(value);
        if (child -> values.size() >= M) {
            split_node();
        }
    } 
}

template <class DT>
void MTree<DT>::split_node() { //This one hurts. It's a big one

    //Ensure that the node is full before splitting. Pretty simple check
    if (values.size() < M - 1) {
        return;
    }

    int midIndex = (M - 1) / 2; // Find the middle
    DT medianValue = values[midIndex];

    //Create M new child nodes. This should be unaffected by the split, as the extra value was moved to the parent
    std::vector<MTree*> newChildren;
    for (int i = 0; i < M; ++i) {
        newChildren.push_back(new MTree(M));
    }

    //Distribute values among the new child nodes
    for (int i = 0; i < values.size(); ++i) {
        if (i < midIndex) {
            newChildren[0] -> values.push_back(values[i]);

        } else if (i > midIndex) {
            newChildren[1] -> values.push_back(values[i]);
        }
    }

    //If this node is not a leaf, distribute children as well. The more the merrier, I suppose
    if (!is_leaf()) {
        for (int i = 0; i <= midIndex; ++i) {
            newChildren[0] -> children.push_back(children[i]);
        }
        for (int i = midIndex + 1; i < children.size(); ++i) {
            newChildren[1] -> children.push_back(children[i]);
        }
    }

    //Clear current node's values and children, then add the middle value
    values.clear();
    values.push_back(medianValue);
    children.clear();
    children.push_back(newChildren[0]);
    children.push_back(newChildren[1]);

    //Now we get into a lot of the conditional cases
    //Assign the parent to the new nodes and promote the median if needed
    for (int i = 0; i < newChildren.size(); ++i) {
        newChildren[i] -> parent = this;
    }

    //If this node has a parent, insert the median into the parent
    if (parent != nullptr) {
        parent -> insert_into_parent(medianValue, newChildren[0], newChildren[1]);
    } else {
        //If this is the root, create a new root with the median
        MTree* newRoot = new MTree(M);
        newRoot -> values.push_back(medianValue);
        newRoot -> children.push_back(this);
        newRoot -> children.push_back(newChildren[1]);
        this -> parent = newRoot;
        newChildren[1] -> parent = newRoot;
    }
}

template <class DT>
void MTree<DT>::insert_into_parent(const DT& value, MTree* leftChild, MTree* rightChild) { //Helps out the splitting
    //Find the position to insert the value
    int index = 0;
    while (index < values.size() && values[index] < value) {
        ++index;
    }

    //Insert the value and update children and parents
    values.insert(values.begin() + index, value);
    children[index] = leftChild;
    children.insert(children.begin() + index + 1, rightChild);

    leftChild -> parent = this;
    rightChild -> parent = this;

    //If the parent node is full, split it
    if (values.size() >= M) {
        split_node();
    }
} 

template <class DT>
MTree<DT>* MTree<DT>::find_child(const DT& value) { //Another helpful method for insertion and splitting

    //Find the right kid
    for (int i = 0; i < values.size(); i++) {
        if (value < values[i]) {

            //If its there and doesnt send us out of bounds, return the child
            if (i < children.size()) {
                return children[i];
            } else {
                std::cerr << "Error: Attempted access to non-existent child index " << i 
                          << " for value " << value << std::endl;
                return nullptr;
            }
        }
    }

    //Grab the last child if the value is greater than all values
    int lastChildIndex = values.size();

    //Grab last child otherwise
    if (lastChildIndex < children.size()) {
        return children[lastChildIndex];
        //Check for out of bounds
    } else {
        std::cerr << "Error: Attempted access to non-existent last child index " << lastChildIndex 
                  << " for value " << value << std::endl;
        return nullptr;
    }
}

template <class DT>
bool MTree<DT>::search(const DT& value) { //Mostly just uses binarySearch to find the value
    //Check the current node for the value
    if (binarySearch(values, value) != -1) {
        return true;
    }

    //If it's a leaf, the value isn't there
    if (is_leaf()) {
        return false;
    }

    //Recursively search until we get where we want to go
    MTree* child = find_child(value);
    return child -> search(value);
}

template <class DT>
void MTree<DT>::remove(const DT& value) { //A complex function to remove values, but it is important
    vector<DT> allValues = this -> collect_values(); //Move the values to a vector

    //Look through the vector and remove the value
    typename vector<DT>::iterator it = std::find(allValues.begin(), allValues.end(), value);
    if (it != allValues.end()) {
        allValues.erase(it);

        //Get rid of duplicates
        std::sort(allValues.begin(), allValues.end());
        for (int i = 1; i < allValues.size(); ++i) {
            if (allValues[i] == allValues[i - 1]) {
                allValues.erase(allValues.begin() + i);
                --i; //Decrement so we do not skip values. It gets angry without this
            }
        }

        clear(); //Clear current node values and children
        buildTree(allValues); //Now, we can rebuild the tree
    } else {
        throw NotFoundException(); //Just in case
    }
}



template <class DT>
void MTree<DT>::clear() { //Clears the tree, very useful if you need to start from scratch
    //Clear the values in the current node
    values.clear();
    
    //Delete each child
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
        children[i] = nullptr;
    }

    //Now, clear the children
    children.clear();
}

template <class DT>
void MTree<DT>::buildTree(const std::vector<DT>& data) { //I know we were given a buildTree function, but I did not like it
    //As such, I built this new one. It takes advantage of the other methods I have built
    clear(); //Clear the tree

    //Insert each value from collect_values vector
    for (int i = 0; i < data.size(); i++) {
        insert(data[i]);
    } //A lot cleaner looking than the other one
}

template <class DT>
std::vector<DT> MTree<DT>::collect_values() { //This one outsources the collection, and sorts the collected values
    std::vector<DT> willBeSorted;
    sortIt(willBeSorted); //Collect the values

    std::sort(willBeSorted.begin(), willBeSorted.end()); //Sort them
    return willBeSorted;
}

template <class DT>
void MTree<DT>::sortIt(std::vector<DT>& sortTime) { //This is where the actual collection happens
    if (is_leaf()) { //If it's a leaf, just insert the values
        sortTime.insert(sortTime.end(), values.begin(), values.end());

    } else { //Otherwise, collect the values
        for (int i = 0; i < children.size(); i++) {
            children[i] -> sortIt(sortTime);
            if (i < values.size()) {
                sortTime.push_back(values[i]);
            }
        }
    }
}

template <class DT> //This one was a function we needed to implement, but I am not sure why
bool MTree<DT>::find(DT& value) { //It is just a wrapper for search
    return search(value);
}

template <class DT>
int MTree<DT>::binarySearch(const std::vector<DT>& tree, int value) { //Old reliable right here. You know how it works
    //std::cout << "Binary Search" << std::endl; //For debugging
    int left = 0;
    int right = tree.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (tree[mid] == value) {
            return mid;
        }
        if (tree[mid] < value) { //By the time I graduate, I'll probably have this seared in my brain
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

template <class DT>
void MTree<DT>::display() { //This one is just for outputting the final list
    //I already had this method signature, but I was not using it, so I used it for the final list
    vector<int> values = this -> collect_values();
    std::cout << "Final List: ";
    for (int i = 0; i < values.size(); i++) {
        cout << values[i] << " ";
    }
    cout << endl;
}


int main() {
int n; // number of numbers in the initial sorted array
int numTimesRun = 0;
int MValue;
int numCommands;
char command;
int value;
cin >> n;
vector<int> mySortedValues(n); //We need to read in the sorted values
for (int i = 0; i < n; ++i) {
    cin >> mySortedValues[i];
}

std::sort(mySortedValues.begin(), mySortedValues.end()); //Sort em


//read n numbers from the input and add them to the vector mySortedValues
//Get the M value
cin >> MValue;
MTree<int>* myTree = new MTree<int>(MValue);

//Build the tree
(*myTree).buildTree(mySortedValues);
cin >> numCommands; // Read the number of commands

/************** Read each command Process ***************/

for (int i = 0; i < numCommands; i++) {
    if (numTimesRun >= numCommands) {
        break;
    }
std::cin >> command; // Read the command type
switch (command) {

    case 'I': { // Insert
    cin >> value;
    try {
    (*myTree).insert(value);
    std::cout << "The value = " << value << " was inserted." << endl;
    }
    catch (duplicateInsertion& e) {
    std::cout << "The value = " << value << " already in the tree." << endl;
    }
    break;
    }

    case 'R': { // Remove
    cin >> value;
    try {
    (*myTree).remove(value);
    std::cout << "The value = " << value << " has been removed." << endl;
    }
    catch (NotFoundException& e) {
    cout << "The value = " << value << " not found." << endl;
    }
    break;
    }

    case 'F': { // Find
    cin >> value;
    if (myTree -> find(value)) {
    std::cout << "The element with value = " << value << " was found." << endl;
    } else {
    cout << "The element with value = " << value << " not found." << endl;
    }
    break;
    }

    case 'B': { // rebuild tree
    vector<int> myValues = (*myTree).collect_values();
    (*myTree).buildTree (myValues);
    cout << "The tree was rebuilt." << endl;
    break;
    }

    default:
    cout << "Invalid command!" << endl;
    //break;
}
}

//Make the final list of the tree
myTree -> display();
delete myTree;
return 0;
}

/* DOCUMENTATION

    LLM Usage: Prompts and Rationale

Can you formally define the catch keyword and what it means in C++?
Rationale: I recalled what this keyword meant, but I just wanted a refresher. I also made sure it did not have any other
implications in C++ that I was not aware of.

Is a dublicate Insertion exception defined or will I need to define it?
Rationale: I knew I would need it, and although I was pretty sure I would have to define it, I had hope otherwise.

argument list for class template "duplicateInsertion" is missing
Rationale: I was checking up on an error. I now know to be more careful of where I place template declarations, as this
was a consequence of one I did not need to place.

How would an insert method work in an mTree?
Rationale: I have never built a robust insert method for a Binary Tree before, so I wanted to know the ins and outs of
proper implementation. This kind of stuff really helps for future projects.

identifier "DT" is undefined
Rationale: Not proud of this question, but it was late and I forgot to place template declarations over methods. I will
not be making that mistake again soon.

non-const lvalue reference to type 'int' cannot bind to a value of unrelated type 'vector<int>'
Rationale: I was trying to find out what this error was all about. I had simply just assigned an unexcpected refrence
to a vector that could not take it. It is the little mistakes that get you.

reference to stack memory associated with local variable 'result' returned [-Wreturn-stack-address]
Rationale: As the error suggests, the variable was to be destroyed after I returned a reference to it. I was more
careful about this afterward.

How can I implement a collect_values function to get all values into an array or vector, sort it with mergeSort, 
then call buildTree on it to get a new, sorted MTree?
Rationale: This is a look at an early attempt at collect values. I had originally planned to use mergeSort, but I
decided against it due to the extra space it required. I know it probably would have not mattered, but still. I did end
up using what I learned from this question to eventually implement collect_values.

Alternatively, could doing an inorder traversal of the tree be a better approach?
Rationale: As an inorder traversal would still require me to sort it if I got it wrong, (which would no longer allow
it to be considered an inorder traversal) I decided against it. I went for the simpler approach of using vector's sort.

What does the sort function in the vector class use to sort? That is, is it a binary sort? MergeSort?
Rationale: This is sort of a follow-up to the last question. I was curious as to the actual composition of vector's
sort function. I was unsure if it was better if I used it or if I implemented my own sort function. It turns out that
it is pretty fast and efficient, all things considered. I'll keep that in mind for future projects.

Can I add a display function to show an entire MTree?
Rationale: This was during debugging. A display method would make it a lot easier to visualize the tree. It turns out,
this is actually pretty difficult to build properly, though I did manage it. It taught me that sometimes, you need to
take a few more steps to get the results you want.

The remove function is not working, but it does seem to affect the structure somewhat. What does this mean?
Rationale: It actually just meant that the buildTree function was also not working properly. This was prior to the
point where I made the buildTree function my own. It did not like to cooperate with my functions, so I replaced it.
Let this be a lesson: It is usually better to build your own functions, even if the TA's give you a function 
(no offense meant).

Will I need to define the assign function?
Rationale: As with a lot of keywords in C++, I am not always sure if I need to define them. I do not this time, and I'll
keep that in mind for future reference.

Why can I not use a range-based for loop?
Rationale: It looks like my compiler does not support range-based for loops, at least for the version of C++ I am on.
I may need to get that looked at for future project's sake.

What assertions and statements can I add to my code to track its process?
Rationale: This is another look at the debugging process. I was trying to find out just where things were breaking. This
is and will remain a great way for me to find where issues are.

Is there a key to automatically stop a running file in C++?
Rationale: This one is self explanatory. I was looking for a way to stop the program from running. The key is
Ctrl + C, by the way. It is the same for Mac and Windows. Hope it helps.

zsh: segmentation fault  ./a.out
Rationale: I did not quite remember what this meant, but I remembered it was very bad. My memory was correct. I hate
these errors. It does not even tell me where it occured! I avoid these at all costs.

    Incremental Development:

I used the tools for this project a little differently, and I believe that is because 
my process for this project was actually a little different than that of my previous projects. Even though I still
utilized tools like github copilot, I spent more time brainstorming on how to implement these methods than I usually
do. The tools I used definitely helped me when I fell short, and as usualy, AI helped me when my own knowledge began
to fall short. It helped me quite a bit with gradually developing my methods, like buildTree. It also helped me to
select vector's sort function as my primary sort, as it explained to me its versitility and efficiency. Additionally,
whenever I found an error I could not get past, like the segmentation error, AI helped me to learn why that was happening
My class notes were more helpful here than ever, expecially in the case of 2-3 trees, as they are somewhat similar to 
M-Trees. Overall, I took a larger step back sometimes to think about how I would implement something rather than just
jumping in, and with my notes and AI by my side, I think it helped to develop this project quicker and more efficiently.

    Debugging and Testing Plans:

My debugging/testing plans in this project were extensive, but they were not proccesses I had not used before. I placed
assertions and print statements thoughout my code where I tought necessary to track errors down. These mostly lied in
the find_child, split_node, and buildTree functions, as this was where I sourced the errors to. The problems I faced
this project were mostly due to faulty methods and splitting in particular, so my plan of moving methods to a separate
file one at a time and running them to see if they were the problem was effective to an extent, but not as much as it
was for the last project. As the majority of my debugging had to do with traking down not errors but segmentation faults,
the assertions were my best bet until I got smart with my buildTree function. I basically just repeatedly ran the 
program and kept rewriting a few different functions until they worked. I also used the display() function at times to
see if the tree was building properly. Mostly just a lot of trial and error with this project.

Issues and Resolutions:
As I have previously mentioned, the main issues I was faced with was actually building the tree and maintaining it. 
Failure to do so would result in segmentation errors as my methods would inevitably reach for nodes that were not there.
I ended up solving this by ensuring that the split_node and insert function worked as they needed to, and used them to
write buildtree. If those two could maintain the tree, then I would use them to build the tree too. This worked well,
and it was what I stuck with until the end. That was the end of the segmentation errors. Other issues I ran into that
were not as major were general syntax errors and, on multiple occasions, forgetting to place template declarations where
they needed to be. That was a case of me being off of my game. Time, energy, AI, dedication, brainpower, and sheer will
got me through these struggles.

Verification:
Once again, I ran the programs though a seperate file to ensure that they were working properly, and used a display
function to further inspect the tree as it was built and ensured its structure was correct. I created a mock input
and some test cases in the test file to make sure that these methods worked. After that, I led them back to the main
program, and I made sure they ran with the actual redirected input. I also placed a lot of print statements and 
assertions throughout these methods to make for absolute certainty that they were running as they were meant to.
Through these verifications, I made sure that my program was what it needed to be and ran as it should.

*/
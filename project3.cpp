#include <iostream>
#include <string>
using namespace std;

class CPUJob {
public:
int job_id; // Unique identifier for the job
int priority; // Priority level of the job (1-10)
int job_type; // Job type (1-10)
int cpu_time_consumed; // Total CPU time consumed by the job
int memory_consumed; // Total memory consumed thus far

CPUJob(); // Default constructor and parameterized constructors follow
CPUJob(int id, int pri, int type, int cpuTime, int memory) 
        : job_id(id), priority(pri), job_type(type), cpu_time_consumed(cpuTime), memory_consumed(memory) {}
CPUJob(const CPUJob& job);
void display() const;

//Standard getters and setters
int getJobID() const;
int getJobType() const;
int getPriority() const;
int getCPUTime() const;
int getMemory() const;

void setPriority(int newPriority);
void setJobType(int newJobType);
void setCPUTime(int newCPUTime);
void setMemory(int newMemory);
};

CPUJob::CPUJob() {
job_id = 0;
priority = 0;
job_type = 0;
cpu_time_consumed = 0;
memory_consumed = 0;
}

CPUJob::CPUJob(const CPUJob& job) { //This constructor had to be added specifically to copy
    this -> job_id = job.job_id;
    this -> job_type = job.job_type;
    this -> priority = job.priority;
    this -> cpu_time_consumed = job.cpu_time_consumed;
    this -> memory_consumed = job.memory_consumed;
}

void CPUJob::display() const { //This display function displays each individual job
    std::cout << "Job ID: " <<  job_id << ", Priority: " << priority << ", Job Type: " << job_type
    << ", CPU Time Consumed: " << cpu_time_consumed << ", Memory Consumed: " << memory_consumed << endl;
}

int CPUJob::getJobID() const {
    return job_id;
}

int CPUJob::getPriority() const {
    return priority;
}

int CPUJob::getJobType() const {
    return job_type;
}

int CPUJob::getCPUTime() const {
    return cpu_time_consumed;
}

int CPUJob::getMemory() const {
    return memory_consumed;
}

void CPUJob::setPriority(int newPriority) {
    priority = newPriority;
}

void CPUJob::setJobType(int newJobType) {
    job_type = newJobType;
}

void CPUJob::setCPUTime(int newCPUTime) {
    cpu_time_consumed = newCPUTime;
}

void CPUJob::setMemory(int newMemory) {
    memory_consumed = newMemory;
}

template <class DT>
class Queue {
public:
DT* JobPointer; // Pointer to a job (e.g., CPUJob)
Queue<DT>* next; // Pointer to the next node in the queue

Queue(); //More default constructors and parameterized constructors
Queue(DT* job);
Queue(CPUJob* job); //This one is the result of VSCode being stubborn and not liking the DT* job parameter
};

template <class DT>
Queue<DT>::Queue() {
    JobPointer = nullptr;
    next = nullptr;
}

template <class DT>
Queue<DT>::Queue(DT* job) {
    JobPointer = job;
    next = nullptr;
}

template <>
Queue<CPUJob*>::Queue(CPUJob* job) { //Notice how it does not have class DT. Had to improvise to allow enqueue to work
    CPUJob** newJob = new CPUJob*(new CPUJob(*job));
    JobPointer = newJob;
    next = nullptr;
}

template <class DT>
class NovelQueue {
public:
Queue<DT>* front; // Pointer to the front of the queue
Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
Queue<DT>* next() { //Dont think I used this one, but it is a good example of the different directions I took
    if (front && front -> next) {
        return front -> next;
    }
    return nullptr;
}
Queue<DT>* rear; //Had to define a new keyword to access the back of NovelQueue
int size; // Number of elements in the queue

NovelQueue() : front(nullptr), NodePtrs(nullptr), rear(nullptr), size(0) {}

~NovelQueue(); //Destructor
//Commands added
void enqueue(DT* job); //These that follow are the basic, fundamental functions of this project, from enqueue to reorder
DT dequeue();
void modify(int jobId, int priority, int jobType, int cpuTimeConsumed, int memoryConsumed);
void change(int jobId, int fieldIndex, int newValue); //Notice how some of these give no output
void promote(int jobId, int positions); //I usually only had them output if I had to assign it to a specific job for display
NovelQueue<DT>& reorder(int attributeIndex); 

void display() const; //Same thing here, except for count
int count() const;
void listJobs(); //ListJobs could not be const. It had to reorder the queue before displaying it
//DT* binarySearch(NovelQueue<DT>* queue, CPUJob* targetJob); // I originally had two binarySearches, but I just didnt use the other after a while
DT* binarySearch(int jobID); //This one became the main one as it was just easier to use
NovelQueue<DT>* mergeSort(NovelQueue<DT>* queue, int attributeIndex); //All three of these next methods were originally built for only reorder
void mergeSortArray(DT** jobs, int left, int right, int attributeIndex);
void mergeArray(DT** jobs, int left, int mid, int right, int attributeIndex);
//void insertionSort(DT** jobs, int size); // I also had an insertionSort for listJobs, but just used reorder instead
bool compareJobs(DT* job1, DT* job2, int attributeIndex); //This one was also original meant only for reorder
int getSize() const; //this and the next are simple helper functions
bool isEmpty() const;
};

template <class DT>
void NovelQueue<DT>::enqueue(DT* job) { //This is the backbone of the program. It adds to the NovelQueue.
    // We first check if the job already exists in the queue
    if (this -> binarySearch((*job) -> job_id) != nullptr) {
        std::cout << "Job ID " << (*job) -> job_id << " already exists!" << endl;
        return;
    }
    
    Queue<DT>* newNode = new Queue<DT>(job); //New node
    
    if (front == nullptr) { //If the front is empty, add node
        front = newNode;
        rear = newNode; 
        size = 1; 
        NodePtrs = new Queue<DT>*[size]; // More memory for NodePtrs
        NodePtrs[0] = newNode;
    } else {
        // Unless it is the first job added, we'll go into this loop
        rear -> next = newNode;
        rear = newNode; 
        size++; // Update nodes and size to fit new functiom
        
        // Move NodePtrs around to fit the new node
        Queue<DT>** newNodePtrs = new Queue<DT>*[size];
        for (int i = 0; i < size - 1; i++) {
            newNodePtrs[i] = NodePtrs[i]; 
        }
        newNodePtrs[size - 1] = newNode; // Time to add new node
        
        // This serves to clean up memory
        if (NodePtrs != nullptr) {
            delete[] NodePtrs; 
        }
        NodePtrs = newNodePtrs; //Reroute the old NodePtrs to the new one
    }
}

template <class DT>
DT NovelQueue<DT>::dequeue() { //This one was actually simplier than I thought it would be

    if (!front) { //Make sure the front is not empty
        
        return nullptr;
    } 

    DT removedJob = *(front -> JobPointer); //Take out job from the front

    Queue<DT>* temp = front; //store the front for a bit, move front, then clean up and return the removed job
    front = front -> next;

    delete temp;
    size--;

return removedJob;
}

template <class DT>
void NovelQueue<DT>::modify(int jobID, int newPriority, int newJobType, int newCPUTime, int newMemory) {
    DT* jobPointer = this -> binarySearch(jobID); //We use the given ID to modify rest of the attributes
    if (jobPointer != nullptr) { //Assign new values
        (*jobPointer) -> setPriority(newPriority);
        (*jobPointer) -> setJobType(newJobType);
        (*jobPointer) -> setCPUTime(newCPUTime);
        (*jobPointer) -> setMemory(newMemory);
        (*jobPointer) -> display(); //Show it to the world
    } else {
        std::cout << "Job with ID " << jobID << " not found in the queue." << std::endl; //Nice to know if it fails
    }
}

template <class DT>
void NovelQueue<DT>::change(int jobId, int fieldIndex, int newValue) { //We change a single value with this one
bool changed = false;
    Queue<DT>* current = front; //This and modify were not too teribly difficult, which is good because later methods suck
    int i = 0;
    while (current && i < size) { //Just find the field you need and change the value
        if ((*current -> JobPointer) -> job_id == jobId) {
            if (fieldIndex == 1) {
                (*current -> JobPointer) -> priority = newValue;
            } else if (fieldIndex == 2) {
                (*current -> JobPointer) -> job_type = newValue;
            } else if (fieldIndex == 3) {
                (*current -> JobPointer) -> cpu_time_consumed = newValue;
            } else {
                (*current -> JobPointer) -> memory_consumed = newValue;
            }
            std::cout << "Changed Job ID " << jobId << " field " << fieldIndex << " to " << newValue << ": " << endl;
            (*current -> JobPointer) -> display(); //Again, some of this stuff is just easier to have up here
            std::cout << "Jobs after changing field:" << std::endl; //Rest of the output is in the main function
            changed = true;
            break;
        }
        current = current -> next; //I forgot this originally. Did not know why I was stuck in an infinite loop for a while
        i++;
    }

    if (!changed) {
        std::cout << "Job with ID " << jobId << " not found in the queue." << std::endl;
    }
}

template <class DT>
void NovelQueue<DT>::promote(int jobID, int positions) { //Now we get into pain in the ass territory
    // binarySearch clutch
    DT* promotionMaterial = this -> binarySearch(jobID);
    (*promotionMaterial) -> display();
    DT* jobPointer = this -> binarySearch(jobID);
    if (jobPointer == nullptr) {
        return;
    }

    // The first of several array conversions in this program. I am a lot better with these
    Queue<DT>** array = new Queue<DT>*[size];
    Queue<DT>* current = front;
    int index = 0;
    while (current != nullptr) { //Iterate through and create an array
        array[index++] = current;
        current = current -> next;
    }

    // Get the location of the job
    int jobIndex = -1;
    for (int i = 0; i < size; ++i) {
        if (array[i] -> JobPointer == jobPointer) {
            jobIndex = i;
            break;
        }
    }

    if (jobIndex == -1) { //Precuations and old test cases
        delete[] array;
        return;
    }

    // Find the new position
    int newIndex = jobIndex - positions;
    if (newIndex < 0) {
        newIndex = 0;
    }

    // Move that job
    Queue<DT>* jobNode = array[jobIndex];
    for (int i = jobIndex; i > newIndex; --i) {
        array[i] = array[i - 1];
    }
    array[newIndex] = jobNode;

    // This part was a pain
    front = array[0];
    current = front;
    for (int i = 1; i < size; ++i) {
        current -> next = array[i];
        current = current->next;
    }
    current -> next = nullptr;
    rear = current;

    // Clean up
    delete[] array;
}

template <class DT>
NovelQueue<DT>& NovelQueue<DT>::reorder(int attributeIndex) { //I hate this one
    NovelQueue<DT>* sortedQueue = mergeSort(this, attributeIndex);

    //This function took so damn long. It has like four or five different methods that were originally all for itself
    //All so I could use mergeSort and its nice complexity. I still thought it was worth it until about an hour and a half into debugging it alone

    // Give the returned values from sortedQueue to the old one
    this->front = sortedQueue->front;
    this->rear = sortedQueue->rear;
    this->size = sortedQueue->size;
    this->NodePtrs = sortedQueue->NodePtrs;

    // It gets really angry if you dont do this
    sortedQueue->front = nullptr;
    sortedQueue->rear = nullptr;
    sortedQueue->NodePtrs = nullptr;

    delete sortedQueue;
    return *this;
}

template <class DT>
void NovelQueue<DT>::listJobs() { //Around the end of my work on this project I got smart and figured that I could just piggyback off of reorder
    this -> reorder(1);
    this -> display();
    //There was a lot more here before, and an insertionSort method
}

template <class DT>
DT* NovelQueue<DT>::binarySearch(int jobID) { //Love this one. So useful
    int queueSize = this -> getSize(); 
    DT* jobArray[queueSize];  

    
    int index = 0; //Convert to an array again
    Queue<DT>* current = this -> front;
    while (current) {
        jobArray[index++] = current -> JobPointer;
        current = current -> next;
    }

    int low = 0;
    int high = queueSize - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // base case
        if ((*jobArray[mid]) -> job_id == jobID) {
            return jobArray[mid];  
        }
        if ((*jobArray[mid]) -> job_id < jobID) { //These are the iterative loops
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return nullptr;  //In case we do not find the element in the array
}

template <class DT>
NovelQueue<DT>* NovelQueue<DT>::mergeSort(NovelQueue<DT>* queue, int attributeIndex) { //For merging and sorting the queue
    if (!queue || queue->isEmpty()) {
        return queue;
    }

    // Array time again
    DT** jobs = new DT*[size];
    Queue<DT>* current = queue -> front;
    int index = 0;
    while (current) {
        jobs[index++] = current->JobPointer;
        current = current->next;
    }

    //Call another function for this guy
    mergeSortArray(jobs, 0, size - 1, attributeIndex);

    // Now we go back to a queue
    NovelQueue<DT>* sortedQueue = new NovelQueue<DT>();
    for (int i = 0; i < size; ++i) {
        sortedQueue->enqueue(jobs[i]);
    }

    delete[] jobs;
    return sortedQueue;
}

template <class DT>
void NovelQueue<DT>::mergeSortArray(DT** jobs, int left, int right, int attributeIndex) {
    if (left < right) { //Direct the left and right to the right directions to get properly merged
        int mid = left + (right - left) / 2;
        mergeSortArray(jobs, left, mid, attributeIndex);
        mergeSortArray(jobs, mid + 1, right, attributeIndex);
        mergeArray(jobs, left, mid, right, attributeIndex);
    }
}

template <class DT>
void NovelQueue<DT>::mergeArray(DT** jobs, int left, int mid, int right, int attributeIndex) {
    //This is the confusing one because it is the one that actually does all the work
    int n1 = mid - left + 1;
    int n2 = right - mid;

    DT** leftArray = new DT*[n1]; //Create some arrays for left and right
    DT** rightArray = new DT*[n2];

    for (int i = 0; i < n1; ++i) { //Put all jobs into these arrays
        leftArray[i] = jobs[left + i];
    }
    for (int i = 0; i < n2; ++i) {
        rightArray[i] = jobs[mid + 1 + i];
    }

    int i = 0, j = 0, k = left; //This is the merge aspect of the code. The cout lines are rements of debugging.
    while (i < n1 && j < n2) {
        //std::cout << "Comparing: Left Job ID " << (*leftArray[i]) -> getJobID() << " with Right Job ID " << (*rightArray[j]) -> getJobID() << std::endl;
        if (compareJobs(leftArray[i], rightArray[j], attributeIndex)) {
            //std::cout << "Choosing Left: " << (*leftArray[i]) -> getJobID() << std::endl;
            jobs[k] = leftArray[i];
            ++i;
        } else {
            //std::cout << "Choosing Right: " << (*rightArray[j]) -> getJobID() << std::endl;
            jobs[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) { //if there is anything left in either of the arrays, it gets tacked on here
        jobs[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        jobs[k] = rightArray[j];
        ++j;
        ++k;
    }

    delete[] leftArray; //Get rid of the space wasters
    delete[] rightArray;
}

template <class DT>
bool NovelQueue<DT>::compareJobs(DT* job1, DT* job2, int attributeIndex) { //This is simply for the comparisons of two jobs
    //If any of the attributes are the same, then they get sorted by ID instead
    switch (attributeIndex) {
        case 1: 
            return (*job1) -> getJobID() < (*job2) -> getJobID();   
        case 2: 
            if ((*job1) -> getPriority() == (*job2) -> getPriority()) {
                return (*job1) -> getJobID() < (*job2) -> getJobID();  
            }
            return (*job1) -> getPriority() > (*job2) -> getPriority();  
        case 3: 
            if ((*job1) -> getJobType() == (*job2) -> getJobType()) {
                return (*job1) -> getJobID() < (*job2) -> getJobID();  
            }
            return (*job1) -> getJobType() > (*job2) -> getJobType();  
        case 4: 
            if ((*job1) -> getCPUTime() == (*job2) -> getCPUTime()) {
                return (*job1) -> getJobID() < (*job2) -> getJobID();  
            }
            return (*job1) -> getCPUTime() > (*job2) -> getCPUTime(); 
        case 5: 
            if ((*job1) -> getMemory() == (*job2) -> getMemory()) {
                return (*job1) -> getJobID() < (*job2) -> getJobID();  
            }
            return (*job1) -> getMemory() > (*job2) -> getMemory();  
        default: 
            return false;
    }

}

template <class DT>
int NovelQueue<DT>::getSize() const { //Honestly, now that I look at it, getSize is a bit redundant. I'm not getting rid of it
    return size;
}

template <class DT>
bool NovelQueue<DT>::isEmpty() const { //Is it empty? If so, then return true
    return (front == nullptr);
}

template <class DT>
int NovelQueue<DT>::count() const { //Why have only one function return size when you can have two?
    //std::cout << size << std::endl;
    return size;
}

template <class DT>
void NovelQueue<DT>::display() const { //This is the other display function. It just uses the CPUJob one for each element in the queue

    Queue<DT>* current = front;
    while (current) {
        (*current -> JobPointer) -> display();
        current = current -> next;
    }
}

template <class DT>
NovelQueue<DT>::~NovelQueue() { //The destructor iterates through and gets rid of all the nodes and where they lead
    
    Queue<DT>* current = front;
    while (current != nullptr) {

        Queue<DT>* nextNode = current -> next; 
        delete current -> JobPointer; 
        delete current; 
        current = nextNode; 
    }

    front = nullptr; 
    size = 0; 
}

int main() { //I left most of the original comments. Why not?
int n; // Number of commands
cin >> n; // Read the number of commands
// Instantiate a NovelQueue for CPUJob pointers
NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
char command; // Variable to store the command type
// Variables for job attributes
int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
// Variables for modifying a job
int new_priority, new_job_type, new_cpu_time_consumed;
int new_memory_consumed;
int field_index, new_value;
// Variable for the number of positions in the 'Promote' command
int positions;
int attribute_index; // Variable for the 'Reorder' command
/************** Read each command Process ***************/

for (int i = 0; i < n; ++i) {
cin >> command; // Read the command type
switch (command) {
case 'A': { // Add (Enqueue)
cin >> job_id >> priority >> job_type;
cin >> cpu_time_consumed >> memory_consumed;
CPUJob** newJob = new CPUJob*(new CPUJob(job_id, priority, job_type,
cpu_time_consumed, memory_consumed)); //I changed this to a double pointer to fit NovelQueue and the enqueue function
(*myNovelQueue).enqueue(newJob);
std::cout << "Enqueued Job: " << std::endl; //Output information
(*newJob) -> display();
std::cout << "Jobs after enqueue: " << std::endl;
(*myNovelQueue).display();
break;
}

case 'R': { // Remove (Dequeue)
CPUJob* removedJob = (*myNovelQueue).dequeue(); //Remove the job
if (removedJob) {
cout << "Dequeued Job: "; //Output information
(*removedJob).display();
std::cout << "Jobs after dequeue:" << std::endl;
(*myNovelQueue).display();
delete removedJob; // Clean up memory after use
}
break;
}

case 'M': { // Modify
std::cin >> job_id >> new_priority >> new_job_type;
cin >> new_cpu_time_consumed >> new_memory_consumed;
std::cout << "Modified Job ID " << job_id << ": " << std::endl; //Output Information
(*myNovelQueue).modify(job_id, new_priority, new_job_type,
new_cpu_time_consumed, new_memory_consumed); 
std::cout << "Jobs after modification: " << std::endl; //Output Information
(*myNovelQueue).display();
break;
}

case 'C': { // Change Job Values
cin >> job_id >> field_index >> new_value;
(*myNovelQueue).change(job_id, field_index, new_value); //Part of the output is in the function itself
(*myNovelQueue).display();
break;
}

case 'P': { // Promote
cin >> job_id >> positions;
std::cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s): " << std::endl; //Output Information
(*myNovelQueue).promote(job_id, positions);
std::cout << "Jobs after promotion:" << std::endl;
(*myNovelQueue).display();
break;
}

case 'O': { // Reorder
cin >> attribute_index;
NovelQueue<CPUJob*>& reorderedQueue =
(*myNovelQueue).reorder(attribute_index);
cout << "Reordered Queue by attribute " << attribute_index << ": " << endl; //Output Information
reorderedQueue.display();
break;
}

case 'D': { // Display
std::cout << "Displaying all jobs in the queue: " << std::endl; //Output Information
(*myNovelQueue).display();
break;
}

case 'N': { // Count
int numElements = (*myNovelQueue).count(); // I guess I could have just used getSize()
cout << "Number of elements in the queue: " << numElements << endl; //Output Information
break;
}

case 'L': { // List Jobs
std::cout << "List of Jobs sorted by Job IDs: " << std::endl; //Output Information
(*myNovelQueue).listJobs();
break;
}
default:
cout << "Invalid command!" << endl; //Always need a base case
}
}
delete myNovelQueue; // Clean up the NovelQueue after all operations
return 0;
}

/* DOCUMENTATION

    LLM Usage: Prompts and Rationale

How can I properly implement a LinkedList implementation of a queue with generics in mind?
Rationale: I have never extensively implemented this before. This has helped me both with this type of implementation,
generics, and queues themselves.

Can I implement class methods inside the class declaration
Rationale: I had seen references to this before, but never understand it. I was curious about it

Why does my header say that DT is undefined?
Rationale: I forgot to include the template <class DT>. Never forgot it again

How can I use a linkedList implementation of a queue to implement its own queue class with Node pointers?
Rationale: Again, this is my first time working extensively with this type of implementations. I was still figuring
out how to use the pointers and front/next keywords

What should an enqueue function return
Rationale: I was seeing if I could afford to make it void. I learned I could, and this spread to some of the other 
functions.

What does argument of type "CPUJob *" is incompatible with parameter of type "int" mean?
Rationale: My arrows and functions were throwing errors. I ended up having a lot more problems with pointers and 
dereferencing than this. Good news is that I am a lot better with them now.

Now it says argument of type "CPUJob *" is incompatible with parameter of type "CPUJob **"
Rationale: This one I kind of knew what the problem was because I can read, but I wanted to know how to definitively 
fix it.

Changing a pointer to a pointer to just a pointer no doubt has consequences, yes
Rationale: It does. I wanted to know what kinds of consequences for certain. Now I know.

Now it says a value of type "CPUJob **" cannot be used to initialize an entity of type "CPUJob *
Rationale: Again, I had a lot of trouble with pointers. I learned I was worse with them than I thought. Not anymore.

How can I move an object up a queue of those objects?
Rationale: I was building my promote method and needed ideas. This grew into what it is now.

How can I add a binary search to a queue of pointers?
Rationale: I had built binarySearches before, but never for a queue. Once I had it down, it became really useful.
I had to convert to an array first though, and that is when I began to queues to arrays if I needed random access or 
something like that.

Can I call binarySort as a seperate method?
Rationale: This was the prelude to insertionSort, which was eventually discarded in favor of mergeSort. It was
actully really interesting to see these in action and extensively code them. It also helped me to get the queue's
keywords and working down.

Can binarySort take a vector?
Rationale: I was originally working on using vectors and arrays, but vectors require an imported library I could not
use due to the constraints of this project. Thus, arrays took the forefront.

I only am able to have the <iostream> and <string> headers. Is the sort method for vectors included in another header?
Rationale: This is the specific point where I realized I could not use vectors. I am getting better at reading code
and figuring out if it is included in the base libraries or not.

What is the best way to convert a ** to a *?
Rationale: I had to convert quite a bit, so I wanted to know the best way to do so. Double pointers were less of a 
problem after that.

Can you update the binarySearch method to search for the specific index and details of that index that it is given, 
and not use the sort method because it is not included in the headers I am allowed to use?
Rationale: The binarySearch method it gave me used the sort method, which I could not.

What if we converted it to an array in the method itself, and then the binarySearch takes an array and searches that?
Rationale: I was seeing if the binarySeach method could actually do this. Like I said earlier, this was the start of
me using arrays more often.

How can I build a reorder method without using a vector?
Rationale: This is where I got the idea for the mergeSort method, as it can take vectors or arrays.

Ok, thank you. How can I implement mergeSort?
Rationale: I have never actually written a mergeSort. I needed some pointers. I'll be able to implement this a lot
easier in the future.

To track and traverse a queue, should I have a pointer to the front and rear?
Rationale: Up until this point, I was using only front and next. I will never confine myself like that again.

This line  DT* alreadyExists = binarySearch(this, jobIDen);
Throws this error:
non-const lvalue reference to type 'NovelQueue<CPUJob *>' cannot bind to a temporary of type 'NovelQueue<CPUJob *> *'
Rationale: I was just trying to understand why it broke. Another lesson in references and pointers.

What about this one?
non-const lvalue reference to type 'NovelQueue<CPUJob *>' cannot bind to a temporary of type 'std::nullptr_t'
Rationale: Null pointers continue to work and not whenever they want to, it seems. At least in this case, I was 
able to fix it and learn from it.

Can I overload the delete method so that it can delete expression of type 'CPUJob *[queueSize]'?
Rationale: I did not know how to overload the delete method. I do now, and though I did not use it, I am kind of 
glad I asked this question, as I am not great at overloading operators.

The lines that include arrows meant to dereference things are throwing me errors. They will not compile, and they say 
member reference base type 'CPUJob *' is not a structure or union. Even if I change out the getter method for job_id, 
which should work, I still get an error. Is this a problem with the current keyword, JobPointer, or the general 
structure of NovelQueue and CPUJob?
Rationale: I was having a lot of trouble properly dereferencing things. I learned to be more cautious with double
pointers and how to properly dereference them.

    Incremental Development:

My Rationale mostly covers the lessons I learned from this project, and what AI had to do them. Although I learned a lot
on my own, like how to navigate with front and next, Ai helped shore up what I was uncertain about, which was a lot.
Another new tool I used this time was github copilot, which I had refrained from getting until now. It was an incredible
help. I got it out of frustration with not being able to get around dereferencing and pointers, and I was amazed by how
helpful it was. Anytime I had bugs I could not squash, or a program that I did not know how to finish and could not
feel it out, AI lended me a hand. Between this and the notes I have from class, I was able to get through this project.
Overall, AI, class notes, and a little ingenuity saw me through. 

    Debugging and Testing plans:

The total debugging and testing plans I used in this project were by far the most extensive and thorough I have ever
implemented. I spent quite a lot of time debugging this project, much more than normal. I was stuck for a while, 
trying to fix errors that would pop up out of nowhere and for seemingly no reason. I knew they were cascading errors,
but I did not even know where to start looking for them. I ended up creating a seperate file, copying over my
class declarations, and then moving my functions over one by one and testing them to see where the problems lied.
To my surprise, most of the methods actually worked fine except for one or two bad pointers (except for reorder).
Thus, my plan eventually included testing each method individually, asking AI for a little help as detailed above,
and referencing my old notes and some videos to learn more about pointers and the queue structure. This is how all the
helper functions came to be, and how I eventually finished this project.

Issues and Resolutions: As I have pointed out much already, I had a lot of issues with pointers and dereferencing. 
I had to learn how to properly use them and how to properly dereference them. This in accordance with general syntax
errors and building so many methods that I had to create a seperate file just to figure out what was wrong meant that
this was a very heaviliy debugged project. Github Copilot and its automatic solutions were a huge help, and I learned
how to get myself out of several situations that I thought myself stuck in. I dealt with a few infinite loops, and 
incapsulate issues, but these were quite simple to fix. I have already talked long enough about the extensive
issues I have had with this project, but suffice to say that pointers and dereferencing were the main culprits. I was able
to resolve the situation with a little brainpower, patience, and AI.

Verification: To ensure that my classes worked, I first ran them in a seperate file to see if they would both compile
and run. I had statements strewn thoughout them to see if they entered the right loops and displayed the correct
information. By far the biggest part of the verification was reimplementing the methods in the main file and testing to
see if I could get them to match the sample outputs as closely as possible. I looked through them quite thoughoughly, 
and I am certain that, unless there are simple spaces that I cannot see, my outputs match the ones provided. I hope 
they pass the autograder, but we'll see.
*/
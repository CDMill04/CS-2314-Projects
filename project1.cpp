#include <iostream>
using namespace std;
//Overall Goals: 
//1: Finish constructors provided
//2: Be able to read matrices in
//3: Be able to format them in a sparse matrix format
//4: Be able to perform operations on the matrices

class SparseRow {
    protected:
    int row; //Row#
    int col; //Column#
    int value; //We will assume that all our values will be integers

    public:
    SparseRow (); //default constructor; row=-1;col=-1;value=0
    void display(); // print Row#, Column#, value
    friend ostream& operator<< (ostream& s, const SparseRow& t); //Added a value for SparseRow, had to define it as friend
    //other methods that are necessary such as get and set
    int getRow() const;
    int getCol() const;
    int getValue() const;

    void setRow(int newRow);
    void setCol(int newCol);
    void setValue(int newVal);
};

//Getters and Setters for row, col, and value
int SparseRow::getRow() const {
        return row;
}

int SparseRow::getCol() const {
        return col;
}

int SparseRow::getValue() const {
        return value;
}

void SparseRow::setRow(int newRow) {
    row = newRow;
}

void SparseRow::setCol(int newCol) {
    col = newCol;
}

void SparseRow::setValue(int newVal) {
    value = newVal;
}

SparseRow::SparseRow() { //Default Constructor Definition
    row = 1;
    col = 1;
    value = 0;
}

void SparseRow::display() { //Defining SparseRow's display function to output row, col, and value numbers
    std::cout << "The row number is " << row << " , the column number is " << col << " , and the value is " << value << "." << endl;
}

ostream& operator<< (ostream& s, const SparseRow& t) { //Overriding operator << to output SparseRow format
    s << t.getRow() << ", " << t.getCol() << ", " << t.getValue();
    return s;
}
//This method will hopefully allow each line of the sparse matrix format to be constructed. The idea is that I can call this
//function later in another function for each line of the format and construct it that way.


class SparseMatrix {
    protected:
    int noRows; //Number of rows of the original matrix
    int noCols; //Number of columns of the original matrix
    int commonValue; //read from input
    int noNonSparseValues;
    SparseRow* myMatrix; //Array

    public:
    SparseMatrix ();
    SparseMatrix (int n, int m, int cv, int noNSV); //added the int noNSV
    SparseMatrix* Transpose (); //Matrix Transpose
    SparseMatrix* Multiply (SparseMatrix& M);
    SparseMatrix* Addition (SparseMatrix& M);
    friend ostream& operator<< (ostream& s, const SparseMatrix& sm);
    void displayMatrix (); //Display the matrix in its original format
    ~SparseMatrix(); //Added a destroyer to try and get rid of the clutter
    //other methods that are necessary such as get and set

    int getNoRows() const;
    int getNoCols() const;
    int getCommonValue() const;
    int getNoNonSparseValues() const;
    int getValueSM(SparseMatrix& M, int row, int col);

    void setNoRows(int rowNumber);
    void setNoCols(int colNumber);
    void setCommonValues(int comVals);
    void setNoNonSparseValues(int noValues);
};
//Getters and setters for SparseMatrix class
int SparseMatrix::getNoRows() const {
    return noRows;
}

int SparseMatrix::getNoCols() const {
    return noCols;
}

int SparseMatrix::getCommonValue() const {
    return commonValue;
}

int SparseMatrix::getNoNonSparseValues() const {
    return noNonSparseValues;
}

int SparseMatrix::getValueSM(SparseMatrix& M,int row, int col) { //Had to implement this method to grab a specific value already in a SparseMatrix
    int matrixValue = -100;

    //SparseMatrix* grabber = new SparseMatrix(M.getNoRows(), M.getNoCols(), M.getCommonValue(), M.getNoNonSparseValues());

    for (int i = 0; i < M.getNoRows(); i++) {
        for (int j = 0; j < M.getNoCols(); j++) {
            if ((i == row) && (j == col)) {
                matrixValue = (myMatrix -> getValue()); //Returns the value in exchange for the variables given
                return matrixValue;
            }
        }

    }
    
    return matrixValue; //If any of the items in the matrix are -100, we know that the tiered for-loops did not work.
}

void SparseMatrix::setNoRows(int rowNumber) {
    noRows = rowNumber;
}

void SparseMatrix::setNoCols(int colNumber) {
    noCols = colNumber;
}

void SparseMatrix::setCommonValues(int comVals) {
    commonValue = comVals;
}

void SparseMatrix::setNoNonSparseValues(int noValues) {
    noNonSparseValues = noValues;
}

SparseMatrix::SparseMatrix() { //Defines the default constructor
    noRows = 1;
    noCols = 1;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = nullptr;
}

SparseMatrix::SparseMatrix(int n, int m, int cv, int noNSV) { //Defines the values given to the matrix and reseves space for myMatrix
    noRows = n;
    noCols = m;
    commonValue = cv;
    noNonSparseValues = 0;

    myMatrix = new SparseRow[noNSV]; //I need to make sure that this does not waste space, so its gotta be able to adapt
    int index = 0; // new variable for function
    for (int i = 0; i < noRows; i++) { //needs to iterate through all the rows and columns
        for (int j = 0; j < noCols; j++) {
            int value;
            std::cin >> value; // Must read a new value

            if (value != commonValue) {
                myMatrix[index].setRow(i); // Remembers the rows and columns, and creates space accordingly
                myMatrix[index].setCol(j);
                myMatrix[index].setValue(value);
                //std::cout << "Storing non-sparse value: " << value << " at (" << i << ", " << j << ")" << std::endl;
                index++; //Had th line above for debugging purposes
            }
        }
    }
    noNonSparseValues = index;
    
}

ostream& operator<< (ostream& s, const SparseMatrix& sm) { //Initially used AI to help here (Check documentation).
    for (int i = 0; i < sm.noNonSparseValues; i++) { //Ended up changing what it gave me in favor of using the SparseRow << operator
        s << sm.myMatrix[i] << "\n";
    }
    return s; 
}

void SparseMatrix::displayMatrix() { //Must display the original format of the matrix
    int index = 0;
    
    for (int i = 0; i < noRows; i++) { //Loops through each row and column and compares the input to the number of nonSparse
        for (int j = 0; j < noCols; j++) { //Also takes into consideration that the rows and columns line up
            if (index < noNonSparseValues && myMatrix[index].getRow() == i && myMatrix[index].getCol() == j) {
                std::cout << myMatrix[index].getValue() << " "; //Output the number
                index++;
            } else {
                std::cout << commonValue << " "; //Otherwise, output the common value
            }
        }
        std::cout << endl; //Note for documentation: had to fix endl
    }


}

SparseMatrix* SparseMatrix::Addition(SparseMatrix& M) { //Must add the two matrices together. I had to ask AI how to finish it
    SparseMatrix* Temp = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues);

    if ((M.getNoRows() != noRows) || (M.getNoCols() != noCols)) {
        std::cout << "Matrix addition is not possible" << endl; //Accounts for if the addition is not possible
        return nullptr;
    }

   int index1 = 0; 
    int index2 = 0; 
    int resultIndex = 0; 
    int newValue = 0; 

    for (int i = 0; i < noRows; i++) { //This will traverse the entire matrix
        for (int j = 0; j < noCols; j++) {
            // It will then grab a value from the matrix
            int val1 = (index1 < noNonSparseValues && myMatrix[index1].getRow() == i && myMatrix[index1].getCol() == j)
                       ? myMatrix[index1].getValue()
                       : commonValue;

            if (index1 < noNonSparseValues && myMatrix[index1].getRow() == i && myMatrix[index1].getCol() == j) {
                index1++; // Then it moves on
            }

            // Now it will do the same with SparseMatrix M
            int val2 = (index2 < M.getNoNonSparseValues() && M.myMatrix[index2].getRow() == i && M.myMatrix[index2].getCol() == j)
                       ? M.myMatrix[index2].getValue()
                       : M.getCommonValue();

            if (index2 < M.getNoNonSparseValues() && M.myMatrix[index2].getRow() == i && M.myMatrix[index2].getCol() == j) {
                index2++; //The index increases as it looks for the next entry in matrix M
            }

            // It adds
            newValue = val1 + val2;

            // After, It checks to see if it is the commonValue, if not, it will store it
            if (newValue != commonValue) {
                Temp -> myMatrix[resultIndex].setRow(i);
                Temp -> myMatrix[resultIndex].setCol(j);
                Temp -> myMatrix[resultIndex].setValue(newValue);
                resultIndex++; 
            }
        }
    }

    // This last step is present in quite a few methods. It is just for updating the number of non Sparse Values
    Temp -> noNonSparseValues = resultIndex;

    return Temp;

}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) { //This one has to multiply

    if(M.getNoRows() != noCols) {
        std::cout << "Matrix multplication is not possible" << endl;
        return nullptr; //This ensures that we only do possible matrix multiplication
    }

    int initialSize = noNonSparseValues * M.getNoNonSparseValues();
    SparseMatrix* Temp = new SparseMatrix(noRows, M.getNoCols(), commonValue, initialSize);

    //int resultIndex = 0;

    for (int i = 0; i < Temp -> getNoNonSparseValues(); i++) { 
        Temp -> myMatrix[i].setValue(0); //Ensures that the values of the temp matrix are filled with 0
    }

    // This for loop and the next for loop through both arrays and grab the right columns and rows
    for (int i = 0; i < noNonSparseValues; i++) {
        int R1 = myMatrix[i].getRow();   
        int C1 = myMatrix[i].getCol();  
        int V1 = myMatrix[i].getValue(); 

        for (int j = 0; j < M.getNoNonSparseValues(); j++) {
            int R2 = M.myMatrix[j].getRow();   
            int C2 = M.myMatrix[j].getCol();   
            int V2 = M.myMatrix[j].getValue(); 

            //This if statement ensures that the multiplication is only done when the columns and rows line up
            if (C1 == R2) {
                int newR = R1;  
                int newC = C2;  
                int newV = V1 * V2; // Multiply the values from both 

                //Now it looks for where the value should go
                bool valueFound = false;
                for (int k = 0; k < Temp->getNoNonSparseValues(); k++) {
                    if (Temp->myMatrix[k].getRow() == newR && Temp->myMatrix[k].getCol() == newC) {
                        // If something is already here, it adds
                        Temp->myMatrix[k].setValue(Temp->myMatrix[k].getValue() + newV);
                        valueFound = true;
                        break;
                    }
                }

                // Otherwise, it adds a new element at that point
                if (!valueFound) {
                    if (Temp->noNonSparseValues >= Temp->noRows * Temp->noCols) {
                        // Resize the matrix if needed
                        SparseRow* newMatrix = new SparseRow[Temp->noNonSparseValues * 2]; // Double the size
                        std::copy(Temp->myMatrix, Temp->myMatrix + Temp->noNonSparseValues, newMatrix);
                        delete[] Temp->myMatrix;
                        Temp->myMatrix = newMatrix;
                    }

                    //This part adds the results 
                    Temp->myMatrix[Temp->noNonSparseValues].setRow(newR);
                    Temp->myMatrix[Temp->noNonSparseValues].setCol(newC);
                    Temp->myMatrix[Temp->noNonSparseValues].setValue(newV);
                    Temp->noNonSparseValues++; // It then increases noNonSparseValues's count
                }
            }
        }
    }


    return Temp;

} 

SparseMatrix* SparseMatrix::Transpose() { //This method was relatively simple, just changing the rows and columns, but keeping values.
//I wrote the above comment prior to debugging. Please note its naivity. This method gave me so much trouble.

    SparseMatrix* Temp = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

    int index = 0;

    // Loops all over the matrix
    for (int i = 0; i < noNonSparseValues; i++) {
        int newR = myMatrix[i].getCol(); //These lines are pretty simple, they change the rows and columns
        int newC = myMatrix[i].getRow(); 
        int value = myMatrix[i].getValue(); 
        
        bool gotNumber = false;

        // See if the supposed position already has a non-commonValue number at this location
        for (int j = 0; j < index; j++) {
            if (Temp->myMatrix[j].getRow() == newR && Temp->myMatrix[j].getCol() == newC) {
                // Add it if so
                Temp->myMatrix[j].setValue(Temp->myMatrix[j].getValue() + value);
                gotNumber = true;
                break;
            }
        }

        // Like the last method, we will create an element if there is none already
        if (!gotNumber) {
            Temp->myMatrix[index].setRow(newR);
            Temp->myMatrix[index].setCol(newC);
            Temp->myMatrix[index].setValue(value);
            index++;
        }
    }

    Temp->setNoNonSparseValues(index); 


    return Temp;
}

SparseMatrix::~SparseMatrix() { //Defining the destructor method
    delete[] myMatrix;
}

int main () {
    int n, m, cv, noNSV;

    SparseMatrix* temp;
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
    //std::cout << "The first line SparseMatrix works." << endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    //Had to add the line above due to the newLine in the input file causing issues with reading in the next file

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    //std::cout << "The second line works." << endl;

    std::cout << "First one in sparse matrix format" << endl;
    std::cout << (*firstOne);

    std::cout << "After transpose" << endl;
    std::cout << (*(*firstOne).Transpose());

     std::cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix(); 

    std::cout << "Second one in sparse matrix format" << endl;
    std::cout << (*secondOne);

    std::cout << "After Transpose" << endl;
    std::cout << (*(*secondOne).Transpose());

     std::cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    std::cout << "Matrix Addition Result" << endl;
    temp = (*firstOne).Addition(*secondOne);
    std::cout << *temp;
    (*temp).displayMatrix(); 

    std::cout << "Matrix Multiplication Result" << endl;
    temp = (*firstOne).Multiply(*secondOne);
    std::cout << *temp;
    (*temp).displayMatrix();
   delete firstOne; //These two make sure that the SparseMatrices are deleted after every go
   delete secondOne;
}

/* DOCUMENTATION

AI Prompts: 
ostream& operator<< (ostream& s, const SparseRow t); Why does this method throw an error? The error it throws says "too many parameters for this operator functionC/C++(344)"
Rationale: This error I had never seen before, and so I thought AI might know the answer. It told me to define the keyword "friend" before the signature of the operator I was trying to overload. That will help me in future cases

Can you remind me the importance of * and & in C++?
Rationale: I needed a refresher, and it explained it very well. Really saved a lot of time when coding, given that otherwise I would have been scratching my head trying to remmeber the importance of those operators

Given a Matrix, how can I format the overloaded operator << to read it in and output it in sparsematrix format
Rationale: I needed a start with this one, and it did help me, but I ended up scrapping a lot of it in favor of a simpler approach using the one from SparseRow.

Given that the input is from a redirected file, how do I save it and build the displayMatrix() method to output its original form?
Rationale: Again, I needed a start, as all my other stuff had been building with the SparseMatrix format. This helped a lot when trying to debugg my addition method, believe it or not.

What would be the proper way to write this function header?
SparseMatrix* Transpose() {}
Rationale: I didnt know how to write a header like that. I ended up using the same format for addition and multiplication

Why does this throw errors?
SparseMatrix* SparseMatrix::Addition(SparseMatrix& M) {
    SparseMatrix* Temp = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues);
    int newRowVal = 0;

    if (!(M.getNoRows() == Temp.getNoRows()) || !(M.getNoCols() == Temp.getNoCols())) {
        cout << "Matrix addition is not possible" << endl;
    }
Rationale: This was REALLY bad and rough code. I was struggling and was trying to finish this method before I had to leave, so I asked. This would most all be rewritten over the course of debugging

Why do all of my lines that start with cout throw an error and say that cout is ambiguous?
Rationale: This is how I figured out how to use the std:: prefix. I'll keep that in mind

Whenever I run my code, it just gives me an endless stream of 0's. Why is this?
Rationale: I had never seen something like this, and needed answers.

How can I implement a type of matrix multiplication method in my code?
Rationale: I had already built the method, I just needed to refine it. This helped with teh transpose method as well.

Could the infinite zeros be an issue with the newline between the inputs?
Rationale: I knew I was probably right about this causing the issue with the infinite zeros, I just had no idea how to fix it.

Transpose of the first one in matrix
0, 0, 100
3, 0, 900
2, 1, 200
4, 1, 300
1, 2, 400
2, 3, 200
0, 4, 1600
4, 4, 700
1, 3, 1832399264
1, 4, 1832399264
2, 0, 1832399264
2, 1, 1832399264
2, 2, 1832399264
2, 3, 1832399264
2, 4, 1832399264
3, 0, 1832399264
3, 1, 1832399264
3, 2, 1832399264
3, 3, 1832399264
3, 4, 1832399264
4, 0, 1832399264
4, 1, 1832399264
4, 2, 1832399264
4, 3, 1832399264
4, 4, 1832399264
Everything after 4, 4, 700 is wrong, and should not be there. How do I fix this?
Rationale: I had no idea what I was looking at. Eventually I got it fixed and used that knowledge to help fix addition and multiply

zsh: segmentation fault  ./a.out < input1.txt
Rationale: This error code stumped me for a long time, and took forver to get around. I needed help.

I keep getting zsh: segmentation fault  ./a.out < input1.txt
What is the problem with this? I think it has something to do with the second matrix, but I do not know what I have to fix?
Rationale: Continued errors with the problem.

The addition method is very close to being done, but it is still adding wrong. I need the values of each of the columns and rows to add up. 
Rationale: I was getting frustrated at this point, and I did not know why the addition would not add properly

Debugging Plan:

The issues I encounted during debugging were infinite zeros upon code execution, improper transposing, improper addition and multiplicaton, a segmentation fault error, and several smaller syntax errors
There were a lot of problems, and the ways I solved them were by commenting part of main out and running different methods to see what was giving me the most issues, and by asking AI, as documented above
I also would put statements in the code for parts that I though could be wrong, either on my own intuition or the AI's.
I looked up why I kept getting that segmentation error. It did not help.
Verification basically went the same way: I ran some specific methods with inputs, and I made sure they all worked


*/


//Data_Structure_Project2_Scheme Interpreter_2016-12161_Taewon Lee
//Hash_Table, Node_Array are implemented with class, snake_Case
//Other function: camelCase
//all command should start with '('
//doesn't return the input when input doesn't start with '('
//since it doesn't make Node_Array
//variable(variable is one variable
//recursion is not implemented
#include <iostream>
#include <cstring>
#include <cctype>
#define hash_capacity 500
#define node_capacity 100
using namespace std;
//contains hash_table, and hash_functions
class Hash_Table
{
public:
    int getHashValue(string symbol);   //check the value is in hash & read from it
    void print();                      //print the hash_table
    string getSymbol(int hashKey);     //to get symbol
    int setLink(int address, int ptr); //to set link
    int getLink(int address);          //to get link
    int getcap();                      //to get capacity
    Hash_Table()
    {
        hash_table[0].Symbol = "nil";
        hash_table[0].link = 0;
    };

private:
    int capacity = hash_capacity + 1;
    class hash_cell
    {
        friend Hash_Table;
        string Symbol;
        int link;
    };
    hash_cell *hash_table = new hash_cell[capacity];
};

//nextRoot is controlled by variable "current"(root at now)
class Node_Array
{
public:
    int Read(string *token, Hash_Table *HT);                  //read the scheme command(build the structure)
    void print();                                             //print the Node_Array
    int nextRoot() { return (current + 1) % (capacity - 1); } //return the Root of free List
    string readCommand(int root, Hash_Table *HT);             //return the command according to the Node_array built
    void free(int root);                                      //delete nodes
    string Eval(int root, Hash_Table *HT);                    //Evaluation Function
    int allocateNew(Hash_Table *HT);                          //allocate new memory
    void printList(int node, Hash_Table *HT);                 //print the List
    int checkStructure(int rootA, int rootB, Hash_Table *HT); //for the isequal
    void garbageCollection(Hash_Table *HT);                   //if node is full, remove node cells if it's not pointed by hash
    int getNroot() { return nroot; };                         //return nroot
    void setNroot(int num) { nroot = num; };                  //setting nroot
    Node_Array()                                              //constructor
    {
        for (int i = 0; i < capacity; i++)
        {
            node_array[i].left = 0;
            node_array[i].right = 0;
        }
    }

private:
    class node_cell
    {
        friend Node_Array;
        int left, right;
        void copyTree(node_cell *new_node_array, node_cell *node_array, int i); //copy the original tree to new tree
    };
    int nroot = 0; //store the root(which is buliding for now)
    int capacity = node_capacity + 1;
    int current = 0;
    node_cell *node_array = new node_cell[capacity];
};
//functions needed
string tolower(string input);                        //change CAPITAL to lower case
unsigned int StringToInt(string s);                  //change string to int, to get a hash_key
string preprocess(string *input, string newcommand); // will be added as the project goes on
string next_token(string *sentence);                 //to get next token
int isNumber(string p);                              //0: non-number 1: int 2: float

int main()
{
    Hash_Table HT = Hash_Table();
    Node_Array NA = Node_Array();
    while (true)
    {
        //input
        string input;
        cout << "> ";
        getline(cin, input);
        input = tolower(input);
        string newcommand = "";
        //preprocess
        newcommand = preprocess(&input, newcommand);
        //read
        int root = NA.Read(&newcommand, &HT);
        NA.setNroot(NA.nextRoot());
        //Eval
        string result = NA.Eval(root, &HT);
        //print
        cout << "] ";
        int result_int = stoi(result);
        if (result_int < 0)
            cout << HT.getSymbol(result_int) << endl;
        if (result_int > 0)
            NA.printList(result_int, &HT);
    }
    return 0;
}

//it checks the structure + value
int Node_Array::checkStructure(int rootA, int rootB, Hash_Table *HT)
{
    if (node_array[rootA].left <= 0 && node_array[rootA].right <= 0) //leaf
    {
        if (node_array[rootA].left == node_array[rootB].left && node_array[rootA].right == node_array[rootB].right)
            return 1;
        else
            return 0;
    }
    if (node_array[rootA].left <= 0)
    {
        if (node_array[rootA].left != node_array[rootB].left)
            return 0;
        else
            return checkStructure(node_array[rootA].right, node_array[rootB].right, HT);
    }
    if (node_array[rootA].right <= 0)
    {
        if (node_array[rootA].right != node_array[rootB].right)
            return 0;
        else
            return checkStructure(node_array[rootA].left, node_array[rootB].left, HT);
    }
    return 1;
}

//allocate new memory, if it's full garbage collection
int Node_Array::allocateNew(Hash_Table *HT)
{
    while (true)
    {
        current++;
        if (node_array[current].left == 0 && node_array[current].right == 0)
            break;
        if (current == capacity)
            break;
    }
    if (current == capacity)
        garbageCollection(HT);
    return current;
}
//copy tree to new tree
void Node_Array::node_cell::copyTree(node_cell *new_node_array, node_cell *node_array, int i)
{
    new_node_array[i].left = node_array[i].left;
    new_node_array[i].right = node_array[i].right;
    if (node_array[i].left > 0)
        copyTree(new_node_array, node_array, node_array[i].left);
    if (node_array[i].right > 0)
        copyTree(new_node_array, node_array, node_array[i].right);
    return;
}

//1.make a new node_array  2.copy when the tree is pointed by hash 3.use new node_array
void Node_Array::garbageCollection(Hash_Table *HT)
{
    int low = 0;
    int *link = new int;
    int howmany = 0;
    for (int i = 0; i < HT->getcap(); i++)
    {
        if (HT->getLink(i) > 0 && HT->getLink(i) <= capacity) //link exist to node
        {
            link[howmany] = HT->getLink(i); //find seed
            howmany++;
        }
    }
    link[howmany] = getNroot(); //also store current node
    howmany++;
    node_cell *new_node_array = new node_cell[capacity];
    for (int i = 0; i < capacity; i++)
    {
        new_node_array[i].left = 0;
        new_node_array[i].right = 0;
    }
    for (int i = 0; i < howmany; i++)
        new_node_array->copyTree(new_node_array, node_array, link[i]);
    node_cell *temp = node_array;
    delete[] temp;
    node_array = new_node_array;
    current = 0;
    //find next current
    while (true)
    {
        current++;
        if (node_array[current].left == 0 && node_array[current].right == 0)
            break;
    }
    delete[] link;
    cout << "---------- garbage collection ----------" << endl;
}
int Hash_Table::getLink(int address)
{
    return hash_table[address].link;
}
int Hash_Table::setLink(int address, int ptr)
{
    hash_table[address].link = ptr;
    return address;
}
int Hash_Table::getcap()
{
    return capacity;
}

int Hash_Table::getHashValue(string symbol)
{
    int hashValue = StringToInt(symbol) % capacity;
    int temp = hashValue;
    for (int i = 0; i < capacity; i++)
    {
        //since deleting hash is not implemented yet
        if (symbol.compare(hash_table[hashValue].Symbol) == 0 || hash_table[hashValue].Symbol.empty())
        {
            hash_table[hashValue].Symbol = symbol;
            return -hashValue;
        }
        hashValue = (hashValue + 1) % capacity;
    }
    throw runtime_error("hash is full for now");
};

int Node_Array::Read(string *input, Hash_Table *HT)
//following the pseudo code
{
    int root = 0;
    bool first = true;
    int token_hash_value = HT->getHashValue(next_token(input));
    if (token_hash_value == HT->getHashValue("("))
    {
        int temp;
        token_hash_value = HT->getHashValue(next_token(input));
        while (token_hash_value != HT->getHashValue(")"))
        {
            if (first)
            {
                root = temp = allocateNew(HT);
                first = false;
            }
            else
            {
                //node_array[temp].right = alloc();
                node_array[temp].right = allocateNew(HT);
                temp = node_array[temp].right;
            }
            if (token_hash_value == HT->getHashValue("("))
            {
                *input = "(" + *input;
                node_array[temp].left = Read(input, HT);
            }
            else
            {
                node_array[temp].left = token_hash_value;
                //                cout << "thv: " << HT->getSymbol(token_hash_value) << "   ";
            }
            if (!first)
                node_array[temp].right = 0;

            //                node_array[temp].right = HT->getHashValue(")");
            token_hash_value = HT->getHashValue(next_token(input));
        }
        return root;
    }
    else
    {
        return token_hash_value;
    }
}

string Node_Array::Eval(int root, Hash_Table *HT)
{
    if (root == 0)
        return "0";
    if (root == hash_capacity + 1) //check if it's list, but 0
    {
        return "0";
    }
    if (root < 0) //if it's a token
    {
        if (HT->getLink(-root) > 0) //link exist, it's a list
        {
            if (HT->getLink(-root) == hash_capacity + 1)
                return "0";
            return to_string(HT->getLink(-root));
        }
        else if (HT->getLink(-root) < 0) //link exist, it's a token
            return Eval(HT->getLink(-root), HT);
        return to_string(root);
    }
    int token_index = node_array[root].left; //to know what the root is;
    if (token_index == HT->getHashValue("+"))
    {
        string l = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        string r = HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)));
        if (isNumber(l) == 1 && isNumber(r) == 1)
            return to_string(HT->getHashValue(to_string(stoi(l) + stoi(r))));
        else
            return to_string(HT->getHashValue(to_string(stof(l) + stof(r))));
    }
    if (token_index == HT->getHashValue("="))
    {
        string l = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        string r = HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)));
        if (isNumber(l) && isNumber(r))
            if (stof(l) == stof(r))
                return to_string(HT->getHashValue("#t"));
            else
                return to_string(HT->getHashValue("#f"));
        else
            throw runtime_error("= error");
    }
    if (token_index == HT->getHashValue("-"))
        return to_string(HT->getHashValue(to_string(stof(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) - stof(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    if (token_index == HT->getHashValue("*"))
    {
        string l = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        string r = HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)));
        return to_string(HT->getHashValue(to_string(stof(l) * stof(r))));
    }
    if (token_index == HT->getHashValue("/"))
        return to_string(HT->getHashValue(to_string(stof(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) / stof(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    if (token_index == HT->getHashValue("%"))
        return to_string(HT->getHashValue(to_string(stoi(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) % stoi(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));

    else if (token_index == HT->getHashValue("eq?"))
    {
        //at same memory?
        // just can know by eval, since eval returns the hash memory at last
        string l = Eval(node_array[node_array[root].right].left, HT);
        string r = Eval(node_array[node_array[node_array[root].right].right].left, HT);
        if (l == r)
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("equal?"))
    {
        int l = stoi(Eval(node_array[node_array[root].right].left, HT));
        int r = stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT));
        if (checkStructure(l, r, HT))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }

    // don't consider the error case(parameter error) e.g) (isnumber) or (isnumber 2  a)
    else if (token_index == HT->getHashValue("number?"))
    {
        if (isNumber(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("symbol?"))
    {
        string result = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        if (!isNumber(result))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("null?")) //is null?
    {
        string eval = Eval(node_array[node_array[root].right].left, HT);
        if (node_array[root].right == 0 || eval == "0" || HT->getLink(-node_array[node_array[root].right].left) == hash_capacity + 1) //NIL should be 0 from constructor;
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("car"))
        return to_string(node_array[stoi(Eval(node_array[node_array[root].right].left, HT))].left);
    else if (token_index == HT->getHashValue("cdr"))
        return to_string(node_array[stoi(Eval(node_array[node_array[root].right].left, HT))].right);
    else if (token_index == HT->getHashValue("define"))
    {
        if (node_array[node_array[node_array[node_array[root].right].right].left].left == HT->getHashValue("lambda")) //if it's a function
        {
            HT->setLink(-node_array[node_array[root].right].left, node_array[node_array[node_array[root].right].right].left); //set link
        }
        else //it's not a function
        {
            int p = node_array[node_array[node_array[root].right].right].left;
            if (p > 0)
                HT->setLink(-node_array[node_array[root].right].left, p);
            else
                HT->setLink(-node_array[node_array[root].right].left, HT->getHashValue(HT->getSymbol(node_array[node_array[node_array[root].right].right].left)));
        }
    }
    else if (token_index == HT->getHashValue("quote"))
        return to_string(node_array[node_array[root].right].left);
    else if (token_index == HT->getHashValue("cons"))
    {
        int newmemory = allocateNew(HT);
        node_array[newmemory].left = stoi(Eval(node_array[node_array[root].right].left, HT));
        node_array[newmemory].right = stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT));
        return to_string(newmemory);
    }
    else if (token_index == HT->getHashValue("cond"))
    {
        while (node_array[node_array[root].right].right != 0) //this is nil
        {
            root = node_array[root].right;
            string p = Eval(node_array[node_array[root].left].left, HT);
            if (p == to_string(HT->getHashValue("#t")))
            {
                string result = Eval(node_array[node_array[node_array[root].left].right].left, HT);
                return result;
            }
        }
        if (node_array[node_array[node_array[root].right].left].left != HT->getHashValue("else"))
            throw runtime_error("else syntax error");
        return Eval(node_array[node_array[node_array[root].right].left].right, HT);
    }
    //when it's user define funciton
    else if (node_array[HT->getLink(-node_array[root].left)].left == HT->getHashValue("lambda"))
    {
        int *array = new int[10];
        int howmany = 0;
        int param = node_array[node_array[HT->getLink(-node_array[root].left)].right].left;
        int input = node_array[root].right;
        while (param != 0) //remember the link of parameter
        {
            array[howmany] = HT->getLink(-node_array[param].left);
            howmany++;
            param = node_array[param].right;
        }
        param = node_array[node_array[HT->getLink(-node_array[root].left)].right].left; //reset parameter
        while (param != 0)                                                              //set new link to parameter
        {
            int p = stoi(Eval(input, HT));
            if (p == 0)
                p = hash_capacity + 1;
            if (node_array[input].left == node_array[param].left) //if param == input, keep param as it is
                p = HT->getLink(-node_array[param].left);
            HT->setLink(-node_array[param].left, p);
            param = node_array[param].right;
            input = node_array[input].right;
        }
        string result = Eval(node_array[node_array[node_array[HT->getLink(-node_array[root].left)].right].right].left, HT); //°á°ú get
        param = node_array[node_array[HT->getLink(-node_array[root].left)].right].left;
        int temp = 0;
        while (param != 0) //get parameters back
        {
            HT->setLink(-node_array[param].left, array[temp]);
            temp++;
            param = node_array[param].right;
        }
        delete[] array;
        return result;
    }
    else if (token_index > 0) //if it's a list
    {
        return Eval(token_index, HT);
    }
    else
    {
        return Eval(token_index, HT);
    }
    return "0";
}

string Hash_Table::getSymbol(int hashKey)
{
    if (hashKey <= 0)
        return hash_table[-hashKey].Symbol;
    return "error";
}
void Node_Array::printList(int node, Hash_Table *HT)
{
    int root = node;
    cout << "'(";
    while (node_array[root].right != 0)
    {
        cout << HT->getSymbol(node_array[root].left) << " ";
        root = node_array[root].right;
    }
    cout << HT->getSymbol(node_array[root].left) << ")" << endl;
}
void Node_Array::print()
{
    cout << " ========== Memory Array ==========" << endl;
    for (int i = 1; i < current + 1; i++)
        cout << "node: " << i << " l:" << node_array[i].left << " r:" << node_array[i].right << endl;
}
void Hash_Table::print()
{
    cout << " ========== Hash_Table ==========" << endl;
    for (int i = 0; i < capacity; i++)
        if (!hash_table[i].Symbol.empty())
            cout << "index: " << i << " symbol: " << hash_table[i].Symbol << "\t link: " << hash_table[i].link << endl;
}

string Node_Array::readCommand(int root, Hash_Table *HT)
{
    if (root < 1)
        return "";
    string command = "";
    if (node_array[root].left > 0)
        command += "(" + readCommand(node_array[root].left, HT);
    else
        command += " " + HT->getSymbol(node_array[root].left);
    if (node_array[root].right > 0)
        command += readCommand(node_array[root].right, HT);
    else
        command += " " + HT->getSymbol(node_array[root].right);
    return command;
}
void Node_Array::free(int root) { current = root - 1; }

string preprocess(string *input, string newcommand)
{
    string token = next_token(input);
    while (!token.empty())
    {
        if (token == "define") // hash value compare?
        {
            newcommand = newcommand + " define";
            token = next_token(input);
            if (token == "(")
            {
                token = next_token(input);
                newcommand = newcommand + " " + token + " (lambda (" + preprocess(input, "") + ")";
            }
            else
                newcommand = newcommand + " " + token;
        }
        else if (token == "'")
        {
            newcommand = newcommand + " (quote";
            int num_of_left_paren = 0;
            do
            {
                token = next_token(input);
                newcommand = newcommand + " " + token;
                if (token == "(")
                    num_of_left_paren++;
                else if (token == ")")
                    num_of_left_paren--;
            } while (num_of_left_paren > 0);
            newcommand = newcommand + " )";
        }
        else
            newcommand = newcommand + " " + token;
        token = next_token(input);
    }
    return newcommand;
}
unsigned int StringToInt(string s)
//following the pseudo code
{
    int length = (int)s.length();
    unsigned int answer = 0;
    if (length % 2 == 1)
    {
        answer = s.at(length - 1);
        length--;
    }
    for (int i = 0; i < length; i += 2)
    {
        answer += s.at(i);
        answer += ((int)s.at(i + 1)) << 8;
    }
    return answer;
};

string next_token(string *sentence)
{
    if (sentence->empty())
        return "";
    int k = sentence->find(" ");
    if (k == 0) //remove useless space
        return next_token(&(*sentence = sentence->substr(1, -1)));
    string symbol = sentence->substr(0, k);
    if (symbol.at(0) == '(' || symbol.at(0) == ')' || symbol.at(0) == '\'') // separating (state to ( state
    {
        symbol = symbol.at(0);
        *sentence = sentence->substr(1, -1);
        return symbol;
    }
    if (symbol.back() == ')' || symbol.back() == '(') //separating state) to state )
    {
        while (symbol.back() == ')' || symbol.back() == '(')
            symbol.pop_back();
        *sentence = sentence->substr(symbol.length(), -1);
        return symbol;
    }
    if (k != -1)
        *sentence = sentence->substr(k, -1);
    if (k == -1)
        *sentence = "";
    return symbol;
}

int isNumber(string symbol)
{
    bool float_point = false;
    if (isdigit(symbol[0]) || (symbol.at(0) == '-' && symbol.length() != 1)) // minus checking
        for (int i = 1; i < symbol.length(); i++)
        {
            if (!isdigit(symbol[i]))
            {
                if (symbol.at(i) == '.' && float_point == false) //float checking
                    float_point = true;
                else
                    return false;
            }
        }
    else
        return false;
    if (float_point)
        return 2;
    return 1;
}

string tolower(string input)
{
    int length = input.length();
    string output = "";
    char onec;
    for (int i = 0; i < length; i++)
        output += onec = (input.at(i) >= 'A' && input.at(i) <= 'Z') ? input.at(i) + 32 : input.at(i);
    return output;
}

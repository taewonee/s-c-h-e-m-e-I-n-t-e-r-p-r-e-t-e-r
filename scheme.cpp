//Data_Structure_Project1_Scheme Interpreter_2016-12161_Taewon Lee
//Hash_Table, Node_Array are implemented with class, snake_Case
//Other function: camelCase
//all command should start with '('
//doesn't return the input when input doesn't start with '('
//since it doesn't make Node_Array
//variable(variable is one variable
//���� user define function ���� ���ϴ���
//nil�� 0���� �ٲ㼭 ����� )�� �ȵǴ���
#include <iostream>
#include <cstring>
#include <cctype>
#include <stack> //change
using namespace std;
//constructor will be added as the project goes on
class Hash_Table
{
public:
    int getHashValue(string symbol); //check the value is in hash & read from it
    void print();                    //print the hash_table
    string getSymbol(int hashKey);
    int setLink(int address, int ptr);
    bool isUserDef(int address);
    int getLink(int address);
    stack<int> stackk;
    Hash_Table()
    {
        hash_table[0].Symbol = "nil";
        hash_table[0].link = 0;
    };

private:
    int capacity = 100;
    class hash_cell
    {
        friend Hash_Table;
        string Symbol;
        int link;
    };
    hash_cell *hash_table = new hash_cell[capacity];
};
//nextRoot is controlled by variable current(root at now)
//constructor will be added as the project goes on
class Node_Array
{
public:
    int Read(string *token, Hash_Table *HT);      //read the scheme command(build the structure)
    void print();                                 //print the Node_Array
    int nextRoot() { return current + 1; }        //return the Root of free List
    string readCommand(int root, Hash_Table *HT); //return the command according to the Node_array built
    void free(int root);
    string Eval(int root, Hash_Table *HT);
    int allocateNew();
    void printList(int node, Hash_Table *HT);
    int checkStructure(int rootA, int rootB);

private:
    class node_cell
    {
        friend Node_Array;
        int left, right;
    };
    int capacity = 100;
    int current = 0;
    node_cell *node_array = new node_cell[capacity];
};

//functions needed
string tolower(string input);                        //change CAPITAL to lower case
unsigned int StringToInt(string s);                  //change string to int, to get a hash_key
string preprocess(string *input, string newcommand); // will be added as the project goes on
string next_token(string *sentence);
int isNumber(string p); //0: non-number 1: int 2: float
int main()
{
    Hash_Table HT = Hash_Table();
    Node_Array NA;
    while (true)
    {
        string input;
        cout << "> ";
        getline(cin, input);
        input = tolower(input);
        string newcommand = "";
        newcommand = preprocess(&input, newcommand);
        cout << "preprocessed: " << newcommand << endl;
        int root = NA.Read(&newcommand, &HT);
        int printRoot = (root < 0) ? -1 : root;
        cout << "] Free list's root = " << NA.nextRoot() << endl;
        cout << "List's root = " << printRoot << endl;
        NA.print();
        HT.print();

        string result = NA.Eval(root, &HT);
        int result_int = stoi(result);
        cout << "result: " << result_int << endl;
        cout << "[[[ ";
        if (result_int < 0)
            cout << HT.getSymbol(result_int) << endl;
        if (result_int > 0)
            NA.printList(result_int, &HT);
        cout << " ]]]";
        if (root > 0)
        {
            cout << "input: (" << NA.readCommand(root, &HT) << endl; //print the input
                                                                     //            NA.free(root);                                           //deallocate the memory array
        }
    }
    return 0;
}

int Node_Array::checkStructure(int rootA, int rootB)
{
    cout << "rA: " << rootA << " rB: " << rootB << endl;

    if (node_array[rootA].left <= 0 && node_array[rootA].right <= 0)
        if (node_array[rootA].left == node_array[rootB].left && node_array[rootA].right == node_array[rootB].right)
            return 1;
    if (node_array[rootA].left <= 0)
    {
        if (node_array[rootA].left != node_array[rootB].left)
            return 0;
        else
            return checkStructure(node_array[rootA].right, node_array[rootB].right);
    }

    if (node_array[rootA].right <= 0)
    {
        if (node_array[rootA].right != node_array[rootB].right)
            return 0;
        else
            return checkStructure(node_array[rootA].left, node_array[rootB].left);
    }
    return 1;
}
int Node_Array::allocateNew()
{
    current++;
    if (current == capacity)
        throw runtime_error("node is full for now");
    return current;
}

bool Hash_Table::isUserDef(int address)
{
    if (hash_table[address].link > 0)
        return 1;
    return 0;
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
                root = temp = allocateNew();
                first = false;
            }
            else
            {
                //node_array[temp].right = alloc();
                node_array[temp].right = allocateNew();
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
//string CheckStructure(string a, string b)
//{
//}
//Eval�� string�̴�. (�̰� ���� �ᱹ)

string Node_Array::Eval(int root, Hash_Table *HT)
{
    if (root < 0) //if it's a leaf
    {
        //        cout << "getLInk, root: " << root << endl;
        //        cout << HT->getLink(-root) << endl;
        if (HT->getLink(-root))
        {
            //            cout << "link exist";
            return Eval(HT->getLink(-root), HT);
        }
        //        cout << "root: " << to_string(root) << endl;
        return to_string(root);
    }
    int token_index = node_array[root].left; //should check root->left first to know what the root is;
    // minus ����� ���� ¥���Ѵ�. �ٵ� ���⼭ getValue�� ¥���Ѵ� ����;
    cout << "token_index: " << token_index << endl;
    if (token_index == HT->getHashValue("+"))
    {
        //        cout << "+ called" << endl;
        string l = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        string r = HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)));
        if (isNumber(l) == 1 && isNumber(r) == 1)
            return to_string(HT->getHashValue(to_string(stoi(l) + stoi(r))));
        else
            return to_string(HT->getHashValue(to_string(stof(l) + stof(r))));
    }
    if (token_index == HT->getHashValue("-"))
        return to_string(HT->getHashValue(to_string(stof(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) - stof(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    if (token_index == HT->getHashValue("*"))
        return to_string(HT->getHashValue(to_string(stof(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) * stof(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    if (token_index == HT->getHashValue("/"))
        return to_string(HT->getHashValue(to_string(stof(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) / stof(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    if (token_index == HT->getHashValue("%"))
        return to_string(HT->getHashValue(to_string(stoi(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))) % stoi(HT->getSymbol(stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT)))))));
    /*
    else if (token_index == HT->getHashValue("iseq"))
    {
        string l = Eval(node_array[node_array[root].right].left, HT);
        string r = Eval(node_array[node_array[node_array[root].right].right].left, HT);
        cout << endl;
        cout << l << endl;
        cout << r << endl;
        if (l == r)
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("isequal"))
    {
        int l = node_array[node_array[root].right].left;
        int r = node_array[node_array[node_array[root].right].right].left;
        cout << endl
             << l << endl;
        cout << r << endl;
        if (checkStructure(l, r))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    */
    // don't consider the error case(parameter error) e.g) (isnumber) or (isnumber 2  a)
    else if (token_index == HT->getHashValue("number?"))
    {
        cout << "number called " << HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT))) << endl;
        if (isNumber(HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)))))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("symbol?"))
    {
        string result = HT->getSymbol(stoi(Eval(node_array[node_array[root].right].left, HT)));
        cout << "is symbol? " << result << endl;
        if (!isNumber(result))
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("null?")) //change
    {
        //        cout << "isnull: " << node_array[root].right << " " << node_array[root].right << endl;
        if (node_array[root].right == 0 || Eval(node_array[node_array[root].right].left, HT) == "0") //NIL should be 0 from constructor;
            return to_string(HT->getHashValue("#t"));
        else
            return to_string(HT->getHashValue("#f"));
    }
    else if (token_index == HT->getHashValue("car"))
        return to_string(node_array[stoi(Eval(node_array[node_array[root].right].left, HT))].left);
    else if (token_index == HT->getHashValue("cdr"))
    {
        return to_string(node_array[stoi(Eval(node_array[node_array[root].right].left, HT))].right);
    }
    else if (token_index == HT->getHashValue("define")) //how to know this is function define?
    {
        //        cout << "define called" << endl;
        //��ġ�� define ���Ŀ� ���� ���� �ִ� �κ��� ���� eval�ϸ� �� �� �� ������... �������־��� �� ���⵵? ���Ŀ� call ���� �� eval�� ���̴�
        if (node_array[node_array[node_array[node_array[root].right].right].left].left == HT->getHashValue("lambda"))
        {
            //            cout << "node: " << node_array[node_array[node_array[node_array[root].right].right].left].left << " " << HT->getHashValue("lambda") << endl;
            HT->setLink(-node_array[node_array[root].right].left, node_array[node_array[node_array[root].right].right].left); //��� index�� ����
                                                                                                                              //            cout << "link: " << -node_array[node_array[root].right].left << " " << node_array[node_array[node_array[root].right].right].left << endl;
        }
        else
        {
            HT->setLink(-node_array[node_array[root].right].left, node_array[node_array[root].right].right); //����� check
        }
    }
    else if (token_index == HT->getHashValue("quote"))
    {
        //        cout << "quote called: " << node_array[node_array[root].right].left << endl;
        return to_string(node_array[node_array[root].right].left);
    }
    else if (token_index == HT->getHashValue("cons"))
    {
        int newmemory = allocateNew();
        node_array[newmemory].left = stoi(Eval(node_array[node_array[root].right].left, HT));
        node_array[newmemory].right = stoi(Eval(node_array[node_array[node_array[root].right].right].left, HT));
        return to_string(newmemory);
    }
    else if (token_index == HT->getHashValue("cond")) //to change
    {
        while (node_array[node_array[root].right].right != 0) //this is nil
        {
            root = node_array[root].right;
            cout << "COnd Eval: " << node_array[node_array[root].left].left << endl;
            cout << "to string #t: " << to_string(HT->getHashValue("#t")) << endl;
            string p = Eval(node_array[node_array[root].left].left, HT);
            cout << "Eval Result: " << p << endl;
            if (p == to_string(HT->getHashValue("#t")))
            {
                cout << "cond true!" << endl;
                return Eval(node_array[node_array[node_array[root].left].right].left, HT);
            }
        }
        cout << "else here? " << node_array[root].right << endl;
        cout << "then what else: ? " << node_array[node_array[node_array[node_array[root].right].left].left].left << endl;
        cout << "else: " << HT->getHashValue("else") << endl;
        if (node_array[node_array[node_array[root].right].left].left != HT->getHashValue("else"))
            throw runtime_error("else syntax error");
        return Eval(node_array[node_array[node_array[root].right].left].right, HT);
    }
    //�̚j �� minus�� ���� ���ѵ�
    //else if (HT->isUserDef(-token_index)) //user define function? link�� ����?
    else if (node_array[HT->getLink(-node_array[root].left)].left == HT->getHashValue("lambda"))
    {
        //        cout << "user define function called" << endl;
        //        HT->stackk.push(HT->getLink(-token_index));
        //        HT->setLink(-token_index, 3);
        int *array = new int[10];
        int howmany = 0;
        int param = node_array[node_array[HT->getLink(-node_array[root].left)].right].left;
        int input = node_array[root].right;
        while (input != 0) //setting param as input variable
        {
            array[howmany] = HT->getLink(-node_array[param].left);
            howmany++;
            HT->setLink(-node_array[param].left, input);
            param = node_array[param].right;
            input = node_array[input].right;
            //            cout << "parameter: " << param << " "
            //                 << "input: " << input << endl;
        }
        string result = Eval(node_array[node_array[node_array[HT->getLink(-node_array[root].left)].right].right].left, HT);

        param = node_array[node_array[HT->getLink(-node_array[root].left)].right].left;
        input = node_array[root].right;
        while (input != 0) //setting param as input variable
        {
            int temp = 0;
            HT->setLink(-node_array[param].left, array[temp]);
            temp++;
            howmany--;
            HT->setLink(-node_array[param].left, input);
            param = node_array[param].right;
            input = node_array[input].right;
        }

        //resetting        HT->setLink(token_index, HT->stackk.top());
        return result;
    }
    //userdefinefunction ��ĥ��
    else if (token_index > 0) //list�� ����
    {
        cout << "it's a list!" << endl;
        cout << to_string(token_index);
        return to_string(token_index);
    }
    else
    {
        cout << "else called" << endl;
        return Eval(token_index, HT); //list�� ���� ó�� �Ұ���
                                      //        cout << "else called, token_index: " << token_index << endl;
                                      //        if (node_array[root].right == 0)
                                      //        {
                                      //           cout << "returing token index!" << endl;
                                      //          return to_string(token_index);
                                      //     }
                                      //   else
                                      //     return to_string(root);
    }
    cout << "now here?" << endl;
    //stack
    return "0";
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

string Hash_Table::getSymbol(int hashKey)
{
    if (hashKey <= 0)
        return hash_table[-hashKey].Symbol;
    return "error";
}
void Node_Array::printList(int node, Hash_Table *HT)
{
    int root = node;
    while (node_array[root].right != 0)
    {
        cout << HT->getSymbol(node_array[root].left) << " ";
        root = node_array[root].right;
    }
    cout << HT->getSymbol(node_array[root].left);
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

string tolower(string input)
{
    int length = input.length();
    string output = "";
    char onec;
    for (int i = 0; i < length; i++)
        output += onec = (input.at(i) >= 'A' && input.at(i) <= 'Z') ? input.at(i) + 32 : input.at(i);
    return output;
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
//���� ��� �ڵ� ���� �ŷ� ������ preprocess���� 2������ �ʿ��ѵ�, quote�� define ó���ϴ°Ŵ�
// �׷��� �׳� ù ������ preprocess�� �����ָ� �װ� �ٲ� �� ���� �� ������,
// �׿� ���ÿ� ������Ե� next_token���� �и�? �۾��� ���ش� ���� �� ���� ��ߴ�
//1. tokenizer�� ���� ��� => �ϴ� �ϳ��ϳ� �� ��ū ���� �и�
//2. PreProcess�� �ϸ� �� ��ū���� �����鼭 ���� �̰�

// 3' 40''
//preprocess�� ¥���ϳ�? ���� ¥���Ѵ�
//quote �� lambda �̷��͵��� �ʿ��ѵ�, ���� tokenizer�� ���� �־����
//�׷��� preprocess�� �� ���� �ʿ��� �͵鸸 �����,
//tokenizer�� ���� ���� �־��� �� �ֵ��� �����ؾ��ҵ� ����
//���⼭ ���� �׷��� tokenizer�� �ϴ� ���� ����� �ߴ� ����
// �׷� å������ �� ���ΰ�?
//���� �ƹ����� ���� tokenizer�� �־�� define����� �� �� �� �ֵ� ����?

//allocate �Լ��� �ʿ��ұ�?

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
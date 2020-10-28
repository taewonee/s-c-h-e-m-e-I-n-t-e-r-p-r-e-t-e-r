// hash table funciton 먼저 만들 것 10월 27일의 목표
// 즉, symbol로 먼저 나누어야함 ㅇㅇ
// symbol을 나누는 기준: ' ' 가 들어오면 ' ' 이전의 모든 녀석들이 같은 symbol
// '('가 들어오면 ')'가 들어오기 전까지??
// ex. (define (square x) (* x x))
// 괄호는 아직 잘 모르겠고,,, 음 일단 ' ' 가 있으면 그 전까지는 symbol이 맞는듯
// 그럼일단 main에서 한줄로 받아서, hash하고 바로 조져야하나?
// 아님 main에서 symbol로 변환해야하나?
// 먼저 line 을 받는다
// '(', ' '를 기준으로 symbolize 한다(하나만)
// symbol 하나 들어오면, 그걸 hash로 보낸다
// hash table update or hash table에서 불러온다
// tree node에 추가한다(이걸 어떻게?)
// '(' 면 일단 current -> right = new node, new node -> left 가 다음녀석, new node ->right = ();
// 헉 근데 시작 '('는 바로 전개 해버리네 ㅇㅇ 참고하자

#include <iostream>
#include <cstring>
using namespace std;
unsigned int StringToInt(string s);
class Hash_Table
{
public:
    int hashFind(string hashKey);
    void print();

private:
    int capacity = 10;
    class hash_cell
    {
        friend Hash_Table;
        string Symbol;
        hash_cell *link;
    };
    hash_cell *hash_table = new hash_cell[capacity];
    //    hash_cell* hash_table = new hash_cell[10];
};
class Node_Array
{
public:
    void nodearrayprocess(string input);
    int Read(string *token, Hash_Table *HT);
    void print();

private:
    class node_cell
    {
        friend Node_Array;
        int left;
        int right;
    };
    int capacity = 300;
    int current = 0;
    node_cell *node_array = new node_cell[capacity];
    //constructor로 옮겨야한다
};
// get a hash value;
int Hash_Table::hashFind(string hashKey)
{
    int hashValue = StringToInt(hashKey) % capacity;
    //   cout << "STI: " << StringToInt(hashKey);
    //   cout << "HV is: " << hashValue;
    int temp = hashValue;
    for (int i = 0; i < capacity; i++)
    {
        if (hashKey.compare(hash_table[hashValue].Symbol) == 0 || hash_table[hashValue].Symbol.empty())
        {
            hash_table[hashValue].Symbol = hashKey;
            return -hashValue;
        }
        hashValue = (hashValue + 1) % capacity;
    }
    throw runtime_error("hash is full for now");
    //    throw(string("hash is full for now"));
};
//constructor 필요
//
unsigned int StringToInt(string s)
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
    if (symbol.at(0) == '(') //check the first char is '(' thinking about "(variable" or "( variable"
    {
        symbol = "(";
        *sentence = sentence->substr(1, -1);
        return symbol;
    }
    if (symbol.at(0) == ')')
    {
        symbol = ")";
        *sentence = sentence->substr(1, -1);
        return symbol;
    }
    if (symbol.back() == ')')
    {
        while (symbol.back() == ')')
        {
            symbol.pop_back();
        }
        *sentence = sentence->substr(symbol.length(), -1);
        return symbol;
    }
    if (k != -1)
        *sentence = sentence->substr(k, -1);
    if (k == -1)
        *sentence = "";
    return symbol;
}

int main()
{
    Hash_Table HT;
    Node_Array NA;
    while (true)
    {
        string input;
        getline(cin, input);
        string line = input;
        //    input = "( define (square x (* x x)))";
        NA.Read(&input, &HT);
        HT.print();
        NA.print();
        cout << "input: " << line << endl;
    }
    return 0;
}

//흠 일단 current는 보존

int Node_Array::Read(string *input, Hash_Table *HT)
{
    int root = 0;
    bool first = true;
    int token_hash_value = HT->hashFind(next_token(input));
    if (token_hash_value == HT->hashFind("("))
    {
        int temp;
        token_hash_value = HT->hashFind(next_token(input));
        while (token_hash_value != HT->hashFind(")"))
        {
            if (first)
            {
                //root = temp = alloc();
                current++;
                if (current == capacity)
                    throw runtime_error("node is full for now");
                root = temp = current;
                first = false;
            }
            else
            {
                //node_array[temp].right = alloc();
                current++;
                if (current == capacity)
                    throw runtime_error("node is full for now");
                node_array[temp].right = current;
                temp = node_array[temp].right;
            }
            if (token_hash_value == HT->hashFind("("))
            {
                *input = "(" + *input;
                node_array[temp].left = Read(input, HT);
            }
            else
                node_array[temp].left = token_hash_value;
            if (!first)
                node_array[temp].right = -1;
            token_hash_value = HT->hashFind(next_token(input));
        }
        return root;
    }
    else
        return token_hash_value;
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
            cout << "index: " << i << " symbol: " << hash_table[i].Symbol << endl;
}
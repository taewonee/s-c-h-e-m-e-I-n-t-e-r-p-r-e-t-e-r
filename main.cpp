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

class Hash_Table
{
public:
    int hashFind(string hashKey);
private:
    int capacity = 10;
    class hash_cell
    {
        friend Hash_Table;
        string Symbol;
        hash_cell *link;
    };
    hash_cell * hash_table = new hash_cell[capacity];
    //    hash_cell* hash_table = new hash_cell[10];
};

class Node_Array
{
public:
    void nodearrayprocess(string input);

private:
    class node_cell
    {
        int left;
        int right;
    };
    node_cell node_array[30];
    int current = 1;
    //constructor로 옮겨야한다
};

int main()
{
    Hash_Table HT;
    Node_Array NA;
    string input;
    cin >> input;
    //NA.NodeArrayProcess(input);

    return 0;
}

// get a hash value;
int Hash_Table::hashFind(string hashKey)
{
    int hashValue = StringToInt(hashKey) % capacity;
    int temp = hashValue;
    for(; hashValue<temp+capacity;hashValue++)
    {
        if (hash_table[hashValue].Symbol.compare(hashKey) || hash_table[hashValue].Symbol.empty())
        {
            hash_table[hashValue].Symbol = hashKey;
            return -hashValue;
        }
    }
    cout<<"Somethig is error";
    throw("hash is full for now");

};

void Node_Array::nodearrayprocess(string input)
{
    char k;
    string symbol; //이걸 hash로 먼저 바꿔놔야하네ㅇㅇ
    //symbol = hashvalue(Symbol);
    //hashvalue 불러오면서 없으면 node array update, return

    //    while(input[k] == ' ')
    //        symbol.(input[k]);
    if (symbol == "(") //여튼 (를 만났을 때
    {
        node_array[current].right = ++current;
        node_array[current].right = 0;
    }
    else if(0?) //걍 다른 symbol 만나씅ㄹ 때
    {
    }
    else
    { //대충 다른 symbol일때
        node_array[current].left = toHashValue(symbol);
        node_array[current].right = ++current;
    }
};
//constructor 필요
//

unsigned int StringToInt(string s){
    int length = (int)s.length();
    unsigned int answer = 0;
    if(length%2==1)
    {
        answer=s.at(length-1);
        length--;
    }
    for(int i =0; i<length;i+=2){
        answer+=s.at(i);
        answer+=((int)s.at(i+1))<<8;
    }
    return answer;
};

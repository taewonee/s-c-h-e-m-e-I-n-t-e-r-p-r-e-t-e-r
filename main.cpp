// hash table funciton ���� ���� �� 10�� 27���� ��ǥ
// ��, symbol�� ���� ��������� ����
// symbol�� ������ ����: ' ' �� ������ ' ' ������ ��� �༮���� ���� symbol 
// '('�� ������ ')'�� ������ ������?? 
// ex. (define (square x) (* x x))
// ��ȣ�� ���� �� �𸣰ڰ�,,, �� �ϴ� ' ' �� ������ �� �������� symbol�� �´µ�
// �׷��ϴ� main���� ���ٷ� �޾Ƽ�, hash�ϰ� �ٷ� �������ϳ�?
// �ƴ� main���� symbol�� ��ȯ�ؾ��ϳ�?
// ���� line �� �޴´�
// '(', ' '�� �������� symbolize �Ѵ�(�ϳ���)
// symbol �ϳ� ������, �װ� hash�� ������
// hash table update or hash table���� �ҷ��´�
// tree node�� �߰��Ѵ�(�̰� ���?)
// '(' �� �ϴ� current -> right = new node, new node -> left �� �����༮, new node ->right = ();
// �� �ٵ� ���� '('�� �ٷ� ���� �ع����� ���� ��������

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
    //constructor�� �Űܾ��Ѵ�
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
    string symbol; //�̰� hash�� ���� �ٲ�����ϳפ���
    //symbol = hashvalue(Symbol);
    //hashvalue �ҷ����鼭 ������ node array update, return

    //    while(input[k] == ' ')
    //        symbol.(input[k]);
    if (symbol == "(") //��ư (�� ������ ��
    {
        node_array[current].right = ++current;
        node_array[current].right = 0;
    }
    else if(0?) //�� �ٸ� symbol �������� ��
    {
    }
    else
    { //���� �ٸ� symbol�϶�
        node_array[current].left = toHashValue(symbol);
        node_array[current].right = ++current;
    }
};
//constructor �ʿ�
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

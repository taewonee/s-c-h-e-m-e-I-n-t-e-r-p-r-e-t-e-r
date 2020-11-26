Scheme Interpreter ver 2.
Windows10, g++ version 8.1.0

- Steps:
1. get a line as an input
2. preprocess(input)
    : remove meaningless parenthesis & change list -> quote, function -> lambda
3. build a node_tree class based on input
4. build a hash_table class based on input
5. Evaluation
6. Print

 - Additional Info :
1. suppose there is no input error for now (always start with '(')
2. node Capacity: 100, Hash Capacity: 500;
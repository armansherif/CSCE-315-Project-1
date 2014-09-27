#ifndef NODE_H
#define NODE_H

#include <string>
#include <Token.h>
using namespace std;

class Node{

private:
	Node* Left;
	Node* Right;
public:
	Token_Type Type;
	string Value;

	Node();
	Node(string value, Token_Type type, Node *left=NULL, Node *right=NULL);
	Node(const Node& node);

	//Getters
	Node& Get_left();
	Node& Get_right();
	//Setters
	void Set_left(Node n);
	void Set_right(Node n);
	
	//check if node is int or string
	bool Is_literal() const;

	ostream& Print_node(ostream& os);
};

#endif
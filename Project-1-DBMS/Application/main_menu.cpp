#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include "main_menu.h"

using namespace std;


char check_input(char input, char low, char high){			// Used to check if input is within range of desired inputs
	bool check = false;
	char in;
	if ((input <= high) && (input >= low)){
		check = true;
	}
	while (!check){
		cout << "* Command not found, Please try again: ";
		cin >> in;
		if ((in <= high) && (in >= low)){
			input = in;
			check = true;
		}
	}
	return input;
}

void main_menu(DBParser& dbparser){

	char input, temp;

	cout << endl << "[MAIN MENU]" << endl << endl;
	cout << "1. Address Book" << endl;
	cout << "2. Calendar" << endl;
	cout << "3. Memo Pad" << endl;
	cout << "4. Todo List" << endl << endl;
	cout << "* Enter app to use: ";
	cin >> input;
	temp = check_input(input, '1', '4');

	switch (temp){
	case '1':
		Address_Book(dbparser);
		break;
	case '2':
		Calendar(dbparser);
		break;
	case '3':
		Memo_Pad(dbparser);
		break;
	case '4':
		Todo_List(dbparser);
		break;
	default:
		cerr << "Input Error!" << endl;
	}
}


// --------------------------------------------------------------
//			Converting into correct Query Formats
// --------------------------------------------------------------
string query_for_addressbook_insert(string table, string name, string phone, string email, string address){
	string temp = "INSERT INTO ";
	temp = temp + table + " " + "VALUES FROM (";
	temp = temp + R"delim(")delim" + name + R"delim(")delim" + ", ";				// The delim lets us use quotes in our string;
	temp = temp + R"delim(")delim" + phone + R"delim(")delim" + ", ";
	temp = temp + R"delim(")delim" + email + R"delim(")delim" + ", ";
	temp = temp + R"delim(")delim" + address + R"delim(")delim" + ");";

	// OUTPUT = INSERT INTO table VALUES FROM("name", "phone", "email", "address");
	return temp;
}
string query_for_addressbook_delete(string table, string name){
	string temp = "DELETE FROM ";
	temp = temp + table + " " + "WHERE((name == ";
	temp = temp + R"delim(")delim" + name + R"delim(")delim" + "));";				// The delim lets us use quotes in our string;
	cout << temp << endl;
	return temp;
}
// --------------------------------------------------------------
//			Address Book Functions
// --------------------------------------------------------------

void retrieve_record(string _name){					// not finished need to actually retrieve record from Database!!
	string name, phone, email, address;
	cout << endl << "Retrieved Record" << endl;
	cout << "1. Name: " << name << endl; 
	cout << "2. Phone: " << phone << endl;
	cout << "3. Email: " << email << endl; 
	cout << "4. Address: " << address << endl;
}

string create_contact(){
	string query;
	string name, phone, email, address;

	cout << endl << "Creating New Contact" << endl;
	cout << "* Enter Name: ";
	cin >> name;										// CANT HANDLE SPACES RIGHT NOW!!!!!!!!!!!
	cout << "* Enter Phone Number With No Spaces or Dashes (1234567890): ";
	cin >> phone;
	cout << "* Enter Email: ";
	cin >> email;
	cout << "* Enter Address: ";							// CANT HANDLE SPACES RIGHT NOW!!!!!!!!!!!
	cin >> address;
	
	query = query_for_addressbook_insert("addressbook", name, phone, email, address);
	return query;
}
/*
string edit_contact(){
	string query;
	string name;

	cout << "*Enter name to edit: ";
	cin >> name;
	cout << endl << "Retrieved Record";
	
}*/

string delete_contact(){						// ERROR IN WHILE LOOP GOTTA FIX RAN OUT OF TIME
	bool check = false;
	string name, answer;
	string query = "";
	cout << "* Enter Name to delete: ";
	cin >> name;
	retrieve_record(name);
	while (!check){
		cout << "Are you sure you want to delete this record <y/n>: ";
		cin >> answer;
		if (answer != "y" || answer != "n"){
			cout << "Incorrect Answer Please Try Again <y/n>: ";
			cin >> answer;
		}
		else if ((answer == "y") || (answer == "n"))
			check = true;
	}
	if (answer == "y")
		query = query_for_addressbook_delete("addressbook",name);
	if (answer == "n"){
		cout << "No record Deleted Returning to Main Menu" << endl;
	}
	return query;
}

void Address_Book(DBParser& dbparser){
	char input, temp; 
	string query;
	string name, phone, email, address;
	string debug;

	cout << endl << "[Address Book Menu]" << endl << endl;
	cout << "1. Display list" << endl;
	cout << "2. Search" << endl;
	cout << "3. Edit" << endl;
	cout << "4. Create" << endl;
	cout << "5. Delete" << endl;
	cout << "6. Return to main menu" << endl << endl;
	cout << "* Enter command: ";
	cin >> input;
	temp = check_input(input, '1', '6');

	switch(temp){
		case '1':
		cout << "Displaying Address Book list " << endl;
		dbparser.execute_query("SHOW addressbook;");
		break;
	case '2':
		cout << "Searching in Address Book" << endl;
		cout << "* Enter Desired Name to Search For: ";
		cin >> name;
		break;
	case '3':
		cout << endl << "[Address Book Edit]" << endl << endl;
		break;
	case '4':
		cout << endl << "[Address Book Create]" << endl << endl;
		query = create_contact();
		cout << endl << query << endl;
		dbparser.execute_query(query);
		main_menu(dbparser);
		break;
	case '5':
		cout << endl << "[Address Book Delete]" << endl << endl;
		query = delete_contact();
		cout << endl << query << endl;
		dbparser.execute_query(query);
		main_menu(dbparser);
		break;
	case '6':
		main_menu(dbparser);
		break;
	default:
		cerr << "Input Error in Address book!" << endl;
	}
}

void Calendar(DBParser& dbparser){
	char input, temp;
	cout << endl << "[Calendar Menu]" << endl << endl;
	cout << "1. Display list" << endl;
	cout << "2. Search" << endl;
	cout << "3. Edit" << endl;
	cout << "4. Return to main menu" << endl << endl;
	cout << "* Enter command: ";
	cin >> input;
	temp = check_input(input, '1', '4');

	switch (temp){
	case '1':
		cout << "Display list CHECK" << endl;
		break;
	case '2':
		cout << "Search CHECK" << endl;
		break;
	case '3':
		cout << "Edit CHECK" << endl;
		break;
	case '4':
		main_menu(dbparser);
		break;
	default:
		cerr << "Input Error in Calendar!" << endl;
	}
}

void Memo_Pad(DBParser& dbparser){
	char input, temp;
	cout << endl << "[Memo Pad Menu]" << endl << endl;
	cout << "1. Display list" << endl;
	cout << "2. Search" << endl;
	cout << "3. Edit" << endl;
	cout << "4. Create" << endl;
	cout << "5. Delete" << endl;
	cout << "6. Return to main menu" << endl << endl;
	cout << "* Enter command: ";
	cin >> input;
	temp = check_input(input, '1', '6');

	switch (temp){
	case '1':
		cout << "Display list CHECK" << endl;
		break;
	case '2':
		cout << "Search CHECK" << endl;
		break;
	case '3':
		cout << "Edit CHECK" << endl;
		break;
	case '4':
		cout << "Create CHECK" << endl;
		break;
	case '5':
		cout << "Delete CHECK" << endl;
		break;
	case '6':
		main_menu(dbparser);
		break;
	default:
		cerr << "Input Error in Memo!" << endl;
	}
}

void Todo_List(DBParser& dbparser){
	char input, temp;
	cout << endl << "[Todo List Menu]" << endl << endl;
	cout << "1. Display list" << endl;
	cout << "2. Search" << endl;
	cout << "3. Edit" << endl;
	cout << "4. Create" << endl;
	cout << "5. Delete" << endl;
	cout << "6. Return to main menu" << endl << endl;
	cout << "* Enter command: ";
	cin >> input;
	temp = check_input(input, '1', '6');

	switch (temp){
	case '1':
		cout << "Display list CHECK" << endl;
		break;
	case '2':
		cout << "Search CHECK" << endl;
		break;
	case '3':
		cout << "Edit CHECK" << endl;
		break;
	case '4':
		cout << "Create CHECK" << endl;
		break;
	case '5':
		cout << "Delete CHECK" << endl;
	case '6':
		main_menu(dbparser);
		break;
	default:
		cerr << "Input Error in Todo!" << endl;
	}
}

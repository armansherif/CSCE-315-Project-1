#include "DBParser.h"
#include <iostream>

using namespace std;

bool DBParser::execute_query(string query) {
	ts = new Token_stream(query);

	bool valid = program();
	delete ts;
	return valid;
}

//entry point 
//------------------------------------------------------------------------------
bool DBParser::program() {
	if (query()) {
		return true;
	}
	else if (command()) {
		return true;
	}
	else {
		return false;
	}
}

//query ::= relation_name <- expr ;
bool DBParser::query() {
	if (relation_name()) {
		Token assignment_token = ts->get();
		if (assignment_token.get_type() == _assign) {
			cout << " valid assign token\n\n";

			bool value = expr();
			if (value) {
				return true;
			}
			else return false;
		}
		else {
			ts->unget(assignment_token);
			return false;
		}
	}
	else return false;
}

//command ::= (open-cmd | close-cmd | write-cmd | exit-cmd | show-cmd | create-cmd | update-cmd | insert-cmd | delete-cmd);
bool DBParser::command() {
	bool value;

	if (open_cmd()) { value = true; }
	else if (close_cmd()) { value = true; }
	else if (write_cmd()) { value = true; }
	else if (exit_cmd()) { value = true; }
	else if (show_cmd()) { value = true; }
	else if (create_cmd()) { value = true; }
	else if (update_cmd()) { value = true; }
	else if (insert_cmd()) { value = true; }
	else if (delete_cmd()) { value = true; }
	else value = false;
	
	if (value) {
		Token semicolon_token = ts->get();
		if (semicolon_token.get_type() == _semicolon) {
			return true;
		}
		else {
			ts->unget(semicolon_token);
			return false;
		}
	}
	else {
		return false;
	}
}

//queries
//------------------------------------------------------------------------------

//selection ::= select (condition) atomic_expr
bool DBParser::selection() {
	Token select_token = ts->get();
	if (select_token.get_type() == _select) {
		Token lpar_token = ts->get();
		if (lpar_token.get_type() == _lpar) {
			if (condition()) {
				Token rpar_token = ts->get();
				if (rpar_token.get_type() == _rpar) {
					return atomic_expr();
				}
				else {
					ts->unget(rpar_token);
					ts->unget(lpar_token);
					ts->unget(select_token);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			ts->unget(lpar_token);
			ts->unget(select_token);
			return false;
		}
	}
	else {
		ts->unget(select_token);
		return false;
	}
}

//projection ::= project (attribute_list) atomic_expr
bool DBParser::projection() {
	Token project_token = ts->get();
	if (project_token.get_type() == _project) {
		Token lpar_token = ts->get();
		if (lpar_token.get_type() == _lpar) {
			if (attribute_list()) {
				Token rpar_token = ts->get();
				if (rpar_token.get_type() == _rpar) {
					return atomic_expr();
				}
				else {
					ts->unget(rpar_token);
					ts->unget(lpar_token);
					ts->unget(project_token);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			ts->unget(lpar_token);
			ts->unget(project_token);
			return false;
		}
	}
	else {
		ts->unget(project_token);
		return false;
	}
}

//renaming ::= rename (attribute_list) atomic_expr
bool DBParser::rename() {
	Token rename_token = ts->get();
	if (rename_token.get_type() == _rename) {
		Token lpar_token = ts->get();
		if (lpar_token.get_type() == _lpar) {
			if (attribute_list()) {
				Token rpar_token = ts->get();
				if (rpar_token.get_type() == _rpar) {
					return atomic_expr();
				}
				else {
					ts->unget(rpar_token);
					ts->unget(lpar_token);
					ts->unget(rename_token);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			ts->unget(lpar_token);
			ts->unget(rename_token);
			return false;
		}
	}
	else {
		ts->unget(rename_token);
		return false;
	}
}

//union ::= atomic_expr + atomic_expr
bool DBParser::set_union() {
	if (atomic_expr()) {
		Token plus_token = ts->get();
		if (plus_token.get_type() == _plus) {
			return atomic_expr();
		}
		else {
			ts->unget(plus_token);
			return false;
		}
	}
	else {
		return false;
	}
}

//difference ::= atomic_expr - atomic_expr
bool DBParser::difference() {
	if (atomic_expr()) {
		Token minus_token = ts->get();
		if (minus_token.get_type() == _minus) {
			return atomic_expr();
		}
		else {
			ts->unget(minus_token);
			return false;
		}
	}
	else {
		return false;
	}
}

//product ::= atomic_expr * atomic_expr 
bool DBParser::product() {
	if (atomic_expr()) {
		Token multiply_token = ts->get();
		if (multiply_token.get_type() == _multiply) {
			return atomic_expr();
		}
		else {
			ts->unget(multiply_token);
			return false;
		}
	}
	else {
		return false;
	}
}

//condition ::= conjunction {|| conjunction}
bool DBParser::condition() {
	if (conjunction()) {
		while (true) {
			Token or_token = ts->get();
			if (or_token.get_type() == _or) {
				if (conjunction()) continue;
				else {
					ts->unget(or_token);
					return false;
				}
			}
			else {
				ts->unget(or_token);
				return true;
			}
		}
	}
	else {
		return false;
	}
}

//conjunction ::= comparison {&& comparison}
bool DBParser::conjunction() {
	if (comparison()) {
		while (true) {
			Token and_token = ts->get();
			if (and_token.get_type() == _and) {
				if (comparison()) continue;
				else {
					ts->unget(and_token);
					return false;
				}
			}
			else {
				ts->unget(and_token);
				return true;
			}
		}
	}
	else {
		return false;
	}
}

//comparison ::= operand op operand | (condition)
bool DBParser::comparison() {
	if (operand()) {
		if (op()) {
			if (operand()) {
				return true;
			}
			else return false;
		}
		else {
			return false;
		}
	}
	else {
		Token lpar_token = ts->get();
		if (lpar_token.get_type() == _lpar) {
			if (condition()) {
				Token rpar_token = ts->get();
				if (rpar_token.get_type() == _rpar) {
					return true;
				}
				else {
					ts->unget(rpar_token);
					ts->unget(lpar_token);
					return false;
				}
			}
		}
		else {
			ts->unget(lpar_token);
			return false;
		}
	}
	return false;
}

//op ::= == | != | < | > | <= | >=
bool DBParser::op() {
	Token op_token = ts->get();
	switch (op_token.get_type()) {
	case _equals:
		return true;
	case _not_eq:
		return true;
	case _less:
		return true;
	case _greater:
		return true;
	case _less_eq:
		return true;
	case _greater_eq:
		return true;
	default:
		ts->unget(op_token);
		return false;
	}
}

//operand ::= attribute_name | literal
bool DBParser::operand() {
	if (attribute_name())
		return true;
	else if (literal()) {
		return true;
	}
	else return false;
}


//commands
//------------------------------------------------------------------------------

//open_cmd ::== OPEN relation_name 
bool DBParser::open_cmd() {
	Token open_token = ts->get();
	if (open_token.get_type() == _open) {
		return relation_name();
	}
	else {
		ts->unget(open_token);
		return false;
	}
}

//close_cmd ::== CLOSE relation_name 
bool DBParser::close_cmd() {
	Token close_token = ts->get();
	if (close_token.get_type() == _close) {
		return relation_name();
	}
	else {
		ts->unget(close_token);
		return false;
	}
}

//write_cmd ::== WRITE relation_name 
bool DBParser::write_cmd() {
	Token write_token = ts->get();
	if (write_token.get_type() == _write) {
		return relation_name();
	}
	else {
		ts->unget(write_token);
		return false;
	}
}

//exit_cmd ::== EXIT 
bool DBParser::exit_cmd() {
	Token exit_token = ts->get();
	if (exit_token.get_type() == _exit_program) {
		return true;
	}
	else {
		ts->unget(exit_token);
		return false;
	}
}

//show-cmd ::== SHOW atomic_expr 
bool DBParser::show_cmd() {
	Token show_token = ts->get();
	if (show_token.get_type() == _show) {
		if (atomic_expr()) {
			return true;
		}
		else return false;
	}
	else {
		ts->unget(show_token);
		return false;
	}
}

//create-cmd ::= CREATE TABLE relation_name (typed_attribute_list) PRIMARY KEY (attribute_list)
bool DBParser::create_cmd() {
	Token create_token = ts->get();
	if (create_token.get_type() == _create) {
		if (ts->get().get_type() == _table) {

			if (relation_name()) {

				if (ts->get().get_type() == _lpar) {
					if (typed_attribute_list()) {
						if (ts->get().get_type() == _rpar) {

							if (ts->get().get_type() == _primary) {
								if (ts->get().get_type() == _key) {

									if (ts->get().get_type() == _lpar) {
										if (attribute_list()) {
											if (ts->get().get_type() == _rpar) {
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}
	else {
		ts->unget(create_token);
		return false;
	}
}

//update_cmd ::= UPDATE relation_name SET attribute_name = literal { , attribute_name = literal } WHERE condition 
bool DBParser::update_cmd() {
	Token update_token = ts->get();
	if (update_token.get_type() == _update) {
		if (relation_name()) {

			if (ts->get().get_type() == _set) {

				if (attribute_name()) {
					if (ts->get().get_type() == _assign_eq) {

						if (literal()) {
							while (true) {
								Token comma_token = ts->get();
								if (comma_token.get_type() == _comma) {
									if (literal()) continue;
									else {
										ts->unget(comma_token);
										break;
									}
								}
								else {
									ts->unget(comma_token);
									
									if (ts->get().get_type() == _where) {
										if (condition()) {
											return true;
										}
										else break;
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}
	else {
		ts->unget(update_token);
		return false;
	}
}

//insert_cmd :: = INSERT INTO relation_name VALUES FROM(literal {, literal }) | INSERT INTO relation_name VALUES FROM RELATION expr
bool DBParser::insert_cmd() {
	return true;
}

//delete_cmd ::= DELETE FROM relation_name WHERE condition
bool DBParser::delete_cmd() {
	Token delete_token = ts->get();
	if (delete_token.get_type() == _delete) {
		Token from_token = ts->get();
		if (from_token.get_type() == _from) {
			if (relation_name()) {
				Token where_token = ts->get();
				if (where_token.get_type() == _where) {
					if (condition()) {
						return true;
					}
					else {
						ts->unget(where_token);
						ts->unget(from_token);
						ts->unget(delete_token);
						return false;
					}
				}
				else {
					ts->unget(from_token);
					ts->unget(delete_token);
					return false;
				}
			}
			else {
				ts->unget(from_token);
				ts->unget(delete_token);
				return false;
			}
		}
		else {
			ts->unget(from_token);
			ts->unget(delete_token);
			return false;
		}
	}
	else {
		ts->unget(delete_token);
		return false;
	}
}

//Data structures
//------------------------------------------------------------------------------

//relation_name ::= identifier
//identifier ::= alpha { ( alpha | digit ) } (read in as single token)
bool DBParser::relation_name() {
	Token relation_name_token = ts->get();
	if (relation_name_token.get_type() == _identifier) {
		return true;
	}
	else {
		ts->unget(relation_name_token);
		return false;
	}
}

//expr ::= atomic-expr | selection | projection | renaming | union | difference | product
bool DBParser::expr() {
	if (atomic_expr())
		return true;
	else if (selection())
		return true;
	else if (projection())
		return true;
	else if (rename())
		return true;
	else if (set_union())
		return true;
	else if (difference())
		return true;
	else if (product())
		return true;
	else 
		return false;
}

//atomic_expr ::= relation_name | (expr)
bool DBParser::atomic_expr() {
	Token lpar_token = ts->get();
	if (lpar_token.get_type() == _lpar) {
		if (expr()) {
			Token rpar_token = ts->get();
			if (rpar_token.get_type() == _rpar) {
				return true;
			}
			else {
				ts->unget(rpar_token);
				ts->unget(lpar_token);
				return false;
			}
		}
		else {
			ts->unget(lpar_token);
			return false;
		}
	}
	else {
		ts->unget(lpar_token);
		return relation_name();
	}
}

//attribute_name ::= identifier
bool DBParser::attribute_name() {
	Token identifier_token = ts->get();
	if (identifier_token.get_type() == _identifier) {
		return true;
	}
	else {
		ts->unget(identifier_token);
		return false;
	}
}

//type ::= VARCHAR(integer) | INTEGER
bool DBParser::type() {
	Token type_token = ts->get();
	if (type_token.get_type() == _int_type) {
		return true;
	}
	else if (type_token.get_type() == _float_type) {
		return true;
	}
	else if (type_token.get_type() == _varchar) {
		return true;
	}
	else {
		ts->unget(type_token);

		return false;
	}
}

//typed_attribute_list ::= attribute_name type {, attribute_name type}
bool DBParser::typed_attribute_list() {
	if (attribute_name()) {
		if (type()) {
			while (true) {
				Token comma_token = ts->get();
				if (comma_token.get_type() == _comma) {
					if (attribute_name() && type()) continue;
					else {
						ts->unget(comma_token);
						return false;
					}
				}
				else {
					ts->unget(comma_token);
					return true;
				}
			}
		}
	}
	else {
		return false;
	}
}

//attribute_list ::= attribute_name {, attribute_name} 
bool DBParser::attribute_list() {
	if (attribute_name()) {
		while (true) {
			Token comma_token = ts->get();
			if (comma_token.get_type() == _comma) {
				if (attribute_name()) continue;
				else {
					ts->unget(comma_token);
					return false;
				}
			}
			else {
				ts->unget(comma_token);
				return true;
			}
		}
	}
	else {
		return false;
	}
}

//literal ::= "identifier" | integer | float
bool DBParser::literal() {
	Token literal_token = ts->get();
	if (literal_token.get_type() == _quotation) {
		Token string_token = ts->get();
		if (string_token.get_type() == _identifier) {
			Token end_quote_token = ts->get();
			if (end_quote_token.get_type() == _quotation) {
				return true;
			}
			else {
				ts->unget(end_quote_token);
				ts->unget(string_token);
				ts->unget(literal_token);
				return false;
			}
		}
		else {
			ts->unget(string_token);
			ts->unget(literal_token);
			return false;
		}
	}
	else if (literal_token.get_type() == _int_num) {
		return true;
	}
	else if (literal_token.get_type() == _float_num) {
		return true;
	}
	else {
		ts->unget(literal_token);
		return false;
	}
}
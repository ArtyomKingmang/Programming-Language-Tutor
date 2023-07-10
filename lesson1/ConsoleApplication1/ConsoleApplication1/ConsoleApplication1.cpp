#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> lexer(const string& input) {
    vector<string> tokens;
    string token;

    for (char ch : input) {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, ch));
        }
        else if (isdigit(ch)) {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}


int parse(const std::vector<std::string>& tokens) {
    int result = 0;
    int i = 0;

    while (i < tokens.size()) {
        std::string token = tokens[i];

        if (token == "+") {
            result += std::stoi(tokens[i + 1]);
            i += 2;
        }
        else if (token == "-") {
            result -= std::stoi(tokens[i + 1]);
            i += 2;
        }
        else if (token == "*") {
            result *= std::stoi(tokens[i + 1]);
            i += 2;
        }
        else if (token == "/") {
            result /= std::stoi(tokens[i + 1]);
            i += 2;
        }
        else {
            result = std::stoi(token);
            i++;
        }
    }

    return result;
}


struct ASTNode {
    virtual ~ASTNode() = default;
    virtual int evaluate() const = 0;
};

struct NumberNode : public ASTNode {
    int value;

    explicit NumberNode(int value) : value(value) {}

    int evaluate() const override {
        return value;
    }
};

struct BinaryOperatorNode : public ASTNode {
    char op;
    ASTNode* left;
    ASTNode* right;

    BinaryOperatorNode(char op, ASTNode* left, ASTNode* right)
        : op(op), left(left), right(right) {}

    int evaluate() const override {
        switch (op) {
        case '+':
            return left->evaluate() + right->evaluate();
        case '-':
            return left->evaluate() - right->evaluate();
        case '*':
            return left->evaluate() * right->evaluate();
        case '/':
            return left->evaluate() / right->evaluate();
        default:
            return 0;
        }
    }
};


ASTNode* parseExpression(const std::vector<std::string>& tokens, int& i) {
    ASTNode* left = new NumberNode(std::stoi(tokens[i]));
    i++;

    while (i < tokens.size()) {
        std::string token = tokens[i];

        if (token == "+" || token == "-") {
            char op = token[0];
            i++;

            ASTNode* right = new NumberNode(std::stoi(tokens[i]));
            i++;

            left = new BinaryOperatorNode(op, left, right);
        }
        else if (token == "*" || token == "/") {
            char op = token[0];
            i++;

            ASTNode* right = new NumberNode(std::stoi(tokens[i]));
            i++;

            left = new BinaryOperatorNode(op, left, right);
        }
        else {
            break;
        }
    }

    return left;
}

int main() {
    while (true) {
        std::string input;
        std::cout << "\nEnter: ";
        std::getline(std::cin, input);

        std::vector<std::string> tokens = lexer(input);
        int i = 0;
        ASTNode* root = parseExpression(tokens, i);

        int result = root->evaluate();
        std::cout << "Result: " << result << std::endl;

        delete root;
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

#include "Token.hpp"

class LexicalAnalyzer
{
public:
  LexicalAnalyzer(std::istream &input) : input(input) {}

  std::vector<Token> analyze()
  {
    std::vector<Token> tokens;

    while (this->input.peek() != EOF)
    {
      char currentSymbol = this->input.peek();

      if (isWhitespace(currentSymbol))
      {
        tokens.push_back(this->readWhitespace());
      }
      else if (currentSymbol == '/')
      {
        tokens.push_back(this->readComment());
      }
      else if (currentSymbol == '\'' || currentSymbol == '"')
      {
        tokens.push_back(this->readStringLiteral());
      }
      else if (isDigit(currentSymbol))
      {
        tokens.push_back(this->readNumberLiteral());
      }
      else if (isWordSymbol(currentSymbol))
      {
        tokens.push_back(this->readWord());
      }
      else
      {
        tokens.push_back(this->readOperator());
      }
    }

    return tokens;
  }

private:
  std::istream &input;

  char peek()
  {
    return this->input.peek();
  }

  void moveForward()
  {
    this->input.seekg(1, this->input.cur);
  }

  bool readSpecificWord(const std::string &word)
  {
    for (int i = 0; i < word.length(); i++)
    {
      if (this->peek() != word[i])
      {
        this->input.seekg(-i, this->input.cur);
        return false;
      }

      this->moveForward();
    }

    return true;
  }

  Token readWhitespace()
  {
    std::string content = "";

    while (this->peek() != EOF && this->isWhitespace(this->peek()))
    {
      content += this->peek();
      this->moveForward();
    }

    return Token(content, TokenType::WHITESPACE, false);
  }

  Token readComment()
  {
    this->moveForward();
    char secondSymbol = this->peek();
    this->moveForward();

    if (secondSymbol == '*')
    {
      std::string content = "/";
      content += secondSymbol;

      bool isInvalid = true;

      while (this->peek() != EOF)
      {
        content += this->peek();
        this->moveForward();

        if (this->peek() == '*')
        {
          content += this->peek();
          this->moveForward();

          if (this->peek() == '/')
          {
            content += this->peek();
            this->moveForward();
            isInvalid = false;
            break;
          }
        }
      }

      return Token(content, TokenType::COMMENT, isInvalid);
    }
    else
    {
      std::string content = "/";
      content += secondSymbol;

      while (this->peek() != EOF && !this->isEndOfLine(this->peek()))
      {
        content += this->peek();
        this->moveForward();
      }

      return Token(content, TokenType::COMMENT, secondSymbol != '/');
    }
  }

  Token readStringLiteral()
  {
    char quoteSymbol = this->peek();
    this->moveForward();
    std::string content;
    content += quoteSymbol;
    bool isInvalid = true;

    while (this->peek() != EOF && !this->isEndOfLine(this->peek()))
    {
      content += this->peek();

      if (this->peek() == quoteSymbol)
      {
        this->moveForward();
        isInvalid = false;
        break;
      }

      this->moveForward();
    }

    return Token(content, TokenType::STRING_LITERAL, isInvalid);
  }

  Token readNumberLiteral()
  {
    std::string content = "";
    bool isInvalid = false;

    content += this->peek();
    this->moveForward();

    if (content == "0" && this->peek() == 'x')
    {
      while (this->peek() != EOF && this->isWordSymbol(this->peek()) || this->peek() == '.')
      {
        if (!this->isHexadecimalDigit(this->peek()))
        {
          isInvalid = true;
        }

        content += this->peek();
        this->moveForward();
      }

      return Token(content, TokenType::NUMBER_LITERAL, isInvalid);
    }
    else
    {
      bool contentHasDot = false;

      while (this->peek() != EOF && this->isDigit(this->peek()) || this->peek() == '.')
      {
        if (this->peek() == '.')
        {
          if (contentHasDot)
          {
            isInvalid = true;
          }
          else
          {
            contentHasDot = true;
            isInvalid = true;
          }
        }
        else
        {
          isInvalid = false;
        }

        content += this->peek();
        this->moveForward();
      }

      return Token(content, TokenType::NUMBER_LITERAL, isInvalid);
    }
  }

  Token readWord()
  {
    std::string content = "";

    while (this->peek() != EOF && this->isWordSymbol(this->peek()))
    {
      content += this->peek();
      this->moveForward();
    }

    if (this->isKeyword(content))
    {
      return Token(content, TokenType::KEYWORD, false);
    }
    else
    {
      return Token(content, TokenType::IDENTIFIER, false);
    }
  }

  Token readOperator()
  {
    auto operators = getOperators();

    for (auto operator_ : operators)
    {
      if (this->readSpecificWord(operator_))
      {
        return Token(operator_, TokenType::OPERATOR, false);
      }
    }

    std::string content = "";

    while (this->peek() != EOF && !this->isWhitespace(this->peek()) && !this->isWordSymbol(this->peek()))
    {
      content += this->peek();
      this->moveForward();
    }

    return Token(content, TokenType::OPERATOR, false);
  }

  static bool isDigit(char symbol)
  {
    return symbol >= '0' && symbol <= '9';
  }

  static bool isHexadecimalDigit(char symbol)
  {
    return isDigit(symbol) || (symbol >= 'a' && symbol <= 'f') || (symbol >= 'A' && symbol <= 'F');
  }

  static bool isLetter(char symbol)
  {
    return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
  }

  static bool isWordSymbol(char symbol)
  {
    return isDigit(symbol) || isLetter(symbol) || symbol == '_' || symbol == '$';
  }

  static bool isEndOfLine(char symbol)
  {
    return symbol == '\n' || symbol == '\r';
  }

  static bool isWhitespace(char symbol)
  {
    return isEndOfLine(symbol) || symbol == '\t' || symbol == ' ';
  }

  static std::vector<std::string> getOperators()
  {
    std::vector<std::string> operators = {"="
                                          "+="
                                          "-="
                                          "*="
                                          "/="
                                          "%="
                                          "**="
                                          "<<="
                                          ">>="
                                          ">>>="
                                          "&="
                                          "^="
                                          "|="
                                          "&&="
                                          "||="
                                          "??="
                                          "["
                                          "]"
                                          "("
                                          ")"
                                          "{"
                                          "}"
                                          "."
                                          "..."
                                          ";"
                                          ","
                                          ":"
                                          "=="
                                          "!="
                                          "==="
                                          "!=="
                                          "<"
                                          ">"
                                          "<="
                                          ">="
                                          "&"
                                          "|"
                                          "^"
                                          "~"
                                          "<<"
                                          ">>"
                                          ">>>"
                                          "&&"
                                          "||"
                                          "!"
                                          "%"
                                          "++"
                                          "--"
                                          "+"
                                          "-"
                                          "*"
                                          "/"
                                          "**"
                                          "?"
                                          "??"};

    std::sort(operators.begin(), operators.end(), [](const std::string &a, const std::string &b)
              { return a.length() > b.length(); });

    return operators;
  };

  static bool isKeyword(const std::string &word)
  {
    return word == "break" ||
           word == "case" ||
           word == "catch" ||
           word == "class" ||
           word == "const" ||
           word == "continue" ||
           word == "debugger" ||
           word == "default" ||
           word == "delete" ||
           word == "do" ||
           word == "else" ||
           word == "export" ||
           word == "extends" ||
           word == "finally" ||
           word == "for" ||
           word == "function" ||
           word == "if" ||
           word == "import" ||
           word == "in" ||
           word == "instanceof" ||
           word == "new" ||
           word == "null" ||
           word == "return" ||
           word == "super" ||
           word == "switch" ||
           word == "this" ||
           word == "throw" ||
           word == "try" ||
           word == "typeof" ||
           word == "var" ||
           word == "void" ||
           word == "while" ||
           word == "with";
  }
};
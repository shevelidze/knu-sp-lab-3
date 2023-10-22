#include <string>

enum TokenType
{
  NUMBER_LITERAL,
  STRING_LITERAL,
  COMMENT,
  KEYWORD,
  OPERATOR,
  WHITESPACE,
  IDENTIFIER,
};

class Token
{
public:
  Token(std::string content, TokenType type, bool isInvalid) : content(content), type(type), isInvalid(isInvalid) {}

  std::string getContent() { return content; }
  std::string getANSIEscapeCode()
  {
    return this->isInvalid ? Token::getANSIEscapeCodeForInvalidToken() : Token::getANSIEscapeCodeByType(this->type);
  }

private:
  static std::string getANSIEscapeCodeByType(TokenType type)
  {
    switch (type)
    {
    case NUMBER_LITERAL:
      return "\033[0;33m";
    case STRING_LITERAL:
      return "\033[0;32m";
    case COMMENT:
      return "\033[0;90m";
    case KEYWORD:
      return "\033[0;34m";
    case OPERATOR:
      return "\033[0;35m";
    case WHITESPACE:
      return "\033[0m";
    case IDENTIFIER:
      return "\033[0m";
    }

    return "";
  }

  static std::string getANSIEscapeCodeForInvalidToken()
  {
    return "\033[0;31m";
  }

  std::string content;
  TokenType type;
  bool isInvalid;
};
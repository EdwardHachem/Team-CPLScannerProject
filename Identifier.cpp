#include "Identifier.h"

ostream& operator<<(ostream& os, const Identifier& identifier)
{
    if (identifier.identifierType == TokenType::NUMBERLITERAL)
        os << "Name: " << identifier.name << " Type: " << (int) identifier.identifierType << " Value: " << identifier.valueDouble << endl;
    else
        os << "Name: " << identifier.name << " Type: " << (int)identifier.identifierType << " Value: " << identifier.valueString << endl;

    return os;
}

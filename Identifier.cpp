#include "Identifier.h"

ostream& operator<<(ostream& os, const Identifier& identifier)
{
    if (identifier.identifierType == TokenType::NUMBERDBLLITERAL)
        os << "Name: " << identifier.name << " Type: " << (int)identifier.identifierType << " Value: " << identifier.valueDouble << endl;
    else if (identifier.identifierType == TokenType::NUMBERINTLITERAL)
        os << "Name: " << identifier.name << " Type: " << (int)identifier.identifierType << " Value: " << identifier.valueInt << endl;
    else
        os << "Name: " << identifier.name << " Type: " << (int)identifier.identifierType << " Value: " << identifier.valueString << endl;

    return os;
}

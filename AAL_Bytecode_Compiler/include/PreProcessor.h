#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <cstring>
#include <vector>
#include "TokenTypes.h"
#include "Token.h"
#include "commonFuncs.h"
#include <iostream>
#include <fstream>
#include <algorithm>

void insertIncludes(std::string& code, const std::string& includeDir);
void compileAsmToken(Token* currToken,int cCreatedFiles);
std::string preProcess(const std::string&);
void extractPreProcess(Token* currToken, std::vector<Token*>& extracted );
void processPreProcessTokens(std::vector<Token*>& extracted);

#endif // PREPROCESSOR_H

#include "PreProcessor.h"

/** \brief This functions executes some preprocessing tokens
 *
 * \param
 * \param
 *
 */
void processPreProcessTokens(std::vector<Token*>& extracted)
{
	Token* currToken;
	int cCreatedFiles=0;
	for(auto it=extracted.begin(); it!=extracted.end(); ++it)
	{
		currToken=*it;
		if(currToken->type==TOK_PREPROCESS)
		{
			if(iequals(currToken->tokenValue,"#CompileASM"))
			{
				if(currToken->childTokens.empty())
				{
					//throw error or nothing to do?
				}
				else
				{
                    compileAsmToken(currToken,cCreatedFiles);
					cCreatedFiles++;
				}
			}
		}
	}

}

/** \brief This function extracts all preprocessable tokens
 *
 * \param
 * \param
 *
 */
void extractPreProcess(Token* currToken, std::vector<Token*>& extracted )
{
	Token* currChildToken;
	for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
	{
		currChildToken=*it;
		if(currChildToken->type==TOK_PREPROCESS)
		{
			extracted.push_back(currChildToken);
		}
		extractPreProcess(currChildToken,extracted);
	}
}

/** \brief This function inserts recursive includes into given code (should be first operation after rading code)
 *
 * \param
 * \param
 *
 */
void insertIncludes(std::string& code)
{
    std::string preProcessedCode;
    bool preProcessPossible=true;
	char currChar;

	std::map<std::string,bool> alreadyIncluded;

	int lCode=code.length();
	int i;
	for(i=0;i<lCode;i++)
	{
		currChar=code[i];
		if(currChar=='\n')
		{
            preProcessPossible=true;
		}
		else if(currChar!=' ' && currChar!='\t' && currChar!='\n' && currChar!='\r' && currChar!='#')
		{
            preProcessPossible=false;
		}

		if(preProcessPossible==true && currChar=='#')
		{
			int foundInclude=i;
            std::string tmpValue="";
            int j;
			// extract string after #
			// so '#include("abc")' will result in 'include'
            for(j=i+1;j<lCode;j++)
            {
                currChar=code[j];
                if((currChar<'a' || currChar>'z') && (currChar<'A' || currChar>'Z') &&
                    (currChar<'0' || currChar>'9') && currChar != '_')
				{
                    break;
				}
                tmpValue+=currChar;
            }

            if(iequals(tmpValue,"include"))
            {
				// we found an include directive
				tmpValue="";
                int k;
				// get string after '#include' and remove all '(', ')', '<', '>' and '"'
                for(k=j+1;k < lCode;k++)
                {
					currChar=code[k];
					if(currChar!='(' && currChar!='"' && currChar!='\'' && currChar!='<')
					{
						if(currChar==')' || currChar=='>' ||  currChar=='\n')
						{
							break;
						}
						else
						{
							tmpValue+=currChar;
						}
					}
                }
				// remove #include("abc")
				code.erase(foundInclude,k-foundInclude+1);

				// check if the file should only be included one time
				std::transform(tmpValue.begin(), tmpValue.end(), tmpValue.begin(), ::tolower);
				if(alreadyIncluded.find(tmpValue)==alreadyIncluded.end())
				{
					std::string includeFileContent=readStringFile(tmpValue);

					std::string tmp="#include-once";
					int foundPos=0;
					while(foundPos>=0)
					{
						foundPos=stringFindIncase(includeFileContent,tmp);
						if(foundPos!=-1)
						{
							alreadyIncluded[tmpValue]=true;
							includeFileContent.erase(foundPos,tmp.length());
						}
					}

					// insert read file content into code
					code.insert(foundInclude,includeFileContent+"\n");
				}
				// restart parsing from beginning so we get recursive includes, too
				i=-1;
				lCode=code.length();
            }
        }
    }
}

/** \brief This function interprets/compiles a #CompileASM-Token
 *
 * \param
 * \param
 *
 */
void compileAsmToken(Token* currToken,int cCreatedFiles)
{
    std::string asmFile=std::string(".\\fasm\\codes\\")+toString(cCreatedFiles)+".asm";
    std::string binFile=std::string(".\\fasm\\codes\\")+toString(cCreatedFiles)+".bin";

    std::ofstream asmFileStream;
    asmFileStream.open(asmFile);
    for(auto childIt = currToken->childTokens.begin();childIt!=currToken->childTokens.end();++childIt)
    {
        asmFileStream << (*childIt)->tokenValue;
    }
    asmFileStream.close();

    char commandLine[512];
    strcpy(commandLine,("./fasm/FASM.exe "+asmFile+std::string(" ")+binFile).c_str());
    runWaitUntilFinished("./fasm/FASM.exe",commandLine);

    std::ifstream generatedBinFileStream(binFile, std::ios::in|std::ios::binary|std::ios::ate );
    if (generatedBinFileStream.is_open())
    {
        char * readBin;
        std::streampos lBinFile = generatedBinFileStream.tellg();
        readBin = new char [lBinFile];
        generatedBinFileStream.seekg (0, std::ios::beg);
        generatedBinFileStream.read (readBin, lBinFile);
        generatedBinFileStream.close();

        currToken->setToken(TOK_LITERAL,convertCharArrayToStringHex(readBin,lBinFile));
        currToken->childTokens.clear();

        delete[] readBin;
    }
    else
    {
        std::cout<<"Error opening compiled ASM-File #"<<cCreatedFiles<<std::endl;
    }
}

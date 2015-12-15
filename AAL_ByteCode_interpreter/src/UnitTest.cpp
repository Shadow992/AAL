#include "UnitTest.h"

UnitTest::UnitTest()
{
    //ctor
}

UnitTest::~UnitTest()
{
    //dtor
}

void UnitTest::doAllTests()
{
    testHashTable();
    testAALVariable();
    testAALArray();
    testVariableManagement();
    testBytecodeParser();
}

void UnitTest::testBytecodeParser()
{
    std::string code="$10=@param\nep\n$20=69 $10 0\njf $20 11\n";
    BytecodeParser parser;
    parser.parseBytecode(code,true);
    assert(parser.parsedLinesVector.size()>3);

    assert(parser.parsedLinesVector[0]->type==LINE_ASSIGN);
    assert(parser.parsedLinesVector[1]->type==LINE_ENDPARAM);
    assert(parser.parsedLinesVector[2]->type==LINE_COMMAND);
    assert(parser.parsedLinesVector[3]->type==LINE_FALSE_JMP);

    std::cout << "BytecodeParser-Test succeeded" << std::endl;
}

void UnitTest::testVariableManagement()
{
    VariableManagement manager;
    AalVariable* var=manager.getVar(0,TYPE_VARIABLE);
    AalVariable* var2=manager.getVar(1,TYPE_VARIABLE);

    var->setValue(111LL,TYPE_LONG);
    var2->setValue(222LL,TYPE_LONG);

    assert(manager.getVar(0,TYPE_VARIABLE)==var);
    assert(manager.getVar(1,TYPE_VARIABLE)==var2);

    manager.enterNextHierarchyLevel();
    AalVariable* var3=manager.getVar(2,TYPE_VARIABLE);
    var3->setValue(3333LL,TYPE_LONG);
    assert(manager.getVar(0,TYPE_VARIABLE)==var);
    assert(manager.getVar(2,TYPE_VARIABLE)->getLongValue()==3333);
    manager.leaveCurrentHierarchyLevel();

    manager.enterFunction(123);
    var3=manager.getVar(2,TYPE_VARIABLE);
    var3->setValue(3333LL,TYPE_LONG);
    assert(manager.getVar(2,TYPE_VARIABLE)->getLongValue()==3333);
    int line=0;
    manager.leaveFunction(line);
    assert(line==123);

    std::cout << "VariableManagement-Test succeeded" << std::endl;
}

void UnitTest::testAALArray()
{
    AalVariable var1;
    AalVariable var2;

    var1.appendDimension(10);
    var1.appendDimension(5);

    assert(var1.arrayInfo!=nullptr);
    assert(var1.arrayInfo->overallArraySize==50);

    AalVariable* varIdx2 = var1.getVariablePointer(2);
    AalVariable* varIdx21 = varIdx2->getVariablePointer(1);
    varIdx21->setValue(12345LL,TYPE_LONG);

    AalVariable* varIdx24 = varIdx2->getVariablePointer(4);
    varIdx24->setValue(123456LL,TYPE_LONG);

    var2.setValue(&var1,-1,-1);

    assert(var2.arrayInfo!=nullptr);
    assert(var2.arrayInfo->overallArraySize==50);

    AalVariable* var2Idx2 = var2.getVariablePointer(2);
    AalVariable* var2Idx21 = var2Idx2->getVariablePointer(1);
    AalVariable* var2Idx24 = var2Idx2->getVariablePointer(4);

    assert(var2Idx21->getLongValue()==12345LL);
    assert(var2Idx24->getLongValue()==123456LL);

    var1.clear();
    var2.clear();
    std::cout << "AALArray-Test succeeded" << std::endl;
}

void UnitTest::testAALVariable()
{
    AalVariable var1;
    AalVariable var2;
    AalVariable var3;

    var1.setValue(10LL,TYPE_LONG);
    assert(var1.getLongValue()==10LL);

    var2.setValue(10.4,TYPE_DOUBLE);
    assert(var2.getLongValue()==10LL);
    assert(var2.getDoubleValue()==10.4);

    var3.setValue(106LL,TYPE_STRING);
    assert(var3.getLongValue()==106);
    assert(var3.getStringValue()=="106");

    var1.add(var1.getLongValue(),var2.getLongValue());
    assert(var1.getLongValue()==20);
    assert(var1.getStringValue()=="20");

    var1.sub(var3.getLongValue(),var2.getLongValue());
    assert(var1.getLongValue()==96);
    assert(var1.getStringValue()=="96");

    var1.append("xx");
    assert(var1.getStringValue()=="96xx");

    assert(var1.isDouble()==false);
    assert(var1.isLong()==false);
    assert(var1.isString()==true);

    var1.setTrue();
    assert(var1.isTrue()==true);

    var1.setFalse();
    assert(var1.isTrue()==false);

    var1.join("ab","cd");
    assert(var1.getStringValue()=="abcd");

    var1.setValue(2LL,TYPE_LONG);
    var1.mul(10LL,var1.getLongValue());
    assert(var1.getStringValue()=="20");

    std::cout << "AALVariable-Test succeeded" << std::endl;
}

void UnitTest::testHashTable()
{
	Hashtable<int> t;
	for(unsigned int i=0;i<200*3;i+=3)
	{
		int* tmp=new int;
		*tmp=128*2+i;
		t.set(i,tmp);
	}

	for(unsigned int i=0;i<200*3;i+=3)
	{
		unsigned long long tmp=*(t.get(i));
		if(tmp!=128*2+i)
		{
			std::cout<<"Error expected "<<128*2+i<<" but found "<<tmp<<std::endl;
			return;
		}
	}

	if(t.get(12356)!=nullptr)
	{
		std::cout<<"Error expected nullptr but found "<<t.get(12356)<<std::endl;
		return;
	}

    std::cout << "Hashtable-Test succeeded" << std::endl;
}

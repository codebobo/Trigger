#include <fstream>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using   namespace   std;



void genHttpViewHeaderFile(std::ofstream &oHeadFile,const std::string className)
{
	std::string upName=className;
	transform(upName.begin(),upName.end(),upName.begin(),::toupper);
	oHeadFile<<"#ifndef __"<<upName<<"_H__\n";
	oHeadFile<<"#define __"<<upName<<"_H__\n";
	oHeadFile<<"#include \"HttpController.h\"\n";
	oHeadFile<<"class "<<className<<":public HttpController\n";
	oHeadFile<<"{\npublic:\n\t"<<className<<"(){};\n\tvirtual ~"<<className<<"(){};\n\tDECLARE_TROBJECT_CLASS("<<className<<");\n\tvirtual void handleHttpAsyncRequest(const HttpRequest & req, std::function < void(HttpResponse &)> callback);\n};\n";
	oHeadFile<<"#endif\n";
}



void genHttpViewSourceFile(std::ofstream &oSrcFile,const std::string className)
{
	oSrcFile << "#include \"" << className << ".h\"\n";
	oSrcFile <<"#include \"HttpView.h\"\n";
	oSrcFile <<"#include <string>\n";
	oSrcFile <<"#include <sstream>\n";
	oSrcFile <<"#include <map>\n";
	oSrcFile <<"#include <vector>\n";
	oSrcFile <<"#include <set>\n";
	oSrcFile <<"IMPL_TROBJECT_CLASS("<<className<<");\n";
	oSrcFile <<"void "<<className<<"::handleHttpAsyncRequest(const HttpRequest & req, std::function < void(HttpResponse &)> callback)\n{\n}\n";
};
int getHttpViewSrc(std::string className)
{
	std::cout<<"create HttpController Class "<<className<<std::endl;


	std::string headFileName=className+".h";
	std::string sourceFilename=className+".cc";
	std::ofstream oHeadFile(headFileName.c_str(),std::ofstream::out);
	std::ofstream oSourceFile(sourceFilename.c_str(),std::ofstream::out);
	if(!oHeadFile||!oSourceFile)
		return -1;

	genHttpViewHeaderFile(oHeadFile,className);
	genHttpViewSourceFile(oSourceFile,className);


	return 0;
}
int main(int argc,char *argv[])
{
	if(argc<=1)
	{
		std::cout<<"usage:"<<argv[0]<<" classname"<<std::endl;
		return -1;
	}
	int i=0;
	for(i=1;i<argc;i++)
	{
		if(getHttpViewSrc(argv[i]))
			return -1;
	}
	return 0;
}

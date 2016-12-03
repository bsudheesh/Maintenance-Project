#include<iostream>
#include<unordered_map>
#include<vector>
#include<fstream>
using namespace std;
int main(){
	string line;

	ifstream myFile ("imp.txt");
	if(!myFile){
		cout<<"\nThe file is not present";
	}
	else{
		while(getline(myFile,line)){
		}
	}
	
	/* Adding element to the vector*/
	vector<string>myString;
	string temp="";
	

	/* This makes sure that the input from the file is in proper format. Checking additional spaces after ; */
	string tempLine;
	bool semicolon=false;
	for(int i=0;i<line.length();i++){
		if(line[i]==';'){
			semicolon=true;
			tempLine+=line[i];

		}
			
		else if(line[i]==' '){
			if(semicolon){
				//do nothing
				semicolon=false;
			}
			else{
				tempLine+=line[i];
			
			}
		}
		else{
			tempLine+=line[i];
			semicolon=false;
		}


	}

	line="";
	line=tempLine;
	/* storing the element in hashmao for the modules*/

	for(int i=0;i<line.length();i++){
		if(line[i]==';'){
			myString.push_back(temp);
			temp="";
		}
		else
			temp+=line[i];
	}
	myString.push_back(temp);



	for(int i=0;i<myString.size();i++)
		cout<<myString.at(i)<<endl;

	unordered_map<string,int>myMap;
	bool answer=true;
    int index=0;
    vector<string> unique;
    vector<string> first;
    while(answer){
        string str=myString.at(index);
        bool space=false;
        string firstLetter,secondLetter;
        if(str=="**")
            break;
        for(int i=0;i<str.length();i++){
            if(str[i]==' ' || !space){
                if(str[i]==' '){
                    //do nothing
                }
                else
                    firstLetter+=str[i];
                space=true;
            }
            else
                secondLetter+=str[i];
            
            
        }
        first.push_back(firstLetter);
        myMap[secondLetter]++;
        index++;
    }
    unordered_map<string,int>uniqueMap;
    cout<<"\nThe unique elements are : ";
    for(int i=0;i<first.size();i++){
        unordered_map<string,int>::iterator finding;
        finding=myMap.find(first.at(i));
        if(finding==myMap.end()){
            uniqueMap[first.at(i)]++;
        }
    }
    unordered_map<string,int>::iterator iter;
    for(iter=uniqueMap.begin();iter!=uniqueMap.end();++iter)
        unique.push_back(iter->first);
    for(int i=0;i<unique.size();i++)
        cout<<unique.at(i)<<" ";
    cout<<endl;
    return 0;
	
}
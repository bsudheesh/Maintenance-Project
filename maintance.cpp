#include<iostream>
#include<unordered_map>
#include<vector>
#include<queue>
#include<fstream>
using namespace std;
unordered_map<string,vector<string> > storeValues;
vector<string> allNodes;
queue<string>myQueue;

int getElementIndex(string transaction){
    int count=0;
    for(int i=0;i<allNodes.size();i++){
        if(allNodes.at(i)==transaction)
            count=i;
    }
    return count;

}

void printAllPathsUtil(string transaction,string defective,bool visited[],vector<string> path,int pathIndex){
    int count = getElementIndex(transaction);
    
    visited[count]=true;
    
    path.insert(path.begin()+pathIndex,transaction);
    //path[pathIndex]=transaction;
    pathIndex++;
    if(transaction==defective){
        for(int i=0;i<pathIndex;i++){
            cout<<path.at(i)<<" ";
        }
        cout<<endl;
    }
    else{
        vector<string> first;
        unordered_map<string,vector<string> >::iterator iter;
        iter=storeValues.find(transaction);
        if(iter!=storeValues.end()){
            first=iter->second;
            for(int i=0;i<first.size();i++){
                int getCount=getElementIndex( first.at(i));
                if(!visited[getCount]){
                    printAllPathsUtil(first.at(i),defective,visited,path,pathIndex);
                }
            }
        }
    }
    pathIndex--;
    visited[count]=false;
}

void printAllPaths(string transaction,string defective, int size){
    //cout<<"\nFunction called with transaction : "<<transaction<<" defective node : "<<defective<<" and size : "<<size<<endl;
    bool visited[size];
    for(int i=0;i<size;i++)
        visited[i]=false; 
    //string path[size];
    vector<string> path;
    int pathIndex=0;
    printAllPathsUtil(transaction,defective,visited,path,pathIndex);
    



}



int main(){
	string line;
    int uniqueNodes=0;

	ifstream myFile ("imp.txt");
	if(!myFile){
		cout<<"\nThe file is not present";
        return 0;
	}
	else{
		while(getline(myFile,line)){
		}
	}
	
	/* Adding element to the vector*/
	vector<string>myString;
	string temp="";
	

	/* This makes sure that the input from the file is in proper format. Checking additional spaces after ; */
	string tempLine,defective="",transaction="";
	bool semicolon=false,space=false;
	for(int i=0;i<line.length();i++){
		if(line[i]==';'){
			semicolon=true;
			tempLine+=line[i];

		}
			

		else if(line[i]==' '){
            if(space){
                //do nothing
                space=false;
            }

			else if(semicolon){
				//do nothing
				semicolon=false;
			}
			else{
				tempLine+=line[i];
			
			}
            space=true;
		}
		else{
			tempLine+=line[i];
			semicolon=false;
            space=false;
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
                    space=true;
                    //do nothing
                }
                else
                    firstLetter+=str[i];
                
            }
            else
                secondLetter+=str[i];
            
            
        }
        unordered_map<string,vector<string> >::iterator findIter;
        findIter=storeValues.find(firstLetter);
        vector<string> value;
        if(findIter==storeValues.end()){
        	value.push_back(secondLetter);
        }
        else{
        	value=findIter->second;
        	value.push_back(secondLetter);
        }
        bool firstWord=false,secondWord=false;
        for(int i=0;i<allNodes.size();i++){
            if(allNodes.at(i) == firstLetter)
                firstWord=true;
            if(allNodes.at(i) == secondLetter)
                secondWord=true;
        }
        if(!firstWord)
            allNodes.push_back(firstLetter);
        if(!secondWord)
            allNodes.push_back(secondLetter);
        storeValues[firstLetter]=value;
        first.push_back(firstLetter);
        myMap[secondLetter]=1;
        index++;
    }
    index++;
    defective=myString.at(index++);
    transaction=myString.at(index);
    unordered_map<string,int>uniqueMap;
    for(int i=0;i<first.size();i++){
        unordered_map<string,int>::iterator finding;
        finding=myMap.find(first.at(i));
        if(finding==myMap.end()){
            uniqueMap[first.at(i)]++;
        }
    }
    uniqueNodes=myMap.size()+first.size();
    unordered_map<string,int>::iterator iter;
    cout<<"\nThe transaction are : ";

    

    for(iter=uniqueMap.begin();iter!=uniqueMap.end();++iter){
        unique.push_back(iter->first);
        cout<<iter->first<<" ";
    }

    cout<<endl;
    cout<<"\nThe defective node is : "<<defective<<endl;
        
    /* Unique has the unique transaction */


    /* Now finding the path to the defective node and for the explosion*/
    vector<string>secondNodes;
    cout<<"\nThe User Interface is"<<endl;
    unordered_map<string,vector<string> >:: iterator i;
    for(i=storeValues.begin();i!=storeValues.end();i++){

    	cout<<i->first<<" : ";
    	vector<string> val=i->second;
    	for(int ii=0;ii<val.size();ii++)
    		cout<<val.at(ii)<<" ";
    	cout<<endl;
    }
    cout<<endl; 
    //myQueue.push(transaction);
    vector<string>firstVector;
    unordered_map<string,vector<string> >::iterator i1;
    i1=storeValues.find(transaction);
    firstVector=i1->second;
    for(int i=0;i<firstVector.size();i++)
        myQueue.push(firstVector.at(i));
    string parent=transaction;

    cout<<"\nThe paths from transaction to the defective node is \n";
    printAllPaths(transaction,defective,allNodes.size());
    //printAllPaths(defective,transaction,parent);
    /* Finding parth from the transition node to the defective node */

    
    cout<<endl;
    return 0;
	
}
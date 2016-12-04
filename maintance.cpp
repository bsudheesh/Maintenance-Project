#include<iostream>
#include<unordered_map>
#include<vector>
#include<queue>
#include<fstream>
using namespace std;
unordered_map<string,vector<string> > storeValues; //stores all the vertices going from a particular node 
vector<string> allNodes; //stores all the nodes 
unordered_map<string,int> countNodesForBFS; //checks to see if a node is present for putting *


int getElementIndex(vector<string> myVector,string transaction){
    int count=0;
    for(int i=0;i<myVector.size();i++){
        if(myVector.at(i)==transaction){
            count=i;
            break;
        }
    }
    return count;

}


int printingIndex(vector<string> myVector,string transaction){
    int count=-1;
    for(int i=0;i<myVector.size();i++){
        if(myVector.at(i)==transaction){
            count=i;
            break;
        }
    }
    return count;

}



void printAllPathsUtil(string transaction,string defective,bool visited[],vector<string> path,int pathIndex){
    int count = getElementIndex(allNodes,transaction);
    
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
                int getCount=getElementIndex(allNodes, first.at(i));
                if(!visited[getCount]){
                    printAllPathsUtil(first.at(i),defective,visited,path,pathIndex);
                }
            }
        }
    }
    pathIndex--;
    visited[count]=false;
}

void printAllPathsToExplosion(string transaction,string defective,bool visited[],vector<string> &path,vector<string> myVector){
    if(myVector.size()!=0){
        int count = getElementIndex(allNodes,transaction);
        visited[count]=true;
        string tem=transaction;
        path.push_back(tem);
        vector<string> first;
        unordered_map<string,vector<string> >::iterator iter;
        int index=getElementIndex(myVector,transaction);
        iter=storeValues.find(transaction);
        if(iter!=storeValues.end()){
            first=iter->second;
            myVector.erase(myVector.begin()+index);
            for(int i=0;i<first.size();i++){
                int getCount=getElementIndex(allNodes, first.at(i));
                myVector.push_back(first.at(i));
                if(!visited[getCount]){
                    printAllPathsToExplosion(first.at(i),defective,visited,path,myVector);
                }
            }
        }
        else{
            
            myVector.erase(myVector.begin()+index);
        }
        visited[count]=false;
    }

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
    cout<<"\nThe path from transaction to explosion is "<<endl;
    unordered_map<string,vector<string> >::iterator iter;
    iter=storeValues.find(transaction);
    vector<string> value = iter->second;
    printAllPathsToExplosion(transaction,defective,visited,path,value);

    //printing in proper order
    vector<string> seenElement;
    seenElement.push_back(transaction);
    string parent=transaction;
    cout<<transaction<<endl;


    int childernSize;
    int i=1;
    while(i<path.size()){
        string temp = path.at(i);
        vector<string> value;
        unordered_map<string,vector<string> >::iterator findIter;
        findIter=storeValues.find(parent);
        if(findIter==storeValues.end()){
            int i = seenElement.size()-1;
            seenElement.erase(seenElement.begin()+i);
        }
        else{
            value=findIter->second;
            childernSize=value.size();
            int count = printingIndex(value,temp);
            if(count==-1){
                int i = seenElement.size()-1;
                seenElement.erase(seenElement.begin()+i);
            }
            else{
                seenElement.push_back(temp);
                int size=seenElement.size();
                unordered_map<string,int>::iterator it;
                it=countNodesForBFS.find(temp);
                if(it==countNodesForBFS.end()){
                    countNodesForBFS[temp]++;
                }
                else
                    temp+="*";

                for(int index=1;index<=size-1;index++)
                    cout<<" ";
                cout<<temp;
                i++;
            }
            cout<<endl;
        }
        parent=seenElement.at(seenElement.size()-1);
    }
    cout<<endl;
}

int main(){
	string line;
    int uniqueNodes=0;

	ifstream myFile ("imp.txt");
	if(!myFile){
		cout<<"\nThe file is not present";
        return 0;
	}
	else
		(getline(myFile,line));
	
	
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
    cout<<"\nThe paths from transaction to the defective node is \n";
    printAllPaths(transaction,defective,allNodes.size());
    /* Printing the unique modules */
    cout<<"\nThe unique modules are : ";
    for(int i=0;i<allNodes.size();i++){
        if(allNodes.at(i) == defective || allNodes.at(i) == transaction){
            //do nothing
        }
        else
            cout<<allNodes.at(i)<<" ";
    }
    cout<<endl;
    return 0;
	
}
#include<iostream>
#include<unordered_map>
#include<vector>
#include<queue>
#include<fstream>
using namespace std;
unordered_map<string,vector<string> > storeValues; //stores all the vertices going from a particular node 
vector<string> allNodes; //stores all the nodes 
unordered_map<string,int> countNodesForBFS; //checks to see if a node is present for putting *
vector<string>myString; //stores the input from the file


/**
    This function is used to see if the element is present in the vector or not.
    Pre Condition: myVector and transaction are valid.
    Post Condition: If element found return the index, else return 0.
*/
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

/**
    Similar to getElementIndex, but returns -1 for Element not found. 
    Deleting from index 0 may result in unwanted actions.
    Pre Condition: myVector and transaction are valid.
    Post Condition: If element found return the index, else return -1.

*/
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

/**
    Prints all the path to defective node. It takes the node at current, defective node, and a boolean
    values. Boolean values is used to check if the node has been visited before. This checks if a cycle is present in graph.
    path is the path from the transaction to defective node. pathIndex is used to add the node at proper Index.
    The main idea here is to do a Depth First Traversal. We keep storing all the visited nodes. 
    If the transaction is the destination node, we print the value are vector path,

    Precondition: The arguments are valid.
    Postcondition: The path is printed from the source to the defective node.
*/

void printAllPathsUtil(string transaction,string defective,bool visited[],vector<string> path,int pathIndex){
    /*
    A boolean array is created to the size of all unique nodes from the file.
    Each index is marked with a unique node. Count returns the position of the transaction node in the vector.
    The element is marked visited by marking the boolean value of that index as true
    */
    int count = getElementIndex(allNodes,transaction); //returns the index
    visited[count]=true; //mark node visited
    path.insert(path.begin()+pathIndex,transaction); //store the path at pathIndex
    pathIndex++;
    if(transaction==defective){ //when we have reached the defective node
        //Pring the path
        for(int i=0;i<pathIndex;i++){
            cout<<path.at(i)<<" ";
        }
        cout<<endl;
    }
    else{
        //if the destination is not reached, get all the children of the transaction node 
        vector<string> children; //stores the childredn
        unordered_map<string,vector<string> >::iterator iter;
        iter=storeValues.find(transaction);
        //If the element has no children, this will not be executed.
        if(iter!=storeValues.end()){
            children=iter->second; //store the children of the transaction node
            for(int i=0;i<children.size();i++){
                int getCount=getElementIndex(allNodes, children.at(i)); //gets the index of the children
                //if the children is not visited, call the function again with transaction node being the children
                if(!visited[getCount]){
                    printAllPathsUtil(children.at(i),defective,visited,path,pathIndex);
                }
            }
        }
    }
    pathIndex--; //decrease the path Index
    visited[count]=false; //mark the node unVisited
}

/**
    Prints all the path to defective node. It takes the node at current, defective node, and a boolean
    values. Boolean values is used to check if the node has been visited before. This checks if a cycle is present in graph.
    path is the path from the transaction to defective node. myVector is used to store nodes.
    The main idea here is to do a Depth First Traversal. We keep storing all the visited nodes. 
    The path is changed at the end of the function call.

    Precondition: The arguments are valid.
    Postcondition: The path is changed with all the nodes from the transaction to defective node.

*/

void printAllPathsToExplosion(string transaction,string defective,bool visited[],vector<string> &path,vector<string> myVector){
    /*
        If the size of myVector is 0, we know that we have all paths possible from transaction to the defective node.
    */
    if(myVector.size()!=0){
        /*
            The main idea here is to store elements in myVector.
            First get the index of the transaction node.
            Find to see if the transaction node has children. If it doesn't, delete it
            If it has children, delete the transaction node and add it's childrent to myVector.

        */

        int count = getElementIndex(allNodes,transaction); //get the element index
        visited[count]=true; //mark that as visited
        string tem=transaction;
        path.push_back(tem); //store the current element in path
        vector<string> children; //stores children of the current transaction node
        unordered_map<string,vector<string> >::iterator iter;
        int index=getElementIndex(myVector,transaction);
        iter=storeValues.find(transaction);
        if(iter!=storeValues.end()){
            children=iter->second;
            //delete the node currently begin under consideration, aka, the transaction node
            myVector.erase(myVector.begin()+index);
            for(int i=0;i<children.size();i++){
                int getCount=getElementIndex(allNodes, children.at(i)); //add the children in the transaction node
                myVector.push_back(children.at(i));
                if(!visited[getCount]){
                    printAllPathsToExplosion(children.at(i),defective,visited,path,myVector);
                }
            }
        }
        else{
            myVector.erase(myVector.begin()+index);
        }
        visited[count]=false;
    }

}

/**
    This prints the explosion path. It takes the path and the current node as argument.

    Pre Condition: The input is valid.
    Post Condition: The path is printed.
*/
void printExplosion(vector<string> path,string transaction){
    /*
        The idea here is to iterate all the element in the vector path.
        The idea is to have a parent variable. It is used to check if the current transaction node
        is a children of it. If if a childern, we provide the proper tabbing and add the children to vector.
        If it's not a childern, we delete the parent from the vector and change the parent variable.
        We do this until we have found it's parent.
        If there is no children of the transaction node, delete the last element from vector.
        The parent is changed at every iteration.
        The repeated elements are tracked by using countNodesForBST hashmap. If element is already seen, we add *
    */
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
        findIter=storeValues.find(parent); //finding the children.
        if(findIter==storeValues.end()){
            //if it has no childern, delete the last element.
            int i = seenElement.size()-1;
            seenElement.erase(seenElement.begin()+i);
        }
        else{
            value=findIter->second;
            childernSize=value.size();
            int count = printingIndex(value,temp);
            //check if the current node is a children of it's parent. If not, delete the last element
            if(count==-1){
                int i = seenElement.size()-1;
                seenElement.erase(seenElement.begin()+i);
            }
            else{
                //if child, add the children to hashmap and provide proper tabbing
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
        parent=seenElement.at(seenElement.size()-1); //changing parent
    }
    cout<<endl;
}

/**
    This is a utility function that is used to print the all possible path from the origin to source
    and the explosion path. size is the count of unique nodes. 

    Precondition: The inputs are valid.
    Postcondition: All possible and the explosion paths are printed.
*/
void printAllPaths(string transaction,string defective, int size){
    bool visited[size]; //used to track the visited node. All are false at first,
    for(int i=0;i<size;i++)
        visited[i]=false; 
    vector<string> path;
    int pathIndex=0;
    printAllPathsUtil(transaction,defective,visited,path,pathIndex); //prints all possible path
    cout<<"\nThe path from transaction to explosion is "<<endl;
    /*
        This code below is used to print the explosion path. We first store the children of the transaction node
        and add it to the vector.
    */
    unordered_map<string,vector<string> >::iterator iter;
    iter=storeValues.find(transaction);
    vector<string> value = iter->second;
    printAllPathsToExplosion(transaction,defective,visited,path,value); //path has all the way of path
    printExplosion(path,transaction); //prints the path
}

int main(){
	string line; //stores the input from the file
    vector<string> unique; //stores all the transactions
    vector<string> first; //stores all the first nodes
    //mymap and first are used to find the unique modules.
    string temp="";
    string tempLine,defective="",transaction=""; //tempLine stores the line without 
    unordered_map<string,int>myMap;//myMap stores all the nodes that come in the second Place
    bool answer=true;
    int index=0;
    unordered_map<string,int>uniqueMap; //this hashmap stores the unique modules. It is used to avoid duplicate nodes


    /*
        Opening file. Return if no file is found.
    */

	ifstream myFile ("imp.txt");
	if(!myFile){
		cout<<"\nThe file is not present";
        return 0;
	}
	else
		(getline(myFile,line));

    /* Testing the following conditions
    1) If the file is empty.
    2) If the file doesn't have proper amount of information to make the user interface
    3) File doesn't have two nodes in any transaction.
    4) The file has no **
    5) No defective or transaction node is provided.
    6) If the transaction or the defective not is not present in the graph.
    */
    if(line.length()==0){
        cout<<"\nThe file is empty!! Can't get data\n";
        return 0;
    }
    else if(line.length()<3){
        cout<<"\nInvalid data! Problem with number of nodes\n";
        return 0;
    }
    else if(line.length()<6){
        cout<<"\nDefective and transaction node not definded\n";
        return 0;
    }
    else if(line.length()<8){
        cout<<"\nRequired information not provided\n";
    }
	
    /* This makes sure that the input from the file is in proper format. Checking additional spaces after ; */
	
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
	line=tempLine; //line has proper orientation by removing extra spaces between two nodes


	/* stores all the nodes in vector myString*/
	for(int i=0;i<line.length();i++){
		if(line[i]==';'){
			myString.push_back(temp);
			temp="";
		}
		else
			temp+=line[i];
	}
	myString.push_back(temp);
    while(answer){
        if(index>=myString.size()){
            cout<<"\nError! No end of file specified. No defective and transaction node found.";
            return 0;
        }
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
        if(firstLetter=="" || secondLetter==""){
            cout<<"\nEither the first node, the second node value is empty or the end of file is not provided";
            return 0;
        }
        //firstLetter stores the first node and the secondLetter stores the second node
        unordered_map<string,vector<string> >::iterator findIter;
        findIter=storeValues.find(firstLetter);
        vector<string> value;
        /*
            storeValues is a hashmap used to show the user interface. It would look like
            Z : B1 F 
            Y : X 
            X : P 
            C : F 
            Q : C D 
            A : B C D 
            D : X Y Z 
            B : X Y Z 

            the key is a node and it's value are all the nodes possible to be visited from the key node

        */
        if(findIter==storeValues.end()){
        	value.push_back(secondLetter);
        }
        else{
        	value=findIter->second;
        	value.push_back(secondLetter);
        }
        /*
            all Nodes stores all the unique modules.
            firstWord and secondWord are used to see that the element doesn't gets added again.
            They are added to allNodes if firstLetter and SecondLetter is/are not found.
        */
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
        storeValues[firstLetter]=value; //storing the value in the hashmap
        first.push_back(firstLetter); //storing the first letter in the vector
        myMap[secondLetter]=1; //storing the second letter in the hashmap
        index++;
    }
    index++;
    //After we encounter **, the first node is the defective node and the second is transacation node
    defective=myString.at(index++);
    transaction=myString.at(index);
    if(defective=="" || transaction==""){
        cout<<"\nThe defective or transaction node is not provided\n";
        return 0;
    }
    for(int i=0;i<first.size();i++){
        unordered_map<string,int>::iterator finding;
        finding=myMap.find(first.at(i));
        if(finding==myMap.end()){
            uniqueMap[first.at(i)]++;
        }
    }
    unordered_map<string,int>::iterator iter;
    cout<<"\nThe transaction are : ";
    for(iter=uniqueMap.begin();iter!=uniqueMap.end();++iter){
        unique.push_back(iter->first); 
        cout<<iter->first<<" ";
    }

    cout<<endl;
    cout<<"\nThe defective node is : "<<defective<<endl;
        
    /* Printing the unique modules */
     cout<<"\nThe unique modules are : ";
    for(int i=0;i<allNodes.size();i++){
        bool tempBool = false;
        for(int j=0;j<unique.size();j++){
            if(allNodes.at(i) == unique.at(j)){
                tempBool=true;
                break;
            }
        }
        if(tempBool){
            //do nothing
        }
        else
            cout<<allNodes.at(i)<<" ";
    }
    cout<<endl;

    /* Showing the user Interface*/
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

    /*
        Checking to see if the defective or transaction is present in the graph.
        If not abort.
    */

    int checkIndex=printingIndex(allNodes,defective);
    if(checkIndex==-1){
        cout<<"\nError!! The defective node is not present in the graph";
        return 0;
    }
    checkIndex=printingIndex(allNodes,transaction);
    if(checkIndex==-1){
        cout<<"\nError!! The transaction node is not present in the graph";
        return 0;
    }



    /* Printing all posible path from explosion to transaction and the explosion path*/
    cout<<"\nThe paths from transaction to the defective node is \n";
    printAllPaths(transaction,defective,allNodes.size());//this function will also show the explosion path
    return 0;
	
}
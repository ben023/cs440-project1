#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <cstring>

using namespace std;

int h_lvl = 2;
int h1_lvl = h_lvl + 1;
class Record{
	public:
		float eid;
		string name;
		string bio;
		float mid;
		int hash_value;
};






string getHash(int id){
	string key = bitset<32>(id).to_string();
	return key;
}
		//give a full id and how many end bits you want 
//110000  000
string levelKey(string fullkey, int bit_ct){
	int length = fullkey.length();
	return fullkey.substr(length-bit_ct);
}
		

bool CapOk(int recordsCt, int blockCt, int emptyOverflowCt){
	if( (float) recordsCt / (float) (blockCt - emptyOverflowCt) * 5.0 > 0.8){
		return false;
	}
	else{
		return true;
	}
}


void addBlock(){
	ofstream fp;
	fstream file("test.txt");
	fp.open("test.dat", ios::binary);
	string temp("11432121,Michell Haney,Lorem ipsum dolor sit amet consectetur adipiscing elit sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident sunt in culpa qui officia deserunt mollit anim id est laborum.,11432122");
    string temp2("11432113,Kory Born,Quis commodo odio aenean sed adipiscing diam donec adipiscing. Scelerisque purus semper eget duis at tellus at. Cras pulvinar mattis nunc sed blandit libero volutpat sed. Dictumst vestibulum rhoncus est pellentesque elit ullamcorper dignissim cras tincidunt. Sapien pellentesque habitant morbi tristique senectus et. Mauris in aliquam sem fringilla. Amet mauris commodo quis imperdiet massa tincidunt. Accumsan sit amet nulla facilisi morbi tempus. Pellentesque habitant morbi tristique senectus et.,11432114");
    temp.resize(4096, ' ');
    temp2.resize(4096, ' ');
    char char_array[4097];
    char char_array2[4097];
    strcpy(char_array2, temp2.c_str());
    strcpy(char_array, temp.c_str());
	file.seekp(4096*3);
	file.write(char_array, 4096);
	file.write(char_array2, 4096);
	// fp << temp;
	// fp << temp2;
	fp.close();
}
int readNumber(ifstream &fp){
	string str;
	std::getline(fp, str);
	return stoi(str);
}
string readBlock(int offset){
	ifstream fp;
	fp.open("test.txt");
	char block[4097];
    block[4096] = 0;
    fp.seekg(4096 * offset, ios::beg);
    fp.read((char *)&block, 4096);
    fp.close();
    string str(block);
    return str;
}
// void readBucket(){
// 	ifstream fp;
// 	fp.open("bucket.dat", ios::binary);
// 	// string num_record_str;
// 	// std::getline(fp, num_record_str);
// 	// int num_record = stoi(num_record_str);
// 	int num_record = readNumber(fp);
// 	// string N_str;
// 	// std::getline(fp, N_str);
// 	// int N = stoi(N_str);
// 	int N = readNumber(fp);
// 	cout << "num records " << num_record << endl;
// 	for(int i = 0;i < N; i++){
// 		cout << i<<endl;
// 	}
// 	if(!CapOk){
// 		cout << "need a split" << endl;
// 	}
// 	else{
// 		cout << "no split needed" << endl;
// 	}
// 	fp.close();
// }
// [ [1] [2,3]   ]
void createBucketArray(vector<vector<int>> &vect){
	ifstream fp;
	fp.open("bucket.txt");
	int emptyOverflowCt = readNumber(fp);
	int recordCt = readNumber(fp);
	int N = readNumber(fp);
	int blockCt = readNumber(fp);


	//build the bucket array from getlining the rest of the file
	for(int i = 0;i < N; i++){
		int offset = readNumber(fp);
		int overflow = readNumber(fp);

		//get block offset
		vector<int> temp;
		temp.push_back(offset);
		vect.push_back(temp);

		//get overflow offsets
		for(int j = 0;j < overflow;j++){
			int overflow_offset = readNumber(fp);
			vect.at(i).push_back(overflow_offset);
		}
	}
	fp.close();
}
void printBucketArray(vector<vector<int>> vect){
	for(int i =0; i<vect.size();i++){
		for(int j =0;j<vect.at(i).size();j++){
			cout << vect.at(i).at(j) << " ";
		}
		cout << endl;
	}
}
//make sure you grab the size of the key and check those last bits to the original search.
void buildIndex(vector<string> &vect, int N){
	int Next = 0;
	int total = 2;
	for(int i=0; i< N-2;i++){
		if(Next == total){
			Next = 0;
			total *= 2;
		}
		string curr = vect.at(Next);
		curr.insert(0, "0");
		vect.at(Next) = curr;
		curr[0] = '1';
		vect.push_back(curr);
		Next++;
	}

}

//AXE THIS
// void incrementRecordCt(){
// 	ifstream ifile;
// 	ofstream ofile;
// 	ifile.open("bucket.txt");
// 	// fstream file("bucket.txt", ios::out | ios::in);
// 	string remade_str;
// 	string str;
// 	std::getline(ifile, str);
// 	int ct = stoi(str);
// 	ct++;
// 	remade_str = std::to_string(ct);
// 	remade_str += '\n';
// 	while(std::getline(ifile, str)){
// 		remade_str = remade_str + str+'\n';
// 	}

// 	ifile.close();
// 	ofile.open("bucket.txt");

// 	int string_length = remade_str.length();
// 	char char_array[string_length+1];
//     strcpy(char_array, remade_str.c_str());
//     char_array[string_length] = 0;
//     ofile << remade_str;
// 	ofile.close();
// }
// void incrementBucketCt(){
// 	ifstream ifile;
// 	ofstream ofile;
// 	ifile.open("bucket.txt");
// 	// fstream file("bucket.txt", ios::out | ios::in);
// 	string remade_str;
// 	string str;
// 	std::getline(ifile, str);
// 	remade_str = str + '\n';
// 	std::getline(ifile, str);
// 	int ct = stoi(str);
// 	ct++;
// 	str = std::to_string(ct);
// 	remade_str += str + '\n';
// 	while(std::getline(ifile, str)){
// 		remade_str = remade_str + str+'\n';
// 	}

// 	ifile.close();
// 	ofile.open("bucket.txt");

// 	int string_length = remade_str.length();
// 	char char_array[string_length+1];
//     strcpy(char_array, remade_str.c_str());
//     char_array[string_length] = 0;
//     ofile << remade_str;
// 	ofile.close();
// }
void addBucket(){
	ifstream ifile;
	ofstream ofile;

	ifile.open("bucket.txt");
	string remade_str;
	string str;

	//grab meta data
	int emptyOverflowCt = readNumber(ifile);
	remade_str = std::to_string(emptyOverflowCt) + '\n';
	int recordCt = readNumber(ifile);
	remade_str += std::to_string(recordCt) + '\n';
	int N = readNumber(ifile);
	remade_str += std::to_string(N+1) + '\n';
	int blockCt = readNumber(ifile);
	remade_str += std::to_string(blockCt+1) + '\n';

	//find the block
	for(int i = 0;i < N; i++){
		int offset = readNumber(ifile);
		remade_str += std::to_string(offset) + '\n';
		int overflow = readNumber(ifile);
		remade_str += std::to_string(overflow) + '\n';
		for(int j = 0;j < overflow;j++){
			int overflow_offset = readNumber(ifile);
			remade_str += std::to_string(overflow_offset) + '\n';
		}
	}

	//add new block
	remade_str += std::to_string(blockCt) + '\n' + '0'+'\n';

	ifile.close();
	ofile.open("bucket.txt");

	int string_length = remade_str.length();
	char char_array[string_length+1];
    strcpy(char_array, remade_str.c_str());
    char_array[string_length] = 0;
    ofile << remade_str;
	ofile.close();
}
//remember this functinos index is not 0 index, it starts at 1
void addOverflow(int index){
	ifstream ifile;
	ofstream ofile;

	ifile.open("bucket.txt");
	string remade_str;
	string str;

	//grab meta data
	int emptyOverflowCt = readNumber(ifile);
	remade_str = std::to_string(emptyOverflowCt) + '\n';
	int recordCt = readNumber(ifile);
	remade_str += std::to_string(recordCt) + '\n';
	int N = readNumber(ifile);
	remade_str += std::to_string(N) + '\n';
	int blockCt = readNumber(ifile);
	remade_str += std::to_string(blockCt+1) + '\n';

	//find the block
	for(int i = 0;i < index; i++){
		int offset = readNumber(ifile);
		remade_str += std::to_string(offset) + '\n';
		int overflow = readNumber(ifile);
		remade_str += std::to_string(overflow+1) + '\n';
		for(int j = 0;j < overflow;j++){
			int overflow_offset = readNumber(ifile);
			remade_str += std::to_string(overflow_offset) + '\n';
		}
	}
	//add new overflow
	remade_str += std::to_string(blockCt) + '\n';

	//get rest of file
	while(std::getline(ifile, str)){
		remade_str = remade_str + str+'\n';
	}

	ifile.close();
	ofile.open("bucket.txt");

	int string_length = remade_str.length();
	char char_array[string_length+1];
    strcpy(char_array, remade_str.c_str());
    char_array[string_length] = 0;
    ofile << remade_str;
	ofile.close();
}
void removeOverflow(int index, int lostRecordCt){
	ifstream ifile;
	ofstream ofile;

	ifile.open("bucket.txt");
	string remade_str;
	string str;

	//grab meta data
	int emptyOverflowCt = readNumber(ifile);
	remade_str = std::to_string(emptyOverflowCt+1) + '\n';
	int recordCt = readNumber(ifile);
	remade_str += std::to_string(recordCt-lostRecordCt) + '\n';
	int N = readNumber(ifile);
	remade_str += std::to_string(N) + '\n';
	int blockCt = readNumber(ifile);
	remade_str += std::to_string(blockCt)+'\n';

	//find the block
	for(int i = 0;i < index; i++){
		int offset = readNumber(ifile);
		remade_str += std::to_string(offset) + '\n';
		int overflow = readNumber(ifile);
		remade_str += std::to_string(overflow-1) + '\n';
		for(int j = 0;j < overflow-1;j++){
			int overflow_offset = readNumber(ifile);
			remade_str += std::to_string(overflow_offset) + '\n';
		}
		//skip last block
		int overflow_offset = readNumber(ifile);
	}


	//get rest of file
	while(std::getline(ifile, str)){
		remade_str = remade_str + str+'\n';
	}

	ifile.close();
	ofile.open("bucket.txt");

	int string_length = remade_str.length();
	char char_array[string_length+1];
    strcpy(char_array, remade_str.c_str());
    char_array[string_length] = 0;
    ofile << remade_str;
	ofile.close();
}
// void removeOverflow(){

// }
// //maybe 
// //if you delete an voerflow then remake it, just add a new one, and leave the old one eempty
// //this way the pointers will never change, just what is known to the 
// //search program will guide to the right blocks
// //deleting a overflow block means you cant decrement block count or else
// void deleteOverflow(){

// }
int main(int argc, char *argv[]){
	addBlock();
	vector<string> index;
	index.push_back("0");
	index.push_back("1");
	vector<vector<int>> bucketArray;
	createBucketArray(bucketArray);
	printBucketArray(bucketArray);
	// buildIndex(index, 5);
    	buildIndex(index, 9);

    cout << "Printing index: "<< endl;
	for(int i=0;i<index.size();i++){
	

		cout << index[i] << endl;
	}
	// cout << readBlock(0);
	// incrementRecordCt();
	// incrementBucketCt();
	// addBucket();
	// addOverflow(1);
	// removeOverflow(1,1);
}
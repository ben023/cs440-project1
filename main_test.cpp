#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cstring>

using namespace std;
int num_lines = 5; // test value, change later according to csv length
int num_cols = 4; // 4 data fields


int h0_lvl = 2;
int h1_lvl = h0_lvl + 1;
class Record{
	public:
		string eid;
		string name;
		string bio;
		string mid;
		string hash_value;
        string hash_at_current_level;

		Record(string eid, string name, string bio, string mid, string hash_at_current_level, string hash_value){
			this->eid = eid;
			this->name = name;
			this->bio = bio;
			this->mid = mid;
			this->hash_value = hash_value;
			this->hash_at_current_level = hash_at_current_level;
		}
};

class MetaData{
	public:
		bool h_bool;
		bool exists;
		int line_number;
};

string getHash(int id){
	string key = bitset<32>(id).to_string();
	return key;
}
string levelKey(string fullkey, int bit_ct){
	int length = fullkey.length();
	return fullkey.substr(length-bit_ct);
}


bool CapOk(int num_rec, int N){
	if( (float) num_rec / ((float) N) * 5.0 > 0.8){
		return false;
	}
	else{
		return true;
	}
}


void addBlock(){
	ofstream fp;
	fstream file("test.txt");
	fp.open("test_output.dat", ios::binary);
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

void createBucketArray(vector<vector<int>> &vect){
	ifstream fp;
	fp.open("bucket.txt");

	int num_record = readNumber(fp);
	int N = readNumber(fp);
	// cout << "debug5";
	//build the bucket array from getlining the rest of the file
	for(int i = 0;i < N; i++){
		int offset = readNumber(fp);
		int overflow = readNumber(fp);
		// cout << "debug6";

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
// input index that you get from the index 
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


int main(int argc, char *argv[]){
	ifstream employee_csv("Employees.csv");
	string line;
	


	// parse through csv one line at a time, create record object to hold data temporarily
	// while (getline(employee_csv, line)){ uncomment later after done testing with just one line
	getline(employee_csv, line);

		addBlock();
		vector<string> index;
		index.push_back("0");
		index.push_back("1");

		vector<vector<int>> bucketArray;

		cout << "Creating bucket array: ";
		createBucketArray(bucketArray);
		cout << endl;

		cout << "Printing bucket array: ";
		printBucketArray(bucketArray);
		cout << endl;

		cout << "Building index: ";
		buildIndex(index, 2);
		cout << endl;

		// addBucket();

		// for(int i=0;i<index.size();i++){
		// 	cout << index[i] << " ";
		// }
		// cout << readBlock(0);
	
		stringstream sst(line);

		size_t st;
		string hash_value;
		string eid;
		string name, bio, mid;
		// 0. before adding a record, always check that CapOk returns true by searching through bucket array for num_Records and N (number of blocks and overflow blocks?)
		
		
		// 1. if CapOk == false:
		// split upon and "Next"? and read in all matching blocks for the bucket (including overflow), take out all records that will go in new bucket and rewrite original block
		
		// if capOk == true:
		// 1. find offset for all matching-id blocks (can't skip to last one since redistributing may make space in earlier blocks?)
		// 2. loop through all matching-id blocks until you find the first block with available space (if no space is available in any of the matching blocks, create new overflow block)
		// 3. update bucket array everytime new record is added, new block is added, and new bucket is added. Need to update index when new bucket is added?


		/* 
		how do the index and bucket functions work together? 
		how do I search for an index in the hash index and know where to look 
		for the corresponding metadata within the bucket array?

		do your addbucket and index functions update accordingly

		*/
		getline(sst, eid, ',');
		getline(sst, name, ',');
		getline(sst, bio, ',');
		getline(sst, mid, ',');

		hash_value = getHash(stol(eid, &st));
		 string last_n_bits;
		

		int found_matching_id;
		for(int i=0;i<index.size();i++){
			// cout << index[i] << " ";
			if (index.at(i) == levelKey(hash_value, index.at(i).length()) ){
				last_n_bits = levelKey(hash_value, index.at(i).length());
				cout << "found matching key" << last_n_bits <<  endl;
				// now find where corresponding key is within the bucket array, and grab all relevant metadata
			
			} 
		}

		// delimiter '#' for separating elements within a record, '$' for separating whole records within a block
		string new_record = "";
		new_record.append(eid);
		new_record.append("#");
		new_record.append(name);
		new_record.append("#");
		new_record.append(bio);
		new_record.append("#");
		new_record.append(mid);
		new_record.append("$");

		fstream s("test.txt");
		

		// find start of second test "block", each block is 40 bytes
		s.seekg(4096, ios::beg);

		// save all of second test "block", save space for null terminating and new line?
		char save_current_data[4097];
		s.read(save_current_data, 4096);
		save_current_data[4096] = 0;

		string old_data(save_current_data);

		std::string::iterator end_pos = std::remove(old_data.begin(), old_data.end(), ' ');
		old_data.erase(end_pos, old_data.end());		


		// add new record to block, old data now contains all data
		old_data.append(new_record);
		old_data.resize(4096,' '); // make sure extra room is filled with spaces
		char buffer[4096];
		snprintf(buffer, 4096, old_data.c_str()); // buffer now contains updated block
		cout << "Buffer: " << buffer << endl;

		s.seekp(4096, ios_base::beg);
		
		s.write(buffer, 4096);
		


	return 0;
}
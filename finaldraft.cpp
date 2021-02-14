  
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <cstring>



#include <sstream>
#include <algorithm>


using namespace std;

int h_lvl = 2;
int h1_lvl = h_lvl + 1;
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



int readNumber(ifstream &fp){
	string str;
	std::getline(fp, str);
	return stoi(str);
}
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
		
int getN(int value){
	ifstream ifile;
	ifile.open("bucket.txt");
	int emptyOverflowCt = readNumber(ifile);
	int recordCt = readNumber(ifile);
	int N = readNumber(ifile);
	int blockCt = readNumber(ifile);
	if (value == 0){
		return recordCt;
	} else if (value == 1){
		return N;
	} else if (value == 2){
		return blockCt;
	}
}
bool CapOk(int recordsCt, int blockCt){
	if( (float) recordsCt / (float) (blockCt) * 5.0 > 0.8){
		return false;
	}
	else{
		return true;
	}
}


void addBlock(int num_blocks, int offset){
	ofstream fp;
	fstream file("test.txt");
	fp.open("test.dat", ios::binary);
    // string temp("");
    // string temp2("Test2");
	string temp("");
    // string temp2("");
    temp.resize(4096*offset, ' ');
    // temp2.resize(4096, ' ');
    // char char_array[(4096*2)+1];
	char char_array[(4096*num_blocks)+1];

    // char char_array2[4097];
    strcpy(char_array, temp.c_str());
    // strcpy(char_array2, temp2.c_str());
	// file.seekp(4096*0);
	file.write(char_array, 4096*num_blocks);
	// file.write(char_array2, 4096);
	// fp << temp;
	// fp << temp2;
	fp.close();
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
    // cout << "block: " << str << endl;
    return str;
}
void createBucketArray(vector<vector<int>> &vect){
	ifstream fp;
	fp.open("bucket.txt");
	vect.clear();
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
		// cout << "working" << endl;

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
void buildIndex(vector<string> &vect){
	vect.clear();
	vect.push_back("0");
	vect.push_back("1");
	int N = getN(1);
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
	for(int i = -1;i < index; i++){
		int offset = readNumber(ifile);
		remade_str += std::to_string(offset) + '\n';
		int overflow = readNumber(ifile);
		if(i == index-1){
			remade_str += std::to_string(overflow+1) + '\n';
		}
		else{
			remade_str += std::to_string(overflow) + '\n';
		}
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
void removeOverflow(int index){
	cout << "debugging3" << endl;
	ifstream ifile;
	ofstream ofile;

	ifile.open("bucket.txt");
	string remade_str;
	string str;

	//grab meta data
	int emptyOverflowCt = readNumber(ifile);
	remade_str = std::to_string(emptyOverflowCt+1) + '\n';
	int recordCt = readNumber(ifile);
	remade_str += std::to_string(recordCt) + '\n';
	int N = readNumber(ifile);
	remade_str += std::to_string(N) + '\n';
	int blockCt = readNumber(ifile);
	remade_str += std::to_string(blockCt)+'\n';

	//find the block
	for(int i = -1;i < index; i++){
		int offset = readNumber(ifile);
		remade_str += std::to_string(offset) + '\n';
		int overflow = readNumber(ifile);
		if(i == index-1){
			remade_str += std::to_string(overflow-1) + '\n';
		}
		else{
			remade_str += std::to_string(overflow) + '\n';
		}
		remade_str += std::to_string(overflow) + '\n';
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

void incrementRecordCt(){
	ifstream ifile;
	ofstream ofile;

	ifile.open("bucket.txt");	
	string remade_str;
	string str;

	//grab meta data
	int emptyOverflowCt = readNumber(ifile);
	remade_str = std::to_string(emptyOverflowCt) + '\n';
	int recordCt = readNumber(ifile);
	remade_str += std::to_string(recordCt+1) + '\n';
	int N = readNumber(ifile);
	remade_str += std::to_string(N) + '\n';
	int blockCt = readNumber(ifile);
	remade_str += std::to_string(blockCt) + '\n';
	
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

int countRecords(string block) {
  int counter = 0;

  for (int i = 0; i < block.size(); i++)
    if (block[i] == '#') counter++;

  return counter;
}

int getNextIndex(vector<string> vect){
	int N = getN(1);
	int log = std::log(N)/std::log(2);
	// cout << "this is log" << log << endl;
	//means we have finished a cycle
	if (ceil(log2(N)) == floor(log2(N))){
		return 0;
	}
	else{
		int level = vect.at(0).size();
		for(int i = 0;i< log *2; i++){
			if(vect.at(i).size() == level-1){
				return i;
			}
		}
	}

}

int main(int argc, char *argv[]){
	addBlock(2, 0);

	//base index
	vector<string> index;
	index.push_back("0");
	index.push_back("1");

	//grab the entire strcuture of the current hash table
	vector<vector<int>> bucketArray;
	createBucketArray(bucketArray);
	// printBucketArray(bucketArray);

	//grab how many buckets we currently have in bucketArray to build
	// N sized index with binary keys eg. '00,01,10,11'
	// int N = getN(1);
	buildIndex(index);
    // addBlock();

    ifstream employee_csv("Employees.csv");
	string line;
	// while (getline(employee_csv, line)){ //uncomment later after done testing with just one line
  // and add } at end
		for (int k=0; k<5; k++){
			getline(employee_csv,line); // remove when uncommenting the while loop
		stringstream sst(line);
		fstream s("test.txt");
		
		size_t st;
		string hash_value;
		string eid, name, bio, mid;
		string field;
		
        // get record data from one row of csv
		getline(sst, eid, ',');
		getline(sst, name, ',');
		getline(sst, bio, ',');
		getline(sst, mid, ',');

		// cout << "Testing all fields: " << eid << name << bio << mid << endl;
		// mid.erase(s.find('\r');
		
		hash_value = getHash(stoi(eid, &st));
		string last_n_bits;
		int found_index;
		int found_matching_id;
		for(int i=0;i<index.size();i++){
			// cout << index[i] << " ";
			if (index.at(i) == levelKey(hash_value, index.at(i).length()) ){
				last_n_bits = levelKey(hash_value, index.at(i).length());
				// cout << "found matching key" << last_n_bits <<  endl;
				// cout << "at index i = " << i << endl;
                found_index = i;
                // now find grab all relevant metadata from bucket vector at index i
			
			} 
		}
		;
	
	
		// delimiter '#' for separating elements within a record, '$' for separating whole records within a block
		// string new_record = "";
		string new_record = eid + "#" + name + "#" + bio + "#" + mid + "$";

		// cout << "new_record: " << new_record << endl;
        int last_block_offset; 
        // find offset for last block in bucket array at matching index (it will be the block we add 
        // the new record into, assuming each block is filled before going to next block)
        for (int i=0; i<bucketArray.at(found_index).size(); i++){
            // cout << "Bucket array offsets: " << bucketArray.at(found_index).at(i) << endl;

        }

        last_block_offset = bucketArray.at(found_index).at((bucketArray.at(found_index).size()-1));
		// cout << "Last block offset: " << last_block_offset << endl;
        // read in block and check if the block is full or not (5 records)
        string old_block = readBlock(last_block_offset);
		
        // cout << "block: " << old_block << endl;
		int counter = countRecords(old_block);
		// cout << "count of records in block: " << counter << endl;
		

		//testing add bucket:
		// addbucket();
		// createBucketArray();


		if (counter < 5){
			// cout << "Add record here, room available" << endl;
			// cout << "Old block: " << old_block << endl;

			// code to strip white spaces from end used from:
			// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			size_t endpos = old_block.find_last_not_of(" \t");
			size_t startpos = old_block.find_first_not_of(" \t");
			if( std::string::npos != endpos )
			{
				old_block = old_block.substr( 0, endpos+1 );
				old_block = old_block.substr( startpos );
			}
			else {
				old_block.erase(std::remove(std::begin(old_block), std::end(old_block), ' '), std::end(old_block));
			}

			// std::string::iterator end_pos = std::remove(old_block.begin(), old_block.end(), ' ');
			// old_block.erase(end_pos, old_block.end());		
			// cout << "Old block: " << old_block << endl;
			// append new record to block, old data now contains all data
			old_block.append(new_record);
			old_block.resize(4096,' '); // make sure extra room is filled with spaces
		
			// cout << "old_block updated: " << old_block << endl;


			// add new record to block, old data now contains all data
			char buffer[4097];
			// snprintf(buffer, 4096, old_block.c_str()); // buffer now contains updated block
			strcpy(buffer, old_block.c_str());
			// cout << "Buffer: " << buffer << endl;
			s.seekp(4096*last_block_offset, ios_base::beg);
			s.write(buffer, 4096);
			
			incrementRecordCt();
			//call increment record count function here to update bucket.txt

		} 
		else {
			// no room in last block, must add new overflow block
			addOverflow(found_index);
			createBucketArray(bucketArray);

			// update last block offset with new offset of new overflow block
			last_block_offset = bucketArray.at(found_index).at((bucketArray.at(found_index).size()-1));
			new_record.resize(4096, ' ');
			char buffer[4097];
			strcpy(buffer, new_record.c_str());
			s.seekp(4096*last_block_offset, ios_base::beg);
			s.write(buffer, 4096);

			incrementRecordCt();
			//call increment record count function here to update bucket.txt
		}

		if (!CapOk(getN(0), getN(2))){
			// cap exceeded, must split and redistribute
			
			int splitting_index = getNextIndex(index);
			
			addBucket();
			createBucketArray(bucketArray);
			buildIndex(index);
			
			int added_index_from_split = index.size()-1;
			int offset = bucketArray.at(added_index_from_split).at(0);
			// cout << "Offset: "<< offset << endl;
			// // addBlock(1, offset);

			string new_block = "";
			new_block.resize(4096, ' ');
			char buffer[4097];
			strcpy(buffer, new_block.c_str());
			s.seekp(4096*offset, ios_base::beg);
			s.write(buffer, 4096);


			vector <string> save_all_blocks;

			for (int i=0; i<bucketArray.at(splitting_index).size(); i++){
				string blocks = readBlock(bucketArray.at(splitting_index).at(i));
				save_all_blocks.push_back(blocks);
        	}



			// calculate new key for split bucket
			string key_after_split = index.at(splitting_index);
			int key_after_split_size = index.at(splitting_index).length();
			// use to access bucket array and find offset for start of first block within new bucket
			

			vector <string> keep_records;
			vector <string> move_records;
			for (int i=0; i<save_all_blocks.size(); i++){
				stringstream ss1(save_all_blocks.at(i));
				string s;
				
				while (getline(ss1, s, '$')){
					string eid_temp;
					cout << "S: " << s << endl;
					if (getline(ss1,eid_temp,'#')){
						stringstream convert_eid(eid_temp); // use stringstream to convert stringtype eid_temp from record to int type
						int final_eid = 0;
						convert_eid >> final_eid;
						string hash_temp = getHash(final_eid);
						if (levelKey(hash_temp,key_after_split_size) == key_after_split){
							s.append("$");
							// cout << "pushing to keep records: " << s << endl;
							keep_records.push_back(s);
						} else {
							s.append("$");
							// cout << "pushing to move records: " << s << endl;

							move_records.push_back(s);
						}
					}
				}

			}
			cout << "KEEP RECORDS: " << endl;
			for (int i =0; i<keep_records.size(); i++){
				cout << keep_records.at(i) << endl;
			}
			cout << "MOVE RECORDS: " << endl;

			for (int i =0; i<move_records.size(); i++){
				cout << move_records.at(i) << endl;

			}


			// gathered all records and organized into move and keep, now rewrite blocks accordingly
			// first go through all block offsets for original bucket
			int number_keep_records = keep_records.size();
			int number_move_records = move_records.size();
			int count_down1 = number_keep_records;
			int count_down2 = number_move_records;

			vector<string> all_keep_blocks;
			vector<string> all_move_blocks;
			cout << "Num keep records: " << number_keep_records << "number move records: " << number_move_records << endl;

			int num_keep_blocks = number_keep_records / 5 + (number_keep_records % 5 != 0);
			int num_move_blocks = number_move_records / 5 + (number_move_records % 5 != 0);

			cout << "Num move blocks: " << number_move_records << endl;
			// build up new keep block 
			for (int i=0; i<num_keep_blocks; i++){
				all_keep_blocks.push_back("");
				for (int j=0; j<5; j++){
					if (count_down1 == 0){
						break;
					} else {
						all_keep_blocks.at(i) = all_keep_blocks.at(i) + keep_records.back();
						keep_records.pop_back();
						count_down1--;
					}
				}
				cout << "Keep block: " << all_keep_blocks.at(i) << endl;
			}
			
			// //debug forloop
			// cout << "Num keep blocks: " << num_keep_blocks << "Num move blocks: " << num_move_blocks << endl;

			// loop through all original blocks and start to overwrite
			for (int i=0; i<bucketArray.at(splitting_index).size(); i++){
				int offset = bucketArray.at(splitting_index).at(i);
				if (i >= all_keep_blocks.size()){
					// rest of blocks need to be removed now
					cout << "debugging1" << endl;
					removeOverflow(splitting_index);
					cout << "debugging3" << endl;

				} else {
					cout << "debugging2" << endl;

					// blocks to be overwritten
					string updated_block = "";
					updated_block.append(all_keep_blocks.at(i));
					updated_block.resize(4096, ' ');
					char buffer[4097];
					strcpy(buffer, updated_block.c_str());
					s.seekp(4096*offset, ios_base::beg);
					s.write(buffer, 4096);
				}
			}
		
			// for (int i=0; i<num_move_blocks; i++){
			// 	all_move_blocks.push_back("");
			// 	for (int j=0; j<5; j++){
			// 		if (count_down2 == 0){
			// 			break;
			// 		} else {
			// 			all_move_blocks.at(i) = all_move_blocks.at(i) + move_records.back();
			// 			move_records.pop_back();
			// 			count_down2--;
			// 		}
			// 	}
			// 	// cout << "Keep block: " << all_move_blocks.at(i) << endl;
			// }

			
			// // now loop through new indexed bucket and create blocks from the movers
			// for (int i=0; i<num_move_blocks; i++){
			// 	if (i==0){
			// 		// working on primary block, find offset from bucketarray
			// 		int offset = bucketArray.at(added_index_from_split).at(0);
			// 		string updated_block = "";
			// 		updated_block.append(all_move_blocks.at(i));
			// 		updated_block.resize(4096, ' ');
			// 		char buffer[4097];
			// 		strcpy(buffer, updated_block.c_str());
			// 		s.seekp(4096*offset, ios_base::beg);
			// 		s.write(buffer, 4096);
			// 	} else {
			// 		// need to add overflow block
			// 		addOverflow(added_index_from_split);
			// 		createBucketArray(bucketArray);
			// 		int offset_after_adding =  bucketArray.at(added_index_from_split).at((bucketArray.at(added_index_from_split).size()-1));
			// 		string updated_block = "";
			// 		updated_block.append(all_move_blocks.at(i));
			// 		updated_block.resize(4096, ' ');
			// 		char buffer[4097];
			// 		strcpy(buffer, updated_block.c_str());
			// 		s.seekp(4096*offset_after_adding, ios_base::beg);
			// 		s.write(buffer, 4096);
			// 	}
				
			// }


//TRACK HOW MANY BLOCKS THERE WERE ORIGINALLY, THEN COMPARE TO NUMBER OF BLOCKS NOW, CALL REMOVEOVERFLOWBLOCK AS NEEDED

			// cout << "number keep:" << number_keep_records << endl;
			// cout << "number move: " << number_move_records << endl;
			// for (int i=0; i<bucketArray.at(splitting_index).size(); i++){
			// 	string updated_block = "";
			// 	updated_block.append
			// 	updated_block.resize(4096, ' ');
			// 	char buffer[4097];
			// 	strcpy(buffer, updated_block.c_str());
			// 	s.seekp(4096*offset, ios_base::beg);
			// 	s.write(buffer, 4096);

			// }
		
		
		
		}

		
		}

		// check overall load capacity at end of each add iteration and handle splitting here:






    //CALL CREATE BUCKET ARRAY EVERYITME U CALL ADDBUCKET TO UPDATE BUCKETARRAY VECTOR
}
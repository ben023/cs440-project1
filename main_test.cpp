#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <algorithm>


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

class Block{
	// String size = 5;
	public:
		string block_key0;
		string block_key1 = NULL;
		vector <Record> primary;
		vector <Record> overflow;
};

class Buckets{
	public:
		Block* block;
		string block_key0;
		//remember to change this
		string block_key1 = NULL;
		int h_level;
		bool h_plus = false;

};

class LinearHash{
	public:
		int num_entries = 0;
		int num_buckets = 0;
		int bucket_cap = 5;
		int N = 4;
		int Next = 0;

		vector <Buckets> buckets;

		//pass an int id to get a string in bit format
		// string getHash(string id){
		// 	string key = bitset<32>(id).to_string();
		// 	return key;
		// }
		//give a full id and how many end bits you want 
		// string levelKey(string fullkey, int bit_ct){
		// 	int length = fullkey.length();
		// 	return fullkey.substr(length-bit_ct);
		// }
		// void split(){
		// 	//addBucket based off next
		// 	addBucket();
		// 				// redistribute
		// 	Next++;
		// 	if(Next > N){
		// 		Next = 0;
		// 		N *= 2;
		// 		h_lvl++;
		// 	}

		// }

		// void addBucket(){
		// 	key = buckets[i].block_key0;
		// 	buckets[i].block_key1 = "0" + key;
		// 	buckets.push_back(Buckets());
		// 	num_buckets++;
		// 	int size = buckets.size();
		// 	buckets[size-1].block_key0 = key;
		// 	buckets[size-1].block_key1 = "1" + key;			
		// }
		// void newLevel(){
		// 	for (int i =0; i <= num_buckets; i++){
		// 		buckets[i].h_plus = false;
		// 	}
		// }


        // void addRecord(string eid, string name, string bio, string mid, string hash_value){
            
        //     string hash_at_current_level;
        //     bool bucket_exists = false;
        //     for (int i=0; i<buckets.size(); i++){
        //         // check if matching bucket already exists

        //         // check for matching keys, first by checking to key0
        //         if (levelKey(hash_value, h_lvl) == buckets.at(i).block_key0){
        //             // then check if block is at next level, and if true must find matching key1
        //             if (buckets.at(i).h_plus == false || (buckets.at(i).h_plus == true && buckets.at(i).block_key1 == levelKey(hash_value, h1_lvl)) ){
        //                 hash_at_current_level = levelKey(hash_value, h1_lvl);

        //                 // put record in overflow if primary is full
        //                 if (buckets.at(i).block->primary.size() > 4){
        //                     buckets.at(i).block->overflow.push_back(Record(eid,name,bio,mid,hash_at_current_level, hash_value));
        //                     break;
        //                 } 
        //                 // otherwise put record in primary
        //                 else{
        //                     buckets.at(i).block->primary.push_back(Record(eid,name,bio,mid,hash_at_current_level, hash_value));
        //                     break;
        //                 }
        //                 bucket_exists = true;
        //             }
        //         }
        //     }
        //     if (bucket_exists == false){
        //         // call add bucket, then push_back record to primary
        //     }
        // }

        // void redistribute(int next){
        //     string hash_at_current_level;
        //     if (buckets.at(next).h_plus == false){
        //         hash_at_current_level = buckets.at(next).block->block_key0;
        //     } else {
        //         hash_at_current_level = buckets.at(next).block->block_key1;
        //     }
        //     for (int i=0; i<buckets.at(next).block->primary.size(); i++){
        //         if (levelKey(buckets.at(next).block->primary.at(i).hash_value, h1_lvl) == hash_at_current_level){

        //         }
        //     }
        // }
        
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

void gatherMetaData(string last_n_bits, MetaData metadata){ // use for later
	metadata.h_bool = true;
	metadata.exists = true;
	metadata.line_number = 0;
}



int main(int argc, char *argv[]){
	LinearHash temp;
	ifstream employee_csv("Employees.csv");
	string line;

	// parse through csv one line at a time, create record object to hold data temporarily
	// while (getline(employee_csv, line)){ uncomment later after done testing with just one line
	getline(employee_csv, line);
		stringstream sst(line);
		size_t st;
		string hash_value;
		string eid;
		string name, bio, mid;
		getline(sst, eid, ',');
		getline(sst, name, ',');
		getline(sst, bio, ',');
		getline(sst, mid, ',');

		hash_value = getHash(stoi(eid, &st));
		string last_n_bits = levelKey(hash_value, h0_lvl);

		MetaData metadata;

		gatherMetaData(last_n_bits, metadata); // store all relevant metadata to add record correctly for later
		if (metadata.exists){
			
			fstream s("test.txt");
			

			// find start of second test "block", each block is 40 bytes
			s.seekg(40, ios::beg);

			// save all of second test "block", save space for null terminating and new line?
			char save_current_data[41];
			s.read(save_current_data, 40);
			save_current_data[40] = 0;
			// string converted_data(save_current_data);
			cout << "Converted_data: " << save_current_data << endl;
			string old_data(save_current_data);
			
			string new_record_data = "";	
			new_record_data.append("12321"); // want second block to add this string at end


			// cout << "original with spaces: " << old_data << "debug1" << endl;
			// remove spaces from end of block to then append new record
			std::string::iterator end_pos = std::remove(old_data.begin(), old_data.end(), ' ');
			old_data.erase(end_pos, old_data.end());		
			// cout << "original without spaces " << old_data << "debug2" << endl;
			

			// add new record to block, old data now contains all data
			old_data.append(new_record_data);
			old_data.resize(40,' '); // make sure extra room is filled with spaces
			char buffer[41];
			snprintf(buffer, 41, old_data.c_str()); // buffer now contains updated block
			cout << "Buffer: " << buffer << endl;
			// new_record_data.append(eid);
			// new_record_data.append(name);
			// new_record_data.append(bio);
			// new_record_data.append(mid);

			s.seekp(40, ios_base::beg);
			
			s.write(buffer, 40);
		}


	// }

	//creation mode
	// if (argv[1] == "C"){
	// 	FILE * pFile;
	// 	  pFile = fopen ("Employees.csv","r");
	// 	  if (pFile!=NULL)
	// 	  {
	// 	    while(fscanf(pFile))
	// 	  }
	// }
	// //search mode
	// else if(argv[1] == 'L'){
	// 	target_id = stoi(argv[2])

	// }
	return 0;
}
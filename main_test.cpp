#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <bitset>

using namespace std;

int h_lvl = 2;
int h1_lvl = h_lvl + 1;
class Record{
	public:
		float eid;
		string name;
		string bio;
		float mid;
		string hash_value;
        string hash_at_current_level;

		Record(float eid, string name, string bio, float mid, string hash_at_current_level, string hash_value){
			this->eid = eid;
			this->name = name;
			this->bio = bio;
			this->mid = mid;
			this->hash_value = hash_value;
			this->hash_at_current_level = hash_at_current_level;
		}
};

class Block{
	// float size = 5;
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
		string getHash(int id){
			string key = bitset<32>(id).to_string();
			return key;
		}
		//give a full id and how many end bits you want 
		string levelKey(string fullkey, int bit_ct){
			int length = fullkey.length();
			return fullkey.substr(length-bit_ct);
		}
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
		void newLevel(){
			for (int i =0; i <= num_buckets; i++){
				buckets[i].h_plus = false;
			}
		}


        void addRecord(float eid, string name, string bio, float mid, string hash_value){
            
            string hash_at_current_level;
            bool bucket_exists = false;
            for (int i=0; i<buckets.size(); i++){
                // check if matching bucket already exists

                // check for matching keys, first by checking to key0
                if (levelKey(hash_value, h_lvl) == buckets.at(i).block_key0){
                    // then check if block is at next level, and if true must find matching key1
                    if (buckets.at(i).h_plus == false || (buckets.at(i).h_plus == true && buckets.at(i).block_key1 == levelKey(hash_value, h1_lvl)) ){
                        hash_at_current_level = levelKey(hash_value, h1_lvl);

                        // put record in overflow if primary is full
                        if (buckets.at(i).block->primary.size() > 4){
                            buckets.at(i).block->overflow.push_back(Record(eid,name,bio,mid,hash_at_current_level, hash_value));
                            break;
                        } 
                        // otherwise put record in primary
                        else{
                            buckets.at(i).block->primary.push_back(Record(eid,name,bio,mid,hash_at_current_level, hash_value));
                            break;
                        }
                        bucket_exists = true;
                    }
                }
            }
            if (bucket_exists == false){
                // call add bucket, then push_back record to primary
            }
        }

        void redistribute(int next){
            string hash_at_current_level;
            if (buckets.at(next).h_plus == false){
                hash_at_current_level = buckets.at(next).block->block_key0;
            } else {
                hash_at_current_level = buckets.at(next).block->block_key1;
            }
            for (int i=0; i<buckets.at(next).block->primary.size(); i++){
                if (levelKey(buckets.at(next).block->primary.at(i).hash_value, h1_lvl) == hash_at_current_level){

                }
            }
        }
        
};



int main(int argc, char *argv[]){
	LinearHash temp;
	// temp.buckets.push_back(Buckets());
	// temp.buckets[0].block_key0 = "00";
	// temp.buckets.push_back(Buckets());
	// temp.buckets[1].block_key0 = "01";
	// temp.buckets.push_back(Buckets());
	// temp.buckets[2].block_key0 = "10";
	// temp.buckets.push_back(Buckets());
	// temp.buckets[3].block_key0 = "11";
	// temp.addBucket();
	//testing getHas
	// string key = temp.getHash(100);
	// cout << key<< endl;
	// cout << key[0] << endl;

	//testing levleKey
	// cout << temp.levelKey(key, 4);


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
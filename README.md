# Hashing-C-language
This project is creating hash tables according to the hash algorithm of text files in the same folder and create new text files in the same folder.
How it works?
1-)A hash value is calculated according to the content of the text file.
for(i=0;i<j;i++){
		key=key+(icerik[i] * pow(r,j-i-1));
	};//r= 31, j =len(content)
2-)Mod operation is performed with the key value and the length of the hash table.
 	  indis=(key%len(hash_table));//  Mod operation
3-)Placed if the index value(indis) of the hash table is empty, else index value(indis) is incremented by 1 until it is empty.

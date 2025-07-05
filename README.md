# Huffman-compressor

Hi there, since my university (which is public) is currently on strike and my Systems Programming course isn't very good, I decided to create a file compressor in C to practice my programming skills and learn the things the university doesn't teach me

# Design Phase

- I need to encode one or more files
    - Create a new file
    - For each file
        - Create a huffman code
        - Write in the compress file:
            - name
            - bytes
            - encoded bytes
            - Huffman code
- I need to decode all the files
    - Read compress file until EOF
    - Get line: Name
    - Get long long: Size
    - Get short: encoded bytes
    - Read Huffman Code
    - For size of file
        - Create file
            - Read encoded bytes and write decoded information
        - Close file

- solution to save file: just save the amount of bytes in the file

# Process of implementation

1. hc_endoce_file
    1. create priority_queue 
    2. erase priority_queue
    3. huffman tree
        1. pop priority_queue
        2. build code matrix
    



# Promises:

## About compress file

Each huffman code in the file begins at a different byte than were the previous one ends


## About huffman tree

- The left child is 0 and the most likely of appearing

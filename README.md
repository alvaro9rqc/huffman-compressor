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

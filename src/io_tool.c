#include "io_tool.h"
#include "huffman.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096 // can be parameterized

/*
 * 1 if is internal node, 0 if leaf
 * there is a byte after the indicator
 * always return new position in buffer
 */
int io_write_in_orden(Node *node, unsigned char *buffer, int index) {
  if (node->is_leaf) {
    buffer[index++] = 0; // Indicate leaf
    buffer[index++] = node->byte;
  } else {
    buffer[index++] = 1; // Indicate internal node
    index = io_write_in_orden(node->left, buffer, index);
    index = io_write_in_orden(node->right, buffer, index);
  }
  return index;
}

/*
 * Use BUFFER_SIZE to write the huffman tree
 */
int io_write_huffman_tree(FILE *wfile, Node *root) {
  int status = 0;
  if (root == NULL)
    return -1;
  // Write the tree in-order
  // Precondition: each node has two children or is a leaf
  unsigned char buffer[BUFFER_SIZE];
  int size = io_write_in_orden(root, buffer, 0);
  fwrite(buffer, sizeof(unsigned char), size, wfile);
  return status;
}

/*
 * NOTE: I am using 'long long' to save the file size, take care with capacity
 */
[[nodiscard]]
int io_write_huffman_code(FILE *wfile, unsigned char **huff_code,
                          char *file_name) {
  FILE *rfile = fopen(file_name, "rb");
  if (rfile == NULL) {
    fprintf(stderr, "No se pudo abrir el archivo: %s\n", file_name);
    return -1;
  }
  // save file size
  off_t file_size;
  fseeko(rfile, 0, SEEK_END);
  file_size = ftello(rfile);
  fseeko(rfile, 0, SEEK_SET);
  // Write file size
  if (fwrite(&file_size, sizeof(off_t), 1, wfile) < 1) {
    fprintf(stderr, "Error writing file size to file.\n");
    fclose(rfile);
    return -1;
  }
  unsigned char rbuff[BUFFER_SIZE];
  size_t r_s = 0; // read bytes
  unsigned char wbuff[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; ++i)
    wbuff[i] = 0;
  size_t w_lim = BUFFER_SIZE * 8;
  size_t w_idx = 0; // new position in write buffer
  int bit = 0;
  while ((r_s = fread(rbuff, 1, BUFFER_SIZE, rfile)) > 0) {
    for (size_t i = 0; i < r_s; ++i) {
      unsigned char c = rbuff[i];
      // if has space
      if (huff_code[c][0] <= w_lim - w_idx) {
        for (size_t j = 0; j < huff_code[c][0]; ++j) {
          // locate byte and write bit
          size_t ofs = w_idx % 8;
          bit = (huff_code[c][j / 8 + 1] & 1 << (7 - (j % 8))) ? 1 : 0;
          wbuff[w_idx / 8] |= bit << (7 - w_idx % 8);
          ++w_idx;
        }
      } else {
        // write buffer
        size_t w_size = w_idx / 8;
        if (fwrite(wbuff, sizeof(unsigned char), w_size, wfile) < w_size) {
          fprintf(stderr, "Error writing huffman code to file.\n");
          fclose(rfile);
          return -1;
        }
        // save the rest of the bits
        if (w_idx % 8 != 0)
          wbuff[0] = w_idx / 8;
        for (size_t i = (w_idx % 8) ? 1 : 0; i < BUFFER_SIZE; ++i)
          wbuff[i] = 0;
        w_idx %= 8; // reset index to write
        // write last char
        for (size_t j = 0; j < huff_code[c][0]; ++j) {
          // locate byte and write bit
          wbuff[w_idx / 8] |= 1 << (7 - (w_idx % 8));
          ++w_idx;
        }
      }
    }
  }
  // Write the remaining bits in the buffer
  if (w_idx > 0) {
    size_t w_size = w_idx / 8 + (w_idx % 8 ? 1 : 0);
    if (fwrite(wbuff, sizeof(unsigned char), w_size, wfile) < w_size) {
      fprintf(stderr, "Error writing remaining bits to file.\n");
      fclose(rfile);
      return -1;
    }
  }
  // close file
  fclose(rfile);
  return 0;
}

int io_create_directories(const char *path) {
  char temp[256];
  strncpy(temp, path, sizeof(temp) - 1);
  temp[sizeof(temp) - 1] = '\0';

  for (char *p = temp + 1; *p; p++) {
    if (*p == '/') {
      *p = '\0';
      if (access(temp, F_OK) != 0) {
        if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
          perror("mkdir");
          return -1;
        }
      }
      *p = '/';
    }
  }

  return 0;
}

/*
 *
 *
 *
 *
 *
 * Public functions
 *
 *
 *
 *
 *
 * */

double io_read_bytes(Node *pq, char *file_name) {
  FILE *file = fopen(file_name, "rb");
  if (file == NULL) {
    fprintf(stderr, "No se pudo leer el archivo: %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  unsigned char buffer[BUFFER_SIZE];
  double total_bytes = 0;
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    total_bytes += bytes_read;
    for (size_t i = 0; i < bytes_read; ++i) {
      ++pq[buffer[i]].frequency;
    }
  }
  fclose(file);
  return total_bytes;
}

/*
 * Requires
 * - File descriptor
 * - filename
 * - huffman code
 * - huffman tree
 * Do:
 * 1. Write name
 * 2. Write tree
 * 3. Save position to save file size with long long
 * 4. Write file
 * */
[[nodiscard("Handling error")]]
int io_save_code(FILE *file, char *filename, unsigned char **huff_code,
                 Node *root) {
  // Write name
  int status = 0;
  status = fwrite(filename, sizeof(char), strlen(filename) + 1, file);
  if (status < 0) {
    fprintf(stderr, "Error writing filename: %s\n", filename);
    return -1;
  }
  // Write tree
  io_write_huffman_tree(file, root);
  status = io_write_huffman_code(file, huff_code, filename);
  // handle error
  if (status < 0) {
    fprintf(stderr, "Error writing huffman code for file: %s\n", filename);
    return -1;
  }
  return status;
}

int io_read_filename(FILE *file, char *filename) {
  int n = 0;
  char c;
  while (n < 255) {
    c = fgetc(file);
    if (c == EOF) {
      if (n == 0) {
        fprintf(stderr, "Error reading filename: unexpected end of file.\n");
        return -1; // Error: no filename read
      }
      break; // End of filename
    }
    if (c == '\0')
      break; // Null character indicates end of filename
    filename[n++] = c;
  }
  if (n == 255) {
    fprintf(stderr, "No se encontró el caracter nulo\n");
    return -1;
  }
  return n;
}

Node *io_read_huffman_tree(FILE *file) {
  char is_internal = fgetc(file);
  if (is_internal == EOF) {
    fprintf(stderr, "Error reading huffman tree: unexpected end of file.\n");
    return NULL;
  }
  Node *root = (Node *)calloc(1, sizeof(Node));
  if (is_internal == 0) {
    char c = fgetc(file);
    if (c == EOF) {
      fprintf(stderr, "Error reading huffman tree: unexpected end of file.\n");
      free(root);
      return NULL;
    }
    root->byte = c;
    root->is_leaf = 1;
  } else if (is_internal == 1) {
    root->left = io_read_huffman_tree(file);
    root->right = io_read_huffman_tree(file);
    if (root->left == NULL || root->right == NULL) {
      fprintf(stderr, "Error reading huffman tree: incomplete tree.\n");
      free(root);
      return NULL;
    }
  } else {
    fprintf(stderr, "Error reading huffman tree: invalid node type.\n");
    free(root);
    return NULL;
  }
  return root;
}

off_t io_read_file_size(FILE *file) {
  off_t file_size;
  if (fread(&file_size, sizeof(off_t), 1, file) < 1) {
    fprintf(stderr, "Error reading file size.\n");
    return -1;
  }
  return file_size;
}

int io_write_decompress_file(FILE *wfile, FILE *rfile, Node *root,
                             off_t file_size) {
  // Create read buffer
  char read_buffer[BUFFER_SIZE];
  size_t bytes_read;
  // Create write buffer
  char write_buffer[BUFFER_SIZE];
  size_t write_index = 0; // Index in write buffer
  // Parameters
  off_t dec_bytes = 0;  // Decompressed bytes
  Node *current = root; // Current node in the huffman tree
  off_t offset = 0;     // Offset in the file
  // Read from file
  while ((bytes_read = fread(read_buffer, 1, BUFFER_SIZE, rfile)) > 0) {
    for (int i = 0; i < bytes_read << 3; ++i) { // For each bit
      // Tree transition
      if ((read_buffer[i >> 3] & (1 << (7 - (i % 8))))) {
        current = current->right; // Go right
      } else {
        current = current->left; // Go left
      }
      if (current->is_leaf) {
        // Add byte to write buffer
        write_buffer[write_index++] = current->byte;
        current = root; // Reset to root
        ++dec_bytes;
        // Write buffer to file
        if (write_index == BUFFER_SIZE) {
          if (fwrite(write_buffer, sizeof(char), write_index, wfile) <
              write_index) {
            fprintf(stderr, "Error writing decompressed data to file.\n");
            return -1;
          }
          write_index = 0; // Reset write index
        }
        // Check if we have decompressed enough bytes
        if (dec_bytes == file_size) {
          // Move the file descriptor to the end of the previous code
          offset = (i >> 3) - bytes_read + 1;
          break;
        }
      }
    }
  }
  if (dec_bytes != file_size) {
    fprintf(stderr, "Decompressed bytes do not match expected file size.\n");
    return -1; // Error: decompressed bytes do not match expected size
  }
  // Write remaining bytes in buffer
  if (write_index > 0) {
    if (fwrite(write_buffer, sizeof(char), write_index, wfile) < write_index) {
      fprintf(stderr, "Error writing remaining decompressed data to file.\n");
      return -1;
    }
  }
  printf("pos: %ld\n", ftell(rfile));
  fseek(rfile, offset, SEEK_CUR);
  printf("pos: %ld\n", ftell(rfile));
  return 0; // Decompression complete
}

FILE *io_open_unique_file(const char *filename, const char *mode) {
  char new_name[256];
  int count = 0;

  FILE *fp = NULL;

  strncpy(new_name, filename, 255);
  new_name[sizeof(new_name) - 1] = '\0';

  io_create_directories(new_name);

  while (access(new_name, F_OK) == 0) { // archivo ya existe
    count++;
    snprintf(new_name, sizeof(new_name), "%s.%d", filename, count);
  }

  fp = fopen(new_name, mode);
  if (fp == NULL) {
    fprintf(stderr, "Error opening file: %s\n", new_name);
    return NULL; // Error opening file
  }
  return fp;
}

int io_is_end_of_file(FILE *file) {
  int c = fgetc(file);
  if (c == EOF) {
    return 1; // End of file
  } else {
    ungetc(c, file); // Put back the character
    return 0;        // Not end of file
  }
}
